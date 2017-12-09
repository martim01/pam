#include "radarmeter.h"
#include <wx/dc.h>
#include <wx/dcbuffer.h>
#include <wx/log.h>
#include <wx/image.h>
#include "levelcalculator.h"


using namespace std;

BEGIN_EVENT_TABLE(RadarMeter, wxWindow)
    EVT_PAINT(RadarMeter::OnPaint)
    EVT_SIZE(RadarMeter::OnSize)
    EVT_LEFT_UP(RadarMeter::OnLeftUp)
END_EVENT_TABLE()


const float RadarMeter::LEVEL_PPM[7] = {0.031, 0.05, 0.079, 0.126, 0.199, 0.316, 0.501};
const wxString RadarMeter::LABEL_SCALE[3] = {wxT("dB"), wxT("Linear"), wxT("Auto")};

RadarMeter::RadarMeter(wxWindow *parent, wxWindowID id, const wxPoint& pos, const wxSize& size)
{
    Create(parent, id, pos, size);
    #ifndef __WXMSW__
    SetCursor(wxCURSOR_BLANK);
    #endif // __WXMSW__
    m_bRotate = false;
    m_nScaling = SCALE_DB;
    m_bShowLevels = false;

    m_dAngle = 0.0;
    m_dLevel = 0.0;

    SetMindB(70.0);

    m_nSampleRate = 48000;

    m_timerSecond.SetOwner(this, wxNewId());
    m_timerSecond.Start(250);
    m_nRefreshRate = 250;
    SetTimespan(60, false);


    m_nSamples = 0;
    m_nMode = LevelCalculator::PEAK;

    Connect(wxID_ANY, wxEVT_TIMER, (wxObjectEventFunction)&RadarMeter::OnTimer);
}

bool RadarMeter::Create(wxWindow *parent, wxWindowID id, const wxPoint& pos, const wxSize& size)
{
    wxSize szInit(size);
    wxSize bestSize(600,481);
    if(size.x<=0)
        szInit.SetWidth(bestSize.x);
    if(size.y <= 0)
        szInit.SetHeight(bestSize.y);

    if(!wxWindow::Create(parent,id,pos,szInit,wxWANTS_CHARS, wxT("Radar")))
        return false;

    SetMinSize(size);

    SetBackgroundStyle(wxBG_STYLE_CUSTOM);

    m_nChannels = 2;

    int nX = min(GetClientRect().GetWidth()-2, GetClientRect().GetHeight()-2);
    m_rectGrid = wxRect(5, 0, nX, nX);




    return true;
}

RadarMeter::~RadarMeter()
{

}


void RadarMeter::OnPaint(wxPaintEvent& event)
{
    wxAutoBufferedPaintDC dc(this);
    wxBrush br(*wxBLACK);
    dc.SetBrush(br);
    dc.DrawRectangle(GetClientRect());
    dc.SetPen(wxColour(0,0,200));

    DrawRadar(dc);

}


