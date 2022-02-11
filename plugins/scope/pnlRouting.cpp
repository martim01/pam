#include "pnlRouting.h"
#include "scopebuilder.h"
#include <wx/tokenzr.h>
//(*InternalHeaders(pnlRouting)
#include <wx/intl.h>
#include <wx/string.h>
//*)

//(*IdInit(pnlRouting)
const long pnlRouting::ID_M_PLST16 = wxNewId();
const long pnlRouting::ID_M_PLST1 = wxNewId();
//*)

BEGIN_EVENT_TABLE(pnlRouting,wxPanel)
	//(*EventTable(pnlRouting)
	//*)
END_EVENT_TABLE()

pnlRouting::pnlRouting(wxWindow* parent,ScopeBuilder* pBuilder, wxWindowID id,const wxPoint& pos,const wxSize& size) :
    m_pBuilder(pBuilder)
{
	Create(parent, id, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL, _T("id"));
	SetBackgroundColour(wxColour(0,0,0));

	m_plblRouting = new wmLabel(this, wxNewId(), "Display", wxPoint(0,0), wxSize(190,30));
	m_plblRouting->SetBackgroundColour(wxColour(80,100,128));
	m_plblRouting->SetForegroundColour(*wxWHITE);
	m_plstRouting = new wmList(this, ID_M_PLST16, wxPoint(0,35), wxSize(190,72), wmList::STYLE_SELECT | wmList::STYLE_SELECT_MULTI, 0, wxSize(-1,35), 4, wxSize(0,0));
	m_plstRouting->SetBackgroundColour(wxColour(0,0,0));

	m_plblTrigger = new wmLabel(this, wxNewId(), "Trigger On", wxPoint(0,120), wxSize(190,30));
	m_plblTrigger->SetBackgroundColour(wxColour(80,100,128));
	m_plblTrigger->SetForegroundColour(*wxWHITE);
	m_plstTrigger = new wmList(this, wxNewId(), wxPoint(0,165), wxSize(190,72), wmList::STYLE_SELECT , 0, wxSize(-1,35), 4, wxSize(0,0));
	m_plstTrigger->SetBackgroundColour(wxColour(0,0,0));

	Connect(ID_M_PLST16,wxEVT_LIST_SELECTED,(wxObjectEventFunction)&pnlRouting::OnlstScope_RoutingSelected);
	Connect(m_plstTrigger->GetId(),wxEVT_LIST_SELECTED,(wxObjectEventFunction)&pnlRouting::OnlstTriggerSelected);



}

pnlRouting::~pnlRouting()
{
	//(*Destroy(pnlRouting)
	//*)
}


void pnlRouting::OnlstScope_RoutingSelected(wxCommandEvent& event)
{
    wxString sPlot;
    for(size_t i = 0; i < 8; i++)
    {
        if(i < m_plstRouting->GetItemCount() && m_plstRouting->IsSelected(i))
        {
            if(sPlot.empty() == false)
            {
                sPlot += ",";
            }
            sPlot << i;
        }
    }
    m_pBuilder->WriteSetting("Plot", sPlot);
}



void pnlRouting::SetNumberOfChannels(unsigned int nChannels)
{
    ShowRouting(m_plstRouting,1, nChannels);

    m_plstTrigger->Freeze();
    m_plstTrigger->Clear();
    for(int i = 0; i < nChannels; i++)
    {
        m_plstTrigger->AddButton(wxString::Format("Ch %d", i+1));
    }
    m_plstTrigger->SelectButton(m_pBuilder->ReadSetting("TriggerOn", 0));
    m_plstTrigger->Thaw();
}

void pnlRouting::ShowRouting(wmList* pLst, unsigned int nPlot, unsigned int nChannels)
{
    pLst->Freeze();
    pLst->Clear();
    if(nChannels == 2)
    {
        pLst->AddButton(wxT("Left"));
        pLst->AddButton(wxT("Right"));
    }
    else
    {
        wxArrayString asPlot = wxStringTokenize(m_pBuilder->ReadSetting("Plot", "0,1"), ",");
        for(int i = 0; i < nChannels; i++)
        {
            pLst->AddButton(wxString::Format("Ch %d", i+1));
            if(asPlot.Index(wxString::Format("%d", i)) != wxNOT_FOUND)
            {
                pLst->SelectButton(i);
            }
        }
    }

    pLst->Thaw();
}

void pnlRouting::OnlstTriggerSelected(wxCommandEvent& event)
{
    m_pBuilder->WriteSetting("TriggerOn", event.GetInt());
}
