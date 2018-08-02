#include "pnlRouting.h"
#include "scopebuilder.h"

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
	//(*Initialize(pnlRouting)
	Create(parent, id, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL, _T("id"));
	SetBackgroundColour(wxColour(0,0,0));
	m_plstRouting1 = new wmList(this, ID_M_PLST16, wxPoint(0,0), wxSize(190,88), wmList::STYLE_SELECT, 0, wxSize(-1,-1), 4, wxSize(0,0));
	m_plstRouting1->SetBackgroundColour(wxColour(0,0,0));
	m_plstRouting2 = new wmList(this, ID_M_PLST1, wxPoint(0,100), wxSize(190,88), wmList::STYLE_SELECT, 0, wxSize(-1,-1), 4, wxSize(0,0));
	m_plstRouting2->SetBackgroundColour(wxColour(0,0,0));

	Connect(ID_M_PLST16,wxEVT_LIST_SELECTED,(wxObjectEventFunction)&pnlRouting::OnlstScope_RoutingSelected);
	Connect(ID_M_PLST1,wxEVT_LIST_SELECTED,(wxObjectEventFunction)&pnlRouting::OnlstRouting2Selected);
	//*)


}

pnlRouting::~pnlRouting()
{
	//(*Destroy(pnlRouting)
	//*)
}


void pnlRouting::OnlstScope_RoutingSelected(wxCommandEvent& event)
{
    m_pBuilder->WriteSetting(wxT("Routing1"), (int)event.GetClientData());
}



void pnlRouting::SetNumberOfChannels(unsigned int nChannels)
{
    ShowRouting(m_plstRouting1,1, nChannels);
    ShowRouting(m_plstRouting2,2, nChannels);
}

void pnlRouting::ShowRouting(wmList* pLst, unsigned int nPlot, unsigned int nChannels)
{
    pLst->Freeze();
    pLst->Clear();
    if(nChannels == 2)
    {
        pLst->AddButton(wxT("Left"), wxNullBitmap,(void*)0);
        pLst->AddButton(wxT("Right"), wxNullBitmap,(void*)1);
        pLst->AddButton(wxT("Middle"), wxNullBitmap,(void*)8);
        pLst->AddButton(wxT("Side"), wxNullBitmap,(void*)9);
    }
    else
    {
        for(int i = 0; i < nChannels; i++)
        {
            pLst->AddButton(wxString::Format(wxT("Channel %d"), i+1), wxNullBitmap, (void*)i);
        }
    }

    size_t nButton = pLst->FindButton((void*)m_pBuilder->ReadSetting(wxString::Format(wxT("Routing%d"), nPlot),0));
    if(nButton == -1)
    {
        nButton = 0;
    }
    pLst->SelectButton(nButton, true);
    pLst->Thaw();
}

void pnlRouting::OnlstRouting2Selected(wxCommandEvent& event)
{
    m_pBuilder->WriteSetting(wxT("Routing2"), (int)event.GetClientData());
}
