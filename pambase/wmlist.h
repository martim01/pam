#pragma once
#include <wx/window.h>
#include <map>
#include <vector>
#include <set>
#include <list>
#include <queue>
#include <wx/timer.h>
#include "uirect.h"
#include "pmcontrol.h"
#include "wmbuttonfactory.h"

class wmSlideWnd;


/** @brief Window which displays a list or grid of buttons
**/
class PAMBASE_IMPEXPORT wmList : public pmControl
{
    #ifdef WXSPAM
    DECLARE_DYNAMIC_CLASS(wmList)
    #else
    wxDECLARE_DYNAMIC_CLASS(wmList);
    #endif // WXSPAM
    DECLARE_EVENT_TABLE()

   public:
        ///< @brief Default constructor
        wmList();
        ///< @brief Destructor
        virtual ~wmList();

        /** @brief Constructor - simply calls Create
        *   @param sName the name of the control
        *   @param pParent the parent window
        *   @param id the wxWindowID of the control
        *   @param pos the top-left position of the control in its parent window
        *   @param size the size of the control
        *   @param nStyle the style of the list
        *   @param nScroll the direction in which scrolling is possible, either wmList::SCROLL_NONE, wmList::SCROLL_VERTICAL or wmList::SCROLL_HORIZONTAL
        *   @param szButtons the size of the buttons in the list if the width is -1 then the width of the column is used
        *   @param nColumns the number of columns in the list
        *   @param szGap the size of the gap between buttons and columns
        **/
        wmList(wxWindow* pParent, wxWindowID id, const wxPoint& pos = wxDefaultPosition,
            const wxSize& size = wxDefaultSize, unsigned int nStyle=0, unsigned short nScroll = SCROLL_NONE, const wxSize& szButtons=wxSize(-1,40), unsigned int nColumns=1, const wxSize& szGap=wxSize(1,1));

        /** @brief Creates the control and intiliazes it
        *   @param sName the name of the control
        *   @param pParent the parent window
        *   @param id the wxWindowID of the control
        *   @param pos the top-left position of the control in its parent window
        *   @param size the size of the control
        *   @param nStyle the style of the list
        *   @param nScroll the direction in which scrolling is possible, either wmList::SCROLL_NONE, wmList::SCROLL_VERTICAL or wmList::SCROLL_HORIZONTAL
        *   @param szButtons the size of the buttons in the list if the width is -1 then the width of the column is used
        *   @param nColumns the number of columns in the list
        *   @param szGap the size of the gap between buttons and columns
        **/
        bool Create(wxWindow* pParent, wxWindowID id, const wxPoint& pos = wxDefaultPosition,
            const wxSize& size = wxDefaultSize, unsigned int nStyle=0, unsigned short nScroll = SCROLL_NONE, const wxSize& szButtons=wxSize(-1,40), unsigned int nColumns=1, const wxSize& szGap=wxSize(1,1));

        /** @brief Returns the best size for the control for wxSizer
        *   @return <i>wxSize</i>
        **/
        wxSize DoGetBestSize() const;


        /** @brief Sets the sensitivity for horizontal scrolling. This is the number of pixels the user must horizontally move while touching the control in order for a horizontal slide to happen
        *   @param nPixels
        **/
        void SetSwipeSensitivity(unsigned long nPixels);

        /** @brief Adds a button to the list
        *   @param sButton the label to go on to the button
        *   @param bmp the bitmap to show
        *   @param pData any data to associate with the button
        *   @param nEnabled the state of the button, one of wmList::wmENABLED, wmlist::wmREADONLY, wmList::wmDISABLED, wmList::wmHIDDEN
        *   @param clrButton the colour of the button, if wxNullColour is passed then the default colour will be used
        *   @param clrButton the colour of the button when pressed, if wxNullColour is passed then the default colour will be used
        *   @param clrButton the colour of the button when selected, if wxNullColour is passed then the default colour will be used
        *   @return <i>size_t</i> The index of the added button
        **/
        size_t AddButton(const wxString& sButton, const wxBitmap& bmp = wxNullBitmap, void* pData=0, short nEnabled=wmENABLED, const wxColour& clrButton = wxNullColour, const wxColour& clrPressed = wxNullColour, const wxColour& clrSelected = wxNullColour);

