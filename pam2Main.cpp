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
#include "wmlogevent.h"
#include <wx/stdpaths.h>
#include "soundfile.h"
#include "wxpammclient.h"
#include "images/splash.xpm"
#include "updatemanager.h"
#include "pnlHelp.h"
#include "generator.h"
#include "audioevent.h"
#include "soundcardmanager.h"


//(*InternalHeaders(pam2Dialog)
#include <wx/bitmap.h>
#include <wx/intl.h>
#include <wx/image.h>
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
const long pam2Dialog::ID_BITMAPBUTTON1 = wxNewId();
const long pam2Dialog::ID_PANEL4 = wxNewId();
const long pam2Dialog::ID_M_PSWP1 = wxNewId();
const long pam2Dialog::ID_M_PLST1 = wxNewId();
const long pam2Dialog::ID_M_PLST3 = wxNewId();
const long pam2Dialog::ID_M_PLBL3 = wxNewId();
const long pam2Dialog::ID_M_PLBL1 = wxNewId();
const long pam2Dialog::ID_M_PLBL4 = wxNewId();
const long pam2Dialog::ID_M_PLBL2 = wxNewId();
const long pam2Dialog::ID_PANEL7 = wxNewId();
const long pam2Dialog::ID_PANEL5 = wxNewId();
const long pam2Dialog::ID_M_PLST2 = wxNewId();
const long pam2Dialog::ID_PANEL2 = wxNewId();
const long pam2Dialog::ID_M_PSWP2 = wxNewId();
const long pam2Dialog::ID_PANEL6 = wxNewId();
const long pam2Dialog::ID_M_PSWP4 = wxNewId();
const long pam2Dialog::ID_PANEL1 = wxNewId();
const long pam2Dialog::ID_PANEL3 = wxNewId();
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
    m_pSelectedMonitor(0),
    m_ppnlLog(0)
{

    //(*Initialize(pam2Dialog)
    Create(parent, id, _("wxWidgets app"), wxDefaultPosition, wxDefaultSize, wxNO_BORDER, _T("id"));
    SetClientSize(wxSize(800,480));
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
    m_plstScreens = new wmList(Panel2, ID_M_PLST1, wxPoint(0,5), wxSize(200,350), wmList::STYLE_SELECT, 2, wxSize(-1,40), 3, wxSize(5,5));
    m_plstScreens->SetBackgroundColour(wxColour(0,0,0));
    m_plstScreens->SetButtonColour(wxColour(wxT("#008000")));
    m_plstScreens->SetSelectedButtonColour(wxColour(wxT("#FF8000")));
    m_plstInbuilt = new wmList(Panel2, ID_M_PLST3, wxPoint(0,355), wxSize(200,44), wmList::STYLE_SELECT, 0, wxSize(-1,40), 3, wxSize(5,5));
    m_plstInbuilt->SetForegroundColour(wxColour(0,0,0));
    m_plstInbuilt->SetBackgroundColour(wxColour(0,0,0));
    m_plstInbuilt->SetButtonColour(wxColour(wxT("#3DBEAB")));
    m_plstInbuilt->SetSelectedButtonColour(wxColour(wxT("#FF8000")));
    m_plstInbuilt->SetTextButtonColour(wxColour(wxT("#000000")));
    Panel4 = new wxPanel(Panel2, ID_PANEL7, wxPoint(4,400), wxSize(193,50), wxTAB_TRAVERSAL, _T("ID_PANEL7"));
    Panel4->SetBackgroundColour(wxColour(255,255,255));
    m_pLbl1 = new wmLabel(Panel4, ID_M_PLBL3, _("Monitor"), wxPoint(1,1), wxSize(95,23), 0, _T("ID_M_PLBL3"));
    m_pLbl1->SetBorderState(uiRect::BORDER_NONE);
    m_pLbl1->SetForegroundColour(wxColour(255,255,255));
    m_pLbl1->SetBackgroundColour(wxColour(0,0,0));
    m_plblInput = new wmLabel(Panel4, ID_M_PLBL1, _("Input"), wxPoint(1,25), wxSize(95,24), 0, _T("ID_M_PLBL1"));
    m_plblInput->SetBorderState(uiRect::BORDER_NONE);
    m_plblInput->SetForegroundColour(wxColour(255,255,255));
    m_plblInput->SetBackgroundColour(wxColour(0,128,0));
    m_pLbl2 = new wmLabel(Panel4, ID_M_PLBL4, _("Output"), wxPoint(97,1), wxSize(95,23), 0, _T("ID_M_PLBL4"));
    m_pLbl2->SetBorderState(uiRect::BORDER_NONE);
    m_pLbl2->SetForegroundColour(wxColour(255,255,255));
    m_pLbl2->SetBackgroundColour(wxColour(0,0,0));
    m_plblOutput = new wmLabel(Panel4, ID_M_PLBL2, _("Output"), wxPoint(97,25), wxSize(95,24), 0, _T("ID_M_PLBL2"));
    m_plblOutput->SetBorderState(uiRect::BORDER_NONE);
    m_plblOutput->SetForegroundColour(wxColour(255,255,255));
    m_plblOutput->SetBackgroundColour(wxColour(0,128,0));
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
    m_pswpSplash->AddPage(pnlSplash, _("Splash"), true);
    m_pswpSplash->AddPage(pnlMain, _("Main"), false);
    timerStart.SetOwner(this, ID_TIMER1);
    timerStart.Start(10, true);
    m_timerFile.SetOwner(this, ID_TIMER2);
    m_timerIpc.SetOwner(this, ID_TIMER3);
    m_timerIpc.Start(1000, false);

    Connect(ID_BITMAPBUTTON1,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&pam2Dialog::OnbmpSplashClick);
    Connect(ID_M_PLST1,wxEVT_LIST_SELECTED,(wxObjectEventFunction)&pam2Dialog::OnlstScreensSelected);
    Connect(ID_M_PLST3,wxEVT_LIST_SELECTED,(wxObjectEventFunction)&pam2Dialog::OnlstScreensSelected);
    Connect(ID_M_PLST2,wxEVT_LIST_SELECTED,(wxObjectEventFunction)&pam2Dialog::OnplstOptionsSelected);
    Connect(ID_TIMER1,wxEVT_TIMER,(wxObjectEventFunction)&pam2Dialog::OntimerStartTrigger);
    Connect(ID_TIMER2,wxEVT_TIMER,(wxObjectEventFunction)&pam2Dialog::Onm_timerFileTrigger);
    Connect(ID_TIMER3,wxEVT_TIMER,(wxObjectEventFunction)&pam2Dialog::OntimerIpcTrigger);
    Connect(wxID_ANY,wxEVT_CLOSE_WINDOW,(wxObjectEventFunction)&pam2Dialog::OnClose);
    //*)




    m_pClient = new PammClient();
    if(m_pClient->Connect(wxT("localhost"), wxT("/tmp/pamm.ipc"), wxT("pam")))
    {

    }
    else
    {
        delete m_pClient;
        m_pClient = 0;
    }

    m_plstScreens->SetFont(wxFont(10,wxFONTFAMILY_SWISS,wxFONTSTYLE_NORMAL,wxFONTWEIGHT_NORMAL,false,_T("Arial"),wxFONTENCODING_DEFAULT));
    m_plstOptions->SetFont(wxFont(10,wxFONTFAMILY_SWISS,wxFONTSTYLE_NORMAL,wxFONTWEIGHT_NORMAL,false,_T("Arial"),wxFONTENCODING_DEFAULT));

    pnlLists->SetBackgroundColour(*wxBLACK);

    m_plstScreens->SetBackgroundColour(*wxBLACK);
    m_plstOptions->SetBackgroundColour(*wxBLACK);

    Connect(wxID_ANY,wxEVT_DATA,(wxObjectEventFunction)&pam2Dialog::OnAudioData);
    Connect(wxID_ANY,wxEVT_RTP_SESSION,(wxObjectEventFunction)&pam2Dialog::OnRTPSession);
    Connect(wxID_ANY,wxEVT_RTP_SESSION_CLOSED,(wxObjectEventFunction)&pam2Dialog::OnRTPSessionClosed);
    Connect(wxID_ANY,wxEVT_QOS_UPDATED,(wxObjectEventFunction)&pam2Dialog::OnQoS);



    Settings::Get().AddHandler(wxT("Input"),wxT("Type"), this);
    Settings::Get().AddHandler(wxT("Input"),wxT("RTP"), this);
    Settings::Get().AddHandler(wxT("Input"),wxT("Device"), this);
    Settings::Get().AddHandler(wxT("Input"),wxT("File"), this);


    Settings::Get().AddHandler(wxT("Output"),wxT("Device"), this);
    Settings::Get().AddHandler(wxT("Output"),wxT("Enabled"), this);
    Settings::Get().AddHandler(wxT("Output"),wxT("Buffer"), this);
    Settings::Get().AddHandler(wxT("Output"),wxT("Latency"), this);

    Settings::Get().AddHandler(wxT("Output"),wxT("Source"), this);
    Settings::Get().AddHandler(wxT("Output"),wxT("File"), this);
    Settings::Get().AddHandler(wxT("Output"),wxT("Sequence"), this);
    Settings::Get().AddHandler(wxT("Output"),wxT("Generator"), this);

    Settings::Get().AddHandler(wxT("Generator"),wxT("Frequency"), this);
    Settings::Get().AddHandler(wxT("Generator"),wxT("Amplitude"), this);
    Settings::Get().AddHandler(wxT("Generator"),wxT("Shape"), this);
    Settings::Get().AddHandler(wxT("Noise"),wxT("Colour"), this);
    Settings::Get().AddHandler(wxT("Noise"),wxT("Amplitude"), this);


    Settings::Get().AddHandler(wxT("QoS"),wxT("Interval"), this);
    Settings::Get().AddHandler(wxT("Test"), wxT("Lock"), this);

    Connect(wxID_ANY, wxEVT_SETTING_CHANGED, (wxObjectEventFunction)&pam2Dialog::OnSettingChanged);
    Connect(wxID_ANY, wxEVT_MONITOR_REQUEST, (wxObjectEventFunction)&pam2Dialog::OnMonitorRequest);
    Connect(wxID_ANY, wxEVT_PLUGINS_APPLY, (wxObjectEventFunction)&pam2Dialog::OnPluginsReload);


    Connect(wxID_ANY, wxEVT_HELP_CLOSE, (wxObjectEventFunction)&pam2Dialog::OnHelpClose);

    m_pGenerator = 0;

}

