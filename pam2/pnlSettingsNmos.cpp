#ifdef __NMOS__
#include "pnlSettingsNmos.h"
#include "settings.h"
#include "settingevent.h"

//(*InternalHeaders(pnlSettingsNmos)
#include <wx/intl.h>
#include <wx/string.h>
//*)

//(*IdInit(pnlSettingsNmos)
const long pnlSettingsNmos::ID_M_PBTN22 = wxNewId();
const long pnlSettingsNmos::ID_M_PBTN1 = wxNewId();
//*)

BEGIN_EVENT_TABLE(pnlSettingsNmos,wxPanel)
	//(*EventTable(pnlSettingsNmos)
	//*)
END_EVENT_TABLE()

pnlSettingsNmos::pnlSettingsNmos(wxWindow* parent,wxWindowID id,const wxPoint& pos,const wxSize& size, long nStyle, const wxString& sId)
{
	//(*Initialize(pnlSettingsNmos)
	Create(parent, id, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL, _T("id"));
	SetBackgroundColour(wxColour(0,0,0));
	m_pbtnNmos = new wmButton(this, ID_M_PBTN22, _("NMOS"), wxPoint(10,10), wxSize(200,40), wmButton::STYLE_SELECT, wxDefaultValidator, _T("ID_M_PBTN22"));
	m_pbtnNmos->SetForegroundColour(wxColour(255,255,255));
	m_pbtnNmos->SetBackgroundColour(wxColour(0,128,0));
	m_pbtnNmos->SetToggleLook(true, wxT("OFF"), wxT("ON"), 50);
	m_pbtnClient = new wmButton(this, ID_M_PBTN1, _("Client"), wxPoint(10,60), wxSize(200,40), wmButton::STYLE_SELECT, wxDefaultValidator, _T("ID_M_PBTN1"));
	m_pbtnClient->SetForegroundColour(wxColour(255,255,255));
	m_pbtnClient->SetBackgroundColour(wxColour(0,128,0));
	m_pbtnClient->SetToggleLook(true, wxT("OFF"), wxT("ON"), 50);

	Connect(ID_M_PBTN22,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&pnlSettingsNmos::OnbtnNmosClick);
	Connect(ID_M_PBTN1,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&pnlSettingsNmos::OnbtnClientClick);
	//*)

	m_pbtnNmos->ToggleSelection(Settings::Get().Read(wxT("NMOS"), wxT("Activate"), false), true);
	m_pbtnClient->ToggleSelection(Settings::Get().Read(wxT("NMOS"), wxT("Client"), false));
}

pnlSettingsNmos::~pnlSettingsNmos()
{
	//(*Destroy(pnlSettingsNmos)
	//*)
}


void pnlSettingsNmos::OnbtnNmosClick(wxCommandEvent& event)
{
    Settings::Get().Write(wxT("NMOS"), wxT("Activate"), event.IsChecked());
    m_pbtnClient->Enable(event.IsChecked());

}

void pnlSettingsNmos::OnbtnClientClick(wxCommandEvent& event)
{
    Settings::Get().Write(wxT("NMOS"), wxT("Client"), event.IsChecked());
}

#endif
