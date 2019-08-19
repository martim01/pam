#pragma once
#include <wx/window.h>
#include <wx/timer.h>
#include "uirect.h"
#include <wx/bitmap.h>
#include "pmcontrol.h"
//#include "wmscroller.h"

/** @class a class that draws a button on the screen, derives from wxWindow
**/
class PAMBASE_IMPEXPORT wmButton : public pmControl
{
    DECLARE_EVENT_TABLE()
    #ifdef WXSPAM
    DECLARE_DYNAMIC_CLASS(wmButton)
    #else
    wxDECLARE_DYNAMIC_CLASS(wmButton);
    #endif
    public:

        /** @brief default constructor
        **/
        wmButton();

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
        wmButton(wxWindow *parent,
                 wxWindowID id,
                 const wxString& label = wxEmptyString,
                 const wxPoint& pos = wxDefaultPosition,
                 const wxSize& size = wxDefaultSize,
                 long style = 0, const wxValidator& validator = wxDefaultValidator, const wxString& name =wxT("button"));

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
        bool Create(wxWindow *parent,
                    wxWindowID id,
                    const wxString& label = wxEmptyString,
                    const wxPoint& pos = wxDefaultPosition,
                    const wxSize& size = wxDefaultSize,
                    long style = 0, const wxValidator& validator = wxDefaultValidator, const wxString& name = wxT("button"));


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
        wmButton( wxWindow* parent, wxWindowID id, const wxBitmap& bitmap, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxDefaultSize, long style = 0, const wxValidator& validator = wxDefaultValidator, const wxString& name = wxT("button"));

        /** @brief creates the button - made to be the same as a wxBitmapButton
        *   @param parent pointer to the parent window
        *   @param id the wxWindowId of the button
        *   @param bitmap the wxBitmap to draw on the button
        *   @param pos the top-left point of the button
        *   @param size the width and height of the button
        *   @param style the button style: wmButton::STYLE_NORMAL, wmButton::STYLE_SELECT, wmButton::STYLE_REPEAT, wmButton::STYLE_HOLD
        *   @param validator not used - just here to have same structure as wxBitmapButton
        *   @param name not used - just here to have same structure as wxBitmapButton
        **/
        bool  Create(wxWindow* parent, wxWindowID id, const wxBitmap& bitmap, const wxPoint& pos, const wxSize& size = wxDefaultSize, long style = 0, const wxValidator& validator = wxDefaultValidator, const wxString& name = wxT("button"));

        virtual ~wmButton();

        /** @brief Sets the background colour of the button for all button states
        *   @param colour
        *   @return <i>bool</i> true
        **/
        bool SetBackgroundColour(const wxColour &colour);

        /** @brief Sets the text colour of the button for all button states
        *   @param colour
        *   @return <i>bool</i> true
        **/
        bool SetForegroundColour(const wxColour &colour);

        /** @brief Sets the background colour of the button when it is in its normal state
        *   @param colour
        **/
        void SetColour(const wxColour &colour);

        /** @brief Sets the background colour of the button when it is in its disabled state
        *   @param colour
        **/
        void SetColourDisabled(const wxColour &colour);

        /** @brief Sets the background colour of the button when it is in its selected state
        *   @param colour
        **/
        void SetColourSelected(const wxColour &colour);

        /** @brief Sets the background colour of the button when it is in its armed state
        *   @param colour
        **/
        void SetColourArmed(const wxColour &colour);

        /** @brief Sets the background colour of the button when it is flashing
        *   @param colour
        **/
        void SetColourFlash(const wxColour &colour);

        /** @brief Sets the text colour of the button when it is in its normal state
        *   @param colour
        **/
        void SetTextColour(const wxColour &colour);

        /** @brief Sets the text colour of the button when it is in its disabled state
        *   @param colour
        **/
        void SetTextColourDisabled(const wxColour &colour);

        /** @brief Sets the text colour of the button when it is in its selected state
        *   @param colour
        **/
        void SetTextColourSelected(const wxColour &colour);

        /** @brief Sets the text colour of the button when it is in its armed state
        *   @param colour
        **/
        void SetTextColourArmed(const wxColour &colour);

        /** @brief Sets the text colour of the button when it is flashing
        *   @param colour
        **/
        void SetTextColourFlash(const wxColour &colour);

        /** @brief Sets the text to draw on the button
        *   @param sLabel
        **/
        void SetLabel(const wxString& sLabel);

