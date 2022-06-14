#include "fftdiffmeter.h"
#include <wx/dc.h>
#include <wx/dcbuffer.h>
#include <wx/log.h>
#include "kaiserwindow.h"
#include "uirect.h"
#include "fftAlgorithm.h"
#include "timedbuffer.h"
#include "fftdiffbuilder.h"
//#include "settings.h"
#include "log.h"
#include "delayline.h"

using namespace std;

 wxIMPLEMENT_DYNAMIC_CLASS(fftdiffMeter, pmControl);

BEGIN_EVENT_TABLE(fftdiffMeter, pmControl)
    EVT_PAINT(fftdiffMeter::OnPaint)
    EVT_SIZE(fftdiffMeter::OnSize)
    EVT_LEFT_DOWN(fftdiffMeter::OnLeftDown)
    EVT_LEFT_UP(fftdiffMeter::OnLeftUp)
END_EVENT_TABLE()



const wxString fftdiffMeter::LABEL_WINDOW[6] = {wxT("None"), wxT("Hann"), wxT("Hamming"), wxT("Blackman"), wxT("Kaiser"), wxT("KaiserBessel")};
const wxColour fftdiffMeter::COLOUR_LABEL = wxColour(65,92,192);
const double fftdiffMeter::OCTAVE_LOW_1 = 8.77;
const double fftdiffMeter::OCTAVE_LOW_2 = 11.05;
const double fftdiffMeter::OCTAVE_LOW_3 = 13.9;


fftdiffMeter::fftdiffMeter(wxWindow *parent, fftdiffBuilder* pBuilder, wxWindowID id, const wxPoint& pos, const wxSize& size) : pmControl(),
    m_pBuilder(pBuilder),
    m_dVerticalResolution(100.0),
    m_nDelayMode(0)
{
    Create(parent, id, pos, size);
    m_nPeakCutoff = -50;
    SetWindowType(WINDOW_BLACKMAN);
    SetOverlap(50.0);
    m_dFall = 0.2;
    m_nBinSelected = 0;
    m_bCursorMode = false;
    m_bShowMax = false;
    m_bShowMin = false;
    m_bShowAverage = true;

    m_nSelectedChannels[0] = 0;
    m_nSelectedChannels[1] = 1;

    m_vChannels.push_back(subsession::channelGrouping(0,subsession::enumChannelGrouping::ST, subsession::enumChannel::LEFT));
    m_vChannels.push_back(subsession::channelGrouping(0,subsession::enumChannelGrouping::ST, subsession::enumChannel::RIGHT));

    m_nSampleRate = 48000;

    m_nNudge = NONE;
    m_bColour = (pBuilder->ReadSetting(wxT("Colour"), 0)==1);

    m_HeatMap.createDefaultHeatMapGradient();

    SetNumberOfBins(1024);

    m_pBuilder->RegisterForSettingsUpdates(this);
    Bind(wxEVT_SETTING_CHANGED, &fftdiffMeter::OnSettingChanged, this);
}

bool fftdiffMeter::Create(wxWindow *parent, wxWindowID id, const wxPoint& pos, const wxSize& size)
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
    Connect(wxID_ANY, wxEVT_TIMER, (wxObjectEventFunction)&fftdiffMeter::OnTimerNudge);

    SetFont(wxFont(8,wxFONTFAMILY_SWISS,wxFONTSTYLE_NORMAL,wxFONTWEIGHT_NORMAL,false,_T("Arial"),wxFONTENCODING_DEFAULT));

    return true;
}

fftdiffMeter::~fftdiffMeter()
{

}


