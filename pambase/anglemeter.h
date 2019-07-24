#pragma once
#include <wx/window.h>
#include <vector>
#include "uirect.h"
#include <wx/bitmap.h>
#include "pmcontrol.h"

class PAMBASE_IMPEXPORT AngleMeter : public pmControl
{
    #ifdef WXSPAM
    DECLARE_DYNAMIC_CLASS(AngleMeter)
    #else
    wxDECLARE_DYNAMIC_CLASS(AngleMeter);
    #endif // WXSPAM
public:
    AngleMeter();
    virtual ~AngleMeter();

    AngleMeter(wxWindow *parent, wxWindowID id, const wxString & sText,double dMin, unsigned int nRouting, unsigned int nChannel, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxDefaultSize);

    void InitMeter(const wxString & sText,double dMin);

    void SetLevels(const std::vector<double>& vLevels, double dOffset, double dScaling, const wxString& sTitle, const wxString& sUnits, double dOverMod, wxString sReference=wxEmptyString);


    bool SetMeterColour(const wxColour& clrMeter1, const wxColour& clrMeter2);
    void ResetMeter(void);
    void OnPaint(wxPaintEvent& event);

    void SetLabel(const wxString& sLabel);

    void ShowValue(double dValue[2]);

    void ShowSurround(bool bSurround);


    void SetRouting(short nRouting);
    void SetMeterDisplay(short nDisplay);
    void OnSize(wxSizeEvent& event);
    wxSize DoGetBestSize()
    {
        return wxSize(50,100);
    }

    void SetPeakMode(int nMode);
    void FreezeMeter(bool bFreeze);

    void SetMeterMSMode(long nMode);

    void SetInputChannels(unsigned int nInputChannels)
    {
        m_nInputChannels = nInputChannels;
    }

    void DisplayCurrentLevelAsText(bool bShow);
    void DisplayPeakLevelAsText(bool bShow);

    enum {PEAK_HIDE, PEAK_SHOW, PEAK_HOLD};
    enum {MONO, LEFT_RIGHT, MONO_STEREO};

    enum {PPM, PEAK, ENERGY, LOUD, TOTAL, AVERAGE};
protected:



    void WorkoutAngles(double dLevel, double& dAngle);
    bool m_bSurround;

    uiRect m_uiLabel;
    uiRect m_uiLevelText[2];
    uiRect m_uiPeakText[2];
    unsigned int m_nBevel;
    wxBitmap m_bmpMeter;
    wxColour m_clrBackground;

    double m_dMin;
    double m_dMax;
    unsigned short m_nLightGap;
    double m_dAngleRatio;

    wxPen m_penText;
    wxColour m_clrText;

    bool m_bInit;

    double m_dLastValue[2];
    unsigned int m_nRouting;

    double m_dPeakValue[2];

    double m_dFall;


    short m_nMeterDisplay;

    int m_nPeakMode;
    bool m_bFreeze;
    wxRect m_rectLastPeak;
    wxRect m_rectLastBlack;

    wxColour m_clrMeter[2];
    double m_dAngle[2];
    double m_dAngleMax[2];

    wxRect m_rectGrid;
    uiRect m_uiType;


    int m_nPeakCounter[2];
    unsigned int m_nChannel;
    unsigned int m_nInputChannels;


    long m_nMeterMSMode;

    double m_dOffset;
    std::vector<double> m_vLevels;
    std::vector<double> m_vLevelAngles;
    wxString m_sUnit;
    double m_dScalingFactor;

    double m_dOverMod;
    wxString m_sReference;

    bool m_bTextCurrent;
    bool m_bTextPeak;
};


