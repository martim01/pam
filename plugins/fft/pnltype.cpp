#include "pnltype.h"

//(*InternalHeaders(pnlType)
#include <wx/intl.h>
#include <wx/string.h>
//*)

#include "fftbuilder.h"

//(*IdInit(pnlType)
const long pnlType::ID_M_PLST6 = wxNewId();
//*)

BEGIN_EVENT_TABLE(pnlType,wxPanel)
	//(*EventTable(pnlType)
	//*)
END_EVENT_TABLE()

pnlType::pnlType(wxWindow* parent,FFTBuilder* pBuilder, wxWindowID id,const wxPoint& pos,const wxSize& size) :
    m_pBuilder(pBuilder)
{
	Create(parent, id, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL, _T("id"));
	SetBackgroundColour(wxColour(0,0,0));
	m_plstFFT_Type = new wmList(this, ID_M_PLST6, wxPoint(0,0), wxSize(190,140), wmList::STYLE_SELECT, 0, wxSize(-1,-1), 2, wxSize(5,5));
	m_plstFFT_Type->SetBackgroundColour(wxColour(0,0,0));

    m_plstFFT_Type->AddButton(wxT("FFT"));
    m_plstFFT_Type->AddButton(wxT("1/3 Octave"));
    m_plstFFT_Type->AddButton(wxT("Peaks"));
    m_plstFFT_Type->ConnectToSetting(m_pBuilder->GetSection(), "Type",size_t(0));


}

pnlType::~pnlType()
{
	//(*Destroy(pnlType)
	//*)
}