        /** @brief Selects a button with the same label as sButton, triggering an event if bEvent=true
        *   @param sButton the label of the button to select
        *   @param bEvent if true then a wxEVT_LIST_SELECTED is generated
        **/
        void SelectButton(const wxString& sButton, bool bEvent=true);

        /** @brief Selects the button widht given index, triggering an event if bEvent=true
        *   @param nButton the index of the button to select
        *   @param bEvent if true then a wxEVT_LIST_SELECTED is generated
        **/
        void SelectButton(size_t nButton, bool bEvent=true);

        /** @brief Selects/Deselects all the buttons in the list (if wmList::STYLE_MULTI) is selected
        *   @param bSelect true to select, false to deselect
        *   @param bEvent if true then a wxEVT_LIST_SELECTED event is generated
        **/
        void SelectAll(bool bSelect=true, bool bEvent=true);


        /** @brief Deletes all button with the same clientdata as pData
        *   @param pData
        **/
        void DeleteButtons(void* pData);

        /** @brief Deletes the button with index nIndex
        *   @param nIndex the index of the button to delete
        **/
        void DeleteButton(size_t nIndex);

        /** @brief Finds the first button in the list with the same client data as pData
        *   @param pData the client data to search for
        *   @return <i>size_t</i> the index of the button or 0xFFFFFFFF if not found
        **/
        size_t FindButton(void* pData);

        /** @brief Finds the first button in the list with the same text as sLabel
        *   @param sLabel the text
        *   @return <i>size_t</i> the index of the button or 0xFFFFFFFF if not found
        **/
        size_t FindButton(const wxString& sLabel);

        /** @brief Sets the border style of the buttons
        *   @param nStyle, one of uiRect::BORDER_UP, uiRect::BORDER_DOWN, uiRect::BORDER_FLAT, uiRect::BORDER_NONE
        **/
        void SetBorderStyle(unsigned short nStyle);

        /** @brief Gets the border style of the buttons
        *   @return <i>unsigned short</i> one of uiRect::BORDER_UP, uiRect::BORDER_DOWN, uiRect::BORDER_FLAT, uiRect::BORDER_NONE
        **/
        unsigned short GetBorderStyle() const
        {
            return m_nBorderStyle;
        }

        /** @brief Sets the colour of the buttons
        *   @param clr
        **/
        void SetButtonColour(const wxColour& clr);

        /** @brief Sets the colour of a pressed button
        *   @param clr
        **/
        void SetPressedButtonColour(const wxColour& clr);

        /** @brief Sets the colour of a selected button
        *   @param clr
        **/
        void SetSelectedButtonColour(const wxColour& clr);


        /** @brief Sets the colour of all buttons with client data equal to pData
        *   @param pData the client data
        *   @param clr the colour to set clr is wxNullColour then the default colour is used
        **/
        void SetButtonsColour(void* pData, const wxColour& clr);

        /** @brief Sets the colour of a disabled button
        *   @param clr if clr is wxNullColour then the default colour is used
        **/
        void SetDisabledColour(const wxColour& clr);

        /** @brief Sets the colour of an individual button
        *   @param nButton the index of the button
        *   @param clr if clr is wxNullColour then the default colour is used
        **/
        void SetButtonColour(size_t nButton, const wxColour& clr);

        /** @brief Sets the colour of a pressed button
        *   @param nButton the index of the button
        *   @param clr if clr is wxNullColour then the default colour is used
        **/
        void SetPressedButtonColour(size_t nButton, const wxColour& clr);

        /** @brief Sets the colour of a selected button
        *   @param nButton the index of the button
        *   @param clr if clr is wxNullColour then the default colour is used
        **/
        void SetSelectedButtonColour(size_t nButton, const wxColour& clr);

        /** @brief Sets the colour of a flashing button
        *   @param nButton the index of the button
        *   @param clr if clr is wxNullColour then the default colour is used
        **/
        void SetFlashButtonColour(size_t nButton, const wxColour& clr);

        /** @brief Sets the default colour of a flashing button
        *   @param clr
        **/
        void SetFlashButtonColour(const wxColour& clr);

        /** @brief Sets the bitmap to display on the button if it is flashing
        *   @param nButton the index of the button
        *   @param bmp the bitmap to display
        **/
        void SetFlashButtonBitmap(size_t nButton, const wxBitmap& bmp);

        /** @brief Flashes a given button
        *   @param nIndex the index of the button
        *   @param bFlash true to flash, false to stop flashing
        **/
        void FlashButton(size_t nIndex, bool bFlash);