        /** @brief Set the alignment of the text in the button
        *   @param nAlign, a union of one of wxALIGN_LEFT,wxALIGN_CENTER_HORIZONTAL, wxALIGN_RIGHT and one of wxALIGN_TOP, wxALIGN_BOTTOM, wxALIGN_CENTER_VERTICAL
        **/
        void SetTextAlign(unsigned int nAlign);

        /** @brief Set the alignment of the bitmap in the button
        *   @param nAlign, a union of one of wxALIGN_LEFT,wxALIGN_CENTER_HORIZONTAL, wxALIGN_RIGHT and one of wxALIGN_TOP, wxALIGN_BOTTOM, wxALIGN_CENTER_VERTICAL
        **/
        void SetBitmapAlign(unsigned int nAlign);

        /** @brief Set the border state of the button
        *   @param
        **/
        void SetBorderState(unsigned short nState);

        /** @brief Sets the bitmap to draw on the button when it is in its normal state
        *   @param bitmap
        **/
        void SetBitmapLabel(const wxBitmap& bitmap);

        /** @brief Sets the bitmap to draw on the button when it is in its disabled state
        *   @param bitmap
        **/
        void SetBitmapDisabled(const wxBitmap& bitmap);

        /** @brief Sets the bitmap to draw on the button when it is in its selected state
        *   @param bitmap
        **/
        void SetBitmapSelected(const wxBitmap& bitmap);

        /** @brief Sets the bitmap to draw on the button when it is in its armed state
        *   @param bitmap
        **/
        void SetBitmapArmed(const wxBitmap& bitmap);

        /** @brief Sets the bitmap to draw on the button when it is flashing
        *   @param bitmap
        **/
        void SetBitmapFlash(const wxBitmap& bitmap);

        /** @brief Get the bitmap that is drawn when the button is in its normal state
        *   @return <i>wxBitmap</i>
        **/
        const wxBitmap& GetBitmapLabel() const;

        /** @brief Get the bitmap that is drawn when the button is in its disabled state
        *   @return <i>wxBitmap</i>
        **/
        const wxBitmap& GetBitmapDisabled() const;

        /** @brief Get the bitmap that is drawn when the button is in its selected state
        *   @return <i>wxBitmap</i>
        **/
        const wxBitmap& GetBitmapSelected() const;

        /** @brief Get the bitmap that is drawn when the button is in its armed state
        *   @return <i>wxBitmap</i>
        **/
        const wxBitmap& GetBitmapArmed() const;

        /** @brief Get the bitmap that is drawn when the button is flashing
        *   @return <i>wxBitmap</i>
        **/
        const wxBitmap& GetBitmapFlash() const;

        /** @brief Enable/Disable the button
        *   @param bEnable
        **/
        bool Enable(bool bEnable = true);

        /** @brief Make the button start/stop flashing
        *   @param bFlash if true the button will "flash", changing between its current colour etc and the set flash colour etc
        *   @param nInterval the speed of the flash in milliseconds
        **/
        void Flash(bool bFlash, unsigned int nInterval=500);

        /** @brief Get the text drawn on the button
        *   @return <i>wxString</i>
        **/
        wxString GetLabel() const
        {
            return m_uiRect.GetLabel();
        }

        void SetAuxillaryText(const wxString& sText);
        const wxString& GetAuxillaryText() const;

        /** @brief Changes the selected state of the button
        *   @param bSelected true to select, false to unselect
        *   @param bEvent true to generate a button clicked event (if bSelected = true)
        **/
        void ToggleSelection(bool bSelected, bool bEvent=false);

        /** @brief sets the syle of the button
        *   @param nStyle the new button style
        **/
        void SetStyle(int nStyle)
        {
            m_nStyle = nStyle;
        }

        bool IsChecked() const;

        /** @brief set associated client data
        *   @param pData a void* to some data
        **/
        void SetClientData(void* pData)
        {
            m_uiRect.SetClientData(pData);
        }

        /** @brief set associated integer data
        *   @param nData the data
        **/
        void SetIntData(int nData)
        {
            m_uiRect.SetIntData(nData);
        }

        /** @brief get associated client data
        *   @return <i>pData</i> a void* to some data
        **/
        void* GetClientData() const
        {
            return m_uiRect.GetClientData();
        }

        /** @brief get associated int data
        *   @return <i>nData</i> the data
        **/
        int GetIntData() const
        {
            return m_uiRect.GetIntData();
        }

        /** @brief returns the default size of the button for sizers
        **/
        wxSize DoGetBestSize() const
        {
            return wxSize(60,60);
        }

