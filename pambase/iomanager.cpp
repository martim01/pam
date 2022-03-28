#include "iomanager.h"
#include "settingevent.h"
#include "settings.h"
#include "audioevent.h"
#include "session.h"
#include "log.h"
#include "soundcardmanager.h"
#include "generator.h"
#include "rtpthread.h"
#include <wx/log.h>
#include "rtpserverthread.h"
#include <ctime>
#include "wxptp.h"
#include "aoipsourcemanager.h"
#include "ondemandstreamer.h"
#include "ondemandaes67mediasubsession.h"
#include "sapserver.h"
#include "dnssd.h"
#include <wx/msgdlg.h>
#include "networkcontrol.h"

using namespace std;


DEFINE_EVENT_TYPE(wxEVT_SESSION)
DEFINE_EVENT_TYPE(wxEVT_PLAYBACK_CHANNELS)
DEFINE_EVENT_TYPE(wxEVT_INPUT_FAILED)

IOManager& IOManager::Get()
{
    static IOManager iom;
    return iom;
}


void IOManager::RegisterForRTCPTransmission(wxEvtHandler* pHandler)
{
    m_setRTCPHandlers.insert(pHandler);
}

void IOManager::DeregisterForRTCPTransmission(wxEvtHandler* pHandler)
{
    m_setRTCPHandlers.erase(pHandler);
}

void IOManager::RegisterForRTSPTransmission(wxEvtHandler* pHandler)
{
    m_setRTSPHandlers.insert(pHandler);
}

void IOManager::DeregisterForRTSPTransmission(wxEvtHandler* pHandler)
{
    m_setRTSPHandlers.erase(pHandler);
}

void IOManager::RegisterHandler(wxEvtHandler* pHandler)
{
    if(m_bSingleHandler)
    {
        m_setHandlers.clear();
    }
    m_setHandlers.insert(pHandler);
}

void IOManager::DeregisterHandler(wxEvtHandler* pHandler)
{
    m_setHandlers.erase(pHandler);
}

IOManager::IOManager() :
    m_bSingleHandler(true),
    m_SessionIn(session()),
    m_SessionOut(session()),
    m_nInputSource(-1),
    m_nOutputDestination(-1),
    m_nPlaybackSource(-1),
    m_bPlaybackInput(false),
    m_bMonitorOutput(false),
    m_pGenerator(nullptr),
    m_bStreamAlwaysOn(false),
    m_pAlwaysOnServer(nullptr),
    m_pOnDemandServer(nullptr),
    m_pOnDemandSubsession(nullptr),
    m_pSapServer(nullptr),
    m_pPublisher(nullptr),
    m_bQueueToStream(false),
    m_bStreamActive(false)

{

    AoipSourceManager::Get();

    Settings::Get().AddHandler(this, wxT("Input"));

    m_vRatio.resize(8);
    for(int i = 0; i < 8; i++)
    {
        m_vRatio[i] = Settings::Get().Read("Input",  wxString::Format("Ratio_%02d", i), 1.0);
    }
    CheckIfGain();

    Settings::Get().AddHandler(this, wxT("Output"));
    Settings::Get().AddHandler(this, wxT("Generator"));
    Settings::Get().AddHandler(this, wxT("Noise"));
    Settings::Get().AddHandler(this, wxT("Monitor"), wxT("Source"));
    Settings::Get().AddHandler(this, wxT("QoS"),wxT("Interval"));
    Settings::Get().AddHandler(this, "ChannelMapping");

    Settings::Get().AddHandler(this,wxT("Server"));

    Connect(wxID_ANY,wxEVT_DATA,(wxObjectEventFunction)&IOManager::OnAudioEvent);
    Connect(wxID_ANY,wxEVT_RTP_SESSION,(wxObjectEventFunction)&IOManager::OnRTPSession);
    Connect(wxID_ANY,wxEVT_RTP_SESSION_CLOSED,(wxObjectEventFunction)&IOManager::OnRTPSessionClosed);
    Connect(wxID_ANY, wxEVT_SETTING_CHANGED, (wxObjectEventFunction)&IOManager::OnSettingEvent);

    Connect(wxID_ANY,wxEVT_QOS_UPDATED,(wxObjectEventFunction)&IOManager::OnQoS);


    #ifdef PTPMONKEY
    wxPtp::Get().AddHandler(this);
    Connect(wxID_ANY, wxEVT_CLOCK_MASTER, (wxObjectEventFunction)&IOManager::OnPtpEvent);
    Connect(wxID_ANY, wxEVT_CLOCK_SLAVE, (wxObjectEventFunction)&IOManager::OnPtpEvent);
    wxPtp::Get().RunDomain(std::string(Settings::Get().Read(wxT("AoIP_Settings"), wxT("Interface"), wxT("eth0")).mb_str()),
    Settings::Get().Read(wxT("AoIP_Settings"), wxT("Domain"), 0));
    #endif // PTPMONKEY
    m_pGenerator = new Generator();
    m_pGenerator->SetSampleRate(48000);

    m_timerSilence.SetOwner(this, wxNewId());
    Connect(m_timerSilence.GetId(), wxEVT_TIMER, (wxObjectEventFunction)&IOManager::OnTimerSilence);

    m_timerResetStream.SetOwner(this, wxNewId());
    Connect(m_timerResetStream.GetId(), wxEVT_TIMER, (wxObjectEventFunction)&IOManager::OnTimerReset);

    m_timerSilence.Start(250, true);

    srand(time(NULL));

}


IOManager::~IOManager()
{
    Stop();
    Settings::Get().RemoveHandler(this);
}

void IOManager::Stop()
{
    delete m_pGenerator;
    m_pGenerator = 0;

    for(auto pairThread : m_mRtp)
    {
        pairThread.second->SetToClose();
        pairThread.second->Wait();
        delete pairThread.second;
    }
    m_mRtp.clear();
    SoundcardManager::Get().Terminate();

    StopStream();
}

void IOManager::StopStream()
{
    if(m_pAlwaysOnServer)
    {
        m_pAlwaysOnServer->StopStream();
        m_pAlwaysOnServer->Wait();
        delete m_pAlwaysOnServer;
        m_pAlwaysOnServer = nullptr;
        RTPServerFinished();
    }
    else if(m_pOnDemandServer)
    {
        m_pOnDemandServer->Stop();
        m_pOnDemandServer->Wait();
        delete m_pOnDemandServer;
        m_pOnDemandServer = nullptr;
        m_pOnDemandSubsession = nullptr;
        RTPServerFinished();
    }


    m_pSapServer = nullptr;
    m_pPublisher = nullptr;
}