void fftdiffMeter::OnPaint(wxPaintEvent& event)
{
    wxAutoBufferedPaintDC dc(this);
    wxBrush br(*wxBLACK);
    dc.SetBrush(br);
    dc.DrawRectangle(GetClientRect());


    //draw horizonal lines
    uiRect uiLevel;
    for(double i = 0; i < m_dVerticalResolution; i += 10)
    {
        int nDB = (static_cast<double>(m_rectGrid.GetHeight()/m_dVerticalResolution) * i);

        uiLevel.SetRect(5, nDB-10, m_rectGrid.GetLeft()-5, 20);
        uiLevel.SetLabel(wxString::Format(wxT("%.0f"),-(i-m_dVerticalResolution/2)));
        uiLevel.Draw(dc,uiRect::BORDER_NONE);
        dc.SetPen(wxPen(wxColour(100,100,100),1 ));
        dc.DrawLine(m_rectGrid.GetLeft(), nDB, m_rectGrid.GetWidth()+m_rectGrid.GetLeft(), nDB);

    }

    DrawFFT(dc);

    m_uiSettingsAnalyse.Draw(dc, uiRect::BORDER_DOWN);
    m_uiSettingsWindow.Draw(dc, uiRect::BORDER_FLAT);
    m_uiSettingsOverlap.Draw(dc, uiRect::BORDER_FLAT);
    m_uiSettingsBins.Draw(dc, uiRect::BORDER_FLAT);
    m_uiSettingsDisplay.Draw(dc, uiRect::BORDER_FLAT);
    m_uiSettingsMeter.Draw(dc, uiRect::BORDER_FLAT);
    m_uiPeakFrequency.Draw(dc, uiRect::BORDER_FLAT);
    m_uiPeakLevel.Draw(dc, uiRect::BORDER_FLAT);
}

void fftdiffMeter::DrawFFT(wxDC& dc)
{
    dc.SetTextForeground(GetForegroundColour());

    uiRect uiLabel;
    wxPen penLine(wxColour(120,120,120),1);
    for(size_t i = 1; i < m_vfft_out[0].size()-1; i*= 2)
    {
        dc.SetPen(penLine);
        int x = static_cast<int>( (static_cast<double>(m_rectGrid.GetWidth())/(log(m_vfft_out[0].size()))) * log(static_cast<double>(i))) + m_rectGrid.GetLeft();
        dc.DrawLine(x, 0, x, m_rectGrid.GetHeight());
        uiLabel.SetRect(wxRect(x-20, m_rectGrid.GetBottom()+1, 40, 20));
        uiLabel.Draw(dc, wxString::Format(wxT("%.0f"), m_dBinSize*static_cast<double>(i)), uiRect::BORDER_NONE);

    }


    //dc.SetPen(*wxWHITE_PEN);
    //dc.SetBrush(wxBrush(wxColour(90,60,200)));
    DrawGraph(dc, m_vAmplitude, wxColour(140,140,140));

    if(m_bShowAverage)
    {
        DrawGraph(dc, m_vAverage, *wxWHITE);
    }

    if(m_bShowMax)
    {
        DrawGraph(dc, m_vMax, *wxBLUE);
    }
    if(m_bShowMin)
    {
        DrawGraph(dc, m_vMin, *wxBLUE);
    }
   // DrawGraph(dc, m_vSD, *wxBLUE, -3.0);
//


    if(m_bCursorMode)
    {
        int x = static_cast<int>( (static_cast<double>(m_rectGrid.GetWidth())/log(m_vAmplitude.size())) * static_cast<double>(log(m_nBinSelected)))+m_rectGrid.GetLeft();
        dc.SetPen(wxPen(wxColour(255,100,00), 1));
        dc.DrawLine(x, m_rectGrid.GetTop(), x, m_rectGrid.GetBottom());

        m_uiClose.Draw(dc, wxT("Exit"), uiRect::BORDER_UP);
        m_uiAmplitude.Draw(dc, wxString::Format(wxT("%.2f dB"), m_vAmplitude[m_nBinSelected]), uiRect::BORDER_NONE);
        m_uiAverage.Draw(dc, wxString::Format(wxT("%.2f dB"), m_vAverage[m_nBinSelected]), uiRect::BORDER_NONE);
        m_uiBin.Draw(dc, wxString::Format(wxT("%.0f Hz"), m_dBinSize*static_cast<double>(m_nBinSelected)), uiRect::BORDER_NONE);
        m_uiNudgeDown.Draw(dc,wxT("-"), (m_nNudge==DOWN)? uiRect::BORDER_DOWN : uiRect::BORDER_UP);;

        m_uiNudgeUp.Draw(dc,wxT("+"), (m_nNudge==UP)? uiRect::BORDER_DOWN : uiRect::BORDER_UP);

    }

}

