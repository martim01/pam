#include "anglemeter.h"
#include <wx/busyinfo.h>
#include <wx/dcbuffer.h>
#include <list>
#include <wx/dcmemory.h>
#include <wx/log.h>
#include <algorithm>
#include "levelcalculator.h"
using namespace std;

// AngleMeter
#ifdef WXSPAM
IMPLEMENT_DYNAMIC_CLASS(AngleMeter, pmControl)
#else
wxIMPLEMENT_DYNAMIC_CLASS(AngleMeter, pmControl);
#endif // WXSPAM


AngleMeter::AngleMeter()
    : pmControl()
    , m_bSurround(false)
	, m_nBevel(0)
    , m_dMin(-70)
    , m_dMax(0)
	, m_nLightGap(0)
	, m_dAngleRatio(0.0)
	, m_bInit(false)
	, m_dLastValue{-80.0,-80.0}
	, m_nRouting(0)
	, m_dPeakValue{-80.0,-80.0}
	, m_dFall(0.0)
    , m_nMeterDisplay(PEAK)
	, m_nPeakMode(0)
	, m_bFreeze(false)
	, m_dAngle{0.0,0.0}
	, m_dAngleMax{0.0,0.0}
	, m_nPeakCounter{0,0}
	, m_nChannel(0)
	, m_nInputChannels(2)
	, m_nMeterMSMode(0)
	, m_dOffset(0.0)
	, m_dScalingFactor(0.0)
	, m_dOverMod(0.0)
	, m_bTextCurrent(false)
	, m_bTextPeak(false)

{
    //create our font
    Connect(wxEVT_PAINT, (wxObjectEventFunction)&AngleMeter::OnPaint);
    Connect(wxEVT_SIZE, (wxObjectEventFunction)&AngleMeter::OnSize);
    m_clrText = wxColour(200,180,255);


}

AngleMeter::AngleMeter(wxWindow *parent, wxWindowID id, const wxString & sText,double dMin, unsigned int nRouting, unsigned int nChannel, const wxPoint& pos, const wxSize& size) :
    pmControl(),
    m_bSurround(false)
	, m_nBevel(0)
    , m_dMin(dMin)
    , m_dMax(-80.0)
	, m_nLightGap(0)
	, m_dAngleRatio(0.0)
	, m_bInit(false)
	, m_nRouting(nRouting)
	, m_dFall(0.0)
    , m_nMeterDisplay(PEAK)
	, m_nPeakMode(0)
	, m_bFreeze(false)
	, m_dAngle{0.0,0.0}
	, m_dAngleMax{0,0}
	, m_nPeakCounter{0,0}
	, m_nChannel(nChannel)
	, m_nInputChannels(2)
	, m_nMeterMSMode(0)
	, m_dOffset(0.0)
	, m_dScalingFactor(0.0)
	, m_dOverMod(0.0)
	, m_bTextCurrent(false)
	, m_bTextPeak(false)
{
    m_dLastValue[0] = dMin;
    m_dLastValue[1] = dMin;
    m_dPeakValue[0] = dMin;
    m_dPeakValue[1] = dMin;

    wxSize szInit(size);
    wxSize bestSize = DoGetBestSize();
    if(size.x<=0)
        szInit.SetWidth(bestSize.x);
    if(size.y <= 0)
        szInit.SetHeight(bestSize.y);


    wxWindow::Create(parent,id,pos,szInit,wxWANTS_CHARS, wxT("AngleMeter"));


    SetMinSize(size);

    m_nPeakMode = PEAK_SHOW;
    m_bFreeze = false;

    SetBackgroundStyle(wxBG_STYLE_CUSTOM);

    SetMeterMSMode(LevelCalculator::M6);

    m_nRouting = nRouting;

    Connect(wxEVT_PAINT, (wxObjectEventFunction)&AngleMeter::OnPaint);
    Connect(wxEVT_SIZE, (wxObjectEventFunction)&AngleMeter::OnSize);
    m_clrText = wxColour(200,180,255);

    switch(m_nRouting)
    {
        case LEFT_RIGHT:
            m_clrMeter[0] = *wxRED;
            m_clrMeter[1] = *wxGREEN;
            break;
        case MONO_STEREO:
            m_clrMeter[0] = *wxWHITE;
            m_clrMeter[1] = wxColour(255,180,0);
            break;
        case MONO:
            m_clrMeter[0] = *wxWHITE;
            m_clrMeter[1] = *wxWHITE;
            break;
    }

    InitMeter(sText, dMin);

    SetMeterDisplay(m_nMeterDisplay);
}

