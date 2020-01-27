#include "fftmeter.h"
#include <wx/dc.h>
#include <wx/dcbuffer.h>
#include <wx/log.h>
#include "kaiserwindow.h"
#include "uirect.h"
#include "fftAlgorithm.h"
#include "timedbuffer.h"
#include "fftbuilder.h"
//#include "settings.h"


using namespace std;

 wxIMPLEMENT_DYNAMIC_CLASS(FftMeter, pmControl);

BEGIN_EVENT_TABLE(FftMeter, pmControl)
    EVT_PAINT(FftMeter::OnPaint)
    EVT_SIZE(FftMeter::OnSize)
    EVT_LEFT_DOWN(FftMeter::OnLeftDown)
    EVT_LEFT_UP(FftMeter::OnLeftUp)
END_EVENT_TABLE()



const wxString FftMeter::LABEL_DISPLAY[3] = {wxT("Graph"), wxT("Lines"), wxT("EQ")};
const wxString FftMeter::LABEL_ANALYSE[10] = {wxT("CH 1"), wxT("CH 2"), wxT("CH 3"), wxT("CH 4"), wxT("CH 5"), wxT("CH 6"), wxT("CH 7"), wxT("CH 8"), wxT("Middle"), wxT("Side")};
const wxString FftMeter::LABEL_WINDOW[6] = {wxT("None"), wxT("Hann"), wxT("Hamming"), wxT("Blackman"), wxT("Kaiser"), wxT("KaiserBessel")};
const wxString FftMeter::LABEL_TYPE[3] = {wxT("FFT"), wxT("1/3 Octave"), wxT("Peaks")};
const wxColour FftMeter::COLOUR_LABEL = wxColour(65,92,192);
const double FftMeter::OCTAVE_LOW_1 = 8.77;
const double FftMeter::OCTAVE_LOW_2 = 11.05;
const double FftMeter::OCTAVE_LOW_3 = 13.9;


FftMeter::FftMeter(wxWindow *parent, FFTBuilder* pBuilder, wxWindowID id, const wxPoint& pos, const wxSize& size) : pmControl(),
    m_pBuilder(pBuilder)
{
    Create(parent, id, pos, size);
    m_nPeakCutoff = -50;
    SetDisplayType(DISPLAY_GRAPH);
    SetAnalyseMode(ANALYSE_L);
    SetWindowType(WINDOW_BLACKMAN);
    SetMeter(FFT);
    m_vThirdOctave.resize(35);
    SetOverlap(50.0);
    m_dFall = 0.2;
    m_nBinSelected = 0;
    m_bCursorMode = false;

    m_nChannels = 2;
    m_nSampleRate = 48000;

    m_nNudge = NONE;
    m_bColour = (pBuilder->ReadSetting(wxT("Colour"), 0)==1);

    m_HeatMap.createDefaultHeatMapGradient();

    SetNumberOfBins(1024);
}

bool FftMeter::Create(wxWindow *parent, wxWindowID id, const wxPoint& pos, const wxSize& size)
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

    m_rectGrid = wxRect(GetClientRect().GetLeft()+40, GetClientRect().GetTop(), GetClientRect().GetWidth()-40, GetClientRect().GetHeight()-50);

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
    Connect(wxID_ANY, wxEVT_TIMER, (wxObjectEventFunction)&FftMeter::OnTimerNudge);

    SetFont(wxFont(8,wxFONTFAMILY_SWISS,wxFONTSTYLE_NORMAL,wxFONTWEIGHT_NORMAL,false,_T("Arial"),wxFONTENCODING_DEFAULT));

    return true;
}

FftMeter::~FftMeter()
{

}


