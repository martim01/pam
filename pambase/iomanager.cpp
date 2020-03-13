#include "iomanager.h"
#include "settingevent.h"
#include "settings.h"
#include "audioevent.h"
#include "session.h"
#include "wmlogevent.h"
#include "soundcardmanager.h"
#include "generator.h"
#include "rtpthread.h"
#include <wx/log.h>
#include "rtpserverthread.h"
#include <ctime>
#include "wxptp.h"
#include "aoipsourcemanager.h"

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
    m_pGenerator(0),
    m_bStream(false),
    m_pRtpServer(0)

{

    AoipSourceManager::Get();
    Settings::Get().Write(wxT("Server"), wxT("Stream"), 0); //can't be streaming at startup so set to 0 in case we exited whilst streaming

    Settings::Get().AddHandler(wxT("Input"),wxT("Type"), this);
    Settings::Get().AddHandler(wxT("Input"),wxT("AoIP"), this);
    Settings::Get().AddHandler(wxT("Input"),wxT("Device"), this);
    Settings::Get().AddHandler(wxT("Input"),wxT("File"), this);


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
        bool bDelete = m_setRtpOrphan.insert(itThread->first).second;
        if(bDelete)
        {
            itThread->second->Delete();
        }
    }
    m_mRtp.clear();
    SoundcardManager::Get().Terminate();

    if(m_pRtpServer)
    {
        m_pRtpServer->StopStream();
    }
    m_pRtpServer = 0;
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
    else if(event.GetSection() == wxT("Server") && event.GetKey() == wxT("Stream"))
    {
        m_bStream = event.GetValue(false);
        if(event.GetValue(false) == false)
        {
            if(m_pRtpServer)
            {
                m_pRtpServer->StopStream();
            }
            m_pRtpServer = NULL;
        }
        else
        {
            InitAudioOutputDevice();
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
                if(m_pRtpServer)
                {
                    m_pRtpServer->AddSamples(event.GetBuffer());
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
                if(m_pRtpServer)
                {
                    m_pRtpServer->AddSamples(pBuffer);
                }
                break;
            default:
                wxLogDebug(wxT("Output=%d"), m_nOutputDestination);
        }
        delete pBuffer;
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
        wmLog::Get()->Log(wxT("IOManager::InputChanged: AoIP"));
        AoIPSource source = AoipSourceManager::Get().FindSource(Settings::Get().Read(wxT("Input"), wxT("AoIP"), 0));

        if(source.nIndex != m_nCurrentRtp)
        {
            wmLog::Get()->Log(wxT("Audio Input Device Changed: Close AoIP Session"));
            ClearSession();
            map<unsigned int, RtpThread*>::iterator itThread = m_mRtp.find(m_nCurrentRtp);
            if(itThread != m_mRtp.end())
            {
                bool bDelete = m_setRtpOrphan.insert(itThread->first).second;
                if(bDelete)
                {
                    itThread->second->Delete();
                }
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
    wxLogDebug(wxT("IOManager::InputTypeChanged"));
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
                wmLog::Get()->Log(wxT("Audio Input Device Changed: Close AoIP"));
                bool bDelete = m_setRtpOrphan.insert(itThread->first).second;
                if(bDelete)
                {
                    itThread->second->Delete();
                }
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
            wmLog::Get()->Log(wxT("Output Destination changed: was Soundcard"));
            OpenSoundcardDevice(SoundcardManager::Get().GetOutputSampleRate());    //this will remove the input stream
            break;
        case AudioEvent::RTP:
            wmLog::Get()->Log(wxT("Output Destination changed: was AoIP"));
            if(m_pRtpServer)
            {
                m_pRtpServer->StopStream();
            }
            m_pRtpServer = NULL;
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
                if(m_pRtpServer)
                {
                    m_pRtpServer->FlushQueue();
                }
                break;
        }

        if(sType == wxT("File"))
        {
            m_nPlaybackSource = AudioEvent::FILE;
            wmLog::Get()->Log(wxT("Create Audio Output Generator: File"));
            InitGeneratorFile();
        }
        else if(sType == wxT("Sequence"))
        {
            m_nPlaybackSource = AudioEvent::GENERATOR;
            wmLog::Get()->Log(wxT("Create Audio Output Generator: Sequence"));
            InitGeneratorSequence();
        }
        else if(sType == wxT("Generator"))
        {
            m_nPlaybackSource = AudioEvent::GENERATOR;
            wmLog::Get()->Log(wxT("Create Audio Output Generator: Generator"));
            InitGeneratorTone();
        }
        else  if(sType == wxT("Noise"))
        {
            m_nPlaybackSource = AudioEvent::GENERATOR;
            InitGeneratorNoise();
        }
        else if(sType == wxT("Input"))
        {
            wmLog::Get()->Log(wxT("Output source is input"));
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
            wmLog::Get()->Log(wxT("Soundcard output device changed"));
            OpenSoundcardDevice(SoundcardManager::Get().GetOutputSampleRate());
        }
    }
    else if(sKey == wxT("Latency"))
    {
        // @todo change the latency by closing outpt and open agin
    }
    else if(sKey == wxT("File") && Settings::Get().Read(wxT("Output"), wxT("Source"), wxT("Input")) == wxT("File"))
    {
        wmLog::Get()->Log(wxT("Change Audio Output Generator: File"));
        InitGeneratorFile();
    }
    else if(sKey == wxT("Sequence") && Settings::Get().Read(wxT("Output"), wxT("Source"), wxT("Input")) == wxT("Sequence"))
    {
        wmLog::Get()->Log(wxT("Change Audio Output Generator: Sequence"));
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
        wmLog::Get()->Log(wxT("Change Audio Output Generator: Noise"));
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
            wmLog::Get()->Log(wxString::Format(wxT("Generating sequence file %s"), Settings::Get().Read(wxT("Output"), wxT("Sequence"), wxT("glits"))).c_str());
            //m_sCurrentSequence = sSequence;

            CreateSessionFromOutput(Settings::Get().Read(wxT("Output"), wxT("Sequence"), wxT("glits")));
            CheckPlayback(m_pGenerator->GetSampleRate(), m_pGenerator->GetChannels());

        }
        else
        {
            wmLog::Get()->Log(wxString::Format(wxT("Could not open sequence file %s"), Settings::Get().Read(wxT("Output"), wxT("Sequence"), wxT("glits"))).c_str());
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


        wmLog::Get()->Log(wxString::Format(wxT("Generating fixed frequency %dHz at %.1fdB"),Settings::Get().Read(wxT("Generator"), wxT("Frequency"), 1000), Settings::Get().Read(wxT("Generator"), wxT("Amplitude"), -18.0)));

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
    wmLog::Get()->Log(wxT("Open Audio Device: Soundcard"));

    int nInput(-1);
    if(Settings::Get().Read(wxT("Input"), wxT("Type"), wxT("Soundcard")) == wxT("Soundcard"))
    {
        nInput = Settings::Get().Read(wxT("Input"), wxT("Device"), 0);
    }

    int nOutput(-1);
    if(Settings::Get().Read(wxT("Output"), wxT("Type"), wxT("Soundcard")) == wxT("Soundcard"))
    {
         nOutput = Settings::Get().Read(wxT("Output"), wxT("Device"), 0);
    }

    if(SoundcardManager::Get().Init(this, nInput, nOutput, nOutputSampleRate))
    {
        wmLog::Get()->Log(wxString::Format(wxT("Audio Device Created: Input [%d][%s] Latency %.2f"), SoundcardManager::Get().GetInputDevice(), SoundcardManager::Get().GetInputDeviceName().c_str(), SoundcardManager::Get().GetInputLatency()));
        wmLog::Get()->Log(wxString::Format(wxT("Audio Device Created: Output [%d][%s] Latency %.2f"), SoundcardManager::Get().GetOutputDevice(), SoundcardManager::Get().GetOutputDeviceName().c_str(), SoundcardManager::Get().GetOutputLatency()));
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
        wxLogDebug(wxT("IOManager::InitAudioInputDevice: AoIP"));
        m_nInputSource = AudioEvent::RTP;
        wmLog::Get()->Log(wxT("Create Audio Input Device: AoIP"));

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
    }
    else if(sType == wxT("Output"))
    {
        m_nInputSource = AudioEvent::OUTPUT;
        wmLog::Get()->Log(wxT("Monitoring output"));

        CreateSessionFromOutput(wxEmptyString);
    }
    else
    {
        wxLogDebug(wxT("IOManager::InitAudioInputDevice: Unknown"));
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
        wmLog::Get()->Log(wxT("Create Audio Destination Device: Soundcard"));
        OpenSoundcardDevice(SoundcardManager::Get().GetOutputSampleRate());

        m_nOutputDestination = AudioEvent::SOUNDCARD;

    }
    else if(sType == wxT("AoIP"))
    {
        m_nOutputDestination = AudioEvent::RTP;
        wmLog::Get()->Log(wxT("Create Audio Destination Device: AoIP"));
        if(m_pRtpServer)
        {
            m_pRtpServer->StopStream();
            m_pRtpServer = NULL;
        }
        if(m_bStream)
        {
            wxString sDestinationIp = Settings::Get().Read(wxT("Server"), wxT("DestinationIp"), wxEmptyString);
            unsigned long nByte;
            bool bSSM(sDestinationIp.BeforeFirst(wxT('.')).ToULong(&nByte) && nByte >= 224 && nByte <= 239);

            m_pRtpServer = new RtpServerThread(this, Settings::Get().Read(wxT("Server"), wxT("RTSP_Address"), wxEmptyString),
                                                     Settings::Get().Read(wxT("Server"), wxT("RTSP_Port"), 5555),
                                                     sDestinationIp,
                                                     Settings::Get().Read(wxT("Server"), wxT("RTP_Port"), 5004),
                                                     bSSM,
                                                     (LiveAudioSource::enumPacketTime)Settings::Get().Read(wxT("Server"), wxT("PacketTime"), 1000));
           // m_pRtpServer->SetupStream(wxT("PAM"), wxT("Info"), wxT("Description"));
            m_pRtpServer->Run();
        }

    }
    else
    {
        wmLog::Get()->Log(wxT("Output Disabled"));
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
    wmLog::Get()->Log(wxString::Format(wxT("Session Changed: %d"), m_setHandlers.size()));
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
    m_setRtpOrphan.erase(event.GetInt());
    m_mRtp.erase(event.GetInt());

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