pam2Dialog::~pam2Dialog()
{
    //(*Destroy(pam2Dialog)
    //*)
    if(m_pClient && m_pClient->IsConnected())
    {
        m_pClient->Disconnect();
        delete m_pClient;
        m_pClient = NULL;
    }

    for(map<wxString, RtpThread*>::iterator itThread = m_mRtp.begin(); itThread != m_mRtp.end(); ++itThread)
    {
        bool bDelete = m_setRtpOrphan.insert(itThread->first).second;
        if(bDelete)
        {
            itThread->second->Delete();
        }
    }

    if(m_pGenerator)
    {
        delete m_pGenerator;
    }

    SoundcardManager::Get().Terminate();


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
    if(!m_pClient || !m_pClient->Restart())
    {
        wxExecute(wxStandardPaths::Get().GetExecutablePath());
        Close();
    }
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

    m_ppnlLog = new pnlLog(m_pswpMain);
    m_pswpMain->AddPage(m_ppnlLog, wxT("Log"));

    m_ppnlSettings = new pnlSettings(m_pswpMain);
    m_pswpMain->AddPage(m_ppnlSettings, wxT("Settings"));

    m_ppnlHelp =  new pnlHelp(m_pswpMain);
    m_pswpMain->AddPage(m_ppnlHelp, wxT("Help"));

    pnlLogControl* pnlControl = new pnlLogControl(m_pswpOptions, m_ppnlLog);
    m_ppnlLog->SetLogControl(pnlControl);

    m_pswpOptions->AddPage(new pnlSettingsOptions(m_pswpOptions), wxT("Settings|Options"));
    m_pswpOptions->AddPage(pnlControl, wxT("Log|Control"));

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
    if(event.GetString() == wxT("Settings"))
    {
        m_plstScreens->SelectAll(false,false);
        Settings::Get().Write(wxT("Main"), wxT("Monitor"), event.GetString());
        m_pSelectedMonitor = 0;
        ShowSettingsPanel();
    }
    else if(event.GetString() == wxT("Help"))
    {
        m_plstScreens->SelectAll(false,false);
        ShowHelpPanel();
        m_pSelectedMonitor = 0;
        Settings::Get().Write(wxT("Main"), wxT("Monitor"), event.GetString());
    }
    else if(event.GetString() == wxT("Log"))
    {
        m_plstScreens->SelectAll(false,false);
        Settings::Get().Write(wxT("Main"), wxT("Monitor"), event.GetString());
        m_pSelectedMonitor = 0;
        ShowLogPanel();
    }
    else
    {
        m_plstInbuilt->SelectAll(false, false);

        if(event.GetString() == wxT("Tests"))
        {


            Settings::Get().Write(wxT("Main"), wxT("Monitor"), event.GetString());
            m_pSelectedMonitor = 0;
            ShowTestPanels();
        }
        else if(event.GetString() == wxT("Next Page"))
        {
            m_nCurrentMonitorPage++;
            ShowMonitorList();
        }
        else if(event.GetString() == wxT("Prev Page"))
        {
            m_nCurrentMonitorPage--;
            ShowMonitorList();
        }
        else
        {
            Settings::Get().Write(wxT("Main"), wxT("Monitor"), event.GetString());
            ShowMonitorPanel(event.GetString());
        }
    }
    m_pswpSplash->ChangeSelection(1);
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
        m_pSelectedMonitor = 0;
        MaximizeMonitor(false);
    }
}

