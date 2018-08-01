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

using namespace std;


DEFINE_EVENT_TYPE(wxEVT_SESSION)
DEFINE_EVENT_TYPE(wxEVT_PLAYBACK_CHANNELS)

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
    m_Session(session()),
    m_nMonitorSource(-1),
    m_nPlaybackSource(-1),
    m_bPlaybackInput(false),
    m_pGenerator(0)
{

    Settings::Get().AddHandler(wxT("Input"),wxT("Type"), this);
    Settings::Get().AddHandler(wxT("Input"),wxT("AoIP"), this);
    Settings::Get().AddHandler(wxT("Input"),wxT("Device"), this);
    Settings::Get().AddHandler(wxT("Input"),wxT("File"), this);


    Settings::Get().AddHandler(wxT("Output"),wxT("Device"), this);
    Settings::Get().AddHandler(wxT("Output"),wxT("Enabled"), this);
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

    Settings::Get().AddHandler(wxT("QoS"),wxT("Interval"), this);

    Connect(wxID_ANY,wxEVT_DATA,(wxObjectEventFunction)&IOManager::OnAudioEvent);
    Connect(wxID_ANY,wxEVT_RTP_SESSION,(wxObjectEventFunction)&IOManager::OnRTPSession);
    Connect(wxID_ANY,wxEVT_RTP_SESSION_CLOSED,(wxObjectEventFunction)&IOManager::OnRTPSessionClosed);
    Connect(wxID_ANY, wxEVT_SETTING_CHANGED, (wxObjectEventFunction)&IOManager::OnSettingEvent);

    Connect(wxID_ANY,wxEVT_QOS_UPDATED,(wxObjectEventFunction)&IOManager::OnQoS);

    m_pGenerator = new Generator();
    m_pGenerator->SetSampleRate(48000);
}


IOManager::~IOManager()
{
    Stop();
}

void IOManager::Stop()
{
    delete m_pGenerator;
    m_pGenerator = 0;

    for(map<wxString, RtpThread*>::iterator itThread = m_mRtp.begin(); itThread != m_mRtp.end(); ++itThread)
    {
        bool bDelete = m_setRtpOrphan.insert(itThread->first).second;
        if(bDelete)
        {
            itThread->second->Delete();
        }
    }
    m_mRtp.clear();
    SoundcardManager::Get().Terminate();
}


void IOManager::OnSettingEvent(SettingEvent& event)
{
    if(event.GetSection() == wxT("Input"))
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
        GeneratorNoiseChanged();
    }
    else if(event.GetSection() == wxT("QoS"))
    {
        if(event.GetKey() == wxT("Interval"))
        {
            map<wxString, RtpThread*>::iterator itThread = m_mRtp.find(m_sCurrentRtp);
            if(itThread != m_mRtp.end())
            {
                itThread->second->SetQosMeasurementIntervalMS(Settings::Get().Read(wxT("QoS"), wxT("Interval"), 1000));
            }
        }
    }
}

void IOManager::OnAudioEvent(AudioEvent& event)
{
    if(SoundcardManager::Get().IsOutputStreamOpen())
    {
        if(event.GetCreator() == m_nPlaybackSource)
        {
            SoundcardManager::Get().AddOutputSamples(event.GetBuffer());
        }
        else if(event.GetCreator() == AudioEvent::OUTPUT)
        {
            if(m_pGenerator && m_nPlaybackSource != m_nMonitorSource)
            {
                m_pGenerator->Generate(event.GetBuffer()->GetBufferSize());
            }
        }
    }

    if(event.GetCreator() == m_nMonitorSource)
    {
        for(set<wxEvtHandler*>::iterator itHandler = m_setHandlers.begin(); itHandler != m_setHandlers.end(); ++itHandler)
        {
            (*itHandler)->ProcessEvent(event);
        }
    }

    delete event.GetBuffer();
}


