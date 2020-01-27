#include <wx/log.h>
#include "levelmeter.h"
#include <wx/busyinfo.h>
#include <wx/dcbuffer.h>
#include <list>
#include <wx/dcmemory.h>
#include <algorithm>
#include "levelcalculator.h"
using namespace std;

// LevelMeter
#ifdef WXSPAM
IMPLEMENT_DYNAMIC_CLASS(LevelMeter, pmControl)
#else
wxIMPLEMENT_DYNAMIC_CLASS(LevelMeter, pmControl);
#endif // WXSPAM

//BEGIN_EVENT_TABLE(LevelMeter, wxWindow)
//END_EVENT_TABLE()

LevelMeter::LevelMeter() : pmControl()
    , m_dMin(-70)
    , m_dMax(0)
	, m_nLightGap(0)
	, m_bInit(false)
	, m_dLastValue(-80.0)
	, m_bLevelDisplay(false)
	, m_dPeakValue(-80.0)
	, m_dFall(-80.0)
	, m_dPixelsPerdB(0.0)
	, m_dPixelsPerPPM(0.0)
	, m_nMeterDisplay(0)
	, m_nPeakMode(0)
	, m_bFreeze(false)
	, m_nPeakCounter(0)
	, m_nMeterMSMode(0)
	, m_nMeterSpeed(0)
    , m_bShading(true)
	, m_dLevelOffset(0.0)
	, m_dScalingFactor(0.0)
	, m_nChannels(2)
{
    //create our font
    Connect(wxEVT_PAINT, (wxObjectEventFunction)&LevelMeter::OnPaint);
    Connect(wxEVT_SIZE, (wxObjectEventFunction)&LevelMeter::OnSize);
    m_clrText = wxColour(200,180,255);
}

LevelMeter::LevelMeter(wxWindow *parent, wxWindowID id, const wxString & sText,double dMin, bool bLevelDisplay, const wxPoint& pos, const wxSize& size) : pmControl(),
    m_dMax(0)
	, m_nLightGap(0)
	, m_bInit(false)
	, m_dLastValue(-80.0)
	, m_bLevelDisplay(false)
	, m_dPeakValue(-80.0)
	, m_dFall(-80.0)
	, m_dPixelsPerdB(0.0)
	, m_dPixelsPerPPM(0.0)
	, m_nMeterDisplay(0)
	, m_nPeakMode(0)
	, m_bFreeze(false)
	, m_nPeakCounter(0)
	, m_nMeterMSMode(0)
	, m_nMeterSpeed(0)
    , m_bShading(true)
	, m_dLevelOffset(0.0)
	, m_dScalingFactor(0.0)
	, m_nChannels(2)
{
    m_dLastValue = -180;
    wxSize szInit(size);
    wxSize bestSize = DoGetBestSize();
    if(size.x<=0)
        szInit.SetWidth(bestSize.x);
    if(size.y <= 0)
        szInit.SetHeight(bestSize.y);


    m_dLevelOffset = 0.0;
    wxWindow::Create(parent,id,pos,szInit,wxWANTS_CHARS, wxT("levelmeter"));

    SetMinSize(size);

    m_nPeakMode = PEAK_SHOW;
    m_bFreeze = false;

    SetBackgroundStyle(wxBG_STYLE_CUSTOM);

    m_bLevelDisplay = bLevelDisplay;

    Connect(wxEVT_PAINT, (wxObjectEventFunction)&LevelMeter::OnPaint);
    Connect(wxEVT_SIZE, (wxObjectEventFunction)&LevelMeter::OnSize);
    m_clrText = wxColour(200,180,255);

    m_pairColour[0] = make_pair(m_dMin/2, GetBackgroundColour());
    m_pairColour[1] = make_pair(m_dMin/4, GetBackgroundColour());

    m_uiBlack.SetBackgroundColour(*wxBLACK);
    InitMeter(sText, dMin);

    SetFont(wxFont(8,wxFONTFAMILY_SWISS,wxFONTSTYLE_NORMAL,wxFONTWEIGHT_NORMAL,false,_T("Arial"),wxFONTENCODING_DEFAULT));
    m_nChannels = 2;

}

