#include "pnlDistortionChannel.h"
#include "fftAlgorithm.h"
#include "distortionbuilder.h"
#include "log.h"

//(*InternalHeaders(pnlDistortionChannel)
#include <wx/font.h>
#include <wx/intl.h>
#include <wx/string.h>
//*)

//(*IdInit(pnlDistortionChannel)
const long pnlDistortionChannel::ID_M_PLBL9 = wxNewId();
const long pnlDistortionChannel::ID_M_PLBL1 = wxNewId();
const long pnlDistortionChannel::ID_M_PLBL2 = wxNewId();
const long pnlDistortionChannel::ID_M_PLBL3 = wxNewId();
const long pnlDistortionChannel::ID_M_PLBL4 = wxNewId();
const long pnlDistortionChannel::ID_M_PLBL5 = wxNewId();
const long pnlDistortionChannel::ID_M_PLBL6 = wxNewId();
//*)

BEGIN_EVENT_TABLE(pnlDistortionChannel,wxPanel)
	//(*EventTable(pnlDistortionChannel)
	//*)
END_EVENT_TABLE()

pnlDistortionChannel::pnlDistortionChannel(wxWindow* parent, const wxString& sLabel,  unsigned int nChannel, unsigned int nSampleRate, wxWindowID id,const wxPoint& pos,const wxSize& size) : m_nChannel(nChannel), m_nSampleRate(nSampleRate)
{
	//(*Initialize(pnlDistortionChannel)
	Create(parent, id, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL, _T("id"));
	m_plbLChannel = new wmLabel(this, ID_M_PLBL9, wxEmptyString, wxPoint(0,0), wxSize(70,0), 0, _T("ID_M_PLBL9"));
	m_plbLChannel->SetBorderState(uiRect::BORDER_FLAT);
	m_plbLChannel->SetForegroundColour(wxColour(255,255,255));
	m_plbLChannel->SetBackgroundColour(wxColour(64,0,64));
	wxFont m_plbLChannelFont(10,wxFONTFAMILY_SWISS,wxFONTSTYLE_NORMAL,wxFONTWEIGHT_BOLD,false,_T("Arial"),wxFONTENCODING_DEFAULT);
	m_plbLChannel->SetFont(m_plbLChannelFont);
	m_plblFrequency = new wmLabel(this, ID_M_PLBL1, wxEmptyString, wxPoint(0,30), wxSize(70,25), 0, _T("ID_M_PLBL1"));
	m_plblFrequency->SetBorderState(uiRect::BORDER_FLAT);
	m_plblFrequency->SetForegroundColour(wxColour(255,255,255));
	m_plblFrequency->SetBackgroundColour(wxColour(91,91,0));
	wxFont m_plblFrequencyFont(10,wxFONTFAMILY_SWISS,wxFONTSTYLE_NORMAL,wxFONTWEIGHT_BOLD,false,_T("Arial"),wxFONTENCODING_DEFAULT);
	m_plblFrequency->SetFont(m_plblFrequencyFont);
	m_plblAmplitude = new wmLabel(this, ID_M_PLBL2, wxEmptyString, wxPoint(0,60), wxSize(70,25), 0, _T("ID_M_PLBL2"));
	m_plblAmplitude->SetBorderState(uiRect::BORDER_FLAT);
	m_plblAmplitude->SetForegroundColour(wxColour(255,255,255));
	m_plblAmplitude->SetBackgroundColour(wxColour(91,91,0));
	wxFont m_plblAmplitudeFont(10,wxFONTFAMILY_SWISS,wxFONTSTYLE_NORMAL,wxFONTWEIGHT_BOLD,false,_T("Arial"),wxFONTENCODING_DEFAULT);
	m_plblAmplitude->SetFont(m_plblAmplitudeFont);
	m_plblPeaks = new wmLabel(this, ID_M_PLBL3, wxEmptyString, wxPoint(0,90), wxSize(70,25), 0, _T("ID_M_PLBL3"));
	m_plblPeaks->SetBorderState(uiRect::BORDER_FLAT);
	m_plblPeaks->SetForegroundColour(wxColour(255,255,255));
	m_plblPeaks->SetBackgroundColour(wxColour(91,91,0));
	wxFont m_plblPeaksFont(10,wxFONTFAMILY_SWISS,wxFONTSTYLE_NORMAL,wxFONTWEIGHT_BOLD,false,_T("Arial"),wxFONTENCODING_DEFAULT);
	m_plblPeaks->SetFont(m_plblPeaksFont);
	m_plblDistortion = new wmLabel(this, ID_M_PLBL4, wxEmptyString, wxPoint(0,120), wxSize(70,25), 0, _T("ID_M_PLBL4"));
	m_plblDistortion->SetBorderState(uiRect::BORDER_FLAT);
	m_plblDistortion->SetForegroundColour(wxColour(255,255,255));
	m_plblDistortion->SetBackgroundColour(wxColour(91,91,0));
	wxFont m_plblDistortionFont(10,wxFONTFAMILY_SWISS,wxFONTSTYLE_NORMAL,wxFONTWEIGHT_BOLD,false,_T("Arial"),wxFONTENCODING_DEFAULT);
	m_plblDistortion->SetFont(m_plblDistortionFont);
	m_plblMax = new wmLabel(this, ID_M_PLBL5, wxEmptyString, wxPoint(0,150), wxSize(70,25), 0, _T("ID_M_PLBL5"));
	m_plblMax->SetBorderState(uiRect::BORDER_FLAT);
	m_plblMax->SetForegroundColour(wxColour(255,255,255));
	m_plblMax->SetBackgroundColour(wxColour(91,91,0));
	wxFont m_plblMaxFont(10,wxFONTFAMILY_SWISS,wxFONTSTYLE_NORMAL,wxFONTWEIGHT_BOLD,false,_T("Arial"),wxFONTENCODING_DEFAULT);
	m_plblMax->SetFont(m_plblMaxFont);
	m_plblTime = new wmLabel(this, ID_M_PLBL6, wxEmptyString, wxPoint(0,180), wxSize(70,40), 0, _T("ID_M_PLBL6"));
	m_plblTime->SetBorderState(uiRect::BORDER_FLAT);
	m_plblTime->SetForegroundColour(wxColour(255,255,255));
	m_plblTime->SetBackgroundColour(wxColour(91,91,0));
	wxFont m_plblTimeFont(8,wxFONTFAMILY_SWISS,wxFONTSTYLE_NORMAL,wxFONTWEIGHT_NORMAL,false,_T("Arial"),wxFONTENCODING_DEFAULT);
	m_plblTime->SetFont(m_plblTimeFont);
	//*)
	SetBackgroundColour(*wxBLACK);
	SetPosition(pos);
	SetSize(70, 250);
	m_plbLChannel->SetLabel(sLabel);
	m_dMax = 0;
}