void pam2Dialog::ShowSettingsPanel()
{
    m_pswpMain->ChangeSelection(wxT("Settings"));

    m_plstOptions->Freeze();
    m_plstOptions->Clear();

    m_plstOptions->AddButton(wxT("Monitor"));
    m_plstOptions->AddButton(wxT("Output Device"));
    m_plstOptions->AddButton(wxT("Output Source"));
    m_plstOptions->AddButton(wxT("AoIP"));
    m_plstOptions->AddButton(wxT("Network"));
    m_plstOptions->AddButton(wxT("Plugins"));
    m_plstOptions->AddButton(wxT("Update"));
    m_plstOptions->AddButton(wxT("Threads"));


    m_plstOptions->SelectButton(Settings::Get().Read(wxT("Settings"), wxT("_Options"), 0));
    m_plstOptions->Thaw();

    m_pswpOptions->ChangeSelection(wxT("Settings|Options"));
    m_pswpScreens->ChangeSelection(wxT("Options"));
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
    m_pswpScreens->ChangeSelection(wxT("Options"));
}


void pam2Dialog::ShowLogPanel()
{
    m_pswpMain->ChangeSelection(wxT("Log"));

    m_plstOptions->Freeze();
    m_plstOptions->Clear();
    m_plstOptions->Thaw();

    m_pswpOptions->ChangeSelection(wxT("Log|Control"));

    m_pswpScreens->ChangeSelection(wxT("Options"));
}

