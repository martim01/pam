#include "pnlRouting.h"
#include "scopebuilder.h"

//(*InternalHeaders(pnlRouting)
#include <wx/intl.h>
#include <wx/string.h>
//*)

//(*IdInit(pnlRouting)
const long pnlRouting::ID_M_PLST16 = wxNewId();
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
	m_plstScope_Routing = new wmList(this, ID_M_PLST16, wxPoint(0,0), wxSize(190,210), wmList::STYLE_SELECT, 0, wxSize(-1,-1), 2, wxSize(5,5));
	m_plstScope_Routing->SetBackgroundColour(wxColour(0,0,0));

	Connect(ID_M_PLST16,wxEVT_LIST_SELECTED,(wxObjectEventFunction)&pnlRouting::OnlstScope_RoutingSelected);
	//*)


}

pnlRouting::~pnlRouting()
{
	//(*Destroy(pnlRouting)
	//*)
}


void pnlRouting::OnlstScope_RoutingSelected(wxCommandEvent& event)
{
    m_pBuilder->WriteSetting(wxT("Routing"), (int)event.GetClientData());
}



void pnlRouting::SetNumberOfChannels(unsigned int nChannels)
{
    m_plstScope_Routing->Freeze();
    m_plstScope_Routing->Clear();
    if(nChannels == 2)
    {
        m_plstScope_Routing->AddButton(wxT("Left"), wxNullBitmap,(void*)0);
        m_plstScope_Routing->AddButton(wxT("Right"), wxNullBitmap,(void*)1);
        m_plstScope_Routing->AddButton(wxT("Mono"), wxNullBitmap,(void*)8);
        m_plstScope_Routing->AddButton(wxT("Stereo"), wxNullBitmap,(void*)9);
    }
    else
    {
        for(int i = 0; i < nChannels; i++)
        {
            m_plstScope_Routing->AddButton(wxString::Format(wxT("Channel %d"), i+1), wxNullBitmap, (void*)i);
        }
    }

    size_t nButton = m_plstScope_Routing->FindButton((void*)m_pBuilder->ReadSetting(wxT("Routing"),0));
    if(nButton == -1)
    {
        nButton = 0;
    }
    m_plstScope_Routing->SelectButton(nButton, true);
    m_plstScope_Routing->Thaw();
}
