#include "pnlDistortion.h"
#include "pnlDistortionChannel.h"
//(*InternalHeaders(pnlDistortion)
#include <wx/font.h>
#include <wx/intl.h>
#include <wx/string.h>
//*)
#include "session.h"
#include "distortionbuilder.h"

//(*IdInit(pnlDistortion)
const long pnlDistortion::ID_M_PLBL37 = wxNewId();
const long pnlDistortion::ID_M_PLBL1 = wxNewId();
const long pnlDistortion::ID_M_PLBL3 = wxNewId();
const long pnlDistortion::ID_M_PLBL4 = wxNewId();
const long pnlDistortion::ID_M_PLBL5 = wxNewId();
const long pnlDistortion::ID_M_PLBL38 = wxNewId();
const long pnlDistortion::ID_M_PLBL40 = wxNewId();
const long pnlDistortion::ID_M_PBTN1 = wxNewId();
const long pnlDistortion::ID_PANEL1 = wxNewId();
const long pnlDistortion::ID_TIMER1 = wxNewId();
//*)

BEGIN_EVENT_TABLE(pnlDistortion,wxPanel)
	//(*EventTable(pnlDistortion)
	//*)
END_EVENT_TABLE()

pnlDistortion::pnlDistortion(wxWindow* parent, DistortionBuilder* pBuilder, wxWindowID id,const wxPoint& pos,const wxSize& size) :
    m_pBuilder(pBuilder)
{
	//(*Initialize(pnlDistortion)
	Create(parent, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL, _T("wxID_ANY"));
	SetBackgroundColour(wxColour(0,0,0));
	m_pLbl12 = new wmLabel(this, ID_M_PLBL37, _("Distortion Measurements"), wxPoint(0,0), wxSize(600,40), 0, _T("ID_M_PLBL37"));
	m_pLbl12->SetBorderState(uiRect::BORDER_NONE);
	m_pLbl12->SetForegroundColour(wxColour(255,255,255));
	m_pLbl12->SetBackgroundColour(wxColour(91,149,57));
	wxFont m_pLbl12Font(12,wxFONTFAMILY_SWISS,wxFONTSTYLE_NORMAL,wxFONTWEIGHT_BOLD,false,_T("Arial"),wxFONTENCODING_DEFAULT);
	m_pLbl12->SetFont(m_pLbl12Font);
	pnlLeft = new wxPanel(this, ID_PANEL1, wxPoint(0,50), wxSize(600,430), wxTAB_TRAVERSAL, _T("ID_PANEL1"));
	pnlLeft->SetBackgroundColour(wxColour(0,0,0));
	m_pLbl1 = new wmLabel(pnlLeft, ID_M_PLBL1, _("Frq"), wxPoint(5,30), wxSize(32,25), wxALIGN_LEFT, _T("ID_M_PLBL1"));
	m_pLbl1->SetBorderState(uiRect::BORDER_NONE);
	m_pLbl1->SetForegroundColour(wxColour(255,255,255));
	m_pLbl1->SetBackgroundColour(wxColour(47,94,94));
	m_pLbl3 = new wmLabel(pnlLeft, ID_M_PLBL3, _("Amp"), wxPoint(5,60), wxSize(32,25), 0, _T("ID_M_PLBL3"));
	m_pLbl3->SetBorderState(uiRect::BORDER_NONE);
	m_pLbl3->SetForegroundColour(wxColour(255,255,255));
	m_pLbl3->SetBackgroundColour(wxColour(47,94,94));
	m_pLbl2 = new wmLabel(pnlLeft, ID_M_PLBL4, _("Peaks"), wxPoint(5,90), wxSize(32,25), 0, _T("ID_M_PLBL4"));
	m_pLbl2->SetBorderState(uiRect::BORDER_NONE);
	m_pLbl2->SetForegroundColour(wxColour(255,255,255));
	m_pLbl2->SetBackgroundColour(wxColour(47,94,94));
	m_pLbl4 = new wmLabel(pnlLeft, ID_M_PLBL5, _("THD"), wxPoint(5,120), wxSize(32,25), 0, _T("ID_M_PLBL5"));
	m_pLbl4->SetBorderState(uiRect::BORDER_NONE);
	m_pLbl4->SetForegroundColour(wxColour(255,255,255));
	m_pLbl4->SetBackgroundColour(wxColour(47,94,94));
	m_pLbl15 = new wmLabel(pnlLeft, ID_M_PLBL38, _("Max"), wxPoint(5,150), wxSize(32,25), 0, _T("ID_M_PLBL38"));
	m_pLbl15->SetBorderState(uiRect::BORDER_NONE);
	m_pLbl15->SetForegroundColour(wxColour(255,255,255));
	m_pLbl15->SetBackgroundColour(wxColour(47,94,94));
	m_pLbl17 = new wmLabel(pnlLeft, ID_M_PLBL40, _("Max\nTime"), wxPoint(5,180), wxSize(32,40), 0, _T("ID_M_PLBL40"));
	m_pLbl17->SetBorderState(uiRect::BORDER_NONE);
	m_pLbl17->SetForegroundColour(wxColour(255,255,255));
	m_pLbl17->SetBackgroundColour(wxColour(47,94,94));
	m_pbtnReset = new wmButton(pnlLeft, ID_M_PBTN1, _("Reset"), wxPoint(5,370), wxSize(90,50), 0, wxDefaultValidator, _T("ID_M_PBTN1"));
	m_timerTest.SetOwner(this, ID_TIMER1);
	m_timerTest.Start(500, false);

	Connect(ID_M_PBTN1,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&pnlDistortion::OnbtnResetClick);
	Connect(ID_TIMER1,wxEVT_TIMER,(wxObjectEventFunction)&pnlDistortion::OntimerTestTrigger);
	//*)
}

