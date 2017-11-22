#include "anglemeter.h"
#include <wx/busyinfo.h>
#include <wx/dcbuffer.h>
#include <list>
#include <wx/dcmemory.h>
#include <algorithm>
#include "meter.h"

using namespace std;

// AngleMeter
BEGIN_EVENT_TABLE(AngleMeter, wxWindow)
END_EVENT_TABLE()

AngleMeter::AngleMeter()
    : m_dMin(-70)
    , m_dMax(0)
    , m_nMeterDisplay(PEAK)
    , m_pPPM(0)
    , m_pLoud(0)
{
    //create our font
    Connect(wxEVT_PAINT, (wxObjectEventFunction)&AngleMeter::OnPaint);
    Connect(wxEVT_SIZE, (wxObjectEventFunction)&AngleMeter::OnSize);
    m_clrText = wxColour(200,180,255);
}

AngleMeter::AngleMeter(wxWindow *parent, wxWindowID id, const wxString & sText,double dMin, unsigned int nRouting, const wxPoint& pos, const wxSize& size) :
    m_dMax(0),
    m_nMeterDisplay(PEAK),
    m_pPPM(0),
    m_pLoud(0)
{
    m_dLastValue[0] = -180;
    m_dLastValue[1] = -180;
    wxSize szInit(size);
    wxSize bestSize = DoGetBestSize();
    if(size.x<=0)
        szInit.SetWidth(bestSize.x);
    if(size.y <= 0)
        szInit.SetHeight(bestSize.y);


    wxWindow::Create(parent,id,pos,szInit,wxWANTS_CHARS, wxT("AngleMeter"));

#ifndef __WXMSW__
    SetCursor(wxCURSOR_BLANK);
#endif // __WXMSW__
    SetMinSize(size);

    m_nPeakMode = PEAK_SHOW;
    m_bFreeze = false;

    SetBackgroundStyle(wxBG_STYLE_CUSTOM);

    SetMeterSpeed(meter::NORMAL);
    SetMeterMSMode(meter::M6);

    m_nRouting = nRouting;

    Connect(wxEVT_PAINT, (wxObjectEventFunction)&AngleMeter::OnPaint);
    Connect(wxEVT_SIZE, (wxObjectEventFunction)&AngleMeter::OnSize);
    m_clrText = wxColour(200,180,255);

    m_clrMeter[0] = *wxRED;
    m_clrMeter[1] = *wxGREEN;

    InitMeter(sText, dMin);

    SetMeterDisplay(m_nMeterDisplay);
}

AngleMeter::~AngleMeter()
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



void AngleMeter::OnPaint(wxPaintEvent& event)
{
    wxAutoBufferedPaintDC dc(this);


    dc.SetTextForeground(*wxWHITE);
    dc.SetFont(GetFont());
    wxBrush br(*wxBLACK);
    dc.SetPen(*wxWHITE_PEN);
    dc.SetBrush(br);
    dc.DrawRectangle(GetClientRect());

    wxPoint pntBottom(m_rectGrid.GetLeft() + m_rectGrid.GetWidth()/2, m_rectGrid.GetBottom()+50);
    double dH = m_rectGrid.GetHeight()+20;
    double dHT = m_rectGrid.GetHeight()+30;
    double dHL = m_rectGrid.GetHeight()+10;


    //Draw the text
    for(int i = 1; i < 8; i++)
    {
        double dAngle = (15.0*(i-4.0))*M_PI/180.0;
        double dAngleDeg = (15.0*(i-4.0));
        double dX = dHT*sin(dAngle);
        double dXTo = dHL*sin(dAngle);
        double dY = dHT*cos(dAngle);
        double dYTo = dHL*cos(dAngle);

        double dTextX = (dHT+15.0)*sin(dAngle);
        double dTextY = (dHT+15.0)*cos(dAngle);

        dc.SetPen(wxPen(wxColour(120,120,120),1, wxDOT));
        dc.DrawLine(pntBottom.x+dXTo, pntBottom.y-dYTo, pntBottom.x+static_cast<int>(dX), pntBottom.y-static_cast<int>(dY));

        switch(m_nMeterDisplay)
        {
            case PPM:
                dc.DrawRotatedText(wxString::Format(wxT("%d"),i), wxPoint(pntBottom.x+static_cast<int>(dTextX)-5, pntBottom.y-static_cast<int>(dTextY)), -dAngleDeg);
                break;
            default:
                dc.DrawRotatedText(wxString::Format(wxT("%d"),((i-4)*4)-18), wxPoint(pntBottom.x+static_cast<int>(dTextX), pntBottom.y-static_cast<int>(dTextY)), -dAngleDeg);
                break;
        }
    }

    m_uiType.Draw(dc, uiRect::BORDER_NONE);

    dc.SetClippingRegion(m_rectGrid);
    for(int i = 1; i >= 0; i--)
    {
        //draw the needles
        double dX = dH*sin(m_dAngle[i]);
        double dY = dH*cos(m_dAngle[i]);
        dc.SetPen(wxPen(m_clrMeter[i],3,wxSOLID));
        dc.DrawLine(pntBottom.x, pntBottom.y, pntBottom.x+static_cast<int>(dX), pntBottom.y-static_cast<int>(dY));

        //draw the peaks
        if(m_nPeakMode != PEAK_HIDE)
        {
            dX = dHT*sin(m_dAngleMax[i]);
            double dXTo = dHL*sin(m_dAngleMax[i]);
            dY = dHT*cos(m_dAngleMax[i]);
            double dYTo = dHL*cos(m_dAngleMax[i]);
            dc.DrawLine(pntBottom.x+dXTo, pntBottom.y-dYTo, pntBottom.x+static_cast<int>(dX), pntBottom.y-static_cast<int>(dY));
        }
    }
    dc.DestroyClippingRegion();

    m_uiLevelText[0].Draw(dc, uiRect::BORDER_DOWN);
    m_uiLevelText[1].Draw(dc, uiRect::BORDER_DOWN);


}


