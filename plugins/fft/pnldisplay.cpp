#include "pnldisplay.h"

//(*InternalHeaders(pnlDisplay)
#include <wx/intl.h>
#include <wx/string.h>
//*)

#include "fftbuilder.h"

//(*IdInit(pnlDisplay)
const long pnlDisplay::ID_M_PLST4 = wxNewId();
//*)

BEGIN_EVENT_TABLE(pnlDisplay,wxPanel)
	//(*EventTable(pnlDisplay)
	//*)
END_EVENT_TABLE()

pnlDisplay::pnlDisplay(wxWindow* parent,FFTBuilder* pBuilder, wxWindowID id,const wxPoint& pos,const wxSize& size) :
    m_pBuilder(pBuilder)
{
	//(*Initialize(pnlDisplay)
	Create(parent, id, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL, _T("id"));
	SetBackgroundColour(wxColour(0,0,0));
	m_plstFFT_Display = new wmList(this, ID_M_PLST4, wxPoint(0,0), wxSize(190,140), wmList::STYLE_SELECT, 0, wxSize(-1,-1), 2, wxSize(5,5));
	m_plstFFT_Display->SetBackgroundColour(wxColour(0,0,0));

	Connect(ID_M_PLST4,wxEVT_LIST_SELECTED,(wxObjectEventFunction)&pnlDisplay::OnlstFFT_DisplaySelected);
	//*)
	m_pbtnColour = new wmButton(this,wxNewId(), wxT("Colour"), wxPoint(0,150), wxSize(200, 40),wmButton::STYLE_SELECT);
	m_pbtnColour->SetToggleLook(true, wxT("Off"), wxT("On"), 45);
    m_pbtnColour->ToggleSelection(pBuilder->ReadSetting(wxT("Colour"), 0), true);

    Connect(m_pbtnColour->GetId(), wxEVT_COMMAND_BUTTON_CLICKED, (wxObjectEventFunction)&pnlDisplay::OnbtnColour);

    m_plstFFT_Display->AddButton(wxT("Graph"));
    m_plstFFT_Display->AddButton(wxT("Lines"));
    m_plstFFT_Display->AddButton(wxT("EQ"));
    m_plstFFT_Display->SelectButton(0,false);

}

pnlDisplay::~pnlDisplay()
{
	//(*Destroy(pnlDisplay)
	//*)
}


void pnlDisplay::OnlstFFT_DisplaySelected(wxCommandEvent& event)
{
    m_pBuilder->OnDisplayChanged(event);
}

void pnlDisplay::OnbtnColour(wxCommandEvent& event)
{
    m_pBuilder->ColourSelected(event.IsChecked());
}
