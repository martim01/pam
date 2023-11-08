/***************************************************************
 * Name:      pam2Main.cpp
 * Purpose:   Code for Application Frame
 * Author:    Matthew Martin (martim01@outlook.com)
 * Created:   2017-11-01
 * Copyright: Matthew Martin ()
 * License:
 **************************************************************/

#include "pam2Main.h"
#include "pam2App.h"
#include <wx/msgdlg.h>
#include "settings.h"
#include <map>
#include "monitorpluginfactory.h"
#include "monitorpluginbuilder.h"
#include <wx/log.h>
#include <wx/tokenzr.h>
#include "audio.h"
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
#include "aoipsourcemanager.h"
#include <wx/stdpaths.h>
#include <wx/dynlib.h>
#include "soundfile.h"
#include "wxpammclient.h"
#include "images/splash.xpm"
#include "pnlHelp.h"
#include "generator.h"
#include "audioevent.h"
#include "soundcardmanager.h"
#include "pcstats.h"
#include "pam2_version.h"
#include "iomanager.h"
#include "pnlTestOptions.h"
#include "dlgPin.h"
#include "dlgNoInput.h"
#include "log.h"
#include <wx/bitmap.h>
#include <wx/dcscreen.h>
#include <wx/dcmemory.h>
#include <wx/dcclient.h>
#include "usbchecker.h"
#include "remoteapi.h"
#include "networkcontrol.h"
#include "generatorpluginfactory.h"

#ifdef __NMOS__
#include "nmos.h"
#endif // __WXMSW__

//(*InternalHeaders(pam2Dialog)
#include <wx/bitmap.h>
#include <wx/image.h>
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

std::set<wxString> GetTestNames()
{
    std::set<wxString> setNames;
    for(auto itTest = TestPluginFactory::Get()->GetPluginBegin(); itTest != TestPluginFactory::Get()->GetPluginEnd(); ++itTest)
    {
        setNames.insert(itTest->second->GetName());
    }
    return setNames;
}

std::map<int, wxString> GetAoipSources()
{
    return AoipSourceManager::Get().GetSourceNames(false);
}

std::set<wxString> GetInterfaceNames()
{
    return NetworkControl::Get().GetInterfaceNames();
}

std::set<wxString> GetWavFiles()
{
    wxArrayString asFiles;
    wxDir::GetAllFiles(Settings::Get().GetWavDirectory(), &asFiles, wxT("*.wav"), wxDIR_FILES);

    std::set<wxString> setFiles;
    for(size_t i = 0; i < asFiles.GetCount(); i++)
    {
        wxFileName fn(asFiles[i]);
        setFiles.insert(fn.GetName());
    }
    return setFiles;
}

std::set<wxString> GetSequences()
{
    wxArrayString asFiles;
    wxDir::GetAllFiles(wxString::Format(wxT("%s/generator"), Settings::Get().GetDocumentDirectory().c_str()), &asFiles, wxT("*.xml"), wxDIR_FILES);
    std::set<wxString> setFiles;

    for(size_t i = 0; i < asFiles.GetCount(); i++)
    {
        wxFileName fn(asFiles[i]);
        setFiles.insert(fn.GetName());
    }
    return setFiles;
}

std::set<wxString> GetOutputSources()
{
    std::set<wxString> setSources{"Input", "File", "Sequence", "Generator", "Noise"};

    for(auto itPlugin = GeneratorPluginFactory::Get()->GetPluginBegin(); itPlugin != GeneratorPluginFactory::Get()->GetPluginEnd(); ++itPlugin)
    {
        setSources.insert(itPlugin->first);
    }
    return setSources;
}


//(*IdInit(pam2Dialog)
const long pam2Dialog::ID_BITMAPBUTTON1 = wxNewId();
const long pam2Dialog::ID_PANEL4 = wxNewId();
const long pam2Dialog::ID_M_PSWP1 = wxNewId();
const long pam2Dialog::ID_M_PLST1 = wxNewId();
const long pam2Dialog::ID_M_PLST3 = wxNewId();
const long pam2Dialog::ID_M_PBTN2 = wxNewId();
const long pam2Dialog::ID_M_PBTN4 = wxNewId();
const long pam2Dialog::ID_M_PBTN3 = wxNewId();
const long pam2Dialog::ID_M_PLBL2 = wxNewId();
const long pam2Dialog::ID_PANEL7 = wxNewId();
const long pam2Dialog::ID_M_PBTN1 = wxNewId();
const long pam2Dialog::ID_PANEL5 = wxNewId();
const long pam2Dialog::ID_M_PLST2 = wxNewId();
const long pam2Dialog::ID_PANEL2 = wxNewId();
const long pam2Dialog::ID_M_PSWP2 = wxNewId();
const long pam2Dialog::ID_PANEL6 = wxNewId();
const long pam2Dialog::ID_M_PSWP4 = wxNewId();
const long pam2Dialog::ID_PANEL1 = wxNewId();
const long pam2Dialog::ID_PANEL3 = wxNewId();
const long pam2Dialog::ID_PANEL8 = wxNewId();
const long pam2Dialog::ID_PANEL9 = wxNewId();
const long pam2Dialog::ID_M_PSWP3 = wxNewId();
const long pam2Dialog::ID_TIMER1 = wxNewId();
const long pam2Dialog::ID_TIMER2 = wxNewId();
const long pam2Dialog::ID_TIMER3 = wxNewId();
//*)

using   namespace std;
BEGIN_EVENT_TABLE(pam2Dialog,wxDialog)
    //(*EventTable(pam2Dialog)
    //*)
END_EVENT_TABLE()


