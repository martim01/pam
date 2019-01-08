#include "lineuppanel.h"
#include "lineupbuilder.h"
#include "timedbuffer.h"
#include "session.h"
#include "fftAlgorithm.h"
#include "levelcalculator.h"
#include "ppmtypes.h"

//(*InternalHeaders(lineupPanel)
#include <wx/font.h>
#include <wx/intl.h>
#include <wx/string.h>
//*)

//(*IdInit(lineupPanel)
const long lineupPanel::ID_M_PLBL8 = wxNewId();
const long lineupPanel::ID_M_PLBL9 = wxNewId();
const long lineupPanel::ID_M_PLBL10 = wxNewId();
const long lineupPanel::ID_M_PLBL60 = wxNewId();
const long lineupPanel::ID_M_PLBL1 = wxNewId();
const long lineupPanel::ID_M_PLBL2 = wxNewId();
const long lineupPanel::ID_M_PLBL6 = wxNewId();
const long lineupPanel::ID_M_PLBL7 = wxNewId();
const long lineupPanel::ID_M_PLBL3 = wxNewId();
const long lineupPanel::ID_M_PLBL4 = wxNewId();
const long lineupPanel::ID_M_PLBL5 = wxNewId();
//*)

wxIMPLEMENT_DYNAMIC_CLASS(lineupPanel,pmPanel);

BEGIN_EVENT_TABLE(lineupPanel,wxPanel)
	//(*EventTable(lineupPanel)
	//*)
END_EVENT_TABLE()

