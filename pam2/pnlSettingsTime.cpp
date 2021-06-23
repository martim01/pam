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
const long pnlSettingsTime::ID_M_PBTN22 = wxNewId();
const long pnlSettingsTime::ID_M_PBTN1 = wxNewId();
const long pnlSettingsTime::ID_M_PBTN2 = wxNewId();
const long pnlSettingsTime::ID_M_PLBL3 = wxNewId();
const long pnlSettingsTime::ID_M_PLST1 = wxNewId();
const long pnlSettingsTime::ID_M_PBTN3 = wxNewId();
const long pnlSettingsTime::ID_M_PBTN4 = wxNewId();
const long pnlSettingsTime::ID_M_PBTN5 = wxNewId();
const long pnlSettingsTime::ID_M_PBTN6 = wxNewId();
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
	m_pbtnPTP = new wmButton(this, ID_M_PBTN22, _("PTP"), wxPoint(10,10), wxSize(200,40), wmButton::STYLE_SELECT, wxDefaultValidator, _T("ID_M_PBTN22"));
	m_pbtnPTP->SetForegroundColour(wxColour(255,255,255));
	m_pbtnPTP->SetBackgroundColour(wxColour(0,128,0));
	m_pbtnPTP->SetToggle(true, wxT("Off"), wxT("Sync"), 50);
	m_pbtnLTC = new wmButton(this, ID_M_PBTN1, _("LTC"), wxPoint(240,10), wxSize(200,40), wmButton::STYLE_SELECT, wxDefaultValidator, _T("ID_M_PBTN1"));
	m_pbtnLTC->SetForegroundColour(wxColour(255,255,255));
	m_pbtnLTC->SetBackgroundColour(wxColour(0,128,0));
	m_pbtnLTC->SetToggle(true, wxT("Off"), wxT("Sync"), 50);
	m_pbtnNTP = new wmButton(this, ID_M_PBTN2, _("NTP"), wxPoint(10,60), wxSize(200,40), wmButton::STYLE_SELECT, wxDefaultValidator, _T("ID_M_PBTN2"));
	m_pbtnNTP->SetForegroundColour(wxColour(255,255,255));
	m_pbtnNTP->SetBackgroundColour(wxColour(0,128,0));
	m_pbtnNTP->SetToggle(true, wxT("Off"), wxT("Sync"), 50);
	m_pLbl3 = new wmLabel(this, ID_M_PLBL3, _("NTP Servers"), wxPoint(0,110), wxSize(600,30), 0, _T("ID_M_PLBL3"));
	m_pLbl3->SetBorderState(uiRect::BORDER_NONE);
	m_pLbl3->GetUiRect().SetGradient(0);
	m_pLbl3->SetForegroundColour(wxColour(255,255,255));
	m_pLbl3->SetBackgroundColour(wxColour(0,64,0));
	wxFont m_pLbl3Font(10,wxFONTFAMILY_SWISS,wxFONTSTYLE_NORMAL,wxFONTWEIGHT_BOLD,false,_T("Arial"),wxFONTENCODING_DEFAULT);
	m_pLbl3->SetFont(m_pLbl3Font);
	m_plstNTPServers = new wmList(this, ID_M_PLST1, wxPoint(0,140), wxSize(600,250), wmList::STYLE_SELECT, 0, wxSize(-1,40), 3, wxSize(5,5));
	m_plstNTPServers->SetBackgroundColour(wxColour(0,0,0));
	m_plstNTPServers->SetSelectedButtonColour(wxColour(wxT("#008000")));
	m_plstNTPServers->SetDisabledColour(wxColour(wxT("#808080")));
	m_pbtnNtpServerAdd = new wmButton(this, ID_M_PBTN3, _("Add Server"), wxPoint(70,400), wxSize(100,40), 0, wxDefaultValidator, _T("ID_M_PBTN3"));
	m_pbtnNtpServerAdd->SetColourDisabled(wxColour(wxT("#808080")));
	m_pbtnNtpServerEdit = new wmButton(this, ID_M_PBTN4, _("Edit Server"), wxPoint(180,400), wxSize(100,40), 0, wxDefaultValidator, _T("ID_M_PBTN4"));
	m_pbtnNtpServerEdit->Disable();
	m_pbtnNtpServerEdit->SetColourDisabled(wxColour(wxT("#808080")));
	m_pbtnNtpServerDelete = new wmButton(this, ID_M_PBTN5, _("Delete Server"), wxPoint(290,400), wxSize(100,40), 0, wxDefaultValidator, _T("ID_M_PBTN5"));
	m_pbtnNtpServerDelete->Disable();
	m_pbtnNtpServerDelete->SetColourDisabled(wxColour(wxT("#808080")));
	m_pbtnNTPServerDeleteAll = new wmButton(this, ID_M_PBTN6, _("Delete All"), wxPoint(400,400), wxSize(100,40), 0, wxDefaultValidator, _T("ID_M_PBTN6"));
	m_pbtnNTPServerDeleteAll->SetColourDisabled(wxColour(wxT("#808080")));

	Connect(ID_M_PBTN22,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&pnlSettingsTime::OnbtnPTPClick);
	Connect(ID_M_PBTN1,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&pnlSettingsTime::OnbtnLTCClick);
	Connect(ID_M_PBTN2,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&pnlSettingsTime::OnbtnNTPClick);
	Connect(ID_M_PLST1,wxEVT_LIST_SELECTED,(wxObjectEventFunction)&pnlSettingsTime::OnlstNTPServersSelected);
	Connect(ID_M_PBTN3,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&pnlSettingsTime::OnbtnNtpServerAddClick);
	Connect(ID_M_PBTN4,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&pnlSettingsTime::OnbtnNtpServerEditClick);
	Connect(ID_M_PBTN5,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&pnlSettingsTime::OnbtnNtpServerDeleteClick);
	Connect(ID_M_PBTN6,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&pnlSettingsTime::OnbtnNTPServerDeleteAllClick);
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

    m_pbtnNTP->ToggleSelection(Settings::Get().Read("Time", "NTP", 1) == 1);
    m_pbtnLTC->ToggleSelection(Settings::Get().Read("Time", "LTC", 0) == 1);
    m_pbtnPTP->ToggleSelection(Settings::Get().Read("Time", "PTP", 0) == 1);

    m_nSelectedServer = -1;
}

pnlSettingsTime::~pnlSettingsTime()
{
	//(*Destroy(pnlSettingsTime)
	//*)
}


void pnlSettingsTime::OnbtnPTPClick(wxCommandEvent& event)
{
    Settings::Get().Write("Time", "PTP", event.IsChecked());
}

void pnlSettingsTime::OnbtnLTCClick(wxCommandEvent& event)
{
    Settings::Get().Write("Time", "LTC", event.IsChecked());
}

void pnlSettingsTime::OnbtnNTPClick(wxCommandEvent& event)
{
    Settings::Get().Write("Time", "NTP", event.IsChecked());
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
        if(aDlg.m_nChannels == 1)
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
            if(aDlg.m_nChannels == 1)
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
