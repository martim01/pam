#include "pnlControl.h"
#include "r128builder.h"

//(*InternalHeaders(pnlControl)
#include <wx/intl.h>
#include <wx/string.h>
//*)

//(*IdInit(pnlControl)
const long pnlControl::ID_M_PBTN1 = wxNewId();
const long pnlControl::ID_M_PBTN2 = wxNewId();
//*)

BEGIN_EVENT_TABLE(pnlControl,wxPanel)
	//(*EventTable(pnlControl)
	//*)
END_EVENT_TABLE()

pnlControl::pnlControl(wxWindow* parent,R128Builder* pBuilder, wxWindowID id,const wxPoint& pos,const wxSize& size) :
    m_pBuilder(pBuilder)
{

	//(*Initialize(pnlControl)
	Create(parent, id, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL, _T("id"));
	SetBackgroundColour(wxColour(0,0,0));
	m_pbtnCalculate = new wmButton(this, ID_M_PBTN1, _("Calculate"), wxPoint(0,0), wxSize(200,40), wmButton::STYLE_SELECT, wxDefaultValidator, _T("ID_M_PBTN1"));
	m_pbtnReset = new wmButton(this, ID_M_PBTN2, _("Reset"), wxPoint(0,50), wxSize(100,40), 0, wxDefaultValidator, _T("ID_M_PBTN2"));

	Connect(ID_M_PBTN1,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&pnlControl::OnbtnCalculateClick);
	Connect(ID_M_PBTN2,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&pnlControl::OnbtnResetClick);
	//*)

	m_pbtnCalculate->SetToggle(true, wxT("Pause"), wxT("Run"), 50.0);
	m_pbtnCalculate->ToggleSelection(m_pBuilder->ReadSetting(wxT("Calculate"),1) == 1, true);
}

pnlControl::~pnlControl()
{
	//(*Destroy(pnlControl)
	//*)
}


void pnlControl::OnbtnCalculateClick(wxCommandEvent& event)
{
    m_pBuilder->WriteSetting(wxT("Calculate"), event.IsChecked());
}

void pnlControl::OnbtnResetClick(wxCommandEvent& event)
{
    m_pBuilder->ClearMeter();
}