lineupPanel::lineupPanel(wxWindow* parent,lineupBuilder* pBuilder, wxWindowID id,const wxPoint& pos,const wxSize& size) : pmPanel(),
    m_pBuilder(pBuilder)
{
	//(*Initialize(lineupPanel)
	Create(parent, id, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL, _T("id"));
	SetBackgroundColour(wxColour(0,0,0));
	m_pLbl2 = new wmLabel(this, ID_M_PLBL8, _("Level:"), wxPoint(5,5), wxSize(120,81), 0, _T("ID_M_PLBL8"));
	m_pLbl2->SetBorderState(uiRect::BORDER_NONE);
	m_pLbl2->GetUiRect().SetGradient(0);
	m_pLbl2->SetForegroundColour(wxColour(255,255,255));
	m_pLbl2->SetBackgroundColour(wxColour(0,128,64));
	wxFont m_pLbl2Font(16,wxFONTFAMILY_SWISS,wxFONTSTYLE_NORMAL,wxFONTWEIGHT_NORMAL,false,_T("Tahoma"),wxFONTENCODING_DEFAULT);
	m_pLbl2->SetFont(m_pLbl2Font);
	m_plblLevelL = new wmLabel(this, ID_M_PLBL9, _("1000 kHz"), wxPoint(126,5), wxSize(150,40), 0, _T("ID_M_PLBL9"));
	m_plblLevelL->SetBorderState(uiRect::BORDER_NONE);
	m_plblLevelL->GetUiRect().SetGradient(0);
	m_plblLevelL->SetForegroundColour(wxColour(255,255,255));
	m_plblLevelL->SetBackgroundColour(wxColour(0,128,64));
	wxFont m_plblLevelLFont(16,wxFONTFAMILY_SWISS,wxFONTSTYLE_NORMAL,wxFONTWEIGHT_NORMAL,false,_T("Tahoma"),wxFONTENCODING_DEFAULT);
	m_plblLevelL->SetFont(m_plblLevelLFont);
	m_plblLevelR = new wmLabel(this, ID_M_PLBL10, _("1000 kHz"), wxPoint(126,46), wxSize(150,40), 0, _T("ID_M_PLBL10"));
	m_plblLevelR->SetBorderState(uiRect::BORDER_NONE);
	m_plblLevelR->GetUiRect().SetGradient(0);
	m_plblLevelR->SetForegroundColour(wxColour(255,255,255));
	m_plblLevelR->SetBackgroundColour(wxColour(0,128,64));
	wxFont m_plblLevelRFont(16,wxFONTFAMILY_SWISS,wxFONTSTYLE_NORMAL,wxFONTWEIGHT_NORMAL,false,_T("Tahoma"),wxFONTENCODING_DEFAULT);
	m_plblLevelR->SetFont(m_plblLevelRFont);
	m_plblInput = new wmLabel(this, ID_M_PLBL60, _("Dominant:"), wxPoint(5,100), wxSize(120,81), 0, _T("ID_M_PLBL60"));
	m_plblInput->SetBorderState(uiRect::BORDER_NONE);
	m_plblInput->GetUiRect().SetGradient(0);
	m_plblInput->SetForegroundColour(wxColour(255,255,255));
	m_plblInput->SetBackgroundColour(wxColour(0,128,64));
	wxFont m_plblInputFont(16,wxFONTFAMILY_SWISS,wxFONTSTYLE_NORMAL,wxFONTWEIGHT_NORMAL,false,_T("Tahoma"),wxFONTENCODING_DEFAULT);
	m_plblInput->SetFont(m_plblInputFont);
	m_plblDominantHzL = new wmLabel(this, ID_M_PLBL1, _("1000 kHz"), wxPoint(126,100), wxSize(150,40), 0, _T("ID_M_PLBL1"));
	m_plblDominantHzL->SetBorderState(uiRect::BORDER_NONE);
	m_plblDominantHzL->GetUiRect().SetGradient(0);
	m_plblDominantHzL->SetForegroundColour(wxColour(255,255,255));
	m_plblDominantHzL->SetBackgroundColour(wxColour(0,128,64));
	wxFont m_plblDominantHzLFont(16,wxFONTFAMILY_SWISS,wxFONTSTYLE_NORMAL,wxFONTWEIGHT_NORMAL,false,_T("Tahoma"),wxFONTENCODING_DEFAULT);
	m_plblDominantHzL->SetFont(m_plblDominantHzLFont);
	m_plblDominantdBL = new wmLabel(this, ID_M_PLBL2, _("-12dBFS"), wxPoint(277,100), wxSize(150,40), 0, _T("ID_M_PLBL2"));
	m_plblDominantdBL->SetBorderState(uiRect::BORDER_NONE);
	m_plblDominantdBL->GetUiRect().SetGradient(0);
	m_plblDominantdBL->SetForegroundColour(wxColour(255,255,255));
	m_plblDominantdBL->SetBackgroundColour(wxColour(0,128,64));
	wxFont m_plblDominantdBLFont(16,wxFONTFAMILY_SWISS,wxFONTSTYLE_NORMAL,wxFONTWEIGHT_NORMAL,false,_T("Tahoma"),wxFONTENCODING_DEFAULT);
	m_plblDominantdBL->SetFont(m_plblDominantdBLFont);
	m_plblDominantHzR = new wmLabel(this, ID_M_PLBL6, _("1000 kHz"), wxPoint(126,141), wxSize(150,40), 0, _T("ID_M_PLBL6"));
	m_plblDominantHzR->SetBorderState(uiRect::BORDER_NONE);
	m_plblDominantHzR->GetUiRect().SetGradient(0);
	m_plblDominantHzR->SetForegroundColour(wxColour(255,255,255));
	m_plblDominantHzR->SetBackgroundColour(wxColour(0,128,64));
	wxFont m_plblDominantHzRFont(16,wxFONTFAMILY_SWISS,wxFONTSTYLE_NORMAL,wxFONTWEIGHT_NORMAL,false,_T("Tahoma"),wxFONTENCODING_DEFAULT);
	m_plblDominantHzR->SetFont(m_plblDominantHzRFont);
	m_plblDominantdBR = new wmLabel(this, ID_M_PLBL7, _("-12dBFS"), wxPoint(277,141), wxSize(150,40), 0, _T("ID_M_PLBL7"));
	m_plblDominantdBR->SetBorderState(uiRect::BORDER_NONE);
	m_plblDominantdBR->GetUiRect().SetGradient(0);
	m_plblDominantdBR->SetForegroundColour(wxColour(255,255,255));
	m_plblDominantdBR->SetBackgroundColour(wxColour(0,128,64));
	wxFont m_plblDominantdBRFont(16,wxFONTFAMILY_SWISS,wxFONTSTYLE_NORMAL,wxFONTWEIGHT_NORMAL,false,_T("Tahoma"),wxFONTENCODING_DEFAULT);
	m_plblDominantdBR->SetFont(m_plblDominantdBRFont);
	m_pLbl1 = new wmLabel(this, ID_M_PLBL3, _("Phase:"), wxPoint(5,200), wxSize(120,40), 0, _T("ID_M_PLBL3"));
	m_pLbl1->SetBorderState(uiRect::BORDER_NONE);
	m_pLbl1->GetUiRect().SetGradient(0);
	m_pLbl1->SetForegroundColour(wxColour(255,255,255));
	m_pLbl1->SetBackgroundColour(wxColour(0,128,64));
	wxFont m_pLbl1Font(16,wxFONTFAMILY_SWISS,wxFONTSTYLE_NORMAL,wxFONTWEIGHT_NORMAL,false,_T("Tahoma"),wxFONTENCODING_DEFAULT);
	m_pLbl1->SetFont(m_pLbl1Font);
	m_plblPhaseSamples = new wmLabel(this, ID_M_PLBL4, _("Samples"), wxPoint(126,200), wxSize(150,40), 0, _T("ID_M_PLBL4"));
	m_plblPhaseSamples->SetBorderState(uiRect::BORDER_NONE);
	m_plblPhaseSamples->GetUiRect().SetGradient(0);
	m_plblPhaseSamples->SetForegroundColour(wxColour(255,255,255));
	m_plblPhaseSamples->SetBackgroundColour(wxColour(0,128,64));
	wxFont m_plblPhaseSamplesFont(16,wxFONTFAMILY_SWISS,wxFONTSTYLE_NORMAL,wxFONTWEIGHT_NORMAL,false,_T("Tahoma"),wxFONTENCODING_DEFAULT);
	m_plblPhaseSamples->SetFont(m_plblPhaseSamplesFont);
	m_plblPhaseDegrees = new wmLabel(this, ID_M_PLBL5, _("Degrees"), wxPoint(277,200), wxSize(150,40), 0, _T("ID_M_PLBL5"));
	m_plblPhaseDegrees->SetBorderState(uiRect::BORDER_NONE);
	m_plblPhaseDegrees->GetUiRect().SetGradient(0);
	m_plblPhaseDegrees->SetForegroundColour(wxColour(255,255,255));
	m_plblPhaseDegrees->SetBackgroundColour(wxColour(0,128,64));
	wxFont m_plblPhaseDegreesFont(16,wxFONTFAMILY_SWISS,wxFONTSTYLE_NORMAL,wxFONTWEIGHT_NORMAL,false,_T("Tahoma"),wxFONTENCODING_DEFAULT);
	m_plblPhaseDegrees->SetFont(m_plblPhaseDegreesFont);
	//*)

	Connect(wxEVT_LEFT_UP,(wxObjectEventFunction)&lineupPanel::OnLeftUp);

	SetSize(size);
	SetPosition(pos);

    m_nChannel[0] = 0;
	m_nChannel[1] = 1;

	m_vBufferL.reserve(4096);
    m_vBufferR.reserve(4096);

    m_nSampleSize = 4096;
    m_vfft_out.resize((m_nSampleSize/2)+1);


    m_pLevelCalc = new LevelCalculator(-70.0);

	m_pCalc = 0;
	std::map<wxString, ppmtype>::const_iterator itType = PPMTypeManager::Get().FindType(wxT("Digital"));
    if(itType != PPMTypeManager::Get().GetTypeEnd())
    {
        m_pLevelCalc->SetMode(itType->second.nType);
        m_pLevelCalc->SetDynamicResponse(itType->second.dRiseTime, itType->second.dRisedB, itType->second.dFallTime, itType->second.dFalldB);
    }

	Connect(wxID_ANY, wxEVT_OFFSET_DONE, (wxObjectEventFunction)&lineupPanel::OnOffsetDone);
}

