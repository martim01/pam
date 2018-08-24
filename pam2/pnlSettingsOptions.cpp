#include <wx/app.h>
#include "pnlSettingsOptions.h"

//(*InternalHeaders(pnlSettingsOptions)
#include <wx/font.h>
#include <wx/intl.h>
#include <wx/string.h>
//*)

//(*IdInit(pnlSettingsOptions)
const long pnlSettingsOptions::ID_M_PBTN1 = wxNewId();
const long pnlSettingsOptions::ID_M_PBTN2 = wxNewId();
const long pnlSettingsOptions::ID_M_PBTN3 = wxNewId();
//*)

BEGIN_EVENT_TABLE(pnlSettingsOptions,wxPanel)
	//(*EventTable(pnlSettingsOptions)
	//*)
END_EVENT_TABLE()

pnlSettingsOptions::pnlSettingsOptions(wxWindow* parent,wxWindowID id,const wxPoint& pos,const wxSize& size)
{
	//(*Initialize(pnlSettingsOptions)
	Create(parent, id, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL, _T("id"));
	SetBackgroundColour(wxColour(0,0,0));
	m_pbtnExit = new wmButton(this, ID_M_PBTN1, _("Hold to Exit"), wxPoint(50,20), wxSize(100,60), wmButton::STYLE_HOLD, wxDefaultValidator, _T("ID_M_PBTN1"));
	m_pbtnExit->SetBackgroundColour(wxColour(128,0,0));
	wxFont m_pbtnExitFont(12,wxFONTFAMILY_SWISS,wxFONTSTYLE_NORMAL,wxFONTWEIGHT_BOLD,false,_T("Arial"),wxFONTENCODING_DEFAULT);
	m_pbtnExit->SetFont(m_pbtnExitFont);
	m_pbtnReboot = new wmButton(this, ID_M_PBTN2, _("Hold To Reboot"), wxPoint(50,90), wxSize(100,60), wmButton::STYLE_HOLD, wxDefaultValidator, _T("ID_M_PBTN2"));
	m_pbtnReboot->SetBackgroundColour(wxColour(128,0,0));
	wxFont m_pbtnRebootFont(12,wxFONTFAMILY_SWISS,wxFONTSTYLE_NORMAL,wxFONTWEIGHT_BOLD,false,_T("Arial"),wxFONTENCODING_DEFAULT);
	m_pbtnReboot->SetFont(m_pbtnRebootFont);
	m_pbtnShutdown = new wmButton(this, ID_M_PBTN3, _("Hold to Shutdown"), wxPoint(50,160), wxSize(100,60), wmButton::STYLE_HOLD, wxDefaultValidator, _T("ID_M_PBTN3"));
	m_pbtnShutdown->SetBackgroundColour(wxColour(128,0,0));
	wxFont m_pbtnShutdownFont(12,wxFONTFAMILY_SWISS,wxFONTSTYLE_NORMAL,wxFONTWEIGHT_BOLD,false,_T("Arial"),wxFONTENCODING_DEFAULT);
	m_pbtnShutdown->SetFont(m_pbtnShutdownFont);

	Connect(ID_M_PBTN1,wxEVT_BUTTON_HELD,(wxObjectEventFunction)&pnlSettingsOptions::OnbtnExitHeld);
	Connect(ID_M_PBTN2,wxEVT_BUTTON_HELD,(wxObjectEventFunction)&pnlSettingsOptions::OnbtnRebootHeld);
	Connect(ID_M_PBTN3,wxEVT_BUTTON_HELD,(wxObjectEventFunction)&pnlSettingsOptions::OnbtnShutdownHeld);
	//*)
	SetBackgroundColour(*wxBLACK);
	#ifdef __WXMSW__
	m_pbtnShutdown->Hide();
	m_pbtnReboot->Hide();
	#endif // __WXMSW__
}

pnlSettingsOptions::~pnlSettingsOptions()
{
	//(*Destroy(pnlSettingsOptions)
	//*)
}


void pnlSettingsOptions::OnbtnExitClick(wxCommandEvent& event)
{

}

void pnlSettingsOptions::OnbtnExitHeld(wxCommandEvent& event)
{
    wxTheApp->GetTopWindow()->Close();
}

void pnlSettingsOptions::OnbtnRebootHeld(wxCommandEvent& event)
{
    wxShutdown(wxSHUTDOWN_REBOOT);
}

void pnlSettingsOptions::OnbtnShutdownHeld(wxCommandEvent& event)
{
    wxShutdown();
}
