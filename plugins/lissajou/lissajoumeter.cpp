#include "lissajoumeter.h"
#include <wx/dc.h>
#include <wx/dcbuffer.h>
#include <wx/log.h>
#include <wx/image.h>
#include "settings.h"
#include "audio.h"
#include "jarvis.h"

using namespace std;

BEGIN_EVENT_TABLE(LissajouMeter, pmControl)
    EVT_PAINT(LissajouMeter::OnPaint)
    EVT_SIZE(LissajouMeter::OnSize)
    EVT_LEFT_UP(LissajouMeter::OnLeftUp)
END_EVENT_TABLE()

 wxIMPLEMENT_DYNAMIC_CLASS(LissajouMeter, pmControl);


const float LissajouMeter::LEVEL_PPM[7] = {0.031, 0.05, 0.079, 0.126, 0.199, 0.316, 0.501};
const wxString LissajouMeter::LABEL_SCALE[3] = {wxT("dB"), wxT("Linear"), wxT("Auto")};

LissajouMeter::LissajouMeter(wxWindow *parent, wxWindowID id, const wxPoint& pos, const wxSize& size) : pmControl()
{
    Create(parent, id, pos, size);
    m_bRotate = false;
    m_nScaling = SCALE_DB;
    m_bShowLevels = false;

    m_Compressor.initRuntime();
    m_Compressor.setRatio(0.1);
    m_Compressor.setThresh(-26.0);
    m_Compressor.setAttack(50.0);
    m_Compressor.setRelease(2000.0);
    //m_Compressor.setThresh(-30.0);

}

bool LissajouMeter::Create(wxWindow *parent, wxWindowID id, const wxPoint& pos, const wxSize& size)
{
    wxSize szInit(size);
    wxSize bestSize(600,481);
    if(size.x<=0)
        szInit.SetWidth(bestSize.x);
    if(size.y <= 0)
        szInit.SetHeight(bestSize.y);

    if(!wxWindow::Create(parent,id,pos,szInit,wxWANTS_CHARS, wxT("lissajou")))
        return false;

    SetMinSize(size);
    m_nType = STARS;

    SetBackgroundStyle(wxBG_STYLE_CUSTOM);

    m_nAxis[0] = 0;
    m_nAxis[1] = 1;
    m_nChannels = 2;

    int nX = min(GetClientRect().GetWidth()-2, GetClientRect().GetHeight()-2);
    m_rectGrid = wxRect(5, 0, nX, nX);

    SetMindB(-60.0);

    return true;
}

LissajouMeter::~LissajouMeter()
{

}


void LissajouMeter::OnPaint(wxPaintEvent& event)
{
    wxAutoBufferedPaintDC dc(this);
    wxBrush br(*wxBLACK);
    dc.SetBrush(br);
    dc.DrawRectangle(GetClientRect());
    dc.SetPen(wxColour(0,0,200));

    if(m_vBuffer.empty() == false)
    {
        if(m_nType == STARS)
        {
            DrawStars(dc);
        }
        if(m_nType == PHASEOMETER)
        {
            DrawHull(dc);
        }

    }

    m_uiScale.Draw(dc, LABEL_SCALE[m_nScaling], uiRect::BORDER_NONE);
    if(m_nScaling == SCALE_AUTO)
    {
        m_uiAuto.Draw(dc, wxString::Format(wxT("x %.1f"), (1.0/m_dAutoScale)), uiRect::BORDER_NONE);
    }


}

static inline wxRealPoint
wxRotatePoint(const wxRealPoint& p, double cos_angle, double sin_angle,
              const wxRealPoint& p0)
{
    return wxRealPoint(p0.x + (p.x - p0.x) * cos_angle - (p.y - p0.y) * sin_angle,
                       p0.y + (p.y - p0.y) * cos_angle + (p.x - p0.x) * sin_angle);
}

static inline wxRealPoint
wxRotatePoint(double x, double y, double cos_angle, double sin_angle,
              const wxRealPoint & p0)
{
    return wxRotatePoint (wxRealPoint(x,y), cos_angle, sin_angle, p0);
}

