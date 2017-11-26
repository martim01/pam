/***************************************************************
 * Name:      pam2Main.cpp
 * Purpose:   Code for Application Frame
 * Author:    Matthew Martin (martim01@outlook.com)
 * Created:   2017-11-01
 * Copyright: Matthew Martin ()
 * License:
 **************************************************************/

#include "pam2Main.h"
#include <wx/msgdlg.h>
#include "settings.h"
#include <map>
#include "monitorpluginfactory.h"
#include "monitorpluginbuilder.h"
#include <wx/log.h>
#include <wx/tokenzr.h>
#include "audio.h"
#include "playout.h"
#include "pnlSettings.h"
#include "pnlSettingsOptions.h"
#include "pnlSettingsPlugins.h"
#include "rtpthread.h"
#include "settingevent.h"
#include "pnlTests.h"
#include "monitorevent.h"
#include "pnlLog.h"
#include "PamUsageEnvironment.h"
#include "pnlLogControl.h"
#include "testpluginfactory.h"
#include "testpluginbuilder.h"
//(*InternalHeaders(pam2Dialog)
#include <wx/intl.h>
#include <wx/string.h>
//*)

//helper functions
enum wxbuildinfoformat {
    short_f, long_f };

wxString wxbuildinfo(wxbuildinfoformat format)
{
    wxString wxbuild(wxVERSION_STRING);

    if (format == long_f )
    {
#if defined(__WXMSW__)
        wxbuild << _T("-Windows");
#elif defined(__UNIX__)
        wxbuild << _T("-Linux");
#endif

#if wxUSE_UNICODE
        wxbuild << _T("-Unicode build");
#else
        wxbuild << _T("-ANSI build");
#endif // wxUSE_UNICODE
    }

    return wxbuild;
}

//(*IdInit(pam2Dialog)
const long pam2Dialog::ID_M_PSWP1 = wxNewId();
const long pam2Dialog::ID_M_PLST1 = wxNewId();
const long pam2Dialog::ID_M_PLST2 = wxNewId();
const long pam2Dialog::ID_M_PSWP2 = wxNewId();
const long pam2Dialog::ID_PANEL1 = wxNewId();
const long pam2Dialog::ID_PANEL2 = wxNewId();
//*)

using   namespace std;
BEGIN_EVENT_TABLE(pam2Dialog,wxDialog)
    //(*EventTable(pam2Dialog)
    //*)
END_EVENT_TABLE()