pnlDistortionChannel::~pnlDistortionChannel()
{
	//(*Destroy(pnlDistortionChannel)
	//*)
}


void pnlDistortionChannel::AddAudioData(float dValue)
{
    m_lstBuffer.push_back(dValue);
}

pnlDistortionChannel::result pnlDistortionChannel::RunTest()
{
    result res;

    if(m_lstBuffer.empty() == false)
    {
        res.bOk = true;
        FFTAlgorithm fft;
        res.dDistortion = fft.GetTHDistortion(m_lstBuffer, m_nSampleRate, 1, 0, FFTAlgorithm::WINDOW_BLACKMAN, 1024, 0);
        res.dFrequency = fft.GetFundamentalBinFrequency();
        res.dAmplitude = fft.GetFundamentalAmplitude();
        res.nPeaks = fft.GetNumberOfPeaks();


        m_plblDistortion->SetLabel(wxString::Format(wxT("%.2f%"),res.dDistortion));
        m_plblFrequency->SetLabel(wxString::Format(wxT("~ %.0f Hz"), fft.GetFundamentalBinFrequency()));
        m_plblAmplitude->SetLabel(wxString::Format(wxT("%.2f dB"), fft.GetFundamentalAmplitude()* 0.719));
        m_plblPeaks->SetLabel(wxString::Format(wxT("%lu"), fft.GetNumberOfPeaks()));

        if(m_dMax < res.dDistortion)
        {
            m_dMax = res.dDistortion;
            m_plblMax->SetLabel(wxString::Format(wxT("%.2f%"), m_dMax));
            m_plblTime->SetLabel(wxDateTime::UNow().Format(wxT("%Y-%m-%d %H:%M:%S.%l")));

            res.dMaxDistortion = res.dDistortion;
            res.sMaxTime = m_plblTime->GetLabel();

       }

    }
    m_lstBuffer.clear();
    return res;
}

void pnlDistortionChannel::Reset()
{
    m_lstBuffer.clear();
    m_dMax = 0.0;
    m_plblDistortion->SetLabel(wxEmptyString);
    m_plblFrequency->SetLabel(wxEmptyString);
    m_plblAmplitude->SetLabel(wxEmptyString);
    m_plblMax->SetLabel(wxEmptyString);
    m_plblPeaks->SetLabel(wxEmptyString);
    m_plblTime->SetLabel(wxEmptyString);

}