void IOManager::OnSettingEvent(SettingEvent& event)
{
    if(event.GetSection() == wxT("Monitor"))
    {
        OnSettingEventMonitor(event);
    }
    else if(event.GetSection() == wxT("Input"))
    {
        OnSettingEventInput(event);
    }
    else if(event.GetSection() == wxT("Output"))
    {
        OutputChanged(event.GetKey());
    }
    else if(event.GetSection() == wxT("Generator"))
    {
        GeneratorToneChanged();
    }
    else if(event.GetSection() == wxT("Noise"))
    {
        GeneratorNoiseChanged(event.GetKey());
    }
    else if(event.GetSection() == wxT("QoS"))
    {
        OnSettingEventQoS(event);
    }
    else if(event.GetSection() == wxT("Server"))
    {
        OnSettingEventServer(event);
    }
    else if(event.GetSection() == wxT("ChannelMapping"))
    {
        OnSettingEventChannelMapping(event);
    }

}

void IOManager::OnSettingEventMonitor(SettingEvent& event)
{
    if(event.GetKey() == wxT("Source") && m_bMonitorOutput != event.GetValue(false))
    {
        m_bMonitorOutput = event.GetValue(false);
        SessionChanged();
    }
}

void IOManager::OnSettingEventInput(SettingEvent& event)
{
    if(event.GetKey() == wxT("Type"))
    {
        InputTypeChanged();
    }
    else if(event.GetKey().Left(5) == "Ratio")
    {
        unsigned long nChannel;
        if(event.GetKey().AfterFirst('_').ToULong(&nChannel) && nChannel < m_vRatio.size())
        {
            m_vRatio[nChannel] = event.GetValue(1.0);
        }
        CheckIfGain();
    }
    else
    {
        InputChanged(event.GetKey());
    }
}

void IOManager::OnSettingEventQoS(SettingEvent& event)
{
    if(event.GetKey() == wxT("Interval"))
    {
        auto itThread = m_mRtp.find(m_nCurrentRtp);
        if(itThread != m_mRtp.end())
        {
            itThread->second->SetQosMeasurementIntervalMS(event.GetValue(1000l));
        }
    }
}

void IOManager::OnSettingEventServer(SettingEvent& event)
{
    if(event.GetKey() == wxT("Stream"))
    {
        m_bStreamAlwaysOn = (event.GetValue() == "AlwaysOn");
        if(!m_bStreamAlwaysOn)
        {
            if(m_pAlwaysOnServer)
            {
                m_pAlwaysOnServer->StopStream();
                m_pAlwaysOnServer->Wait();
                delete m_pAlwaysOnServer;
                m_pAlwaysOnServer = nullptr;
                RTPServerFinished();
            }
        }
        else
        {
            if(m_pOnDemandServer)
            {
                m_pOnDemandServer->Stop();
                m_pOnDemandServer->Wait();
                delete m_pOnDemandServer;
                m_pOnDemandServer = nullptr;
                m_pOnDemandSubsession = nullptr;
                RTPServerFinished();
            }
        }
        InitAudioOutputDevice();
    }
    else if(event.GetKey() == "State")
    {
        if(event.GetValue(false))
        {
            InitAudioOutputDevice();
        }
        else
        {
            StopStream();
        }
    }
    else if(event.GetKey() == wxT("SAP"))
    {
        DoSAP(event.GetValue(false));
    }
    else if(event.GetKey() == wxT("DNS-SD"))
    {
        DoDNSSD(event.GetValue(false));
    }
}

void IOManager::OnSettingEventChannelMapping(SettingEvent& event)
{
    //output session changed...
    UpdateOutputSession(true);
}

void IOManager::OnAudioEvent(AudioEvent& event)
{
    //pass on output audio
    if(event.GetCreator() == m_nPlaybackSource)
    {
        switch(m_nOutputDestination)
        {
            case AudioEvent::SOUNDCARD:
                SoundcardManager::Get().AddOutputSamples(event.GetBuffer());
                break;
            case AudioEvent::RTP:
                if(m_pAlwaysOnServer)
                {
                    m_pAlwaysOnServer->AddSamples(event.GetBuffer());
                }
                else if(m_pOnDemandSubsession)
                {
                    m_pOnDemandSubsession->AddSamples(event.GetBuffer());
                }
                break;
        }
    }
    else if(event.GetCreator() == AudioEvent::OUTPUT)
    {

        if(m_bMonitorOutput == true)
        {   //monitoring the output
           PassOnAudio(event);
        }
        if(m_nPlaybackSource != m_nInputSource)
        {
            AddOutputSamples(event.GetBuffer()->GetBufferSizePerChannel());
        }
    }


    //pass on input audio
    if(!m_bMonitorOutput && event.GetCreator() == m_nInputSource)
    {
        //Do the gain here
        if(m_bGain)
        {
            DoGain(event);
        }
        PassOnAudio(event);
    }


    delete event.GetBuffer();
}

void IOManager::AddOutputSamples(size_t nSize)
{
    if(m_pGenerator)
    {


        timedbuffer* pBuffer(m_pGenerator->Generate(nSize));
        switch(m_nOutputDestination)
        {
            case AudioEvent::SOUNDCARD:
                SoundcardManager::Get().AddOutputSamples(pBuffer);
                break;
            case AudioEvent::RTP:
                if(m_pAlwaysOnServer)
                {
                    m_pAlwaysOnServer->AddSamples(pBuffer);
                }
                else if(m_pOnDemandSubsession)
                {
                    m_pOnDemandSubsession->AddSamples(pBuffer);
                }
                break;
        }
        delete pBuffer;
    }
}

void IOManager::DoGain(AudioEvent& event)
{
    auto itSub = m_SessionIn.GetCurrentSubsession();
    if(itSub != m_SessionIn.lstSubsession.end() && itSub->nChannels != 0)
    {
        for(size_t i = 0; i < event.GetBuffer()->GetBufferSize(); i+= itSub->nChannels)
        {
            for(size_t nChannel = 0; nChannel < itSub->nChannels; nChannel++)
            {
                if(nChannel < m_vRatio.size())
                {
                    event.GetBuffer()->GetWritableBuffer()[i+nChannel] *= m_vRatio[nChannel];
                }
            }
        }
    }
}

void IOManager::PassOnAudio(AudioEvent& event)
{
    m_timerSilence.Stop();
    m_timerSilence.Start(250,true);

    for(set<wxEvtHandler*>::iterator itHandler = m_setHandlers.begin(); itHandler != m_setHandlers.end(); ++itHandler)
    {
        (*itHandler)->ProcessEvent(event);
    }

}


void IOManager::InputChanged(const wxString& sKey)
{
    if(sKey == "AoIP" || sKey == "AoIPManual")
    {
        pmlLog(pml::LOG_DEBUG) << "IOManager\tInputChanged: AoIP";

        AoIPSource source = AoipSourceManager::Get().FindSource(Settings::Get().Read(wxT("Input"), wxT("AoIP"), 0));

        if(source.nIndex != m_nCurrentRtp)
        {
            pmlLog(pml::LOG_DEBUG) << "IOManager\tAudio Input Device Changed: Close AoIP Session";

            ClearSession();
            auto itThread = m_mRtp.find(m_nCurrentRtp);
            if(itThread != m_mRtp.end())
            {
                pmlLog(pml::LOG_DEBUG) << "IOManager\tDestroy thread";
                itThread->second->SetToClose();
                itThread->second->Wait();
                itThread->second->Delete();
                m_mRtp.erase(m_nCurrentRtp);
                m_nCurrentRtp = 0;
                pmlLog(pml::LOG_DEBUG) << "IOManager\tDestroy thread: Done";

            }
            InitAudioInputDevice();
        }
    }
    else if(sKey == wxT("Device"))
    {
        InitAudioInputDevice();
    }
}

