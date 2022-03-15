#include "pnlroutiing.h"

//(*InternalHeaders(pnlRoutiing)
#include <wx/intl.h>
#include <wx/string.h>
//*)
#include "fftbuilder.h"
#include "fftAlgorithm.h"

//(*IdInit(pnlRoutiing)
const long pnlRoutiing::ID_M_PLST3 = wxNewId();
//*)

BEGIN_EVENT_TABLE(pnlRoutiing,wxPanel)
	//(*EventTable(pnlRoutiing)
	//*)
END_EVENT_TABLE()

pnlRoutiing::pnlRoutiing(wxWindow* parent,FFTBuilder* pBuilder, wxWindowID id,const wxPoint& pos,const wxSize& size) :
    m_pBuilder(pBuilder)
{
	Create(parent, id, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL, _T("id"));
	SetBackgroundColour(wxColour(0,0,0));
	m_plstFFT_Routing = new wmList(this, ID_M_PLST3, wxPoint(0,0), wxSize(190,200), wmList::STYLE_SELECT, 0, wxSize(-1,-1), 2, wxSize(2,2));
	m_plstFFT_Routing->SetBackgroundColour(wxColour(0,0,0));

}

pnlRoutiing::~pnlRoutiing()
{
	//(*Destroy(pnlRoutiing)
	//*)
}

void pnlRoutiing::SetChannels(const std::vector<std::pair<unsigned char, wxString>>& vChannels)
{
    m_plstFFT_Routing->Freeze();
    m_plstFFT_Routing->Clear();

    if(vChannels.size() == 2)
    {
        m_plstFFT_Routing->AddButton(wxT("Left"), wxNullBitmap, (void*)FFTAlgorithm::ANALYSE_L);
        m_plstFFT_Routing->AddButton(wxT("Right"), wxNullBitmap, (void*)FFTAlgorithm::ANALYSE_R);
        m_plstFFT_Routing->AddButton(wxT("Middle"), wxNullBitmap, (void*)FFTAlgorithm::ANALYSE_L_P_R);
        m_plstFFT_Routing->AddButton(wxT("Side"), wxNullBitmap, (void*)FFTAlgorithm::ANALYSE_L_M_R);
    }
    else
    {
        for(unsigned int i = 0; i < vChannels.size(); i++)
        {
            m_plstFFT_Routing->AddButton(vChannels[i].second+CH_GROUPING[vChannels[i].first], wxNullBitmap, (void*)i);
        }
    }

    m_plstFFT_Routing->ConnectToSetting(m_pBuilder->GetSection(), "Routing", size_t(0));

    m_plstFFT_Routing->Thaw();
}