pnlDistortion::~pnlDistortion()
{
	//(*Destroy(pnlDistortion)
	//*)
}


void pnlDistortion::InputSession(const session& aSession)
{
    for(size_t i = 0; i < m_vChannels.size(); i++)
    {
        pnlLeft->RemoveChild(m_vChannels[i]);
    }
    m_vChannels.clear();

    if(aSession.GetCurrentSubsession() != aSession.lstSubsession.end())
    {
        m_vChannels.resize(std::min((unsigned int)256 ,aSession.GetCurrentSubsession()->nChannels));
    }

    for(size_t i = 0; i < m_vChannels.size(); i++)
    {
        m_vChannels[i] = new pnlDistortionChannel(pnlLeft, i,m_pBuilder,  wxID_ANY, wxPoint(38+(71*i), 0));
    }
}

void pnlDistortion::SetAudioData(const timedbuffer* pBuffer)
{
    for(size_t i = 0; i < pBuffer->GetBufferSize(); i+= m_vChannels.size())
    {
        for(size_t j = 0; j < m_vChannels.size(); j++)
        {
            m_vChannels[j]->AddAudioData(pBuffer->GetBuffer()[i+j]);
        }
    }
    /*
    double dPeak[2] = {0.0,0.0};
    for(unsigned int i = 0; i < nFrames*2; i+=2)
    {
        m_lstBuffer.push_back(pBuffer[i]);
        m_lstBuffer.push_back(pBuffer[i+1]);

        dPeak[0] = max((double)fabs(pBuffer[i]), (double)dPeak[0]);
        dPeak[1] = max((double)fabs(pBuffer[i+1]), (double)dPeak[1]);
    }


    if(m_dLevelPeakMax[0] != dPeak[0])
    {
        TestLog::Get()->Log(wxT("Distortion+Level"), wxT("Left:\tMax Level: %.5f\tCurrent Level: %.5f"), m_dLevelPeakMax[0], dPeak[0]);
    }
    if(m_dLevelPeakMax[1] != dPeak[1])
    {
        TestLog::Get()->Log(wxT("Distortion+Level"), wxT("Right:\tMax Level: %.5f\tCurrent Level: %.5f"), m_dLevelPeakMax[1], dPeak[1]);
    }

    for(int i = 0; i < 2; i++)
    {

        m_dLevelPeakMax[i] = max(m_dLevelPeakMax[i], dPeak[i]);
        m_dLevelPeakMin[i] = min(m_dLevelPeakMin[i], dPeak[i]);
    }
    m_pGraph_Left->SetLevels(m_dLevelPeakMax[0], m_dLevelPeakMin[0], dPeak[0]);
    m_pGraph_Right->SetLevels(m_dLevelPeakMax[1], m_dLevelPeakMin[1], dPeak[1]);

    m_plblLevel_Left->SetLabel(wxString::Format(wxT("%.5f"), dPeak[0]));
    m_plblLevel_Right->SetLabel(wxString::Format(wxT("%.5f"), dPeak[1]));
    m_plblLevelMax_Left->SetLabel(wxString::Format(wxT("%.5f"), m_dLevelPeakMax[0]));
    m_plblLevelMax_Right->SetLabel(wxString::Format(wxT("%.5f"), m_dLevelPeakMax[1]));

    m_plblLevelMin_Left->SetLabel(wxString::Format(wxT("%.5f"), m_dLevelPeakMin[0]));
    m_plblLevelMin_Right->SetLabel(wxString::Format(wxT("%.5f"), m_dLevelPeakMin[1]));

    */
}