void AngleMeter::InitMeter(const wxString& sText,double dMin)
{
    m_dMin = dMin;
    m_dPeakValue[0] = m_dMin;
    m_dPeakValue[1] = m_dMin;

    //work out size of rects
    wxString sLevel;

    m_uiLabel.SetLabel(sText);

    m_uiLabel.SetRect(0,GetClientRect().GetBottom()-20, GetClientRect().GetWidth(), 20);


    m_rectGrid = wxRect(1, 1, GetClientSize().x-2, GetClientSize().y-25);

    m_uiType.SetRect(m_rectGrid.GetRight()-80, m_rectGrid.GetBottom()-40, 70, 30);
    m_uiType.SetBackgroundColour(*wxBLACK);

    m_uiLevelText[0].SetRect(5,GetClientRect().GetBottom()-23, m_rectGrid.GetWidth()/2-5, 20);
    m_uiLevelText[1].SetRect(m_rectGrid.GetWidth()/2+5,GetClientRect().GetBottom()-23, m_rectGrid.GetWidth()/2-7, 20);

}

bool AngleMeter::SetMeterColour(const wxColour& clrMeter1, const wxColour& clrMeter2)
{

    m_clrMeter[0] = clrMeter1;
    m_clrMeter[1] = clrMeter2;

    return true;

}
void AngleMeter::ResetMeter(void)
{
    m_nPeakCounter[0] = 0;
    m_nPeakCounter[1] = 0;

    switch(m_nMeterDisplay)
    {
        case PEAK:
        case ENERGY:
        case TOTAL:
        case AVERAGE:
            m_dLastValue[0] = 0;
            m_dLastValue[1] = 0;
            m_dPeakValue[0] = -80.0;
            m_dPeakValue[1] = -80.0;
            ShowValue(m_dLastValue);
            break;
        case PPM:
            if(m_pPPM && m_nRouting == MONO_STEREO)
            {
                m_pPPM->setMode(m_nMeterMSMode);
            }
            else if(m_pPPM)
            {
                m_pPPM->setMode(meter::AB);
            }
            m_dLastValue[0] = 0.0;
            m_dLastValue[1] = 0.0;
            m_dPeakValue[0] = 0.0;
            m_dPeakValue[1] = 0.0;
            ShowValue(m_dLastValue, true);
            break;
        case LOUD:
            if(m_pLoud && m_nRouting == MONO_STEREO)
            {
                m_pLoud->setMode(m_nMeterMSMode);
            }
            else if(m_pLoud)
            {
                m_pLoud->setMode(meter::AB);
            }
            m_dLastValue[0] = -80.0;
            m_dLastValue[1] = -80.0;
            m_dPeakValue[0] = -80.0;
            m_dPeakValue[1] = -80.0;
            ShowValue(m_dLastValue, true);
            break;
    }


}
void AngleMeter::ShowValue(double dValue[2], bool bdB)
{
    for(int i = 0; i < 2; i++)
    {
        if(!m_bFreeze)
        {
            if(!bdB)
            {
                if(dValue[i] != 0)
                {
                    dValue[i] = 20*log10(dValue[i]);
                }
                else
                {
                    dValue[i] = -1e10;
                }
            }
            if(dValue[i] > m_dLastValue[i]-m_dFall || m_nMeterDisplay == PPM || m_nMeterDisplay == LOUD)
            {
                m_dLastValue[i] = dValue[i];
            }
            else
            {
                m_dLastValue[i] -= m_dFall;
            }
        }
        if(m_nPeakMode == PEAK_SHOW)
        {
            m_nPeakCounter[i]++;
        }
        if(dValue[i] >= m_dPeakValue[i] || m_nPeakCounter[i] >= 96)
        {
            m_dPeakValue[i] = min(dValue[i], m_dMax);
            m_nPeakCounter[i] = 0;
        }

        WorkoutAngles(i);

        m_uiLevelText[i].SetLabel(wxString::Format(wxT("%.1f [%.1f]"), m_dLastValue[i], m_dPeakValue[i]));
    }
    Refresh();
}

