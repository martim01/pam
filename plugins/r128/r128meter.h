#pragma once
#include <wx/window.h>
#include <vector>
#include "uirect.h"
#include <wx/bitmap.h>
#include "pmcontrol.h"


class R128Meter : public pmControl
{
    DECLARE_EVENT_TABLE()
    wxDECLARE_DYNAMIC_CLASS(R128Meter);

public:
    R128Meter();
    virtual ~R128Meter();

    R128Meter(wxWindow *parent, wxWindowID id, const wxString & sText,double dMin, double dMax, bool bLevelDisplay, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxDefaultSize);

    void InitMeter(const wxString & sText,double dMin, double dMax);
    void SetMinMax(double dMin, double dMax);

    void SetLevels(const std::vector<double>& vLevels,  double dOffset);

    bool SetLightColours(double dLow, wxColour clrLow, double dMid, wxColour clrMid,  wxColour clrHigh);
    void ResetMeter(void);
    void OnPaint(wxPaintEvent& event);

    void SetLabel(const wxString& sLabel);

    void ShowValue(double dValue);

    void SetTargetLevel(double dValue, const wxPen& penLevel);

    void SetLevelDisplay(bool bLevels);
    void OnSize(wxSizeEvent& event);
    wxSize DoGetBestSize()
    {
        return wxSize(50,100);
    }
    void SetPeakMode(int nMode);
    void FreezeMeter(bool bFreeze);
    void SetShading(bool bShading);

    void SetFall(double dFall)
    {
        m_dFall = dFall;
    }

    double GetValue()
    {
        return m_dLastValue;
    }



    enum {PEAK_HIDE, PEAK_SHOW, PEAK_HOLD};
protected:

    uiRect m_uiSimple;
    uiRect m_uiLevel[3];
    uiRect m_uiBlack;
    uiRect m_uiPeak;
    std::vector<uiRect>	m_vLevelText;
    std::vector<double> m_vLevels;

    uiRect m_uiLabel;
    uiRect m_uiLevelText;

    wxBitmap m_bmpMeter;
    wxColour m_clrBackground;

    double m_dMin;
    double m_dMax;
    unsigned short m_nLightGap;

    wxPen m_penText;
    wxColour m_clrText;

    bool m_bInit;

    double m_dLastValue;
    bool m_bLevelDisplay;

    double m_dPeakValue;

    double m_dFall;

    double m_dPixelsPerdB;

    short m_nMeterDisplay;

    int m_nPeakMode;
    bool m_bFreeze;
    wxRect m_rectLastPeak;
    wxRect m_rectLastBlack;

    std::pair<double, wxColour> m_pairColour[3];

    int m_nPeakCounter;

    long m_nMeterMSMode;
    long m_nMeterSpeed;

    bool m_bShading;

    double m_dLevelOffset;

    unsigned int m_nChannels;

    double m_dTargetLevel;
    wxPen m_penTarget;
};



