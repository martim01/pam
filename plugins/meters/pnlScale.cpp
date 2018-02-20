#include "pnlScale.h"
#include "metersbuilder.h"

//(*InternalHeaders(pnlScale)
#include <wx/intl.h>
#include <wx/string.h>
//*)

//(*IdInit(pnlScale)
const long pnlScale::ID_M_PLST1 = wxNewId();
//*)

BEGIN_EVENT_TABLE(pnlScale,wxPanel)
	//(*EventTable(pnlScale)
	//*)
END_EVENT_TABLE()

pnlScale::pnlScale(wxWindow* parent,MetersBuilder* pBuilder,wxWindowID id,const wxPoint& pos,const wxSize& size) :
    m_pBuilder(pBuilder)
{
	//(*Initialize(pnlScale)
	Create(parent, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL, _T("wxID_ANY"));
	SetBackgroundColour(wxColour(0,0,0));
	m_plstScale = new wmList(this, ID_M_PLST1, wxPoint(0,0), wxSize(190,140), wmList::STYLE_SELECT, 0, wxSize(-1,-1), 2, wxSize(5,5));
	m_plstScale->SetBackgroundColour(wxColour(0,0,0));
	m_plstScale->SetTextButtonColour(wxColour(wxT("#FFFFFF")));

	Connect(ID_M_PLST1,wxEVT_LIST_SELECTED,(wxObjectEventFunction)&pnlScale::OnlstScaleSelected);
	//*)

	m_plstScale->AddButton(wxT("dBFS"));
	m_plstScale->AddButton(wxT("dBu (-18dBFS)"));
	m_plstScale->AddButton(wxT("dBu (-13dBFS)"));

	m_plstScale->SelectButton(m_pBuilder->ReadSetting(wxT("Scale"), wxT("dBFS")), true);
}

pnlScale::~pnlScale()
{
	//(*Destroy(pnlScale)
	//*)
}


void pnlScale::OnlstScaleSelected(wxCommandEvent& event)
{
    m_pBuilder->WriteSetting(wxT("Scale"), event.GetString());
}
