#include "pnlScale.h"
#include "r128builder.h"
//(*InternalHeaders(pnlScale)
#include <wx/intl.h>
#include <wx/string.h>
//*)

//(*IdInit(pnlScale)
const long pnlScale::ID_M_PBTN1 = wxNewId();
const long pnlScale::ID_M_PBTN2 = wxNewId();
//*)

BEGIN_EVENT_TABLE(pnlScale,wxPanel)
	//(*EventTable(pnlScale)
	//*)
END_EVENT_TABLE()

pnlScale::pnlScale(wxWindow* parent,R128Builder* pBuilder, wxWindowID id,const wxPoint& pos,const wxSize& size) :
     m_pBuilder(pBuilder)
{
	//(*Initialize(pnlScale)
	Create(parent, id, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL, _T("id"));
	SetBackgroundColour(wxColour(0,0,0));
	m_pbtnScale = new wmButton(this, ID_M_PBTN1, _("EBU Scale"), wxPoint(0,0), wxSize(200,40), wmButton::STYLE_SELECT, wxDefaultValidator, _T("ID_M_PBTN1"));
	m_pbtnScale->SetBackgroundColour(wxColour(0,0,160));
	m_pbtnZero = new wmButton(this, ID_M_PBTN2, _("Zero Point"), wxPoint(0,50), wxSize(200,40), wmButton::STYLE_SELECT, wxDefaultValidator, _T("ID_M_PBTN2"));
	m_pbtnZero->SetBackgroundColour(wxColour(0,0,160));

	Connect(ID_M_PBTN1,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&pnlScale::OnbtnScaleClick);
	Connect(ID_M_PBTN2,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&pnlScale::OnbtnZeroClick);
	//*)

	m_pbtnScale->SetToggleLook(true, wxT("+9"), wxT("+18"), 50.0);
	m_pbtnScale->ToggleSelection(m_pBuilder->ReadSetting(wxT("Scale"),1) == 1, true);

	m_pbtnZero->SetToggleLook(true, wxT("-23"), wxT("Abs"), 50.0);
	m_pbtnZero->ToggleSelection(m_pBuilder->ReadSetting(wxT("Zero"),1) == 1, true);
}

pnlScale::~pnlScale()
{
	//(*Destroy(pnlScale)
	//*)
}


void pnlScale::OnbtnScaleClick(wxCommandEvent& event)
{
    m_pBuilder->WriteSetting(wxT("Scale"), event.IsChecked());
}

void pnlScale::OnbtnZeroClick(wxCommandEvent& event)
{
    m_pBuilder->WriteSetting(wxT("Zero"), event.IsChecked());
}
