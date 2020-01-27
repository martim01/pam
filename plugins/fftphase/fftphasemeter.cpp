#include "fftphasemeter.h"
#include <wx/dc.h>
#include <wx/dcbuffer.h>
#include <wx/log.h>
#include "uirect.h"
#include "fftAlgorithm.h"
#include "fftphasebuilder.h"
#include "settings.h"
#include "timedbuffer.h"


using namespace std;

BEGIN_EVENT_TABLE(fftphaseMeter, pmControl)
    EVT_PAINT(fftphaseMeter::OnPaint)
    EVT_SIZE(fftphaseMeter::OnSize)
    EVT_LEFT_UP(fftphaseMeter::OnLeftUp)
END_EVENT_TABLE()

wxIMPLEMENT_DYNAMIC_CLASS(fftphaseMeter, pmControl);

const wxString fftphaseMeter::LABEL_WINDOW[6] = {wxT("None"), wxT("Hann"), wxT("Hamming"), wxT("Blackman"), wxT("Kaiser"), wxT("KaiserBessel")};


fftphaseMeter::fftphaseMeter(wxWindow *parent, fftphaseBuilder* pBuilder, wxWindowID id, const wxPoint& pos, const wxSize& size) : pmControl(),
    m_pBuilder(pBuilder),
    m_nSampleRate(48000),
    m_nChannels(2),
    m_nWindowType(WINDOW_KAISER),
    m_bFall(false)
{
    SetOverlap(50.0);
    SetNumberOfBins(1024);
    m_HeatMap.createDefaultHeatMapGradient();
    Create(parent, id, pos, size);

}

bool fftphaseMeter::Create(wxWindow *parent, wxWindowID id, const wxPoint& pos, const wxSize& size)
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


    return true;
}

fftphaseMeter::~fftphaseMeter()
{

}


