#include "pnlTestOptions.h"
#include "settings.h"

//(*InternalHeaders(pnlTestOptions)
#include <wx/font.h>
#include <wx/intl.h>
#include <wx/string.h>
//*)

//(*IdInit(pnlTestOptions)
const long pnlTestOptions::ID_M_PBTN4 = wxNewId();
//*)

BEGIN_EVENT_TABLE(pnlTestOptions,wxPanel)
	//(*EventTable(pnlTestOptions)
	//*)
END_EVENT_TABLE()

pnlTestOptions::pnlTestOptions(wxWindow* parent,wxWindowID id,const wxPoint& pos,const wxSize& size)
{
	//(*Initialize(pnlTestOptions)
	Create(parent, id, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL, _T("id"));
	SetBackgroundColour(wxColour(0,0,0));
	m_pbtnLog = new wmButton(this, ID_M_PBTN4, _("Log"), wxPoint(10,5), wxSize(180,40), wmButton::STYLE_SELECT, wxDefaultValidator, _T("ID_M_PBTN4"));
	m_pbtnLog->SetBackgroundColour(wxColour(52,105,105));
	wxFont m_pbtnLogFont(12,wxFONTFAMILY_SWISS,wxFONTSTYLE_NORMAL,wxFONTWEIGHT_BOLD,false,_T("Arial"),wxFONTENCODING_DEFAULT);
	m_pbtnLog->SetFont(m_pbtnLogFont);

	Connect(ID_M_PBTN4,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&pnlTestOptions::OnbtnLogClick);
	//*)
	m_pbtnLog->SetToggleLook(true, wxT("Off"), wxT("On"),50);

	m_pbtnLog->ToggleSelection((Settings::Get().Read(wxT("Tests"), wxT("Log"), 0)==1), false);
}

pnlTestOptions::~pnlTestOptions()
{
	//(*Destroy(pnlTestOptions)
	//*)
}


void pnlTestOptions::OnbtnLogClick(wxCommandEvent& event)
{
    Settings::Get().Write(wxT("Tests"), wxT("Log"), event.IsChecked());
}
