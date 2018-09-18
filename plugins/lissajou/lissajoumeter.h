#pragma once
#include <wx/window.h>
#include <wx/bitmap.h>
#include <list>
#include "uirect.h"
#include "SimpleComp.h"

//#include "wmscroller.h"

/** @class a class that draws a button on the screen, derives from wxWindow
**/
class LissajouMeter : public wxWindow
{
    DECLARE_EVENT_TABLE()

    public:

        /** @brief default constructor
        **/
        LissajouMeter(){}

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
        LissajouMeter(wxWindow *parent,
                 wxWindowID id,
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



        virtual ~LissajouMeter();

        void SetNumberOfChannels(unsigned int nChannels);
        void SetAxis(unsigned int nX, unsigned int nY);
        void SetLissajouData(const float* pBuffer, int nBufferSize);

        void RotateLissajou(bool bRotate)
        {
            m_bRotate = bRotate;
            Refresh();
        }

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
            return wxSize(250,250);
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

        enum {STARS, PHASEOMETER, JOINT};

  protected:


        void GetAmplitude(float dSample, float& dAmplitude, bool& bNegative);


        /** Called to draw the console
        *   @param event
        **/
        void OnPaint(wxPaintEvent& event);

        /** Called when the console resizes
        *   @param event
        **/
        virtual void OnSize(wxSizeEvent& event);

        void OnLeftUp(wxMouseEvent& event);

        void DrawStars(wxDC& dc);
        void DrawPeaks(wxDC& dc);
        void DrawHull(wxDC& dc);

        void DrawLines(wxImage& img, const wxPoint& pntCenter);

        void Scale(float dSampleL, float dSampleR, float& x, float& y, const wxPoint& pntCenter);


        void Rotate(float& x, float& y, const wxPoint& pntCenter);

        float* m_pBuffer;
        size_t m_nBufferSize;
        wxRect m_rectGrid;

        short m_nType;

        std::pair<float, float> m_pairStep;
        wxBitmap m_bmpScreen;
        bool m_bRotate;
        int m_nScaling;

        std::list<double> m_lstMax;
        double m_dAutoScale;

        bool m_bShowLevels;

        float m_dMindB;

        uiRect m_uiScale;
        uiRect m_uiAuto;
        wxPoint m_pntRotateOffset;

        chunkware_simple::SimpleComp m_Compressor;
        static const float LEVEL_PPM[7];
        static const wxString LABEL_SCALE[3];

        unsigned int m_nChannels;
        unsigned int m_nAxis[2];
};