void IOManager::InputTypeChanged()
{

    //Stop the current monitoring...
    switch(m_nInputSource)
    {
        case AudioEvent::SOUNDCARD:
            OpenSoundcardDevice(SoundcardManager::Get().GetOutputSampleRate());    //this will remove the input stream
            break;
        case AudioEvent::RTP:
            auto itThread = m_mRtp.find(m_nCurrentRtp);
            if(itThread != m_mRtp.end())
            {
                pmlLog(pml::LOG_DEBUG) << "IOManager\tAudio Input Device Changed: Close AoIP";

                pmlLog(pml::LOG_DEBUG) << "IOManager\tDestroy thread";
                itThread->second->SetToClose();
                itThread->second->Wait();
                itThread->second->Delete();
                m_mRtp.erase(m_nCurrentRtp);
                m_nCurrentRtp = 0;
                pmlLog(pml::LOG_DEBUG) << "IOManager\tDestroy thread: Done";

            }
            break;
    }

    InitAudioInputDevice();
}

void IOManager::OutputDestinationChanged()
{
    //Stop the current monitoring...
    switch(m_nOutputDestination)
    {
        case AudioEvent::SOUNDCARD:
            pmlLog(pml::LOG_INFO) << "IOManager\tOutput Destination changed: was Soundcard";
            OpenSoundcardDevice(SoundcardManager::Get().GetOutputSampleRate());    //this will remove the input stream
            break;
        case AudioEvent::RTP:
            pmlLog(pml::LOG_INFO) << "IOManager\tOutput Destination changed: was AoIP";
            StopStream();


            break;
    }

    InitAudioOutputDevice();
}


void IOManager::OutputChanged(const wxString& sKey)
{
    if(sKey == wxT("Destination"))
    {
        OutputDestinationChanged();
    }
    else if(sKey == wxT("Source"))
    {
        wxString sType(Settings::Get().Read(wxT("Output"), wxT("Source"), wxT("Input")));
        m_bPlaybackInput = false;

        switch(m_nOutputDestination)
        {
            case AudioEvent::SOUNDCARD:
                SoundcardManager::Get().FlushOutputQueue();
                break;
            case AudioEvent::RTP:
                if(m_pAlwaysOnServer)
                {
                    m_pAlwaysOnServer->FlushQueue();
                }
                else if(m_pOnDemandSubsession)
                {
                    m_pOnDemandSubsession->FlushQueue();
                }
                break;
        }

        if(sType == wxT("File"))
        {
            m_nPlaybackSource = AudioEvent::FILE;
            pmlLog(pml::LOG_INFO) << "IOManager\tCreate Audio Output Generator: File";
            InitGeneratorFile();
        }
        else if(sType == wxT("Sequence"))
        {
            m_nPlaybackSource = AudioEvent::GENERATOR;
            pmlLog(pml::LOG_INFO) << "IOManager\tCreate Audio Output Generator: Sequence";
            InitGeneratorSequence();
        }
        else if(sType == wxT("Generator"))
        {
            m_nPlaybackSource = AudioEvent::GENERATOR;
            pmlLog(pml::LOG_INFO) << "IOManager\tCreate Audio Output Generator: Generator";
            InitGeneratorTone();
        }
        else  if(sType == wxT("Noise"))
        {
            m_nPlaybackSource = AudioEvent::GENERATOR;
            InitGeneratorNoise();
        }
        else if(sType == wxT("Input"))
        {
            pmlLog(pml::LOG_INFO) << "IOManager\tOutput source is input";
            m_nPlaybackSource = m_nInputSource;
            m_bPlaybackInput = true;
            m_pGenerator->Stop();
            return;     //rturn here as we dont want to genareate sampless
        }
        else
        {   //plugin
            m_nPlaybackSource = AudioEvent::GENERATOR;
            InitGeneratorPlugin(sType);
        }
        AddOutputSamples(8192);
    }
    else if(sKey == wxT("Device"))
    {
        if(m_nOutputDestination == AudioEvent::SOUNDCARD)
        {
            pmlLog(pml::LOG_INFO) << "IOManager\tSoundcard output device changed";
            OpenSoundcardDevice(SoundcardManager::Get().GetOutputSampleRate());
        }
    }
    else if(sKey == wxT("Latency"))
    {
        // @todo change the latency by closing outpt and open agin
    }
    else if(sKey == wxT("File") && Settings::Get().Read(wxT("Output"), wxT("Source"), wxT("Input")) == wxT("File"))
    {
        pmlLog(pml::LOG_INFO) << "IOManager\tChange Audio Output Generator: File";
        InitGeneratorFile();
    }
    else if(sKey == wxT("Sequence") && Settings::Get().Read(wxT("Output"), wxT("Source"), wxT("Input")) == wxT("Sequence"))
    {
        pmlLog(pml::LOG_INFO) << "IOManager\tChange Audio Output Generator: Sequence";
        InitGeneratorSequence();
    }
    else if(sKey.Left(7) == "Channel")
    {
        OutputChannelsChanged();
    }
}

void IOManager::GeneratorToneChanged()
{
    if(Settings::Get().Read(wxT("Output"), wxT("Source"), wxT("Input")) == wxT("Generator"))
    {
        if(m_pGenerator)
        {
            m_pGenerator->SetFrequency(Settings::Get().Read(wxT("Generator"), wxT("Frequency"), 1000), Settings::Get().Read(wxT("Generator"), wxT("Amplitude"), -18.0), Settings::Get().Read(wxT("Generator"), wxT("Shape"), 0));
        }
    }
}

void IOManager::GeneratorNoiseChanged(const wxString& sKey)
{
    if(sKey == wxT("Amplitude"))
    {
        m_pGenerator->SetNoiseAmplitude(Settings::Get().Read(wxT("Noise"), wxT("Amplitude"), -18.0));
    }
    else if(Settings::Get().Read(wxT("Output"), wxT("Source"), wxT("Input")) == wxT("Noise"))
    {
        pmlLog(pml::LOG_INFO) << "IOManager\tChange Audio Output Generator: Noise";
        InitGeneratorNoise();
    }
}


void IOManager::InitGeneratorFile()
{
    if(m_pGenerator && m_pGenerator->SetFile())
    {
        CreateSessionFromOutput(Settings::Get().Read(wxT("Output"), wxT("File"), wxEmptyString));
        CheckPlayback(m_pGenerator->GetSampleRate(), m_pGenerator->GetChannels());
    }
}