pam2Dialog::pam2Dialog(wxWindow* parent,wxWindowID id) :
    m_pAudio(0),
    m_pPlayback(0),
    m_pSelectedMonitor(0),
    m_ppnlLog(0)
{

    //(*Initialize(pam2Dialog)
    Create(parent, id, _("wxWidgets app"), wxDefaultPosition, wxDefaultSize, wxNO_BORDER, _T("id"));
    SetClientSize(wxSize(800,480));
    SetBackgroundColour(wxColour(0,0,0));
    m_pswpMain = new wmSwitcherPanel(this, ID_M_PSWP1, wxPoint(0,0), wxSize(600,480), wmSwitcherPanel::STYLE_NOSWIPE|wmSwitcherPanel::STYLE_NOANIMATION, _T("ID_M_PSWP1"));
    m_pswpMain->SetPageNameStyle(0);
    pnlLists = new wxPanel(this, ID_PANEL1, wxPoint(600,0), wxSize(200,480), wxTAB_TRAVERSAL, _T("ID_PANEL1"));
    m_plstScreens = new wmList(pnlLists, ID_M_PLST1, wxPoint(0,0), wxSize(200,136), wmList::STYLE_SELECT, 2, wxSize(-1,40), 3, wxSize(5,5));
    m_plstScreens->SetBackgroundColour(wxColour(0,0,0));
    m_plstScreens->SetButtonColour(wxColour(wxT("#008000")));
    m_plstScreens->SetSelectedButtonColour(wxColour(wxT("#FF8000")));
    m_plstOptions = new wmList(pnlLists, ID_M_PLST2, wxPoint(0,136), wxSize(200,132), wmList::STYLE_SELECT, 2, wxSize(-1,40), 3, wxSize(5,5));
    m_plstOptions->SetBackgroundColour(wxColour(0,0,0));
    m_plstOptions->SetButtonColour(wxColour(wxT("#000080")));
    m_plstOptions->SetSelectedButtonColour(wxColour(wxT("#FF8000")));
    m_pswpOptions = new wmSwitcherPanel(pnlLists, ID_M_PSWP2, wxPoint(0,270), wxSize(200,210), wmSwitcherPanel::STYLE_NOSWIPE|wmSwitcherPanel::STYLE_NOANIMATION, _T("ID_M_PSWP2"));
    m_pswpOptions->SetPageNameStyle(0);
    m_pswpOptions->SetBackgroundColour(wxColour(0,0,0));
    Panel1 = new wxPanel(m_pswpOptions, ID_PANEL2, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL, _T("ID_PANEL2"));
    Panel1->SetBackgroundColour(wxColour(0,0,0));
    m_pswpOptions->AddPage(Panel1, _("Blank"), false);

    Connect(ID_M_PLST1,wxEVT_LIST_SELECTED,(wxObjectEventFunction)&pam2Dialog::OnlstScreensSelected);
    Connect(ID_M_PLST2,wxEVT_LIST_SELECTED,(wxObjectEventFunction)&pam2Dialog::OnplstOptionsSelected);

    //*)

    m_plstScreens->SetFont(wxFont(10,wxFONTFAMILY_SWISS,wxFONTSTYLE_NORMAL,wxFONTWEIGHT_NORMAL,false,_T("Arial"),wxFONTENCODING_DEFAULT));
    m_plstOptions->SetFont(wxFont(10,wxFONTFAMILY_SWISS,wxFONTSTYLE_NORMAL,wxFONTWEIGHT_NORMAL,false,_T("Arial"),wxFONTENCODING_DEFAULT));

    pnlLists->SetBackgroundColour(*wxBLACK);

    m_plstScreens->SetBackgroundColour(*wxBLACK);
    m_plstOptions->SetBackgroundColour(*wxBLACK);

    Connect(wxID_ANY,wxEVT_DATA,(wxObjectEventFunction)&pam2Dialog::OnAudioData);
    Connect(wxID_ANY,wxEVT_RTP_SESSION,(wxObjectEventFunction)&pam2Dialog::OnRTPSession);
    Connect(wxID_ANY,wxEVT_RTP_SESSION_CLOSED,(wxObjectEventFunction)&pam2Dialog::OnRTPSessionClosed);
    Connect(wxID_ANY,wxEVT_QOS_UPDATED,(wxObjectEventFunction)&pam2Dialog::OnQoS);


    Pa_Initialize();


    Settings::Get().AddHandler(wxT("Input"),wxT("Type"), this);
    Settings::Get().AddHandler(wxT("Input"),wxT("RTP"), this);
    Settings::Get().AddHandler(wxT("Input"),wxT("Device"), this);

    Settings::Get().AddHandler(wxT("Output"),wxT("Device"), this);
    Settings::Get().AddHandler(wxT("Output"),wxT("Enabled"), this);
    Settings::Get().AddHandler(wxT("Output"),wxT("Buffer"), this);
    Settings::Get().AddHandler(wxT("Output"),wxT("Latency"), this);

    Connect(wxID_ANY, wxEVT_SETTING_CHANGED, (wxObjectEventFunction)&pam2Dialog::OnSettingChanged);
    Connect(wxID_ANY, wxEVT_MONITOR_REQUEST, (wxObjectEventFunction)&pam2Dialog::OnMonitorRequest);
    Connect(wxID_ANY,wxEVT_PAMUE,(wxObjectEventFunction)&pam2Dialog::OnLog);

    LoadMonitorPanels();

    m_pSession = 0;

    m_plstScreens->SelectButton(Settings::Get().Read(wxT("Main"), wxT("Monitor"), wxEmptyString));

    CreateAudioInputDevice();

    m_pPlayback = 0;
    m_pPlayback = new Playback();
    m_pPlayback->Init(this, 2048);
}

pam2Dialog::~pam2Dialog()
{
    //(*Destroy(pam2Dialog)
    //*)
    for(map<wxString, RtpThread*>::iterator itThread = m_mRtp.begin(); itThread != m_mRtp.end(); ++itThread)
    {
        bool bDelete = m_setRtpOrphan.insert(itThread->first).second;
        if(bDelete)
        {
            itThread->second->Delete();
        }
    }

    if(m_pPlayback)
    {
        m_pPlayback->ClosePlayback();
    //    m_pPlayback->Delete();
        delete m_pPlayback;
    }

    if(m_pSession)
    {
        delete m_pSession;
    }
    Pa_Terminate();
}

