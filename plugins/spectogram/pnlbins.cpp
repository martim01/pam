#include "pnlbins.h"
#include "spectogrambuilder.h"

//(*InternalHeaders(pnlBins)
#include <wx/intl.h>
#include <wx/string.h>
//*)

//(*IdInit(pnlBins)
const long pnlBins::ID_M_PLST11 = wxNewId();
//*)

BEGIN_EVENT_TABLE(pnlBins,wxPanel)
	//(*EventTable(pnlBins)
	//*)
END_EVENT_TABLE()

pnlBins::pnlBins(wxWindow* parent,SpectogramBuilder* pBuilder, wxWindowID id,const wxPoint& pos,const wxSize& size) :
    m_pBuilder(pBuilder)
{
	Create(parent, id, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL, _T("id"));
	SetBackgroundColour(wxColour(0,0,0));
	m_plstFFT_Bins = new wmList(this, ID_M_PLST11, wxPoint(0,0), wxSize(190,140), wmList::STYLE_SELECT, 0, wxSize(-1,-1), 2, wxSize(5,5));
	m_plstFFT_Bins->SetBackgroundColour(wxColour(0,0,0));

    m_plstFFT_Bins->AddButton(wxT("46 Hz"), wxNullBitmap, reinterpret_cast<void*>(512));
    m_plstFFT_Bins->AddButton(wxT("23 Hz"), wxNullBitmap, reinterpret_cast<void*>(1024));
    m_plstFFT_Bins->AddButton(wxT("16 Hz"), wxNullBitmap, reinterpret_cast<void*>(1536));
    m_plstFFT_Bins->AddButton(wxT("12 Hz"), wxNullBitmap, reinterpret_cast<void*>(2048));

    m_plstFFT_Bins->ConnectToSetting(m_pBuilder->GetName(), "Bins", reinterpret_cast<void*>(1024));


}

pnlBins::~pnlBins()
{
	//(*Destroy(pnlBins)
	//*)
}