void fftdiffMeter::DrawGraph(wxDC& dc, const std::vector<float>& vSpectrum, const wxColour& clr, double dMultiplier)
{
    //draw the 1025 FFT points
    int x_old(m_rectGrid.GetLeft());
    int y_old(m_rectGrid.GetHeight());

    for(size_t i = 1; i < vSpectrum.size(); i++)
    {
        int x = static_cast<int>( (static_cast<double>(m_rectGrid.GetWidth())/log(vSpectrum.size())) * static_cast<double>(log(i)))+m_rectGrid.GetLeft();
        int y = -static_cast<int>(  (static_cast<double>(m_rectGrid.GetHeight()/m_dVerticalResolution) * vSpectrum[i]*dMultiplier));
        y += m_rectGrid.GetHeight()/2;
        if(i == 1)
        {
            y_old = min(y, m_rectGrid.GetHeight()/2);
        }

        dc.SetPen(clr);

        dc.DrawLine(x_old, min(y_old, m_rectGrid.GetBottom()), x, min(y, m_rectGrid.GetBottom()));
        x_old = x;
        y_old = y;

    }
}

void fftdiffMeter::OnSize(wxSizeEvent& event)
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
    m_uiAmplitude.SetBackgroundColour(*wxWHITE);
    m_uiAmplitude.SetForegroundColour(*wxBLACK);

    m_uiAverage.SetRect(m_uiAmplitude.GetLeft()-105, m_uiClose.GetTop(), 100, m_uiClose.GetHeight());
    m_uiAverage.SetBackgroundColour(*wxWHITE);
    m_uiAverage.SetForegroundColour(*wxBLACK);

    m_uiBin.SetRect(m_uiAverage.GetLeft()-180, m_uiAmplitude.GetTop(), 100, m_uiAmplitude.GetHeight());
    m_uiNudgeUp.SetRect(m_uiBin.GetRight()+5, m_uiAmplitude.GetTop(), 50, m_uiAmplitude.GetHeight());
    m_uiNudgeDown.SetRect(m_uiBin.GetLeft()-55, m_uiAmplitude.GetTop(), 50, m_uiAmplitude.GetHeight());

    m_uiNudgeDown.SetBackgroundColour(wxColour(100,100,160));
    m_uiNudgeUp.SetBackgroundColour(wxColour(100,100,160));


    Refresh();

}


void fftdiffMeter::SetFreqMag(const freq_mag& fm)
{


}

void fftdiffMeter::SetSampleRate(unsigned long nSampleRate)
{
    m_nSampleRate = nSampleRate;
}

void fftdiffMeter::SetChannels(const std::vector<subsession::channelGrouping>& vChannels)
{
    m_vChannels = vChannels;
}

void fftdiffMeter::SetAudioData(const timedbuffer* pBuffer)
{
    if(!m_bHold &&  m_vChannels.size() != 0)
    {
        nonInterlacedVector data;
        data.first.reserve((pBuffer->GetBufferSize()/m_vChannels.size()));
        data.second.reserve((pBuffer->GetBufferSize()/m_vChannels.size()));

        for(size_t i = 0; i < pBuffer->GetBufferSize(); i+=m_vChannels.size())
        {
            data.first.push_back(pBuffer->GetBuffer()[i+m_nSelectedChannels[0]]);
            data.second.push_back(pBuffer->GetBuffer()[i+m_nSelectedChannels[1]]);

        }

        if(m_nDelayMode != DELAY_OFF)
        {
            m_nOffset = m_delayLine.ProcessAudio(data);

            m_uiSettingsDisplay.SetLabel(wxString::Format("%.2fms", static_cast<double>(m_nOffset*1000)/static_cast<double>(m_nSampleRate)));
        }

        //copy the data to the fft buffer
        std::copy(data.first.begin(), data.first.end(), std::back_inserter(m_buffer.first));
        std::copy(data.second.begin(), data.second.end(), std::back_inserter(m_buffer.second));


        while(m_buffer.first.size() > (m_vfft_out[0].size()-1))
        {
            DoFFT();
        }
    }
}

void fftdiffMeter::DoFFT()
{
    FFTRoutine();
    Refresh();

    if(m_pBuilder->WebsocketsActive())
    {
       m_pBuilder->SendWebsocketMessage(CreateWebsocketMessage());
    }
}

