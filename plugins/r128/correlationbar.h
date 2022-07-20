#pragma once
#include <wx/window.h>
#include <wx/bitmap.h>
#include "uirect.h"
#include <wx/timer.h>
#include <list>
#include "uirect.h"
#include "pmcontrol.h"

class timedbuffer;

struct rgb
{
    double r;       // a fraction between 0 and 1
    double g;       // a fraction between 0 and 1
    double b;       // a fraction between 0 and 1
};

struct hsv
{
    double h;       // angle in degrees
    double s;       // a fraction between 0 and 1
    double v;       // a fraction between 0 and 1
};


/** @class a class that draws a button on the screen, derives from wxWindow
**/
class CorrelationBar : public pmControl
{
    DECLARE_EVENT_TABLE()
    wxDECLARE_DYNAMIC_CLASS(CorrelationBar);

    public:

        /** @brief default constructor
        **/
        CorrelationBar() : pmControl() {}

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
        CorrelationBar(wxWindow *parent,
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



        virtual ~CorrelationBar();


        /** @brief returns the default size of the button for sizers
        **/
        wxSize DoGetBestSize() const
        {
            return wxSize(470,470);
        }

        void SetAudioData(const timedbuffer* pBuffer);
        void SetNumberOfInputChannels(unsigned int nInputChannels);
        void SetAxisX(unsigned int nChannel);
        void SetAxisY(unsigned int nChannel);


        void ClearMeter();

        void WorkoutBalance(const timedbuffer* pBuffer);
        void SetMode(int nMode);


      protected:


        /** Called to draw the console
        *   @param event
        **/
        void OnPaint(wxPaintEvent& event);

        void DrawPoints(wxDC& dc);
        void DrawLevels(wxDC& dc);

        void DrawConvexHull(wxDC& dc);
        /** Called when the console resizes
        *   @param event
        **/
        virtual void OnSize(wxSizeEvent& event);

        void CreateRects();
        void OnLeftUp(wxMouseEvent& event);

        rgb hsv2rgb(hsv in);
        hsv rgb2hsv(rgb in);

        void WorkoutLevel();
        wxRect m_rectGrid;
        wxRect m_rectCorrelation;
        wxRect m_rectBalance;
        wxPoint m_pntPole;
        double m_dResolution;
        double m_dResolutionCorrelation;

        int m_nMode;

        float m_dMindB;

        unsigned int m_nInputChannels;
        unsigned int m_nAxisX;
        unsigned int m_nAxisY;

        std::list<std::pair<double, double> > m_lstLevels;
        double m_dCorrelation;


        std::list<double> m_lstCorrelation;

        uiRect m_uiCorrelation;

        wxBitmap* m_pBmpCorrelationOut;
        wxBitmap* m_pBmpCorrelationIn;
};





