#include "pnlLevels.h"
#include "session.h"
#include "timedbuffer.h"
#include "maxmingraph.h"
#include "levelsbuilder.h"
#include <iostream>
//(*InternalHeaders(pnlLevels)
#include <wx/font.h>
#include <wx/intl.h>
#include <wx/string.h>
//*)

using namespace std;

//(*IdInit(pnlLevels)
const long pnlLevels::ID_M_PLBL37 = wxNewId();
const long pnlLevels::ID_M_PBTN1 = wxNewId();
const long pnlLevels::ID_M_PLST1 = wxNewId();
const long pnlLevels::ID_CUSTOM1 = wxNewId();
const long pnlLevels::ID_M_PLBL9 = wxNewId();
//*)

BEGIN_EVENT_TABLE(pnlLevels,wxPanel)
	//(*EventTable(pnlLevels)
	//*)
END_EVENT_TABLE()

pnlLevels::pnlLevels(wxWindow* parent,LevelsBuilder* pBuilder, wxWindowID id,const wxPoint& pos,const wxSize& size) :
    m_pBuilder(pBuilder)
{
	Create(parent, id, wxDefaultPosition, wxSize(600,480), wxTAB_TRAVERSAL, _T("id"));

	SetBackgroundColour(wxColour(0,0,0));
	m_pLbl12 = new wmLabel(this, ID_M_PLBL37, _("Level Measurements"), wxPoint(0,0), wxSize(600,40), 0, _T("ID_M_PLBL37"));
	m_pLbl12->SetBorderState(uiRect::BORDER_NONE);
	m_pLbl12->GetUiRect().SetGradient(0);
	m_pLbl12->SetForegroundColour(wxColour(255,255,255));
	m_pLbl12->SetBackgroundColour(wxColour(91,149,57));
	wxFont m_pLbl12Font(12,wxFONTFAMILY_SWISS,wxFONTSTYLE_NORMAL,wxFONTWEIGHT_BOLD,false,_T("Arial"),wxFONTENCODING_DEFAULT);
	m_pLbl12->SetFont(m_pLbl12Font);
	m_pbtnReset = new wmButton(this, ID_M_PBTN1, _("Reset"), wxPoint(5,400), wxSize(80,50), 0, wxDefaultValidator, _T("ID_M_PBTN1"));
	m_plstMontor = new wmList(this, ID_M_PLST1, wxPoint(95,398), wxSize(165,82), wmList::STYLE_SELECT, 0, wxSize(-1,40), 2, wxSize(3,1));
	m_plstMontor->SetBackgroundColour(wxColour(0,0,0));
	m_plstMontor->SetButtonColour(wxColour(wxT("#808040")));
	m_plstMontor->SetSelectedButtonColour(wxColour(wxT("#EC7600")));
	m_pAmplitude = new wmSlider(this, wxNewId(),wxT("Amplitude"), wxPoint(270,400), wxSize(265,35),5,_T("ID_CUSTOM1"));
	m_pbtndB = new wmButton(this, wxNewId(), _("dB"), wxPoint(540,400), wxSize(60,35), 0, wxDefaultValidator, _T("ID_M_PLBL9"));
	m_pbtndB->SetForegroundColour(wxColour(0,0,0));
	m_pbtndB->SetBackgroundColour(wxColour(255,255,255));
	m_pRange = new wmSlider(this,ID_CUSTOM1,wxT("Range"),wxPoint(270,440),wxSize(265,35),5,_T("ID_CUSTOM1"));
	m_pbtnRangedB = new wmButton(this, ID_M_PLBL9, _("dB"), wxPoint(540,440), wxSize(60,35), 0, wxDefaultValidator, _T("ID_M_PLBL9"));
	m_pbtnRangedB->SetForegroundColour(wxColour(0,0,0));
	m_pbtnRangedB->SetBackgroundColour(wxColour(255,255,255));


	m_plstMontor->AddButton(wxT("Monitor\nLevel"));
	m_plstMontor->AddButton(wxT("Monitor\nRange"));
	m_plstMontor->AddButton(wxT("Monitor\nMin,Max"));

	m_plstMontor->SelectButton(m_pBuilder->ReadSetting(wxT("Monitor"),0), true);

	Connect(m_pAmplitude->GetId(), wxEVT_SLIDER_MOVE, (wxObjectEventFunction)&pnlLevels::OnSliderMove);
	Connect(m_pRange->GetId(), wxEVT_SLIDER_MOVE, (wxObjectEventFunction)&pnlLevels::OnRangeSliderMove);
    Connect(ID_M_PBTN1,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&pnlLevels::OnbtnResetClick);
	Connect(ID_M_PLST1,wxEVT_LIST_SELECTED,(wxObjectEventFunction)&pnlLevels::OnlstMontorSelected);

	Connect(m_pbtndB->GetId(),wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&pnlLevels::OnbtndBClick);
	Connect(m_pbtnRangedB->GetId(),wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&pnlLevels::OnbtnRangedBClick);

    std::cout << "IniLevelsAmp" << std::endl;
    m_pAmplitude->SetSliderColour(wxColour(128,128,128));
    m_pAmplitude->SetButtonColour(wxColour(100,255,100));

    std::cout << "IniLevelsRange" << std::endl;
    m_pRange->SetSliderColour(wxColour(128,128,128));
    m_pRange->SetButtonColour(wxColour(100,255,100));

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
        m_vGraph.push_back(new MaxMinGraph(i+1, this, m_pBuilder, wxNewId(), wxPoint(10+(i*60), 80), wxSize(50,300)));
    }

    switch(m_pBuilder->ReadSetting(wxT("Monitor"),0))
    {
        case 1:
            for(size_t i = 0; i < m_vGraph.size(); i++)
            {
                m_vGraph[i]->SetMaxRange(m_pRange->GetPosition());
            }
            break;
        case 2:
            for(size_t i = 0; i < m_vGraph.size(); i++)
            {
                m_vGraph[i]->SetMinAmplitude(m_pRange->GetPosition()-90.0);
                m_vGraph[i]->SetMaxAmplitude(m_pAmplitude->GetPosition()-90.0);
            }
            break;
    }

    m_vLevelPeakMax = vector<double>(m_vGraph.size(), 0.0);
    m_vLevelPeakMin = vector<double>(m_vGraph.size(), 1.0);
}