void RadarMeter::DrawRadar(wxDC& dc)
{

    double dYa = (sin(m_dAngle) * m_dLevel)*m_dResolution;
    double dXa = (cos(m_dAngle) * m_dLevel)*m_dResolution;
    m_queueLines.push(wxPoint(m_pntCenter.x+dXa, m_pntCenter.y+dYa));
    m_queueFade.push(wxPoint(m_pntCenter.x+dXa, m_pntCenter.y+dYa));

    wxPoint pnt[3] = {m_pntCenter, m_pntLast, m_queueLines.back()};
    wxRegion rgn(3, pnt);


    wxMemoryDC memDC;
    memDC.SelectObject(m_bmpScreen);



    if(rgn.IsEmpty() == false && m_queueLines.size()>1)
    {
        memDC.SetDeviceClippingRegion(rgn);
        memDC.DrawBitmap(m_bmpBackground, 0,0);
        memDC.DestroyClippingRegion();
    }

    m_pntLast = m_queueLines.back();


    if(m_queueFade.size() > m_nPoints-5)//(static_cast<double>(m_nPoints*0.8)))
    {
        wxPoint pntFade[3] = {m_pntCenter, m_pntLastFade, m_queueFade.front()};
        wxRegion rgnFade(3, pntFade);
        if(rgnFade.IsEmpty() == false)
        {
            memDC.SetDeviceClippingRegion(rgnFade);
            memDC.DrawBitmap(m_bmpFade, 0,0);
            memDC.DestroyClippingRegion();
        }
        m_pntLastFade = m_queueFade.front();
        m_queueFade.pop();
    }

    if(m_queueLines.size() > m_nPoints-2)
    {
        //draw the blank bit
        memDC.SetPen(*wxBLACK_PEN);
        memDC.SetBrush(*wxBLACK_BRUSH);
        wxPoint pnt[3] = {m_pntCenter, m_pntLastBlack, m_queueLines.front()};
        memDC.DrawPolygon(3, pnt);

        m_pntLastBlack = m_queueLines.front();
        m_queueLines.pop();

    }
    memDC.SetBrush(*wxTRANSPARENT_BRUSH);
    memDC.SetPen(wxPen(wxColour(wxT("#354e61"))));

    if(m_nMode != LevelCalculator::PPM)
    {
        for(int i = m_dMindB; i > 0; i-= 10)
        {
            memDC.DrawCircle(m_pntCenter, i*m_dResolution);
        }
    }
    else
    {
        for(size_t i = 0; i < 9; i++)
        {
            int nPPM = (i*4)-34;
            if(i == 4)
            {
                memDC.SetPen(wxPen(wxColour(wxT("#654e61"))));
            }
            else
            {
                memDC.SetPen(wxPen(wxColour(wxT("#354e61"))));
            }
            memDC.DrawCircle(m_pntCenter, (m_dMindB+nPPM)*m_dResolution);
        }
    }

    for(size_t i = 0; i < 12; i++)
    {
        dYa = (sin((M_PI/6*i)) * m_dMindB)*m_dResolution;
        dXa = (cos((M_PI/6*i)) * m_dMindB)*m_dResolution;
        memDC.DrawLine(m_pntCenter.x, m_pntCenter.y, m_pntCenter.x+dXa, m_pntCenter.y+dYa);
    }


    dc.Blit(m_rectGrid.GetLeft(),m_rectGrid.GetTop(),m_rectGrid.GetWidth(), m_rectGrid.GetHeight(), &memDC,0,0);

    dYa = (sin(m_dAngle) * m_dMindB)*m_dResolution;
    dXa = (cos(m_dAngle) * m_dMindB)*m_dResolution;
    dc.SetPen(*wxWHITE_PEN);
    dc.DrawLine(m_pntCenter.x, m_pntCenter.y, m_pntCenter.x+dXa, m_pntCenter.y+dYa);


    m_dAngle += m_dAngleMod;

    m_dLevel = 0.0;
}



void RadarMeter::OnSize(wxSizeEvent& event)
{
    int nX = min(GetClientRect().GetWidth()-2, GetClientRect().GetHeight()-2);
    m_rectGrid = wxRect(5, 0, nX, nX);

    m_bmpScreen.Create(m_rectGrid.GetWidth()-2, m_rectGrid.GetHeight()-2);

    m_pntCenter = wxPoint(m_rectGrid.GetLeft() + m_rectGrid.GetWidth()/2 -7, m_rectGrid.GetTop() + m_rectGrid.GetHeight()/2);
    m_pntLast = m_pntCenter;
    m_pntLastBlack = m_pntCenter;
    m_pntLastFade = m_pntCenter;

    m_bmpBackground.Create(m_rectGrid.GetWidth()-2, m_rectGrid.GetHeight()-2);
    m_bmpFade.Create(m_rectGrid.GetWidth()-2, m_rectGrid.GetHeight()-2);

    CreateBackgroundBitmap();


    Refresh();
}


