#include "spectogrammeter.h"
#include <wx/dc.h>
#include <wx/dcbuffer.h>
#include <wx/log.h>
#include "kaiserwindow.h"
#include "uirect.h"
#include "fftAlgorithm.h"
#include "timedbuffer.h"
#include "spectogrambuilder.h"

#include "settings.h"


using namespace std;

BEGIN_EVENT_TABLE(SpectogramMeter, pmControl)
    EVT_PAINT(SpectogramMeter::OnPaint)
    EVT_SIZE(SpectogramMeter::OnSize)
    EVT_LEFT_DOWN(SpectogramMeter::OnLeftDown)
    EVT_LEFT_UP(SpectogramMeter::OnLeftUp)
END_EVENT_TABLE()

 wxIMPLEMENT_DYNAMIC_CLASS(SpectogramMeter, pmControl);

const wxString SpectogramMeter::LABEL_DISPLAY[3] = {wxT("Graph"), wxT("Lines"), wxT("EQ")};
const wxString SpectogramMeter::LABEL_ANALYSE[10] = {wxT("CH 1"), wxT("CH 2"), wxT("CH 3"), wxT("CH 4"), wxT("CH 5"), wxT("CH 6"), wxT("CH 7"), wxT("CH 8"), wxT("Middle"), wxT("Side")};
const wxString SpectogramMeter::LABEL_WINDOW[6] = {wxT("None"), wxT("Hann"), wxT("Hamming"), wxT("Blackman"), wxT("Kaiser"), wxT("KaiserBessel")};
const wxString SpectogramMeter::LABEL_TYPE[3] = {wxT("FFT"), wxT("1/3 Octave"), wxT("Peaks")};
const wxColour SpectogramMeter::COLOUR_LABEL = wxColour(65,92,192);
const double SpectogramMeter::OCTAVE_LOW_1 = 8.77;
const double SpectogramMeter::OCTAVE_LOW_2 = 11.05;
const double SpectogramMeter::OCTAVE_LOW_3 = 13.9;


SpectogramMeter::SpectogramMeter(wxWindow *parent, SpectogramBuilder* pBuilder, wxWindowID id, const wxPoint& pos, const wxSize& size) : pmControl(),
    m_pBuilder(pBuilder)
{
    Create(parent, id, pos, size);
    m_nPeakCutoff = -50;
    SetDisplayType(DISPLAY_GRAPH);
    SetAnalyseMode(ANALYSE_L);
    SetWindowType(WINDOW_BLACKMAN);
    SetOverlap(50.0);
    m_dFall = 2;
    m_nBinSelected = 0;
    m_bCursorMode = false;

    m_nChannels = 2;
    m_nSampleRate = 48000;

    m_nNudge = NONE;

    SetHeatMap(MAP_PPM);
    SetNumberOfBins(1024);
}

bool SpectogramMeter::Create(wxWindow *parent, wxWindowID id, const wxPoint& pos, const wxSize& size)
{
    wxSize szInit(size);
    wxSize bestSize = DoGetBestSize();
    if(size.x<=0)
        szInit.SetWidth(bestSize.x);
    if(size.y <= 0)
        szInit.SetHeight(bestSize.y);

    if(!wxWindow::Create(parent,id,pos,szInit,wxWANTS_CHARS, wxT("fft")))
        return false;

    SetMinSize(size);

    SetBackgroundStyle(wxBG_STYLE_CUSTOM);

    m_rectGrid = wxRect(GetClientRect().GetLeft()+10, GetClientRect().GetTop()+5, GetClientRect().GetWidth()-100, GetClientRect().GetHeight()-55);

    m_uiSettingsDisplay.SetBackgroundColour(COLOUR_LABEL);
    m_uiSettingsMeter.SetBackgroundColour(COLOUR_LABEL);
    m_uiSettingsAnalyse.SetBackgroundColour(COLOUR_LABEL);

    m_uiSettingsWindow.SetBackgroundColour(COLOUR_LABEL);
    m_uiSettingsOverlap.SetBackgroundColour(COLOUR_LABEL);
    m_uiSettingsBins.SetBackgroundColour(COLOUR_LABEL);

    m_uiPeakFrequency.SetBackgroundColour(COLOUR_LABEL);
    m_uiPeakLevel.SetBackgroundColour(COLOUR_LABEL);

    m_bHold = false;

    m_timerNudge.SetOwner(this);
    Connect(wxID_ANY, wxEVT_TIMER, (wxObjectEventFunction)&SpectogramMeter::OnTimerNudge);


    return true;
}