void pnlDistortion::RunTest()
{
/*
    FFTAlgorithm fft;
    double dDistortion = fft.GetTHDistortion(m_lstBuffer, 48000, FFTAlgorithm::ANALYSE_L, FFTAlgorithm::WINDOW_BLACKMAN, 1024, 0);
    m_plblDistortion_Left->SetLabel(wxString::Format(wxT("%.2f%"),dDistortion));
    m_plblFrequency_Left->SetLabel(wxString::Format(wxT("~ %.0f Hz"), fft.GetFundamentalBinFrequency()));
    m_plblAmplitude_Left->SetLabel(wxString::Format(wxT("%.2f dB"), fft.GetFundamentalAmplitude()* 0.719));
    m_plblPeaks_Left->SetLabel(wxString::Format(wxT("%d"), fft.GetNumberOfPeaks()));

    if(m_dMax[FFTAlgorithm::ANALYSE_L] < dDistortion)
    {
        m_dMax[FFTAlgorithm::ANALYSE_L] = dDistortion;
        m_plblMax_Left->SetLabel(wxString::Format(wxT("%.2f%"), m_dMax[FFTAlgorithm::ANALYSE_L]));
        m_plblTime_Left->SetLabel(wxDateTime::UNow().Format(wxT("%Y-%m-%d %H:%M:%S.%l")));

   }
    if(fft.GetNumberOfPeaks() > 1)
    {
        TestLog::Get()->Log(wxT("Distortion+Level"), wxT("Left:\tPeaks = %03d\tDistortion=%.2f"), fft.GetNumberOfPeaks(), dDistortion);
    }

    FFTAlgorithm fftRight;
    dDistortion = fftRight.GetTHDistortion(m_lstBuffer, 48000, FFTAlgorithm::ANALYSE_R, FFTAlgorithm::WINDOW_BLACKMAN, 1024, 0);
    m_plblDistortion_Right->SetLabel(wxString::Format(wxT("%.2f%"),dDistortion));
    m_plblFrequency_Right->SetLabel(wxString::Format(wxT("~ %.0f Hz"), fftRight.GetFundamentalBinFrequency()));
    m_plblAmplitude_Right->SetLabel(wxString::Format(wxT("%.2f dB"), fftRight.GetFundamentalAmplitude()* 0.719));
    m_plblPeaks_Right->SetLabel(wxString::Format(wxT("%d"), fftRight.GetNumberOfPeaks()));

    if(m_dMax[FFTAlgorithm::ANALYSE_R] < dDistortion)
    {
        m_dMax[FFTAlgorithm::ANALYSE_R] = dDistortion;
        m_plblMax_Right->SetLabel(wxString::Format(wxT("%.2f%"), m_dMax[FFTAlgorithm::ANALYSE_R]));
        m_plblTime_Right->SetLabel(wxDateTime::UNow().Format(wxT("%Y-%m-%d %H:%M:%S.%l")));

    }
    if(fft.GetNumberOfPeaks() > 1)
    {
        TestLog::Get()->Log(wxT("Distortion+Level"), wxT("Right:\tPeaks = %03d\tDistortion=%.2f"), fft.GetNumberOfPeaks(), dDistortion);
    }




    m_lstBuffer.clear();
*/

}



void pnlDistortion::ResetTest()
{
    for(size_t i = 0; i < m_vChannels.size(); i++)
    {
        m_vChannels[i]->Reset();
    }
}




void pnlDistortion::OntimerTestTrigger(wxTimerEvent& event)
{
    for(size_t i = 0; i < m_vChannels.size(); i++)
    {
        m_vChannels[i]->RunTest();
    }
}

void pnlDistortion::OnbtnResetClick(wxCommandEvent& event)
{
}