void LissajouMeter::Rotate(float &x, float &y, const wxPoint& pntCenter)
{
    double dAngle = 1.0/sqrt(2.0); // 0.6;

    wxRealPoint pntRotated = ::wxRotatePoint(y,x, /*x,y,*/ dAngle, dAngle, wxRealPoint(pntCenter.x, pntCenter.y));

    x = max(double(0), min(static_cast<double>(m_bmpScreen.GetWidth()-1), pntRotated.x));
    y = max(double(0), min(static_cast<double>(m_bmpScreen.GetHeight()-1), pntRotated.y));

}

void LissajouMeter::DrawLines(wxImage& img, const wxPoint& pntCenter)
{
    //draw the lines L+R, M+S
    for(int i = 0; i < m_bmpScreen.GetWidth(); i++)
    {
        if(i%2)
        {
            if(!m_bRotate)
            {
                //Mono
                img.GetData()[i*m_bmpScreen.GetWidth()*3+(i*3)] = 0x88;
                img.GetData()[i*m_bmpScreen.GetWidth()*3+(i*3)+1] = 0x88;
                img.GetData()[i*m_bmpScreen.GetWidth()*3+(i*3)+2] = 0x88;

                //Stereo
                int nY = (m_bmpScreen.GetWidth()-i);
                img.GetData()[nY*m_bmpScreen.GetWidth()*3+(i*3)] = 0x88;
                img.GetData()[nY*m_bmpScreen.GetWidth()*3+(i*3)+1] = 0x88;
                img.GetData()[nY*m_bmpScreen.GetWidth()*3+(i*3)+2] = 0x88;

                //Left
                if(i > 5 && i < m_bmpScreen.GetWidth()-5)
                {
                    img.GetData()[i*m_bmpScreen.GetWidth()*3+pntCenter.x*3] = 0x88;
                    img.GetData()[i*m_bmpScreen.GetWidth()*3+pntCenter.x*3+1] = 0x00;
                    img.GetData()[i*m_bmpScreen.GetWidth()*3+pntCenter.x*3+2] = 0x00;

                    //Right
                    img.GetData()[(pntCenter.y*m_bmpScreen.GetWidth())*3+i*3] = 0x00;
                    img.GetData()[(pntCenter.y*m_bmpScreen.GetWidth())*3+i*3+1] = 0x88;
                    img.GetData()[(pntCenter.y*m_bmpScreen.GetWidth())*3+i*3+2] = 0x00;
                }

            }
            else
            {
                //Mono
                img.GetData()[i*m_bmpScreen.GetWidth()*3+pntCenter.x*3] = 0x88;
                img.GetData()[i*m_bmpScreen.GetWidth()*3+pntCenter.x*3+1] = 0x88;
                img.GetData()[i*m_bmpScreen.GetWidth()*3+pntCenter.x*3+2] = 0x88;

                //Stereo
                img.GetData()[(pntCenter.y*m_bmpScreen.GetWidth())*3+i*3] = 0x88;
                img.GetData()[(pntCenter.y*m_bmpScreen.GetWidth())*3+i*3+1] = 0x88;
                img.GetData()[(pntCenter.y*m_bmpScreen.GetWidth())*3+i*3+2] = 0x88;

                //Left
                if(i > 5 && i < m_bmpScreen.GetWidth()-5)
                {
                    img.GetData()[i*m_bmpScreen.GetWidth()*3+(i*3)] = 0x88;
                    img.GetData()[i*m_bmpScreen.GetWidth()*3+(i*3)+1] = 0x00;
                    img.GetData()[i*m_bmpScreen.GetWidth()*3+(i*3)+2] = 0x00;

                    //Right
                    int nY = (m_bmpScreen.GetWidth()-i);
                    img.GetData()[nY*m_bmpScreen.GetWidth()*3+(i*3)] = 0x00;
                    img.GetData()[nY*m_bmpScreen.GetWidth()*3+(i*3)+1] = 0x88;
                    img.GetData()[nY*m_bmpScreen.GetWidth()*3+(i*3)+2] = 0x00;
                }
            }
        }
    }

    if(m_bShowLevels)
    {
        for(int i = 0; i < 7; i++)
        {
            for(double dRad = 0.0; dRad < 480; dRad+=1.0)
            {
                float x,y;
                double dSine = sin(dRad*M_PI/240.0)/0.707;
                double dCos = cos(dRad*M_PI/240.0)/0.707;

                Scale(dSine*LEVEL_PPM[i], dCos*LEVEL_PPM[i], x,y, pntCenter);

                if(m_bRotate)
                {
                    Rotate(x,y,pntCenter);
                }

                int nX(min(m_bmpScreen.GetWidth()-1, max(0, static_cast<int>(x))));
                int nY(min(m_bmpScreen.GetHeight()-1, max(0, static_cast<int>(y))));



                if(i != 3)
                {
                    img.GetData()[nY*m_bmpScreen.GetWidth()*3+(nX*3)] = 0x44;
                    img.GetData()[nY*m_bmpScreen.GetWidth()*3+(nX*3)+1] = 0x44;
                    img.GetData()[nY*m_bmpScreen.GetWidth()*3+(nX*3)+2] = 0x66;
                }
                else
                {
                    img.GetData()[nY*m_bmpScreen.GetWidth()*3+(nX*3)] = 0x22;
                    img.GetData()[nY*m_bmpScreen.GetWidth()*3+(nX*3)+1] = 0x22;
                    img.GetData()[nY*m_bmpScreen.GetWidth()*3+(nX*3)+2] = 0x99;
                }
            }
        }
    }
}