SpectogramMeter::~SpectogramMeter()
{

}


void SpectogramMeter::OnPaint(wxPaintEvent& event)
{
    wxAutoBufferedPaintDC dc(this);
    wxBrush br(*wxBLACK);
    dc.SetBrush(br);
    dc.DrawRectangle(GetClientRect());


    DrawSpectogram(dc);


    dc.SetTextForeground(GetForegroundColour());
    wxPen penLine(wxColour(120,120,120),1);
    uiRect uiLabel(wxRect(m_rectGrid.GetLeft()-50, 0, m_rectGrid.GetLeft(), 0));

    dc.SetFont(wxFont(7,wxFONTFAMILY_SWISS,wxFONTSTYLE_NORMAL,wxFONTWEIGHT_NORMAL,false,_T("Verdana"),wxFONTENCODING_DEFAULT));
    for(size_t i = 1; i < m_vfft_out.size()-1; i++)
    {
        dc.SetPen(penLine);
        int x = m_rectGrid.GetLeft()+i;
        if(x > uiLabel.GetLeft()+65 && x +40 < m_rectGrid.GetRight())
        {
            dc.DrawLine(x, 0, x, m_rectGrid.GetHeight());
            uiLabel.SetRect(wxRect(x-20, m_rectGrid.GetBottom()+1, 40, 20));
            uiLabel.Draw(dc, wxString::Format(wxT("%.0f"), m_dBinSize*static_cast<double>(i)), uiRect::BORDER_NONE);
        }

    }

    dc.DrawBitmap(m_bmpScale, m_rectGrid.GetRight()+10, m_rectGrid.GetTop());

    double dPos = static_cast<double>(m_rectGrid.GetHeight())/80.0;
    dc.SetPen(wxPen(*wxWHITE,1));

    dc.DrawLine(m_rectGrid.GetRight()+10, m_rectGrid.GetTop()+ (dPos*6.0), m_rectGrid.GetRight()+60, m_rectGrid.GetTop()+ (dPos*6.0));
    uiLabel.SetRect(wxRect(m_rectGrid.GetRight()+25,  m_rectGrid.GetTop()+(dPos*6.0)-7, 20,14));
    uiLabel.Draw(dc, wxT("7"), uiRect::BORDER_NONE);

    dc.SetPen(wxPen(*wxWHITE,1));
    dc.DrawLine(m_rectGrid.GetRight()+10, m_rectGrid.GetTop()+ (dPos*10.0), m_rectGrid.GetRight()+60, m_rectGrid.GetTop()+ (dPos*10.0));
    uiLabel.SetRect(wxRect(m_rectGrid.GetRight()+25,  m_rectGrid.GetTop()+(dPos*10.0)-7, 20,14));
    uiLabel.Draw(dc, wxT("6"), uiRect::BORDER_NONE);

    dc.SetPen(wxPen(*wxWHITE,1));
    dc.DrawLine(m_rectGrid.GetRight()+10, m_rectGrid.GetTop()+ (dPos*18.0), m_rectGrid.GetRight()+60, m_rectGrid.GetTop()+ (dPos*18.0));
    uiLabel.SetRect(wxRect(m_rectGrid.GetRight()+25,  m_rectGrid.GetTop()+(dPos*18.0)-7, 20,14));
    uiLabel.Draw(dc, wxT("4"), uiRect::BORDER_NONE);

    dc.SetPen(wxPen(*wxWHITE,1));
    dc.DrawLine(m_rectGrid.GetRight()+10, m_rectGrid.GetTop()+ (dPos*26.0), m_rectGrid.GetRight()+60, m_rectGrid.GetTop()+ (dPos*26.0));
    uiLabel.SetRect(wxRect(m_rectGrid.GetRight()+25,  m_rectGrid.GetTop()+(dPos*26.0)-7, 20,14));
    uiLabel.Draw(dc, wxT("2"), uiRect::BORDER_NONE);



    m_uiSettingsAnalyse.Draw(dc, uiRect::BORDER_DOWN);
    m_uiSettingsWindow.Draw(dc, uiRect::BORDER_FLAT);
    m_uiSettingsOverlap.Draw(dc, uiRect::BORDER_FLAT);
    m_uiSettingsBins.Draw(dc, uiRect::BORDER_FLAT);
    m_uiSettingsDisplay.Draw(dc, uiRect::BORDER_FLAT);
    m_uiSettingsMeter.Draw(dc, uiRect::BORDER_FLAT);
    m_uiPeakFrequency.Draw(dc, uiRect::BORDER_FLAT);
    m_uiPeakLevel.Draw(dc, uiRect::BORDER_FLAT);
}


