#include <wx/app.h>
#include "pnlSettingsOptions.h"

//(*InternalHeaders(pnlSettingsOptions)
#include <wx/font.h>
#include <wx/intl.h>
#include <wx/string.h>
//*)

//(*IdInit(pnlSettingsOptions)
const long pnlSettingsOptions::ID_M_PBTN1 = wxNewId();
//*)

BEGIN_EVENT_TABLE(pnlSettingsOptions,wxPanel)
	//(*EventTable(pnlSettingsOptions)
	//*)
END_EVENT_TABLE()

pnlSettingsOptions::pnlSettingsOptions(wxWindow* parent,wxWindowID id,const wxPoint& pos,const wxSize& size)
{
	//(*Initialize(pnlSettingsOptions)
	Create(parent, id, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL, _T("id"));
	m_pbtnExit = new wmButton(this, ID_M_PBTN1, _("Hold to Exit"), wxPoint(50,140), wxSize(100,60), wmButton::STYLE_HOLD, wxDefaultValidator, _T("ID_M_PBTN1"));
	m_pbtnExit->SetBackgroundColour(wxColour(128,0,0));
	wxFont m_pbtnExitFont(12,wxFONTFAMILY_SWISS,wxFONTSTYLE_NORMAL,wxFONTWEIGHT_BOLD,false,_T("Arial"),wxFONTENCODING_DEFAULT);
	m_pbtnExit->SetFont(m_pbtnExitFont);

	Connect(ID_M_PBTN1,wxEVT_BUTTON_HELD,(wxObjectEventFunction)&pnlSettingsOptions::OnbtnExitHeld);
	//*)
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