void LissajouMeter::Scale(float dSampleL, float dSampleR, float& x, float& y, const wxPoint& pntCenter)
{
    switch(m_nScaling)
    {
        case SCALE_DB:
            if(dSampleL == 0)
            {
                x = pntCenter.x;
            }
            else if(dSampleL < 0)
            {
                x = 20*log10(-dSampleL)*m_pairStep.first;
                x = min(-x, static_cast<float>(pntCenter.x));
            }
            else
            {
                x = 20*log10(dSampleL)*m_pairStep.first;
                x = max(m_bmpScreen.GetWidth()+x, static_cast<float>(pntCenter.x));
            }
            if(dSampleR == 0)
            {
                y = pntCenter.y;
            }
            else if(dSampleR < 0)
            {
                y = 20*log10(-dSampleR)*m_pairStep.second;
                y = min(-y, static_cast<float>(pntCenter.y));
            }
            else
            {
                y = 20*log10(dSampleR)*m_pairStep.second;
                y = max(m_bmpScreen.GetHeight()+y, static_cast<float>(pntCenter.y));
            }
            break;
        case SCALE_LINEAR:
        case SCALE_AUTO:
            x = max(min(static_cast<double>(m_bmpScreen.GetWidth()-1), (dSampleL*static_cast<float>(pntCenter.x)/m_dAutoScale)+static_cast<float>(pntCenter.x)),double(0));
            y = max(min(static_cast<double>(m_bmpScreen.GetHeight()-1), (dSampleR*static_cast<float>(pntCenter.y)/m_dAutoScale)+static_cast<float>(pntCenter.y)),double(0));
            break;
    }

}

void LissajouMeter::DrawStars(wxDC& dc)
{
    dc.SetPen(*wxGREEN_PEN);

    wxPoint pntCenter(m_bmpScreen.GetWidth()/2, m_bmpScreen.GetHeight()/2);

    wxImage img(m_bmpScreen.GetWidth(), m_bmpScreen.GetHeight());

    DrawLines(img, pntCenter);


    for(size_t nSample = 0; nSample < m_vBuffer.size(); nSample+=m_nChannels)
    {
        float x;
        float y;

        Scale(m_vBuffer[nSample+m_nAxis[1]], m_vBuffer[nSample+m_nAxis[0]], x, y, pntCenter);



       if(m_bRotate)
       {
           Rotate(x,y,pntCenter);
       }

        int nX(static_cast<int>(x));
        int nY(static_cast<int>(y));

        img.GetData()[nY*m_bmpScreen.GetWidth()*3+(nX*3)] = 0x00;
        img.GetData()[nY*m_bmpScreen.GetWidth()*3+(nX*3)+1] = 0xFF;
        img.GetData()[nY*m_bmpScreen.GetWidth()*3+(nX*3)+2] = 0x00;

    }
    m_pntRotateOffset = wxPoint(0,0);
    dc.DrawBitmap(wxBitmap(img),m_rectGrid.GetLeft()+1+m_pntRotateOffset.x, m_rectGrid.GetTop()+1+m_pntRotateOffset.y);

}

