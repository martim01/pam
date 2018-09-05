#include "pnlLevels.h"
#include "session.h"
#include "timedbuffer.h"
#include "maxmingraph.h"

//(*InternalHeaders(pnlLevels)
#include <wx/font.h>
#include <wx/intl.h>
#include <wx/string.h>
//*)

using namespace std;

//(*IdInit(pnlLevels)
const long pnlLevels::ID_M_PLBL37 = wxNewId();
const long pnlLevels::ID_M_PBTN1 = wxNewId();
//*)

BEGIN_EVENT_TABLE(pnlLevels,wxPanel)
	//(*EventTable(pnlLevels)
	//*)
END_EVENT_TABLE()

pnlLevels::pnlLevels(wxWindow* parent,wxWindowID id,const wxPoint& pos,const wxSize& size)
{
	//(*Initialize(pnlLevels)
	Create(parent, id, wxDefaultPosition, wxSize(600,480), wxTAB_TRAVERSAL, _T("id"));
	SetBackgroundColour(wxColour(0,0,0));
	m_pLbl12 = new wmLabel(this, ID_M_PLBL37, _("Level Measurements"), wxPoint(0,0), wxSize(600,40), 0, _T("ID_M_PLBL37"));
	m_pLbl12->SetBorderState(uiRect::BORDER_NONE);
	m_pLbl12->SetForegroundColour(wxColour(255,255,255));
	m_pLbl12->SetBackgroundColour(wxColour(91,149,57));
	wxFont m_pLbl12Font(12,wxFONTFAMILY_SWISS,wxFONTSTYLE_NORMAL,wxFONTWEIGHT_BOLD,false,_T("Arial"),wxFONTENCODING_DEFAULT);
	m_pLbl12->SetFont(m_pLbl12Font);
	m_pbtnReset = new wmButton(this, ID_M_PBTN1, _("Reset"), wxPoint(5,420), wxSize(90,50), 0, wxDefaultValidator, _T("ID_M_PBTN1"));

	Connect(ID_M_PBTN1,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&pnlLevels::OnbtnResetClick);
	//*)
}

pnlLevels::~pnlLevels()
{
	//(*Destroy(pnlLevels)
	//*)
}


void pnlLevels::OnbtnResetClick(wxCommandEvent& event)
{
    ResetTest();
}


void pnlLevels::SetAudioData(const timedbuffer* pBuffer)
{
    vector<double> vPeak(m_vGraph.size(), 0.0);

    for(unsigned int i = 0; i < pBuffer->GetBufferSize(); i+=m_vGraph.size())
    {
        for(size_t j = 0; j < vPeak.size(); j++)
        {
            vPeak[j] = max((double)fabs(pBuffer->GetBuffer()[i+j]), (double)vPeak[j]);
        }
    }

    for(int i = 0; i < m_vLevelPeakMax.size(); i++)
    {
        m_vLevelPeakMax[i] = max(m_vLevelPeakMax[i], vPeak[i]);
        m_vLevelPeakMin[i] = min(m_vLevelPeakMin[i], vPeak[i]);

        m_vGraph[i]->SetLevels(m_vLevelPeakMax[i], m_vLevelPeakMin[i], vPeak[i]);
    }

}

void pnlLevels::ResetTest()
{
    CreateGraphs(m_vGraph.size());
}

void pnlLevels::InputSession(const session& aSession)
{
    if(aSession.GetCurrentSubsession() != aSession.lstSubsession.end())
    {
        CreateGraphs(min((unsigned int)256 ,aSession.GetCurrentSubsession()->nChannels));
    }
    else
    {
        CreateGraphs(0);
    }
}

void pnlLevels::CreateGraphs(unsigned int nChannels)
{
    for(size_t i = 0; i < m_vLabel.size(); i++)
    {
        m_vLabel[i]->Destroy();
        m_vGraph[i]->Destroy();
    }

    m_vLabel.clear();
    m_vGraph.clear();
    m_vLevelPeakMax.clear();
    m_vLevelPeakMin.clear();


    for(size_t i = 0 ; i < nChannels; i++)
    {
        #ifdef __WXGNU__
        wmLabel* pLabel = new wmLabel(this, wxNewId(), wxString::Format(wxT("Ch %zu"), i+1), wxPoint(10+(i*60), 50), wxSize(50,30));
        #else
        wmLabel* pLabel = new wmLabel(this, wxNewId(), wxString::Format(wxT("Ch %u"), i+1), wxPoint(10+(i*60), 50), wxSize(50,30));
        #endif
        pLabel->SetBackgroundColour(wxColour(50,50,100));
        pLabel->SetForegroundColour(*wxWHITE);
        m_vLabel.push_back(pLabel);
        m_vGraph.push_back(new MaxMinGraph(this, wxNewId(), wxPoint(10+(i*60), 80), wxSize(50,300)));
    }

    m_vLevelPeakMax = vector<double>(m_vGraph.size(), 0.0);
    m_vLevelPeakMin = vector<double>(m_vGraph.size(), 1.0);
}