void FftMeter::OnPaint(wxPaintEvent& event)
{
    wxAutoBufferedPaintDC dc(this);
    wxBrush br(*wxBLACK);
    dc.SetBrush(br);
    dc.DrawRectangle(GetClientRect());





    //draw horizonal lines
    uiRect uiLevel;
    for(double i = 0; i < 80; i += 10)
    {
        int nDB = (static_cast<double>(m_rectGrid.GetHeight()/70) * i);

        uiLevel.SetRect(5, nDB-10, m_rectGrid.GetLeft()-5, 20);
        uiLevel.SetLabel(wxString::Format(wxT("%.0f"),(-i)));
        uiLevel.Draw(dc,uiRect::BORDER_NONE);
        dc.SetPen(wxPen(wxColour(100,100,100),1 ));
        dc.DrawLine(m_rectGrid.GetLeft(), nDB, m_rectGrid.GetWidth()+m_rectGrid.GetLeft(), nDB);

    }

    switch(m_nMeterType)
    {
        case FFT:
        case PEAKS:
            DrawFFT(dc);
            break;
        case OCTAVE:
            DrawThirds(dc);
            break;
    }



    m_uiSettingsAnalyse.Draw(dc, uiRect::BORDER_DOWN);
    m_uiSettingsWindow.Draw(dc, uiRect::BORDER_FLAT);
    m_uiSettingsOverlap.Draw(dc, uiRect::BORDER_FLAT);
    m_uiSettingsBins.Draw(dc, uiRect::BORDER_FLAT);
    m_uiSettingsDisplay.Draw(dc, uiRect::BORDER_FLAT);
    m_uiSettingsMeter.Draw(dc, uiRect::BORDER_FLAT);
    m_uiPeakFrequency.Draw(dc, uiRect::BORDER_FLAT);
    m_uiPeakLevel.Draw(dc, uiRect::BORDER_FLAT);
}

void FftMeter::DrawThirds(wxDC& dc)
{
    double dStep = static_cast<double>(m_rectGrid.GetWidth())/static_cast<double>(m_vThirdOctave.size());
    int x_old(0);
    int y_old(m_rectGrid.GetHeight());
    dc.SetPen(*wxWHITE_PEN);
    dc.SetBrush(wxBrush(wxColour(90,60,200)));

    dc.SetClippingRegion(m_rectGrid);

    for(size_t i = 0; i < m_vThirdOctave.size(); i++)
    {
        int x = m_rectGrid.GetLeft()+static_cast<int>((static_cast<double>(i)*dStep));
        float dLog;

        if(m_vThirdOctave[i] == 0)
        {
            dLog = -80;
        }
        else
        {
            dLog = 20*log10(m_vThirdOctave[i]);
        }
                //int y = m_rectGrid.GetHeight()+m_rectGrid.GetTop()-dLog;
        int y = -static_cast<int>(  (static_cast<double>(m_rectGrid.GetHeight()/70) * dLog));
        ////int y = -static_cast<int>(  (static_cast<double>(m_rectGrid.GetHeight()/100) * dLog));
   //     if(y < m_rectGrid.GetHeight())
        {
            switch(m_nDisplayType)
            {
                case DISPLAY_GRAPH:
                    dc.DrawLine(x_old, y_old, x, y);
                    x_old = x;
                    y_old = y;
                    break;
                case DISPLAY_LINES:
                    dc.DrawLine(x, m_rectGrid.GetHeight(), x, y);
                    break;
                case DISPLAY_EQ:
                    if(x_old+10 <= x)
                    {
                        dc.DrawRectangle(x-5, y, 10, m_rectGrid.GetHeight()-y);
                        x_old = x;
                        y_old = y;
                    }
                    break;
            }
        }
    }
    dc.DestroyClippingRegion();
}

