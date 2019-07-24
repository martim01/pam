#pragma once
#include <wx/panel.h>
#include <vector>
#include <wx/timer.h>
#include "uirect.h"
#include "pmpanel.h"

/**
*   @brief wxWidgets control which allows switching between different pages
*   @author Matthew Martin
*   @version 0.1
*   @date 2008
*
*
*
**/


class PAMBASE_IMPEXPORT wmSwitcherPanel : public pmPanel
{
    #ifdef WXSPAM
    DECLARE_DYNAMIC_CLASS(wmSwitcherPanel)
    #else
    wxDECLARE_DYNAMIC_CLASS(wmSwitcherPanel);
    #endif // WXSPAM
    DECLARE_EVENT_TABLE()
    public:
        wmSwitcherPanel();
        virtual ~wmSwitcherPanel();

    /** @brief Constructor
    *   Creates the wxWidget control
    *   @param pParent the parent window of the control
	*   @param id the unique id of the control
	*   @param pos the top left position of the control
	*   @param size the width and height of the control
	*   @param nStyle the style of the control see @ref wmSwitcherPanel_Styles
	**/
    wmSwitcherPanel(wxWindow* pParent, wxWindowID id, const wxPoint& pos = wxDefaultPosition,
            const wxSize& size = wxDefaultSize, long nStyle=0, const wxString& sName=wxEmptyString);

    /** @brief Creates the wxWidget control
    *   @param pParent the parent window of the control
	*   @param id the unique id of the control
	*   @param pos the top left position of the control
	*   @param size the width and height of the control
	*   @param nStyle the style of the control see @ref wmSwitcherPanel_Styles
	**/
	 bool Create(wxWindow* pParent, wxWindowID id, const wxPoint& pos = wxDefaultPosition,
            const wxSize& size = wxDefaultSize, long nStyle=0);


    /** @brief Adds a panel to the control
    *   @param pPage the panel to add
    *   @param sName the name of the panel
    *   @param bSelect true to make the new page visible
    **/
    void AddPage(wxWindow* pPage, const wxString& sName, bool bSelect=true);

    /** @brief Shows the previous or next page
    *   @param bForward true to show the next page, false to show the previous
    *   @param bLoop if true then will loop to the first page when past the last
    **/
    void AdvanceSelection(bool bForward = true, bool bLoop = false);

    /** @brief Removes all pages from the panel - but does not delete them
    **/
    void DeleteAllPages();

    /** @brief Removes a page from the panel - but does not delete it
    *   @param nPage the number of the page to remove
    **/
    void DeletePage(size_t nPage);

    /** @brief Removes a page from the panel - but does not delete it
    *   @param sName the page of the panel to delete
    **/
    void DeletePage(const wxString& sName);

    /** @brief Returns the number of pages in the control
    *   @return <i>size_t</i>
    **/
    size_t GetPageCount();

    /** @brief Returns the number of the currently selected page
    *   @return <i>size_t</i>
    **/
    size_t GetSelection();

    /** @brief Return the nPage panel
    *   @param nPage
    *   @return <i>wxWindow*</i>
    **/
    wxWindow* GetPage(size_t nPage);

    /** @brief Returns the name of the currently selected page
    *   @return <i>wxString</i>
    **/
    wxString GetSelectionName();

    /** @brief Gets the page label
    *   @param nPage the page
    *   @return <i>wxString</i> the label
    **/
    wxString GetPageText(size_t nPage);

    /** @brief Inserts a page into the control
    *   @param nPage the position to insert the page
    *   @param pPage the panel to insert
    *   @param sName the name of the panel
    *   @param bSelect true to make the new page visible
    **/
    void InsertPage(size_t nPage, wxWindow* pPage, const wxString& sName, bool bSelect = true);

    /** @brief Makes a page visible
    *   @param nPage the page to make visible
    *   @return <i>size_t</i> the number of the previously visible page
    **/
    size_t ChangeSelection(size_t nPage);

    /** @brief Makes a page visible
    *   @param sPage the name of the page to make visible
    *   @return <i>wxString</i> the name of the previously visible page
    **/
    wxString ChangeSelection(const wxString& sPage);

    /** @brief Sets the style of the page name
    *   @param nStyle -
    **/
    void SetPageNameStyle(unsigned long nStyle);

    /** @brief Sets whether the page selection can be changed by swiping the window
    *   @param bDisable true to disable swiping
    **/
    void DisableSwipe(bool bDisable);

    /** @brief Sets whether the page selection changing should be animated or not
    *   @param bDisable true to disable animation
    **/
    bool DisableAnimation(bool bDisable);

    /** @brief Sets the direction of swiping and animation either wmSwitcherPanel::STYLE_VERTICAL or wmSwitcherPanel::STYLE_HORIZONTAL
    *   @param nDirection
    **/
    void SetSwipeDirection(unsigned int nDirection);


    static const unsigned short SCROLL_NONE     = 0;
    static const unsigned short SCROLL_HORIZONTAL = 1;
    static const unsigned short SCROLL_VERTICAL = 2;

    /** @brief enum of possible ways of displaying the names of the pages of the switcher panel
    **/
    enum PageNameStyle{PAGE_HIDDEN = 0, ///< @brief Show no names
                        PAGE_NONE,      ///< @brief Show page name but no borders
                        PAGE_FLAT,      ///< @brief Show page names with a flat border
                        PAGE_BUTTON     ///< @brief Show page names as a button
                        };

    static const unsigned long STYLE_NOSWIPE = 1;
    static const unsigned long STYLE_NOANIMATION = 2;
    static const unsigned long STYLE_VERTICAL = 4;
    static const unsigned long STYLE_HORIZONTAL = 0;

    private:
        void OnSize(wxSizeEvent& event);
        void OnPaint(wxPaintEvent& event);
        void OnLeftDown(wxMouseEvent& event);
        void OnLeftUp(wxMouseEvent& event);
        void OnScroll(wxTimerEvent& event);

        void DrawScroll();
        bool ScrollHorizontal(int nXDiff);
        bool ScrollVertical(int nYDiff);
        void ResizePages();

        void ResetCurrentSwipe();
        void SwipeFinished();
        struct page
        {
            page(const wxString& sp, wxWindow* pp) : sPage(sp), pPanel(pp){}
            wxString sPage;
            wxWindow* pPanel;
            uiRect uiLabel;
            //wxRect rectLabel;
        };
        std::vector<page> m_vPages;
        size_t m_nSelectedPage;
        size_t m_nSwipePage;
        int m_nSwipeLeft;
        unsigned short m_nScrollAllowed;
        wxPoint m_pntMouse;
        short m_nScrolling;
        wxTimer m_timerScroll;
        int m_nSwipeOffset;
        bool m_bDownInWindow;


        unsigned int m_nSwipeHeight;
        unsigned int m_nNameHeight;
        wxRect m_rectSwipe;

        int m_nSwipeTotal;
        int m_nSwipeCount;

        unsigned long m_nPageNameStyle;
        unsigned long m_nStyle;


        static const long ID_SCROLLING;

        static const unsigned short SCROLL_HORIZONTAL_RIGHT = 3;

        static const unsigned short SCROLL_RETURN  = 4;
        static const unsigned short SCROLL_VERTICAL_DOWN = 5;

};


