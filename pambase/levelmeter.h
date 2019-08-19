#pragma once
#include <wx/window.h>
#include <vector>
#include "uirect.h"
#include <wx/bitmap.h>
#include "pmcontrol.h"


class PAMBASE_IMPEXPORT LevelMeter : public pmControl
{
    #ifdef WXSPAM
    DECLARE_DYNAMIC_CLASS(LevelMeter)
    #else
    wxDECLARE_DYNAMIC_CLASS(LevelMeter);
    #endif // WXSPAM
    //DECLARE_EVENT_TABLE()
public:
    LevelMeter();
    virtual ~LevelMeter();

    LevelMeter(wxWindow *parent, wxWindowID id, const wxString & sText,double dMin, bool bLevelDisplay, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxDefaultSize);

    void InitMeter(const wxString & sText,double dMin);

    //void SetLevels(const double dLevels[], size_t nSize, double dOffset, wxString sUnit, wxString sTitle, double dScalingFactor=1.0);
    void SetLevels(const std::vector<double>& vLevels, double dOffset, wxString sUnit, wxString sTitle, wxString sReference=wxEmptyString, double dScalingFactor=1.0);


    bool SetLightColours(wxColour clrLow, double dOverMod, wxColour clrOver);
    void ResetMeter(void);
    void OnPaint(wxPaintEvent& event);

    void SetLabel(const wxString& sLabel);

    void ShowValue(double dValue);

    void SetSpeed(long nSpeed);



    void SetLevelDisplay(bool bLevels);
    void SetMeterDisplay(short nDisplay);
    void OnSize(wxSizeEvent& event);
    wxSize DoGetBestSize()
    {
        return wxSize(50,100);
    }
    void SetNumberOfChannels(unsigned int nChannels);

    void SetPeakMode(int nMode);
    void FreezeMeter(bool bFreeze);

    //void SetMeterMSMode(long nMode);
    //void SetMeterSpeed(long nSpeed);

    void SetShading(bool bShading);


    enum {PEAK_HIDE, PEAK_SHOW, PEAK_HOLD};
    enum {LEFT=0, RIGHT=1, MIDDLE=8, SIDE=9, LEVELS=10};

    enum {PPM, PEAK, ENERGY, LOUD, TOTAL, AVERAGE, TRUEPEAK};
protected:

    void ShowPPM(double dValue);

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
    double m_dPixelsPerPPM;

    short m_nMeterDisplay;

    int m_nPeakMode;
    bool m_bFreeze;
    wxRect m_rectLastPeak;
    wxRect m_rectLastBlack;

    std::pair<double, wxColour> m_pairColour[2];

    int m_nPeakCounter;

    long m_nMeterMSMode;
    long m_nMeterSpeed;

    bool m_bShading;

    double m_dLevelOffset;
    wxString m_sUnit;
    double m_dScalingFactor;
    wxString m_sReference;

    unsigned int m_nChannels;
};


