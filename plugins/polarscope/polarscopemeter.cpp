#include "polarscopemeter.h"
#include <wx/dc.h>
#include <wx/dcbuffer.h>
#include <wx/log.h>
#include <wx/image.h>
#include "levelcalculator.h"
#include "timedbuffer.h"

using namespace std;

BEGIN_EVENT_TABLE(PolarScope, wxWindow)
    EVT_PAINT(PolarScope::OnPaint)
    EVT_SIZE(PolarScope::OnSize)
    EVT_LEFT_UP(PolarScope::OnLeftUp)
END_EVENT_TABLE()



PolarScope::PolarScope(wxWindow *parent, wxWindowID id, const wxPoint& pos, const wxSize& size) : m_dMindB(70.0), m_pBuffer(0), m_nBufferSize(0)
{
    Create(parent, id, pos, size);
    #ifndef __WXMSW__
    SetCursor(wxCURSOR_BLANK);
    #endif // __WXMSW__

    SetForegroundColour(*wxWHITE);
    SetFont(wxFont(8,wxFONTFAMILY_SWISS,wxFONTSTYLE_NORMAL,wxFONTWEIGHT_NORMAL,false,_T("Arial"),wxFONTENCODING_DEFAULT));

}

bool PolarScope::Create(wxWindow *parent, wxWindowID id, const wxPoint& pos, const wxSize& size)
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

    m_nInputChannels = 2;
    m_nAxisX = 0;
    m_nAxisY = 1;

    CreateRects();



    return true;
}

PolarScope::~PolarScope()
{
    if(m_pBuffer)
    {
        delete m_pBuffer;
    }
}


void PolarScope::OnPaint(wxPaintEvent& event)
{
    wxAutoBufferedPaintDC dc(this);

    dc.SetPen(*wxTRANSPARENT_PEN);
    dc.SetBrush(*wxBLACK_BRUSH);
    dc.DrawRectangle(GetClientRect());


    dc.SetClippingRegion(m_rectGrid);


    dc.SetBrush(*wxTRANSPARENT_BRUSH);
    dc.SetPen(wxPen(wxColour(82,90,61)));
    dc.DrawCircle(m_pntPole, m_dResolution*m_dMindB);

    dc.SetPen(wxPen(wxColour(82,110,61)));

    double dCoordX = cos(M_PI_4)*(m_dMindB*m_dResolution);
    double dCoordY = sin(M_PI_4)*(m_dMindB*m_dResolution);

    dc.DrawLine(m_pntPole.x, m_pntPole.y, m_pntPole.x+dCoordX, m_pntPole.y-dCoordY);
    dc.DrawLine(m_pntPole.x, m_pntPole.y, m_pntPole.x-dCoordX, m_pntPole.y-dCoordY);
    dc.DrawLine(m_rectGrid.GetLeft(), m_pntPole.y, m_rectGrid.GetRight(), m_pntPole.y);

    DrawPoints(dc);
    //DrawLevels(dc);

    dc.DestroyClippingRegion();

    dc.SetPen(wxPen(wxColour(82,110,61)));
    dc.DrawRectangle(m_rectCorrelation);
    dc.DrawRectangle(m_rectCorrelation.GetLeft()+m_dCorrelation, m_rectCorrelation.GetTop(), 2, m_rectCorrelation.GetHeight());

    m_uiCorrelation.Draw(dc, wxString::Format(wxT("%.2f"), m_dCorrelation), uiRect::BORDER_NONE);

}

void PolarScope::DrawPoints(wxDC& dc)
{
    if(m_pBuffer)
    {
        dc.SetPen(wxColour(50,255,50));
        wxPoint pntOld(m_pntPole);
        for(size_t i = 0; i < m_nBufferSize; i+=m_nInputChannels)
        {
            float dX = m_pBuffer[i+m_nAxisX];
            float dY = m_pBuffer[i+m_nAxisY];
            float dHeight = sqrt(dX*dX + dY*dY);
            dHeight =  max(double(0.0), m_dMindB + 20*log10(dHeight))*m_dResolution;

            if(dY != 0.0 && dX != 0.0)
            {
                double dAngle = atan(dX/dY);//-atan(1.0);
                dAngle -= M_PI_4;


                double dCoordX = sin(dAngle)*dHeight;
                double dCoordY = cos(dAngle)*dHeight;
                if(dCoordY < 0)
                {
                    dCoordX = -dCoordX;
                    dCoordY = -dCoordY;
                }


                dc.DrawPoint(m_pntPole.x+dCoordX, m_pntPole.y-dCoordY);

                m_dCorrelation = cos(dAngle);
            }
        }

    }

}


void PolarScope::DrawLevels(wxDC& dc)
{

    rgb rgb_pen;
    rgb_pen.r =50.0/255.0;
    rgb_pen.g = 255.0/255.0;
    rgb_pen.b = 50.0/255.0;
    hsv hsv_pen = rgb2hsv(rgb_pen);



    wxPoint pntOld(m_pntPole);
    for(list<pair<double, double> >::reverse_iterator itLine = m_lstLevels.rbegin(); itLine != m_lstLevels.rend(); ++itLine)
    {
        rgb_pen = hsv2rgb(hsv_pen);
        dc.SetPen(wxColour(rgb_pen.r*255.0, rgb_pen.g*255.0, rgb_pen.b*255.0));

        hsv_pen.v -= 0.001;

        dc.DrawPoint(m_pntPole.x+(*itLine).first, m_pntPole.y-(*itLine).second);

    }
}

void PolarScope::OnSize(wxSizeEvent& event)
{
    CreateRects();
    Refresh();
}

