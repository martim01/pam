#include "pnlSettingsTime.h"
#include "timemanager.h"
#include "dlgEditName.h"
#include "settings.h"


//(*InternalHeaders(pnlSettingsTime)
#include <wx/font.h>
#include <wx/intl.h>
#include <wx/string.h>
//*)

//(*IdInit(pnlSettingsTime)
const long pnlSettingsTime::ID_M_PLST1 = wxNewId();
const long pnlSettingsTime::ID_M_PBTN3 = wxNewId();
const long pnlSettingsTime::ID_M_PBTN4 = wxNewId();
const long pnlSettingsTime::ID_M_PBTN5 = wxNewId();
const long pnlSettingsTime::ID_M_PBTN6 = wxNewId();
const long pnlSettingsTime::ID_PANEL1 = wxNewId();
const long pnlSettingsTime::ID_M_PLBL8 = wxNewId();
const long pnlSettingsTime::ID_M_PEDT1 = wxNewId();
const long pnlSettingsTime::ID_M_PKBD1 = wxNewId();
const long pnlSettingsTime::ID_M_PBTN1 = wxNewId();
const long pnlSettingsTime::ID_PANEL2 = wxNewId();
const long pnlSettingsTime::ID_M_PLBL13 = wxNewId();
const long pnlSettingsTime::ID_M_PLST2 = wxNewId();
const long pnlSettingsTime::ID_PANEL3 = wxNewId();
const long pnlSettingsTime::ID_M_PSWP1 = wxNewId();
const long pnlSettingsTime::ID_M_PLST3 = wxNewId();
const long pnlSettingsTime::ID_M_PLBL1 = wxNewId();
const long pnlSettingsTime::ID_TIMER1 = wxNewId();
//*)

BEGIN_EVENT_TABLE(pnlSettingsTime,wxPanel)
	//(*EventTable(pnlSettingsTime)
	//*)
END_EVENT_TABLE()

