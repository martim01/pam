#include "pnloverlap.h"

//(*InternalHeaders(pnlOverlap)
#include <wx/intl.h>
#include <wx/string.h>
//*)

#include "fftphasebuilder.h"

//(*IdInit(pnlOverlap)
const long pnlOverlap::ID_M_PLST10 = wxNewId();
//*)

BEGIN_EVENT_TABLE(pnlOverlap,wxPanel)
	//(*EventTable(pnlOverlap)
	//*)
END_EVENT_TABLE()

pnlOverlap::pnlOverlap(wxWindow* parent,fftphaseBuilder* pBuilder, wxWindowID id,const wxPoint& pos,const wxSize& size) :
    m_pBuilder(pBuilder)
{
	//(*Initialize(pnlOverlap)
	Create(parent, id, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL, _T("id"));
	SetBackgroundColour(wxColour(0,0,0));
	m_plstFFT_Overlap = new wmList(this, ID_M_PLST10, wxPoint(0,0), wxSize(190,140), wmList::STYLE_SELECT, 0, wxSize(-1,-1), 2, wxSize(5,5));
	m_plstFFT_Overlap->SetBackgroundColour(wxColour(0,0,0));

	Connect(ID_M_PLST10,wxEVT_LIST_SELECTED,(wxObjectEventFunction)&pnlOverlap::OnlstFFT_OverlapSelected);
	//*)

    m_plstFFT_Overlap->AddButton(wxT("0%"), wxNullBitmap, reinterpret_cast<void*>(0));
    m_plstFFT_Overlap->AddButton(wxT("25%"), wxNullBitmap, reinterpret_cast<void*>(25));
    m_plstFFT_Overlap->AddButton(wxT("50%"), wxNullBitmap, reinterpret_cast<void*>(50));
    m_plstFFT_Overlap->AddButton(wxT("75%"), wxNullBitmap, reinterpret_cast<void*>(75));


}

pnlOverlap::~pnlOverlap()
{
	//(*Destroy(pnlOverlap)
	//*)
}


void pnlOverlap::OnlstFFT_OverlapSelected(wxCommandEvent& event)
{
    m_pBuilder->OnOverlapChanged(event);
}
