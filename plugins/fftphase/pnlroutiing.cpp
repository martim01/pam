#include "pnlroutiing.h"

//(*InternalHeaders(pnlRoutiing)
#include <wx/intl.h>
#include <wx/string.h>
//*)
#include "fftphasebuilder.h"
#include "fftAlgorithm.h"

//(*IdInit(pnlRoutiing)
const long pnlRoutiing::ID_M_PLST3 = wxNewId();
//*)

BEGIN_EVENT_TABLE(pnlRoutiing,wxPanel)
	//(*EventTable(pnlRoutiing)
	//*)
END_EVENT_TABLE()

pnlRoutiing::pnlRoutiing(wxWindow* parent,fftphaseBuilder* pBuilder, wxWindowID id,const wxPoint& pos,const wxSize& size) :
    m_pBuilder(pBuilder)
{
	//(*Initialize(pnlRoutiing)
	Create(parent, id, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL, _T("id"));
	SetBackgroundColour(wxColour(0,0,0));
	m_plstFFT_Routing = new wmList(this, ID_M_PLST3, wxPoint(0,0), wxSize(190,200), wmList::STYLE_SELECT, 0, wxSize(-1,-1), 2, wxSize(5,5));
	m_plstFFT_Routing->SetBackgroundColour(wxColour(0,0,0));

	Connect(ID_M_PLST3,wxEVT_LIST_SELECTED,(wxObjectEventFunction)&pnlRoutiing::OnlstFFT_RoutingSelected);
	//*)




}

pnlRoutiing::~pnlRoutiing()
{
	//(*Destroy(pnlRoutiing)
	//*)
}

void pnlRoutiing::SetNumberOfChannels(unsigned int nChannels)
{
    m_plstFFT_Routing->Freeze();
    m_plstFFT_Routing->Clear();

    if(nChannels == 2)
    {
        m_plstFFT_Routing->AddButton(wxT("Left"), wxNullBitmap, (void*)FFTAlgorithm::ANALYSE_L);
        m_plstFFT_Routing->AddButton(wxT("Right"), wxNullBitmap, (void*)FFTAlgorithm::ANALYSE_R);
        m_plstFFT_Routing->AddButton(wxT("Middle"), wxNullBitmap, (void*)FFTAlgorithm::ANALYSE_L_P_R);
        m_plstFFT_Routing->AddButton(wxT("Side"), wxNullBitmap, (void*)FFTAlgorithm::ANALYSE_L_M_R);
    }
    else
    {
        for(unsigned int i = 0; i < nChannels; i++)
        {
            m_plstFFT_Routing->AddButton(wxString::Format(wxT("Channel %d"), i+1), wxNullBitmap, (void*)i);
        }
    }

    m_plstFFT_Routing->SelectButton(m_pBuilder->ReadSetting(wxT("Routing"),0), true);
    m_plstFFT_Routing->Thaw();
}

void pnlRoutiing::OnlstFFT_RoutingSelected(wxCommandEvent& event)
{
    m_pBuilder->OnRoutingChanged(event);
}