LevelMeter::~LevelMeter()
{
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
        dc.DrawBitmap(m_bmpMeter, 0,m_uiLevelText.GetBottom());
        m_uiBlack.Draw(dc, uiRect::BORDER_NONE);

        if(m_nPeakMode != PEAK_HIDE)
        {
            m_uiPeak.Draw(dc,uiRect::BORDER_FLAT);
        }
        dc.SetPen(wxPen(wxColour(120,120,120),1));

        for(size_t i = 0; i < m_vLevels.size(); ++i)
        {
            dc.DrawLine(0, m_uiLevelText.GetBottom()-static_cast<int>(m_dPixelsPerdB*(m_vLevels[i])), GetClientRect().GetWidth(), m_uiLevelText.GetBottom()-static_cast<int>(m_dPixelsPerdB*(m_vLevels[i])));
        }

        dc.SetPen(wxNullPen);
        m_uiLabel.Draw(dc, uiRect::BORDER_NONE);
        m_uiLevelText.Draw(dc, uiRect::BORDER_NONE);

    }
    else
    {
        for(size_t i = 0; i < m_vLevels.size(); i++)
        {
            int nY(m_uiLevelText.GetBottom()-(m_dPixelsPerdB*(m_vLevels[i])));


            dc.SetPen(wxPen(wxColour(160,160,160),1));
            dc.DrawLine(0, m_uiLevelText.GetBottom()-(m_dPixelsPerdB*(m_vLevels[i])), GetClientRect().GetWidth(), m_uiLevelText.GetBottom()-(m_dPixelsPerdB*(m_vLevels[i])));
            uiRect uiLevel(wxRect(10, nY-10,GetClientSize().x-25, 20));
            uiLevel.SetBackgroundColour(*wxBLACK);
            uiLevel.SetForegroundColour(*wxWHITE);

            if((m_vLevels[i]-m_dLevelOffset) < -0.5 || (m_vLevels[i]-m_dLevelOffset) > 0.5 || m_sReference.empty())
            {
                uiLevel.Draw(dc, wxString::Format(wxT("%.0f"), (m_vLevels[i]-m_dLevelOffset)/m_dScalingFactor), uiRect::BORDER_NONE);
            }
            else
            {
                uiLevel.Draw(dc, m_sReference, uiRect::BORDER_NONE);
            }
        }
        m_uiLevelText.Draw(dc, uiRect::BORDER_NONE);
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


    int nMid = -m_pairColour[1].first*m_dPixelsPerdB;


    m_uiLevel[0].SetRect(0, m_uiLevelText.GetBottom(), GetClientRect().GetWidth(), GetClientRect().GetHeight()-m_uiLevelText.GetHeight()-m_uiLabel.GetHeight());
    m_uiLevel[1].SetRect(0, m_uiLevelText.GetBottom(), GetClientRect().GetWidth(), nMid-m_uiLevelText.GetHeight());




    //draw to the bmp..
    wxMemoryDC dc;
    m_bmpMeter = wxBitmap(GetClientSize().x, GetClientSize().y-m_uiLevelText.GetHeight()-m_uiLabel.GetHeight(), 24);
    dc.SelectObject(m_bmpMeter);
    if(m_bShading)
    {
        for(int i= 0; i < 2; i++)
        {
            m_uiLevel[i].SetGradient(wxSOUTH);
        }
    }
    else
    {
        for(int i= 0; i < 2; i++)
        {
            m_uiLevel[i].SetGradient(0);
        }
    }

    m_uiLevel[0].Draw(dc, uiRect::BORDER_NONE);
    m_uiLevel[1].Draw(dc, uiRect::BORDER_NONE);



    m_uiPeak.SetBackgroundColour(m_pairColour[1].second);

}

bool LevelMeter::SetLightColours(wxColour clrLow, double dOverMod, wxColour clrOver)
{

    m_pairColour[0] = make_pair(-80, clrLow);
    m_pairColour[1] = make_pair(dOverMod, clrOver);

    m_uiSimple.SetBackgroundColour(clrLow,clrLow);

    m_uiLevel[0].SetBackgroundColour(wxNullColour, clrLow);
    m_uiLevel[1].SetBackgroundColour(clrOver, clrOver);

    InitMeter(m_uiLabel.GetLabel(), m_dMin);

    return true;

}
void LevelMeter::ResetMeter(void)
{
    m_nPeakCounter = 0;
    m_dLastValue = -80;
    m_dPeakValue = -80;
    Refresh();
}
void LevelMeter::ShowValue(double dValue)
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
        m_dPeakValue = min(dValue, m_dMax);
        m_uiLevelText.SetLabel(wxString::Format(wxT("%.1f%s"), (m_dPeakValue-m_dLevelOffset)/m_dScalingFactor, m_sUnit.c_str()));

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
}




//void LevelMeter::SetLevels(const double dLevels[], size_t nSize, double dOffset, wxString sUnit, wxString sTitle, double dScalingFactor)
//{
//    m_dScalingFactor = dScalingFactor;
//    m_dLevelOffset = dOffset;
//    m_vLevels.clear();
//    m_vLevels.resize(nSize);
//    for(size_t i = 0 ; i < nSize; i++)
//    {
//        m_vLevels[i] = dLevels[i];
//    }
//    m_sUnit = sUnit;
//
//    if(m_bLevelDisplay)
//    {
//        m_uiLevelText.SetLabel(sTitle);
//        m_uiLevelText.SetBackgroundColour(wxColour(0,64,128));
//    }
//
//    InitMeter(m_uiLabel.GetLabel(), dLevels[0]);
//    Refresh();
//}

void LevelMeter::SetLevels(const std::vector<double>& vLevels, double dOffset, wxString sUnit, wxString sTitle,wxString sReference,  double dScalingFactor)
{
    m_dScalingFactor = dScalingFactor;
    m_dLevelOffset = dOffset;
    m_vLevels = vLevels;
    m_sUnit = sUnit;

    if(m_bLevelDisplay)
    {
        m_uiLevelText.SetLabel(sTitle);
        m_uiLevelText.SetBackgroundColour(wxColour(0,64,128));
    }
    m_sReference = sReference;
    InitMeter(m_uiLabel.GetLabel(), m_vLevels[0]);
    Refresh();
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
    //convert dValue to PPM
    dValue += 18.0;
    dValue /= 4.0;
    dValue += 4.0;

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
        for(int i= 0; i < 3; i++)
        {
            m_uiLevel[i].SetGradient(wxSOUTH);
        }
    }
    else
    {
        for(int i= 0; i < 3; i++)
        {
            m_uiLevel[i].SetGradient(0);
        }
    }

    m_uiLevel[2].Draw(dc, uiRect::BORDER_NONE);
    m_uiLevel[1].Draw(dc, uiRect::BORDER_NONE);
    m_uiLevel[0].Draw(dc, uiRect::BORDER_NONE);

    ResetMeter();
}


void LevelMeter::SetSpeed(long nSpeed)
{
    switch(nSpeed)
    {
        case LevelCalculator::SLOW:
            m_dFall = 0.8;
            break;
        case LevelCalculator::NORMAL:
            m_dFall = 1.6;
            break;
        case LevelCalculator::FAST:
            m_dFall = 3.2;
    }
}
