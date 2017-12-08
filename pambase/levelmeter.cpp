#include <wx/log.h>
#include "levelmeter.h"
#include <wx/busyinfo.h>
#include <wx/dcbuffer.h>
#include <list>
#include <wx/dcmemory.h>
#include "meter.h"
#include <algorithm>


using namespace std;

// LevelMeter
BEGIN_EVENT_TABLE(LevelMeter, wxWindow)
END_EVENT_TABLE()

LevelMeter::LevelMeter()
    : m_dMin(-70)
    , m_dMax(0)
    , m_nMeterDisplay(PEAK)
    , m_pPPM(0)
    , m_pLoud(0)
    , m_bShading(true)
{
    //create our font
    Connect(wxEVT_PAINT, (wxObjectEventFunction)&LevelMeter::OnPaint);
    Connect(wxEVT_SIZE, (wxObjectEventFunction)&LevelMeter::OnSize);
    m_clrText = wxColour(200,180,255);
}

LevelMeter::LevelMeter(wxWindow *parent, wxWindowID id, const wxString & sText,double dMin, bool bLevelDisplay, const wxPoint& pos, const wxSize& size) :
    m_dMax(0),
    m_nMeterDisplay(PEAK),
    m_pPPM(0),
    m_pLoud(0)
{
    m_dLastValue = -180;
    wxSize szInit(size);
    wxSize bestSize = DoGetBestSize();
    if(size.x<=0)
        szInit.SetWidth(bestSize.x);
    if(size.y <= 0)
        szInit.SetHeight(bestSize.y);


    wxWindow::Create(parent,id,pos,szInit,wxWANTS_CHARS, wxT("levelmeter"));

#ifndef __WXMSW__
    SetCursor(wxCURSOR_BLANK);
#endif // __WXMSW__
    SetMinSize(size);

    m_nPeakMode = PEAK_SHOW;
    m_bFreeze = false;

    SetBackgroundStyle(wxBG_STYLE_CUSTOM);

//    SetMeterSpeed(meter::NORMAL);
//    SetMeterMSMode(meter::M6);

    m_bLevelDisplay = bLevelDisplay;

    Connect(wxEVT_PAINT, (wxObjectEventFunction)&LevelMeter::OnPaint);
    Connect(wxEVT_SIZE, (wxObjectEventFunction)&LevelMeter::OnSize);
    m_clrText = wxColour(200,180,255);

    m_pairColour[0] = make_pair(m_dMin/2, GetBackgroundColour());
    m_pairColour[1] = make_pair(m_dMin/4, GetBackgroundColour());
    m_pairColour[2] = make_pair(m_dMax, GetBackgroundColour());

    m_uiBlack.SetBackgroundColour(*wxBLACK);
    InitMeter(sText, dMin);

    SetFont(wxFont(8,wxFONTFAMILY_SWISS,wxFONTSTYLE_NORMAL,wxFONTWEIGHT_NORMAL,false,_T("Arial"),wxFONTENCODING_DEFAULT));
    m_nChannels = 2;

}

LevelMeter::~LevelMeter()
{
    if(m_pPPM)
    {
        delete m_pPPM;
    }
    if(m_pLoud)
    {
        delete m_pLoud;
    }
}