void PolarScope::CreateRects()
{
    m_rectGrid = wxRect(5, 5, GetClientRect().GetWidth()-10, GetClientRect().GetHeight()-100);
    m_rectCorrelation = wxRect(5, m_rectGrid.GetBottom()+10, m_rectGrid.GetWidth()-10, 20);

    m_uiCorrelation.SetRect(wxRect(5, m_rectCorrelation.GetBottom()+5, m_rectCorrelation.GetWidth(), 30));
    m_pntPole = wxPoint(m_rectGrid.GetLeft() + (m_rectGrid.GetWidth()/2), m_rectGrid.GetBottom());

    m_dResolution = static_cast<double>((m_rectGrid.GetWidth()-4))/(m_dMindB*2.0);
    m_dResolutionCorrelation = static_cast<double>((m_rectCorrelation.GetWidth()))/(2.0);

    Refresh();
}



void PolarScope::OnLeftUp(wxMouseEvent& event)
{
    wxPostEvent(GetParent(), event);
}


void PolarScope::ClearMeter()
{
}


void PolarScope::SetAudioData(const timedbuffer* pBuffer)
{
    if(m_pBuffer)
    {
        delete[] m_pBuffer;
        m_pBuffer = 0;
    }
    m_nBufferSize = (pBuffer->GetBufferSize());
    m_pBuffer = new float[m_nBufferSize];
    memcpy(m_pBuffer, pBuffer->GetBuffer(), m_nBufferSize*sizeof(float));

    WorkoutLevel();

    Refresh();
}

void PolarScope::SetNumberOfInputChannels(unsigned int nInputChannels)
{
    m_nInputChannels = nInputChannels;
}

void PolarScope::SetAxisX(unsigned int nChannel)
{
    m_nAxisX = nChannel;
}

void PolarScope::SetAxisY(unsigned int nChannel)
{
    m_nAxisY = nChannel;
}


void PolarScope::WorkoutLevel()
{
    float dAngle(0.0);
    float dHeight(0.0);
    for(size_t i = 0; i < m_nBufferSize; i+=m_nInputChannels)
    {
        float dX = m_pBuffer[i+m_nAxisX];
        float dY = m_pBuffer[i+m_nAxisY];
        float dHeight_ = sqrt(dX*dX + dY*dY);
        dHeight +=  max(double(0.0), m_dMindB + 20*log10(dHeight_))*m_dResolution;

        if(dY != 0.0 && dX != 0.0)
        {
            double dAngle_ = atan(dX/dY);//-atan(1.0);
            dAngle_ -= M_PI_4;
            dAngle += dAngle_;

        }
    }

    //average
    dHeight /= m_nBufferSize;
    dAngle /= m_nBufferSize;

    double dCoordX = sin(dAngle)*dHeight;
    double dCoordY = cos(dAngle)*dHeight;
    if(dCoordY < 0)
    {
        dCoordX = -dCoordX;
        dCoordY = -dCoordY;
    }


    m_lstLevels.push_back(make_pair(dCoordX, dCoordY));

    if(m_lstLevels.size() > 1000)
    {
        m_lstLevels.pop_front();
    }

}





hsv PolarScope::rgb2hsv(rgb in)
{
    hsv         out;
    double      min, max, delta;

    min = in.r < in.g ? in.r : in.g;
    min = min  < in.b ? min  : in.b;

    max = in.r > in.g ? in.r : in.g;
    max = max  > in.b ? max  : in.b;

    out.v = max;                                // v
    delta = max - min;
    if (delta < 0.00001)
    {
        out.s = 0;
        out.h = 0; // undefined, maybe nan?
        return out;
    }
    if( max > 0.0 ) { // NOTE: if Max is == 0, this divide would cause a crash
        out.s = (delta / max);                  // s
    } else {
        // if max is 0, then r = g = b = 0
        // s = 0, h is undefined
        out.s = 0.0;
        out.h = NAN;                            // its now undefined
        return out;
    }
    if( in.r >= max )                           // > is bogus, just keeps compilor happy
        out.h = ( in.g - in.b ) / delta;        // between yellow & magenta
    else
    if( in.g >= max )
        out.h = 2.0 + ( in.b - in.r ) / delta;  // between cyan & yellow
    else
        out.h = 4.0 + ( in.r - in.g ) / delta;  // between magenta & cyan

    out.h *= 60.0;                              // degrees

    if( out.h < 0.0 )
        out.h += 360.0;

    return out;
}


rgb PolarScope::hsv2rgb(hsv in)
{
    double      hh, p, q, t, ff;
    long        i;
    rgb         out;

    if(in.s <= 0.0) {       // < is bogus, just shuts up warnings
        out.r = in.v;
        out.g = in.v;
        out.b = in.v;
        return out;
    }
    hh = in.h;
    if(hh >= 360.0) hh = 0.0;
    hh /= 60.0;
    i = (long)hh;
    ff = hh - i;
    p = in.v * (1.0 - in.s);
    q = in.v * (1.0 - (in.s * ff));
    t = in.v * (1.0 - (in.s * (1.0 - ff)));

    switch(i) {
    case 0:
        out.r = in.v;
        out.g = t;
        out.b = p;
        break;
    case 1:
        out.r = q;
        out.g = in.v;
        out.b = p;
        break;
    case 2:
        out.r = p;
        out.g = in.v;
        out.b = t;
        break;

    case 3:
        out.r = p;
        out.g = q;
        out.b = in.v;
        break;
    case 4:
        out.r = t;
        out.g = p;
        out.b = in.v;
        break;
    case 5:
    default:
        out.r = in.v;
        out.g = p;
        out.b = q;
        break;
    }
    return out;
}