void SpectogramMeter::DrawSpectogram(wxDC& dc)
{
    int i = m_rectGrid.GetTop();
    for(list<wxBitmap>::iterator itBitmap = m_lstBitmaps.begin(); itBitmap != m_lstBitmaps.end(); ++itBitmap)
    {
        dc.DrawBitmap((*itBitmap), m_rectGrid.GetLeft(), i);
        i++;
    }
}


void SpectogramMeter::OnSize(wxSizeEvent& event)
{
    m_rectGrid = wxRect(GetClientRect().GetLeft()+10, GetClientRect().GetTop(), GetClientRect().GetWidth()-70, GetClientRect().GetHeight()-55);

    CreateScaleBitmap();

    //m_uiSettingsDisplay.SetRect(m_rectGrid.GetLeft(), GetClientRect().GetBottom()-23, 80,20);
    //m_uiSettingsMeter.SetRect(m_uiSettingsDisplay.GetRight()+5, GetClientRect().GetBottom()-23, 80,20);
    m_uiSettingsAnalyse.SetRect(m_uiSettingsMeter.GetRight()+5, GetClientRect().GetBottom()-23, 80,20);

    m_uiSettingsWindow.SetRect(m_uiSettingsAnalyse.GetRight()+10, GetClientRect().GetBottom()-23, 80,20);
    m_uiSettingsOverlap.SetRect(m_uiSettingsWindow.GetRight()+5, GetClientRect().GetBottom()-23, 120,20);
    m_uiSettingsBins.SetRect(m_uiSettingsOverlap.GetRight()+5, GetClientRect().GetBottom()-23, 120,20);

    //m_uiPeakFrequency.SetRect(m_uiSettingsBins.GetRight()+15, GetClientRect().GetBottom()-23, 80,20);
    //m_uiPeakLevel.SetRect(m_uiPeakFrequency.GetRight()+3, GetClientRect().GetBottom()-23, 80,20);

    //m_uiClose.SetRect(GetClientRect().GetRight()-85, GetClientRect().GetTop()+5, 80, 50);
    //m_uiClose.SetBackgroundColour(*wxRED);

    //m_uiAmplitude.SetRect(m_uiClose.GetLeft()-120, m_uiClose.GetTop(), 100, m_uiClose.GetHeight());

    m_uiBin.SetRect(m_uiAmplitude.GetLeft()-180, m_uiAmplitude.GetTop(), 100, m_uiAmplitude.GetHeight());
    m_uiNudgeUp.SetRect(m_uiBin.GetRight()+5, m_uiAmplitude.GetTop(), 50, m_uiAmplitude.GetHeight());
    m_uiNudgeDown.SetRect(m_uiBin.GetLeft()-55, m_uiAmplitude.GetTop(), 50, m_uiAmplitude.GetHeight());

    m_uiNudgeDown.SetBackgroundColour(wxColour(100,100,160));
    m_uiNudgeUp.SetBackgroundColour(wxColour(100,100,160));


    Refresh();

}


void SpectogramMeter::SetFreqMag(const freq_mag& fm)
{


}

void SpectogramMeter::SetSampleRate(unsigned long nSampleRate)
{
    m_nSampleRate = nSampleRate;
}

void SpectogramMeter::SetNumberOfChannels(unsigned int nChannels)
{
    m_nChannels = nChannels;
}

void SpectogramMeter::SetData(const timedbuffer* pBuffer)
{
    if(!m_bHold && m_nChannels != 0)
    {
        for(size_t i = 0; i < pBuffer->GetBufferSize(); i++)
        {
            m_lstBuffer.push_back(pBuffer->GetBuffer()[i]);
        }

        while(m_lstBuffer.size() > (m_vfft_out.size()-1)*2*m_nChannels)
        {
            DoFFT();
        }
    }
}

void SpectogramMeter::DoFFT()
{
    FFTRoutine();
    Refresh();
}