void LevelMeter::OnPaint(wxPaintEvent& event)
{
    wxAutoBufferedPaintDC dc(this);


    dc.SetFont(GetFont());
    wxBrush br(*wxBLACK);
    dc.SetBrush(br);
    dc.DrawRectangle(GetClientRect());
    if(!m_bLevelDisplay)
    {
        m_uiLabel.Draw(dc, uiRect::BORDER_NONE);
        m_uiLevelText.Draw(dc, uiRect::BORDER_NONE);
        dc.DrawBitmap(m_bmpMeter, 0,m_uiLevelText.GetBottom());
        m_uiBlack.Draw(dc, uiRect::BORDER_NONE);

        if(m_nPeakMode != PEAK_HIDE)
        {
            m_uiPeak.Draw(dc,uiRect::BORDER_FLAT);
        }
        dc.SetPen(wxPen(wxColour(200,200,200),1, wxDOT));

        if(m_nMeterDisplay != PPM)
        {
            for(size_t i = 0; i < m_vLevels.size(); i++)
            {
                dc.DrawLine(0, m_uiLevelText.GetBottom()-(m_dPixelsPerdB*m_vLevels[i]), GetClientRect().GetWidth(), m_uiLevelText.GetBottom()-(m_dPixelsPerdB*m_vLevels[i]));
            }
        }
        else
        {
            for(size_t i = 0; i < 8; i++)
            {
                int nY(m_uiLevelText.GetBottom()+(m_dPixelsPerPPM*(8-i)));
                dc.DrawLine(0, nY, GetClientRect().GetWidth(), nY);
            }
        }
        dc.SetPen(wxNullPen);
    }
    else
    {

        if(m_nMeterDisplay != PPM)
        {
                for(size_t i = 0; i < m_vLevels.size(); i++)
                {
                    int nY(m_uiLevelText.GetBottom()-(m_dPixelsPerdB*m_vLevels[i]));


                    dc.SetPen(wxPen(wxColour(200,200,200),1, wxDOT));
                    dc.DrawLine(0, m_uiLevelText.GetBottom()-(m_dPixelsPerdB*m_vLevels[i]), GetClientRect().GetWidth(), m_uiLevelText.GetBottom()-(m_dPixelsPerdB*m_vLevels[i]));
                    uiRect uiLevel(wxRect(15, nY-10,GetClientSize().x-30, 20));
                    uiLevel.SetBackgroundColour(*wxBLACK);
                    uiLevel.SetForegroundColour(*wxWHITE);
                    uiLevel.Draw(dc, wxString::Format(wxT("%.0f"), m_vLevels[i]), uiRect::BORDER_NONE);
                }
        }
        else
        {
            for(size_t i = 0; i < 8; i++)
            {
                int nY(m_uiLevelText.GetBottom()+(m_dPixelsPerPPM*(8-i)));
                dc.SetPen(wxPen(wxColour(200,200,200),1, wxDOT));

                dc.DrawLine(0, nY, GetClientRect().GetWidth(), nY);
                uiRect uiLevel(wxRect(15, nY-10,GetClientSize().x-30, 20));
                uiLevel.SetBackgroundColour(*wxBLACK);
                uiLevel.SetForegroundColour(*wxWHITE);
                #ifdef __WXMSW__
                uiLevel.Draw(dc, wxString::Format(wxT("%u"), i), uiRect::BORDER_NONE);
                #else
                uiLevel.Draw(dc, wxString::Format(wxT("%zu"), i), uiRect::BORDER_NONE);
                #endif
            }
        }
    }
}


void LevelMeter::InitMeter(const wxString& sText,double dMin)
{
    m_dMin = dMin;
    m_dPeakValue = m_dMin;

    //work out size of rects
    wxString sLevel;

    m_uiLabel.SetLabel(sText);
    m_uiLabel.SetGradient(0);
    m_uiLevelText.SetRect(0,0, GetClientRect().GetWidth(), 20);
    m_uiLevelText.SetGradient(0);
    m_uiLabel.SetRect(0,GetClientRect().GetBottom()-20, GetClientRect().GetWidth(), 20);

    m_dPixelsPerdB = (GetClientRect().GetHeight()-40)/(-m_dMin);

    m_dPixelsPerPPM = (GetClientRect().GetHeight()-40)/8.0;


    int nLow = -m_pairColour[0].first*m_dPixelsPerdB;
    int nMid = -m_pairColour[1].first*m_dPixelsPerdB;



    m_uiLevel[0].SetRect(0, m_uiLevelText.GetBottom()+nLow, GetClientRect().GetWidth(), GetClientRect().GetHeight()-m_uiLevelText.GetHeight()-nLow-m_uiLabel.GetHeight());
    m_uiLevel[1].SetRect(0, m_uiLevelText.GetBottom()+nMid, GetClientRect().GetWidth(), m_uiLevel[0].GetTop()-nMid);
    m_uiLevel[2].SetRect(0, m_uiLevelText.GetBottom(), GetClientRect().GetWidth(), nMid);

    m_uiSimple.SetRect(0, m_uiLevelText.GetBottom(), GetClientRect().GetWidth(), GetClientRect().GetHeight()-m_uiLevelText.GetHeight()-m_uiLabel.GetHeight());


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


    m_uiPeak.SetBackgroundColour(m_pairColour[2].second);

}