        /** @brief Sets the bitmap to display on a button
        *   @param nButton the index of the button
        *   @param bmp the bitmap to display
        **/
        void SetButtonBitmap(size_t nButton, const wxBitmap& bmp);


        /** @brief Sets the default colour of the text on the button
        *   @param clr
        **/
        void SetTextButtonColour(const wxColour& clr);

        /** @brief Sets the default colour of the text on the button if pressed
        *   @param clr
        **/
        void SetTextPressedButtonColour(const wxColour& clr);

        /** @brief Sets the default colour of the text on the button if selected
        *   @param clr
        **/
        void SetTextSelectedButtonColour(const wxColour& clr);

        /** @brief Sets the text colour of all buttons with client data equal to pData
        *   @param pData the client data
        *   @param clr the colour to set clr is wxNullColour then the default colour is used
        **/
        void SetTextButtonsColour(void* pData, const wxColour& clr);

        /** @brief Sets the text colour of an individual button
        *   @param nButton the index of the button
        *   @param clr the colour to set clr is wxNullColour then the default colour is used
        **/
        void SetTextButtonColour(size_t nButton, const wxColour& clr);

        /** @brief Sets the text colour of an individual button when pressed
        *   @param nButton the index of the button
        *   @param clr the colour to set clr is wxNullColour then the default colour is used
        **/
        void SetTextPressedButtonColour(size_t nButton, const wxColour& clr);

        /** @brief Sets the text colour of an individual button when selected
        *   @param nButton the index of the button
        *   @param clr the colour to set clr is wxNullColour then the default colour is used
        **/
        void SetTextSelectedButtonColour(size_t nButton, const wxColour& clr);

        /** @brief Sets the text colour of an individual button when flashing
        *   @param nButton the index of the button
        *   @param clr the colour to set clr is wxNullColour then the default colour is used
        **/
        void SetTextFlashButtonColour(size_t nButton, const wxColour& clr);

        /** @brief Sets the default colour of the text on the button if flashing
        *   @param clr
        **/
        void SetTextFlashButtonColour(const wxColour& clr);

        /** @brief Sets the default colour of the text on the button if disabled
        *   @param clr
        **/
        void SetTextDisabledButtonColour(const wxColour& clr);


        /** @brief Enable/Disable a specific button
        *   @param nButton the index of the button
        *   @param bEnable
        **/
        void EnableButton(size_t nButton, short nEnable=wmENABLED);

        bool Enable(bool bEnable = true);

        /** @brief Set the client data of a specified button
        *   @param nIndex the index of the button
        *   @param pData the client data
        **/
        void SetButtonData(size_t nIndex, void* pData);

        void SetButtonAuxillaryText(size_t nIndex, const wxString& sText);
        wxString GetButtonAuxillaryText(size_t nIndex);


        /** @brief enum to decide where to show a button - top, middle, button or simply onscreen
        **/
        enum Position{TOP=0, MIDDLE, BOTTOM, ONSCREEN};

        /** @brief Make sure a specified button is shown
        *   @param nIndex the index of the button to show
        *   @param pos the wmList::Position to show the button
        *   @param bSelect if true then the button will be selected as well and selection events triggered
        **/
        void ShowButton(size_t nIndex, Position pos, bool bSelect);

        /** @brief Make sure the first button with client data matching pdata is shown
        *   @param pData the cleint data
        *   @param pos the wmList::Position to show the button
        *   @param bSelect if true then the button will be selected as well and selection events triggered
        **/
        void ShowButton(void* pData, Position pos, bool bSelect);

        /** @brief Shows the next page or scrolls down a page
        *   @param bSelect if true then selects the first button on the screen
        *   @param bEvent if true and bSelect is also true then a select event will be posted
        **/
        void ShowNextPage(bool bSelect, bool bEvent);

        void ShowFirstPage(bool bSelect, bool bEvent);
        void ShowLastPage(bool bSelect, bool bEvent);

        /** @brief Shows the previous page or scrolls up a page
        *   @param bSelect if true then selects the last button on the screen
        *   @param bEvent if true and bSelect is also true then a select event will be posted
        **/
        void ShowPreviousPage(bool bSelect, bool bEvent);

