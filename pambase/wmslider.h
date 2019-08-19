#pragma once
#include <wx/window.h>
#include <wx/timer.h>
#include "uirect.h"
#include "pmcontrol.h"

/** @class a class that draws a lable on the screen, derives from wxWindow
**/
class PAMBASE_IMPEXPORT wmSlider : public pmControl
{
    DECLARE_EVENT_TABLE()
    #ifdef WXSPAM
    DECLARE_DYNAMIC_CLASS(wmSlider)
    #else
    wxDECLARE_DYNAMIC_CLASS(wmSlider);
    #endif // WXSPAM
    public:

        /** @brief default constructor
        **/
        wmSlider();

        /** @brief Constructor - made to be the same as a wxStaticText
        *   @param parent pointer to the parent window
        *   @param id the wxWindowId of the label
        *   @param label the text to write in the label
        *   @param pos the top-left point of the label
        *   @param size the width and height of the label
        *   @param style not currently used
        *   @param name not currently used
        **/
        wmSlider(wxWindow *parent, wxWindowID id, const wxString& sLabel=wxEmptyString, const wxPoint& pos=wxDefaultPosition, const wxSize& size=wxDefaultSize, long nStyle=0,const wxString& name=wxEmptyString);


        virtual ~wmSlider();


        bool SetSliderColour(const wxColour &colour);
        bool SetButtonColour(const wxColour &colour);

        /** @brief returns the default size of the label for sizers
        **/
        wxSize DoGetBestSize() const
        {
            return wxSize(400,40);
        }

        void Init(double dMin, double dMax, double dStart);

        double GetPosition()
        {
            return m_dPosition;
        }

        void SetSliderPosition(double dPosition, bool bEvent);

        uiRect& GetUiRect();

  protected:

        /** Called to draw the console
        *   @param event
        **/
        void OnPaint(wxPaintEvent& event);

        /** Called when the console resizes
        *   @param event
        **/
        void OnSize(wxSizeEvent& event);

        void OnFlash(wxTimerEvent& event);

        void CreateRects();
        void Draw(wxDC& dc);

        void OnLeftDown(wxMouseEvent& event);
        void OnLeftUp(wxMouseEvent& event);
        void OnMouseMove(wxMouseEvent& event);
        void OnCaptureLost(wxMouseCaptureLostEvent& event);

        void MoveSlider(int x);

        uiRect m_uiSlider;
        uiRect m_uiButton;

        bool m_bDown;

        double m_dResolution;
        double m_dMin;
        double m_dMax;
        double m_dPosition;
};

#ifdef WXSPAM
DECLARE_EXPORTED_EVENT_TYPE(WXEXPORT, wxEVT_SLIDER_MOVE, -1)
#else
wxDECLARE_EXPORTED_EVENT(PAMBASE_IMPEXPORT, wxEVT_SLIDER_MOVE,wxCommandEvent);
#endif