void LissajouMeter::DrawPeaks(wxDC&  dc)
{

    wxPoint pntCenter(m_bmpScreen.GetWidth()/2, m_bmpScreen.GetHeight()/2);

    pair<float,float> pairMidMin = make_pair(pntCenter.x, pntCenter.y);
    pair<float,float> pairMidMax = make_pair(pntCenter.x, pntCenter.y);
    pair<float,float> pairSideMin = make_pair(pntCenter.x, pntCenter.y);
    pair<float,float> pairSideMax = make_pair(pntCenter.x, pntCenter.y);


    for(size_t nSample = 0; nSample < m_vBuffer.size(); nSample +=m_nChannels)
    {
        float x;
        if(m_vBuffer[nSample+m_nAxis[0]] == 0)
        {
            x = pntCenter.x;
        }
        else if(m_vBuffer[nSample+m_nAxis[0]] < 0)
        {
            x = 20*log10(-m_vBuffer[nSample+m_nAxis[0]])*m_pairStep.first;
            x = min(-x, static_cast<float>(pntCenter.x));
        }
        else
        {
            x = 20*log10(m_vBuffer[nSample+m_nAxis[0]])*m_pairStep.first;
            x = max(m_bmpScreen.GetWidth()+x, static_cast<float>(pntCenter.x));
        }

        float y;
        if(m_vBuffer[nSample+m_nAxis[1]] == 0)
        {
            y = pntCenter.y;
        }
        else if(m_vBuffer[nSample+m_nAxis[1]] < 0)
        {
            y = 20*log10(-m_vBuffer[nSample+m_nAxis[1]])*m_pairStep.second;
            y = min(-y, static_cast<float>(pntCenter.y));
        }
        else
        {
            y = 20*log10(m_vBuffer[nSample+m_nAxis[1]])*m_pairStep.second;
            y = max(m_bmpScreen.GetHeight()+y, static_cast<float>(pntCenter.y));
        }

        if(x < pntCenter.x && y < pntCenter.y)
        {
            pairMidMin.first = min(pairMidMin.first, x);
            pairMidMin.second = min(pairMidMin.second, y);
        }
        else if(x > pntCenter.x && y > pntCenter.y)
        {
            pairMidMax.first = max(pairMidMax.first, x);
            pairMidMax.second = max(pairMidMax.second, y);
        }
        else if(x < pntCenter.x && y > pntCenter.y)
        {
            pairSideMin.first = min(pairSideMin.first, x);
            pairSideMin.second = max(pairSideMin.second, y);
        }
        else if(x > pntCenter.x && y < pntCenter.y)
        {
            pairSideMax.first = max(pairSideMax.first, x);
            pairSideMax.second = min(pairSideMax.second, y);
        }
    }

    float dMidValue = sqrt( pow((pairMidMax.first-pairMidMin.first),2)+ pow((pairMidMax.second-pairMidMin.second),2));
    float dSideValue = sqrt( pow((pairSideMax.first-pairSideMin.first),2)+ pow((pairSideMax.second-pairSideMin.second),2));


    if(m_bRotate)
    {
            Rotate(pairMidMin.first,pairMidMin.second,pntCenter);
            Rotate(pairMidMax.first,pairMidMax.second,pntCenter);
            Rotate(pairSideMin.first,pairSideMin.second,pntCenter);
            Rotate(pairSideMax.first,pairSideMax.second,pntCenter);
    }

    wxPoint pntPoly[4] = {wxPoint(pairMidMin.first, pairMidMin.second),
                          wxPoint(pairSideMin.first, pairSideMin.second),
                          wxPoint(pairMidMax.first, pairMidMax.second),
                          wxPoint(pairSideMax.first, pairSideMax.second)};


    if(m_nType == PHASEOMETER)
    {
        dc.SetPen(*wxGREEN_PEN);
        if(dMidValue > dSideValue)
        {
            dc.SetBrush(wxBrush(wxColour(0,150,0)));
        }
        else
        {
            dc.SetBrush(wxBrush(wxColour(150,0,0)));
        }

    }
    else
    {
        if(dMidValue > dSideValue)
        {
            dc.SetPen(wxPen(wxColour(120,120,140), 1, wxLONG_DASH));
        }
        else
        {
            dc.SetPen(wxPen(wxColour(255,100,100), 1, wxLONG_DASH));
        }


        dc.SetBrush(*wxTRANSPARENT_BRUSH);

    }
    dc.DrawPolygon(4, pntPoly);
}