void FftMeter::DrawFFT(wxDC& dc)
{
    dc.SetTextForeground(GetForegroundColour());

    uiRect uiLabel;
    wxPen penLine(wxColour(120,120,120),1);
    for(size_t i = 1; i < m_vfft_out.size()-1; i*= 2)
    {
        dc.SetPen(penLine);
        int x = static_cast<int>( (static_cast<double>(m_rectGrid.GetWidth())/(log(m_vfft_out.size()))) * log(static_cast<double>(i))) + m_rectGrid.GetLeft();
        dc.DrawLine(x, 0, x, m_rectGrid.GetHeight());
        uiLabel.SetRect(wxRect(x-20, m_rectGrid.GetBottom()+1, 40, 20));
        uiLabel.Draw(dc, wxString::Format(wxT("%.0f"), m_dBinSize*static_cast<double>(i)), uiRect::BORDER_NONE);

    }




    dc.SetPen(*wxWHITE_PEN);
    dc.SetBrush(wxBrush(wxColour(90,60,200)));

    //draw the 1025 FFT points
    int x_old(m_rectGrid.GetLeft());
    int y_old(m_rectGrid.GetHeight());
    for(size_t i = 1; i < m_vAmplitude.size(); i++)
    {
        int x = static_cast<int>( (static_cast<double>(m_rectGrid.GetWidth())/log(m_vAmplitude.size())) * static_cast<double>(log(i)))+m_rectGrid.GetLeft();

        int y = -static_cast<int>(  (static_cast<double>(m_rectGrid.GetHeight()/70) * m_vAmplitude[i]));
        if(i == 1)
        {
            y_old = min(y, m_rectGrid.GetBottom());
        }

        if(m_bColour)
        {
            float r,g,b;
            m_HeatMap.getColourAtValue(static_cast<float>(i)/static_cast<float>(m_vAmplitude.size()), r,g,b);
            dc.SetPen(wxPen(wxColour( static_cast<unsigned char>(r*255.0),
                                        static_cast<unsigned char>(g*255.0),
                                      static_cast<unsigned char>(b*255.0))));
        }

        switch(m_nDisplayType)
        {
            case DISPLAY_GRAPH:
                dc.DrawLine(x_old, min(y_old, m_rectGrid.GetBottom()), x, min(y, m_rectGrid.GetBottom()));
                x_old = x;
                y_old = y;
                break;
            case DISPLAY_LINES:
                if(y < m_rectGrid.GetHeight())
                {
                   // if(x_old != x)
                    {
                        dc.DrawLine(x, m_rectGrid.GetHeight(), x, y);
                    }
                }
                x_old = x;
                y_old = y;
                break;
            case DISPLAY_EQ:
                if(x_old+10 <= x && y < m_rectGrid.GetHeight())
                {
                    dc.DrawRectangle(x-5, y, 10, m_rectGrid.GetHeight()-y);
                    x_old = x;
                    y_old = y;
                }
                break;
        }
    }
    if(m_bCursorMode)
    {
        int x = static_cast<int>( (static_cast<double>(m_rectGrid.GetWidth())/log(m_vAmplitude.size())) * static_cast<double>(log(m_nBinSelected)))+m_rectGrid.GetLeft();
        dc.SetPen(wxPen(wxColour(255,100,00), 1));
        dc.DrawLine(x, m_rectGrid.GetTop(), x, m_rectGrid.GetBottom());

        m_uiClose.Draw(dc, wxT("Exit"), uiRect::BORDER_UP);
        m_uiAmplitude.Draw(dc, wxString::Format(wxT("%.2f dB"), m_vAmplitude[m_nBinSelected]), uiRect::BORDER_NONE);
        m_uiBin.Draw(dc, wxString::Format(wxT("%.0f Hz"), m_dBinSize*static_cast<double>(m_nBinSelected)), uiRect::BORDER_NONE);
        m_uiNudgeDown.Draw(dc,wxT("-"), (m_nNudge==DOWN)? uiRect::BORDER_DOWN : uiRect::BORDER_UP);;

        m_uiNudgeUp.Draw(dc,wxT("+"), (m_nNudge==UP)? uiRect::BORDER_DOWN : uiRect::BORDER_UP);

    }

}


void FftMeter::OnSize(wxSizeEvent& event)
{
    m_rectGrid = wxRect(GetClientRect().GetLeft()+40, GetClientRect().GetTop(), GetClientRect().GetWidth()-40, GetClientRect().GetHeight()-50);

    m_uiSettingsDisplay.SetRect(m_rectGrid.GetLeft(), GetClientRect().GetBottom()-23, 80,20);
    m_uiSettingsMeter.SetRect(m_uiSettingsDisplay.GetRight()+5, GetClientRect().GetBottom()-23, 80,20);
    m_uiSettingsAnalyse.SetRect(m_uiSettingsMeter.GetRight()+5, GetClientRect().GetBottom()-23, 80,20);

    m_uiSettingsWindow.SetRect(m_uiSettingsAnalyse.GetRight()+10, GetClientRect().GetBottom()-23, 80,20);
    m_uiSettingsOverlap.SetRect(m_uiSettingsWindow.GetRight()+5, GetClientRect().GetBottom()-23, 120,20);
    m_uiSettingsBins.SetRect(m_uiSettingsOverlap.GetRight()+5, GetClientRect().GetBottom()-23, 120,20);

    m_uiPeakFrequency.SetRect(m_uiSettingsBins.GetRight()+15, GetClientRect().GetBottom()-23, 80,20);
    m_uiPeakLevel.SetRect(m_uiPeakFrequency.GetRight()+3, GetClientRect().GetBottom()-23, 80,20);

    m_uiClose.SetRect(GetClientRect().GetRight()-85, GetClientRect().GetTop()+5, 80, 50);
    m_uiClose.SetBackgroundColour(*wxRED);

    m_uiAmplitude.SetRect(m_uiClose.GetLeft()-120, m_uiClose.GetTop(), 100, m_uiClose.GetHeight());

    m_uiBin.SetRect(m_uiAmplitude.GetLeft()-180, m_uiAmplitude.GetTop(), 100, m_uiAmplitude.GetHeight());
    m_uiNudgeUp.SetRect(m_uiBin.GetRight()+5, m_uiAmplitude.GetTop(), 50, m_uiAmplitude.GetHeight());
    m_uiNudgeDown.SetRect(m_uiBin.GetLeft()-55, m_uiAmplitude.GetTop(), 50, m_uiAmplitude.GetHeight());

    m_uiNudgeDown.SetBackgroundColour(wxColour(100,100,160));
    m_uiNudgeUp.SetBackgroundColour(wxColour(100,100,160));


    Refresh();

}