void SpectogramMeter::FFTRoutine()
{
    FFTAlgorithm fft;

    wxImage anImage(max(730, m_rectGrid.GetWidth()),1);

    m_vfft_out = fft.DoFFT(m_lstBuffer, m_nSampleRate, m_nChannels, m_nFFTAnalyse, m_nWindowType, m_vfft_out.size(), m_nOverlap);
    m_dBinSize = static_cast<double>(m_nSampleRate)/static_cast<double>((m_vfft_out.size()-1)*2);
    float dMax(-80);
    double dMaxBin(0);

    double dTest(0);
    for(size_t i = 0; i < m_vfft_out.size(); i++)
    {
        float dAmplitude(sqrt( (m_vfft_out[i].r*m_vfft_out[i].r) + (m_vfft_out[i].i*m_vfft_out[i].i)));
        if(dAmplitude<0)
        {
            dAmplitude=-dAmplitude;
        }
        dAmplitude /= static_cast<float>(m_vfft_out.size());

        double dLog = WindowMod(20*log10(dAmplitude));
        dLog = max(dLog, -80.0)+80.0;

        dAmplitude = dLog/80.0;

        float dRed, dGreen, dBlue;
        m_HeatMap.getColourAtValue(dAmplitude, dRed, dGreen, dBlue);


        //we show the first 800 bins at most
        unsigned int nPixel = i*3;

        if(nPixel < anImage.GetWidth()*3)
        {
            anImage.GetData()[nPixel] = static_cast<unsigned char>(dRed*255.0);
            anImage.GetData()[nPixel+1] = static_cast<unsigned char>(dGreen*255.0);
            anImage.GetData()[nPixel+2] = static_cast<unsigned char>(dBlue*255.0);

         //   wxLogDebug(wxT("%f=%d,%d,%d"), dAmplitude, anImage.GetData()[nPixel],anImage.GetData()[nPixel+1],anImage.GetData()[nPixel+2]);
        }


    }
    m_lstBitmaps.push_back(wxBitmap(anImage));
    if(m_lstBitmaps.size() > m_rectGrid.GetHeight())
    {
        m_lstBitmaps.pop_front();
    }
    Refresh();
}

float SpectogramMeter::WindowMod(float dAmplitude)
{
    switch(m_nWindowType)
    {
        case WINDOW_RECTANGULAR:
            return dAmplitude * 0.909;
        case WINDOW_HANNING:
            return dAmplitude * 0.754;
        case WINDOW_HAMMING:
            return dAmplitude * 0.909;
        case WINDOW_BLACKMAN:
            return dAmplitude * 0.719;
        case WINDOW_KAISER:
            return dAmplitude * 0.724;
        case WINDOW_KAISERBESSEL:
            return dAmplitude * 0.711;

    }
    return dAmplitude;
}

void SpectogramMeter::SetAnalyseMode(int nMode)
{
    m_nFFTAnalyse = nMode;
    if(m_nChannels != 2 || nMode > 2)
    {
        m_uiSettingsAnalyse.SetLabel(LABEL_ANALYSE[nMode]);
    }
    else if(nMode == 0)
    {
        m_uiSettingsAnalyse.SetLabel(wxT("Left"));
    }
    else if(nMode == 1)
    {
        m_uiSettingsAnalyse.SetLabel(wxT("Right"));
    }

    RefreshRect(m_uiSettingsAnalyse.GetRect());
}

void SpectogramMeter::SetWindowType(int nType)
{
    m_lstBuffer.clear();
    m_nWindowType = nType;
    m_uiSettingsWindow.SetLabel(LABEL_WINDOW[nType]);
    RefreshRect(m_uiSettingsWindow.GetRect());
}

void SpectogramMeter::SetOverlap(double dPercent)
{
    m_dOverlapPercent = max(0.0, min(100.0, dPercent));
    m_nOverlap = m_nSampleSize - static_cast<size_t>(m_dOverlapPercent * static_cast<double>(m_nSampleSize)/100);

    m_uiSettingsOverlap.SetLabel(wxString::Format(wxT("Overlap: %.2f%"), m_dOverlapPercent));
    RefreshRect(m_uiSettingsOverlap.GetRect());

}


void SpectogramMeter::SetNumberOfBins(size_t nBins)
{
    m_nSampleSize = nBins*2;
    m_vfft_out.resize(nBins+1);

    //store here so we can get back
    m_nNumberOfSetBins = nBins;

    SetOverlap(m_dOverlapPercent);
    m_uiSettingsBins.SetLabel(wxString::Format(wxT("Res: %.1f Hz"), 24000.0/static_cast<double>(nBins)));
    RefreshRect(m_uiSettingsBins.GetRect());

}


void SpectogramMeter::SetDisplayType(unsigned int nDisplay)
{
    m_nDisplayType = nDisplay%3;
    m_uiSettingsDisplay.SetLabel(LABEL_DISPLAY[m_nDisplayType]);
    Refresh();
}