void IOManager::InitGeneratorSequence()
{
    if(m_pGenerator)
    {
        if(m_pGenerator->LoadSequence(Settings::Get().Read(wxT("Output"), wxT("Sequence"), wxT("glits"))))
        {
            pmlLog(pml::LOG_INFO) << "IOManager\tGenerating sequence file " << Settings::Get().Read(wxT("Output"), wxT("Sequence"), wxT("glits"));
            //m_sCurrentSequence = sSequence;

            CreateSessionFromOutput(Settings::Get().Read(wxT("Output"), wxT("Sequence"), wxT("glits")));
            CheckPlayback(m_pGenerator->GetSampleRate(), m_pGenerator->GetChannels());

        }
        else
        {
            pmlLog(pml::LOG_ERROR) << "IOManager\tGenerator: Could not open sequence file " << Settings::Get().Read(wxT("Output"), wxT("Sequence"), wxT("glits"));
            //m_sCurrentSequence = wxEmptyString;

            CreateSessionFromOutput(wxEmptyString);
            CheckPlayback(48000,0);
        }
    }
}


void IOManager::InitGeneratorTone()
{
    if(m_pGenerator)
    {
        m_pGenerator->SetFrequency(Settings::Get().Read(wxT("Generator"), wxT("Frequency"), 1000), Settings::Get().Read(wxT("Generator"), wxT("Amplitude"), -18.0), Settings::Get().Read(wxT("Generator"), wxT("Shape"), 0));
        m_pGenerator->Generate(8192);


        pmlLog(pml::LOG_INFO) << "IOManager\tGenerating fixed frequency " << Settings::Get().Read(wxT("Generator"), wxT("Frequency"), 1000) << "Hz at " << Settings::Get().Read(wxT("Generator"), wxT("Amplitude"), -18.0) << "dbFS";

        CreateSessionFromOutput(wxString::Format(wxT("%dHz %.1fdBFS"), Settings::Get().Read(wxT("Generator"), wxT("Frequency"), 1000), Settings::Get().Read(wxT("Generator"), wxT("Amplitude"), -18.0)));
        CheckPlayback(m_pGenerator->GetSampleRate(), m_pGenerator->GetChannels());


    }
}


void IOManager::InitGeneratorPlugin(const wxString& sPlugin)
{
    if(m_pGenerator)
    {
        m_pGenerator->SetPlugin(sPlugin);
         CreateSessionFromOutput(sPlugin);
    }
    else
    {

    }
}


void IOManager::InitGeneratorNoise()
{
    if(m_pGenerator)
    {
        m_pGenerator->SetNoise(Settings::Get().Read(wxT("Noise"), wxT("Colour"), 0), Settings::Get().Read(wxT("Noise"), wxT("Amplitude"), -18.0));

        switch(Settings::Get().Read(wxT("Noise"), wxT("Colour"), 0))
        {
            case 0:
                CreateSessionFromOutput(wxT("White"));
                break;
            case 1:
                CreateSessionFromOutput(wxT("Pink"));
                break;
            case 2:
                CreateSessionFromOutput(wxT("Grey A"));
                break;
            case 3:
                CreateSessionFromOutput(wxT("Grey K"));
                break;

        }
        CheckPlayback(m_pGenerator->GetSampleRate(), m_pGenerator->GetChannels());

    }
}





void IOManager::OpenSoundcardDevice(unsigned long nOutputSampleRate)
{
    pmlLog(pml::LOG_INFO) << "IOManager\tOpen Audio Device: Soundcard";

    int nInput(-1);
    if(Settings::Get().Read(wxT("Input"), wxT("Type"), wxT("Soundcard")) == wxT("Soundcard"))
    {
        nInput = Settings::Get().Read(wxT("Input"), wxT("Device"), 0);
    }

    int nOutput(-1);
    if(Settings::Get().Read(wxT("Output"), wxT("Destination"), wxT("Soundcard")) == wxT("Soundcard"))
    {
         nOutput = Settings::Get().Read(wxT("Output"), wxT("Device"), 0);
    }

    if(SoundcardManager::Get().Init(this, nInput, nOutput, nOutputSampleRate))
    {
        pmlLog(pml::LOG_INFO) << "IOManager\tAudio Device Created: Input [" << SoundcardManager::Get().GetInputDevice()
                                          << "]["<< SoundcardManager::Get().GetInputDeviceName() << "] Latency " << SoundcardManager::Get().GetInputLatency();
        pmlLog(pml::LOG_INFO) << "IOManager\tAudio Device Created: Output [" << SoundcardManager::Get().GetOutputDevice()
                                          << "]["<< SoundcardManager::Get().GetOutputDeviceName() << "] Latency " << SoundcardManager::Get().GetOutputLatency();

    }
    else
    {
//        if(SoundcardManager::Get().GetInputDevice() < 0)
//        {
//            Settings::Get().Write(wxT("Input"), wxT("Type"), wxT("Disabled"));
//        }
//        if(SoundcardManager::Get().GetOutputDevice() < 0)
//        {
//            Settings::Get().Write(wxT("Output"), wxT("Enabled"), 0);
//        }
    }
}

void IOManager::InitAudioInputDevice(bool bStart)
{
    wxString sType(Settings::Get().Read(wxT("Input"), wxT("Type"), wxT("Soundcard")));

    if(sType == wxT("Soundcard"))
    {
        OpenSoundcardDevice(SoundcardManager::Get().GetOutputSampleRate());

        m_nInputSource = AudioEvent::SOUNDCARD;


        m_SessionIn = session(wxEmptyString, wxT("Soundcard"), SoundcardManager::Get().GetInputDeviceName());
        //@todo its possible soundcard might not be two channels
        m_SessionIn.lstSubsession.push_back(subsession(wxEmptyString, SoundcardManager::Get().GetInputDeviceName(), wxEmptyString,
        wxT("L24"), wxEmptyString, SoundcardManager::Get().GetInputDevice(), SoundcardManager::Get().GetInputSampleRate(),
        CreateChannels(2), 0, {0,0}, refclk()));
        m_SessionIn.SetCurrentSubsession();

        SessionChanged();

        CheckPlayback(SoundcardManager::Get().GetInputSampleRate(), SoundcardManager::Get().GetInputNumberOfChannels());
    }
    else if(sType == "AoIP" || (sType == "AoIP Manual") || (sType == "NMOS" && !bStart))
    {
        InitAoIPInput();
    }
    else if(sType == wxT("Output"))
    {
        m_nInputSource = AudioEvent::OUTPUT;
        pmlLog(pml::LOG_INFO) << "IOManager\tMonitoring output";

        CreateSessionFromOutput(wxEmptyString);
    }

    if(m_bPlaybackInput)
    {
        m_nPlaybackSource = m_nInputSource;
    }
}