void pam2Dialog::OnQuit(wxCommandEvent& event)
{
    Close();
}

void pam2Dialog::OnAbout(wxCommandEvent& event)
{
    wxString msg = wxbuildinfo(long_f);
    wxMessageBox(msg, _("Welcome to..."));
}


void pam2Dialog::LoadMonitorPanels()
{

    MonitorPluginFactory::Get()->SetHandler(this);
    MonitorPluginFactory::Get()->SetSwitcherPanels(m_pswpMain, m_pswpOptions);


    Connect(wxID_ANY, wxEVT_MONITOR_MAX, (wxObjectEventFunction)&pam2Dialog::OnMonitorMax);

    map<wxString, wxString>::const_iterator itBegin, itEnd;
    if(Settings::Get().GetSectionDataBegin(wxT("Monitor Plugins"), itBegin) && Settings::Get().GetSectionDataEnd(wxT("Monitor Plugins"), itEnd))
    {
        for(; itBegin != itEnd; ++itBegin)
        {
            MonitorPluginFactory::Get()->LoadLibrary(itBegin->second);
        }
    }


    //Add the test and settings panels
    m_ppnlTests = new pnlTests(m_pswpMain);
    m_pswpMain->AddPage(m_ppnlTests, wxT("Tests"));
    m_ppnlTests->LoadTestPanels();

    m_ppnlLog = new pnlLog(m_pswpMain);
    m_pswpMain->AddPage(m_ppnlLog, wxT("Log"));



    m_ppnlSettings = new pnlSettings(m_pswpMain);
    m_pswpMain->AddPage(m_ppnlSettings, wxT("Settings"));


    pnlLogControl* pnlControl = new pnlLogControl(m_pswpOptions, m_ppnlLog);
    m_ppnlLog->SetLogControl(pnlControl);

    m_pswpOptions->AddPage(new pnlSettingsOptions(m_pswpOptions), wxT("Settings|Options"));
    m_pswpOptions->AddPage(pnlControl, wxT("Log|Control"));

    for(size_t i = 0; i < m_pswpMain->GetPageCount(); i++)
    {
        m_plstScreens->AddButton(m_pswpMain->GetPageText(i));
    }
}



void pam2Dialog::OnlstScreensSelected(wxCommandEvent& event)
{

    Settings::Get().Write(wxT("Main"), wxT("Monitor"), event.GetString());
    if(event.GetString() == wxT("Settings"))
    {
        m_pSelectedMonitor = 0;
        ShowSettingsPanel();
    }
    else if(event.GetString() == wxT("Log"))
    {
        m_pSelectedMonitor = 0;
        ShowLogPanel();
    }
    else if(event.GetString() == wxT("Tests"))
    {
        m_pSelectedMonitor = 0;
        ShowTestPanels();
    }
    else
    {
       ShowMonitorPanel(event.GetString());
    }
}

void pam2Dialog::OnplstOptionsSelected(wxCommandEvent& event)
{
    if(m_pswpMain->GetPage(m_pswpMain->GetSelection()) != m_ppnlTests)
    {
        Settings::Get().Write(m_pswpMain->GetSelectionName(), wxT("_Options"), event.GetInt());
        ShowOptionsPanel(event.GetString());
    }
    else
    {
        Settings::Get().Write(m_pswpMain->GetSelectionName(), wxT("_Panel"), event.GetInt());
        m_ppnlTests->ShowTestPanel(event.GetString());
    }
}

