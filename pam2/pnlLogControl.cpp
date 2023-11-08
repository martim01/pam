#include "pnlLogControl.h"
#include "settings.h"
#include "pnlLog.h"

//(*InternalHeaders(pnlLogControl)
#include <wx/intl.h>
#include <wx/string.h>
//*)

//(*IdInit(pnlLogControl)
const long pnlLogControl::ID_M_PBTN1 = wxNewId();
const long pnlLogControl::ID_M_PBTN2 = wxNewId();
const long pnlLogControl::ID_M_PBTN3 = wxNewId();
const long pnlLogControl::ID_M_PBTN4 = wxNewId();
const long pnlLogControl::ID_M_PBTN5 = wxNewId();
const long pnlLogControl::ID_M_PBTN6 = wxNewId();
//*)

BEGIN_EVENT_TABLE(pnlLogControl,wxPanel)
	//(*EventTable(pnlLogControl)
	//*)
END_EVENT_TABLE()

pnlLogControl::pnlLogControl(wxWindow* parent, pnlLog* pLogWindow, wxWindowID id,const wxPoint& pos,const wxSize& size) :
    m_ppnlLog(pLogWindow)
{
	//(*Initialize(pnlLogControl)
	Create(parent, id, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL, _T("id"));
	SetBackgroundColour(wxColour(0,0,0));

	
	

	m_pbtnHome = new wmButton(this, ID_M_PBTN1, _("Home"), wxPoint(5,0), wxSize(90,30), 0, wxDefaultValidator, _T("ID_M_PBTN1"));
	m_pbtnHome->SetBackgroundColour(wxColour(0,64,64));
	m_pbtnHome->SetColourDisabled(wxColour(wxT("#909090")));
	m_pbtnPageUp = new wmButton(this, ID_M_PBTN2, _("Page Up"), wxPoint(100,0), wxSize(90,30), 0, wxDefaultValidator, _T("ID_M_PBTN2"));
	m_pbtnPageUp->SetBackgroundColour(wxColour(0,64,64));
	m_pbtnPageUp->SetColourDisabled(wxColour(wxT("#909090")));
	m_pbtnEnd = new wmButton(this, ID_M_PBTN3, _("End"), wxPoint(5,30), wxSize(90,30), 0, wxDefaultValidator, _T("ID_M_PBTN3"));
	m_pbtnEnd->SetBackgroundColour(wxColour(0,64,64));
	m_pbtnEnd->SetColourDisabled(wxColour(wxT("#909090")));
	m_ptbnPageDown = new wmButton(this, ID_M_PBTN4, _("Page Down"), wxPoint(100,30), wxSize(90,30), 0, wxDefaultValidator, _T("ID_M_PBTN4"));
	m_ptbnPageDown->SetBackgroundColour(wxColour(0,64,64));
	m_ptbnPageDown->SetColourDisabled(wxColour(wxT("#909090")));
	m_pbtnScroll = new wmButton(this, ID_M_PBTN5, _("Scroll Lock"), wxPoint(5,65), wxSize(180,30), wmButton::STYLE_SELECT, wxDefaultValidator, _T("ID_M_PBTN5"));
	m_pbtnScroll->SetBackgroundColour(wxColour(0,0,128));
	m_pbtnScroll->SetColourSelected(wxColour(wxT("#FF0000")));
	m_ptbnClear = new wmButton(this, ID_M_PBTN6, _("Clear Log"), wxPoint(55,100), wxSize(90,30), 0, wxDefaultValidator, _T("ID_M_PBTN6"));
	m_ptbnClear->SetBackgroundColour(wxColour(128,0,0));

	m_pbtnLogToFile = new wmButton(this, wxNewId(), _("LogToFile"), wxPoint(5,132), wxSize(180,30), wmButton::STYLE_SELECT, wxDefaultValidator, _T("ID_M_PBTN5"));
	m_pbtnLogToFile->SetBackgroundColour(wxColour(0,0,128));
	m_pbtnLogToFile->SetColourSelected(wxColour(wxT("#FF0000")));

	Connect(ID_M_PBTN1,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&pnlLogControl::OnbtnHomeClick);
	Connect(ID_M_PBTN2,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&pnlLogControl::OnbtnPageUpClick);
	Connect(ID_M_PBTN3,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&pnlLogControl::OnbtnEndClick);
	Connect(ID_M_PBTN4,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&pnlLogControl::OntbnPageDownClick);
	Connect(ID_M_PBTN5,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&pnlLogControl::OnbtnScrollClick);
	Connect(ID_M_PBTN6,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&pnlLogControl::OntbnClearClick);
	//*)

	m_plstFilter = new wmList(this, wxNewId(), wxPoint(5,166), wxSize(180, 80), wmList::STYLE_SELECT, wmList::SCROLL_NONE, wxSize(-1,30), 3, wxSize(2,2));
	m_plstFilter->AddButton("Trace");
	m_plstFilter->AddButton("Debug");
	m_plstFilter->AddButton("Info");
	m_plstFilter->AddButton("Warning");
	m_plstFilter->AddButton("Error");
	m_plstFilter->AddButton("Critical");

	m_plstFilter->SetBackgroundColour(*wxBLACK);
    m_plstFilter->ConnectToSetting("Log", "Level", size_t(2));

	Connect(m_plstFilter->GetId(), wxEVT_LIST_SELECTED, (wxObjectEventFunction)&pnlLogControl::OnlstFilterSelected);

	m_pbtnScroll->SetToggle(true, wxT("Scroll"), wxT("Lock"), 50.0);
    m_pbtnScroll->ConnectToSetting("Log", "ScrollLock", false);

	m_pbtnLogToFile->SetToggle(true, wxT("Off"), wxT("On"), 50.0);
    m_pbtnLogToFile->ConnectToSetting("Log", "LogToFile", false);

	
}

pnlLogControl::~pnlLogControl()
{
	//(*Destroy(pnlLogControl)
	//*)
}


void pnlLogControl::OnbtnHomeClick(wxCommandEvent& event)
{
    m_ppnlLog->Home();
}

void pnlLogControl::OnbtnPageUpClick(wxCommandEvent& event)
{
    m_ppnlLog->PageUp();
}

void pnlLogControl::OnbtnEndClick(wxCommandEvent& event)
{
    m_ppnlLog->End();
}

void pnlLogControl::OntbnPageDownClick(wxCommandEvent& event)
{
    m_ppnlLog->PageDown();
}

void pnlLogControl::OnbtnScrollClick(wxCommandEvent& event)
{

}

void pnlLogControl::OntbnClearClick(wxCommandEvent& event)
{
    m_ppnlLog->Clear();
}


void pnlLogControl::UpdateControl(size_t nCurrentPage, size_t nTotalPages)
{
    m_pbtnHome->Enable((nCurrentPage != 0));
    m_pbtnPageUp->Enable((nCurrentPage != 0));
    m_ptbnPageDown->Enable((nCurrentPage != nTotalPages));
    m_pbtnEnd->Enable((nCurrentPage != nTotalPages));
}


void pnlLogControl::OnlstFilterSelected(wxCommandEvent& event)
{
    //Settings::Get().Write("Log","Level", event.GetInt());
}
