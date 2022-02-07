#include "pnlGraph.h"
#include "aoipinfobuilder.h"

//(*InternalHeaders(pnlGraph)
#include <wx/intl.h>
#include <wx/string.h>
//*)

//(*IdInit(pnlGraph)
const long pnlGraph::ID_M_PLST16 = wxNewId();
const long pnlGraph::ID_M_PBTN29 = wxNewId();
//*)

BEGIN_EVENT_TABLE(pnlGraph,wxPanel)
	//(*EventTable(pnlGraph)
	//*)
END_EVENT_TABLE()

pnlGraph::pnlGraph(wxWindow* parent,AoIPInfoBuilder* pBuilder, wxWindowID id,const wxPoint& pos,const wxSize& size) :
    m_pBuilder(pBuilder)
{

	Create(parent, id, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL, _T("id"));
	SetBackgroundColour(wxColour(0,0,0));
	m_plstGraph = new wmList(this, ID_M_PLST16, wxPoint(0,0), wxSize(195,150), wmList::STYLE_SELECT, 0, wxSize(-1,32), 2, wxSize(2,2));
	m_plstGraph->SetBackgroundColour(wxColour(0,0,0));

	m_plstType = new wmList(this, wxNewId(), wxPoint(0,152), wxSize(195,38), wmList::STYLE_SELECT, 0, wxSize(-1,32), 3, wxSize(1,0));
	m_plstType->SetBackgroundColour(wxColour(0,0,0));
    m_plstType->SetButtonColour(wxColour(50,100,0));

    m_pbtnRange = new wmButton(this, wxNewId(), _("Recalculate Range"), wxPoint(5,195), wxSize(90,30), wmButton::STYLE_NORMAL, wxDefaultValidator, _T("ID_M_PBTN29"));

	m_pbtnClear = new wmButton(this, ID_M_PBTN29, _("Clear Graphs"), wxPoint(100,195), wxSize(90,30), wmButton::STYLE_NORMAL, wxDefaultValidator, _T("ID_M_PBTN29"));

	Connect(ID_M_PLST16,wxEVT_LIST_SELECTED,(wxObjectEventFunction)&pnlGraph::OnlstGraphSelected);
	Connect(ID_M_PBTN29,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&pnlGraph::OnbtnClearClick);
	Connect(m_pbtnRange->GetId(),wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&pnlGraph::OnbtnRangeClick);

	Connect(m_plstType->GetId(),wxEVT_LIST_SELECTED,(wxObjectEventFunction)&pnlGraph::OnlstTypeSelected);

    m_plstType->AddButton("Line Graph");
    m_plstType->AddButton("Bar Chart");
    m_plstType->AddButton("Histogram");


	m_plstGraph->AddButton(wxT("kBit/s"));
	m_plstGraph->AddButton(wxT("Packet Gap"));
	m_plstGraph->AddButton(wxT("Packet Loss"));
	m_plstGraph->AddButton(wxT("Jitter"));
	m_plstGraph->AddButton(wxT("Timestamp"));
	m_plstGraph->AddButton(wxT("Timestamp Errors"));
	m_plstGraph->AddButton(wxT("TS-DF"));
	m_plstGraph->AddButton(wxT("Slip"));


	m_plstGraph->ConnectToSetting(m_pBuilder->GetName(), "Graph", "kBit/s");
	m_plstType->ConnectToSetting(m_pBuilder->GetName(), "Type", "Line Graph");

}

pnlGraph::~pnlGraph()
{
	//(*Destroy(pnlGraph)
	//*)
}


void pnlGraph::OnlstGraphSelected(wxCommandEvent& event)
{
}

void pnlGraph::OnbtnClearClick(wxCommandEvent& event)
{
    m_pBuilder->ClearGraphs();
}

void pnlGraph::OnbtnRangeClick(wxCommandEvent& event)
{
    m_pBuilder->RecalculateRange();
}

void pnlGraph::OnlstTypeSelected(wxCommandEvent& event)
{
}