void pam2Dialog::ShowMonitorPanel(const wxString& sPanel)
{
    m_pswpMain->ChangeSelection(sPanel);


    m_plstOptions->Freeze();
    m_plstOptions->Clear();

    m_pswpOptions->ChangeSelection(wxT("Blank"));

    for(size_t i = 0; i < m_pswpOptions->GetPageCount(); i++)
    {
        if(m_pswpOptions->GetPageText(i).BeforeFirst(wxT('|')) == sPanel)
        {
            m_plstOptions->AddButton(m_pswpOptions->GetPageText(i).AfterFirst(wxT('|')));
        }
    }

    m_plstOptions->SelectButton(Settings::Get().Read(sPanel, wxT("_Options"), 0));
    m_plstOptions->Thaw();

    map<wxString, MonitorPluginBuilder*>::iterator itPlugin = MonitorPluginFactory::Get()->FindPlugin(m_pswpMain->GetSelectionName());
    if(itPlugin != MonitorPluginFactory::Get()->GetPluginEnd())
    {
        m_pSelectedMonitor = itPlugin->second;
        MaximizeMonitor(m_pSelectedMonitor->CanBeMaximized());
    }
    else
    {
        m_pSelectedMonitor = 0;
        MaximizeMonitor(false);
    }
}

void pam2Dialog::ShowSettingsPanel()
{
    m_pswpMain->ChangeSelection(wxT("Settings"));

    m_plstOptions->Freeze();
    m_plstOptions->Clear();

    m_plstOptions->AddButton(wxT("Audio Input"));
    m_plstOptions->AddButton(wxT("Audio Output"));
    m_plstOptions->AddButton(wxT("AoIP"));
    m_plstOptions->AddButton(wxT("Network"));
    m_plstOptions->AddButton(wxT("Plugins"));

    m_plstOptions->SelectButton(Settings::Get().Read(wxT("Settings"), wxT("_Options"), 0));
    m_plstOptions->Thaw();

    m_pswpOptions->ChangeSelection(wxT("Settings|Options"));
}


void pam2Dialog::ShowTestPanels()
{
    m_pswpMain->ChangeSelection(wxT("Tests"));

    m_plstOptions->Freeze();
    m_plstOptions->Clear();

    for(size_t i = 0; i < m_ppnlTests->m_pswpTests->GetPageCount(); i++)
    {
        m_plstOptions->AddButton(m_ppnlTests->m_pswpTests->GetPageText(i));
    }

    m_plstOptions->SelectButton(Settings::Get().Read(wxT("Tests"), wxT("_Panel"), 0));
    m_plstOptions->Thaw();

    m_pswpOptions->ChangeSelection(wxT("Settings|Options"));
}


void pam2Dialog::ShowLogPanel()
{
    m_pswpMain->ChangeSelection(wxT("Log"));

    m_plstOptions->Freeze();
    m_plstOptions->Clear();
    m_plstOptions->Thaw();

    m_pswpOptions->ChangeSelection(wxT("Log|Control"));
}


void pam2Dialog::ShowOptionsPanel(const wxString& sPanel)
{
    if(m_pswpMain->GetSelectionName() == wxT("Settings"))
    {
        m_ppnlSettings->m_pswpSettings->ChangeSelection(sPanel);
    }
    else if(m_pswpMain->GetSelectionName() == wxT("Tests"))
    {

    }
    else
    {
        wxString sOptionsPanel(wxString::Format(wxT("%s|%s"),m_pswpMain->GetSelectionName().c_str(), sPanel.c_str()));
        m_pswpOptions->ChangeSelection(sOptionsPanel);
    }
}


void pam2Dialog::OnMonitorMax(wxCommandEvent& event)
{
    MaximizeMonitor((event.GetInt()) == 1);

}

void pam2Dialog::MaximizeMonitor(bool bMax)
{
    if(!bMax)
    {
        m_pswpMain->SetSize(600,481);
        pnlLists->Show();
    }
    else
    {
        pnlLists->Hide();
        m_pswpMain->SetSize(800,481);
    }
}

void pam2Dialog::CreateAudioInputDevice()
{
    if(Settings::Get().Read(wxT("Input"), wxT("Type"), wxT("Soundcard")) == wxT("Soundcard"))
    {
        m_pAudio = new Audio(this, Settings::Get().Read(wxT("Input"), wxT("Device"), 0));
        m_pAudio->Init();

        //tell all the plugins what the input session is...
        session aSession(m_pAudio->GetDeviceName(), wxEmptyString, wxT("Soundcard"), wxT("Audio"), wxEmptyString, wxEmptyString, m_pAudio->GetDevice(), m_pAudio->GetSampleRate(), m_pAudio->GetNumberOfChannels(), 0, make_pair(0,0));

        InputSession(aSession);

        CheckPlayback(m_pAudio->GetSampleRate(), m_pAudio->GetNumberOfChannels());


    }
    else
    {
        wxString sRtp(Settings::Get().Read(wxT("Input"), wxT("RTP"), wxEmptyString));
        sRtp = Settings::Get().Read(wxT("RTP"), sRtp, wxEmptyString);
        if(sRtp.empty() == false && m_mRtp.find(sRtp) == m_mRtp.end())
        {
            m_sCurrentRtp = sRtp;
            RtpThread* pThread = new RtpThread(this, wxT("pam"), sRtp, 2048);
            pThread->Create();
            pThread->Run();
            m_mRtp.insert(make_pair(sRtp, pThread));
        }
    }
}

