#include "correlationpanel.h"
#include "correlationbuilder.h"
#include "timedbuffer.h"
#include "session.h"
#include "audioalgorithms.h"
#include "log.h"

//(*InternalHeaders(correlationPanel)
#include <wx/intl.h>
#include <wx/string.h>
//*)

//(*IdInit(correlationPanel)
//*)

const wxString correlationPanel::GRAPH = "correlation";

wxIMPLEMENT_DYNAMIC_CLASS(correlationPanel, pmPanel)

BEGIN_EVENT_TABLE(correlationPanel,pmPanel)
	//(*EventTable(correlationPanel)
	//*)
END_EVENT_TABLE()


correlationPanel::correlationPanel(wxWindow* parent,correlationBuilder* pBuilder, wxWindowID id,const wxPoint& pos,const wxSize& size) : pmPanel(),
    m_pBuilder(pBuilder)
{
	//(*Initialize(correlationPanel)
	Create(parent, id, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL, _T("id"));
	//*)

	m_pGraph = new HistoryGraph(this, wxNewId(), wxPoint(0,0), wxSize(800,480));
	m_pGraph->AddGraph(GRAPH, *wxWHITE, 853333);

	Connect(wxEVT_LEFT_UP,(wxObjectEventFunction)&correlationPanel::OnLeftUp);

	SetSize(size);
	SetPosition(pos);
}

correlationPanel::~correlationPanel()
{
	//(*Destroy(correlationPanel)
	//*)
}

void correlationPanel::SetAudioData(const timedbuffer* pBuffer)
{
    if(m_nInputChannels != 0)
    {

        auto dValue = PearsonCorrelation(pBuffer, 2, 0, 1);
        m_pGraph->AddPeak(GRAPH, dValue);
    }
}

void correlationPanel::InputSession(const session& aSession)
{
    if(aSession.GetCurrentSubsession() != aSession.lstSubsession.end())
    {
        m_nInputChannels = aSession.GetCurrentSubsession()->nChannels;
    }
    else
    {
        m_nInputChannels = 0;
    }
    m_nX = std::min(m_nInputChannels, m_nX);
    m_nY = std::min(m_nInputChannels, m_nY);
}

void correlationPanel::OutputChannels(const std::vector<char>& vChannels)
{

}


void correlationPanel::OnLeftUp(wxMouseEvent& event)
{
    m_pBuilder->Maximize((GetSize().x <= 600));
}

