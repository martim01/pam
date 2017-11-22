#include "pnlMode.h"
#include "metersbuilder.h"
//(*InternalHeaders(pnlMode)
#include <wx/intl.h>
#include <wx/string.h>
//*)

//(*IdInit(pnlMode)
const long pnlMode::ID_M_PLST14 = wxNewId();
//*)

BEGIN_EVENT_TABLE(pnlMode,wxPanel)
	//(*EventTable(pnlMode)
	//*)
END_EVENT_TABLE()


pnlMode::pnlMode(wxWindow* parent, MetersBuilder* pBuilder, wxWindowID id,const wxPoint& pos,const wxSize& size) :
    m_pBuilder(pBuilder)
{
	//(*Initialize(pnlMode)
	Create(parent, id, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL, _T("id"));
	SetBackgroundColour(wxColour(0,0,0));
	m_plstMeters_Display = new wmList(this, ID_M_PLST14, wxPoint(0,0), wxSize(190,140), wmList::STYLE_SELECT, 0, wxSize(-1,-1), 2, wxSize(5,5));
	m_plstMeters_Display->SetBackgroundColour(wxColour(0,0,0));
	m_plstMeters_Display->SetTextButtonColour(wxColour(wxT("#FFFFFF")));

	Connect(ID_M_PLST14,wxEVT_LIST_SELECTED,(wxObjectEventFunction)&pnlMode::OnlstMeters_DisplaySelected);
	//*)

	m_plstMeters_Display->AddButton(wxT("PPM"));
	m_plstMeters_Display->AddButton(wxT("Peak"));
	m_plstMeters_Display->AddButton(wxT("Energy"));
	m_plstMeters_Display->AddButton(wxT("Loudness"));

	m_plstMeters_Display->SelectButton(m_pBuilder->ReadSetting(wxT("Mode"),1), true);
}

pnlMode::~pnlMode()
{
	//(*Destroy(pnlMode)
	//*)
}


void pnlMode::OnlstMeters_DisplaySelected(wxCommandEvent& event)
{
    m_pBuilder->WriteSetting(wxT("Mode"), event.GetInt());
}
