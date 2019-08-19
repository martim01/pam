#pragma once
#include <wx/window.h>
#include <map>
#include <vector>
#include <set>
#include <list>
#include <queue>
#include <wx/timer.h>
#include "pmcontrol.h"
#include "advelement.h"


class wmSlideWnd;


/** @brief Window which displays a list or grid of buttons
**/
class PAMBASE_IMPEXPORT wmListAdv : public pmControl
{
    //DECLARE_DYNAMIC_CLASS(wmRouterList)
    DECLARE_EVENT_TABLE()

   public:
        ///< @brief Default constructor
        wmListAdv();
        ///< @brief Destructor
        virtual ~wmListAdv();

        /** @brief Constructor - simply calls Create
        *   @param sName the name of the control
        *   @param pParent the parent window
        *   @param id the wxWindowID of the control
        *   @param pos the top-left position of the control in its parent window
        *   @param size the size of the control
        *   @param nStyle the style of the list
        *   @param nScroll the direction in which scrolling is possible, either wmListAdv::SCROLL_NONE, wmListAdv::SCROLL_VERTICAL or wmListAdv::SCROLL_HORIZONTAL
        *   @param szButtons the size of the buttons in the list if the width is -1 then the width of the column is used
        *   @param nColumns the number of columns in the list
        *   @param szGap the size of the gap between buttons and columns
        **/
        wmListAdv(wxWindow* pParent, wxWindowID id, const wxPoint& pos = wxDefaultPosition,
            const wxSize& size = wxDefaultSize, unsigned int nStyle=0, unsigned short nScroll = SCROLL_NONE, const wxSize& szButtons=wxSize(-1,40), unsigned int nColumns=1, const wxSize& szGap=wxSize(1,1));

        /** @brief Creates the control and intiliazes it
        *   @param sName the name of the control
        *   @param pParent the parent window
        *   @param id the wxWindowID of the control
        *   @param pos the top-left position of the control in its parent window
        *   @param size the size of the control
        *   @param nStyle the style of the list
        *   @param nScroll the direction in which scrolling is possible, either wmListAdv::SCROLL_NONE, wmListAdv::SCROLL_VERTICAL or wmListAdv::SCROLL_HORIZONTAL
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


        size_t AddElement(advElement* pElement, bool bEnd=true);

        bool RemoveElement(size_t nIndex);

        enum Position{TOP=0, MIDDLE, BOTTOM, ONSCREEN};

        /** @brief Make sure a specified button is shown
        *   @param nIndex the index of the button to show
        *   @param pos the wmList::Position to show the button
        *   @param bSelect if true then the button will be selected as well and selection events triggered
        **/
        void ShowElement(size_t nIndex, Position pos);


        size_t FindFirstElement(void* pData);

        advElement* GetAdvElement(size_t nIndex);
        advElement* GetFirstAdvElement(void* pData);

        /** @brief Shows the next page or scrolls down a page
        **/
        void ShowNextPage();

        /** @brief Shows the previous page or scrolls up a page
        **/
        void ShowPreviousPage();

        /** @brief If the list is a horizontal scroll then this function will return the number of page of buttons currently showing
        *   @return <i>unsigned long</i>
        **/
        unsigned long GetCurrentPageNumber() const;

        /** @brief If the list is a horizontal scroll then this function will return the number of pages of buttons
        *   @return <i>unsigned long</i>
        **/
        unsigned long GetPageCount() const;

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

        /** @brief Allow the list box to update
        **/
        void Thaw();

        /** @brief Get the number of buttons in the list
        *   @return <i>size_t</i>
        **/
        size_t GetElementCount() const;


        void SelectSubElement(size_t nIndex, size_t nSub);

        wxRect GetSubElementRect(size_t nIndex, size_t nSub);
        void Sort();

        std::list<advElement*>::const_iterator GetElementBegin() const;
        std::list<advElement*>::const_iterator GetElementEnd() const;

        static const unsigned short SCROLL_NONE     = 0;        ///< @brief wmList may not be scrolled
        static const unsigned short SCROLL_VERTICAL = 1;        ///< @brief wmList may be scrolled vertically
        static const unsigned short SCROLL_HORIZONTAL = 2;      ///< @brief wmlist may be swiped left and right


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
        void OnGrowTimer(wxTimerEvent& event);
        void OnScroll(wxTimerEvent& event);
        void OnFlash(wxTimerEvent& event);

        void OnFocus(wxFocusEvent& event);
        void DrawHorizontalScroll();

        void OnCaptureLost(wxMouseCaptureLostEvent& event);

        void OnResizeElement(wxCommandEvent& event);
        void OnGrowElement(wxCommandEvent& event);

        void Draw(wxDC& dc, const std::list<advElement*>::iterator& itTop);
        void DrawElement(wxDC& dc, const std::list<advElement*>::iterator& itElement);

        void CreateRects();
        void GrowBelow();

        void ShowElement(std::list<advElement*>::iterator itButton, Position pos);

        std::list<advElement*>::iterator GetElement(size_t nIndex);

        void CreateSwipeBitmaps();
        void CheckSliding(wxPoint pnt);

        void WorkoutScrollPosition();

        std::list<advElement*> m_lstElements;

        std::set<std::list<advElement*>::iterator> m_setPages;
        std::list<advElement*>::iterator m_itDown;
        std::list<advElement*>::iterator m_itSelected;
        std::list<advElement*>::iterator m_itTop;
        std::list<advElement*>::iterator m_itLast;

        wxTimer m_timerHold;
        wxTimer m_timerGrow;

        static const long ID_HOLDING;
        static const long ID_GROW;

        unsigned int m_nStyle;
        unsigned short m_nScrollAllowed;

        wxPoint m_pntMouse;
        short m_nScrolling;
        short m_nSliding;
        bool m_bDownInWindow;

        wxTimer m_timerScroll;
        int m_nScrollOffset;
        int m_nSwipeOffset;

        wxBitmap m_bmpSwipe[2];
        wxBitmap m_bmpSlide;
        int m_nSwipeLeft;
        std::set<std::list<advElement*>::iterator>::iterator m_itSwipe;

        int m_nSwipeTotal;
        int m_nSwipeCount;
        size_t m_nIndex;

        static const long ID_SCROLLING;

        size_t m_nButtonsPerPage;

        wmSlideWnd* m_pSlideWnd;

        advElement* m_pGrowElement;
        bool m_bGrowing;

        int m_nSensitivity;
        wxSize m_szGap;
        uiRect m_uiScroll;
        unsigned int m_nTopElement;

        static const unsigned short SCROLL_HORIZONTAL_RIGHT = 3;
        static const unsigned short SCROLL_RETURN  = 4;
        static const unsigned short SCROLL_VERTICAL_END  = 5;


};

inline bool operator<(std::list<advElement*>::iterator it1, std::list<advElement*>::iterator it2)
{
    return ((*it1)->GetSort() < (*it2)->GetSort());
}

inline bool compare(advElement* pFirst, advElement* pSecond)
{
    return pFirst->GetSort() < pSecond->GetSort();
}

DECLARE_EXPORTED_EVENT_TYPE(WXEXPORT, wxEVT_LISTADV_SELECTED,-1)
DECLARE_EXPORTED_EVENT_TYPE(WXEXPORT, wxEVT_LISTADV_HELD,-1)
DECLARE_EXPORTED_EVENT_TYPE(WXEXPORT, wxEVT_LISTADV_PAGED,-1)
DECLARE_EXPORTED_EVENT_TYPE(WXEXPORT, wxEVT_LISTADV_MOVED,-1)