void pam2Dialog::InputSession(const session& aSession)
{
    for(map<wxString, MonitorPluginBuilder*>::iterator itMonitor = MonitorPluginFactory::Get()->GetPluginBegin(); itMonitor != MonitorPluginFactory::Get()->GetPluginEnd(); ++itMonitor)
    {
            itMonitor->second->InputSession(aSession);
    }

    for(map<wxString, TestPluginBuilder*>::iterator itTest = TestPluginFactory::Get()->GetPluginBegin(); itTest != TestPluginFactory::Get()->GetPluginEnd(); ++itTest)
    {
        itTest->second->InputSession(aSession);
    }

}

void pam2Dialog::OnAudioData(wxCommandEvent& event)
{
    //m_timerAES.Stop();

    timedbuffer* pTimedBuffer = reinterpret_cast<timedbuffer*>(event.GetClientData());

    if(m_pPlayback && m_pPlayback->IsStreamOpen())
    {
        if(event.GetId() != 1)  //1 =playback
        {
            m_pPlayback->AddSamples(pTimedBuffer);
            PassDataToPanels(pTimedBuffer);
        }
    }
    else
    {
        PassDataToPanels(pTimedBuffer);

    }
    delete pTimedBuffer;
}

void pam2Dialog::OnQoS(wxCommandEvent& event)
{
    qosData* pData = reinterpret_cast<qosData*>(event.GetClientData());
    if(m_pSelectedMonitor)
    {
        m_pSelectedMonitor->SetQoSData(pData);
    }
    delete pData;
}

void pam2Dialog::PassDataToPanels(timedbuffer* pTimedBuffer)
{
    if(m_pSelectedMonitor)
    {
        m_pSelectedMonitor->SetAudioData(pTimedBuffer);
    }
    else
    {
        m_ppnlTests->SetAudioData(pTimedBuffer);
    }
}


void pam2Dialog::OnSettingChanged(SettingEvent& event)
{
    if(event.GetSection() == wxT("Input"))
    {
        InputChanged(event.GetKey());
    }
    else if(event.GetSection() == wxT("Output"))
    {
        OutputChanged(event.GetKey());
    }
}


void pam2Dialog::InputChanged(const wxString& sKey)
{
    //has the type changed
    if(sKey == wxT("Type"))
    {
        if(Settings::Get().Read(wxT("Input"), wxT("Type"), wxT("Soundcard")) != wxT("Soundcard"))
        {
            if(m_pAudio)
            {
                delete m_pAudio;
                m_pAudio = 0;
            }
        }
        else
        {
            map<wxString, RtpThread*>::iterator itThread = m_mRtp.find(m_sCurrentRtp);
            if(itThread != m_mRtp.end())
            {
                bool bDelete = m_setRtpOrphan.insert(itThread->first).second;
                if(bDelete)
                {
                    itThread->second->Delete();
                }
            }
        }
    }
    else if(sKey == wxT("RTP") && Settings::Get().Read(wxT("Input"), wxT("RTP"), wxEmptyString) != m_sCurrentRtp)
    {
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
        CreateAudioInputDevice();
    }
    else if(sKey == wxT("Device"))
    {
        if(m_pAudio)
        {
            delete m_pAudio;
            m_pAudio = 0;
            CreateAudioInputDevice();
        }
    }
}

void pam2Dialog::OutputChanged(const wxString& sKey)
{
    if(Settings::Get().Read(wxT("Output"), wxT("Enabled"), int(1)) == 0)
    {
        if(m_pPlayback)
        {
            m_pPlayback->ClosePlayback();
        }
    }
    else
    {
        CreateAudioOutputDevice();
    }
}

