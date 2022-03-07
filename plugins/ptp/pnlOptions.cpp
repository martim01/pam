#include "pnlOptions.h"
#include "ptpbuilder.h"
//(*InternalHeaders(pnlOptions)
#include <wx/intl.h>
#include <wx/string.h>
//*)

//(*IdInit(pnlOptions)
const long pnlOptions::ID_M_PLST5 = wxNewId();
//*)

BEGIN_EVENT_TABLE(pnlOptions,wxPanel)
	//(*EventTable(pnlOptions)
	//*)
END_EVENT_TABLE()

pnlOptions::pnlOptions(wxWindow* parent, ptpBuilder* pBuilder,wxWindowID id,const wxPoint& pos,const wxSize& size) :
    m_pBuilder(pBuilder)
{
	//(*Initialize(pnlOptions)
	Create(parent, id, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL, _T("id"));
	SetBackgroundColour(wxColour(0,0,0));
	m_plstWindow = new wmList(this, ID_M_PLST5, wxPoint(0,0), wxSize(190,100), wmList::STYLE_SELECT, 0, wxSize(-1,30), 2, wxSize(5,5));
	m_plstWindow->SetBackgroundColour(wxColour(0,0,0));

	Connect(ID_M_PLST5,wxEVT_LIST_SELECTED,(wxObjectEventFunction)&pnlOptions::OnlstWindowSelected);
	//*)



	m_plstWindow->AddButton("Info");
	m_plstWindow->AddButton("Graphs");
	m_plstWindow->AddButton("Histograms");

	m_plstWindow->ConnectToSetting(m_pBuilder->GetSection(), "Window", "Info");


}

pnlOptions::~pnlOptions()
{
	//(*Destroy(pnlOptions)
	//*)
}


void pnlOptions::OnlstWindowSelected(wxCommandEvent& event)
{

}