lineupPanel::~lineupPanel()
{
	//(*Destroy(lineupPanel)
	//*)
}

void lineupPanel::SetAudioData(const timedbuffer* pBuffer)
{
    m_pLevelCalc->CalculateLevel(pBuffer);
    m_plblLevelL->SetLabel(wxString::Format(wxT("%.2f dbFS"), m_pLevelCalc->GetLevel(0)));
    m_plblLevelR->SetLabel(wxString::Format(wxT("%.2f dbFS"), m_pLevelCalc->GetLevel(1)));


    for(size_t i = 0; i < pBuffer->GetBufferSize(); i+=m_nChannels)
    {
        if(m_vBufferL.size() == 48000)
        {
            if(m_pCalc == 0)
            {
                m_pCalc = new OffsetCalculator(this, m_vBufferL, m_vBufferR);
                m_pCalc->Create();
                m_pCalc->Run();
            }
            break;
        }
        else
        {
            m_vBufferL.push_back(pBuffer->GetBuffer()[i+m_nChannel[0]]);
            m_vBufferR.push_back(pBuffer->GetBuffer()[i+m_nChannel[1]]);
        }
    }
}

void lineupPanel::InputSession(const session& aSession)
{
    if(aSession.GetCurrentSubsession() != aSession.lstSubsession.end())
    {
        m_nSampleRate = aSession.GetCurrentSubsession()->nSampleRate;
        m_nChannels = std::min((unsigned int)256 ,aSession.GetCurrentSubsession()->nChannels);
    }
    else
    {
        m_nSampleRate = 48000;
        m_nChannels = 2;
    }

    m_pLevelCalc->InputSession(aSession);
}

