#pragma once
#include <wx/window.h>
#include <wx/timer.h>
#include "uirect.h"
#include <wx/bitmap.h>
#include "pmcontrol.h"

/** @class a class that draws a lable on the screen, derives from wxWindow
**/
class PAMBASE_IMPEXPORT wmLabel : public pmControl
{
    DECLARE_EVENT_TABLE()
    #ifdef WXSPAM
    DECLARE_DYNAMIC_CLASS(wmLabel)
    #else
        wxDECLARE_DYNAMIC_CLASS(wmLabel);
    #endif
    public:

        /** @brief default constructor
        **/
        wmLabel();

        /** @brief Constructor - made to be the same as a wxStaticText
        *   @param parent pointer to the parent window
        *   @param id the wxWindowId of the label
        *   @param label the text to write in the label
        *   @param pos the top-left point of the label
        *   @param size the width and height of the label
        *   @param style not currently used
        *   @param name not currently used
        **/
        wmLabel(wxWindow *parent,
                 wxWindowID id,
                 const wxString& label = wxEmptyString,
                 const wxPoint& pos = wxDefaultPosition,
                 const wxSize& size = wxDefaultSize,
                 long style = 0, const wxString& name =wxT("label"));

        /** @brief Creates the label
        *   @param parent pointer to the parent window
        *   @param id the wxWindowId of the label
        *   @param label the text to write in the label
        *   @param pos the top-left point of the label
        *   @param size the width and height of the label
        *   @param style not currently used
        *   @param name not currently used
        **/
        bool Create(wxWindow *parent,
                    wxWindowID id,
                    const wxString& label = wxEmptyString,
                    const wxPoint& pos = wxDefaultPosition,
                    const wxSize& size = wxDefaultSize,
                    long style = 0, const wxString& name = wxT("label"));


        /** @brief Constructor - made to be the same as a wxStaticBitmap
        *   @param parent pointer to the parent window
        *   @param id the wxWindowId of the label
        *   @param bitmap the wxBitmap to draw on the label
        *   @param pos the top-left point of the label
        *   @param size the width and height of the label
        *   @param style not currently used
        *   @param validator not used
        *   @param name not used
        **/
        wmLabel( wxWindow* parent, wxWindowID id, const wxBitmap& bitmap, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxDefaultSize, long style = 0, const wxString& name = wxT("label"));

        /** @brief creates the button - made to be the same as a wxBitmapButton
        *   @param parent pointer to the parent window
        *   @param id the wxWindowId of the label
        *   @param bitmap the wxBitmap to draw on the label
        *   @param pos the top-left point of the label
        *   @param size the width and height of the label
        *   @param style not currently used
        *   @param validator not used
        *   @param name not used
        **/
        bool  Create(wxWindow* parent, wxWindowID id, const wxBitmap& bitmap, const wxPoint& pos, const wxSize& size = wxDefaultSize, long style = 0, const wxString& name = wxT("label"));

        virtual ~wmLabel();


        /** @brief Sets the background colour of the label when it is flashing
        *   @param colour
        **/
        void SetColourFlash(const wxColour &colour);

        /** @brief Sets the text colour of the label when it is flashing
        *   @param colour
        **/
        void SetTextColourFlash(const wxColour &colour);

        /** @brief Sets the text to draw on the label
        *   @param sLabel
        **/
        void SetLabel(const wxString& sLabel);

        /** @brief Set the alignment of the text in the label
        *   @param nAlign, a union of one of wxALIGN_LEFT,wxALIGN_CENTER_HORIZONTAL, wxALIGN_RIGHT and one of wxALIGN_TOP, wxALIGN_BOTTOM, wxALIGN_CENTER_VERTICAL
        **/
        void SetTextAlign(unsigned int nAlign);

        /** @brief Set the alignment of the bitmap in the label
        *   @param nAlign, a union of one of wxALIGN_LEFT,wxALIGN_CENTER_HORIZONTAL, wxALIGN_RIGHT and one of wxALIGN_TOP, wxALIGN_BOTTOM, wxALIGN_CENTER_VERTICAL
        **/
        void SetBitmapAlign(unsigned int nAlign);

        /** @brief Set the border state of the label
        *   @param
        **/
        void SetBorderState(unsigned short nState);

        /** @brief Sets the bitmap to draw on the label when it is in its normal state
        *   @param bitmap
        **/
        void SetBitmap(const wxBitmap& bitmap);

        /** @brief Sets the bitmap to draw on the label when it is flashing
        *   @param bitmap
        **/
        void SetBitmapFlash(const wxBitmap& bitmap);

        /** @brief Get the bitmap that is drawn when the label is in its normal state
        *   @return <i>wxBitmap</i>
        **/
        const wxBitmap& GetBitmap() const;

        /** @brief Get the bitmap that is drawn when the label is flashing
        *   @return <i>wxBitmap</i>
        **/
        const wxBitmap& GetBitmapFlash() const;


        /** @brief Make the label start/stop flashing
        *   @param bFlash if true the label will "flash", changing between its current colour etc and the set flash colour etc
        *   @param nInterval the speed of the flash in milliseconds
        **/
        void Flash(bool bFlash, unsigned int nInterval=500);

        /** @brief Get the text drawn on the label
        *   @return <i>wxString</i>
        **/
        wxString GetLabel() const
        {
            return m_uiRect.GetLabel();
        }

        /** @brief Sets the background colour of the label
        *   @param colour
        *   @return <i>bool</i> true
        **/
        bool SetBackgroundColour(const wxColour &colour);

        /** @brief Sets the text colour of the label
        *   @param colour
        *   @return <i>bool</i> true
        **/
        bool SetForegroundColour(const wxColour &colour);

        /** @brief returns the default size of the label for sizers
        **/
        wxSize DoGetBestSize() const
        {
            return wxSize(60,25);
        }

        uiRect& GetUiRect();

  protected:

        static const int ID_TIMER_FLASH;


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

        uiRect m_uiRect;
        unsigned int m_nStyle;

        wxTimer m_timerHold;
        unsigned int m_nHoldCount;
        unsigned int m_nHoldTrigger;

        wxTimer m_timerFlash;
        bool m_bFlashState;
        wxColour m_clrFlashBack;
        wxColour m_clrFlashText;
        wxBitmap m_bmpFlash;

        unsigned short m_nBorderState;

        wxColour m_clrBackground;
        wxColour m_clrForeground;

        unsigned short m_nState;
        bool m_bChecked;
        wxBitmap m_bmp;
        unsigned int m_nBitmapAlign;







};

DECLARE_EXPORTED_EVENT_TYPE(PAMBASE_IMPEXPORT, wxEVT_BUTTON_PRESSED,-1)
DECLARE_EXPORTED_EVENT_TYPE(PAMBASE_IMPEXPORT, wxEVT_BUTTON_RELEASED,-1)
DECLARE_EXPORTED_EVENT_TYPE(PAMBASE_IMPEXPORT, wxEVT_BUTTON_HELD,-1)