pam2Dialog::pam2Dialog(wxWindow* parent,wxWindowID id) :
    m_ppnlSettings(0),
    m_ppnlTests(0),
    m_ppnlHelp(0),
    m_ppnlLog(0),
    m_pdlgNoInput(0),
    m_pSelectedMonitor(0),
    m_bInputFailed(false),
    m_usb(this)
{

    //(*Initialize(pam2Dialog)
    Create(parent, id, _("wxWidgets app"), wxDefaultPosition, wxDefaultSize, wxNO_BORDER, _T("id"));
    SetClientSize(wxSize(800,480));
    Move(wxPoint(0,0));
    SetBackgroundColour(wxColour(0,0,0));
    m_pswpSplash = new wmSwitcherPanel(this, ID_M_PSWP3, wxDefaultPosition, wxSize(800,480), wmSwitcherPanel::STYLE_NOSWIPE|wmSwitcherPanel::STYLE_NOANIMATION, _T("ID_M_PSWP3"));
    m_pswpSplash->SetPageNameStyle(0);
    pnlSplash = new wxPanel(m_pswpSplash, ID_PANEL4, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL, _T("ID_PANEL4"));
    pnlSplash->SetBackgroundColour(wxColour(0,0,0));
    m_pbmpSplash = new wxBitmapButton(pnlSplash, ID_BITMAPBUTTON1, wxBitmap(splash_xpm), wxPoint(0,0), wxSize(800,480), wxNO_BORDER, wxDefaultValidator, _T("ID_BITMAPBUTTON1"));
    m_pbmpSplash->SetBackgroundColour(wxColour(0,0,0));
    pnlMain = new wxPanel(m_pswpSplash, ID_PANEL3, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL, _T("ID_PANEL3"));
    pnlMain->SetBackgroundColour(wxColour(0,0,0));
    m_pswpMain = new wmSwitcherPanel(pnlMain, ID_M_PSWP1, wxPoint(0,0), wxSize(600,480), wmSwitcherPanel::STYLE_NOSWIPE|wmSwitcherPanel::STYLE_NOANIMATION, _T("ID_M_PSWP1"));
    m_pswpMain->SetPageNameStyle(0);
    pnlLists = new wxPanel(pnlMain, ID_PANEL1, wxPoint(600,0), wxSize(200,480), wxTAB_TRAVERSAL, _T("ID_PANEL1"));
    pnlLists->SetBackgroundColour(wxColour(0,0,0));
    m_pswpScreens = new wmSwitcherPanel(pnlLists, ID_M_PSWP4, wxDefaultPosition, wxSize(200,480), wmSwitcherPanel::STYLE_NOSWIPE|wmSwitcherPanel::STYLE_NOANIMATION, _T("ID_M_PSWP4"));
    m_pswpScreens->SetPageNameStyle(3);
    Panel2 = new wxPanel(m_pswpScreens, ID_PANEL5, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL, _T("ID_PANEL5"));
    Panel2->SetBackgroundColour(wxColour(0,0,0));
    m_plstScreens = new wmList(Panel2, ID_M_PLST1, wxPoint(0,5), wxSize(200,290), wmList::STYLE_SELECT, 2, wxSize(-1,40), 3, wxSize(5,5));
    m_plstScreens->SetBackgroundColour(wxColour(0,0,0));
    m_plstScreens->SetButtonColour(wxColour(wxT("#008000")));
    m_plstScreens->SetSelectedButtonColour(wxColour(wxT("#FF8000")));
    m_plstInbuilt = new wmList(Panel2, ID_M_PLST3, wxPoint(0,295), wxSize(200,44), wmList::STYLE_SELECT, 0, wxSize(-1,40), 3, wxSize(2,2));
    m_plstInbuilt->SetForegroundColour(wxColour(0,0,0));
    m_plstInbuilt->SetBackgroundColour(wxColour(0,0,0));
    m_plstInbuilt->SetButtonColour(wxColour(wxT("#3DBEAB")));
    m_plstInbuilt->SetSelectedButtonColour(wxColour(wxT("#FF8000")));
    m_plstInbuilt->SetTextButtonColour(wxColour(wxT("#000000")));
    Panel4 = new wxPanel(Panel2, ID_PANEL7, wxPoint(4,390), wxSize(193,50), wxTAB_TRAVERSAL, _T("ID_PANEL7"));
    Panel4->SetBackgroundColour(wxColour(255,255,255));
    m_pbtnCPU = new wmButton(Panel4, ID_M_PBTN2, _("Monitor"), wxPoint(1,1), wxSize(95,23), wmButton::STYLE_SELECT, wxDefaultValidator, _T("ID_M_PBTN2"));
    m_pbtnCPU->SetBackgroundColour(wxColour(0,0,0));
    m_pbtnScreenshot = new wmButton(Panel4, ID_M_PBTN4, _("Screenshot"), wxPoint(97,1), wxSize(95,23), wmButton::STYLE_NORMAL, wxDefaultValidator, _T("ID_M_PBTN4"));
    m_pbtnScreenshot->SetBackgroundColour(wxColour(0,0,108));
    m_pbtnInput = new wmButton(Panel4, ID_M_PBTN3, _("Input"), wxPoint(1,25), wxSize(95,24), wmButton::STYLE_SELECT, wxDefaultValidator, _T("ID_M_PBTN3"));
    m_pbtnInput->SetBackgroundColour(wxColour(0,128,0));
    m_plblOutput = new wmLabel(Panel4, ID_M_PLBL2, _("Output"), wxPoint(97,25), wxSize(95,24), 0, _T("ID_M_PLBL2"));
    m_plblOutput->SetBorderState(uiRect::BORDER_NONE);
    m_plblOutput->GetUiRect().SetGradient(0);
    m_plblOutput->SetForegroundColour(wxColour(255,255,255));
    m_plblOutput->SetBackgroundColour(wxColour(0,128,0));
    m_pbtnMonitor = new wmButton(Panel2, ID_M_PBTN1, _("Monitor"), wxPoint(5,345), wxSize(190,37), wmButton::STYLE_SELECT, wxDefaultValidator, _T("ID_M_PBTN1"));
    m_pbtnMonitor->SetBackgroundColour(wxColour(128,0,128));
    m_pbtnMonitor->SetColourDisabled(wxColour(wxT("#909090")));
    Panel3 = new wxPanel(m_pswpScreens, ID_PANEL6, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL, _T("ID_PANEL6"));
    Panel3->SetBackgroundColour(wxColour(0,0,0));
    m_plstOptions = new wmList(Panel3, ID_M_PLST2, wxPoint(0,5), wxSize(200,200), wmList::STYLE_SELECT, 2, wxSize(-1,40), 3, wxSize(5,5));
    m_plstOptions->SetBackgroundColour(wxColour(0,0,0));
    m_plstOptions->SetButtonColour(wxColour(wxT("#000080")));
    m_plstOptions->SetSelectedButtonColour(wxColour(wxT("#FF8000")));
    m_pswpOptions = new wmSwitcherPanel(Panel3, ID_M_PSWP2, wxPoint(0,210), wxSize(200,230), wmSwitcherPanel::STYLE_NOSWIPE|wmSwitcherPanel::STYLE_NOANIMATION, _T("ID_M_PSWP2"));
    m_pswpOptions->SetPageNameStyle(0);
    m_pswpOptions->SetBackgroundColour(wxColour(0,0,0));
    Panel1 = new wxPanel(m_pswpOptions, ID_PANEL2, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL, _T("ID_PANEL2"));
    Panel1->SetBackgroundColour(wxColour(0,0,0));
    m_pswpOptions->AddPage(Panel1, _("Blank"), false);
    m_pswpScreens->AddPage(Panel2, _("Screens"), false);
    m_pswpScreens->AddPage(Panel3, _("Options"), false);
    pnlAoip = new pnlAoIPSelection(m_pswpSplash, ID_PANEL8, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL, _T("ID_PANEL8"));
    pnlRTCP = new pnlRTCPTransmission(m_pswpSplash, ID_PANEL9, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL, _T("ID_PANEL9"));
    m_pswpSplash->AddPage(pnlSplash, _("Splash"), true);
    m_pswpSplash->AddPage(pnlMain, _("Main"), false);
    m_pswpSplash->AddPage(pnlAoip, _("AoIP"), false);
    m_pswpSplash->AddPage(pnlRTCP, _("RTCP"), false);
    timerStart.SetOwner(this, ID_TIMER1);
    timerStart.Start(10, true);
    m_timerFile.SetOwner(this, ID_TIMER2);
    m_timerIpc.SetOwner(this, ID_TIMER3);
    m_timerIpc.Start(1000, false);

    Connect(ID_BITMAPBUTTON1,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&pam2Dialog::OnbmpSplashClick);
    Connect(ID_M_PSWP1,wxEVT_COMMAND_NOTEBOOK_PAGE_CHANGED,(wxObjectEventFunction)&pam2Dialog::OnswpMainPageChanged);
    Connect(ID_M_PLST1,wxEVT_LIST_SELECTED,(wxObjectEventFunction)&pam2Dialog::OnlstScreensSelected);
    Connect(ID_M_PLST3,wxEVT_LIST_SELECTED,(wxObjectEventFunction)&pam2Dialog::OnlstScreensSelected);
    Connect(ID_M_PBTN2,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&pam2Dialog::OnbtnInputClick);
    Connect(ID_M_PBTN4,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&pam2Dialog::OnbtnScreenshotClick);
    Connect(ID_M_PBTN3,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&pam2Dialog::OnbtnInputClick);
    Connect(ID_M_PBTN1,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&pam2Dialog::OnbtnMonitorClick);
    Connect(ID_M_PLST2,wxEVT_LIST_SELECTED,(wxObjectEventFunction)&pam2Dialog::OnplstOptionsSelected);
    Connect(ID_M_PSWP3,wxEVT_COMMAND_NOTEBOOK_PAGE_CHANGED,(wxObjectEventFunction)&pam2Dialog::OnswpSplashPageChanged);
    Connect(ID_TIMER1,wxEVT_TIMER,(wxObjectEventFunction)&pam2Dialog::OntimerStartTrigger);
    Connect(ID_TIMER2,wxEVT_TIMER,(wxObjectEventFunction)&pam2Dialog::Onm_timerFileTrigger);
    Connect(ID_TIMER3,wxEVT_TIMER,(wxObjectEventFunction)&pam2Dialog::OntimerIpcTrigger);
    Connect(wxID_ANY,wxEVT_CLOSE_WINDOW,(wxObjectEventFunction)&pam2Dialog::OnClose);
    //*)

    m_pbtnScreenshot->SetColourSelected(wxColour(255,128,0));
    m_timerScreenshot.SetOwner(this, wxNewId());


    m_pdlgNoInput = 0;

    m_ppnlLog = new pnlLog(m_pswpMain);

    pml::LogStream::AddOutput(std::make_unique<pml::LogOutput>());

    pml::LogStream::SetOutputLevel(pml::LOG_DEBUG);

    wxImage img(16,16);
    img.SetRGB(wxRect(0,0,16,16),0,0,0);
    img.SetMaskColour(0,0,0);
    img.SetOption(wxIMAGE_OPTION_CUR_HOTSPOT_X,1);
    img.SetOption(wxIMAGE_OPTION_CUR_HOTSPOT_Y,1);
    m_pCursor = new wxCursor(img);

    wxSetCursor(*wxSTANDARD_CURSOR);
    if(Settings::Get().Read("General", "Cursor", 1) == 0)
    {
        wxSetCursor(*m_pCursor);
    }

    //m_pswpScreens->SetEventHandler(this);

    m_pbtnMonitor->SetToggle(true, wxT("Input"), wxT("Output"), 45);
    m_pbtnMonitor->ConnectToSetting("Monitor", "Source", false);
    m_pbtnMonitor->Enable((Settings::Get().Read(wxT("Output"), wxT("Destination"),wxT("Disabled")) != wxT("Disabled")));

    m_plstScreens->SetFont(wxFont(10,wxFONTFAMILY_SWISS,wxFONTSTYLE_NORMAL,wxFONTWEIGHT_NORMAL,false,_T("Arial"),wxFONTENCODING_DEFAULT));
    m_plstScreens->SetBackgroundColour(*wxBLACK);
    m_plstOptions->SetFont(wxFont(10,wxFONTFAMILY_SWISS,wxFONTSTYLE_NORMAL,wxFONTWEIGHT_NORMAL,false,_T("Arial"),wxFONTENCODING_DEFAULT));
    m_plstOptions->SetBackgroundColour(*wxBLACK);

    pnlLists->SetBackgroundColour(*wxBLACK);

    Settings::Get().Write("Splash", "Screen", "Splash");
    Settings::Get().Write(wxT("Version"), wxT("pam2"), pml::pam2::VERSION_STRING);
    Settings::Get().Write(wxT("Input"), wxT("Reset"), false);

    Settings::Get().AddHandler(this, "Main");
    Settings::Get().AddHandler(this, wxT("Input"),wxT("Type"));
    Settings::Get().AddHandler(this, wxT("Output"),wxT("Destination"));
    Settings::Get().AddHandler(this, wxT("Output"),wxT("Source"));
    Settings::Get().AddHandler(this, wxT("AoIP"),wxT("Epoch"));
    Settings::Get().AddHandler(this, wxT("Test"), wxT("Lock"));
    Settings::Get().AddHandler(this, wxT("Server"), wxT("DestinationIp"));
    Settings::Get().AddHandler(this, wxT("Server"), wxT("RTP_Port"));
    Settings::Get().AddHandler(this, wxT("Server"), wxT("PacketTime"));
    Settings::Get().AddHandler(this, wxT("Server"), wxT("Stream"));
    Settings::Get().AddHandler(this, "Splash", "Screen");
    Settings::Get().AddHandler(this, "General", "Cursor");


    IOManager::Get().RegisterHandler(this);


    Connect(wxID_ANY, wxEVT_SETTING_CHANGED, (wxObjectEventFunction)&pam2Dialog::OnSettingChanged);
    Connect(wxID_ANY, wxEVT_MONITOR_REQUEST, (wxObjectEventFunction)&pam2Dialog::OnMonitorRequest);
    Connect(wxID_ANY, wxEVT_PLUGINS_APPLY, (wxObjectEventFunction)&pam2Dialog::OnPluginsReload);
    Connect(wxID_ANY, wxEVT_HELP_CLOSE, (wxObjectEventFunction)&pam2Dialog::OnHelpClose);
    Connect(ID_M_PSWP4,wxEVT_COMMAND_NOTEBOOK_PAGE_CHANGED,(wxObjectEventFunction)&pam2Dialog::OnswpScreensPageChanged);
    Connect(m_timerScreenshot.GetId(),wxEVT_TIMER,(wxObjectEventFunction)&pam2Dialog::OnTimerScreenshot);
    Connect(wxID_ANY,wxEVT_QOS_UPDATED,(wxObjectEventFunction)&pam2Dialog::OnQoS);
    Connect(wxID_ANY,wxEVT_DATA,(wxObjectEventFunction)&pam2Dialog::OnAudioData);
    Connect(wxID_ANY,wxEVT_INPUT_FAILED,(wxObjectEventFunction)&pam2Dialog::OnInputFailed);
    Connect(wxID_ANY,wxEVT_SESSION,(wxObjectEventFunction)&pam2Dialog::OnSession);
    Connect(wxID_ANY,wxEVT_PLAYBACK_CHANNELS,(wxObjectEventFunction)&pam2Dialog::OnPlaybackChannels);

    m_pbtnInput->SetLabel(Settings::Get().Read(wxT("Input"), wxT("Type"), wxT("Soundcard")));
    m_plblOutput->ConnectToSetting("Output", "Source", "Input");
    m_plblOutput->Show(Settings::Get().Read(wxT("Output"), wxT("Destination"),wxT("Disabled"))!=wxT("Disabled"));


    RegisterRemoteApiSettings();

}