void LissajouMeter::DrawHull(wxDC& dc)
{
    wxPoint pntCenter(m_bmpScreen.GetWidth()/2, m_bmpScreen.GetHeight()/2);
    wxImage img(m_bmpScreen.GetWidth(), m_bmpScreen.GetHeight());
    DrawLines(img, pntCenter);

    vector<wxPoint> pntList(m_vBuffer.size()/m_nChannels, pntCenter);

    size_t i = 0;
    for(size_t nSample = 0; nSample < m_vBuffer.size(); nSample+=m_nChannels)
    {
        float x;
        float y;

        Scale(m_vBuffer[nSample+m_nAxis[1]], m_vBuffer[nSample+m_nAxis[0]], x, y, pntCenter);

       if(m_bRotate)
       {
           Rotate(x,y,pntCenter);
       }

        int nX(static_cast<int>(x));
        int nY(static_cast<int>(y));

        pntList[i] = wxPoint(m_rectGrid.GetLeft()+1+nX,m_rectGrid.GetTop()+1+nY);
        i++;

        //img.GetData()[nY*m_bmpScreen.GetWidth()*3+(nX*3)] = 0x00;
        //img.GetData()[nY*m_bmpScreen.GetWidth()*3+(nX*3)+1] = 0xFF;
        //img.GetData()[nY*m_bmpScreen.GetWidth()*3+(nX*3)+2] = 0x00;

    }
    m_pntRotateOffset = wxPoint(0,0);
    dc.DrawBitmap(wxBitmap(img),m_rectGrid.GetLeft()+1+m_pntRotateOffset.x, m_rectGrid.GetTop()+1+m_pntRotateOffset.y);

    dc.SetBrush(*wxTRANSPARENT_BRUSH);
    dc.SetPen(wxPen(wxColour(50,250,50)));
    vector<wxPoint> pntHull(convexHull(&pntList[0], pntList.size()));
    dc.DrawPolygon(pntHull.size(), &pntHull[0]);
}

void LissajouMeter::OnSize(wxSizeEvent& event)
{
    int nX = min(GetClientRect().GetWidth()-2, GetClientRect().GetHeight()-2);
    m_rectGrid = wxRect(5, 0, nX, nX);

    SetMindB(m_dMindB);

    m_bmpScreen.Create(m_rectGrid.GetWidth()-2, m_rectGrid.GetHeight()-2);

    m_uiScale.SetRect(m_rectGrid.GetRight()+5, 1, GetClientRect().GetRight()-m_rectGrid.GetRight()-10, 25);
    m_uiScale.SetBackgroundColour(*wxBLACK);
    m_uiAuto.SetRect(m_rectGrid.GetRight()+5, 30, GetClientRect().GetRight()-m_rectGrid.GetRight()-10, 25);
    m_uiAuto.SetBackgroundColour(*wxBLACK);
    Refresh();
}


void LissajouMeter::SetMindB(float dMin)
{
    m_pairStep.first = static_cast<float>(m_rectGrid.GetWidth())/(-dMin*2.0);
    m_pairStep.second = static_cast<float>(m_rectGrid.GetHeight())/(-dMin*2.0);
    m_dMindB = dMin;
}

void LissajouMeter::SetLissajouData(const std::vector<float>& vBuffer)
{
    if(m_nChannels != 0)
    {
        m_vBuffer = vBuffer;


        if(m_nScaling == SCALE_AUTO)
        {
            double dMax[2] = {0.0,0.0};
            for(int i = 0; i < m_vBuffer.size(); i+=m_nChannels)
            {
                dMax[0] = max((double)dMax[0], (double)fabs(m_vBuffer[i+m_nAxis[0]]));
                dMax[1] = max((double)dMax[1], (double)fabs(m_vBuffer[i+m_nAxis[1]]));
            }
            m_dAutoScale = m_Compressor.getRatio()/m_Compressor.process(dMax[0],dMax[1]);
        }
        else
        {
            m_dAutoScale = 1.0;
        }

        Refresh();
    }
}


void LissajouMeter::SetNumberOfChannels(unsigned int nChannels)
{
    m_nChannels = nChannels;
}

void LissajouMeter::SetAxis(unsigned int nX, unsigned int nY)
{
    m_nAxis[0] = nX;
    m_nAxis[1] = nY;
}


void LissajouMeter::OnLeftUp(wxMouseEvent& event)
{
    wxPostEvent(GetParent(), event);
}