AngleMeter::~AngleMeter()
{
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
    for(int i = 0; i < m_vLevels.size(); i++)
    {
        double dAngle = m_vLevelAngles[i];
        double dAngleDeg = m_vLevelAngles[i];
        double dX = dHT*sin(dAngle);
        double dXTo = dHL*sin(dAngle);
        double dY = dHT*cos(dAngle);
        double dYTo = dHL*cos(dAngle);

        double dTextX = (dHT+15.0)*sin(dAngle);
        double dTextY = (dHT+15.0)*cos(dAngle);


        dc.SetPen(wxPen(wxColour(180,180,180),1, wxSOLID));
        if(m_vLevels[i] < m_dOverMod)
        {
            dc.SetTextForeground(wxColour(255,255,255));
        }
        else
        {
            dc.SetTextForeground(wxColour(255,0,0));
        }

        dc.DrawLine(pntBottom.x+dXTo, pntBottom.y-dYTo, pntBottom.x+static_cast<int>(dX), pntBottom.y-static_cast<int>(dY));
        wxString sText(wxString::Format(wxT("%.0f"),(m_vLevels[i]-m_dOffset)/m_dScalingFactor));
        if(m_vLevels[i]-m_dOffset == 0.0 && m_sReference.empty()==false)
        {
            sText = m_sReference;
        }
        dc.DrawRotatedText(sText, wxPoint(pntBottom.x+static_cast<int>(dTextX)-((sText.length()-1)*5), pntBottom.y-static_cast<int>(dTextY)), -dAngleDeg);

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

    if(m_bTextCurrent)
    {
        m_uiLevelText[0].Draw(dc, uiRect::BORDER_FLAT);
        if(m_nRouting != MONO)
        {
            m_uiLevelText[1].Draw(dc, uiRect::BORDER_FLAT);
        }
    }
    if(m_bTextPeak)
    {
        m_uiPeakText[0].Draw(dc, uiRect::BORDER_FLAT);
        if(m_nRouting != MONO)
        {
            m_uiPeakText[1].Draw(dc, uiRect::BORDER_FLAT);
        }
    }

        dc.SetPen(*wxBLACK_PEN);
    dc.SetBrush(wxBrush(wxColour(50,50,50)));

    wxPoint pntTop[4] = {wxPoint(0,0), wxPoint(m_nBevel,m_nBevel), wxPoint(GetClientRect().GetRight()-m_nBevel,m_nBevel), wxPoint(GetClientRect().GetRight(),0)};
    wxPoint pntRight[4] = {wxPoint(GetClientRect().GetRight(),0), wxPoint(GetClientRect().GetRight()-m_nBevel,m_nBevel), wxPoint(GetClientRect().GetRight()-m_nBevel,GetClientRect().GetBottom()-m_nBevel), wxPoint(GetClientRect().GetRight(),GetClientRect().GetBottom())};
    wxPoint pntBottoms[4] = {wxPoint(0,GetClientRect().GetBottom()), wxPoint(m_nBevel,GetClientRect().GetBottom()-m_nBevel), wxPoint(GetClientRect().GetRight()-m_nBevel,GetClientRect().GetBottom()-m_nBevel), wxPoint(GetClientRect().GetRight(),GetClientRect().GetBottom())};
    wxPoint pntLeft[4] = {wxPoint(0,0), wxPoint(m_nBevel,m_nBevel), wxPoint(m_nBevel,GetClientRect().GetBottom()-m_nBevel), wxPoint(0,GetClientRect().GetBottom())};
    dc.DrawPolygon(4,pntTop);
    dc.DrawPolygon(4,pntRight);
    dc.DrawPolygon(4,pntLeft);
    dc.DrawPolygon(4,pntBottoms);

    m_uiLabel.Draw(dc,uiRect::BORDER_NONE);
}


void AngleMeter::ShowSurround(bool bSurround)
{
    m_bSurround = bSurround;
    InitMeter(m_uiLabel.GetLabel(), m_dMin);
}

void AngleMeter::InitMeter(const wxString& sText,double dMin)
{
    m_dMin = dMin;
    m_dPeakValue[0] = m_dMin;
    m_dPeakValue[1] = m_dMin;
    m_dAngleRatio = 80/(m_dMax-m_dMin);

    //work out size of rects
    wxString sLevel;

    m_uiLabel.SetLabel(sText);

    //m_bSurround= false;
    if(m_bSurround)
    {
        m_nBevel = GetClientSize().x/15;
    }
    else
    {
        m_nBevel = 3;
    }



    m_rectGrid = wxRect(m_nBevel, m_nBevel, GetClientSize().x-2-(m_nBevel*2), GetClientSize().y-25-(m_nBevel*2));

    m_uiLabel.SetRect(m_rectGrid.GetLeft()+10,m_rectGrid.GetBottom()-30, 70, 25);
    m_uiLabel.SetBackgroundColour(*wxBLACK);
    m_uiLabel.SetGradient(0);

    m_uiType.SetRect(m_rectGrid.GetRight()-80, m_rectGrid.GetBottom()-30, 70, 25);
    m_uiType.SetBackgroundColour(*wxBLACK);

    m_uiLevelText[0].SetRect(m_rectGrid.GetLeft()+5, m_rectGrid.GetBottom(), m_rectGrid.GetWidth()/4-2, 20);
    m_uiPeakText[0].SetRect(m_uiLevelText[0].GetRight()+2, m_uiLevelText[0].GetTop(), m_rectGrid.GetWidth()/4-5, 20);

    int nLeft(m_rectGrid.GetLeft()+m_rectGrid.GetWidth()/2+5);
    m_uiLevelText[1].SetRect(nLeft, m_uiLevelText[0].GetTop(), m_rectGrid.GetWidth()/4-2, 20);
    m_uiPeakText[1].SetRect(m_uiLevelText[1].GetRight()+2, m_uiLevelText[0].GetTop(), m_rectGrid.GetWidth()/4-5, 20);

    m_uiLevelText[0].SetGradient(0);
    m_uiLevelText[0].SetBackgroundColour(wxColour(0,0,150));
    m_uiPeakText[0].SetGradient(0);
    m_uiPeakText[0].SetBackgroundColour(wxColour(0,0,100));

    m_uiLevelText[1].SetGradient(0);
    m_uiLevelText[1].SetBackgroundColour(wxColour(0,0,150));
    m_uiPeakText[1].SetGradient(0);
    m_uiPeakText[1].SetBackgroundColour(wxColour(0,0,100));
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

    m_dLastValue[0] = m_dMin;
    m_dLastValue[1] = m_dMin;
    m_dPeakValue[0] = m_dMin;
    m_dPeakValue[1] = m_dMin;
    ShowValue(m_dLastValue);


}
void AngleMeter::ShowValue(double dValue[2])
{
    for(int i = 0; i < 2; i++)
    {
        if(!m_bFreeze)
        {
            m_dLastValue[i] = dValue[i];

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

        WorkoutAngles(m_dLastValue[i], m_dAngle[i]);
        WorkoutAngles(m_dPeakValue[i], m_dAngleMax[i]);

        m_uiLevelText[i].SetLabel(wxString::Format(wxT("%.1f"), (m_dLastValue[i]-m_dOffset)/m_dScalingFactor));
        m_uiPeakText[i].SetLabel(wxString::Format(wxT("%.1f"), (m_dPeakValue[i]-m_dOffset)/m_dScalingFactor));
    }
    Refresh();
}

void AngleMeter::WorkoutAngles(double dLevel, double& dAngle)
{
    dAngle = -40 + (max(dLevel, m_dMin-6)-m_dMin)*m_dAngleRatio;
    dAngle = max(min(dAngle, 45.0), -45.0);
    dAngle *= M_PI/180.0;
    //dAngle = max(-45.0, min(45.0, dAngle))*M_PI/180.0;

}

void AngleMeter::OnSize(wxSizeEvent& event)
{
    InitMeter(m_uiLabel.GetLabel(), m_dMin);


    Refresh();
}


void AngleMeter::SetRouting(short nRouting)
{
    m_nRouting = nRouting;
    ResetMeter();
}

void AngleMeter::SetMeterDisplay(short nDisplay)
{
    m_nMeterDisplay = nDisplay;
    m_dMax = 0.0;
    ResetMeter();
}


void AngleMeter::SetLevels(const std::vector<double>& vLevels, double dOffset, double dScaling, const wxString& sTitle, const wxString& sUnits, double dOverMod, wxString sReference)
{
    m_vLevels = vLevels;
    m_vLevelAngles = vLevels;

    m_dOffset = dOffset;
    m_dScalingFactor = dScaling;
    m_dMin = vLevels[0];
    m_dMax = vLevels[vLevels.size()-1];


    m_dAngleRatio = 80/(m_dMax-m_dMin);


    m_dOverMod = dOverMod;
    m_sReference = sReference;

    for(size_t i = 0 ; i < m_vLevels.size(); i++)
    {
        WorkoutAngles(m_vLevels[i], m_vLevelAngles[i]);
    }
    m_uiLabel.SetLabel(wxString::Format(wxT("%s"), sTitle.c_str()));
    m_uiType.SetLabel(sUnits);

    Refresh();
}

void AngleMeter::SetLabel(const wxString& sLabel)
{
    //m_uiLabel.SetLabel(sLabel);
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




void AngleMeter::SetMeterMSMode(long nMode)
{
    m_nMeterMSMode = nMode;
}

void AngleMeter::DisplayCurrentLevelAsText(bool bShow)
{
    m_bTextCurrent = bShow;
    Refresh();
}

void AngleMeter::DisplayPeakLevelAsText(bool bShow)
{
    m_bTextPeak = bShow;
    Refresh();
}