void fftdiffMeter::FFTRoutine()
{
    FFTAlgorithm fft;
    m_vfft_out[0] = fft.DoFFT(m_buffer.first, m_nSampleRate, 1, 0, m_nWindowType, m_vfft_out[0].size(), m_nOverlap);
    m_vfft_out[1] = fft.DoFFT(m_buffer.second, m_nSampleRate, 1, 0, m_nWindowType, m_vfft_out[1].size(), m_nOverlap);

    m_dBinSize = static_cast<double>(m_nSampleRate)/static_cast<double>((m_vfft_out[0].size()-1)*2);
    m_dPeakLevel = -80;
    double dMaxBin(0);

    for(size_t i = 0; i < m_vfft_out[0].size(); i++)
    {
        auto dAmplitudeA = abs(sqrt( (m_vfft_out[0][i].r*m_vfft_out[0][i].r) + (m_vfft_out[0][i].i*m_vfft_out[0][i].i)))/static_cast<float>(m_vfft_out[0].size());
        auto dAmplitudeB = abs(sqrt( (m_vfft_out[1][i].r*m_vfft_out[1][i].r) + (m_vfft_out[1][i].i*m_vfft_out[1][i].i)))/static_cast<float>(m_vfft_out[1].size());

        float dLogA = WindowMod(20*log10(dAmplitudeA));
        float dLogB = WindowMod(20*log10(dAmplitudeB));
        float dLog = -dLogA+dLogB;

        m_vAmplitude[i] = dLog;

        auto oldAverage = m_vAverage[i];
        m_vAverage[i] = oldAverage + ((dLog-oldAverage)/(m_dTotalFrames+1));
        m_vMax[i] = std::max(dLog, m_vMax[i]);
        m_vMin[i] = std::min(dLog, m_vMin[i]);
    }

    m_dTotalFrames++;
}

float fftdiffMeter::WindowMod(float dAmplitude)
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


void fftdiffMeter::SetChannels(int nA, int nB)
{
    m_nSelectedChannels[0] = nA;
    m_nSelectedChannels[1] = nB;


}

void fftdiffMeter::SetWindowType(int nType)
{
    m_buffer.first.clear();
    m_buffer.second.clear();
    m_nWindowType = nType;
    m_uiSettingsWindow.SetLabel(LABEL_WINDOW[nType]);
    RefreshRect(m_uiSettingsWindow.GetRect());
}

void fftdiffMeter::SetOverlap(double dPercent)
{
    m_dOverlapPercent = max(0.0, min(100.0, dPercent));
    m_nOverlap = m_nSampleSize - static_cast<size_t>(m_dOverlapPercent * static_cast<double>(m_nSampleSize)/100);

    m_dFall = 0.000195 * static_cast<double>(m_nNumberOfSetBins)*(100-m_dOverlapPercent)/100.0;

    m_uiSettingsOverlap.SetLabel(wxString::Format(wxT("Overlap: %.2f%"), m_dOverlapPercent));
    RefreshRect(m_uiSettingsOverlap.GetRect());

}


void fftdiffMeter::SetNumberOfBins(size_t nBins)
{
    m_nSampleSize = nBins*2;
    m_vfft_out[0].resize(nBins+1);
    m_vfft_out[1].resize(nBins+1);
    m_vAmplitude = vector<float>(m_vfft_out[0].size(), 0.0);
    m_vAverage = vector<float>(m_vfft_out[0].size(), 0.0);
    m_vMax = vector<float>(m_vfft_out[0].size(), -80.0);
    m_vMin = vector<float>(m_vfft_out[0].size(), 80.0);
    m_dTotalFrames = 0.0;

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


void fftdiffMeter::SetHold(bool bHold)
{
    m_bHold = bHold;
}


void fftdiffMeter::SetCursorMode(bool bSet)
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

void fftdiffMeter::OnLeftDown(wxMouseEvent& event)
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
            m_nBinSelected = min(m_nBinSelected+1, m_vfft_out[0].size()-1);
            m_timerNudge.Start(100);
            m_nNudge = UP;
        }
        else if(event.GetPosition().y > m_uiNudgeDown.GetBottom())
        {

            //convert x to BIN
            double dBin = static_cast<double>(event.GetPosition().x-m_rectGrid.GetLeft());
            dBin = exp(dBin * log(m_vfft_out[0].size()) / static_cast<double>(m_rectGrid.GetWidth()));

            if(dBin > 0 && dBin < m_vfft_out[0].size())
            {
                m_nBinSelected = dBin;
                Refresh();
            }
        }
    }

}
void fftdiffMeter::OnTimerNudge(wxTimerEvent& event)
{
    if(m_nNudge == UP)
    {
        m_nBinSelected = min(m_nBinSelected+1, m_vfft_out[0].size()-1);
    }
    else if(m_nNudge == DOWN)
    {
        m_nBinSelected = max(m_nBinSelected-1, (size_t)1);
    }
    Refresh();
}