void pnlLevels::OnlstMontorSelected(wxCommandEvent& event)
{
    m_pBuilder->WriteSetting(wxT("Monitor"), event.GetInt());
    switch(event.GetInt())
    {
        case 0:
            m_pAmplitude->Hide();
            m_pbtndB->Hide();
            m_pRange->Hide();
            m_pbtnRangedB->Hide();
            break;
        case 1:
            m_pAmplitude->Hide();
            m_pbtndB->Hide();
            m_pRange->Show();
            m_pbtnRangedB->Show();
            m_pRange->Init(0,50, m_pBuilder->ReadSetting(wxT("Range"), 40));
            m_pbtnRangedB->SetLabel(wxString::Format(wxT("%.1f dB"),m_pRange->GetPosition()));
            break;
        case 2:
            m_pAmplitude->Show();
            m_pbtndB->Show();
            m_pRange->Show();
            m_pbtnRangedB->Show();
            m_pRange->Init(0,90, m_pBuilder->ReadSetting(wxT("LevelMin"), -25)+90);
            m_pbtnRangedB->SetLabel(wxString::Format(wxT("%.1f dB"),m_pRange->GetPosition()-90));
            m_pAmplitude->Init(0,90, m_pBuilder->ReadSetting(wxT("LevelMax"), -10)+90);
            m_pbtndB->SetLabel(wxString::Format(wxT("%.1f dB"),m_pAmplitude->GetPosition()-90));
            break;
    }

}


void pnlLevels::OnRangeSliderMove(wxCommandEvent& event)
{

    if(m_pBuilder->ReadSetting(wxT("Monitor"),0) == 1)
    {
        m_pbtnRangedB->SetLabel(wxString::Format(wxT("%.1f dB"),m_pRange->GetPosition()));
        m_pBuilder->WriteSetting(wxT("Range"), wxString::Format(wxT("%.1f"),m_pRange->GetPosition()));
        for(size_t i = 0; i < m_vGraph.size(); i++)
        {
            m_vGraph[i]->SetMaxRange(m_pRange->GetPosition());
        }
    }
    else
    {
        m_pbtnRangedB->SetLabel(wxString::Format(wxT("%.1f dB"),m_pRange->GetPosition()-90.0));
        m_pBuilder->WriteSetting(wxT("LevelMin"), wxString::Format(wxT("%.1f"),m_pRange->GetPosition()-90.0));

        if(m_pRange->GetPosition() > m_pAmplitude->GetPosition())
        {
            m_pAmplitude->SetSliderPosition(m_pRange->GetPosition(), true);
        }

        for(size_t i = 0; i < m_vGraph.size(); i++)
        {
            m_vGraph[i]->SetMinAmplitude(m_pRange->GetPosition()-90.0);
        }
    }


}

void pnlLevels::OnSliderMove(wxCommandEvent& event)
{
    m_pbtndB->SetLabel(wxString::Format(wxT("%.1f dB"),m_pAmplitude->GetPosition()-90.0));
    m_pBuilder->WriteSetting(wxT("LevelMax"), wxString::Format(wxT("%.1f"),m_pAmplitude->GetPosition()-90.0));

    if(m_pAmplitude->GetPosition() < m_pRange->GetPosition())
    {
        m_pRange->SetSliderPosition(m_pAmplitude->GetPosition(), true);
    }

    for(size_t i = 0; i < m_vGraph.size(); i++)
    {
        m_vGraph[i]->SetMaxAmplitude(m_pAmplitude->GetPosition()-90.0);
    }
}


void pnlLevels::OnbtndBClick(wxCommandEvent& event)
{

}

void pnlLevels::OnbtnRangedBClick(wxCommandEvent& event)
{

}
