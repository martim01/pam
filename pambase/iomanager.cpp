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


using namespace std;


DEFINE_EVENT_TYPE(wxEVT_SESSION)
DEFINE_EVENT_TYPE(wxEVT_PLAYBACK_CHANNELS)
DEFINE_EVENT_TYPE(wxEVT_INPUT_FAILED)

IOManager& IOManager::Get()
{
    static IOManager iom;
    return iom;
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
    m_bStreamMulticast(false),
    m_pMulticastServer(nullptr),
    m_pUnicastServer(nullptr),
    m_pOnDemandSubsession(nullptr),
    m_pSapServer(nullptr),
    m_pPublisher(nullptr),
    m_bQueueToStream(false),
    m_bStreamActive(false)

{

    AoipSourceManager::Get();
    Settings::Get().Write(wxT("Server"), wxT("Stream"), "Unicast"); //can't be streaming at startup so set to 0 in case we exited whilst streaming

    Settings::Get().AddHandler(wxT("Input"),wxT("Type"), this);
    Settings::Get().AddHandler(wxT("Input"),wxT("AoIP"), this);
    Settings::Get().AddHandler(wxT("Input"),wxT("Device"), this);
    Settings::Get().AddHandler(wxT("Input"),wxT("File"), this);

    m_vRatio.resize(8);
    for(size_t i = 0; i < 8; i++)
    {
        Settings::Get().AddHandler("Input", wxString::Format("Ratio_%02d", i), this);
        m_vRatio[i] = Settings::Get().Read("Input",  wxString::Format("Ratio_%02d", i), 1.0);
    }
    CheckIfGain();

    Settings::Get().AddHandler(wxT("Output"),wxT("Device"), this);
    Settings::Get().AddHandler(wxT("Output"),wxT("Destination"), this);
    Settings::Get().AddHandler(wxT("Output"),wxT("Latency"), this);
    Settings::Get().AddHandler(wxT("Output"),wxT("Left"), this);
    Settings::Get().AddHandler(wxT("Output"),wxT("Right"), this);

    Settings::Get().AddHandler(wxT("Output"),wxT("Source"), this);
    Settings::Get().AddHandler(wxT("Output"),wxT("File"), this);
    Settings::Get().AddHandler(wxT("Output"),wxT("Sequence"), this);

    Settings::Get().AddHandler(wxT("Generator"),wxT("Frequency"), this);
    Settings::Get().AddHandler(wxT("Generator"),wxT("Amplitude"), this);
    Settings::Get().AddHandler(wxT("Generator"),wxT("Shape"), this);
    Settings::Get().AddHandler(wxT("Noise"),wxT("Colour"), this);
    Settings::Get().AddHandler(wxT("Noise"),wxT("Amplitude"), this);

    Settings::Get().AddHandler(wxT("Monitor"), wxT("Source"), this);

    Settings::Get().AddHandler(wxT("QoS"),wxT("Interval"), this);

    Settings::Get().AddHandler(wxT("Server"), wxT("Stream"), this);
    Settings::Get().AddHandler(wxT("Server"), wxT("SAP"), this);
    Settings::Get().AddHandler(wxT("Server"), wxT("DNS-SD"), this);

    Connect(wxID_ANY,wxEVT_DATA,(wxObjectEventFunction)&IOManager::OnAudioEvent);
    Connect(wxID_ANY,wxEVT_RTP_SESSION,(wxObjectEventFunction)&IOManager::OnRTPSession);
    Connect(wxID_ANY,wxEVT_RTP_SESSION_CLOSED,(wxObjectEventFunction)&IOManager::OnRTPSessionClosed);
    Connect(wxID_ANY, wxEVT_SETTING_CHANGED, (wxObjectEventFunction)&IOManager::OnSettingEvent);

    Connect(wxID_ANY,wxEVT_QOS_UPDATED,(wxObjectEventFunction)&IOManager::OnQoS);

    Connect(wxID_ANY,wxEVT_ODS_FINISHED,(wxObjectEventFunction)&IOManager::OnUnicastServerFinished);

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

    m_timerSilence.Start(250, true);

    srand(time(NULL));

}


IOManager::~IOManager()
{
    Stop();
}

void IOManager::Stop()
{
    delete m_pGenerator;
    m_pGenerator = 0;

    for(map<unsigned int, RtpThread*>::iterator itThread = m_mRtp.begin(); itThread != m_mRtp.end(); ++itThread)
    {
        itThread->second->SetToClose();
        itThread->second->Wait();
        delete itThread->second;
    }
    m_mRtp.clear();
    SoundcardManager::Get().Terminate();

    StopStream();
}

void IOManager::StopStream()
{
    if(m_pMulticastServer)
    {
        m_pMulticastServer->StopStream();
        m_pMulticastServer = nullptr;
    }
    else if(m_pUnicastServer)
    {
        m_pUnicastServer->Stop();
        m_pUnicastServer = nullptr;
        m_pOnDemandSubsession = nullptr;
    }


    m_pSapServer = nullptr;
    m_pPublisher = nullptr;
}


void IOManager::OnSettingEvent(SettingEvent& event)
{
    if(event.GetSection() == wxT("Monitor"))
    {
        if(event.GetKey() == wxT("Source") && m_bMonitorOutput != (Settings::Get().Read(wxT("Monitor"), wxT("Source"), 0)==1))
        {
            m_bMonitorOutput = (Settings::Get().Read(wxT("Monitor"), wxT("Source"), 0)==1);
            SessionChanged();
        }
    }
    else if(event.GetSection() == wxT("Input"))
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
                m_vRatio[nChannel] = Settings::Get().Read("Input", event.GetKey(), 1.0);
            }
            CheckIfGain();
        }
        else
        {
            InputChanged(event.GetKey());
        }
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
        if(event.GetKey() == wxT("Interval"))
        {
            map<unsigned int, RtpThread*>::iterator itThread = m_mRtp.find(m_nCurrentRtp);
            if(itThread != m_mRtp.end())
            {
                itThread->second->SetQosMeasurementIntervalMS(Settings::Get().Read(wxT("QoS"), wxT("Interval"), 1000));
            }
        }
    }
    else if(event.GetSection() == wxT("Server"))
    {
        if(event.GetKey() == wxT("Stream"))
        {
            m_bStreamMulticast = (event.GetValue() == "Multicast");
            if(!m_bStreamMulticast)
            {
                if(m_pMulticastServer)
                {
                    m_pMulticastServer->StopStream();
                    m_pMulticastServer = nullptr;
                }
            }
            else
            {
                if(m_pUnicastServer)
                {
                    m_pUnicastServer->Stop();
                    m_pUnicastServer = nullptr;
                    m_pOnDemandSubsession = nullptr;
                }
            }
            InitAudioOutputDevice();
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
                if(m_pMulticastServer)
                {
                    m_pMulticastServer->AddSamples(event.GetBuffer());
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
            AddOutputSamples(event.GetBuffer()->GetBufferSize());
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
                if(m_pMulticastServer)
                {
                    m_pMulticastServer->AddSamples(pBuffer);
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
    if(sKey == wxT("AoIP"))
    {
        pml::Log::Get(pml::Log::LOG_DEBUG) << "IOManager\tInputChanged: AoIP" << std::endl;

        AoIPSource source = AoipSourceManager::Get().FindSource(Settings::Get().Read(wxT("Input"), wxT("AoIP"), 0));

        if(source.nIndex != m_nCurrentRtp)
        {
            pml::Log::Get(pml::Log::LOG_DEBUG) << "IOManager\tAudio Input Device Changed: Close AoIP Session" << std::endl;

            ClearSession();
            map<unsigned int, RtpThread*>::iterator itThread = m_mRtp.find(m_nCurrentRtp);
            if(itThread != m_mRtp.end())
            {
                pml::Log::Get(pml::Log::LOG_DEBUG) << "IOManager\tDestroy thread" << std::endl;
                itThread->second->SetToClose();
                itThread->second->Wait();
                itThread->second->Delete();
                m_mRtp.erase(m_nCurrentRtp);
                m_nCurrentRtp = 0;
                pml::Log::Get(pml::Log::LOG_DEBUG) << "IOManager\tDestroy thread: Done" << std::endl;

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
            map<unsigned int, RtpThread*>::iterator itThread = m_mRtp.find(m_nCurrentRtp);
            if(itThread != m_mRtp.end())
            {
                pml::Log::Get(pml::Log::LOG_DEBUG) << "IOManager\tAudio Input Device Changed: Close AoIP" << std::endl;

                pml::Log::Get(pml::Log::LOG_DEBUG) << "IOManager\tDestroy thread" << std::endl;
                itThread->second->SetToClose();
                itThread->second->Wait();
                itThread->second->Delete();
                m_mRtp.erase(m_nCurrentRtp);
                m_nCurrentRtp = 0;
                pml::Log::Get(pml::Log::LOG_DEBUG) << "IOManager\tDestroy thread: Done" << std::endl;

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
            pml::Log::Get(pml::Log::LOG_INFO) << "IOManager\tOutput Destination changed: was Soundcard" << std::endl;
            OpenSoundcardDevice(SoundcardManager::Get().GetOutputSampleRate());    //this will remove the input stream
            break;
        case AudioEvent::RTP:
            pml::Log::Get(pml::Log::LOG_INFO) << "IOManager\tOutput Destination changed: was AoIP" << std::endl;
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
                if(m_pMulticastServer)
                {
                    m_pMulticastServer->FlushQueue();
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
            pml::Log::Get(pml::Log::LOG_INFO) << "IOManager\tCreate Audio Output Generator: File" << std::endl;
            InitGeneratorFile();
        }
        else if(sType == wxT("Sequence"))
        {
            m_nPlaybackSource = AudioEvent::GENERATOR;
            pml::Log::Get(pml::Log::LOG_INFO) << "IOManager\tCreate Audio Output Generator: Sequence" << std::endl;
            InitGeneratorSequence();
        }
        else if(sType == wxT("Generator"))
        {
            m_nPlaybackSource = AudioEvent::GENERATOR;
            pml::Log::Get(pml::Log::LOG_INFO) << "IOManager\tCreate Audio Output Generator: Generator" << std::endl;
            InitGeneratorTone();
        }
        else  if(sType == wxT("Noise"))
        {
            m_nPlaybackSource = AudioEvent::GENERATOR;
            InitGeneratorNoise();
        }
        else if(sType == wxT("Input"))
        {
            pml::Log::Get(pml::Log::LOG_INFO) << "IOManager\tOutput source is input" << std::endl;
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
            pml::Log::Get(pml::Log::LOG_INFO) << "IOManager\tSoundcard output device changed" << std::endl;
            OpenSoundcardDevice(SoundcardManager::Get().GetOutputSampleRate());
        }
    }
    else if(sKey == wxT("Latency"))
    {
        // @todo change the latency by closing outpt and open agin
    }
    else if(sKey == wxT("File") && Settings::Get().Read(wxT("Output"), wxT("Source"), wxT("Input")) == wxT("File"))
    {
        pml::Log::Get(pml::Log::LOG_INFO) << "IOManager\tChange Audio Output Generator: File" << std::endl;
        InitGeneratorFile();
    }
    else if(sKey == wxT("Sequence") && Settings::Get().Read(wxT("Output"), wxT("Source"), wxT("Input")) == wxT("Sequence"))
    {
        pml::Log::Get(pml::Log::LOG_INFO) << "IOManager\tChange Audio Output Generator: Sequence" << std::endl;
        InitGeneratorSequence();
    }
    else if(sKey == wxT("Left") || sKey == wxT("Right"))
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
        pml::Log::Get(pml::Log::LOG_INFO) << "IOManager\tChange Audio Output Generator: Noise" << std::endl;
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
            pml::Log::Get(pml::Log::LOG_INFO) << "IOManager\tGenerating sequence file " << Settings::Get().Read(wxT("Output"), wxT("Sequence"), wxT("glits")) << std::endl;
            //m_sCurrentSequence = sSequence;

            CreateSessionFromOutput(Settings::Get().Read(wxT("Output"), wxT("Sequence"), wxT("glits")));
            CheckPlayback(m_pGenerator->GetSampleRate(), m_pGenerator->GetChannels());

        }
        else
        {
            pml::Log::Get(pml::Log::LOG_ERROR) << "IOManager\tGenerator: Could not open sequence file " << Settings::Get().Read(wxT("Output"), wxT("Sequence"), wxT("glits")) << std::endl;
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


        pml::Log::Get(pml::Log::LOG_INFO) << "IOManager\tGenerating fixed frequency " << Settings::Get().Read(wxT("Generator"), wxT("Frequency"), 1000) << "Hz at " << Settings::Get().Read(wxT("Generator"), wxT("Amplitude"), -18.0) << "dbFS" << std::endl;

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
    pml::Log::Get(pml::Log::LOG_INFO) << "IOManager\tOpen Audio Device: Soundcard" << std::endl;

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
        pml::Log::Get(pml::Log::LOG_INFO) << "IOManager\tAudio Device Created: Input [" << SoundcardManager::Get().GetInputDevice()
                                          << "]["<< SoundcardManager::Get().GetInputDeviceName() << "] Latency " << SoundcardManager::Get().GetInputLatency() << std::endl;
        pml::Log::Get(pml::Log::LOG_INFO) << "IOManager\tAudio Device Created: Output [" << SoundcardManager::Get().GetOutputDevice()
                                          << "]["<< SoundcardManager::Get().GetOutputDeviceName() << "] Latency " << SoundcardManager::Get().GetOutputLatency() << std::endl;

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

void IOManager::InitAudioInputDevice()
{
    wxString sType(Settings::Get().Read(wxT("Input"), wxT("Type"), wxT("Soundcard")));

    if(sType == wxT("Soundcard"))
    {
        OpenSoundcardDevice(SoundcardManager::Get().GetOutputSampleRate());

        m_nInputSource = AudioEvent::SOUNDCARD;


        m_SessionIn = session(wxEmptyString, wxT("Soundcard"), SoundcardManager::Get().GetInputDeviceName());
        m_SessionIn.lstSubsession.push_back(subsession(wxEmptyString, SoundcardManager::Get().GetInputDeviceName(), wxEmptyString, wxT("L24"), wxEmptyString, SoundcardManager::Get().GetInputDevice(), SoundcardManager::Get().GetInputSampleRate(), SoundcardManager::Get().GetInputNumberOfChannels(), wxEmptyString, 0, make_pair(0,0), refclk()));
        m_SessionIn.SetCurrentSubsession();

        SessionChanged();

        CheckPlayback(SoundcardManager::Get().GetInputSampleRate(), SoundcardManager::Get().GetInputNumberOfChannels());
    }
    else if(sType == wxT("AoIP"))
    {

        m_nInputSource = AudioEvent::RTP;
        pml::Log::Get(pml::Log::LOG_INFO) << "IOManager\tCreate Audio Input Device: AoIP" << std::endl;

        AoIPSource source = AoipSourceManager::Get().FindSource(Settings::Get().Read(wxT("Input"), wxT("AoIP"), 0));
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
            pml::Log::Get(pml::Log::LOG_WARN) << "IOManager\tRTP Thread already running for source " << source.nIndex << std::endl;
        }
    }
    else if(sType == wxT("Output"))
    {
        m_nInputSource = AudioEvent::OUTPUT;
        pml::Log::Get(pml::Log::LOG_INFO) << "IOManager\tMonitoring output" << std::endl;

        CreateSessionFromOutput(wxEmptyString);
    }

    if(m_bPlaybackInput)
    {
        m_nPlaybackSource = m_nInputSource;
    }
}



void IOManager::InitAudioOutputDevice()
{
    wxString sType(Settings::Get().Read(wxT("Output"), wxT("Destination"), wxT("Soundcard")));

    if(sType == wxT("Soundcard"))
    {
        pml::Log::Get(pml::Log::LOG_INFO) << "IOManager\tCreate Audio Destination Device: Soundcard" << std::endl;
        OpenSoundcardDevice(SoundcardManager::Get().GetOutputSampleRate());

        m_nOutputDestination = AudioEvent::SOUNDCARD;

        //turn off any advertising of stream
        DoSAP(false);
        DoDNSSD(false);

    }
    else if(sType == wxT("AoIP"))
    {
        m_nOutputDestination = AudioEvent::RTP;
        pml::Log::Get(pml::Log::LOG_INFO) << "IOManager\tCreate Audio Destination Device: AoIP" << std::endl;
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
        pml::Log::Get(pml::Log::LOG_INFO) << "IOManager\tOutput Disabled" << std::endl;
        m_nOutputDestination = AudioEvent::DISABLED;
    }
}


void IOManager::CreateSessionFromOutput(const wxString& sSource)
{
    m_SessionOut = session(wxEmptyString, wxT("Output"), Settings::Get().Read(wxT("Output"), wxT("Source"), wxEmptyString));
    //we need to get the info from the output...
    unsigned int nSampleRate = SoundcardManager::Get().GetOutputSampleRate();

    m_SessionOut.lstSubsession.push_back(subsession(Settings::Get().Read(wxT("Output"), wxT("Source"),wxEmptyString), sSource, wxEmptyString, wxT("F32"), wxEmptyString, 0, nSampleRate, 2, wxEmptyString, 0, make_pair(0,0), refclk()));
    m_SessionOut.SetCurrentSubsession();

    SessionChanged();

}

void IOManager::SessionChanged()
{
    pml::Log::Get(pml::Log::LOG_INFO) << "IOManager\tSession Changed: " << m_setHandlers.size() << std::endl;
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
    pml::Log::Get(pml::Log::LOG_DEBUG) << "IOManager\tOnRTPSessionClosed" << std::endl;
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
        if(SoundcardManager::Get().IsOutputStreamOpen() && (SoundcardManager::Get().GetOutputSampleRate() != nSampleRate ||
                                                            SoundcardManager::Get().GetOutputNumberOfChannels() != nChannels))
        {
            OutputChannelsChanged();
            OpenSoundcardDevice(nSampleRate);
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


    InitAudioInputDevice();
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
    vChannels.push_back(Settings::Get().Read(wxT("Output"), wxT("Left"), 0));
    vChannels.push_back(Settings::Get().Read(wxT("Output"), wxT("Right"), 1));
    SoundcardManager::Get().SetOutputMixer(vChannels, nInputChannels);

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
    if(bRun == false || m_pMulticastServer == nullptr)
    {
        if(m_pSapServer)
        {
            pml::Log::Get(pml::Log::LOG_INFO) << "IOManager\tStop SAP advertising" << std::endl;
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


        m_pSapServer->AddSender(IpAddress(std::string(Settings::Get().Read(wxT("Server"), wxT("RTSP_Address"), wxEmptyString).c_str())), std::chrono::milliseconds(30000), m_pMulticastServer->GetSDP());

        pml::Log::Get(pml::Log::LOG_INFO) << "IOManager\tStart SAP advertising: " << m_pMulticastServer->GetSDP() << std::endl;
    }
}

void IOManager::DoDNSSD(bool bRun)
{
    if(bRun == false || (m_pUnicastServer == nullptr && m_pMulticastServer == nullptr))
    {
        if(m_pPublisher)
        {
            pml::Log::Get(pml::Log::LOG_INFO) << "IOManager\tStop mDNS/SD advertising" << std::endl;
            m_pPublisher = nullptr;
        }
    }
    else
    {
        if(m_pPublisher == nullptr)
        {
            std::string sHostName(wxGetHostName().c_str());
            std::string sService = "PAM_AES67";
            m_pPublisher = std::unique_ptr<pml::Publisher>(new pml::Publisher(sService, "_rtsp._tcp", Settings::Get().Read(wxT("Server"), wxT("RTSP_Port"), 5555), sHostName));
            m_pPublisher->AddTxt("ver", "1.0", false);
            m_pPublisher->Start();

            pml::Log::Get(pml::Log::LOG_INFO) << "IOManager\tStart mDNS/SD advertising" << std::endl;
        }
    }
}


void IOManager::OnUnicastServerFinished(wxCommandEvent& event)
{
    pml::Log::Get(pml::Log::LOG_INFO) << "IOManager\tStream server finished." << std::endl;

    m_bStreamActive = false;
    if(m_bQueueToStream)
    {
        m_bQueueToStream = false;
        Stream();
    }
}


void IOManager::Stream()
{
    if(m_bStreamMulticast)
    {
        StreamMulticast();
        DoSAP(Settings::Get().Read("Server", "SAP",0));
    }
    else
    {
        StreamUnicast();
        DoSAP(false);
    }

    DoDNSSD(Settings::Get().Read("Server", "DNS-SD", 0));
}

void IOManager::StreamMulticast()
{
    pml::Log::Get(pml::Log::LOG_INFO) << "IOManager\tCreate Multicast AES67 Server" << std::endl;

    wxString sDestinationIp = Settings::Get().Read(wxT("Server"), wxT("DestinationIp"), wxEmptyString);
    unsigned long nByte;
    bool bSSM(sDestinationIp.BeforeFirst(wxT('.')).ToULong(&nByte) && nByte >= 224 && nByte <= 239);

    m_pMulticastServer = new RtpServerThread(this, Settings::Get().Read(wxT("Server"), wxT("RTSP_Address"), wxEmptyString),
                                             Settings::Get().Read(wxT("Server"), wxT("RTSP_Port"), 5555),
                                             sDestinationIp,
                                            Settings::Get().Read(wxT("Server"), wxT("RTP_Port"), 5004),
                                            bSSM,
                                             (LiveAudioSource::enumPacketTime)Settings::Get().Read(wxT("Server"), wxT("PacketTime"), 1000));
    m_pMulticastServer->Run();
    m_bStreamActive = true;
}

void IOManager::StreamUnicast()
{
    pml::Log::Get(pml::Log::LOG_INFO) << "IOManager\tCreate Unicast AES67 Server" << std::endl;

    m_pUnicastServer = new OnDemandStreamer(this, Settings::Get().Read(wxT("Server"), wxT("RTSP_Address"), "0.0.0.0"),
                                              Settings::Get().Read(wxT("Server"), wxT("RTSP_Port"), 5555));

    m_pOnDemandSubsession = OnDemandAES67MediaSubsession::createNew(this, *m_pUnicastServer->envir(), 2, (LiveAudioSource::enumPacketTime)Settings::Get().Read(wxT("Server"), wxT("PacketTime"), 1000), Settings::Get().Read(wxT("Server"), wxT("RTP_Port"), 5004));

    m_pUnicastServer->Create();
    m_pUnicastServer->SetSubsession(m_pOnDemandSubsession);


    m_pUnicastServer->Run();
    m_bStreamActive = true;
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