void FftMeter::SetFreqMag(const freq_mag& fm)
{


}

void FftMeter::SetSampleRate(unsigned long nSampleRate)
{
    m_nSampleRate = nSampleRate;
}

void FftMeter::SetNumberOfChannels(unsigned int nChannels)
{
    m_nChannels = nChannels;
}

void FftMeter::SetData(const timedbuffer* pBuffer)
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

void FftMeter::DoFFT()
{

    switch(m_nMeterType)
    {
        case FFT:
            FFTRoutine();
            break;
        case OCTAVE:
            Octave();
            break;
        case PEAKS:
            Peaks();
            break;
    }
    Refresh();
}

void FftMeter::FFTRoutine()
{
    FFTAlgorithm fft;
    m_vfft_out = fft.DoFFT(m_lstBuffer, m_nSampleRate, m_nChannels, m_nFFTAnalyse, m_nWindowType, m_vfft_out.size(), m_nOverlap);
    m_dBinSize = static_cast<double>(m_nSampleRate)/static_cast<double>((m_vfft_out.size()-1)*2);
    float dMax(-80);
    double dMaxBin(0);
    vector<double> vAmp;
    vAmp.resize(m_vAmplitude.size());

    for(size_t i = 0; i < m_vfft_out.size(); i++)
    {
        float dAmplitude(sqrt( (m_vfft_out[i].r*m_vfft_out[i].r) + (m_vfft_out[i].i*m_vfft_out[i].i)));
        if(dAmplitude<0)
        {
            dAmplitude=-dAmplitude;
        }
        dAmplitude /= static_cast<float>(m_vfft_out.size());
        vAmp[i] = dAmplitude;
        double dLog = WindowMod(20*log10(dAmplitude));
        dLog = max(dLog, -80.0);

        m_vAmplitude[i] = min(0.0, max((double)m_vAmplitude[i]-m_dFall, (double)dLog));
        if(dMax < m_vAmplitude[i])
        {
            dMax = m_vAmplitude[i];
            dMaxBin = i;
        }
    }
    double dQ = (vAmp[dMaxBin+1]-vAmp[dMaxBin-1])/(2*(2*vAmp[dMaxBin]-vAmp[dMaxBin-1]-vAmp[dMaxBin+1]));
    m_uiPeakLevel.SetLabel(wxString::Format(wxT("%.1f"),dMax));
    m_uiPeakFrequency.SetLabel(wxString::Format(wxT("%.0fHz"),(dQ+dMaxBin)*m_dBinSize));
}

float FftMeter::WindowMod(float dAmplitude)
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

void FftMeter::Octave()
{
    FFTAlgorithm fft;

    m_vfft_out = fft.DoFFT(m_lstBuffer, m_nSampleRate, m_nChannels, m_nFFTAnalyse, m_nWindowType, m_vfft_out.size(), m_nOverlap);
    m_dBinSize = static_cast<double>(m_nSampleRate)/static_cast<double>((m_vfft_out.size()-1)*2);

    //work out the third octave bands
    double dBins[3] = {3,4,5};
    int nBin(1);
    size_t n = 0;
    while(n < m_vThirdOctave.size())
    {
        for(size_t i = 0; i < 3 && n<m_vThirdOctave.size(); i++)
        {
            double dEnergy = 0.0;
            for(int j = 0; j < dBins[i]; j++)
            {
                float dAmplitude(fabs(sqrt( (m_vfft_out[nBin].r*m_vfft_out[nBin].r) + (m_vfft_out[nBin].i*m_vfft_out[nBin].i))));
                dAmplitude /= static_cast<float>(m_vfft_out.size());
                dEnergy += pow(dAmplitude,2);
                nBin++;
            }
            //m_vThirdOctave[n] = sqrt(dEnergy/dBins[i]);
            m_vThirdOctave[n] = sqrt(dEnergy);///dBins[i]);


            dBins[i]*=2;
            n++;
        }
    }
}