void IOManager::InitAoIPInput()
{
    m_nInputSource = AudioEvent::RTP;
    pmlLog(pml::LOG_INFO) << "IOManager\tCreate Audio Input Device: AoIP";

    AoIPSource source(0);
    source = AoipSourceManager::Get().FindSource(Settings::Get().Read(wxT("Input"), wxT("AoIP"), 0));

    if(source.nIndex != 0 && m_mRtp.find(source.nIndex) == m_mRtp.end())
    {
        m_nCurrentRtp = source.nIndex;
        RtpThread* pThread = new RtpThread(this, Settings::Get().Read(wxT("AoIP_Settings"), wxT("Interface"), "eth0"), wxT("pam"), source, 2048);
        pThread->Create();
        pThread->Run();

        pThread->SetQosMeasurementIntervalMS(Settings::Get().Read(wxT("QoS"), wxT("Interval"), 1000));

        m_mRtp.insert(make_pair(m_nCurrentRtp, pThread));
    }
    else
    {
        pmlLog(pml::LOG_WARN) << "IOManager\tRTP Thread already running for source " << source.nIndex;
    }
}


void IOManager::InitAudioOutputDevice()
{
    wxString sType(Settings::Get().Read(wxT("Output"), wxT("Destination"), wxT("Soundcard")));

    if(sType == wxT("Soundcard"))
    {
        pmlLog(pml::LOG_INFO) << "IOManager\tCreate Audio Destination Device: Soundcard";
        OpenSoundcardDevice(SoundcardManager::Get().GetOutputSampleRate());

        m_nOutputDestination = AudioEvent::SOUNDCARD;


        //turn off any advertising of stream
        DoSAP(false);
        DoDNSSD(false);

    }
    else if(sType == "AoIP"  || sType == "NMOS")
    {
        m_nOutputDestination = AudioEvent::RTP;
        pmlLog(pml::LOG_INFO) << "IOManager\tCreate Audio Destination Device: AoIP";
        StopStream();
        if(m_bStreamActive)
        {
            m_bQueueToStream = true;
        }
        else
        {
            Stream();
        }
    }
    else
    {
        pmlLog(pml::LOG_INFO) << "IOManager\tOutput Disabled";
        m_nOutputDestination = AudioEvent::DISABLED;
    }
    UpdateOutputSession();
}


void IOManager::CreateSessionFromOutput(const wxString& sSource)
{
    m_SessionOut = session(wxEmptyString, wxT("Output"), Settings::Get().Read(wxT("Output"), wxT("Source"), wxEmptyString));


    unsigned int nSampleRate = 48000;
    unsigned int nChannels = 2;
    std::vector<wxString> vChannels;
    if(m_nOutputDestination == AudioEvent::SOUNDCARD)
    {
        nSampleRate = SoundcardManager::Get().GetOutputSampleRate();
        nChannels = 2;
    }
    else
    {
        nSampleRate = Settings::Get().Read("Server", "SampleRate", 48000);
        nChannels = Settings::Get().Read("Server", "Channels", 2);

    }
    if(m_pGenerator)
    {
        m_pGenerator->SetSampleRate(nSampleRate);
    }
    m_SessionOut.lstSubsession.push_back(subsession(Settings::Get().Read(wxT("Output"), wxT("Source"),wxEmptyString),
    sSource, wxEmptyString, wxT("F32"), wxEmptyString, 0, nSampleRate, CreateChannels(nChannels),
                                                    0, {0,0}, refclk()));
    m_SessionOut.SetCurrentSubsession();

    SessionChanged();
}

void SetChannel(std::vector<subsession::channelGrouping>& vChannels, size_t nChannel, unsigned char nId, subsession::enumChannelGrouping grouping, subsession::enumChannel channel)
{
    if(nChannel <= vChannels.size() && nChannel > 0)
    {
        vChannels[nChannel-1] = subsession::channelGrouping(nId, grouping, channel);
    }
}

