#include "pnlGraph.h"
#include "aoipinfobuilder.h"

//(*InternalHeaders(pnlGraph)
#include <wx/intl.h>
#include <wx/string.h>
//*)

//(*IdInit(pnlGraph)
const long pnlGraph::ID_M_PLST16 = wxNewId();
//*)

BEGIN_EVENT_TABLE(pnlGraph,wxPanel)
	//(*EventTable(pnlGraph)
	//*)
END_EVENT_TABLE()

pnlGraph::pnlGraph(wxWindow* parent,AoIPInfoBuilder* pBuilder, wxWindowID id,const wxPoint& pos,const wxSize& size) :
    m_pBuilder(pBuilder)
{
	//(*Initialize(pnlGraph)
	Create(parent, id, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL, _T("id"));
	m_plstGraph = new wmList(this, ID_M_PLST16, wxPoint(0,0), wxSize(190,210), wmList::STYLE_SELECT, 0, wxSize(-1,-1), 2, wxSize(5,5));
	m_plstGraph->SetBackgroundColour(wxColour(0,0,0));

	Connect(ID_M_PLST16,wxEVT_LIST_SELECTED,(wxObjectEventFunction)&pnlGraph::OnlstGraphSelected);
	//*)

	m_plstGraph->AddButton(wxT("kBit/s"));
	m_plstGraph->AddButton(wxT("Packet Gap"));
	m_plstGraph->AddButton(wxT("Packet Loss"));
	m_plstGraph->AddButton(wxT("Jitter"));


	m_plstGraph->SelectButton(m_pBuilder->ReadSetting(wxT("Graph"), wxT("kBit/s")), true);
}

pnlGraph::~pnlGraph()
{
	//(*Destroy(pnlGraph)
	//*)
}


void pnlGraph::OnlstGraphSelected(wxCommandEvent& event)
{
    m_pBuilder->WriteSetting(wxT("Graph"), event.GetString());
}