pnlSettingsTime::pnlSettingsTime(wxWindow* parent,wxWindowID id,const wxPoint& pos,const wxSize& size, long nStyle, const wxString& sId)
{
	//(*Initialize(pnlSettingsTime)
	Create(parent, id, wxDefaultPosition, wxSize(600,400), wxTAB_TRAVERSAL, _T("id"));
	SetBackgroundColour(wxColour(0,0,0));
	m_pswpSettings = new wmSwitcherPanel(this, ID_M_PSWP1, wxPoint(0,110), wxSize(600,330), wmSwitcherPanel::STYLE_NOSWIPE|wmSwitcherPanel::STYLE_NOANIMATION, _T("ID_M_PSWP1"));
	m_pswpSettings->SetPageNameStyle(3);
	m_ppnlNTP = new wxPanel(m_pswpSettings, ID_PANEL1, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL, _T("ID_PANEL1"));
	m_ppnlNTP->SetBackgroundColour(wxColour(0,0,0));
	m_plstNTPServers = new wmList(m_ppnlNTP, ID_M_PLST1, wxPoint(0,10), wxSize(600,240), wmList::STYLE_SELECT, 0, wxSize(-1,40), 3, wxSize(5,5));
	m_plstNTPServers->SetBackgroundColour(wxColour(0,0,0));
	m_plstNTPServers->SetButtonColour(wxColour(wxT("#005555")));
	m_plstNTPServers->SetSelectedButtonColour(wxColour(wxT("#008000")));
	m_plstNTPServers->SetDisabledColour(wxColour(wxT("#808080")));
	m_pbtnNtpServerAdd = new wmButton(m_ppnlNTP, ID_M_PBTN3, _("Add Server"), wxPoint(70,250), wxSize(100,40), 0, wxDefaultValidator, _T("ID_M_PBTN3"));
	m_pbtnNtpServerAdd->SetColourDisabled(wxColour(wxT("#808080")));
	m_pbtnNtpServerEdit = new wmButton(m_ppnlNTP, ID_M_PBTN4, _("Edit Server"), wxPoint(180,250), wxSize(100,40), 0, wxDefaultValidator, _T("ID_M_PBTN4"));
	m_pbtnNtpServerEdit->Disable();
	m_pbtnNtpServerEdit->SetColourDisabled(wxColour(wxT("#808080")));
	m_pbtnNtpServerDelete = new wmButton(m_ppnlNTP, ID_M_PBTN5, _("Delete Server"), wxPoint(290,250), wxSize(100,40), 0, wxDefaultValidator, _T("ID_M_PBTN5"));
	m_pbtnNtpServerDelete->Disable();
	m_pbtnNtpServerDelete->SetColourDisabled(wxColour(wxT("#808080")));
	m_pbtnNTPServerDeleteAll = new wmButton(m_ppnlNTP, ID_M_PBTN6, _("Delete All"), wxPoint(400,250), wxSize(100,40), 0, wxDefaultValidator, _T("ID_M_PBTN6"));
	m_pbtnNTPServerDeleteAll->SetColourDisabled(wxColour(wxT("#808080")));
	m_ppnlPTP = new wxPanel(m_pswpSettings, ID_PANEL2, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL, _T("ID_PANEL2"));
	m_ppnlPTP->SetBackgroundColour(wxColour(0,0,0));
	m_pLbl1 = new wmLabel(m_ppnlPTP, ID_M_PLBL8, _("Domain:"), wxPoint(10,20), wxSize(100,25), 0, _T("ID_M_PLBL8"));
	m_pLbl1->SetBorderState(uiRect::BORDER_NONE);
	m_pLbl1->GetUiRect().SetGradient(0);
	m_pLbl1->SetForegroundColour(wxColour(255,255,255));
	m_pLbl1->SetBackgroundColour(wxColour(0,64,0));
	wxFont m_pLbl1Font(10,wxFONTFAMILY_SWISS,wxFONTSTYLE_NORMAL,wxFONTWEIGHT_BOLD,false,_T("Arial"),wxFONTENCODING_DEFAULT);
	m_pLbl1->SetFont(m_pLbl1Font);
	m_pedtDomain = new wmEdit(m_ppnlPTP, ID_M_PEDT1, wxEmptyString, wxPoint(110,20), wxDefaultSize, 0, wxDefaultValidator, _T("ID_M_PEDT1"));
	m_pedtDomain->SetValidation(4);
	m_pedtDomain->SetFocus();
	m_pedtDomain->SetBackgroundColour(wxColour(255,255,255));
	m_pedtDomain->SetBorderStyle(1,1);
	m_pKbd1 = new wmKeyboard(m_ppnlPTP, ID_M_PKBD1, wxPoint(10,60), wxSize(288,208), 5, 0);
	m_pKbd1->SetForegroundColour(wxColour(255,255,255));
	m_pbtnSync = new wmButton(m_ppnlPTP, ID_M_PBTN1, _("Sync To"), wxPoint(350,60), wxSize(200,40), 0, wxDefaultValidator, _T("ID_M_PBTN1"));
	m_pbtnSync->SetBackgroundColour(wxColour(0,128,64));
	m_pbtnSync->SetColourDisabled(wxColour(wxT("#808080")));
	m_pbtnSync->SetToggleLook(true, wxT("UTC"), wxT("TAI"), 60);
	m_ppnlLTCS = new wxPanel(m_pswpSettings, ID_PANEL3, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL, _T("ID_PANEL3"));
	m_ppnlLTCS->SetBackgroundColour(wxColour(0,0,0));
	m_plblListTitle = new wmLabel(m_ppnlLTCS, ID_M_PLBL13, _("Format:"), wxPoint(0,10), wxSize(200,44), 0, _T("ID_M_PLBL13"));
	m_plblListTitle->SetBorderState(uiRect::BORDER_NONE);
	m_plblListTitle->GetUiRect().SetGradient(0);
	m_plblListTitle->SetForegroundColour(wxColour(255,255,255));
	m_plblListTitle->SetBackgroundColour(wxColour(0,0,0));
	wxFont m_plblListTitleFont(22,wxFONTFAMILY_SWISS,wxFONTSTYLE_NORMAL,wxFONTWEIGHT_NORMAL,false,_T("Courier New"),wxFONTENCODING_DEFAULT);
	m_plblListTitle->SetFont(m_plblListTitleFont);
	m_plstDate = new wmList(m_ppnlLTCS, ID_M_PLST2, wxPoint(200,10), wxSize(400,44), wmList::STYLE_SELECT, 0, wxSize(-1,-1), 5, wxSize(3,1));
	m_plstDate->SetBackgroundColour(wxColour(0,0,0));
	m_plstDate->SetButtonColour(wxColour(wxT("#004000")));
	m_plstDate->SetSelectedButtonColour(wxColour(wxT("#FF8000")));
	m_pswpSettings->AddPage(m_ppnlNTP, _("NTP Servers"), false);
	m_pswpSettings->AddPage(m_ppnlPTP, _("PTP Domain"), false);
	m_pswpSettings->AddPage(m_ppnlLTCS, _("LTC Settings"), false);
	m_plstSync = new wmList(this, ID_M_PLST3, wxPoint(10,10), wxSize(580,45), wmList::STYLE_SELECT, 0, wxSize(-1,-1), 4, wxSize(-1,-1));
	m_plstSync->SetBackgroundColour(wxColour(0,0,0));
	m_plstSync->SetButtonColour(wxColour(wxT("#00006A")));
	m_plstSync->SetSelectedButtonColour(wxColour(wxT("#FF8000")));
	m_plblTime = new wmLabel(this, ID_M_PLBL1, _("2021-12-12 12:22:30"), wxPoint(100,60), wxSize(400,40), 0, _T("ID_M_PLBL1"));
	m_plblTime->SetBorderState(uiRect::BORDER_NONE);
	m_plblTime->GetUiRect().SetGradient(0);
	m_plblTime->SetForegroundColour(wxColour(0,128,0));
	wxFont m_plblTimeFont(16,wxFONTFAMILY_SWISS,wxFONTSTYLE_NORMAL,wxFONTWEIGHT_BOLD,false,_T("Tahoma"),wxFONTENCODING_DEFAULT);
	m_plblTime->SetFont(m_plblTimeFont);
	m_timerTime.SetOwner(this, ID_TIMER1);
	m_timerTime.Start(500, false);

	Connect(ID_M_PLST1,wxEVT_LIST_SELECTED,(wxObjectEventFunction)&pnlSettingsTime::OnlstNTPServersSelected);
	Connect(ID_M_PBTN3,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&pnlSettingsTime::OnbtnNtpServerAddClick);
	Connect(ID_M_PBTN4,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&pnlSettingsTime::OnbtnNtpServerEditClick);
	Connect(ID_M_PBTN5,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&pnlSettingsTime::OnbtnNtpServerDeleteClick);
	Connect(ID_M_PBTN6,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&pnlSettingsTime::OnbtnNTPServerDeleteAllClick);
	Connect(ID_M_PEDT1,wxEVT_COMMAND_TEXT_ENTER,(wxObjectEventFunction)&pnlSettingsTime::OnedtDomainTextEnter);
	Connect(ID_M_PLST2,wxEVT_LIST_SELECTED,(wxObjectEventFunction)&pnlSettingsTime::OnlstDateSelected);
	Connect(ID_M_PLST3,wxEVT_LIST_SELECTED,(wxObjectEventFunction)&pnlSettingsTime::OnlstSyncSelected);
	Connect(ID_TIMER1,wxEVT_TIMER,(wxObjectEventFunction)&pnlSettingsTime::OntimerTimeTrigger);
	//*)

	for(auto pairServer : TimeManager::Get().GetNtpServers())
    {
        if(pairServer.second)
        {
            m_plstNTPServers->AddButton(wxString::Format("[%s]", pairServer.first.c_str()));
        }
        else
        {
            m_plstNTPServers->AddButton(pairServer.first);
        }
    }

    m_plstSync->AddButton("Off");
    m_plstSync->AddButton("NTP");
    m_plstSync->AddButton("LTC");
    m_plstSync->AddButton("PTP");
    m_plstSync->ConnectToSetting("Time","Sync",(int)0);

    m_pswpSettings->ChangeSelection("PTP Domain");

    if(TimeManager::Get().HasNTP() == false)
    {
        m_plstSync->EnableButton(1, wmList::wmDISABLED);
        m_pswpSettings->DeletePage("NTP Servers");
    }
    //currently no LTC syncing
    m_plstSync->EnableButton(2, wmList::wmDISABLED);
    m_pswpSettings->DeletePage("LTC Settings");


    m_nSelectedServer = -1;

    m_pedtDomain->SetValue(Settings::Get().Read("Time","PTP_Domain","0"));

    m_plstDate->AddButton("Auto");
    m_plstDate->AddButton("SMPTE");
    m_plstDate->AddButton("BBC");
    m_plstDate->AddButton("TVE");
    m_plstDate->AddButton("MTD");
    m_plstDate->ConnectToSetting("Time", "LTC_Format", (int)2);

    m_pbtnSync->ConnectToSetting("Time", "Tai", false);
}