void FftMeter::Peaks()
{
    FFTAlgorithm fft;
    map<int, double> mPeaks(fft.GetPeaks(m_lstBuffer, m_nSampleRate, m_nChannels, m_nFFTAnalyse, m_nWindowType, m_vfft_out.size(), m_nOverlap));

    for(size_t i = 0; i < m_vAmplitude.size(); i++)
    {
        map<int,double>::iterator itPeak = mPeaks.find(i);
        if(itPeak == mPeaks.end())
        {
            m_vAmplitude[i] = -80;
        }
        else
        {
            m_vAmplitude[i] = WindowMod(itPeak->second);
        }
    }
    m_uiPeakLevel.SetLabel(wxString::Format(wxT("%d Peaks"),mPeaks.size()));
    m_uiPeakFrequency.SetLabel(wxEmptyString);
}

void FftMeter::SetAnalyseMode(int nMode)
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

void FftMeter::SetWindowType(int nType)
{
    m_lstBuffer.clear();
    m_nWindowType = nType;
    m_uiSettingsWindow.SetLabel(LABEL_WINDOW[nType]);
    RefreshRect(m_uiSettingsWindow.GetRect());
}

void FftMeter::SetOverlap(double dPercent)
{
    m_dOverlapPercent = max(0.0, min(100.0, dPercent));
    m_nOverlap = m_nSampleSize - static_cast<size_t>(m_dOverlapPercent * static_cast<double>(m_nSampleSize)/100);

    m_dFall = 0.000195 * static_cast<double>(m_nNumberOfSetBins)*(100-m_dOverlapPercent)/100.0;

    m_uiSettingsOverlap.SetLabel(wxString::Format(wxT("Overlap: %.2f%"), m_dOverlapPercent));
    RefreshRect(m_uiSettingsOverlap.GetRect());

}


void FftMeter::SetNumberOfBins(size_t nBins)
{
    m_nSampleSize = nBins*2;
    m_vfft_out.resize(nBins+1);
    m_vAmplitude = vector<float>(m_vfft_out.size(), -80.0);

    m_dFall = 0.000195 * static_cast<double>(nBins)*(100-m_dOverlapPercent)/100.0;
    //store here so we can get back
    if(m_nMeterType != OCTAVE)
    {
        m_nNumberOfSetBins = nBins;
    }

    SetOverlap(m_dOverlapPercent);
    m_uiSettingsBins.SetLabel(wxString::Format(wxT("Res: %.1f Hz"), 24000.0/static_cast<double>(nBins)));
    RefreshRect(m_uiSettingsBins.GetRect());

}


void FftMeter::SetDisplayType(unsigned int nDisplay)
{
    m_nDisplayType = nDisplay%3;
    m_uiSettingsDisplay.SetLabel(LABEL_DISPLAY[m_nDisplayType]);
    Refresh();
}

void FftMeter::SetMeter(short nMeter)
{
    //if we were doing octaves and now aren't reset the number of bins to what we last set
    if(m_nMeterType == OCTAVE && nMeter != OCTAVE)
    {
        SetNumberOfBins(m_nNumberOfSetBins);
    }
    m_nMeterType = nMeter;
    if(m_nMeterType == OCTAVE)
    {
        SetNumberOfBins(38912);
    }

    m_uiSettingsMeter.SetLabel(LABEL_TYPE[nMeter]);
    Refresh();
}


void FftMeter::SetHold(bool bHold)
{
    m_bHold = bHold;
}


void FftMeter::SetCursorMode(bool bSet)
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

void FftMeter::OnLeftDown(wxMouseEvent& event)
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
        else if(event.GetPosition().y > m_uiNudgeDown.GetBottom())
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
void FftMeter::OnTimerNudge(wxTimerEvent& event)
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


void FftMeter::TurnoffNudge()
{
    m_timerNudge.Stop();
    m_nNudge = NONE;
    RefreshRect(m_uiNudgeDown.GetRect());
    RefreshRect(m_uiNudgeUp.GetRect());
}


void FftMeter::OnLeftUp(wxMouseEvent& event)
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
            m_pBuilder->OnCursorMode(m_bCursorMode);
        }
        else
        {
            TurnoffNudge();
        }
    }

}