pam2Dialog::~pam2Dialog()
{
    //(*Destroy(pam2Dialog)
    //*)
    IOManager::Get().Stop();
    if(m_pdlgNoInput)
    {
        delete m_pdlgNoInput;
    }
    delete m_pCursor;

    Settings::Get().RemoveHandler(this);
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

void pam2Dialog::OnPluginsReload(wxCommandEvent& event)
{
    wxExecute(wxStandardPaths::Get().GetExecutablePath());
    Close();
}

void pam2Dialog::LoadMonitorPanels()
{
    MonitorPluginFactory::Get()->SetHandler(this);
    MonitorPluginFactory::Get()->SetSwitcherPanels(m_pswpMain, m_pswpOptions);

    Connect(wxID_ANY, wxEVT_MONITOR_MAX, (wxObjectEventFunction)&pam2Dialog::OnMonitorMax);

    map<wxString, wxString>::const_iterator itBegin, itEnd;
    std::vector<wxString> vPanels;
    if(Settings::Get().GetSectionDataBegin(wxT("Monitor Plugins"), itBegin) && Settings::Get().GetSectionDataEnd(wxT("Monitor Plugins"), itEnd))
    {
        for(; itBegin != itEnd; ++itBegin)
        {
            MonitorPluginFactory::Get()->LoadLibrary(itBegin->second);
            vPanels.push_back(itBegin->second);
        }
    }

    //write the settings back so we clear any issues with numbering
    Settings::Get().RemoveSection("Monitor Plugins");
    for(unsigned long i =0; i < vPanels.size(); i++)
    {
        Settings::Get().Write("Monitor Plugins", wxString::Format("%04lu", i), vPanels[i]);
    }


    //now store the plugins that always want audio....
    std::set<wxString> setMon{"Tests"};  //and also for remoteapi
    for(map<wxString, MonitorPluginBuilder*>::iterator itPlugin = MonitorPluginFactory::Get()->GetPluginBegin(); itPlugin != MonitorPluginFactory::Get()->GetPluginEnd(); ++itPlugin)
    {
        if(itPlugin->second->WantsAudioAlways())
        {
            m_setAlwaysPassAudio.insert(itPlugin->second);
        }
        if(itPlugin->second->WantsQoSAlways())
        {
            m_setAlwaysPassQoS.insert(itPlugin->second);
        }
        setMon.insert(itPlugin->first);
    }

    RemoteApi::Get().RegisterRemoteApiEnum("Main", "Screen", setMon, "Settings");

    //Add the test and settings panels
    m_ppnlTests = new pnlTests(m_pswpMain);

    m_ppnlTests->LoadTestPanels();

    if(m_ppnlTests->m_pswpTests->GetPageCount() > 0)
    {
        m_pswpMain->AddPage(m_ppnlTests, "Tests");
        RemoteApi::Get().RegisterRemoteApiEnum("Main", "Test", GetTestNames(), "Record");
    }
    else
    {
        delete m_ppnlTests;
        m_ppnlTests = 0;
    }

    size_t nPage = 0;
    for(size_t i = 0; i < m_pswpMain->GetPageCount(); i++)
    {
        if(m_pswpMain->GetPageCount() > 24)
        {
            if(i!=0 && i%24 ==0)
            {
                nPage++;
            }
        }
        m_mmMonitorPlugins.insert(make_pair(nPage, m_pswpMain->GetPageText(i)));
    }
    m_nCurrentMonitorPage = 0;


    m_pswpMain->AddPage(m_ppnlLog, wxT("Log"));

    m_ppnlSettings = new pnlSettings(m_pswpMain);
    m_pswpMain->AddPage(m_ppnlSettings, wxT("Settings"));

    m_ppnlHelp =  new pnlHelp(m_pswpMain);
    m_pswpMain->AddPage(m_ppnlHelp, wxT("Help"));

    pnlLogControl* pnlControl = new pnlLogControl(m_pswpOptions, m_ppnlLog);
    m_ppnlLog->SetLogControl(pnlControl);

    m_pswpOptions->AddPage(new pnlSettingsOptions(m_pswpOptions), wxT("Settings|Options"));
    m_pswpOptions->AddPage(pnlControl, wxT("Log|Control"));
    m_pswpOptions->AddPage(new pnlTestOptions(m_pswpOptions), wxT("Tests|Options"));

    m_plstInbuilt->AddButton(wxT("Log"));
    m_plstInbuilt->AddButton(wxT("Settings"));
    m_plstInbuilt->AddButton(wxT("Help"));


    ShowMonitorList();
}

void pam2Dialog::ShowMonitorList()
{
    m_plstScreens->Freeze();
    m_plstScreens->Clear();

    size_t nButtons(0);
    for(multimap<size_t, wxString>::const_iterator itText = m_mmMonitorPlugins.lower_bound(m_nCurrentMonitorPage); itText != m_mmMonitorPlugins.upper_bound(m_nCurrentMonitorPage);++itText)
    {
        m_plstScreens->AddButton(itText->second);
        ++nButtons;
    }

    size_t nNext = m_nCurrentMonitorPage+1;
    size_t nLast = m_nCurrentMonitorPage-1;
    if(m_mmMonitorPlugins.lower_bound(nNext) != m_mmMonitorPlugins.upper_bound(nNext))
    {
        size_t nButton = m_plstScreens->AddButton(wxT("Next Page"));
        m_plstScreens->SetButtonColour(nButton, wxColour(50,160,70));
        m_plstScreens->SetTextButtonColour(nButton, *wxBLACK);
    }
    else if(m_mmMonitorPlugins.lower_bound(nLast) != m_mmMonitorPlugins.upper_bound(nLast))
    {
        for(; nButtons < 8; ++nButtons)
        {
            m_plstScreens->AddButton(wxEmptyString, wxNullBitmap, NULL, wmList::wmDISABLED);

        }
        size_t nButton = m_plstScreens->AddButton(wxT("Prev Page"));
        m_plstScreens->SetButtonColour(nButton, wxColour(50,160,70));
        m_plstScreens->SetTextButtonColour(nButton, *wxBLACK);
    }

    m_plstScreens->SelectButton(m_pswpMain->GetSelectionName(), false);
    m_plstScreens->Thaw();
}



void pam2Dialog::OnlstScreensSelected(wxCommandEvent& event)
{
    if(event.GetString() == wxT("Next Page"))
    {
        m_nCurrentMonitorPage++;
        ShowMonitorList();
    }
    else if(event.GetString() == wxT("Prev Page"))
    {
        m_nCurrentMonitorPage--;
        ShowMonitorList();
    }
    else if(event.GetString() == wxT("Tests"))
    {
        Settings::Get().Write(wxT("Main"), "Screen", event.GetString());
        Settings::Get().Write(wxT("Main"), wxT("Last"),event.GetString());
    }
    else if(event.GetString() == wxT("Settings"))
    {
        Settings::Get().Write(wxT("Main"), "Screen", event.GetString());
    }
    else if(event.GetString() == wxT("Help"))
    {
        Settings::Get().Write(wxT("Main"), "Screen", event.GetString());
    }
    else if(event.GetString() == wxT("Log"))
    {
        Settings::Get().Write(wxT("Main"), "Screen", event.GetString());
    }
    else
    {
        Settings::Get().Write(wxT("Main"), "Screen", event.GetString());
        Settings::Get().Write(wxT("Main"), wxT("Last"),event.GetString());
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
        Settings::Get().Write("Main", "Test", event.GetString());

    }
}

void pam2Dialog::ShowMonitorPanel(const wxString& sPanel)
{
    m_pswpMain->ChangeSelection(sPanel);

    if(Settings::Get().Read(wxT("General"), wxT("ShowOptions"),0) == 1)
    {
        m_pswpScreens->ChangeSelection(wxT("Options"));
    }


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
        m_pSelectedMonitor = nullptr;
        MaximizeMonitor(false);
    }
}