void AngleMeter::WorkoutAngles(int i)
{
    // convert the reading to an angle on the scale
    switch (m_nMeterDisplay)
    {
        case PPM:
            m_dAngle[i] = (15.0*(m_dLastValue[i]-4.0));
            m_dAngleMax[i] = (15.0*(m_dPeakValue[i]-4.0));
            break;
        default:
            // Add 18 to put -18 at 0 degrees.
            // Divide by 4 because it is 4dB per 15degree interval.
            m_dAngle[i] = (15.0*(m_dLastValue[i]+18.0)/4.0);
            m_dAngleMax[i] = (15.0*(m_dPeakValue[i]+18.0)/4.0);
            break;
    }
    m_dAngle[i] = max(-55.0, min(55.0, m_dAngle[i]))*M_PI/180.0;
    m_dAngleMax[i] = max(-55.0, min(55.0, m_dAngleMax[i]))*M_PI/180.0;

}

void AngleMeter::ShowPeak(const float* pBuffer, unsigned int nFrameCount)
{
    pair<float, float> pairPeak(GetPeak(pBuffer, nFrameCount));
    double dValue[2] = {pairPeak.first, pairPeak.second};
    ShowValue(dValue);
}

pair<float, float> AngleMeter::GetPeak(const float* pBuffer, unsigned int nFrameCount)
{
    //find largest value
    pair<float, float> pairPeak = make_pair(m_dMin, m_dMin);
    unsigned int i = 0;
    switch(m_nRouting)
    {
    case LEFT_RIGHT:
        for(unsigned int i=0; i < nFrameCount*2; i+=2)
        {
            float dSample[2] = {fabs(pBuffer[i]), fabs(pBuffer[i+1])};

            pairPeak.first = max(pairPeak.first,dSample[0]);
            pairPeak.second = max(pairPeak.second,dSample[1]);
        }
        break;
    case MONO_STEREO:
        for(unsigned int i=0; i < nFrameCount*2; i+=2)
        {
            float dSampleMono(fabs(pBuffer[i]+pBuffer[i+1]));
            float dSampleStereo(fabs(pBuffer[i]-pBuffer[i+1]));

            if(m_nMeterMSMode == meter::M6)
            {
                dSampleMono*=0.5;
                dSampleStereo*=0.5;
            }
            else
            {
                dSampleMono*=0.707;
                dSampleStereo*=0.707;
            }

            pairPeak.first = max(pairPeak.first,dSampleMono);
            pairPeak.second = max(pairPeak.second,dSampleStereo);
        }
        break;
    }
    return pairPeak;
}

void AngleMeter::ShowEnergy(const float* pBuffer, unsigned int nFrameCount)
{
    double dTotal[2] = {0.0,0.0};
    unsigned int i = 0;
    switch(m_nRouting)
    {
    case LEFT_RIGHT:
        for(unsigned int i=0; i < nFrameCount*2; i+=2)
        {
            dTotal[0] += pow(pBuffer[i],2);
            dTotal[1] += pow(pBuffer[i+1],2);
        }
        break;
    case MONO_STEREO:
        for(unsigned int i=0; i < nFrameCount*2; i+=2)
        {
            if(m_nMeterMSMode == meter::M6)
            {
                dTotal[0] += pow((pBuffer[i]+pBuffer[i+1])/2,2);
                dTotal[1] += pow((pBuffer[i]-pBuffer[i+1])/2,2);
            }
            else
            {
                dTotal[0] += pow((pBuffer[i]+pBuffer[i+1]),2);
                dTotal[1] += pow((pBuffer[i]-pBuffer[i+1]),2);
            }

        }
        break;
    }

    dTotal[0] = sqrt(dTotal[0]/nFrameCount);
    dTotal[1] = sqrt(dTotal[1]/nFrameCount);
    ShowValue(dTotal);
}

void AngleMeter::OnSize(wxSizeEvent& event)
{
    InitMeter(m_uiLabel.GetLabel(), m_dMin);

    Refresh();
}


