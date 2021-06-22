#include "pnlSettingsTime.h"

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

pnlSettingsTime::pnlSettingsTime(wxWindow* parent,wxWindowID id,const wxPoint& pos,const wxSize& size)
{
	//(*Initialize(pnlSettingsTime)
	Create(parent, id, wxDefaultPosition, wxSize(600,400), wxTAB_TRAVERSAL, _T("id"));
	SetBackgroundColour(wxColour(0,0,0));
	m_pbtnPTP = new wmButton(this, ID_M_PBTN22, _("PTP"), wxPoint(10,10), wxSize(200,40), wmButton::STYLE_SELECT, wxDefaultValidator, _T("ID_M_PBTN22"));
	m_pbtnPTP->SetForegroundColour(wxColour(255,255,255));
	m_pbtnPTP->SetBackgroundColour(wxColour(0,128,0));
	m_pbtnPTP->SetToggleLook(true, wxT("Off"), wxT("Sync"), 50);
	m_pbtnLTC = new wmButton(this, ID_M_PBTN1, _("LTC"), wxPoint(240,10), wxSize(200,40), wmButton::STYLE_SELECT, wxDefaultValidator, _T("ID_M_PBTN1"));
	m_pbtnLTC->SetForegroundColour(wxColour(255,255,255));
	m_pbtnLTC->SetBackgroundColour(wxColour(0,128,0));
	m_pbtnLTC->SetToggleLook(true, wxT("Off"), wxT("Sync"), 50);
	m_pbtnNTP = new wmButton(this, ID_M_PBTN2, _("NTP"), wxPoint(10,60), wxSize(200,40), wmButton::STYLE_SELECT, wxDefaultValidator, _T("ID_M_PBTN2"));
	m_pbtnNTP->SetForegroundColour(wxColour(255,255,255));
	m_pbtnNTP->SetBackgroundColour(wxColour(0,128,0));
	m_pbtnNTP->SetToggleLook(true, wxT("Off"), wxT("Sync"), 50);
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
}

pnlSettingsTime::~pnlSettingsTime()
{
	//(*Destroy(pnlSettingsTime)
	//*)
}


void pnlSettingsTime::OnbtnPTPClick(wxCommandEvent& event)
{
}

void pnlSettingsTime::OnbtnLTCClick(wxCommandEvent& event)
{
}

void pnlSettingsTime::OnbtnNTPClick(wxCommandEvent& event)
{
}

void pnlSettingsTime::OnlstNTPServersSelected(wxCommandEvent& event)
{
}

void pnlSettingsTime::OnbtnNtpServerAddClick(wxCommandEvent& event)
{
}

void pnlSettingsTime::OnbtnNtpServerEditClick(wxCommandEvent& event)
{
}

void pnlSettingsTime::OnbtnNtpServerDeleteClick(wxCommandEvent& event)
{
}

void pnlSettingsTime::OnbtnNTPServerDeleteAllClick(wxCommandEvent& event)
{
}