void pam2Dialog::ShowSettingsPanel()
{
    if(Settings::Get().Read(wxT("General"), wxT("Pin"),0) == 1)
    {
        dlgPin aDlg(this);
        if(aDlg.ShowModal() == wxID_CANCEL)
        {
            return;
        }
    }


    m_pswpMain->ChangeSelection(wxT("Settings"));

    m_plstOptions->Freeze();
    m_plstOptions->Clear();

    m_plstOptions->AddButton(wxT("Input Device"));
    m_plstOptions->AddButton(wxT("Output Device"));
    m_plstOptions->AddButton(wxT("Output Source"));
    m_plstOptions->AddButton(wxT("Network"));
    m_plstOptions->AddButton(wxT("Time"));
    #ifdef __NMOS__
    m_plstOptions->AddButton(wxT("NMOS"));
    #endif // __NMOS__
    m_plstOptions->AddButton(wxT("Plugins"));

    m_plstOptions->AddButton(wxT("Profiles"));
    m_plstOptions->AddButton(wxT("General"));
    m_plstOptions->AddButton(wxT("Remote API"));
    m_plstOptions->AddButton(wxT("Update"));
    m_plstOptions->AddButton(wxT("About"));




    m_plstOptions->SelectButton(Settings::Get().Read(wxT("Settings"), wxT("_Options"), 0));
    m_plstOptions->Thaw();

    m_pswpOptions->ChangeSelection(wxT("Settings|Options"));
    m_pswpScreens->ChangeSelection(wxT("Options"));
}