void SpectogramMeter::SetHold(bool bHold)
{
    m_bHold = bHold;
}


void SpectogramMeter::SetCursorMode(bool bSet)
{
    m_bCursorMode = bSet;
    if(m_bCursorMode == true)
    {
        m_pBuilder->Maximize(true);
    }
    else
    {
        TurnoffNudge();
        Refresh();
    }
}

void SpectogramMeter::OnLeftDown(wxMouseEvent& event)
{
    if(m_bCursorMode)
    {
        m_nNudge = NONE;
        if(m_uiClose.Contains(event.GetPosition()))
        {
        }
        else if(m_uiNudgeDown.Contains(event.GetPosition()))
        {
            m_nBinSelected = max(m_nBinSelected-1, (size_t)1);
            m_timerNudge.Start(100);
            m_nNudge = DOWN;
        }
        else if(m_uiNudgeUp.Contains(event.GetPosition()))
        {
            m_nBinSelected = min(m_nBinSelected+1, m_vfft_out.size()-1);
            m_timerNudge.Start(100);
            m_nNudge = UP;
        }
        else
        {

            //convert x to BIN
            double dBin = static_cast<double>(event.GetPosition().x-m_rectGrid.GetLeft());
            dBin = exp(dBin * log(m_vfft_out.size()) / static_cast<double>(m_rectGrid.GetWidth()));

            if(dBin > 0 && dBin < m_vfft_out.size())
            {
                m_nBinSelected = dBin;
                Refresh();
            }
        }
    }

}
void SpectogramMeter::OnTimerNudge(wxTimerEvent& event)
{
    if(m_nNudge == UP)
    {
        m_nBinSelected = min(m_nBinSelected+1, m_vfft_out.size()-1);
    }
    else if(m_nNudge == DOWN)
    {
        m_nBinSelected = max(m_nBinSelected-1, (size_t)1);
    }
    Refresh();
}


void SpectogramMeter::TurnoffNudge()
{
    m_timerNudge.Stop();
    m_nNudge = NONE;
    RefreshRect(m_uiNudgeDown.GetRect());
    RefreshRect(m_uiNudgeUp.GetRect());
}


void SpectogramMeter::OnLeftUp(wxMouseEvent& event)
{
    if(m_bCursorMode == false)
    {
        m_pBuilder->Maximize((GetSize().x<=600));
    }
    else
    {
        if(m_uiClose.Contains(event.GetPosition()))
        {
            m_bCursorMode = false;
    }
        else
        {
            TurnoffNudge();
        }
    }

}


void SpectogramMeter::SetHeatMap(int nMap)
{
    switch(nMap)
    {
    case MAP_MONO:
        m_HeatMap.createMonochromeHeatMapGradient();
        break;
    case MAP_HOTCOLD:
        m_HeatMap.createColdHotHeatMapGradient();
        break;
    case MAP_FIVE:
        m_HeatMap.createDefaultHeatMapGradient();
        break;
    case MAP_PPM:
        m_HeatMap.createPPMHeatMapGradient();
        break;
    case MAP_SEVEN:
        m_HeatMap.createSevenHeatMapGradient();
        break;
    }

    CreateScaleBitmap();
    Refresh();
}

void SpectogramMeter::CreateScaleBitmap()
{
    wxImage anImage(50, m_rectGrid.GetHeight());

    double dColour(1.0);
    unsigned int nPixel(0);
    for(int i = 0; i < m_rectGrid.GetHeight(); i++)
    {
        float dRed, dGreen, dBlue;

        m_HeatMap.getColourAtValue(dColour, dRed, dGreen, dBlue);
        dColour -= 1.0/(static_cast<double>(m_rectGrid.GetHeight()));


        for(int j = 0; j < 50; j++)
        {
            anImage.GetData()[nPixel] = static_cast<unsigned char>(dRed*255.0);
            anImage.GetData()[nPixel+1] = static_cast<unsigned char>(dGreen*255.0);
            anImage.GetData()[nPixel+2] = static_cast<unsigned char>(dBlue*255.0);

            nPixel+=3;
        }
    }

    //add in the 0dBU line
    int nZero = static_cast<double>(m_rectGrid.GetHeight())*0.775;
    nZero = (nZero/3)*3;

    for(int i = 0; i < 50; i++)
    {
        anImage.GetData()[nZero+i] = 255;
        anImage.GetData()[nZero+1+i] = 255;
        anImage.GetData()[nZero+2+i] = 255;
    }

    m_bmpScale = wxBitmap(anImage);

    Refresh();

}
