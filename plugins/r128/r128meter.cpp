#include <wx/log.h>
#include "r128meter.h"
#include <wx/busyinfo.h>
#include <wx/dcbuffer.h>
#include <list>
#include <wx/dcmemory.h>
#include <algorithm>
#include "settings.h"


using namespace std;

// R128Meter
BEGIN_EVENT_TABLE(R128Meter, pmControl)
END_EVENT_TABLE()

 wxIMPLEMENT_DYNAMIC_CLASS(R128Meter, pmControl);

R128Meter::R128Meter() : pmControl()
    , m_dMin(-70)
    , m_dMax(0)
    , m_bShading(false)
{
    //create our font
    Connect(wxEVT_PAINT, (wxObjectEventFunction)&R128Meter::OnPaint);
    Connect(wxEVT_SIZE, (wxObjectEventFunction)&R128Meter::OnSize);
    m_clrText = wxColour(200,180,255);
}

R128Meter::R128Meter(wxWindow *parent, wxWindowID id, const wxString & sText,double dMin, double dMax, bool bLevelDisplay, const wxPoint& pos, const wxSize& size) : pmControl(),
    m_dMax(0),
    m_dFall(80)
{
    m_dLastValue = -180;
    wxSize szInit(size);
    wxSize bestSize = DoGetBestSize();
    if(size.x<=0)
        szInit.SetWidth(bestSize.x);
    if(size.y <= 0)
        szInit.SetHeight(bestSize.y);


    m_dLevelOffset = 0.0;
    wxWindow::Create(parent,id,pos,szInit,wxWANTS_CHARS, wxT("R128Meter"));

    SetMinSize(size);

    m_nPeakMode = PEAK_SHOW;
    m_bFreeze = false;

    SetBackgroundStyle(wxBG_STYLE_CUSTOM);

    m_bLevelDisplay = bLevelDisplay;

    Connect(wxEVT_PAINT, (wxObjectEventFunction)&R128Meter::OnPaint);
    Connect(wxEVT_SIZE, (wxObjectEventFunction)&R128Meter::OnSize);
    m_clrText = wxColour(200,180,255);

    m_pairColour[0] = make_pair(m_dMin/2, GetBackgroundColour());
    m_pairColour[1] = make_pair(m_dMin/4, GetBackgroundColour());
    m_pairColour[2] = make_pair(m_dMax, GetBackgroundColour());

    m_uiBlack.SetBackgroundColour(*wxBLACK);
    InitMeter(sText, dMin, dMax);

    SetFont(wxFont(8,wxFONTFAMILY_SWISS,wxFONTSTYLE_NORMAL,wxFONTWEIGHT_NORMAL,false,_T("Arial"),wxFONTENCODING_DEFAULT));
    m_nChannels = 2;

}

R128Meter::~R128Meter()
{
}



void R128Meter::OnPaint(wxPaintEvent& event)
{
    wxAutoBufferedPaintDC dc(this);


    dc.SetFont(GetFont());
    wxBrush br(*wxBLACK);
    dc.SetBrush(br);
    dc.DrawRectangle(GetClientRect());
    if(!m_bLevelDisplay)
    {
        dc.DrawBitmap(m_bmpMeter, 0,m_uiLevelText.GetBottom());
        m_uiBlack.Draw(dc, uiRect::BORDER_NONE);

        if(m_nPeakMode != PEAK_HIDE)
        {
            m_uiPeak.Draw(dc,uiRect::BORDER_FLAT);
        }
        dc.SetPen(wxPen(wxColour(120,120,120),1));

        for(size_t i = 0; i < m_vLevels.size(); i++)
        {
            dc.DrawLine(0, m_uiLevelText.GetBottom()-(m_dPixelsPerdB*(m_vLevels[i]-m_dMax)), GetClientRect().GetWidth(), m_uiLevelText.GetBottom()-(m_dPixelsPerdB*(m_vLevels[i]-m_dMax)));
        }

        if(m_penTarget.IsOk())
        {
            dc.SetPen(m_penTarget);
            dc.DrawLine(0, m_uiLevelText.GetBottom()-(m_dPixelsPerdB*(m_dTargetLevel-m_dMax)), GetClientRect().GetWidth(), m_uiLevelText.GetBottom()-(m_dPixelsPerdB*(m_dTargetLevel-m_dMax)));
        }

        dc.SetPen(wxNullPen);

        m_uiLabel.Draw(dc, uiRect::BORDER_NONE);
        m_uiLevelText.Draw(dc, uiRect::BORDER_NONE);

    }
    else
    {
        for(size_t i = 0; i < m_vLevels.size(); i++)
        {
            int nY(m_uiLevelText.GetBottom()-(m_dPixelsPerdB*(m_vLevels[i]-m_dMax)));
            dc.SetPen(wxPen(wxColour(120,120,120),1));
            dc.DrawLine(0, m_uiLevelText.GetBottom()-(m_dPixelsPerdB*(m_vLevels[i]-m_dMax)), GetClientRect().GetWidth(), m_uiLevelText.GetBottom()-(m_dPixelsPerdB*(m_vLevels[i]-m_dMax)));
            uiRect uiLevel(wxRect(15, nY-10,GetClientSize().x-30, 20));
            uiLevel.SetBackgroundColour(*wxBLACK);
            uiLevel.SetForegroundColour(*wxWHITE);
            uiLevel.Draw(dc, wxString::Format(wxT("%.0f"), m_vLevels[i]-m_dLevelOffset), uiRect::BORDER_NONE);
        }
        if(m_penTarget.IsOk())
        {
            dc.SetPen(m_penTarget);
            dc.DrawLine(0, m_uiLevelText.GetBottom()-(m_dPixelsPerdB*(m_dTargetLevel-m_dMax)), GetClientRect().GetWidth(), m_uiLevelText.GetBottom()-(m_dPixelsPerdB*(m_dTargetLevel-m_dMax)));
        }
    }
}