std::vector<subsession::channelGrouping> IOManager::CreateChannels(unsigned long nChannels)
{
    std::vector<subsession::channelGrouping> vChannels(nChannels);
    if(Settings::Get().Read("Input", "Type", "Soundcard") == "Soundcard")
    {
        SetChannel(vChannels, 1, 0, subsession::enumChannelGrouping::ST, subsession::enumChannel::LEFT);
        SetChannel(vChannels, 2, 0, subsession::enumChannelGrouping::ST, subsession::enumChannel::RIGHT);
    }
    else
    {
        unsigned char nGroup = 0;
        for(int i = 1; i <= nChannels; )
        {
            wxString sValue = Settings::Get().Read("ChannelMapping", wxString::Format("Ch_%d", i), "");


            if(sValue == "Mono")
            {
                SetChannel(vChannels, i, nGroup, subsession::enumChannelGrouping::M, subsession::enumChannel::MONO);
                ++i;
                ++nGroup;
            }
            else if(sValue == "U01")
            {
                SetChannel(vChannels, i, nGroup, subsession::enumChannelGrouping::U01, subsession::enumChannel::UNDEFINED_1);
                ++i;
                ++nGroup;
            }
            else if(sValue == "Dual Mono")
            {
                SetChannel(vChannels, i, nGroup, subsession::enumChannelGrouping::DM, subsession::enumChannel::MONO_1);
                SetChannel(vChannels, i+1, nGroup, subsession::enumChannelGrouping::DM, subsession::enumChannel::MONO_2);
                i+=2;
                ++nGroup;
            }
            else if(sValue == "Stereo")
            {
                SetChannel(vChannels, i, nGroup, subsession::enumChannelGrouping::ST, subsession::enumChannel::LEFT);
                SetChannel(vChannels, i+1, nGroup, subsession::enumChannelGrouping::ST, subsession::enumChannel::RIGHT);
                i+=2;
                ++nGroup;
            }
            else if(sValue == "Matrix")
            {
                SetChannel(vChannels, i, nGroup, subsession::enumChannelGrouping::LtRt, subsession::enumChannel::LEFT_TOTAL);
                SetChannel(vChannels, i+1, nGroup, subsession::enumChannelGrouping::LtRt, subsession::enumChannel::RIGHT_TOTAL);
                i+=2;
                ++nGroup;
            }
            else if(sValue == "U02")
            {
                SetChannel(vChannels, i, nGroup, subsession::enumChannelGrouping::U01, subsession::enumChannel::UNDEFINED_1);
                SetChannel(vChannels, i+1, nGroup, subsession::enumChannelGrouping::U02, subsession::enumChannel::UNDEFINED_2);
                i+=2;
                ++nGroup;
            }
            else if(sValue == "U03")
            {
                SetChannel(vChannels, i, nGroup, subsession::enumChannelGrouping::U01, subsession::enumChannel::UNDEFINED_1);
                SetChannel(vChannels, i+1, nGroup, subsession::enumChannelGrouping::U02, subsession::enumChannel::UNDEFINED_2);
                SetChannel(vChannels, i+2, nGroup, subsession::enumChannelGrouping::U02, subsession::enumChannel::UNDEFINED_3);

                i+=3;
                ++nGroup;
            }
            else if(sValue == "U04")
            {
                SetChannel(vChannels, i, nGroup, subsession::enumChannelGrouping::U01, subsession::enumChannel::UNDEFINED_1);
                SetChannel(vChannels, i+1, nGroup, subsession::enumChannelGrouping::U02, subsession::enumChannel::UNDEFINED_2);
                SetChannel(vChannels, i+2, nGroup, subsession::enumChannelGrouping::U02, subsession::enumChannel::UNDEFINED_3);
                SetChannel(vChannels, i+3, nGroup, subsession::enumChannelGrouping::U02, subsession::enumChannel::UNDEFINED_4);
                i+=4;
                ++nGroup;
            }
            else if(sValue == "SGRP")
            {
                SetChannel(vChannels, i, nGroup, subsession::enumChannelGrouping::SGRP, subsession::enumChannel::SDI_1);
                SetChannel(vChannels, i+2, nGroup, subsession::enumChannelGrouping::SGRP, subsession::enumChannel::SDI_2);
                SetChannel(vChannels, i+3, nGroup, subsession::enumChannelGrouping::SGRP, subsession::enumChannel::SDI_3);
                SetChannel(vChannels, i+4, nGroup, subsession::enumChannelGrouping::SGRP, subsession::enumChannel::SDI_4);
                i+=4;
                ++nGroup;
            }
            else if(sValue == "U05")
            {
                SetChannel(vChannels, i, nGroup, subsession::enumChannelGrouping::U01, subsession::enumChannel::UNDEFINED_1);
                SetChannel(vChannels, i+1, nGroup, subsession::enumChannelGrouping::U02, subsession::enumChannel::UNDEFINED_2);
                SetChannel(vChannels, i+2, nGroup, subsession::enumChannelGrouping::U02, subsession::enumChannel::UNDEFINED_3);
                SetChannel(vChannels, i+3, nGroup, subsession::enumChannelGrouping::U02, subsession::enumChannel::UNDEFINED_4);
                SetChannel(vChannels, i+4, nGroup, subsession::enumChannelGrouping::U02, subsession::enumChannel::UNDEFINED_5);
                i+=5;
                ++nGroup;
            }
            else if(sValue == "U06")
            {
                SetChannel(vChannels, i, nGroup, subsession::enumChannelGrouping::U01, subsession::enumChannel::UNDEFINED_1);
                SetChannel(vChannels, i+1, nGroup, subsession::enumChannelGrouping::U02, subsession::enumChannel::UNDEFINED_2);
                SetChannel(vChannels, i+2, nGroup, subsession::enumChannelGrouping::U02, subsession::enumChannel::UNDEFINED_3);
                SetChannel(vChannels, i+3, nGroup, subsession::enumChannelGrouping::U02, subsession::enumChannel::UNDEFINED_4);
                SetChannel(vChannels, i+4, nGroup, subsession::enumChannelGrouping::U02, subsession::enumChannel::UNDEFINED_5);
                SetChannel(vChannels, i+5, nGroup, subsession::enumChannelGrouping::U02, subsession::enumChannel::UNDEFINED_6);
                i+=6;
                ++nGroup;
            }
            else if(sValue == "5.1")
            {
                SetChannel(vChannels, i, nGroup, subsession::enumChannelGrouping::FIVE1, subsession::enumChannel::LEFT);
                SetChannel(vChannels, i+1, nGroup, subsession::enumChannelGrouping::FIVE1, subsession::enumChannel::RIGHT);
                SetChannel(vChannels, i+2, nGroup, subsession::enumChannelGrouping::FIVE1, subsession::enumChannel::CENTER);
                SetChannel(vChannels, i+3, nGroup, subsession::enumChannelGrouping::FIVE1, subsession::enumChannel::LFE);
                SetChannel(vChannels, i+4, nGroup, subsession::enumChannelGrouping::FIVE1, subsession::enumChannel::LEFT_SIDE);
                SetChannel(vChannels, i+5, nGroup, subsession::enumChannelGrouping::FIVE1, subsession::enumChannel::RIGHT_SIDE);

                i+=6;
                ++nGroup;
            }
            else if(sValue == "U07")
            {
                SetChannel(vChannels, i, nGroup, subsession::enumChannelGrouping::U01, subsession::enumChannel::UNDEFINED_1);
                SetChannel(vChannels, i+1, nGroup, subsession::enumChannelGrouping::U02, subsession::enumChannel::UNDEFINED_2);
                SetChannel(vChannels, i+2, nGroup, subsession::enumChannelGrouping::U02, subsession::enumChannel::UNDEFINED_3);
                SetChannel(vChannels, i+3, nGroup, subsession::enumChannelGrouping::U02, subsession::enumChannel::UNDEFINED_4);
                SetChannel(vChannels, i+4, nGroup, subsession::enumChannelGrouping::U02, subsession::enumChannel::UNDEFINED_5);
                SetChannel(vChannels, i+5, nGroup, subsession::enumChannelGrouping::U02, subsession::enumChannel::UNDEFINED_6);
                SetChannel(vChannels, i+6, nGroup, subsession::enumChannelGrouping::U02, subsession::enumChannel::UNDEFINED_7);

                i+=7;
                ++nGroup;
            }
            else if(sValue == "7.1")
            {
                SetChannel(vChannels, i, nGroup, subsession::enumChannelGrouping::SEVEN1, subsession::enumChannel::LEFT);
                SetChannel(vChannels, i+1, nGroup, subsession::enumChannelGrouping::SEVEN1, subsession::enumChannel::RIGHT);
                SetChannel(vChannels, i+2, nGroup, subsession::enumChannelGrouping::SEVEN1, subsession::enumChannel::CENTER);
                SetChannel(vChannels, i+3, nGroup, subsession::enumChannelGrouping::SEVEN1, subsession::enumChannel::LFE);
                SetChannel(vChannels, i+4, nGroup, subsession::enumChannelGrouping::SEVEN1, subsession::enumChannel::LEFT_SIDE);
                SetChannel(vChannels, i+5, nGroup, subsession::enumChannelGrouping::SEVEN1, subsession::enumChannel::RIGHT_SIDE);
                SetChannel(vChannels, i+6, nGroup, subsession::enumChannelGrouping::SEVEN1, subsession::enumChannel::LEFT_REAR_SIDE);
                SetChannel(vChannels, i+7, nGroup, subsession::enumChannelGrouping::SEVEN1, subsession::enumChannel::RIGHT_REAR_SIDE);

                i+=8;
                ++nGroup;
            }
            else if(sValue == "U08")
            {
                SetChannel(vChannels, i, nGroup, subsession::enumChannelGrouping::U01, subsession::enumChannel::UNDEFINED_1);
                SetChannel(vChannels, i+1, nGroup, subsession::enumChannelGrouping::U02, subsession::enumChannel::UNDEFINED_2);
                SetChannel(vChannels, i+2, nGroup, subsession::enumChannelGrouping::U02, subsession::enumChannel::UNDEFINED_3);
                SetChannel(vChannels, i+3, nGroup, subsession::enumChannelGrouping::U02, subsession::enumChannel::UNDEFINED_4);
                SetChannel(vChannels, i+4, nGroup, subsession::enumChannelGrouping::U02, subsession::enumChannel::UNDEFINED_5);
                SetChannel(vChannels, i+5, nGroup, subsession::enumChannelGrouping::U02, subsession::enumChannel::UNDEFINED_6);
                SetChannel(vChannels, i+6, nGroup, subsession::enumChannelGrouping::U02, subsession::enumChannel::UNDEFINED_7);
                SetChannel(vChannels, i+7, nGroup, subsession::enumChannelGrouping::U02, subsession::enumChannel::UNDEFINED_8);
                i+=8;
                ++nGroup;
            }
            else
            {
                i++;
            }
        }
    }
    return vChannels;
}



