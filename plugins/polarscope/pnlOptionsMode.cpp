#include "pnlOptionsMode.h"
#include "polarscopebuilder.h"

//(*InternalHeaders(pnlOptionsMode)
#include <wx/intl.h>
#include <wx/string.h>
//*)

//(*IdInit(pnlOptionsMode)
const long pnlOptionsMode::ID_M_PLST19 = wxNewId();
//*)

BEGIN_EVENT_TABLE(pnlOptionsMode,wxPanel)
	//(*EventTable(pnlOptionsMode)
	//*)
END_EVENT_TABLE()

pnlOptionsMode::pnlOptionsMode(wxWindow* parent,PolarScopeBuilder* pBuilder, wxWindowID id,const wxPoint& pos,const wxSize& size) :
    m_pBuilder(pBuilder)
{
	//(*Initialize(pnlOptionsMode)
	Create(parent, id, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL, _T("id"));
	SetBackgroundColour(wxColour(0,0,0));
	m_plstMode = new wmList(this, ID_M_PLST19, wxPoint(0,0), wxSize(190,90), wmList::STYLE_SELECT, 0, wxSize(-1,-1), 3, wxSize(2,2));
	m_plstMode->SetBackgroundColour(wxColour(0,0,0));

	Connect(ID_M_PLST19,wxEVT_LIST_SELECTED,(wxObjectEventFunction)&pnlOptionsMode::OnlstModeSelected);
	//*)
	m_plstMode->AddButton(wxT("Points"));
	m_plstMode->AddButton(wxT("Hull"));
	m_plstMode->AddButton(wxT("Levels"));

	m_plstMode->SelectButton(m_pBuilder->ReadSetting(wxT("Mode"), 0), true);
}

pnlOptionsMode::~pnlOptionsMode()
{
	//(*Destroy(pnlOptionsMode)
	//*)
}


void pnlOptionsMode::OnlstModeSelected(wxCommandEvent& event)
{
    m_pBuilder->WriteSetting(wxT("Mode"), event.GetInt());
}