pnlSettingsTime::~pnlSettingsTime()
{
	//(*Destroy(pnlSettingsTime)
	//*)
}


void pnlSettingsTime::OnlstNTPServersSelected(wxCommandEvent& event)
{
    m_pbtnNtpServerEdit->Enable();
    m_pbtnNtpServerDelete->Enable();
    m_nSelectedServer = event.GetInt();
}

void pnlSettingsTime::OnbtnNtpServerAddClick(wxCommandEvent& event)
{
    dlgEditName aDlg(this, "", dlgEditName::CH_SERVER);
    if(aDlg.ShowModal() == wxID_OK)
    {
        if(aDlg.m_nSelectedChannel == 1)
        {
            m_plstNTPServers->AddButton(aDlg.m_pedtName->GetValue());
        }
        else
        {
            m_plstNTPServers->AddButton(wxString::Format("[%s]", aDlg.m_pedtName->GetValue().c_str()));
        }
        m_plstNTPServers->Refresh();
        SaveNtpServers();
    }
}

void pnlSettingsTime::OnbtnNtpServerEditClick(wxCommandEvent& event)
{
    if(m_nSelectedServer != -1)
    {
        wxString sName(m_plstNTPServers->GetButtonText(m_nSelectedServer));
        int nChannel(dlgEditName::CH_SERVER);
        if(sName.GetChar(0) == '[')
        {
            sName = sName.After('[').Before(']');
            nChannel = dlgEditName::CH_POOL;
        }
        dlgEditName aDlg(this, sName, nChannel);
        if(aDlg.ShowModal() == wxID_OK)
        {
            if(aDlg.m_nSelectedChannel == 1)
            {
                m_plstNTPServers->SetButtonText(m_nSelectedServer, aDlg.m_pedtName->GetValue());
            }
            else
            {
                m_plstNTPServers->SetButtonText(m_nSelectedServer, wxString::Format("[%s]", aDlg.m_pedtName->GetValue().c_str()));
            }
            m_plstNTPServers->Refresh();
            SaveNtpServers();
        }
    }
}