        /** @brief Selects the next button after the already selected one. If no buttons are selected then selects the first button in the list
        *   @param bShow if true then makes sure the button is visible
        *   @param bEvent if true then a select event is generated
        **/
        void SelectNextButton(bool bShow, bool bEvent);

        /** @brief Selects the previous button before the already selected one. If no buttons are selected then selects the last button in the list
        *   @param bShow if true then makes sure the button is visible
        *   @param bEvent if true then a select event is generated
        **/
        void SelectPreviousButton(bool bShow, bool bEvent);

        /** @brief If the list is a horizontal scroll then this function will return the number of page of buttons currently showing
        *   @return <i>unsigned long</i>
        **/
        unsigned long GetCurrentPageNumber() const;

        /** @brief If the list is a horizontal scroll then this function will return the number of pages of buttons
        *   @return <i>unsigned long</i>
        **/
        unsigned long GetPageCount() const;

        /** @brief Selects the first button in the list
        *   @param bShow if true then shows the button on the screen
        *   @param bEvent if true then an selection event is generated
        **/
        void SelectFirstButton(bool bShow, bool bEvent);

        /** @brief Selects the last button in the list
        *   @param bShow if true then shows the button on the screen
        *   @param bEvent if true then an selection event is generated
        **/
        void SelectLastButton(bool bShow, bool bEvent);

        /** @brief Sets a button to be slidable.
        *   @param nIndex the index of the button
        *   @param nSlide the direction the button can be slid in. One of wmList::SLID_NONE, wmList::SLID_LEFT, wmList::SLID_RIGHT, wmList::SLID_UP, wmList::SLID_DOWN. Note the slide cannot be in the same plane as the scroll
        *   @param sLabel sets the text to be displayed when the button is slid. If empty then the button text is shown
        *   @param bShowArrow if true then an arrow bitmap is shown in the direction of possible slide
        **/
        void SetSlideAllowed(size_t nIndex, short nSlide, const wxString& sLabel = wxEmptyString, bool bShowArrow = true);


        /** @brief Removes all buttons from the list
        **/
        void Clear();

        /** @brief Sets the size of the list control
        *   @param size
        **/
        void SetSize(const wxSize& size);

        /** @brief Scrolls the list vertically
        *   @param nYDiff the number of pixels to scroll by
        *   @return <i>bool</i> true if scrolled, false if the end has been hit
        **/
        bool ScrollVertical(int nYDiff);


        /** @brief Scrolls the list horizontally
        *   @param nYDiff the number of pixels to scroll by
        *   @return <i>bool</i> true if scrolled, false if the end has been hit
        **/
        bool ScrollHorizontal(int nXDiff);

        /** @brief Set the text alignment
        *   @param nAlign
        **/
        void SetTextAlign(unsigned int nAlign);

        /** @brief Set the bitmap alignment
        *   @param nAlign
        **/
        void SetBitmapAlign(unsigned int nAlign);

        /** @brief Allow the list box to update
        **/
        void Thaw();


        /** @brief Sets the wmButtonFactory to use to create the buttons for wmList. By default uiRect buttons are created. By creating a new class derived from uiRect and a factory derived from wmButtonFactory you can change the appearance of the buttons that appear in the list
        *   @param pFactory
        **/
        void SetButtonFactory(wmButtonFactory* pFactory);

        uiRect* GetButtonuiRect(size_t nButton);

        /** @brief Set the colouring gradient direction for the buttons
        *   @param nGradient one of wxEAST, wxWEST, wxNORTH, wxSOUTH
        **/
        void SetGradient(unsigned int nGradient);

        /** @brief Get colouring gradient direction
        *   @return <i>unsigned int</i> one of wxEAST, wxWEST, wxNORTH, wxSOUTH
        **/
        unsigned int GetGradient() const
        {
            return m_nGradient;
        }

        /** @brief Get the number of buttons that are selected
        *   @return <i>size_t</i>
        **/
        size_t GetSelectionCount() const;

        /** @brief Get the number of buttons in the list
        *   @return <i>size_t</i>
        **/
        size_t GetItemCount() const;

        /** @brief Returns the max index of the buttons in the list - this may be greater than the size of the list if buttons have been deleted
        *   @return <i>size_t</i>
        **/
        size_t GetMaxIndex() const;

        /** @brief Returns an array of indices of the buttons that are currently selected
        *   @return <i>wxArrayInt</i>
        **/
        wxArrayInt GetSelectedButtons() const;