void pam2Dialog::ShowHelpPanel()
{
    wxString sHelp = Settings::Get().Read(wxT("Main"), wxT("Monitor"), wxEmptyString);
    if(sHelp == wxT("Tests"))
    {
        sHelp = m_plstOptions->GetButtonText(Settings::Get().Read(wxT("Tests"), wxT("_Panel"),0));
    }
    else if(sHelp == wxT("Log") || sHelp == wxT("Settings") || sHelp == wxEmptyString)
    {
        sHelp = wxT("Pam");
    }

    wxLogDebug(wxT("Help = %s"), sHelp.c_str());
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

void pam2Dialog::CreateAudioDevice(unsigned long nOutputSampleRate)
{
    wmLog::Get()->Log(wxT("Create Audio Device: Soundcard"));

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

void pam2Dialog::CreateAudioInputDevice()
{
    wxString sType(Settings::Get().Read(wxT("Input"), wxT("Type"), wxT("Soundcard")));

    if(sType == wxT("Soundcard"))
    {
        CreateAudioDevice(SoundcardManager::Get().GetOutputSampleRate());

        m_nMonitorSource = AudioEvent::SOUNDCARD;
        m_plblInput->SetLabel(wxT("Soundcard"));

        if(m_nPlaybackSource == AudioEvent::RTP)
        {
            m_nPlaybackSource = AudioEvent::SOUNDCARD;
        }




        session aSession(wxEmptyString, SoundcardManager::Get().GetInputDeviceName(), wxT("Soundcard"));
        aSession.lstSubsession.push_back(subsession(wxEmptyString, SoundcardManager::Get().GetInputDeviceName(), wxEmptyString, wxT("L24"), wxEmptyString, SoundcardManager::Get().GetInputDevice(), SoundcardManager::Get().GetInputSampleRate(), SoundcardManager::Get().GetInputNumberOfChannels(), wxEmptyString, 0, make_pair(0,0), refclk()));
        aSession.itCurrentSubsession = aSession.lstSubsession.begin();

        InputSession(aSession);

        CheckPlayback(SoundcardManager::Get().GetInputSampleRate(), SoundcardManager::Get().GetInputNumberOfChannels());
    }
    else if(sType == wxT("RTP"))
    {
        m_nMonitorSource = AudioEvent::RTP;
        m_plblInput->SetLabel(wxT("RTP"));
        if(m_nPlaybackSource == AudioEvent::SOUNDCARD)
        {
            m_nPlaybackSource = AudioEvent::RTP;
        }
        wmLog::Get()->Log(wxT("Create Audio Input Device: AoIP"));
        wxString sRtp(Settings::Get().Read(wxT("Input"), wxT("RTP"), wxEmptyString));
        sRtp = Settings::Get().Read(wxT("RTP"), sRtp, wxEmptyString);

        if(sRtp.empty() == false && m_mRtp.find(sRtp) == m_mRtp.end())
        {

            m_sCurrentRtp = sRtp;

            RtpThread* pThread = new RtpThread(this, wxT("pam"), sRtp, 2048);
            pThread->Create();
            pThread->Run();

            pThread->SetQosMeasurementIntervalMS(Settings::Get().Read(wxT("QoS"), wxT("Interval"), 1000));

            m_mRtp.insert(make_pair(m_sCurrentRtp, pThread));
            PopulateThreadList();
        }
    }
    else if(sType == wxT("Output"))
    {
        m_nMonitorSource = AudioEvent::OUTPUT;
        m_plblInput->SetLabel(wxT("Output"));
        wmLog::Get()->Log(wxT("Monitoring output"));

        CreateSessionFromOutput(wxEmptyString);
    }
}

void pam2Dialog::CreateSessionFromOutput(const wxString& sSource)
{
    session aSession(wxEmptyString, wxT("Output"), Settings::Get().Read(wxT("Output"), wxT("Source"), wxEmptyString));
    //we need to get the info from the output...
    unsigned int nSampleRate = SoundcardManager::Get().GetOutputSampleRate();

    aSession.lstSubsession.push_back(subsession(Settings::Get().Read(wxT("Output"), wxT("Source"),wxEmptyString), sSource, wxEmptyString, wxT("F32"), wxEmptyString, 0, nSampleRate, 2, wxEmptyString, 0, make_pair(0,0), refclk()));
    aSession.itCurrentSubsession = aSession.lstSubsession.begin();


    InputSession(aSession);
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

void pam2Dialog::OnAudioData(AudioEvent& event)
{
    //m_timerAES.Stop();

    if(SoundcardManager::Get().IsOutputStreamOpen())
    {
        if(event.GetCreator() == AudioEvent::OUTPUT)
        {
            if(m_pGenerator && m_nPlaybackSource != m_nMonitorSource)
            {
                m_pGenerator->Generate(event.GetBuffer()->GetBufferSize());
            }

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
        else if(event.GetCreator() == m_nPlaybackSource)
        {
            SoundcardManager::Get().AddOutputSamples(event.GetBuffer());
        }
    }

    if(event.GetCreator() != AudioEvent::OUTPUT)
    {
        switch(event.GetStatus())
        {
            case AudioEvent::OK:
                m_plblInput->SetBackgroundColour(wxColour(0,128,0));
                break;
            case AudioEvent::UNDERRUN:
                m_plblInput->SetBackgroundColour(wxColour(128,0,0));
                break;
            case AudioEvent::OVERRUN:
                m_plblInput->SetBackgroundColour(wxColour(255,128,0));
                break;
        }
    }

    if(event.GetCreator() == m_nMonitorSource)
    {
        PassDataToPanels(event.GetBuffer());
    }

    delete event.GetBuffer();
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
    else if(event.GetSection() == wxT("Test"))
    {
        if(event.GetKey() == wxT("Lock"))
        {
            m_plstScreens->Enable((Settings::Get().Read(event.GetSection(), event.GetKey(), 0) == 0));
            m_plstOptions->Enable((Settings::Get().Read(event.GetSection(), event.GetKey(), 0) == 0));
        }
    }
    else if(event.GetSection() == wxT("Generator") && Settings::Get().Read(wxT("Output"), wxT("Source"), wxT("Input")) == wxT("Generator"))
    {
        if(m_pGenerator)
        {
            m_pGenerator->SetFrequency(Settings::Get().Read(wxT("Generator"), wxT("Frequency"), 1000), Settings::Get().Read(wxT("Generator"), wxT("Amplitude"), -18.0), Settings::Get().Read(wxT("Generator"), wxT("Shape"), 0));
        }
    }
    else if(event.GetSection() == wxT("Noise") && Settings::Get().Read(wxT("Output"), wxT("Source"), wxT("Input")) == wxT("Noise"))
    {
        wmLog::Get()->Log(wxT("Change Audio Output Generator: Noise"));
        InitNoiseGenerator();
    }
}


void pam2Dialog::InputChanged(const wxString& sKey)
{
    //has the type changed
    if(sKey == wxT("Type"))
    {
        wxString sType(Settings::Get().Read(wxT("Input"), wxT("Type"), wxT("Soundcard")));
        if(sType != wxT("Soundcard"))
        {
            CreateAudioDevice(SoundcardManager::Get().GetOutputSampleRate());    //this will remove the input stream
        }
        if(sType != wxT("RTP"))
        {
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
        }

        if(sType == wxT("Soundcard"))
        {
            m_nMonitorSource = AudioEvent::SOUNDCARD;

        }
        else if(sType == wxT("RTP"))
        {
            m_nMonitorSource = AudioEvent::RTP;
        }
        else if(sType == wxT("Output"))
        {
            m_nMonitorSource = AudioEvent::OUTPUT;
        }
        m_plblInput->SetLabel(sType);

    }
    else if(sKey == wxT("RTP"))
    {
        wxString sUrl = Settings::Get().Read(wxT("RTP"), Settings::Get().Read(wxT("Input"), wxT("RTP"), wxEmptyString), wxEmptyString);
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
            CreateAudioInputDevice();
        }
    }
    else if(sKey == wxT("Device"))
    {
        CreateAudioInputDevice();
    }
}

void pam2Dialog::OpenFileForReading()
{
    wxString sFilePath;
    sFilePath << Settings::Get().GetDocumentDirectory() << wxT("/") << Settings::Get().Read(wxT("Output"), wxT("File"), wxEmptyString) << wxT(".wav");
    //Does the file exist
    if(wxFileExists(sFilePath) && m_pGenerator)
    {
        if(m_pGenerator->SetFile(sFilePath))
        {
            wmLog::Get()->Log(wxString::Format(wxT("Opened file '%s'"), sFilePath.c_str()));
            wmLog::Get()->Log(wxString::Format(wxT("SampleRate = %d"), m_pGenerator->GetSampleRate()));
            wmLog::Get()->Log(wxString::Format(wxT("Channels = %d"), m_pGenerator->GetChannels()));

            CheckPlayback(m_pGenerator->GetSampleRate(), m_pGenerator->GetChannels());
            m_dtLastRead = wxDateTime::UNow();
            CreateSessionFromOutput(Settings::Get().Read(wxT("Output"), wxT("File"), wxEmptyString));

            m_pGenerator->Generate(8192);
        }
        else
        {
            wmLog::Get()->Log(wxString::Format(wxT("Failed to open file '%s'"), sFilePath.c_str()));
        }
    }
    else
    {
        wmLog::Get()->Log(wxString::Format(wxT("File '%s' does not exist or generator not established"), sFilePath.c_str()));
    }
}

void pam2Dialog::InitGenerator(const wxString& sSequence)
{
    if(m_pGenerator)
    {
        if(sSequence != m_sCurrentSequence)
        {
            m_pGenerator->ClearSequences();

            m_sCurrentSequence = sSequence;

            wxXmlDocument doc;
            if(doc.Load(wxString::Format(wxT("%s/generator/%s.xml"), Settings::Get().GetDocumentDirectory().c_str(), sSequence.c_str())) && doc.GetRoot())
            {
                for(wxXmlNode* pSequenceNode = doc.GetRoot()->GetChildren(); pSequenceNode; pSequenceNode = pSequenceNode->GetNext())
                {
                    if(pSequenceNode->GetName().CmpNoCase(wxT("sequence")) == 0)
                    {
                        unsigned long nChannels(0);
                        pSequenceNode->GetAttribute(wxT("channels"), wxT("0")).ToULong(&nChannels);
                        Sequence* pSequence = new Sequence(nChannels);

                        for(wxXmlNode* pFreqGenNode = pSequenceNode->GetChildren(); pFreqGenNode; pFreqGenNode = pFreqGenNode->GetNext())
                        {
                            if(pFreqGenNode->GetName().CmpNoCase(wxT("genfreq")) == 0)
                            {
                                double dFrequency, ddBFS;
                                long nCycles(0), nType(0);
                                if(pFreqGenNode->GetAttribute(wxT("frequency"), wxEmptyString).ToDouble(&dFrequency) && pFreqGenNode->GetAttribute(wxT("dBFS"), wxEmptyString).ToDouble(&ddBFS) && pFreqGenNode->GetAttribute(wxT("cycles"), wxEmptyString).ToLong(&nCycles) && pFreqGenNode->GetAttribute(wxT("type"), wxT("0")).ToLong(&nType))
                                {
                                    pSequence->AppendGenFreq(dFrequency, ddBFS, nCycles, nType);
                                }
                            }
                        }
                        m_pGenerator->AddSequence(pSequenceNode->GetAttribute(wxT("name"), wxEmptyString), pSequence);
                    }
                }
                wmLog::Get()->Log(wxString::Format(wxT("Generating sequence file %s"), sSequence.c_str()));
            }
            else
            {
                wmLog::Get()->Log(wxString::Format(wxT("Could not open sequence file %s"), sSequence.c_str()));
            }

            CreateSessionFromOutput(Settings::Get().Read(wxT("Output"), wxT("Sequence"), wxT("glits")));
            CheckPlayback(m_pGenerator->GetSampleRate(), m_pGenerator->GetChannels());

            m_pGenerator->Generate(8192);
        }
    }
}


void pam2Dialog::InitGenerator()
{
    if(m_pGenerator)
    {
        m_pGenerator->SetFrequency(Settings::Get().Read(wxT("Generator"), wxT("Frequency"), 1000), Settings::Get().Read(wxT("Generator"), wxT("Amplitude"), -18.0), Settings::Get().Read(wxT("Generator"), wxT("Shape"), 0));

        wmLog::Get()->Log(wxString::Format(wxT("Generating fixed frequency %dHz at %.1fdB"),Settings::Get().Read(wxT("Generator"), wxT("Frequency"), 1000), Settings::Get().Read(wxT("Generator"), wxT("Amplitude"), -18.0)));

        CreateSessionFromOutput(wxString::Format(wxT("%dHz %.1fdBFS"), Settings::Get().Read(wxT("Generator"), wxT("Frequency"), 1000), Settings::Get().Read(wxT("Generator"), wxT("Amplitude"), -18.0)));
        CheckPlayback(m_pGenerator->GetSampleRate(), m_pGenerator->GetChannels());

        m_pGenerator->Generate(8192);
    }
}

void pam2Dialog::OutputChanged(const wxString& sKey)
{
    if(sKey == wxT("Enabled"))
    {
        CreateAudioDevice(SoundcardManager::Get().GetOutputSampleRate());

//        if(m_nPlaybackSource != AudioEvent::SOUNDCARD && m_nPlaybackSource !=AudioEvent::RTP)
//        {
//            m_pGenerator->Generate(8192);
//        }
    }
    else if(sKey == wxT("Source"))
    {
        wxString sType(Settings::Get().Read(wxT("Output"), wxT("Source"), wxT("Input")));

        if(sType == wxT("File"))
        {
            m_nPlaybackSource = AudioEvent::FILE;
            wmLog::Get()->Log(wxT("Create Audio Output Generator: File"));
            OpenFileForReading();
        }
        else if(sType == wxT("Sequence"))
        {
            m_nPlaybackSource = AudioEvent::GENERATOR;
            wmLog::Get()->Log(wxT("Create Audio Output Generator: Sequence"));
            InitGenerator(Settings::Get().Read(wxT("Output"), wxT("Sequence"), wxT("glits")));
        }
        else if(sType == wxT("Generator"))
        {
            m_nPlaybackSource = AudioEvent::GENERATOR;
            wmLog::Get()->Log(wxT("Create Audio Output Generator: Generator"));
            InitGenerator();
        }
        else  if(sType == wxT("Noise"))
        {
            m_nPlaybackSource = AudioEvent::GENERATOR;
            InitNoiseGenerator();
        }
        else if(sType == wxT("Input"))
        {
            m_nPlaybackSource = m_nMonitorSource;
        }
        m_plblOutput->SetLabel(sType);
    }
    else if(sKey == wxT("File") && Settings::Get().Read(wxT("Output"), wxT("Source"), wxT("Input")) == wxT("File"))
    {
        wmLog::Get()->Log(wxT("Change Audio Output Generator: File"));
        OpenFileForReading();
    }
    else if(sKey == wxT("Sequence") && Settings::Get().Read(wxT("Output"), wxT("Source"), wxT("Input")) == wxT("Sequence"))
    {
        wmLog::Get()->Log(wxT("Change Audio Output Generator: Sequence"));
        InitGenerator(Settings::Get().Read(wxT("Output"), wxT("Sequence"), wxT("glits")));
    }

}

void pam2Dialog::OnRTPSessionClosed(wxCommandEvent& event)
{
    wxString sTest = event.GetString();
    sTest.Replace(wxT("%20"), wxT(" "));

    if(m_sCurrentRtp == sTest)
    {
        m_sCurrentRtp = wxEmptyString;
    }
    m_setRtpOrphan.erase(sTest);
    m_mRtp.erase(sTest);

    PopulateThreadList();
}

void pam2Dialog::OnRTPSession(wxCommandEvent& event)
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

void pam2Dialog::CheckPlayback(unsigned long nSampleRate, unsigned long nChannels)
{
    if(m_nPlaybackSource == AudioEvent::RTP || m_nPlaybackSource == AudioEvent::SOUNDCARD)
    {
        //check the stream details against the playing details...
        if(SoundcardManager::Get().IsOutputStreamOpen() && (SoundcardManager::Get().GetOutputSampleRate() != nSampleRate || SoundcardManager::Get().GetOutputNumberOfChannels() != nChannels))
        {
            vector<char> vChannels;
            vChannels.push_back(Settings::Get().Read(wxT("Output"), wxT("Left"), 0));
            vChannels.push_back(Settings::Get().Read(wxT("Output"), wxT("Right"), 1));
            SoundcardManager::Get().SetOutputMixer(vChannels, nChannels);
            TellPluginsAboutOutputChannels();

            CreateAudioDevice(nSampleRate);    // @todo should we allow multichannel cards here??
        }
    }
    else
    {
        vector<char> vChannels;
        vChannels.push_back(Settings::Get().Read(wxT("Output"), wxT("Left"), 0));
        vChannels.push_back(Settings::Get().Read(wxT("Output"), wxT("Right"), 1));
        SoundcardManager::Get().SetOutputMixer(vChannels, nChannels);
        TellPluginsAboutOutputChannels();
    }
}


//void pam2Dialog::CreateAudioOutputDevice(unsigned long nSampleRate, unsigned long nChannels)
//{
//    if(Settings::Get().Read(wxT("Output"), wxT("enabled"), int(1)) == 1)
//    {
//        if(nSampleRate == 0)
//        {
//            nSampleRate = m_pPlayback->GetSampleRate();
//        }
//        if(nChannels == 0)
//        {
//            nChannels = m_pPlayback->GetChannels();
//        }
//        if(m_pPlayback->OpenPlayback(Settings::Get().Read(wxT("Output"), wxT("Device"),0), nSampleRate, nChannels, Settings::Get().Read(wxT("Output"), wxT("Buffer"), (int)0)) == true)
//        {
//            wmLog::Get()->Log(wxString::Format(wxT("Playback started: Latency %d"), m_pPlayback->GetLatency()));
//        }
//    }
//}


void pam2Dialog::OnMonitorRequest(MonitorEvent& event)
{

    if(event.GetChannels().size() >=2)
    {
        unsigned int nInputChannels(0);
        if(m_Session.itCurrentSubsession != m_Session.lstSubsession.end())
        {
            nInputChannels = m_Session.itCurrentSubsession->nChannels;
        }
        SoundcardManager::Get().SetOutputMixer(event.GetChannels(), nInputChannels);
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
        itMonitor->second->OutputChannels(SoundcardManager::Get().GetOutputChannels());
    }

    for(map<wxString, TestPluginBuilder*>::iterator itTest = TestPluginFactory::Get()->GetPluginBegin(); itTest != TestPluginFactory::Get()->GetPluginEnd(); ++itTest)
    {
        itTest->second->OutputChannels(SoundcardManager::Get().GetOutputChannels());
    }

}


void pam2Dialog::ClearSession()
{
    InputSession(session());
    CheckPlayback(48000,0);
}




void pam2Dialog::OntimerStartTrigger(wxTimerEvent& event)
{
    wmLog::Get();

    RemoveOldFiles();


    LoadMonitorPanels();
    //check which page we need.
    wxString sPanel(Settings::Get().Read(wxT("Main"), wxT("Monitor"), wxEmptyString));

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
        m_plstScreens->SelectButton(sPanel);
    }
    else
    {
        m_plstInbuilt->SelectButton(sPanel);
    }


    OutputChanged(wxT("Enabled"));
    OutputChanged(wxT("Source"));

    m_pGenerator = new Generator();
    m_pGenerator->SetSampleRate(48000);

    CreateAudioInputDevice();
}

void pam2Dialog::Onm_timerFileTrigger(wxTimerEvent& event)
{
}



void pam2Dialog::PopulateThreadList()
{
    m_ppnlSettings->m_plstThreads->Freeze();
    m_ppnlSettings->m_plstThreads->Clear();
    for(map<wxString, RtpThread*>::iterator itThread = m_mRtp.begin(); itThread != m_mRtp.end(); ++itThread)
    {
        int nButton = m_ppnlSettings->m_plstThreads->AddButton(itThread->first);
        if(m_setRtpOrphan.find(itThread->first) != m_setRtpOrphan.end())
        {
            m_ppnlSettings->m_plstThreads->SetButtonColour(nButton, wxColour(200,0,0));
        }
    }
    m_ppnlSettings->m_plstThreads->Thaw();
}

void pam2Dialog::OntimerIpcTrigger(wxTimerEvent& event)
{
    if(m_pClient && m_pClient->IsConnected())
    {
        m_pClient->Poke();
    }
}

void pam2Dialog::OnClose(wxCloseEvent& event)
{
    wmLog::Get()->Log(wxT("Closing"));
    if(m_pClient && m_pClient->IsConnected())
    {
        m_pClient->Disconnect();
        delete m_pClient;
        m_pClient = NULL;
    }
    event.Skip();
}

void pam2Dialog::OnbmpSplashClick(wxCommandEvent& event)
{
    m_pswpSplash->ChangeSelection(1);
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
    wxLogDebug(event.GetString());
    MaximizeMonitor(false);
    map<wxString, MonitorPluginBuilder*>::iterator itPlugin = MonitorPluginFactory::Get()->FindPlugin(event.GetString());
    if(itPlugin != MonitorPluginFactory::Get()->GetPluginEnd())
    {
        m_plstScreens->SelectButton(event.GetString(), false);
        Settings::Get().Write(wxT("Main"), wxT("Monitor"), event.GetString());
        ShowMonitorPanel(event.GetString());
    }
    else
    {
        map<wxString, TestPluginBuilder*>::iterator itPlugin = TestPluginFactory::Get()->FindPlugin(event.GetString());
        if(itPlugin != TestPluginFactory::Get()->GetPluginEnd())
        {
            m_plstScreens->SelectButton(wxT("Tests"), false);
            Settings::Get().Write(wxT("Main"), wxT("Monitor"), wxT("Tests"));
            ShowTestPanels();
        }
        else
        {
            m_plstScreens->SelectButton(wxT("Settings"), false);
            Settings::Get().Write(wxT("Main"), wxT("Monitor"), wxT("Settings"));
            ShowSettingsPanel();
        }
    }
}
void pam2Dialog::InitNoiseGenerator()
{
    if(m_pGenerator)
    {
        m_pGenerator->SetNoise(Settings::Get().Read(wxT("Noise"), wxT("Colour"), 0), Settings::Get().Read(wxT("Noise"), wxT("Amplitude"), -18.0));
        CreateSessionFromOutput(wxT("Noise"));
        CheckPlayback(m_pGenerator->GetSampleRate(), m_pGenerator->GetChannels());
        m_pGenerator->Generate(8192);
    }
}