void pnlSettingsTime::OnbtnNtpServerDeleteClick(wxCommandEvent& event)
{
    if(m_nSelectedServer != -1)
    {
        m_plstNTPServers->DeleteButton(m_nSelectedServer);
        m_pbtnNtpServerEdit->Enable(false);
        m_pbtnNtpServerDelete->Enable(false);
        m_nSelectedServer = -1;

        SaveNtpServers();
    }
}

void pnlSettingsTime::OnbtnNTPServerDeleteAllClick(wxCommandEvent& event)
{
    m_plstNTPServers->Clear();
    m_pbtnNtpServerEdit->Enable(false);
    m_pbtnNtpServerDelete->Enable(false);
    m_nSelectedServer = -1;

    TimeManager::Get().SetNtpServers(std::map<wxString,bool>());
}

void pnlSettingsTime::SaveNtpServers()
{
    std::map<wxString,bool> mServers;
    for(size_t i = 0; i < m_plstNTPServers->GetItemCount(); i++)
    {
        if(m_plstNTPServers->GetButtonText(i).GetChar(0) == '[')
        {
            mServers.insert(std::make_pair(m_plstNTPServers->GetButtonText(i).After('[').Before(']'), true));
        }
        else
        {
            mServers.insert(std::make_pair(m_plstNTPServers->GetButtonText(i), false));
        }
    }
    TimeManager::Get().SetNtpServers(mServers);
}

void pnlSettingsTime::OnedtDomainTextEnter(wxCommandEvent& event)
{
    unsigned long nDomain;
    if(m_pedtDomain->GetValue().ToULong(&nDomain) && nDomain < 128)
    {
        Settings::Get().Write("Time", "PTP_Domain", (unsigned int)nDomain);
    }
}

void pnlSettingsTime::OnlstDateSelected(wxCommandEvent& event)
{
//    Settings::Get().Write("Time", "LTC_Format", event.GetInt());
}

void pnlSettingsTime::OnlstSyncSelected(wxCommandEvent& event)
{
  //  Settings::Get().Write("Time", "Sync", event.GetInt());
}

void pnlSettingsTime::OntimerTimeTrigger(wxTimerEvent& event)
{
    m_plblTime->SetLabel(wxDateTime::Now().Format("%Y-%m-%d %H:%M:%S"));
}
