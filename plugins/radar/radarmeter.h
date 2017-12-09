#pragma once
#include <wx/window.h>
#include <wx/bitmap.h>
#include "audio.h"
#include "uirect.h"
#include <wx/timer.h>
#include <queue>
//#include "wmscroller.h"

/** @class a class that draws a button on the screen, derives from wxWindow
**/
class RadarMeter : public wxWindow
{
    DECLARE_EVENT_TABLE()

    public:

        /** @brief default constructor
        **/
        RadarMeter(){}

        /** @brief Constructor - made to be the same as a wxButton
        *   @param parent pointer to the parent window
        *   @param id the wxWindowId of the button
        *   @param label the text to write in the button
        *   @param pos the top-left point of the button
        *   @param size the width and height of the button
        *   @param style the button style: wmButton::STYLE_NORMAL, wmButton::STYLE_SELECT, wmButton::STYLE_REPEAT, wmButton::STYLE_HOLD
        *   @param validator not used - just here to have same structure as wxButton
        *   @param name not used - just here to have same structure as wxButton
        **/
        RadarMeter(wxWindow *parent,
                 wxWindowID id = wxID_ANY,
                 const wxPoint& pos = wxDefaultPosition,
                 const wxSize& size = wxDefaultSize);

        /** @brief Creates the button
        *   @param parent pointer to the parent window
        *   @param id the wxWindowId of the button
        *   @param label the text to write in the button
        *   @param pos the top-left point of the button
        *   @param size the width and height of the button
        *   @param style the button style: wmButton::STYLE_NORMAL, wmButton::STYLE_SELECT, wmButton::STYLE_REPEAT, wmButton::STYLE_HOLD
        *   @param validator not used - just here to have same structure as wxButton
        *   @param name not used - just here to have same structure as wxButton
        **/
        virtual bool Create(wxWindow *parent,
                    wxWindowID id,
                    const wxPoint& pos = wxDefaultPosition,
                    const wxSize& size = wxDefaultSize);



        virtual ~RadarMeter();

        void SetRadarLevel(double dLevel, unsigned int nSamples, bool bInDBAlready);


        void SetScaling(int nScaling)
        {
            m_nScaling = nScaling;
            Refresh();
        }

        enum {SCALE_DB, SCALE_LINEAR, SCALE_AUTO};


        /** @brief returns the default size of the button for sizers
        **/
        wxSize DoGetBestSize() const
        {
            return wxSize(470,470);
        }

        void SetDisplayType(short nType)
        {
            m_nType = nType;
        }

        void SetShowLevels(bool bShow)
        {
            m_bShowLevels = bShow;
        }

        void SetMindB(float dMin);

        void SetTimespan(unsigned int nSeconds, bool bClearMeter=true);
        void SetSampleRate(unsigned int nSampleRate);
        void ClearMeter();

        void SetRefreshRate(unsigned int nMilliseconds);
        void SetMode(unsigned int nMode);

  protected:

        wxTimer m_timerSecond;

        void OnTimer(wxTimerEvent& event);

        void DrawRadar(wxDC& dc);

        void GetAmplitude(float dSample, float& dAmplitude, bool& bNegative);

        void CreateBackgroundBitmap();

        /** Called to draw the console
        *   @param event
        **/
        void OnPaint(wxPaintEvent& event);

        /** Called when the console resizes
        *   @param event
        **/
        virtual void OnSize(wxSizeEvent& event);

        void OnLeftUp(wxMouseEvent& event);

        void DrawLines(wxImage& img, const wxPoint& pntCenter);
        void Scale(float dSampleL, float dSampleR, float& x, float& y, const wxPoint& pntCenter);


        double m_dLevel;
        wxRect m_rectGrid;

        short m_nType;

        std::pair<float, float> m_pairStep;
        wxBitmap m_bmpScreen;
        wxBitmap m_bmpBackground;
        wxBitmap m_bmpFade;
        bool m_bRotate;
        int m_nScaling;

        std::list<double> m_lstMax;
        double m_dAutoScale;

        bool m_bShowLevels;

        double m_dMindB;
        float m_dResolution;

        double m_dAngle;
        double m_dAngleMod;
        wxPoint m_pntCenter;
        static const float LEVEL_PPM[7];
        static const wxString LABEL_SCALE[3];

        std::queue<wxPoint> m_queueLines;
        std::queue<wxPoint> m_queueFade;
        wxPoint m_pntLast;
        wxPoint m_pntLastBlack;
        wxPoint m_pntLastFade;
        unsigned int m_nChannels;
        unsigned int m_nChannel;

        unsigned int m_nTimespan;
        unsigned int m_nSamples;
        unsigned int m_nSampleRate;

        unsigned int m_nRefreshRate;
        unsigned int m_nPoints;

        unsigned int m_nMode;
};