void AngleMeter::ShowMeter(const float* pBuffer, unsigned int nFrameCount)
{
    switch(m_nMeterDisplay)
    {
    case PEAK:
        ShowPeak(pBuffer, nFrameCount);
        break;
    case ENERGY:
        ShowEnergy(pBuffer, nFrameCount);
        break;
    case PPM:
        ShowPPM(pBuffer, nFrameCount);
        break;
    case LOUD:
        ShowLUFS(pBuffer, nFrameCount);
    }
}

void AngleMeter::SetRouting(short nRouting)
{
    m_nRouting = nRouting;
    ResetMeter();
}

void AngleMeter::SetMeterDisplay(short nDisplay)
{
    m_nMeterDisplay = nDisplay;
    switch(m_nMeterDisplay)
    {
        case PPM:
            m_dMax = 7.0;
            m_uiType.SetLabel(wxT("PPM"));
            break;
        case PEAK:
            m_dMax = 0.0;
            m_uiType.SetLabel(wxT("dB Peak"));
            break;
        case ENERGY:
            m_dMax = 0.0;
            m_uiType.SetLabel(wxT("dB Energy"));
            break;
        case LOUD:
            m_dMax = 0.0;
            m_uiType.SetLabel(wxT("LKFS"));
            break;
    }

    if(m_nMeterDisplay != PPM && m_pPPM)
    {
        delete m_pPPM;
        m_pPPM = 0;
    }
    if(m_nMeterDisplay != LOUD && m_pLoud)
    {
        delete m_pLoud;
        m_pLoud = 0;
    }

    if(m_nMeterDisplay == PPM && !m_pPPM)
    {
        m_pPPM = new ppm(2);
        if(m_nRouting == LEFT_RIGHT)
        {
            m_pPPM->setMode(meter::AB);
        }
        else
        {
            m_pPPM->setMode(m_nMeterMSMode);
        }

        m_pPPM->setSpeed(m_nMeterSpeed);
    }
    else if(m_nMeterDisplay == LOUD && !m_pLoud)
    {
        m_pLoud = new loud(2);
        m_pLoud->setIntegrationTime(8);

        if(m_nRouting == LEFT_RIGHT)
        {
            m_pLoud->setMode(meter::AB);
        }
        else
        {
            m_pLoud->setMode(m_nMeterMSMode);
        }
        m_pLoud->setSpeed(m_nMeterSpeed);
    }
    ResetMeter();
}


void AngleMeter::SetLevels(const double dLevels[], size_t nSize)
{
    m_vLevels.clear();
    m_vLevels.resize(nSize);
    for(size_t i = 0 ; i < nSize; i++)
    {
        m_vLevels[i] = dLevels[i];
    }
    RefreshRect(m_uiLabel.GetRect());
}

void AngleMeter::SetLabel(const wxString& sLabel)
{
    m_uiLabel.SetLabel(sLabel);
    RefreshRect(m_uiLabel.GetRect());
}


void AngleMeter::SetPeakMode(int nMode)
{
    m_nPeakMode = nMode;
}

void AngleMeter::FreezeMeter(bool bFreeze)
{
    m_bFreeze = bFreeze;
}


void AngleMeter::ShowPPM(const float* pBuffer, unsigned int nFrameCount)
{
    double dValue[2] = {0,0};
    if(m_pPPM)
    {
        m_pPPM->calcIntermediate(2, nFrameCount, pBuffer);

        dValue[0] = m_pPPM->getValue(0);
        dValue[1] = m_pPPM->getValue(1);

        ShowValue(dValue, true);
    }
}


void AngleMeter::ShowLUFS(const float* pBuffer, unsigned int nFrameCount)
{
    double dValue[2] = {0,0};
    double dMaxValue[2] = {0,0};
    if(m_pLoud)
    {
        m_pLoud->calcIntermediate(2, nFrameCount, pBuffer);

        dValue[0] = m_pLoud->getValue(0);
        dValue[1] = m_pLoud->getValue(1);

        ShowValue(dValue,true);
    }
}

void AngleMeter::SetMeterMSMode(long nMode)
{
    m_nMeterMSMode = nMode;
    if(m_nRouting != LEFT_RIGHT)
    {
        if(m_pPPM)
        {
            m_pPPM->setMode(nMode);
        }
        else if(m_pLoud)
        {
            m_pLoud->setMode(nMode);
        }
    }
}

void AngleMeter::SetMeterSpeed(long nSpeed)
{
    m_nMeterSpeed = nSpeed;
    if(m_pPPM)
    {
        m_pPPM->setSpeed(nSpeed);
    }
    else if(m_pLoud)
    {
        m_pLoud->setSpeed(nSpeed);
    }

    switch(m_nMeterSpeed)
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
