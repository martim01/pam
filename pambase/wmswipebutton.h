#pragma once
#include <wx/window.h>
#include "wmbutton.h"

/** @brief Class that looks a bit like a fader and produces the same events as a wmButton class when slid
**/

class PAMBASE_IMPEXPORT wmSwipeButton : public wmButton
{
    DECLARE_EVENT_TABLE()
    wxDECLARE_DYNAMIC_CLASS(wmSwipeButton);

    public:
        ///< @brief Default constructor
        wmSwipeButton() : wmButton() { }

        /** @brief Constructor - made to be the same as a wxButton
        *   @param parent pointer to the parent window
        *   @param id the wxWindowId of the button
        *   @param label the text to write in the button
        *   @param pos the top-left point of the button
        *   @param size the width and height of the button
        *   @param style the button style: wmButton::STYLE_NORMAL, wmButton::STYLE_SELECT, wmButton::STYLE_REPEAT, wmButton::STYLE_HOLD and wmSwipeButton::STYLE_VERTICAL
        *   @param validator not used - just here to have same structure as wxButton
        *   @param name not used - just here to have same structure as wxButton
        **/
        wmSwipeButton(wxWindow *parent,
                 wxWindowID id,
                 const wxString& label = wxEmptyString,
                 const wxPoint& pos = wxDefaultPosition,
                 const wxSize& size = wxDefaultSize,
                 long style = 0, const wxValidator& validator = wxDefaultValidator, const wxString& name =wxT("button"));

        /** @brief Constructor - made to be the same as a wxBitmapButton
        *   @param parent pointer to the parent window
        *   @param id the wxWindowId of the button
        *   @param bitmap the wxBitmap to draw on the button
        *   @param pos the top-left point of the button
        *   @param size the width and height of the button
        *   @param style the button style: wmButton::STYLE_NORMAL, wmButton::STYLE_SELECT, wmButton::STYLE_REPEAT, wmButton::STYLE_HOLD
        *   @param validator not used - just here to have same structure as wxBitmapButton
        *   @param name not used - just here to have same structure as wxBitmapButton
        **/
        wmSwipeButton( wxWindow* parent, wxWindowID id, const wxBitmap& bitmap, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxDefaultSize, long style = 0, const wxValidator& validator = wxDefaultValidator, const wxString& name = wxT("button"));

        virtual ~wmSwipeButton();

        static const unsigned long  STYLE_VERTICAL = 0xFFFF;

  protected:

        void OnMouseMove(wxMouseEvent& event);
        void OnLeftDown(wxMouseEvent& event);
        void OnLeftDClick(wxMouseEvent& event);
        void OnLeftUp(wxMouseEvent& event);
        void OnCaptureLost(wxMouseCaptureLostEvent& event);

        virtual void Draw(wxDC& dc);
        virtual void CreateRects();

        uiRect m_uiSwitchLine;

        wxPoint m_pntMouse;
        bool m_bVertical;
};