void R128Meter::SetTargetLevel(double dValue, const wxPen& penLevel)
{
    m_dTargetLevel = dValue;
    m_penTarget = penLevel;
}

void R128Meter::InitMeter(const wxString& sText,double dMin, double dMax)
{
    m_dMin = dMin;
    m_dMax = dMax;
    m_dPeakValue = m_dMin;

    //work out size of rects
    wxString sLevel;
    m_uiLabel.SetLabel(sText);
    m_uiLabel.SetGradient(0);
    m_uiLevelText.SetRect(0,0, GetClientRect().GetWidth(), 20);
    m_uiLevelText.SetGradient(0);
    m_uiLabel.SetRect(0,GetClientRect().GetBottom()-20, GetClientRect().GetWidth(), 20);

    SetMinMax(dMin, m_dMax);
}

void R128Meter::SetMinMax(double dMin, double dMax)
{
    m_dMin = dMin;
    m_dMax = dMax;
    m_dPeakValue = m_dMin;

    m_dPixelsPerdB = (GetClientRect().GetHeight()-40)/(m_dMax-m_dMin);
    int nTop = m_uiLevelText.GetBottom();
    int nBottom = m_uiLabel.GetTop();
    //int nTop = m


    int nLow = (m_pairColour[0].first-m_dMax)*m_dPixelsPerdB;
    int nMid = (m_pairColour[1].first-m_dMax)*m_dPixelsPerdB;

    m_uiLevel[0].SetRect(0, nTop, GetClientRect().GetWidth(), nBottom-nTop);
    m_uiLevel[1].SetRect(0, 0, GetClientRect().GetWidth(), -nLow);
    m_uiLevel[2].SetRect(0, 0, GetClientRect().GetWidth(), -nMid);



    //m_uiLevel[2].SetRect(0, nTop, GetClientRect().GetWidth(), -nMid);


    //draw to the bmp..
    wxMemoryDC dc;
    m_bmpMeter = wxBitmap(GetClientSize().x, GetClientSize().y-m_uiLevelText.GetHeight()-m_uiLabel.GetHeight());
    dc.SelectObject(m_bmpMeter);
    if(m_bShading)
    {
        m_uiLevel[0].SetGradient(wxSOUTH);
        m_uiLevel[1].SetGradient(wxSOUTH);
        m_uiLevel[2].SetGradient(wxSOUTH);
    }
    else
    {
        m_uiLevel[0].SetGradient(0);
        m_uiLevel[1].SetGradient(0);
        m_uiLevel[2].SetGradient(0);
    }
    m_uiLevel[0].Draw(dc, uiRect::BORDER_NONE);
    m_uiLevel[1].Draw(dc, uiRect::BORDER_NONE);
    m_uiLevel[2].Draw(dc, uiRect::BORDER_NONE);


    m_uiPeak.SetBackgroundColour(m_pairColour[2].second);

}