void IOManager::InputChanged(const wxString& sKey)
{
    if(sKey == wxT("AoIP"))
    {
        wxString sUrl = Settings::Get().Read(wxT("AoIP"), Settings::Get().Read(wxT("Input"), wxT("AoIP"), wxEmptyString), wxEmptyString);
        if(sUrl != m_sCurrentRtp)
        {
            wmLog::Get()->Log(wxT("Audio Input Device Changed: Close AoIP Session"));
            ClearSession();
            map<wxString, RtpThread*>::iterator itThread = m_mRtp.find(m_sCurrentRtp);
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
    //Stop the current monitoring...
    switch(m_nMonitorSource)
    {
        case AudioEvent::SOUNDCARD:
            OpenSoundcardDevice(SoundcardManager::Get().GetOutputSampleRate());    //this will remove the input stream
            break;
        case AudioEvent::RTP:
            map<wxString, RtpThread*>::iterator itThread = m_mRtp.find(m_sCurrentRtp);
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

void IOManager::OutputChanged(const wxString& sKey)
{
    if(sKey == wxT("Enabled"))
    {
        OpenSoundcardDevice(SoundcardManager::Get().GetOutputSampleRate());
    }
    else if(sKey == wxT("Source"))
    {
        wxString sType(Settings::Get().Read(wxT("Output"), wxT("Source"), wxT("Input")));
        m_bPlaybackInput = false;

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
            m_nPlaybackSource = m_nMonitorSource;
            m_bPlaybackInput = true;
            m_pGenerator->Stop();
        }
    }
    else if(sKey == wxT("Device"))
    {
        wmLog::Get()->Log(wxT("Soundcard output device changed"));
        OpenSoundcardDevice(SoundcardManager::Get().GetOutputSampleRate());
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

void IOManager::GeneratorNoiseChanged()
{
    if(Settings::Get().Read(wxT("Output"), wxT("Source"), wxT("Input")) == wxT("Noise"))
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
        }
        CheckPlayback(m_pGenerator->GetSampleRate(), m_pGenerator->GetChannels());

    }
}





void IOManager::OpenSoundcardDevice(unsigned long nOutputSampleRate)
{
    wmLog::Get()->Log(wxT("Opene Audio Device: Soundcard"));

    int nInput(-1);
    if(Settings::Get().Read(wxT("Input"), wxT("Type"), wxT("Soundcard")) == wxT("Soundcard"))
    {
        nInput = Settings::Get().Read(wxT("Input"), wxT("Device"), 0);
    }



    int nOutput(-1);
    if(Settings::Get().Read(wxT("Output"), wxT("Enabled"), 1) == 1)
    {
         nOutput = Settings::Get().Read(wxT("Output"), wxT("Device"), 0);
    }

    if(SoundcardManager::Get().Init(this, nInput, nOutput, nOutputSampleRate))
    {
        wmLog::Get()->Log(wxString::Format(wxT("Audio Device Created: Input [%d][%s] Latency %.2f"), SoundcardManager::Get().GetInputDevice(), SoundcardManager::Get().GetInputDeviceName().c_str(), SoundcardManager::Get().GetInputLatency()));
        wmLog::Get()->Log(wxString::Format(wxT("Audio Device Created: Output [%d][%s] Latency %.2f"), SoundcardManager::Get().GetOutputDevice(), SoundcardManager::Get().GetOutputDeviceName().c_str(), SoundcardManager::Get().GetOutputLatency()));
    }

}

void IOManager::InitAudioInputDevice()
{
    wxString sType(Settings::Get().Read(wxT("Input"), wxT("Type"), wxT("Soundcard")));

    if(sType == wxT("Soundcard"))
    {
        OpenSoundcardDevice(SoundcardManager::Get().GetOutputSampleRate());

        m_nMonitorSource = AudioEvent::SOUNDCARD;


        session aSession(wxEmptyString, wxT("Soundcard"), SoundcardManager::Get().GetInputDeviceName());
        aSession.lstSubsession.push_back(subsession(wxEmptyString, SoundcardManager::Get().GetInputDeviceName(), wxEmptyString, wxT("L24"), wxEmptyString, SoundcardManager::Get().GetInputDevice(), SoundcardManager::Get().GetInputSampleRate(), SoundcardManager::Get().GetInputNumberOfChannels(), wxEmptyString, 0, make_pair(0,0), refclk()));
        aSession.itCurrentSubsession = aSession.lstSubsession.begin();

        InputSession(aSession);

        CheckPlayback(SoundcardManager::Get().GetInputSampleRate(), SoundcardManager::Get().GetInputNumberOfChannels());
    }
    else if(sType == wxT("AoIP"))
    {
        m_nMonitorSource = AudioEvent::RTP;
        wmLog::Get()->Log(wxT("Create Audio Input Device: AoIP"));
        wxString sRtp(Settings::Get().Read(wxT("Input"), wxT("AoIP"), wxEmptyString));
        sRtp = Settings::Get().Read(wxT("AoIP"), sRtp, wxEmptyString);

        if(sRtp.empty() == false && m_mRtp.find(sRtp) == m_mRtp.end())
        {

            m_sCurrentRtp = sRtp;

            RtpThread* pThread = new RtpThread(this, wxT("pam"), sRtp, 2048);
            pThread->Create();
            pThread->Run();

            pThread->SetQosMeasurementIntervalMS(Settings::Get().Read(wxT("QoS"), wxT("Interval"), 1000));

            m_mRtp.insert(make_pair(m_sCurrentRtp, pThread));
        }
    }
    else if(sType == wxT("Output"))
    {
        m_nMonitorSource = AudioEvent::OUTPUT;
        wmLog::Get()->Log(wxT("Monitoring output"));

        CreateSessionFromOutput(wxEmptyString);
    }

    if(m_bPlaybackInput)
    {
        m_nPlaybackSource = m_nMonitorSource;
    }
}

void IOManager::CreateSessionFromOutput(const wxString& sSource)
{
    if(m_nMonitorSource == AudioEvent::OUTPUT)
    {
        session aSession(wxEmptyString, wxT("Output"), Settings::Get().Read(wxT("Output"), wxT("Source"), wxEmptyString));
        //we need to get the info from the output...
        unsigned int nSampleRate = SoundcardManager::Get().GetOutputSampleRate();

        aSession.lstSubsession.push_back(subsession(Settings::Get().Read(wxT("Output"), wxT("Source"),wxEmptyString), sSource, wxEmptyString, wxT("F32"), wxEmptyString, 0, nSampleRate, 2, wxEmptyString, 0, make_pair(0,0), refclk()));
        aSession.itCurrentSubsession = aSession.lstSubsession.begin();

        InputSession(aSession);
    }
}

void IOManager::InputSession(const session& aSession)
{
    m_Session = aSession;
    //tell our handler that we have changed the session....
    for(set<wxEvtHandler*>::iterator itHandler = m_setHandlers.begin(); itHandler != m_setHandlers.end(); ++itHandler)
    {
        wxCommandEvent event(wxEVT_SESSION);
        (*itHandler)->ProcessEvent(event);
    }


}


void IOManager::OnRTPSessionClosed(wxCommandEvent& event)
{
    wxString sTest = event.GetString();
    sTest.Replace(wxT("%20"), wxT(" "));

    if(m_sCurrentRtp == sTest)
    {
        m_sCurrentRtp = wxEmptyString;
    }
    m_setRtpOrphan.erase(sTest);
    m_mRtp.erase(sTest);

}

void IOManager::OnRTPSession(wxCommandEvent& event)
{
    m_Session = *reinterpret_cast<session*>(event.GetClientData());

    InputSession(m_Session);

    if(m_Session.itCurrentSubsession != m_Session.lstSubsession.end())
    {
        CheckPlayback(m_Session.itCurrentSubsession->nSampleRate, m_Session.itCurrentSubsession->nChannels);
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
        if(SoundcardManager::Get().IsOutputStreamOpen() && (SoundcardManager::Get().GetOutputSampleRate() != nSampleRate || SoundcardManager::Get().GetOutputNumberOfChannels() != nChannels))
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
    InputSession(session());
    CheckPlayback(48000,0);
}


const session& IOManager::GetSession()
{
    return m_Session;
}


void IOManager::Start()
{
    OutputChanged(wxT("Enabled"));
    OutputChanged(wxT("Source"));

    InitAudioInputDevice();
}

void IOManager::OutputChannelsChanged()
{
    unsigned int nInputChannels(0);
    if(m_Session.itCurrentSubsession != m_Session.lstSubsession.end())
    {
        nInputChannels = m_Session.itCurrentSubsession->nChannels;
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
    for(set<wxEvtHandler*>::iterator itHandler = m_setHandlers.begin(); itHandler != m_setHandlers.end(); ++itHandler)
    {
        wxCommandEvent eventUp(wxEVT_QOS_UPDATED);
        eventUp.SetClientData(event.GetClientData());
        (*itHandler)->ProcessEvent(eventUp);
    }
}