        void SetToggleLook(bool bLook, const wxString& sLeft = wxEmptyString, const wxString& sRight = wxEmptyString, double dButtonPercent=33)
        {
            m_bToggleLook = bLook;
            m_uiToggleLeft.SetLabel(sLeft);
            m_uiToggleRight.SetLabel(sRight);
            m_dToggleWidth = dButtonPercent;
            CreateRects();
            Refresh();
        }
        static const unsigned int STYLE_NORMAL = 0;     ///< A normal push button
        static const unsigned int STYLE_SELECT = 1;     ///< A toggle button - i.e. one that stays down when clicked until the next click
        static const unsigned int STYLE_HOLD   = 2;     ///< A button that will send an wxEVT_BUTTON_HELD event once it has been held down for a set time
        static const unsigned int STYLE_REPEAT = 4;     ///< A button that will send a click event every x milliseconds it is held down
        static const unsigned int STYLE_ARM    = 8;     ///< A button where the first click "arms" the button for a set time and the second click triggers the event
        //static const unsigned int STYLE_STICK   = 16;     ///< A button that once clicked can



  protected:
        static const int ID_TIMER_HOLD;
        static const int ID_TIMER_FLASH;


        /** Called to draw the console
        *   @param event
        **/
        void OnPaint(wxPaintEvent& event);

        /** Called when the console resizes
        *   @param event
        **/
        virtual void OnSize(wxSizeEvent& event);

        ///< @brief Called on the left mouse down being pressed
        void OnLeftDown(wxMouseEvent& event);
        ///< @brief Called on the left mouse button being double clicked
        void OnLeftDClick(wxMouseEvent& event);
        ///< @brief Called on the left mouse button being released
        void OnLeftUp(wxMouseEvent& event);

        void OnMouseMove(wxMouseEvent& event);
        ///< @brief Called if the left mouse button is held for a period of time
        void OnHolding(wxTimerEvent& event);

        ///< @brief Called if the button is flashing
        void OnFlash(wxTimerEvent& event);

        ///< @brief Called when mouse capture is lost
        void OnCaptureLost(wxMouseCaptureLostEvent& event);

        ///< @brief Sends a click event etc
        void SendEvent(wxEventType eventType);

        ///< @brief Creates the rectangles for the button
        void CreateRects();
        ///< @brief Draws the button
        virtual void Draw(wxDC& dc);
        virtual void DrawToggle(wxDC& dc);

        uiRect m_uiRect;            ///< @brief the internal uiRect
        unsigned int m_nStyle;      ///< @brief the style of the button

        wxTimer m_timerHold;        ///< @brief a timer for checking whether the button has been held or not
        unsigned int m_nHoldCount;  ///< @brief counter to see if we've held the button long enough
        unsigned int m_nHoldTrigger;///< @brief value the hold count must reach

        wxTimer m_timerFlash;       ///< @brief a timer for button flashing
        bool m_bFlashState;         ///< @brief the flash state

        wxColour m_clrDown[2];      ///< @brief the colour of a pressed/held button
        unsigned short m_nBorderState;///< @brief the state of the button border

        wxColour m_clrBackground[5];    ///< @brief the colour of the background for each button state
        wxColour m_clrForeground[5];    ///< @brief the colour of the foreground for each button state

        unsigned short m_nState;        ///< @brief the state of the button
        bool m_bChecked;                ///< @brief whether the button has been selected
        wxBitmap m_bmp[5];              ///< @brief the bitmap to draw in each button state
        unsigned int m_nBitmapAlign;    ///< @brief the alignment of the bitmap

        bool m_bToggleLook;
        uiRect m_uiGroove;
        uiRect m_uiToggleLeft;
        uiRect m_uiToggleRight;
        double m_dToggleWidth;

        wxString m_sAuxillary;

//        wmScroller* m_pScroller;

        static const unsigned short STATE_NORMAL    = 0;    ///< @brief button in normal state
        static const unsigned short STATE_SELECTED = 1;     ///< @brief button selected
        static const unsigned short STATE_ARMED    = 2;     ///< @brief button armed (pressed once)
        static const unsigned short STATE_DISABLED = 3;     ///< @brief button disabled
        static const unsigned short STATE_FLASH    = 4;     ///< @brief button flashing

};

DECLARE_EXPORTED_EVENT_TYPE(PAMBASE_IMPEXPORT, wxEVT_BUTTON_PRESSED,-1)
DECLARE_EXPORTED_EVENT_TYPE(PAMBASE_IMPEXPORT, wxEVT_BUTTON_RELEASED,-1)
DECLARE_EXPORTED_EVENT_TYPE(PAMBASE_IMPEXPORT, wxEVT_BUTTON_HELD,-1)