        /** @brief Returns true if the given button is selected
        *   @param nButton the index of the button
        *   @return <i>bool</i>
        **/
        bool IsSelected(size_t nButton) const;

        /** @brief Returns an array containing the indices of all the buttons in the list
        *   @param bIncludeHidden if false then buttons which are wmList::wmHIDDEN will be ignored
        *   @param bIncludeDisable if false the buttons which are disabled wll be ignored
        *   @return <i>wxArrayInt</i>
        **/
        wxArrayInt GetButtonIndexes(bool bIncludeHidden=false, bool bIncludeDisabled=true) const;

        /** @brief Return the text of a given button
        *   @param nButton the index of the button
        *   @return <i>wxString</i>
        **/
        wxString GetButtonText(size_t nButton) const;

        /** @brief Return the client data of a given button
        *   @param nButton the index of the button
        *   @return <i>void*</i>
        **/
        void* GetButtonData(size_t nButton);

        /** @brief Set the text for a particular button
        *   @param nButton the index of the button
        *   @param sText the text to put on the button
        **/
        void SetButtonText(size_t nButton, const wxString& sText);

        /** @brief Deselects all buttons
        **/
        void ClearSelection();

        /** @brief Sets the height in pixels of the buttons
        *   @param nHeight
        **/
        void SetButtonHeight(unsigned int nHeight);

//        /** @brief Returns the number of buttons that
//        size_t GetButtonsPerPage() const
//        {
//            return m_nButtonsPerPage;
//        }

        /** @brief Returns the button colour of a particular button
        *   @param nIndex the button index
        *   @return <i>wxColour</i>
        **/
        wxColour GetButtonColour(size_t nIndex) const;

        /** @brief Returns the button colour of a particular button when pressed
        *   @param nIndex the button index
        *   @return <i>wxColour</i>
        **/
        wxColour GetButtonPressedColour(size_t nIndex) const;

        /** @brief Returns the button colour of a particular button selected
        *   @param nIndex the button index
        *   @return <i>wxColour</i>
        **/
        wxColour GetButtonSelectedColour(size_t nIndex) const;

        /** @brief Returns the button colour of a particular button when flashing
        *   @param nIndex the button index
        *   @return <i>wxColour</i>
        **/
        wxColour GetButtonFlashColour(size_t nIndex) const;

        /** @brief Returns the text colour of a particular button
        *   @param nIndex the button index
        *   @return <i>wxColour</i>
        **/
        wxColour GetButtonTextColour(size_t nIndex) const;

        /** @brief Returns the text colour of a particular button when pressed
        *   @param nIndex the button index
        *   @return <i>wxColour</i>
        **/
        wxColour GetButtonTextPressedColour(size_t nIndex) const;

        /** @brief Returns the text colour of a particular button when selected
        *   @param nIndex the button index
        *   @return <i>wxColour</i>
        **/
        wxColour GetButtonTextSelectedColour(size_t nIndex) const;

        /** @brief Returns the text colour of a particular button when flashing
        *   @param nIndex the button index
        *   @return <i>wxColour</i>
        **/
        wxColour GetButtonTextFlashColour(size_t nIndex) const;


        /** @brief Returns the default button colour
        *   @return <i>wxColour</i>
        **/
        wxColour GetButtonsColour() const;

        /** @brief Returns the default pressed button colour
        *   @return <i>wxColour</i>
        **/
        wxColour GetButtonsPressedColour() const;

        /** @brief Returns the default selected button colour
        *   @return <i>wxColour</i>
        **/
        wxColour GetButtonsSelectedColour() const;

        /** @brief Returns the default flashing button colour
        *   @return <i>wxColour</i>
        **/
        wxColour GetButtonsFlashColour() const;

        /** @brief Returns the default disabled button colour
        *   @return <i>wxColour</i>
        **/
        wxColour GetButtonsDisabledColour() const;

        /** @brief Returns the default text colour
        *   @return <i>wxColour</i>
        **/
        wxColour GetButtonsTextColour() const;

        /** @brief Returns the default text colour of a pressed button
        *   @return <i>wxColour</i>
        **/
        wxColour GetButtonsTextPressedColour() const;

        /** @brief Returns the default text colour of a selected button
        *   @return <i>wxColour</i>
        **/
        wxColour GetButtonsTextSelectedColour() const;