void IOManager::UpdateOutputSession(bool bMapping)
{
    if(m_SessionOut.lstSubsession.empty() == false)
    {
        unsigned int nSampleRate = 48000;
        unsigned int nChannels = 2;
        if(m_nOutputDestination == AudioEvent::SOUNDCARD)
        {
            nSampleRate = SoundcardManager::Get().GetOutputSampleRate();
            nChannels = 2;
        }
        else if(m_nOutputDestination == AudioEvent::RTP)
        {
            nSampleRate = Settings::Get().Read("Server", "SampleRate", 48000);
            nChannels = Settings::Get().Read("Server", "Channels", 2);
        }
        else if(m_nOutputDestination == AudioEvent::DISABLED)
        {
            nSampleRate = 0;
            nChannels = 0;
        }

        if(m_pGenerator && nSampleRate != 0)
        {
            m_pGenerator->SetSampleRate(nSampleRate);
        }

        if(m_SessionOut.lstSubsession.back().nSampleRate != nSampleRate || m_SessionOut.lstSubsession.back().nChannels != nChannels || bMapping)
        {
            m_SessionOut.lstSubsession.back().nSampleRate = nSampleRate;
            m_SessionOut.lstSubsession.back().nChannels = nChannels;
            m_SessionOut.lstSubsession.back().vChannels = CreateChannels(nChannels);
            SessionChanged();
        }
    }

}

void IOManager::SessionChanged()
{
    pmlLog(pml::LOG_INFO) << "IOManager\tSession Changed: " << m_setHandlers.size();
    //tell our handler that we have changed the session....
    for(set<wxEvtHandler*>::iterator itHandler = m_setHandlers.begin(); itHandler != m_setHandlers.end(); ++itHandler)
    {

        wxCommandEvent event(wxEVT_SESSION);
        (*itHandler)->ProcessEvent(event);
    }


}


void IOManager::OnRTPSessionClosed(wxCommandEvent& event)
{
    if(m_nCurrentRtp == event.GetInt())
    {
        m_nCurrentRtp = 0;
    }
    //m_setRtpOrphan.erase(event.GetInt());
    m_mRtp.erase(event.GetInt());
    pmlLog(pml::LOG_INFO) << "IOManager\tOnRTPSessionClosed: " << event.GetInt();
}

void IOManager::OnRTPSession(wxCommandEvent& event)
{
    session* pSession = reinterpret_cast<session*>(event.GetClientData());

    m_SessionIn = session(pSession->sRawSDP, pSession->sName, pSession->sType, pSession->sDescription, pSession->sGroups);
    for(list<subsession>::iterator itSub = pSession->lstSubsession.begin(); itSub != pSession->lstSubsession.end(); ++itSub)
    {
        m_SessionIn.lstSubsession.push_back((*itSub));
    }
    m_SessionIn.SetCurrentSubsession();

    SessionChanged();

    if(m_SessionIn.GetCurrentSubsession() != m_SessionIn.lstSubsession.end())
    {
        CheckPlayback(m_SessionIn.GetCurrentSubsession()->nSampleRate, min((unsigned int)256 ,m_SessionIn.GetCurrentSubsession()->nChannels));
    }
    else
    {
        CheckPlayback(0,0);
    }
}

void IOManager::CheckPlayback(unsigned long nSampleRate, unsigned long nChannels)
{
    if(m_nPlaybackSource == AudioEvent::RTP || m_nPlaybackSource == AudioEvent::SOUNDCARD)
    {
        //check the stream details against the playing details...
        if(SoundcardManager::Get().IsOutputStreamOpen() && //this makes sure we only worry if outputting via a soundcard
           (SoundcardManager::Get().GetOutputSampleRate() != nSampleRate
            || SoundcardManager::Get().GetOutputNumberOfChannels() != nChannels))
        {
            OutputChannelsChanged();
            OpenSoundcardDevice(nSampleRate);
        }
        else
        {
            OutputChannelsChanged();
        }
    }
    else
    {
        OutputChannelsChanged();
    }
}

void IOManager::ClearSession()
{
    m_SessionIn = session();
    SessionChanged();
    CheckPlayback(48000,0);
}

const session& IOManager::GetInputSession()
{
    return m_SessionIn;
}

const session& IOManager::GetOutputSession()
{
    return m_SessionOut;
}

const session& IOManager::GetSession()
{
    if(m_bMonitorOutput == false)
    {
        return m_SessionIn;
    }
    return m_SessionOut;
}


void IOManager::Start()
{
    m_bMonitorOutput = (Settings::Get().Read(wxT("Monitor"), wxT("Source"), 0)==1);

    OutputChanged(wxT("Source"));
    OutputChanged(wxT("Destination"));


    InitAudioInputDevice(true);
}

void IOManager::OutputChannelsChanged()
{
    unsigned int nInputChannels(2);
    if(m_bPlaybackInput)
    {
        if(m_SessionIn.GetCurrentSubsession() != m_SessionIn.lstSubsession.end())
        {
            nInputChannels = min((unsigned int)256 ,m_SessionIn.GetCurrentSubsession()->nChannels);
        }
    }

    vector<char> vChannels;
    if(m_SessionOut.GetCurrentSubsession() != m_SessionOut.lstSubsession.end())
    {
        for(int i = 1; i <= m_SessionOut.GetCurrentSubsession()->nChannels; i++)
        {
            vChannels.push_back(Settings::Get().Read("Output", wxString::Format("Channel_%d", i), i-1));
        }
    }
    if(m_pOnDemandSubsession)
    {
        m_pOnDemandSubsession->SetChannelMapping(vChannels);
    }
    else if(m_pAlwaysOnServer)
    {
        m_pAlwaysOnServer->SetChannelMapping(vChannels);
    }
    else
    {
        SoundcardManager::Get().SetOutputMixer(vChannels, nInputChannels);
    }

    for(set<wxEvtHandler*>::iterator itHandler = m_setHandlers.begin(); itHandler != m_setHandlers.end(); ++itHandler)
    {
        wxCommandEvent event(wxEVT_PLAYBACK_CHANNELS);
        (*itHandler)->ProcessEvent(event);
    }

}


void IOManager::OnQoS(wxCommandEvent& event)
{
    for(auto pHandler : m_setHandlers)
    {
        wxCommandEvent eventUp(wxEVT_QOS_UPDATED);
        eventUp.SetClientData(event.GetClientData());
        pHandler->ProcessEvent(eventUp);
    }
}


void IOManager::OnTimerSilence(wxTimerEvent& event)
{
    wxCommandEvent eventInput(wxEVT_INPUT_FAILED);

    for(auto pHandler : m_setHandlers)
    {
        pHandler->ProcessEvent(eventInput);
    }
}


