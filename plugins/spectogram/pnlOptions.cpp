#include "pnlOptions.h"
#include "settings.h"
#include "settingevent.h"

//(*InternalHeaders(pnlOptions)
#include <wx/intl.h>
#include <wx/string.h>
//*)
#include "spectogrambuilder.h"


BEGIN_EVENT_TABLE(pnlOptions,wxPanel)
	//(*EventTable(pnlOptions)
	//*)
END_EVENT_TABLE()

pnlOptions::pnlOptions(wxWindow* parent,SpectogramBuilder* pBuilder, wxWindowID id,const wxPoint& pos,const wxSize& size) :
    m_pBuilder(pBuilder)
{
	Create(parent, id, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL, _T("id"));
	SetBackgroundColour(wxColour(0,0,0));

	m_pbtnLinear = new wmButton(this, wxNewId(), _("Display"), wxPoint(2,50), wxSize(190,30), wmButton::STYLE_SELECT, wxDefaultValidator, _T("ID_M_PBTN1"));
    m_pbtnLinear->SetToggle(true, "Log", "Linear");

    m_pbtnLinear->ToggleSelection((m_pBuilder->ReadSetting("linear", 0)==1), true);

    Connect(m_pbtnLinear->GetId(),wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&pnlOptions::OnbtnLinearClick);

}

pnlOptions::~pnlOptions()
{
	//(*Destroy(pnlOptions)
	//*)
}


void pnlOptions::OnbtnLinearClick(wxCommandEvent& event)
{
    m_pBuilder->WriteSetting("linear", event.IsChecked());
}