void pam2Dialog::OnRTPSessionClosed(wxCommandEvent& event)
{
    if(m_sCurrentRtp == event.GetString())
    {
        m_sCurrentRtp = wxEmptyString;
    }
    //m_ptxtRtpLog->AppendText(wxString::Format(wxT("%s   AoIP Sink %s closed\n"), wxDateTime::UNow().Format(wxT("%H:%M:%S:%l")).c_str(), event.GetString().c_str()));
    m_setRtpOrphan.erase(event.GetString());
    m_mRtp.erase(event.GetString());
}

void pam2Dialog::OnRTPSession(wxCommandEvent& event)
{
    if(m_pSession)
    {
        delete m_pSession;
    }

    m_pSession = reinterpret_cast<session*>(event.GetClientData());

    InputSession(*m_pSession);


    CheckPlayback(m_pSession->nSampleRate, m_pSession->nChannels);


}


void pam2Dialog::CheckPlayback(unsigned long nSampleRate, unsigned long nChannels)
{
    //check the stream details against the playing details...
    if((m_pPlayback) && (m_pPlayback->GetSampleRate() != nSampleRate || m_pPlayback->GetChannels() != nChannels))
    {
        vector<char> vChannels;
        vChannels.push_back(Settings::Get().Read(wxT("Output"), wxT("Left"), 0));
        vChannels.push_back(Settings::Get().Read(wxT("Output"), wxT("Right"), 1));
        m_pPlayback->SetMixer(vChannels, nChannels);
        TellPluginsAboutOutputChannels();

        if(Settings::Get().Read(wxT("Output"), wxT("enabled"), int(1)) == 1)
        {
            m_pPlayback->ClosePlayback();
            CreateAudioOutputDevice(nSampleRate, 2);    // @todo should we allow multichannel cards here??
        }
    }

}


void pam2Dialog::CreateAudioOutputDevice(unsigned long nSampleRate, unsigned long nChannels)
{
    if(Settings::Get().Read(wxT("Output"), wxT("enabled"), int(1)) == 1)
    {
        if(nSampleRate == 0)
        {
            nSampleRate = m_pPlayback->GetSampleRate();
        }
        if(nChannels == 0)
        {
            nChannels = m_pPlayback->GetChannels();
        }
        if(m_pPlayback->OpenPlayback(Settings::Get().Read(wxT("Output"), wxT("Device"),0), nSampleRate, nChannels, Settings::Get().Read(wxT("Output"), wxT("Buffer"), (int)2048)) == false)
        {

        }
    }
}


void pam2Dialog::OnMonitorRequest(MonitorEvent& event)
{

    if(m_pPlayback && event.GetChannels().size() >=2)
    {
        unsigned int nInputChannels(0);
        if(m_pSession)
        {
            nInputChannels = m_pSession->nChannels;
        }
        m_pPlayback->SetMixer(event.GetChannels(), nInputChannels);
        //@todo make this multichannel
        Settings::Get().Write(wxT("Output"), wxT("Left"), event.GetChannels()[0]);
        Settings::Get().Write(wxT("Output"), wxT("Right"), event.GetChannels()[1]);

        TellPluginsAboutOutputChannels();
    }
}


void pam2Dialog::TellPluginsAboutOutputChannels()
{
    //now tell all the monitor panels etc...
    for(map<wxString, MonitorPluginBuilder*>::iterator itMonitor = MonitorPluginFactory::Get()->GetPluginBegin(); itMonitor != MonitorPluginFactory::Get()->GetPluginEnd(); ++itMonitor)
    {
        itMonitor->second->OutputChannels(m_pPlayback->GetOutputChannels());
    }

    for(map<wxString, TestPluginBuilder*>::iterator itTest = TestPluginFactory::Get()->GetPluginBegin(); itTest != TestPluginFactory::Get()->GetPluginEnd(); ++itTest)
    {
        itTest->second->OutputChannels(m_pPlayback->GetOutputChannels());
    }

}


void pam2Dialog::ClearSession()
{
    session aSession(wxEmptyString, wxEmptyString, wxEmptyString, wxT("Audio"), wxEmptyString, wxEmptyString, 0, 48000, 0, 0, make_pair(0,0));

    InputSession(aSession);

    CheckPlayback(48000,0);
}


void pam2Dialog::OnLog(wxCommandEvent& event)
{
    if(m_ppnlLog)
    {
        m_ppnlLog->Log(event.GetString());
    }
}