void fftdiffMeter::TurnoffNudge()
{
    m_timerNudge.Stop();
    m_nNudge = NONE;
    RefreshRect(m_uiNudgeDown.GetRect());
    RefreshRect(m_uiNudgeUp.GetRect());
}


void fftdiffMeter::OnLeftUp(wxMouseEvent& event)
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
            m_pBuilder->WriteSetting("Cursor", false);
        }
        else
        {
            TurnoffNudge();
        }
    }

}

void fftdiffMeter::ShowMax(bool bShow)
{
    m_bShowMax = bShow;
}

void fftdiffMeter::ShowMin(bool bShow)
{
    m_bShowMin = bShow;
}

void fftdiffMeter::ShowAverage(bool bShow)
{
    m_bShowAverage = bShow;
}


void fftdiffMeter::ResetMax()
{
    m_vMax = vector<float>(m_vfft_out[0].size(), -80.0);

}

void fftdiffMeter::ResetMin()
{
    m_vMin = vector<float>(m_vfft_out[0].size(), 80.0);

}

void fftdiffMeter::ResetAverage()
{
    m_vAverage = vector<float>(m_vfft_out[0].size(), 0.0);
    m_dTotalFrames = 0.0;
}

Json::Value fftdiffMeter::CreateWebsocketMessage()
{
    Json::Value jsData;

    jsData["bins"] = Json::Value(Json::arrayValue);

    return jsData;
}

void fftdiffMeter::SetDelayMode(long nMode)
{
    m_nDelayMode = nMode;
    switch(m_nDelayMode)
    {
        case DELAY_ONE:
            m_delayLine.SetCalculationMode(true);
            break;
        case DELAY_AUTO:
            m_delayLine.SetCalculationMode(false);
            break;
    }
}

void fftdiffMeter::SetVerticalRange(unsigned long ndB)
{
    m_dVerticalResolution = std::min(80.0, static_cast<double>(ndB));
}

void fftdiffMeter::SetDelayWindow(unsigned long nWindow)
{
    m_delayLine.SetWindowSize(nWindow*m_nSampleRate/500);
}

void fftdiffMeter::SetDelayAccuracy(unsigned long nAccuracy)
{
    m_delayLine.SetAccuracy(nAccuracy);
}


void fftdiffMeter::OnSettingChanged(SettingEvent& event)
{
    if(event.GetKey() == "max")
    {
        ShowMax(event.GetValue(false));
    }
    else if(event.GetKey() == "min")
    {
        ShowMin(event.GetValue(false));
    }
    else if(event.GetKey() == "average")
    {
        ShowAverage(event.GetValue(false));
    }
    else if(event.GetKey() == "Bins")
    {
        SetNumberOfBins(event.GetValue(long(0)));
    }
    else if(event.GetKey() == "Window")
    {
        SetWindowType(event.GetValue(long(0)));
    }
    else if(event.GetKey() == "Overlap")
    {
        SetOverlap(event.GetValue(0.0));
    }
    else if(event.GetKey() == "Hold")
    {
        SetHold(event.GetValue(long(0)));
    }
    else if(event.GetKey() == "Cursor")
    {
        SetCursorMode(event.GetValue(long(0)));
    }
    else if(event.GetKey() == "Colour")
    {
        SetColourMode(event.GetValue(long(0)));
    }
    else if(event.GetKey() == "DelayMode")
    {
        SetDelayMode(event.GetValue(long(0)));
    }
    else if(event.GetKey() == "DelayWindow")
    {
        SetDelayWindow(event.GetValue(long(0)));
    }
    else if(event.GetKey() == "DelayAccuracy")
    {
        SetDelayAccuracy(event.GetValue(long(0)));
    }
    else if(event.GetKey() == "Range")
    {
        SetVerticalRange(event.GetValue(long(0)));
    }
}