void lineupPanel::OutputChannels(const std::vector<char>& vChannels)
{

}


void lineupPanel::OnLeftUp(wxMouseEvent& event)
{

}


void lineupPanel::OnOffsetDone(wxCommandEvent& event)
{
    m_pCalc = 0;

    //copy the data in to the buffer and do the fft
    m_lstBuffer.clear();
    for(size_t i = 0; i < m_vBufferL.size(); i++)
    {
        if(m_lstBuffer.size() < m_nSampleSize)
        {
            m_lstBuffer.push_back(m_vBufferL[i]);
            m_lstBuffer.push_back(m_vBufferR[i]);
        }
        else
        {
            break;
        }
    }
    DoFFT(false);
    DoFFT(true);


    m_vBufferL.clear();
    m_vBufferR.clear();
    m_vBufferL.reserve(4096);
    m_vBufferR.reserve(4096);

    m_dOffsetSamples = event.GetInt();

    double dPhase = (480000.0/m_dDominantFrequency[0])*m_dOffsetSamples;

    m_plblDominantHzL->SetLabel(wxString::Format(wxT("%.f Hz"), m_dDominantFrequency[0]));
    m_plblDominantdBL->SetLabel(wxString::Format(wxT("%.2f dbFS"), m_dDominantLevel[0]));

    m_plblDominantHzR->SetLabel(wxString::Format(wxT("%.f Hz"), m_dDominantFrequency[1]));
    m_plblDominantdBR->SetLabel(wxString::Format(wxT("%.2f dbFS"), m_dDominantLevel[1]));

    m_plblPhaseSamples->SetLabel(wxString::Format(wxT("%.0f"), m_dOffsetSamples));
    m_plblPhaseDegrees->SetLabel(wxString::Format(wxT("%.2f'"), dPhase));
}

void lineupPanel::DoFFT(bool bRight)
{
    FFTAlgorithm fft;
    m_vfft_out = fft.DoFFT(m_lstBuffer, m_nSampleRate, m_nChannels, bRight, 4, m_vfft_out.size(), 50.0);
    double dBinSize = static_cast<double>(m_nSampleRate)/static_cast<double>((m_vfft_out.size()-1)*2);
    float dMax(-80);
    double dMaxBin(0);
    for(size_t i = 0; i < m_vfft_out.size(); i++)
    {
        float dAmplitude(sqrt( (m_vfft_out[i].r*m_vfft_out[i].r) + (m_vfft_out[i].i*m_vfft_out[i].i)));
        if(dAmplitude<0)
        {
            dAmplitude=-dAmplitude;
        }
        dAmplitude /= static_cast<float>(m_vfft_out.size());
        double dLog = (20*log10(dAmplitude))* 0.724;
        dLog = std::max(dLog, -80.0);

        if(dMax <dLog)
        {
            dMax =dLog;
            dMaxBin = i;
        }
    }
    m_dDominantFrequency[bRight] = dMaxBin*dBinSize;
    m_dDominantLevel[bRight] = dMax;
}