void fftphaseMeter::OnPaint(wxPaintEvent& event)
{
    wxAutoBufferedPaintDC dc(this);
    wxBrush br(*wxBLACK);
    dc.SetBrush(br);
    dc.DrawRectangle(GetClientRect());

    dc.SetTextForeground(GetForegroundColour());

    switch(m_nDisplayType)
    {
        case STARS:
            DrawStars(dc);
            break;
        case PHASE:
            DrawPhase(dc);
            break;
        default:
            DrawGraph(dc);
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

void fftphaseMeter::DrawStars(wxDC& dc)
{
    double dX = 450.0/80.0;
    double dY = 450.0/80.0;

    dc.SetPen(wxPen(wxColour(150,150,150), 1));
    for(int i = 0; i <= 80; i+=10)
    {
        dc.DrawLine(20+(i*dX), 0, 20+(i*dX), 450);
        dc.DrawLine(20, (i*dY), 470, (i*dY));
    }


    int x_old = 20;
    int y_old = 450;
    for(size_t i = 1; i < m_vAmplitude[0].size(); i++)
    {
        int x = 20+(dX *(80.0+m_vAmplitude[0][i]));
        int y = 450-(dY *(80.0+m_vAmplitude[1][i]));
        float r,g,b;

    dc.SetPen(wxPen(m_vColour[i]));
       dc.DrawPoint(x,y);

    }
}

void fftphaseMeter::DrawPhase(wxDC& dc)
{
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

    dc.SetPen(*wxWHITE_PEN);
    dc.SetBrush(wxBrush(wxColour(90,60,200)));

    //draw the 1025 FFT points
    int x_old(m_rectGrid.GetLeft());
    int nCenter = m_rectGrid.GetHeight()/2+m_rectGrid.GetTop();
    int y_old(nCenter);
    double dRes = m_rectGrid.GetHeight()/(M_PI*4);

    int nLeftCenter = nCenter-(dRes*M_PI);
    int nRightCenter = nCenter+(dRes*M_PI);

    for(size_t i = 1; i < m_vPhase[0].size(); i++)
    {
        double dDiff(0.0);
        if(m_vPhase[0][i] < 0.0 && m_vPhase[1][i] > 0.0)
        {
            dDiff = -m_vPhase[1][i];
        }
        else if(m_vPhase[1][i] < 0.0 && m_vPhase[0][i] > 0.0)
        {
            dDiff = m_vPhase[0][i];
        }
        else if(m_vPhase[0][i] > 0.0 && m_vPhase[1][i] > 0.0)
        {
            dDiff = m_vPhase[0][i]-m_vPhase[1][i];
        }

        int x = static_cast<int>( (static_cast<double>(m_rectGrid.GetWidth())/log(m_vPhase[0].size())) * static_cast<double>(log(i)))+m_rectGrid.GetLeft();
        int y = nCenter-static_cast<int>(dRes * dDiff);

        dc.SetPen(wxPen(m_vColour[i]));
        dc.DrawLine(x, nCenter, x, y);

    }

/*
    x_old = m_rectGrid.GetLeft();
    y_old = nCenter;
    for(size_t i = 1; i < m_vPhase[1].size(); i++)
    {
        if(m_vPhase[1][i] >= 0.0)
        {
            int x = static_cast<int>( (static_cast<double>(m_rectGrid.GetWidth())/log(m_vPhase[1].size())) * static_cast<double>(log(i)))+m_rectGrid.GetLeft();
            int y = nCenter+static_cast<int>(  dRes * m_vPhase[1][i]);
            if(i == 1)
            {
                y_old = max(y, nCenter);
            }
            dc.SetPen(wxPen(m_vColour[i]));

            dc.DrawLine(x, nRightCenter, x, max(y, nCenter));
            x_old = x;
            y_old = y;
        }
    }
*/
}

void fftphaseMeter::DrawGraph(wxDC& dc)
{
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




    dc.SetPen(*wxWHITE_PEN);
    dc.SetBrush(wxBrush(wxColour(90,60,200)));

    //draw the 1025 FFT points
    int x_old(m_rectGrid.GetLeft());
    int nCenter = m_rectGrid.GetHeight()/2+m_rectGrid.GetTop();
    int y_old(nCenter);
    double dRes = m_rectGrid.GetHeight()/160.0;

    for(size_t i = 1; i < m_vAmplitude[0].size(); i++)
    {
        int x = static_cast<int>( (static_cast<double>(m_rectGrid.GetWidth())/log(m_vAmplitude[0].size())) * static_cast<double>(log(i)))+m_rectGrid.GetLeft();
        int y = -static_cast<int>(  (dRes * m_vAmplitude[0][i]));
        if(i == 1)
        {
            y_old = min(y, nCenter);
        }

        dc.SetPen(wxPen(m_vColour[i]));

        dc.DrawLine(x_old, min(y_old, nCenter), x, min(y, nCenter));
        x_old = x;
        y_old = y;
    }


    x_old = m_rectGrid.GetLeft();
    y_old = nCenter;
    for(size_t i = 1; i < m_vAmplitude[1].size(); i++)
    {
        int x = static_cast<int>( (static_cast<double>(m_rectGrid.GetWidth())/log(m_vAmplitude[1].size())) * static_cast<double>(log(i)))+m_rectGrid.GetLeft();
        int y = m_rectGrid.GetBottom()+static_cast<int>(  dRes * m_vAmplitude[1][i]);
        if(i == 1)
        {
            y_old = max(y, nCenter);
        }
        dc.SetPen(wxPen(m_vColour[i]));

        dc.DrawLine(x_old, max(y_old, nCenter), x, max(y, nCenter));
        x_old = x;
        y_old = y;
    }

}

void fftphaseMeter::OnSize(wxSizeEvent& event)
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


    m_uiAmplitude.SetRect(GetClientRect().GetRight()-85, GetClientRect().GetTop()+5, 80, 50);
    m_uiBin.SetRect(m_uiAmplitude.GetLeft()-180, m_uiAmplitude.GetTop(), 100, m_uiAmplitude.GetHeight());



    Refresh();

}

void fftphaseMeter::SetAudioData(const timedbuffer* pBuffer)
{
    if(m_nChannels != 0)
    {
        for(size_t i = 0; i < pBuffer->GetBufferSize(); i++)
        {
            m_lstBuffer[0].push_back(pBuffer->GetBuffer()[i]);
            m_lstBuffer[1].push_back(pBuffer->GetBuffer()[i]);
        }

        while(m_lstBuffer[0].size() > (m_vfft_out[0].size()-1)*2*m_nChannels)
        {
            DoFFT(0);
            DoFFT(1);
        }
        Refresh();
    }
}

void fftphaseMeter::DoFFT(int nChannel)
{
    FFTAlgorithm fft;

    m_vfft_out[nChannel] = fft.DoFFT(m_lstBuffer[nChannel], m_nSampleRate, m_nChannels, nChannel, m_nWindowType, m_vfft_out[nChannel].size(), m_nOverlap);

    m_dBinSize = static_cast<double>(m_nSampleRate)/static_cast<double>((m_vfft_out[0].size()-1)*2);

    float dMax(-80);
    double dMaxBin(0);
    vector<double> vAmp;
    vAmp.resize(m_vAmplitude[nChannel].size());

    for(size_t i = 0; i < m_vfft_out[nChannel].size(); i++)
    {
        float dAmplitude(sqrt( (m_vfft_out[nChannel][i].r*m_vfft_out[nChannel][i].r) + (m_vfft_out[nChannel][i].i*m_vfft_out[nChannel][i].i)));

        if(dAmplitude<0)
        {
            dAmplitude=-dAmplitude;
        }
        dAmplitude /= static_cast<float>(m_vfft_out[nChannel].size());
        vAmp[i] = dAmplitude;
        double dLog = WindowMod(20*log10(dAmplitude));
        dLog = max(dLog, -80.0);

        if(!m_bFall)
        {
            m_vAmplitude[nChannel][i] = min(0.0, (double)dLog);//max((double)m_vAmplitude[nChannel][i]-m_dFall, (double)dLog));
        }
        else
        {
            m_vAmplitude[nChannel][i] = min(0.0, (max((double)m_vAmplitude[nChannel][i]-m_dFall, (double)dLog)));
        }

        if(dLog > -70.0)
        {
            m_vPhase[nChannel][i] = atan2(m_vfft_out[nChannel][i].i,m_vfft_out[nChannel][i].r)+M_PI;
          //  wxLogDebug(wxT("%d:%d = %.2f"), nChannel,i, m_vPhase[nChannel][i]);
        }
        else
        {
            m_vPhase[nChannel][i] = -M_PI*3;    //this will hide it
        }

        if(dMax < m_vAmplitude[nChannel][i])
        {
            dMax = m_vAmplitude[nChannel][i];
            dMaxBin = i;

        }

    }
    double dQ = (vAmp[dMaxBin+1]-vAmp[dMaxBin-1])/(2*(2*vAmp[dMaxBin]-vAmp[dMaxBin-1]-vAmp[dMaxBin+1]));
    m_uiPeakLevel.SetLabel(wxString::Format(wxT("%.1f"),dMax));
    m_uiPeakFrequency.SetLabel(wxString::Format(wxT("%.0fHz"),(dQ+dMaxBin)*m_dBinSize));
}

void fftphaseMeter::InputSession(const session& aSession)
{

}

void fftphaseMeter::OutputChannels(const std::vector<char>& vChannels)
{

}



void fftphaseMeter::OnLeftUp(wxMouseEvent& event)
{
	m_pBuilder->Maximize((GetSize().x<=600));

}

float fftphaseMeter::WindowMod(float dAmplitude)
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


void fftphaseMeter::SetOverlap(double dPercent)
{
    m_dOverlapPercent = max(0.0, min(100.0, dPercent));
    m_nOverlap = m_nSampleSize - static_cast<size_t>(m_dOverlapPercent * static_cast<double>(m_nSampleSize)/100);

    m_dFall = 0.000390 * static_cast<double>(m_nNumberOfSetBins)*(100-m_dOverlapPercent)/100.0;

    m_uiSettingsOverlap.SetLabel(wxString::Format(wxT("Overlap: %.2f%"), m_dOverlapPercent));
    RefreshRect(m_uiSettingsOverlap.GetRect());

}


void fftphaseMeter::SetNumberOfBins(size_t nBins)
{
    m_nSampleSize = nBins*2;
    m_vfft_out[0].resize(nBins+1);
    m_vfft_out[1].resize(nBins+1);
    m_vAmplitude[0] = vector<float>(m_vfft_out[0].size(), -80.0);
    m_vAmplitude[1] = vector<float>(m_vfft_out[1].size(), -80.0);
    m_vPhase[0] = vector<float>(m_vfft_out[0].size(), M_PI);
    m_vPhase[1] = vector<float>(m_vfft_out[1].size(), M_PI);

    m_vColour.resize(m_vAmplitude[0].size());

    for(size_t i = 0; i < m_vAmplitude[0].size(); i++)
    {
        float r,g,b;
        m_HeatMap.getColourAtValue(static_cast<float>(i)/static_cast<float>(m_vAmplitude[0].size()), r,g,b);
        m_vColour[i] = (wxColour( static_cast<unsigned char>(r*255.0),
                                        static_cast<unsigned char>(g*255.0),
                                      static_cast<unsigned char>(b*255.0)));
    }


    m_dFall = 0.000195 * static_cast<double>(nBins)*(100-m_dOverlapPercent)/100.0;
    //store here so we can get back
    m_nNumberOfSetBins = nBins;

    SetOverlap(m_dOverlapPercent);
    m_uiSettingsBins.SetLabel(wxString::Format(wxT("Res: %.1f Hz"), 24000.0/static_cast<double>(nBins)));
    RefreshRect(m_uiSettingsBins.GetRect());

}

void fftphaseMeter::SetWindowType(int nType)
{
    m_lstBuffer[0].clear();
    m_lstBuffer[1].clear();
    m_nWindowType = nType;
    m_uiSettingsWindow.SetLabel(LABEL_WINDOW[nType]);
    RefreshRect(m_uiSettingsWindow.GetRect());
}

void fftphaseMeter::SetFall(bool bOn)
{
    m_bFall = bOn;
}

void fftphaseMeter::SetDisplayType(int nType)
{
    m_nDisplayType = nType%3;
    Refresh();
}