        /** @brief Returns the default text colour of a flashing button
        *   @return <i>wxColour</i>
        **/
        wxColour GetButtonsTextFlashColour() const;

        /** @brief Returns the enabled status of a particular button
        *   @param nIndex the button index
        *   @return <i>unsigned short</i> one of wmList::wmENABLED, wmList::wmREADONLY, wmList::wmDISABLED, wmList::wmHIDDEN
        **/
        unsigned short GetEnabled(size_t nIndex) const;

        /** @brief Returns the direction a particular button can be slid in
        *   @param nIndex the button index
        *   @return <i>short</i> one of wmList::SLID_NONW, wmList::SLID_LEFT, wmList::SLID_RIGHT, wmList::SLID_UP, wmList::SLID_DOWN
        **/
        short GetSlideDirection(size_t nIndex) const;

        /** @brief Returns the text that will be on a particular button when slid. Will return an empty string if the text is the same as the button when non-slid
        *   @param nIndex the button index
        *   @return <i>wxString</i>
        **/
        wxString GetSlideText(size_t nIndex) const;

        void SetColumns(unsigned int nColumns);
        void AllowScroll(int nScroll)
        {   m_nScrollAllowed = nScroll; }

        static const unsigned short SCROLL_NONE     = 0;        ///< @brief wmList may not be scrolled
        static const unsigned short SCROLL_VERTICAL = 1;        ///< @brief wmList may be scrolled vertically
        static const unsigned short SCROLL_HORIZONTAL = 2;      ///< @brief wmlist may be swiped left and right

        /** @brief enum of button states.
        **/
        enum { wmDISABLED = 0, wmENABLED, wmREADONLY, wmHIDDEN};

        /** @brief enum of button slide directions
        **/
        enum { SLID_NONE = 0, SLID_LEFT=1, SLID_RIGHT=2, SLID_UP=4, SLID_DOWN=8};

        static const unsigned int STYLE_NORMAL = 0;             ///< @brief default style
        static const unsigned int STYLE_SELECT = 1;             ///< @brief buttons may be selected (i.e. stay down when pressed)
        static const unsigned int STYLE_HOLD   = 2;             ///< @brief buttons may be held and will trigger an event
        static const unsigned int STYLE_RESIZE_BUTTONS = 4;     ///< @brief buttons will be resized to fit the control best
        static const unsigned int STYLE_SELECT_MULTI  = 16;     ///< @brief allow multiple selection
        static const unsigned int STYLE_SELECT_ROW  = 32;       ///< @brief select row of buttons not single one (if more than one column)
        static const unsigned int STYLE_CIRCULAR  = 64;         ///< @brief makes the list circular - i.e scrolling to the end will bring you back to the beginning

        static const unsigned long NOT_FOUND = 0xFFFFFFFF;

        struct button
        {
            button(uiRect* pU) : pUi(pU), nEnabled(true), bFlashing(false), nSlideAllowed(SLID_NONE){}
            //button(const button& btn) : pUi(), clrButton(btn.clrButton), clrSelected(btn.clrSelected), clrPressed(btn.clrPressed), nEnabled(btn.nEnabled){}
            ~button(){ delete pUi;}

            uiRect* pUi;
            wxColour clrButton;
            wxColour clrSelected;
            wxColour clrPressed;
            wxColour clrFlashing;

            wxColour clrText;
            wxColour clrTextSelected;
            wxColour clrTextPressed;
            wxColour clrTextFlashing;

            wxBitmap bmp[2];

            short nEnabled;
            bool bFlashing;
            short nSlideAllowed;
            wxString sSlide;
            wxString sAuxillary;
        };
    protected:
        /** Called to draw the control
        *   @param event
        **/
        void OnPaint(wxPaintEvent& event);

        /** Called when the control resizes
        *   @param event
        **/
        void OnSize(wxSizeEvent& event);

        void OnLeftDown(wxMouseEvent& event);
        void OnLeftUp(wxMouseEvent& event);
        void OnMouseMove(wxMouseEvent& event);

        void OnHolding(wxTimerEvent& event);
        void OnScroll(wxTimerEvent& event);
        void OnFlash(wxTimerEvent& event);

        void OnFocus(wxFocusEvent& event);
        void DrawHorizontalScroll();

        void OnCaptureLost(wxMouseCaptureLostEvent& event);



