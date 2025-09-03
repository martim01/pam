#include "r128meter.h"

#include <algorithm>
#include <list>

#include <wx/busyinfo.h>
#include <wx/dcbuffer.h>
#include <wx/dcmemory.h>

#include "log.h"

#include "settings.h"


using namespace std;

// R128MeterH
BEGIN_EVENT_TABLE(R128MeterH, pmControl)
END_EVENT_TABLE()

 wxIMPLEMENT_DYNAMIC_CLASS(R128MeterH, pmControl);

R128MeterH::R128MeterH() : pmControl()
{
    //create our font
    Connect(wxEVT_PAINT, (wxObjectEventFunction)&R128MeterH::OnPaint);
    Connect(wxEVT_SIZE, (wxObjectEventFunction)&R128MeterH::OnSize);
    m_clrText = wxColour(200,180,255);
}

R128MeterH::R128MeterH(wxWindow *parent, wxWindowID id, const wxString & sText,double dMin, double dMax, bool bLevelDisplay, const wxPoint& pos, const wxSize& size) : pmControl(),
m_clrText(wxColour(200,180,255)),
m_bLevelDisplay(bLevelDisplay)
{
    wxSize szInit(size);
    wxSize bestSize = DoGetBestSize();
    if(size.x<=0)
        szInit.SetWidth(bestSize.x);
    if(size.y <= 0)
        szInit.SetHeight(bestSize.y);

    wxWindow::Create(parent,id,pos,szInit,wxWANTS_CHARS, wxT("R128MeterH"));

    SetMinSize(size);

    SetBackgroundStyle(wxBG_STYLE_CUSTOM);

    Connect(wxEVT_PAINT, (wxObjectEventFunction)&R128MeterH::OnPaint);
    Connect(wxEVT_SIZE, (wxObjectEventFunction)&R128MeterH::OnSize);
    
    m_pairColour[kLow] = make_pair(m_dMin/2, GetBackgroundColour());
    m_pairColour[kMid] = make_pair(m_dMin/4, GetBackgroundColour());
    m_pairColour[kHigh] = make_pair(m_dMax, GetBackgroundColour());

    m_uiBlack.SetBackgroundColour(*wxBLACK);
    InitMeter(dMin, dMax);

    SetFont(wxFont(8,wxFONTFAMILY_SWISS,wxFONTSTYLE_NORMAL,wxFONTWEIGHT_NORMAL,false,_T("Arial"),wxFONTENCODING_DEFAULT));
    

}

R128MeterH::~R128MeterH()=default;



void R128MeterH::OnPaint(wxPaintEvent& event)
{
    wxAutoBufferedPaintDC dc(this);


    dc.SetFont(GetFont());
    wxBrush br(*wxBLACK);
    dc.SetBrush(br);
    dc.DrawRectangle(GetClientRect());
    
    dc.DrawBitmap(m_bmpMeter, 0, 0);
    m_uiBlack.Draw(dc, uiRect::BORDER_NONE);

    if(m_nPeakMode != PEAK_HIDE)
    {
        m_uiPeak.Draw(dc,uiRect::BORDER_FLAT);
    }
    dc.SetPen(wxPen(wxColour(120,120,120),1));

    for(size_t i = 0; i < m_vLevels.size(); i++)
    {
        dc.DrawLine((m_dPixelsPerdB*(m_vLevels[i]-m_dMin)), 
                    0, 
                    (m_dPixelsPerdB*(m_vLevels[i]-m_dMin)),
                    GetClientRect().GetHeight());
    }

    if(m_penTarget.IsOk())
    {
        dc.SetPen(m_penTarget);
        dc.DrawLine((m_dPixelsPerdB*(m_dTargetLevel-m_dMax)),0, (m_dPixelsPerdB*(m_dTargetLevel-m_dMax)), GetClientRect().GetHeight());
    }

}

void R128MeterH::SetTargetLevel(double dValue, const wxPen& penLevel)
{
    m_dTargetLevel = dValue;
    m_penTarget = penLevel;
}

void R128MeterH::InitMeter(double dMin, double dMax)
{
    m_dMin = dMin;
    m_dMax = dMax;
    m_dPeakValue = m_dMin;

    SetMinMax(dMin, m_dMax);
}

