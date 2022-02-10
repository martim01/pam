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
	m_plstRouting = new wmList(this, ID_M_PLST16, wxPoint(0,0), wxSize(190,88), wmList::STYLE_SELECT | wmList::STYLE_SELECT_MULTI, 0, wxSize(-1,-1), 4, wxSize(0,0));
	m_plstRouting->SetBackgroundColour(wxColour(0,0,0));

	Connect(ID_M_PLST16,wxEVT_LIST_SELECTED,(wxObjectEventFunction)&pnlRouting::OnlstScope_RoutingSelected);


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