        void Draw(wxDC& dc, const std::list<button*>::iterator& itTop);
        void DrawButton(wxDC& dc, const std::list<button*>::iterator& itButton);

        void CreateRects();
        void CreateSwipeBitmaps();
        void CreateSlideBitmap();

        void CheckSliding(wxPoint pnt);

        void SetDefaultFactory();

        std::list<button*>::iterator GetButton(size_t nButton);
        std::list<button*>::iterator GetButton(void* pData);
        void SelectButton(std::list<button*>::iterator itSel, bool bEvent=true);

        void ShowButton(std::list<button*>::iterator itButton, Position pos, bool bSelect);

        std::list<button*> m_lstButtons;
        std::set<std::list<button*>::iterator> m_setitSelected;

        std::list<wxColour> m_lstColours[3];

        std::set<std::list<button*>::iterator> m_setPages;
        std::list<button*>::iterator m_itDown;
        std::list<button*>::iterator m_itTop;
        std::list<button*>::iterator m_itLast;

        std::set<std::list<button*>::iterator>  m_setitFlash;

        unsigned int m_nStyle;
        unsigned short m_nScrollAllowed;

        wxTimer m_timerHold;
        unsigned int m_nHoldCount;

        wxColour m_clrDown;

        wxColour m_clrButton;
        wxColour m_clrSelectedButton;
        wxColour m_clrFlash;
        wxColour m_clrText;
        wxColour m_clrTextSelected;
        wxColour m_clrTextFlash;
        wxColour m_clrTextPressed;
        wxColour m_clrPressedButton;
        wxColour m_clrDisabledButton;

        wxPoint m_pntMouse;
        short m_nScrolling;
        short m_nSliding;


        bool m_bDownInWindow;

        wxTimer m_timerScroll;
        int m_nScrollOffset;
        int m_nSwipeOffset;

        wxTimer m_timerFlash;
        unsigned int m_nFlashTime;

        wxSize m_szButtons;
        unsigned int m_nColumns;
        wxSize m_szGap;

        unsigned short m_nBorderStyle;

        unsigned int m_nTextAlign;
        unsigned int m_nBitmapAlign;

        wxBitmap* m_pbmpSwipe[2];
        wxBitmap m_bmpSlide;
        int m_nSwipeLeft;
        std::set<std::list<button*>::iterator>::iterator m_itSwipe;

        int m_nSwipeTotal;
        int m_nSwipeCount;
        unsigned int m_nGradient;

        size_t m_nIndex;

        wmButtonFactory* m_pFactory;

        static const long ID_HOLDING;
        static const long ID_SCROLLING;
        static const long ID_FLASHING;

        size_t m_nButtonsPerPage;

        wmSlideWnd* m_pSlideWnd;

        int m_nSensitivity;


        static const unsigned short SCROLL_HORIZONTAL_RIGHT = 3;
        static const unsigned short SCROLL_RETURN  = 4;
        static const unsigned short SCROLL_VERTICAL_END  = 5;




};

inline bool operator<(std::list<wmList::button*>::iterator it1, std::list<wmList::button*>::iterator it2)
{
    return ((*it1)->pUi->GetIndex() < (*it2)->pUi->GetIndex());
}

#ifdef WXSPAM
DECLARE_EXPORTED_EVENT_TYPE(WXEXPORT, wxEVT_LIST_SELECTED, -1)
DECLARE_EXPORTED_EVENT_TYPE(WXEXPORT, wxEVT_LIST_HELD, -1)
DECLARE_EXPORTED_EVENT_TYPE(WXEXPORT, wxEVT_LIST_PAGED, -1)
DECLARE_EXPORTED_EVENT_TYPE(WXEXPORT, wxEVT_LIST_SLID, -1)
#else
wxDECLARE_EXPORTED_EVENT(PAMBASE_IMPEXPORT, wxEVT_LIST_SELECTED, wxCommandEvent);
wxDECLARE_EXPORTED_EVENT(PAMBASE_IMPEXPORT, wxEVT_LIST_HELD, wxCommandEvent);
wxDECLARE_EXPORTED_EVENT(PAMBASE_IMPEXPORT, wxEVT_LIST_PAGED, wxCommandEvent);
wxDECLARE_EXPORTED_EVENT(PAMBASE_IMPEXPORT, wxEVT_LIST_SLID, wxCommandEvent);
#endif // WXSPAM
