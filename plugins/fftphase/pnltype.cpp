#include "pnltype.h"

//(*InternalHeaders(pnlType)
#include <wx/intl.h>
#include <wx/string.h>
//*)

#include "fftphasebuilder.h"

//(*IdInit(pnlType)
const long pnlType::ID_M_PLST6 = wxNewId();
//*)

BEGIN_EVENT_TABLE(pnlType,wxPanel)
	//(*EventTable(pnlType)
	//*)
END_EVENT_TABLE()

pnlType::pnlType(wxWindow* parent,fftphaseBuilder* pBuilder, wxWindowID id,const wxPoint& pos,const wxSize& size) :
    m_pBuilder(pBuilder)
{
	//(*Initialize(pnlType)
	Create(parent, id, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL, _T("id"));
	SetBackgroundColour(wxColour(0,0,0));
	m_plstFFT_Type = new wmList(this, ID_M_PLST6, wxPoint(0,0), wxSize(190,140), wmList::STYLE_SELECT, 0, wxSize(-1,-1), 2, wxSize(5,5));
	m_plstFFT_Type->SetBackgroundColour(wxColour(0,0,0));

	Connect(ID_M_PLST6,wxEVT_LIST_SELECTED,(wxObjectEventFunction)&pnlType::OnlstFFT_TypeSelected);
	//*)

    m_plstFFT_Type->AddButton(wxT("FFT"));
    m_plstFFT_Type->AddButton(wxT("1/3 Octave"));
    m_plstFFT_Type->AddButton(wxT("Peaks"));
    m_plstFFT_Type->SelectButton(0,false);


}

pnlType::~pnlType()
{
	//(*Destroy(pnlType)
	//*)
}


void pnlType::OnlstFFT_TypeSelected(wxCommandEvent& event)
{
    m_pBuilder->OnTypeChanged(event);
}