bool LevelMeter::SetLightColours(double dLow, wxColour clrLow, double dMid, wxColour clrMid,  wxColour clrHigh)
{

    m_pairColour[0] = make_pair(dLow, clrLow);
    m_pairColour[1] = make_pair(dMid, clrMid);
    m_pairColour[2] = make_pair(m_dMax, clrHigh);

    m_uiSimple.SetBackgroundColour(clrLow,clrLow);

    m_uiLevel[0].SetBackgroundColour(wxNullColour, clrLow);
    m_uiLevel[1].SetBackgroundColour(clrLow, clrMid);
    m_uiLevel[2].SetBackgroundColour(clrMid, clrHigh);

    InitMeter(m_uiLabel.GetLabel(), m_dMin);

    return true;

}
void LevelMeter::ResetMeter(void)
{
    m_nPeakCounter = 0;
    switch(m_nMeterDisplay)
    {
        case PEAK:
        case ENERGY:
        case TOTAL:
        case AVERAGE:
            m_dLastValue = -80;
            m_dPeakValue = -80;
            break;
        case PPM:
            m_dLastValue = 0;
            m_dPeakValue = 0;
            break;
        case LOUD:
            m_dLastValue = -80;
            m_dPeakValue = -80;

    }
    Refresh();
}
void LevelMeter::ShowValue(double dValue)
{
    if(m_nMeterDisplay == PPM)
    {
        ShowPPM(dValue);
    }
    else
    {
        if(!m_bFreeze)
        {
            if(m_nMeterDisplay != LOUD)
            {
                if(dValue != 0)
                {
                    dValue = 20*log10(dValue);
                }
                else
                {
                    dValue = -1e10;
                }
            }
            if(dValue > m_dLastValue-m_dFall)
            {
                m_dLastValue = dValue;
            }
            else
            {
                m_dLastValue -= m_dFall;
            }
        }

        if(m_nPeakMode != PEAK_HOLD)
        {
            m_nPeakCounter++;
        }
        if(m_nPeakCounter >= 96 || dValue >= m_dPeakValue)
        {
            m_dPeakValue = min(dValue, m_dMax);
            if(m_nMeterDisplay != LOUD)
            {
                m_uiLevelText.SetLabel(wxString::Format(wxT("%.1fdB"), m_dPeakValue));
            }
            else
            {
                m_uiLevelText.SetLabel(wxString::Format(wxT("%.1f L"), m_dPeakValue));
            }
            m_nPeakCounter = 0;
            RefreshRect(m_uiLevelText.GetRect());
        }

        int ndB = m_dPixelsPerdB*m_dLastValue;
        ndB = max(ndB, -(m_uiLabel.GetTop()-m_uiLevelText.GetHeight()));

        int nPeakdB = m_dPixelsPerdB*m_dPeakValue;
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


void LevelMeter::OnSize(wxSizeEvent& event)
{
    InitMeter(m_uiLabel.GetLabel(), m_dMin);

    Refresh();
}


void LevelMeter::SetLevelDisplay(bool bLevel)
{
    m_bLevelDisplay = bLevel;
    ResetMeter();
}

void LevelMeter::SetNumberOfChannels(unsigned int nChannels)
{
    m_nChannels = nChannels;
    SetMeterDisplay(m_nMeterDisplay);
}


void LevelMeter::SetMeterDisplay(short nDisplay)
{
    m_nMeterDisplay = nDisplay;

    ResetMeter();
}


void LevelMeter::SetLevels(const double dLevels[], size_t nSize)
{
    m_vLevels.clear();
    m_vLevels.resize(nSize);
    for(size_t i = 0 ; i < nSize; i++)
    {
        m_vLevels[i] = dLevels[i];
    }
    RefreshRect(m_uiLabel.GetRect());
}

void LevelMeter::SetLabel(const wxString& sLabel)
{
    m_uiLabel.SetLabel(sLabel);
    RefreshRect(m_uiLabel.GetRect());
}


void LevelMeter::SetPeakMode(int nMode)
{
    m_nPeakMode = nMode;
}

void LevelMeter::FreezeMeter(bool bFreeze)
{
    m_bFreeze = bFreeze;
}


void LevelMeter::ShowPPM(double dValue)
{

    if(!m_bFreeze)
    {
        m_dLastValue = dValue;
    }

    if(m_nPeakMode != PEAK_HOLD)
    {
        m_nPeakCounter++;
    }
    if(m_nPeakCounter >= 96 || dValue >= m_dPeakValue)
    {
        m_dPeakValue = min(dValue, 8.0);
        m_uiLevelText.SetLabel(wxString::Format(wxT("%.1f"), m_dPeakValue));
        m_nPeakCounter = 0;
        RefreshRect(m_uiLevelText.GetRect());
    }


    int nY = m_dPixelsPerPPM*(8-m_dLastValue);
    nY = min(nY, (m_uiLabel.GetTop()-m_uiLevelText.GetHeight()));

    int nPeakY = m_dPixelsPerPPM*(8-m_dPeakValue);
    nPeakY = min(nPeakY, (m_uiLabel.GetTop()-m_uiLevelText.GetHeight()));

    m_uiBlack.SetRect(0, m_uiLevelText.GetBottom(), GetClientRect().GetWidth(), nY);
    m_uiPeak.SetRect(0,m_uiLevelText.GetBottom()+(nPeakY), GetClientRect().GetWidth(), 2);

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


void LevelMeter::SetShading(bool bShading)
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


void LevelMeter::SetSpeed(long nSpeed)
{
    switch(nSpeed)
    {
        case meter::SLOW:
            m_dFall = 0.15;
            break;
        case meter::NORMAL:
            m_dFall = 0.3;
            break;
        case meter::FAST:
            m_dFall = 0.6;
    }
}
