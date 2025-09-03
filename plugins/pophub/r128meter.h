#pragma once
#include <wx/window.h>
#include <vector>
#include "uirect.h"
#include <wx/bitmap.h>
#include "pmcontrol.h"


class R128MeterH : public pmControl
{
    DECLARE_EVENT_TABLE()
    wxDECLARE_DYNAMIC_CLASS(R128MeterH);

public:
    R128MeterH();
    virtual ~R128MeterH();

    R128MeterH(wxWindow *parent, wxWindowID id, const wxString & sText,double dMin, double dMax, bool bLevelDisplay, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxDefaultSize);

    void InitMeter(double dMin, double dMax);
    void SetMinMax(double dMin, double dMax);

    void SetLevels(const std::vector<double>& vLevels,  double dOffset);

    bool SetLightColours(double dLow, wxColour clrLow, double dMid, wxColour clrMid,  wxColour clrHigh);
    void ResetMeter(void);
    void OnPaint(wxPaintEvent& event);


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

    enum {kLow=0, kMid, kHigh};

    uiRect m_uiSimple;
    uiRect m_uiLevel[3];
    uiRect m_uiBlack;
    uiRect m_uiPeak;

    wxBitmap m_bmpMeter;
    wxColour m_clrBackground;

    double m_dMin = 0.0;
    double m_dMax = -80.0;
    unsigned short m_nLightGap=0;

    wxPen m_penText;
    wxColour m_clrText;

    bool m_bInit;

    double m_dLastValue = -180.0;
    
    double m_dPeakValue = 0.0;

    double m_dFall = 80.0;

    double m_dPixelsPerdB = 0.0;

    short m_nMeterDisplay=0;

    int m_nPeakMode = PEAK_SHOW;
    bool m_bFreeze = false;
    wxRect m_rectLastPeak;
    wxRect m_rectLastBlack;

    std::pair<double, wxColour> m_pairColour[3];

    int m_nPeakCounter;

    long m_nMeterMSMode;
    long m_nMeterSpeed;

    bool m_bShading = false;

    double m_dLevelOffset = 0.0;

    unsigned int m_nChannels = 2;

    double m_dTargetLevel;
    wxPen m_penTarget;

    bool m_bLevelDisplay = false;
    std::vector<double> m_vLevels;
};