void R128MeterH::SetMinMax(double dMin, double dMax)
{
    m_dMin = dMin;
    m_dMax = dMax;
    m_dPeakValue = m_dMin;

    m_dPixelsPerdB = (GetClientRect().GetWidth())/(m_dMax-m_dMin);

    int nLow = (m_pairColour[kLow].first-m_dMin)*m_dPixelsPerdB;
    int nMid = (m_pairColour[kMid].first-m_dMin)*m_dPixelsPerdB;

    pml::log::log(pml::log::Level::kDebug, "Meter") << "Max=" << m_dMax << "\tMin=" << m_dMin << "\tlow=" << m_pairColour[kLow].first << "\tlow pixels=" << nLow << "\tmid=" << m_pairColour[kMid].first << "\tmid pixels=" << nMid;

    m_uiLevel[kLow].SetRect(0, 0, nLow, GetClientRect().GetHeight());
    m_uiLevel[kMid].SetRect(nLow, 0, nMid-nLow, GetClientRect().GetHeight());
    m_uiLevel[kHigh].SetRect(nMid, 0, GetClientRect().GetRight()-nMid, GetClientRect().GetHeight());

    //draw to the bmp..
    wxMemoryDC dc;
    m_bmpMeter = wxBitmap(GetClientSize().x, GetClientSize().y);
    dc.SelectObject(m_bmpMeter);
    if(m_bShading)
    {
        m_uiLevel[kLow].SetGradient(wxSOUTH);
        m_uiLevel[kMid].SetGradient(wxSOUTH);
        m_uiLevel[kHigh].SetGradient(wxSOUTH);
    }
    else
    {
        m_uiLevel[kLow].SetGradient(0);
        m_uiLevel[kMid].SetGradient(0);
        m_uiLevel[kHigh].SetGradient(0);
    }
    m_uiLevel[kLow].Draw(dc, uiRect::BORDER_NONE);
    m_uiLevel[kMid].Draw(dc, uiRect::BORDER_NONE);
    m_uiLevel[kHigh].Draw(dc, uiRect::BORDER_NONE);


    m_uiPeak.SetBackgroundColour(m_pairColour[kHigh].second);

}


bool R128MeterH::SetLightColours(double dLow, wxColour clrLow, double dMid, wxColour clrMid,  wxColour clrHigh)
{
    m_pairColour[kLow] = make_pair(dLow, clrLow);
    m_pairColour[kMid] = make_pair(dMid, clrMid);
    m_pairColour[kHigh] = make_pair(m_dMax, clrHigh);

    m_uiSimple.SetBackgroundColour(clrLow,clrLow);

    m_uiLevel[kLow].SetBackgroundColour(clrLow, *wxBLACK);
    m_uiLevel[kMid].SetBackgroundColour(clrMid, clrLow);
    m_uiLevel[kHigh].SetBackgroundColour(clrHigh, clrMid);

    InitMeter(m_dMin, m_dMax);

    return true;

}
void R128MeterH::ResetMeter(void)
{
    m_nPeakCounter = 0;
    m_dLastValue = -80;
    m_dPeakValue = -80;
    Refresh();
}
void R128MeterH::ShowValue(double dValue)
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
    
        
        int ndB = m_dPixelsPerdB*(m_dLastValue-m_dMin);
        
        ndB = max(ndB, 0);

        int nPeakdB = m_dPixelsPerdB*(m_dPeakValue-m_dMax);
        nPeakdB = max(nPeakdB, 0);

        m_uiBlack.SetTop(0);
        m_uiBlack.SetBottom(GetClientRect().GetBottom());
        m_uiBlack.SetLeft(ndB);
        m_uiBlack.SetRight(GetClientRect().GetRight());

        m_uiPeak.SetTop(1);
        m_uiPeak.SetBottom(GetClientRect().GetBottom()-1);
        m_uiPeak.SetWidth(3);
        m_uiPeak.SetLeft(nPeakdB-1);


        if(m_uiBlack.GetLeft() < m_rectLastBlack.GetLeft())
        {
            RefreshRect(wxRect(m_uiBlack.GetLeft(), 0, m_rectLastBlack.GetLeft()-m_uiBlack.GetLeft(), m_uiBlack.GetHeight()));
        }
        else if(m_uiBlack.GetLeft() > m_rectLastBlack.GetLeft())
        {
            RefreshRect(wxRect(m_rectLastBlack.GetLeft(), 0, m_uiBlack.GetLeft()-m_rectLastBlack.GetLeft(), m_uiBlack.GetHeight()));
        }

        if(m_uiPeak.GetLeft() != m_rectLastPeak.GetLeft())
        {
            RefreshRect(m_uiPeak.GetRect());
            RefreshRect(m_rectLastPeak);
        }

        m_rectLastBlack = m_uiBlack.GetRect();
        m_rectLastBlack = m_uiBlack.GetRect();
        m_rectLastPeak = m_uiPeak.GetRect();
    }
}


void R128MeterH::OnSize(wxSizeEvent& event)
{
    InitMeter(m_dMin, m_dMax);

    Refresh();
}


void R128MeterH::SetLevelDisplay(bool bLevel)
{
    m_bLevelDisplay = bLevel;
    ResetMeter();
}

void R128MeterH::SetLevels(const std::vector<double>& vLevels, double dOffset)
{
    m_dLevelOffset = dOffset;
    m_vLevels = vLevels;
}


void R128MeterH::SetPeakMode(int nMode)
{
    m_nPeakMode = nMode;
}

void R128MeterH::FreezeMeter(bool bFreeze)
{
    m_bFreeze = bFreeze;
}

void R128MeterH::SetShading(bool bShading)
{
    m_bShading = bShading;

    //draw to the bmp..
    wxMemoryDC dc;
    m_bmpMeter = wxBitmap(GetClientSize().x, GetClientSize().y);
    dc.SelectObject(m_bmpMeter);
    if(m_bShading)
    {
        m_uiLevel[kHigh].Draw(dc, uiRect::BORDER_NONE);
        m_uiLevel[kMid].Draw(dc, uiRect::BORDER_NONE);
        m_uiLevel[kLow].Draw(dc, uiRect::BORDER_NONE);
    }
    else
    {
        m_uiSimple.Draw(dc,uiRect::BORDER_NONE);
    }
    ResetMeter();
}