void pam2Dialog::ShowTestPanels()
{
    m_pswpMain->ChangeSelection(wxT("Tests"));
    MaximizeMonitor(false); //make sure we can see the options panels;

    m_plstOptions->Freeze();
    m_plstOptions->Clear();

    for(size_t i = 0; i < m_ppnlTests->m_pswpTests->GetPageCount(); i++)
    {
        m_plstOptions->AddButton(m_ppnlTests->m_pswpTests->GetPageText(i));
    }

    m_plstOptions->SelectButton(Settings::Get().Read("Main", "Test", ""));
    m_plstOptions->Thaw();

    m_pswpOptions->ChangeSelection(wxT("Tests|Options"));
    m_pswpScreens->ChangeSelection(wxT("Options"));
}


void pam2Dialog::ShowLogPanel(bool bTests)
{


    m_pswpMain->ChangeSelection(wxT("Log"));

    if(!bTests)
    {
        m_plstOptions->Freeze();
        m_plstOptions->Clear();
        m_plstOptions->Thaw();

        m_pswpOptions->ChangeSelection(wxT("Log|Control"));

        m_pswpScreens->ChangeSelection(wxT("Options"));
    }
}

void pam2Dialog::ShowHelpPanel()
{
    wxString sHelp = Settings::Get().Read("Main", "Screen", wxEmptyString);
    if(sHelp == wxT("Tests"))
    {
        sHelp = Settings::Get().Read("Main", "Test", "");
    }
    else if(sHelp == wxT("Log") || sHelp == wxT("Settings") || sHelp == wxEmptyString)
    {
        sHelp = wxT("Pam");
    }


    m_ppnlHelp->SelectHelp(sHelp);
    m_pswpMain->ChangeSelection(wxT("Help"));

    m_plstOptions->Freeze();
    m_plstOptions->Clear();
    m_plstOptions->Thaw();

    MaximizeMonitor(true);
}