wxString IOManager::GetRandomMulticastAddress()
{
    wxString sAddress(wxT("239"));

    for(int i = 0; i < 3; i++)
    {
        sAddress += wxString::Format(wxT(".%d"), rand()%256);
    }
    return sAddress;
}


void IOManager::OnPtpEvent(wxCommandEvent& event)
{
    auto itThread = m_mRtp.find(m_nCurrentRtp);
    if(itThread != m_mRtp.end())
    {
        itThread->second->MasterClockChanged();
    }
}

void IOManager::DoSAP(bool bRun)
{
    if(bRun == false || m_pAlwaysOnServer == nullptr)
    {
        if(m_pSapServer)
        {
            pmlLog(pml::LOG_INFO) << "IOManager\tStop SAP advertising";
            m_pSapServer = nullptr;
        }
    }
    else
    {
        if(m_pSapServer == nullptr)
        {
            m_pSapServer = std::unique_ptr<pml::SapServer>(new pml::SapServer(nullptr));
            m_pSapServer->Run();
        }
        else
        {
            m_pSapServer->RemoveAllSenders();
        }


        m_pSapServer->AddSender(IpAddress(NetworkControl::Get().GetAddress(Settings::Get().Read("Server", "RTSP_Interface", "eth0")).ToStdString()), std::chrono::milliseconds(30000), m_pAlwaysOnServer->GetSDP());

        pmlLog(pml::LOG_INFO) << "IOManager\tStart SAP advertising: " << m_pAlwaysOnServer->GetSDP();
    }
}

void IOManager::DoDNSSD(bool bRun)
{
    if(bRun == false || (m_pOnDemandServer == nullptr && m_pAlwaysOnServer == nullptr))
    {
        if(m_pPublisher)
        {
            pmlLog(pml::LOG_INFO) << "IOManager\tStop mDNS/SD advertising";
            m_pPublisher = nullptr;
        }
    }
    else
    {
        if(m_pPublisher == nullptr)
        {
            std::string sHostName(wxGetHostName().c_str());
            m_pPublisher = std::make_unique<pml::dnssd::Publisher>(GetDnsSdService().ToStdString(), "_rtsp._tcp", Settings::Get().Read(wxT("Server"), wxT("RTSP_Port"), 5555), sHostName);
            m_pPublisher->AddTxt("ver", "1.0", false);
            m_pPublisher->Start();

            pmlLog(pml::LOG_INFO) << "IOManager\tStart mDNS/SD advertising";
        }
    }
}


void IOManager::RTPServerFinished()
{
    pmlLog(pml::LOG_INFO) << "IOManager\tStream server finished.";

    m_bStreamActive = false;
    if(m_bQueueToStream)
    {
        m_bQueueToStream = false;
        Stream();
    }
}


void IOManager::Stream()
{
    bool bNmos = (Settings::Get().Read("NMOS", "Node", 0)  == 2 || Settings::Get().Read("NMOS", "Node", 0)  == 3);

    if(Settings::Get().Read("Server", "State", 0) == 1 || bNmos) //set to stream
    {
        m_bStreamAlwaysOn = (Settings::Get().Read("Server", "Stream", "AlwaysOn") == "AlwaysOn");
        if(m_bStreamAlwaysOn || bNmos)  //@todo bodge for NMOS
        {
            StreamAlwaysOn();
            DoSAP(Settings::Get().Read("Server", "SAP",0));
        }
        else
        {
            StreamOnDemand();
            DoSAP(false);
        }
        DoDNSSD(Settings::Get().Read("Server", "DNS-SD", true));
    }
}

void IOManager::StreamAlwaysOn()
{
    pmlLog(pml::LOG_INFO) << "IOManager\tCreate AlwaysOn AES67 Server";
    if(m_pAlwaysOnServer == nullptr && m_pOnDemandServer == nullptr)
    {
        wxString sDestinationIp = Settings::Get().Read(wxT("Server"), wxT("DestinationIp"), wxEmptyString);
        unsigned long nByte;
        bool bSSM(sDestinationIp.BeforeFirst(wxT('.')).ToULong(&nByte) && nByte >= 224 && nByte <= 239);

        m_pAlwaysOnServer = new RtpServerThread(this, m_setRTCPHandlers, NetworkControl::Get().GetAddress(Settings::Get().Read("Server", "RTSP_Interface", "eth0")),
                                                 Settings::Get().Read(wxT("Server"), wxT("RTSP_Port"), 5555),
                                                 sDestinationIp,
                                                Settings::Get().Read(wxT("Server"), wxT("RTP_Port"), 5004),
                                                bSSM,
                                                 (LiveAudioSource::enumPacketTime)Settings::Get().Read(wxT("Server"), wxT("PacketTime"), 1000));
        m_pAlwaysOnServer->Run();
        m_bStreamActive = true;
    }
    else
    {
        pmlLog(pml::LOG_ERROR) << "Attempting to stream AlwaysOn but already streaming";
    }
}

void IOManager::StreamOnDemand()
{
    pmlLog(pml::LOG_INFO) << "IOManager\tCreate OnDemand AES67 Server";
    if(m_pAlwaysOnServer == nullptr && m_pOnDemandServer == nullptr)
    {
        m_pOnDemandServer = new OnDemandStreamer(m_setRTSPHandlers, m_setRTCPHandlers, NetworkControl::Get().GetAddress(Settings::Get().Read("Server", "RTSP_Interface", "eth0")),
                                              Settings::Get().Read(wxT("Server"), wxT("RTSP_Port"), 5555));

        m_pOnDemandSubsession = OnDemandAES67MediaSubsession::createNew(this, *m_pOnDemandServer->envir(), Settings::Get().Read("Server", "Channels", 2),
        GetChannelMapping(),
        Settings::Get().Read("Server", "RtpMap", 96),(LiveAudioSource::enumPacketTime)Settings::Get().Read(wxT("Server"), wxT("PacketTime"), 1000),
        Settings::Get().Read("Server", "Bits", 24),Settings::Get().Read("Server", "SampleRate", 48000), Settings::Get().Read(wxT("Server"), wxT("RTP_Port"), 5004));

        m_pOnDemandServer->Create();
        m_pOnDemandServer->SetSubsession(m_pOnDemandSubsession);


        m_pOnDemandServer->Run();
        m_bStreamActive = true;
    }
    else
    {
        pmlLog(pml::LOG_ERROR) << "Attempting to stream OnDemand but already streaming";
    }
}


void IOManager::CheckIfGain()
{
    m_bGain = false;
    for(size_t i = 0; i < m_vRatio.size(); i++)
    {
        if(m_vRatio[i] != 1.0)
        {
            m_bGain = true;
            break;
        }
    }
}

void IOManager::RestartStream()
{
    StopStream();
    m_timerResetStream.Start(500,true);
}

void IOManager::OnTimerReset(wxTimerEvent& event)
{
    UpdateOutputSession();
    Stream();
}

wxString IOManager::GetDnsSdService() const
{
    return "AES67@"+wxGetHostName();
}