bool R128Meter::SetLightColours(double dLow, wxColour clrLow, double dMid, wxColour clrMid,  wxColour clrHigh)
{
    m_pairColour[0] = make_pair(dLow, clrLow);
    m_pairColour[1] = make_pair(dMid, clrMid);
    m_pairColour[2] = make_pair(m_dMax, clrHigh);

    m_uiSimple.SetBackgroundColour(clrLow,clrLow);

    m_uiLevel[0].SetBackgroundColour(clrLow, *wxBLACK);
    m_uiLevel[1].SetBackgroundColour(clrMid, clrLow);
    m_uiLevel[2].SetBackgroundColour(clrHigh, clrMid);

    InitMeter(m_uiLabel.GetLabel(), m_dMin, m_dMax);

    return true;

}
void R128Meter::ResetMeter(void)
{
    m_nPeakCounter = 0;
    m_dLastValue = -80;
    m_dPeakValue = -80;
    Refresh();
}
void R128Meter::ShowValue(double dValue)
{
    if(!m_bFreeze)
    {
        if(dValue > m_dLastValue-m_dFall)
        {
            m_dLastValue = dValue;
        }
        else
        {
            m_dLastValue -= m_dFall;
        }


        if(m_nPeakMode != PEAK_HOLD)
        {
            m_nPeakCounter++;
        }
        if(m_nPeakCounter >= 10 || dValue >= m_dPeakValue)
        {
            m_dPeakValue = min(m_dLastValue, m_dMax);
            if(m_dLevelOffset == 0.0)
            {
                if(m_dMax <= 0)
                {
                    m_uiLevelText.SetLabel(wxString::Format(wxT("%.1f LUFS"), dValue));
                }
                else
                {
                    m_uiLevelText.SetLabel(wxString::Format(wxT("%.1f dB"), dValue));
                }
            }
            else
            {
                m_uiLevelText.SetLabel(wxString::Format(wxT("%.1f LU"), dValue-m_dLevelOffset));
            }
            m_nPeakCounter = 0;
            RefreshRect(m_uiLevelText.GetRect());
        }

        int ndB = m_dPixelsPerdB*(m_dLastValue-m_dMax);
        ndB = max(ndB, -(m_uiLabel.GetTop()-m_uiLevelText.GetHeight()));

        int nPeakdB = m_dPixelsPerdB*(m_dPeakValue-m_dMax);
        nPeakdB = max(nPeakdB, -(m_uiLabel.GetTop()-m_uiLevelText.GetHeight()));

        m_uiBlack.SetRect(0, m_uiLevelText.GetBottom(), GetClientRect().GetWidth(), -ndB);
        m_uiPeak.SetRect(0,m_uiLevelText.GetBottom()-(nPeakdB)-1, GetClientRect().GetWidth(), 3);

        if(m_uiBlack.GetBottom() != m_rectLastBlack.GetBottom())    //level change
        {
            RefreshRect(wxRect(0,m_uiLevelText.GetBottom(), GetClientSize().x, max(m_uiBlack.GetBottom(), m_rectLastBlack.GetBottom())+1-m_uiLevelText.GetBottom()));
        }

        if(m_uiPeak.GetBottom() != m_rectLastPeak.GetBottom())
        {
            RefreshRect(m_uiPeak.GetRect());
            RefreshRect(m_rectLastPeak);
        }

        m_rectLastBlack = m_uiBlack.GetRect();
        m_rectLastBlack = m_uiBlack.GetRect();
        m_rectLastPeak = m_uiPeak.GetRect();
    }
}


void R128Meter::OnSize(wxSizeEvent& event)
{
    InitMeter(m_uiLabel.GetLabel(), m_dMin, m_dMax);

    Refresh();
}


void R128Meter::SetLevelDisplay(bool bLevel)
{
    m_bLevelDisplay = bLevel;
    ResetMeter();
}

void R128Meter::SetLevels(const double dLevels[], size_t nSize, double dOffset)
{
    m_dLevelOffset = dOffset;
    m_vLevels.clear();
    m_vLevels.resize(nSize);
    for(size_t i = 0 ; i < nSize; i++)
    {
        m_vLevels[i] = dLevels[i];
    }

    RefreshRect(m_uiLabel.GetRect());
}

void R128Meter::SetLabel(const wxString& sLabel)
{
    m_uiLabel.SetLabel(sLabel);
    RefreshRect(m_uiLabel.GetRect());
}


void R128Meter::SetPeakMode(int nMode)
{
    m_nPeakMode = nMode;
}

void R128Meter::FreezeMeter(bool bFreeze)
{
    m_bFreeze = bFreeze;
}

void R128Meter::SetShading(bool bShading)
{
    m_bShading = bShading;

    //draw to the bmp..
    wxMemoryDC dc;
    m_bmpMeter = wxBitmap(GetClientSize().x, GetClientSize().y-m_uiLevelText.GetHeight()-m_uiLabel.GetHeight());
    dc.SelectObject(m_bmpMeter);
    if(m_bShading)
    {
        m_uiLevel[2].Draw(dc, uiRect::BORDER_NONE);
        m_uiLevel[1].Draw(dc, uiRect::BORDER_NONE);
        m_uiLevel[0].Draw(dc, uiRect::BORDER_NONE);
    }
    else
    {
        m_uiSimple.Draw(dc,uiRect::BORDER_NONE);
    }
    ResetMeter();
}