void pam2Dialog::ShowOptionsPanel(const wxString& sPanel)
{
    if(m_pswpMain->GetSelectionName() == wxT("Settings"))
    {
        m_ppnlSettings->m_pswpSettings->ChangeSelection(sPanel);
    }
    else if(m_pswpMain->GetSelectionName() == wxT("Help") ||m_pswpMain->GetSelectionName() == wxT("Tests"))
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
    if(Settings::Get().Read(wxT("Input"), wxT("Reset"), false) == false)
    {
        MaximizeMonitor((event.GetInt()) == 1);
    }

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


void pam2Dialog::OnAudioData(AudioEvent& event)
{
    //m_timerAES.Stop();
    if(m_pdlgNoInput)
    {

        m_bInputFailed = false;
        m_pdlgNoInput->Show(false);
    }

    if(SoundcardManager::Get().IsOutputStreamOpen())
    {
        switch(event.GetStatus())
        {
            case AudioEvent::OK:
                m_plblOutput->SetBackgroundColour(wxColour(0,128,0));
                break;
            case AudioEvent::UNDERRUN:
                m_plblOutput->SetBackgroundColour(wxColour(128,0,0));
                break;
            case AudioEvent::OVERRUN:
                m_plblOutput->SetBackgroundColour(wxColour(255,128,0));
                break;
        }
    }

    if(event.GetCreator() != AudioEvent::OUTPUT)
    {
        switch(event.GetStatus())
        {
            case AudioEvent::OK:
                m_pbtnInput->SetBackgroundColour(wxColour(0,128,0));
                break;
            case AudioEvent::UNDERRUN:
                m_pbtnInput->SetBackgroundColour(wxColour(128,0,0));
                break;
            case AudioEvent::OVERRUN:
                m_pbtnInput->SetBackgroundColour(wxColour(255,128,0));
                break;
        }
    }

    PassDataToPanels(event.GetBuffer());
}

void pam2Dialog::OnQoS(wxCommandEvent& event)
{
    qosData* pData = reinterpret_cast<qosData*>(event.GetClientData());
    if(m_pSelectedMonitor)
    {
        m_pSelectedMonitor->SetQoSData(pData);
    }
    for(auto plugin : m_setAlwaysPassQoS)
    {
        if(plugin != m_pSelectedMonitor)
        {
            plugin->SetQoSData(pData);
        }
    }

    delete pData;
}

void pam2Dialog::PassDataToPanels(timedbuffer* pTimedBuffer)
{

    if(m_pSelectedMonitor)
    {
        m_pSelectedMonitor->SetAudioData(pTimedBuffer);
    }
    else if(m_ppnlTests)
    {
        m_ppnlTests->SetAudioData(pTimedBuffer);
    }

    for(set<MonitorPluginBuilder*>::iterator itPlugin = m_setAlwaysPassAudio.begin(); itPlugin != m_setAlwaysPassAudio.end(); ++itPlugin)
    {
        if((*itPlugin) != m_pSelectedMonitor)
        {
            (*itPlugin)->SetAudioData(pTimedBuffer);
        }
    }
}


void pam2Dialog::OnSettingChanged(SettingEvent& event)
{
    if(event.GetSection() == wxT("Input") && event.GetKey() == wxT("Type"))
    {
        m_pbtnInput->SetLabel(event.GetValue());

        if(event.GetValue() == "Disabled" && m_pdlgNoInput)
        {
            m_pdlgNoInput->Show(false);
        }
    }
    else if(event.GetSection() == wxT("Output"))
    {
        OutputChanged(event.GetKey());
    }

    else if(event.GetSection() == wxT("Test"))
    {
        if(event.GetKey() == wxT("Lock"))
        {
            m_plstScreens->Enable((Settings::Get().Read(event.GetSection(), event.GetKey(), 0) == 0));
            m_plstOptions->Enable((Settings::Get().Read(event.GetSection(), event.GetKey(), 0) == 0));
        }
        if(event.GetKey() == wxT("ShowLog"))
        {
            if(event.GetValue(false))
            {
                ShowLogPanel(true);
            }
        }
    }
    else if(event.GetSection() == "Main")
    {
        if(event.GetKey() == "Screen")
        {
            ChangeMainScreen(event.GetValue());
        }
        else if(event.GetKey() == "Test")
        {
            m_ppnlTests->ShowTestPanel(event.GetValue());
            MaximizeMonitor(m_ppnlTests->GetSelectedTest()->CanBeMaximized());
        }
    }
    else if(event.GetSection() == "Splash")
    {
        if(event.GetKey() == "Screen")
        {
            pmlLog(pml::LOG_TRACE) << "SettingChanged: Splash/Screen=" << Settings::Get().Read("Splash", "Screen", "Main");

            m_pswpSplash->ChangeSelection(Settings::Get().Read("Splash", "Screen", "Main"));
        }
        else
        {
            pmlLog(pml::LOG_WARN) << event.GetSection() << ":" << event.GetKey() << " not handled";
        }
    }
    else if(event.GetSection() == "General")
    {
        if(event.GetKey() == "Cursor")
        {
            if(event.GetValue(true))
            {
                wxSetCursor(*wxSTANDARD_CURSOR);
            }
            else
            {
                wxSetCursor(*m_pCursor);
            }
        }
    }
    else
    {
        pmlLog(pml::LOG_WARN) << event.GetSection() << ":" << event.GetKey() << " not handled";
    }
}



void pam2Dialog::OutputChanged(const wxString& sKey)
{
    if(sKey == wxT("Destination"))
    {
        wxString sDestination(Settings::Get().Read(wxT("Output"), wxT("Destination"),wxT("Disabled")));
        bool bEnabled(sDestination!=wxT("Disabled"));
        m_plblOutput->Show(bEnabled);
        if(!bEnabled)
        {
            m_pbtnMonitor->ToggleSelection(false, true);
        }
        m_pbtnMonitor->Enable(bEnabled);

    }
}


void pam2Dialog::OnMonitorRequest(MonitorEvent& event)
{
    Settings::Get().Write(wxT("Output"), wxT("Channel_1"), event.GetChannels()[0]);
    Settings::Get().Write(wxT("Output"), wxT("Channel_2"), event.GetChannels()[1]);
}

void pam2Dialog::OnPlaybackChannels(wxCommandEvent& event)
{
    TellPluginsAboutOutputChannels();
}

void pam2Dialog::TellPluginsAboutOutputChannels()
{
    //now tell all the monitor panels etc...
    for(map<wxString, MonitorPluginBuilder*>::iterator itMonitor = MonitorPluginFactory::Get()->GetPluginBegin(); itMonitor != MonitorPluginFactory::Get()->GetPluginEnd(); ++itMonitor)
    {
        itMonitor->second->OutputChannels(SoundcardManager::Get().GetOutputChannels());
    }

    for(map<wxString, TestPluginBuilder*>::iterator itTest = TestPluginFactory::Get()->GetPluginBegin(); itTest != TestPluginFactory::Get()->GetPluginEnd(); ++itTest)
    {
        itTest->second->OutputChannels(SoundcardManager::Get().GetOutputChannels());
    }

}


void pam2Dialog::OntimerStartTrigger(wxTimerEvent& event)
{
    pmlLog() << "PAM\tCore Lib Path = " <<  Settings::Get().GetCoreLibDirectory();
    pmlLog() << "PAM\tPlugin Monitor Path = " <<  Settings::Get().GetMonitorPluginDirectory();
    pmlLog() << "PAM\tPlugin Test Path = " <<  Settings::Get().GetTestPluginDirectory();
    pmlLog() << "PAM\tDocuments Path = " <<  Settings::Get().GetDocumentDirectory();
    pmlLog() << "PAM\tLogs Path = " <<  Settings::Get().GetLogDirectory();
    pmlLog() << "PAM\tWav Files Path = " <<  Settings::Get().GetWavDirectory();
    pmlLog() << "PAM\tTemp Path = " <<  Settings::Get().GetTempDirectory();

    RemoveOldFiles();


    LoadMonitorPanels();
    //check which page we need.
    wxString sPanel(Settings::Get().Read("Main", "Screen", "Meters"));

    for(multimap<size_t, wxString>::iterator itPage = m_mmMonitorPlugins.begin(); itPage != m_mmMonitorPlugins.end(); ++itPage)
    {
        if(itPage->second == sPanel)
        {
            m_nCurrentMonitorPage = itPage->first;
            ShowMonitorList();
            break;
        }
    }

    if(sPanel != wxT("Settings") && sPanel != wxT("Log") && sPanel != wxT("Help"))
    {
        m_plstScreens->SelectButton(sPanel,false);
    }
    else
    {
        m_plstInbuilt->SelectButton(sPanel,false);
    }

    //NMOS
    #ifdef __NMOS__
    m_pManager = std::unique_ptr<NmosManager>(new NmosManager(m_ppnlSettings->m_ppnlInputNmos));
    m_pManager->Setup();
    m_pManager->AddHandlerToEventPoster(m_ppnlSettings->m_ppnlNmos);
    m_pManager->AddHandlerToClientEventPoster(m_ppnlSettings->m_ppnlNmos);
    #endif // __NMOS__

    IOManager::Get().Start();

    //Show the current monitor panel
    Settings::Get().Write("Startup", "Starting",0);
    Settings::Get().Write("Splash", "Screen", "Main");
    ChangeMainScreen(sPanel);
}

void pam2Dialog::Onm_timerFileTrigger(wxTimerEvent& event)
{
}



void pam2Dialog::OntimerIpcTrigger(wxTimerEvent& event)
{
    pcStats::Get().CalculateCpuStats();
    m_pbtnCPU->SetLabel(wxString::Format(wxT("CPU: %.0f%%"), pcStats::Get().GetTotalCpuUsage()*100.0));

}

void pam2Dialog::OnClose(wxCloseEvent& event)
{
    IOManager::Get().Stop();
    pmlLog() << "PAM\tClosing";
    event.Skip();
}

void pam2Dialog::OnbmpSplashClick(wxCommandEvent& event)
{
    Settings::Get().Write("Splash", "Screen", "Main");

    Settings::Get().Write("Startup", "Starting",0);
}


void pam2Dialog::RemoveOldFiles()
{
    wxLogNull ln;
    wxArrayString asRemove(wxStringTokenize(Settings::Get().Read(wxT("Startup"), wxT("Remove"), wxEmptyString), wxT(",")));
    for(size_t i = 0; i < asRemove.GetCount(); i++)
    {
        wxRemoveFile(asRemove[i]);
    }
    Settings::Get().Write(wxT("Startup"), wxT("Remove"), wxEmptyString);
}

void pam2Dialog::OnHelpClose(wxCommandEvent& event)
{
    MaximizeMonitor(false);
    map<wxString, MonitorPluginBuilder*>::iterator itPlugin = MonitorPluginFactory::Get()->FindPlugin(event.GetString());
    if(itPlugin != MonitorPluginFactory::Get()->GetPluginEnd())
    {
        m_plstScreens->SelectButton(event.GetString(), false);
        Settings::Get().Write(wxT("Main"), "Screen", event.GetString());
        ShowMonitorPanel(event.GetString());
    }
    else
    {
        map<wxString, TestPluginBuilder*>::iterator itPlugin = TestPluginFactory::Get()->FindPlugin(event.GetString());
        if(itPlugin != TestPluginFactory::Get()->GetPluginEnd())
        {
            m_plstScreens->SelectButton(wxT("Tests"), false);
            Settings::Get().Write(wxT("Main"), "Screen", wxT("Tests"));
            ShowTestPanels();
        }
        else
        {
            m_plstScreens->SelectButton(wxT("Settings"), false);
            Settings::Get().Write(wxT("Main"), "Screen", wxT("Settings"));
            ShowSettingsPanel();
        }
    }
}

void pam2Dialog::OnSession(wxCommandEvent& event)
{
    for(map<wxString, MonitorPluginBuilder*>::iterator itMonitor = MonitorPluginFactory::Get()->GetPluginBegin(); itMonitor != MonitorPluginFactory::Get()->GetPluginEnd(); ++itMonitor)
    {
        itMonitor->second->InputSession(IOManager::Get().GetSession());
    }

    for(map<wxString, TestPluginBuilder*>::iterator itTest = TestPluginFactory::Get()->GetPluginBegin(); itTest != TestPluginFactory::Get()->GetPluginEnd(); ++itTest)
    {
        itTest->second->InputSession(IOManager::Get().GetSession());
    }



    m_ppnlSettings->SessionChanged();
}

void pam2Dialog::OnbtnMonitorClick(wxCommandEvent& event)
{
  //  Settings::Get().Write(wxT("Monitor"), wxT("Source"), event.IsChecked());
}

void pam2Dialog::OnswpScreensPageChanged(wxNotebookEvent& event)
{

    if(m_pswpScreens->GetSelectionName() == wxT("Screens"))
    {
        wxString sLast(Settings::Get().Read(wxT("Main"), "Screen", wxEmptyString));
        if(sLast == wxT("Log") || sLast == wxT("Settings") || sLast == wxT("Help"))
        {
            m_plstScreens->SelectButton(Settings::Get().Read(wxT("Main"), wxT("Last"), wxEmptyString));
        }
    }
}

void pam2Dialog::OnswpMainPageChanged(wxNotebookEvent& event)
{
    if(m_pdlgNoInput)
    {
        if(m_bInputFailed)
        {
            m_pdlgNoInput->Show(m_pswpSplash->GetSelectionName() == "Main" && (m_pswpMain->GetSelectionName() != wxT("Settings") && m_pswpMain->GetSelectionName() != wxT("Log") &&
                                  m_pswpMain->GetSelectionName() != wxT("Help") && Settings::Get().Read("Input","Type","Soundcard")!="Disabled"));
        }
        else
        {
            m_pdlgNoInput->Show(false);
        }
    }

    pam2App* pApp = dynamic_cast<pam2App*>(wxTheApp);
    if(pApp)
    {
        pApp->CheckHold(m_pswpSplash->GetSelectionName() == "Main" && m_pswpMain->GetSelectionName() != wxT("Settings") && m_pswpMain->GetSelectionName() != wxT("Log") &&
                                  m_pswpMain->GetSelectionName() != wxT("Help"));
    }
}

void pam2Dialog::OnswpSplashPageChanged(wxNotebookEvent& event)
{
}


void pam2Dialog::OnInputFailed(wxCommandEvent& event)
{
    if(!m_pdlgNoInput)
    {
        m_pdlgNoInput = new dlgNoInput(this, wxNewId(), wxPoint(0,425));
    }
    m_bInputFailed = true;

    m_pdlgNoInput->SetPosition(wxPoint(0,425));
    m_pdlgNoInput->Show(m_pswpSplash->GetSelectionName() == "Main" && (m_pswpMain->GetSelectionName() != wxT("Settings") && m_pswpMain->GetSelectionName() != wxT("Log")  &&
                                  m_pswpMain->GetSelectionName() != wxT("Help") && Settings::Get().Read("Input","Type","Soundcard")!="Disabled") );
}





void pam2Dialog::OnbtnInputClick(wxCommandEvent& event)
{
    if(Settings::Get().Read(wxT("Input"), wxT("Type"), wxT("Soundcard")) == "AoIP")
    {
        int nmos = 0;
        #ifdef __NMOS__
        nmos = Settings::Get().Read("NMOS", "Node", NmosManager::NODE_OFF);
        #endif
        if(nmos == 0)
        {
            Settings::Get().Write("Splash", "Screen", "AoIP");
            if(m_pdlgNoInput)
            {
                m_pdlgNoInput->Show(false);
            }
        }
        #ifdef __NMOS__
        if((nmos == NmosManager::NODE_BOTH || nmos == NmosManager::NODE_RECEIVER) && Settings::Get().Read("NMOS", "Client", NmosManager::CLIENT_OFF) != NmosManager::CLIENT_OFF)
        {

        }
        #endif // __NMOS__
    }
    else
    {
        Screenshot();
    }
}

void pam2Dialog::OnbtnScreenshotClick(wxCommandEvent& event)
{
    if(m_pSelectedMonitor)
    {
        MaximizeMonitor(m_pSelectedMonitor->CanBeMaximized());
    }
    else if(m_ppnlTests->GetSelectedTest())
    {
        MaximizeMonitor(m_ppnlTests->GetSelectedTest()->CanBeMaximized());
    }

    m_timerScreenshot.Start(100,true);

}

void pam2Dialog::OnTimerScreenshot(const wxTimerEvent& event)
{
    m_usb.SaveToUSB({wxFileName(Screenshot())}, true);
}

wxString pam2Dialog::Screenshot()
{
    wxWindow* pWnd(nullptr);
    if(m_pSelectedMonitor)
    {
        pWnd = m_pSelectedMonitor->GetMainWindow();
    }
    else if(m_ppnlTests)
    {
        pWnd = m_ppnlTests;
    }
    wxString sFilename;
    if(pWnd)
    {
        wxWindowDC dcScreen(pWnd);
        auto dimensions = dcScreen.GetSize();
        wxBitmap bmpScreen(dimensions.x, dimensions.y,-1);
        wxMemoryDC dcMem;
        dcMem.SelectObject(bmpScreen);
        dcMem.Blit(0,0,dimensions.x,dimensions.y, &dcScreen, 0,0);
        dcMem.SelectObject(wxNullBitmap);

        sFilename = wxDateTime::Now().Format("/tmp/%Y%m%dT%H%M%S.jpg");
        if(bmpScreen.SaveFile(sFilename,wxBITMAP_TYPE_JPEG) == false)
        {
            pmlLog(pml::LOG_WARN) << "Failed to save screenshot " << sFilename;
            sFilename.clear();
        }
    }
    return sFilename;

}



void pam2Dialog::RegisterRemoteApiSettings()
{
    pmlLog() << "RegisterRemoteApiSettings";

    RemoteApi::Get().RegisterRemoteApiEnum("Input", "Type", {"Disabled", "Soundcard", "AoIP"}, "Soundcard");
    RemoteApi::Get().RegisterRemoteApiCallback("Input", "AoIP", GetAoipSources, 0);

    RemoteApi::Get().RegisterRemoteApiEnum("Input", "Device", SoundcardManager::Get().GetInputDevices(), 0);

    RemoteApi::Get().RegisterRemoteApiEnum("Server", "Stream", {wxString("OnDemand"), "AlwaysOn"}, "OnDemand");
    RemoteApi::Get().RegisterRemoteApiEnum("Server", "PacketTime", {{125, "125"}, {250,"250"}, {333,"333"}, {1000,"1000"}, {4000,"4000"}}, 1000);
    RemoteApi::Get().RegisterRemoteApiEnum("Server", "SampleRate", {{44100,"44100"}, {48000,"48000"}, {96000,"96000"}}, 48000);


    RemoteApi::Get().RegisterRemoteApiEnum("Server", "Bits", {{16,"16"}, {24,"24"}}, 24);
    RemoteApi::Get().RegisterRemoteApiEnum("Server", "RTCP", {{0,"Off"}, {1,"On"}}, 1);
    RemoteApi::Get().RegisterRemoteApiEnum("Server", "State", {{0,"Off"}, {1,"On"}}, 0);
    RemoteApi::Get().RegisterRemoteApi("Server", "DestinationIp", "");
    RemoteApi::Get().RegisterRemoteApiRangeInt("Server", "RTSP_Port", {1,65535}, 5555);
    RemoteApi::Get().RegisterRemoteApiRangeInt("Server", "RTP_Port", {1,65535}, 5004);
    RemoteApi::Get().RegisterRemoteApiCallback("Server", "RTSP_Interface", GetInterfaceNames, "eth0");
    RemoteApi::Get().RegisterRemoteApiEnum("Server", "SAP", {{0,"Off"}, {1,"On"}}, 0);
    RemoteApi::Get().RegisterRemoteApiEnum("Server", "DNS-SD", {{0,"Off"}, {1,"On"}}, 0);
    RemoteApi::Get().RegisterRemoteApiRangeInt("Server", "Channels", {1,8}, 2);
    RemoteApi::Get().RegisterRemoteApiRangeInt("Server", "RTPMap", {96,127}, 96);
    RemoteApi::Get().RegisterRemoteApiEnum("Server", "MappingEnabled", {{0, "Disabled"}, {1,"Enabled"}}, 0);



    RemoteApi::Get().RegisterRemoteApiEnum("Monitor", "Source", {{0,"Input"}, {1,"Output"}}, 0);

    RemoteApi::Get().RegisterRemoteApiEnum("Output", "Destination", {"Disabled", "Device", "AoIP"}, "Disabled");
    RemoteApi::Get().RegisterRemoteApiRangeInt("Output", "Channel_1", {-1,7}, 0);
    RemoteApi::Get().RegisterRemoteApiRangeInt("Output", "Channel_2", {-1,7}, 1);
    RemoteApi::Get().RegisterRemoteApiRangeInt("Output", "Channel_3", {-1,7}, 0);
    RemoteApi::Get().RegisterRemoteApiRangeInt("Output", "Channel_4", {-1,7}, 1);
    RemoteApi::Get().RegisterRemoteApiRangeInt("Output", "Channel_5", {-1,7}, 0);
    RemoteApi::Get().RegisterRemoteApiRangeInt("Output", "Channel_6", {-1,7}, 1);
    RemoteApi::Get().RegisterRemoteApiRangeInt("Output", "Channel_7", {-1,7}, 0);
    RemoteApi::Get().RegisterRemoteApiRangeInt("Output", "Channel_8", {-1,7}, 1);

    RemoteApi::Get().RegisterRemoteApiEnum("Output", "Source", GetOutputSources(), "Input");

    RemoteApi::Get().RegisterRemoteApiCallback("Output", "File", GetWavFiles, "");
    RemoteApi::Get().RegisterRemoteApiCallback("Output", "Sequence", GetSequences, "Glits");

    RemoteApi::Get().RegisterRemoteApiEnum("Output", "Device", SoundcardManager::Get().GetOutputDevices(),0);
    RemoteApi::Get().RegisterRemoteApiRangeInt("Output", "Latency", {0,400}, 200);

    RemoteApi::Get().RegisterRemoteApiRangeInt("Generator", "Frequency", {1,22000}, 1000);
    RemoteApi::Get().RegisterRemoteApiRangeDouble("Generator", "Amplitude", {-80.0,0.0}, -18.0);
    RemoteApi::Get().RegisterRemoteApiRangeDouble("Noise", "Amplitude", {-80.0,0.0}, -18.0);
    RemoteApi::Get().RegisterRemoteApiEnum("Noise", "Colour", {{0,"White"},{1,"Pink"},{2,"Grey"},{3,"A"},{4,"K"}},0);



    RemoteApi::Get().RegisterRemoteApiEnum("Log", "Level", {{0,"TRACE"},{1,"DEBUG"},{2,"INFO"},{3,"WARNING"},{4,"ERROR"},{5,"CRITICAL"}}, 2);

    RemoteApi::Get().RegisterRemoteApiEnum("Discovery", "SAP", {{0,"Off"}, {1,"On"}}, 0);
    RemoteApi::Get().RegisterRemoteApiEnum("Discovery", "DNSSD", {{0,"Off"}, {1,"On"}}, 1);
    RemoteApi::Get().RegisterRemoteApiEnum("Discovery", pnlRTP::STR_SAP_SETTING[pnlRTP::LOCAL], {{0,"Off"}, {1,"On"}}, 0);
    RemoteApi::Get().RegisterRemoteApiEnum("Discovery", pnlRTP::STR_SAP_SETTING[pnlRTP::ORGANISATION], {{0,"Off"}, {1,"On"}}, 0);
    RemoteApi::Get().RegisterRemoteApiEnum("Discovery", pnlRTP::STR_SAP_SETTING[pnlRTP::GLOBAL], {{0,"Off"}, {1,"On"}}, 0);

    RemoteApi::Get().RegisterRemoteApiRangeInt("Time", "PTP_Domain", {0,127}, 0);
    RemoteApi::Get().RegisterRemoteApiEnum("Time", "Sync", {{0,"Off"}, {1,"NTP"}, {3,"PTP"}}, 0);

    RemoteApi::Get().RegisterRemoteApiEnum("Tests", "Log", {{0,"Off"}, {1,"On"}}, 0);
    RemoteApi::Get().RegisterRemoteApiEnum("Tests", "LogView", {{0,"Off"}, {1,"On"}}, 0);

    RemoteApi::Get().RegisterRemoteApiEnum("NMOS", "Node", {{0,"Off"}, {1, "Receiver Only"}, {2, "Sender Only"}, {3, "Receiver and Sender"}}, 0);
    RemoteApi::Get().RegisterRemoteApiEnum("NMOS", "Client", {{0,"Off"}, {1, "IS04 Connection"}, {2, "IS05 Connection"}}, 0);


    pmlLog() << "RegisterRemoteApiSettings:Done";

}


void pam2Dialog::ChangeMainScreen(const wxString& sPage)
{
    if(sPage == "Settings")
    {
        m_plstScreens->SelectAll(false,false);
        m_pSelectedMonitor = nullptr;
        ShowSettingsPanel();
    }
    else if(sPage == "Help")
    {
        m_plstScreens->SelectAll(false,false);
        ShowHelpPanel();
        m_pSelectedMonitor = nullptr;
    }
    else if(sPage == "Log")
    {
        m_plstScreens->SelectAll(false,false);
        m_pSelectedMonitor = nullptr;
        ShowLogPanel();
    }
    else
    {
        m_plstInbuilt->SelectAll(false, false);

        if(sPage == "Tests")
        {
            m_pSelectedMonitor = nullptr;
            ShowTestPanels();
        }
        else
        {
            ShowMonitorPanel(sPage);
        }
    }
}