void RadarMeter::CreateBackgroundBitmap()
{
    wxMemoryDC memDC;

    memDC.SelectObject(m_bmpBackground);

    memDC.SetPen(*wxTRANSPARENT_PEN);

    memDC.SetBrush(*wxBLACK_BRUSH);
    memDC.DrawRectangle(0,0, m_bmpBackground.GetWidth(), m_bmpBackground.GetHeight());

    memDC.SetBrush(wxBrush(wxColour(wxT("#DA9107"))));
    memDC.DrawCircle(m_pntCenter, (m_dMindB-2)*m_dResolution);

    memDC.SetBrush(wxBrush(wxColour(wxT("#01FF3E"))));
    memDC.DrawCircle(m_pntCenter, (m_dMindB-10)*m_dResolution);

    memDC.SetBrush(wxBrush(wxColour(wxT("#01A836"))));
    memDC.DrawCircle(m_pntCenter, (m_dMindB-18)*m_dResolution);
    memDC.SetBrush(wxBrush(wxColour(wxT("#007473"))));
    memDC.DrawCircle(m_pntCenter, (m_dMindB-34)*m_dResolution);

    memDC.SetBrush(*wxTRANSPARENT_BRUSH);

    memDC.SetPen(wxPen(wxColour(wxT("#354e61"))));

    if(m_nMode != LevelCalculator::PPM)
    {
        for(int i = m_dMindB; i > 0; i-= 10)
        {
            memDC.DrawCircle(m_pntCenter, i*m_dResolution);
        }
    }
    else
    {
        for(size_t i = 0; i < 8; i++)
        {
            int nPPM = (i*4)-34;
            if(i == 4)
            {
                memDC.SetPen(wxPen(wxColour(wxT("#654e61"))));
            }
            else
            {
                memDC.SetPen(wxPen(wxColour(wxT("#354e61"))));
            }
            memDC.DrawCircle(m_pntCenter, (m_dMindB+nPPM)*m_dResolution);
        }
    }




    memDC.SelectObject(m_bmpFade);
    memDC.SetPen(*wxTRANSPARENT_PEN);
    memDC.SetBrush(*wxBLACK_BRUSH);
    memDC.DrawRectangle(0,0, m_bmpFade.GetWidth(), m_bmpFade.GetHeight());

    memDC.SetBrush(wxBrush(wxColour(wxT("#855804"))));
    memDC.DrawCircle(m_pntCenter, (m_dMindB-2)*m_dResolution);

    memDC.SetBrush(wxBrush(wxColour(wxT("#01811f"))));
    memDC.DrawCircle(m_pntCenter, (m_dMindB-10)*m_dResolution);

    memDC.SetBrush(wxBrush(wxColour(wxT("#01601f"))));
    memDC.DrawCircle(m_pntCenter, (m_dMindB-18)*m_dResolution);
    memDC.SetBrush(wxBrush(wxColour(wxT("#004948"))));
    memDC.DrawCircle(m_pntCenter, (m_dMindB-34)*m_dResolution);

    memDC.SetBrush(*wxTRANSPARENT_BRUSH);

    memDC.SetPen(wxPen(wxColour(wxT("#354e61"))));
    if(m_nMode != LevelCalculator::PPM)
    {
        for(int i = m_dMindB; i > 0; i-= 10)
        {
            memDC.DrawCircle(m_pntCenter, i*m_dResolution);
        }
    }
    else
    {
        for(size_t i = 0; i < 8; i++)
        {
            int nPPM = (i*4)-34;
            if(i == 4)
            {
                memDC.SetPen(wxPen(wxColour(wxT("#654e61"))));
            }
            else
            {
                memDC.SetPen(wxPen(wxColour(wxT("#354e61"))));
            }
            memDC.DrawCircle(m_pntCenter, (m_dMindB+nPPM)*m_dResolution);
        }
    }


}

void RadarMeter::SetMindB(float dMin)
{
    m_dMindB = dMin;
    m_dResolution = static_cast<double>((m_rectGrid.GetWidth()-4))/(m_dMindB*2.0);
}

void RadarMeter::SetRadarLevel(double dLevel, unsigned int nSamples, bool bInDBAlready)
{
    if(!bInDBAlready)
    {
        dLevel = 20*log10(dLevel);
    }

    dLevel = max(dLevel, -m_dMindB);

    m_dLevel = max(m_dLevel, -(-m_dMindB - dLevel));


//    m_nSamples += nSamples;
//    if(m_nSamples > m_nTimespan)
//    {
//        m_nSamples -= m_nTimespan;
//        Refresh();
//        Update();
//    }
}

void RadarMeter::OnTimer(wxTimerEvent& event)
{
    Refresh();
    Update();
}

void RadarMeter::OnLeftUp(wxMouseEvent& event)
{
    wxPostEvent(GetParent(), event);
}


void RadarMeter::SetTimespan(unsigned int nSeconds, bool bClearMeter)
{
    m_nTimespan = nSeconds*1000;

    //work out the angle mod
    m_dAngleMod = (M_PI*2)/static_cast<double>(m_nTimespan);
    m_dAngleMod *= static_cast<double>(m_nRefreshRate);

    m_nPoints = (M_PI*2)/m_dAngleMod;

    if(bClearMeter)
    {
        ClearMeter();
    }
}

void RadarMeter::SetSampleRate(unsigned int nSampleRate)
{
    m_nSampleRate = nSampleRate;
}

void RadarMeter::SetRefreshRate(unsigned int nMilliseconds)
{
    m_nRefreshRate = nMilliseconds;
    m_timerSecond.Stop();
    m_timerSecond.Start(m_nRefreshRate);

    SetTimespan(m_nTimespan/1000);  //set timespan to work out angle mode and clear meter...
}


void RadarMeter::ClearMeter()
{
    wxMemoryDC dc;
    dc.SelectObject(m_bmpScreen);
    dc.SetBrush(*wxBLACK_BRUSH);
    dc.DrawRectangle(0,0, m_bmpScreen.GetWidth(), m_bmpScreen.GetHeight());

    while(m_queueFade.empty() == false)
    {
        m_queueFade.pop();
    }
    while(m_queueLines.empty() == false)
    {
        m_queueLines.pop();
    }
    m_dAngle = 0.0;

    m_pntLast = m_pntCenter;
    m_pntLastBlack = m_pntCenter;
    m_pntLastFade = m_pntCenter;

}

void RadarMeter::SetMode(unsigned int nMode)
{
    m_nMode = nMode;
    if(m_nMode == LevelCalculator::PPM)
    {
        SetMindB(34.0);
    }
    else
    {
        SetMindB(70.0);
    }
    CreateBackgroundBitmap();
    ClearMeter();
}
