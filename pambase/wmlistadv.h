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
#include <memory>
#include <functional>

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


        size_t AddElement(std::shared_ptr<advElement> pElement, bool bEnd=true, std::function<bool(std::shared_ptr<advElement>)> pFilter = nullptr);

        bool RemoveElement(size_t nIndex);

        enum Position{TOP=0, MIDDLE, BOTTOM, ONSCREEN};

        /** @brief Make sure a specified button is shown
        *   @param nIndex the index of the button to show
        *   @param pos the wmList::Position to show the button
        *   @param bSelect if true then the button will be selected as well and selection events triggered
        **/
        void ShowElement(size_t nIndex, Position pos);

        size_t FindFirstElement(void* pData);

        std::shared_ptr<advElement> GetFirstElement(void* pData);

        /** @brief Shows the next page or scrolls down a page
        **/
        void ShowNextPage();

        /** @brief Shows the previous page or scrolls up a page
        **/
        void ShowPreviousPage();


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

        const std::list<std::shared_ptr<advElement>>& GetElements() const;

        void Filter(std::function<bool(std::shared_ptr<advElement>)> pFilter);

        static const unsigned short SCROLL_NONE     = 0;        ///< @brief wmList may not be scrolled
        static const unsigned short SCROLL_VERTICAL = 1;        ///< @brief wmList may be scrolled vertically

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

        void OnResizeElement(wxCommandEvent& event);
        void OnHolding(wxTimerEvent& event);
        void OnScroll(wxTimerEvent& event);
        void OnFlash(wxTimerEvent& event);

        void OnFocus(wxFocusEvent& event);

        void OnCaptureLost(wxMouseCaptureLostEvent& event);

        void Draw(wxDC& dc);
        void DrawElement(wxDC& dc, std::shared_ptr<advElement> pElement);

        void CreateRects();

        void ShowElement(std::shared_ptr<advElement> pElement, Position pos);

        std::shared_ptr<advElement> GetElement(size_t nIndex);


        void WorkoutScrollPosition();

        std::list<std::shared_ptr<advElement>> m_lstElements;
        std::list<std::shared_ptr<advElement>> m_lstFilteredElements;

        std::shared_ptr<advElement> m_pDown;
        std::shared_ptr<advElement> m_pSelected;
        std::shared_ptr<advElement> m_pTop;

        wxTimer m_timerHold;

        static const long ID_HOLDING;

        unsigned int m_nStyle;
        unsigned short m_nScrollAllowed;

        wxPoint m_pntMouse;
        short m_nScrolling;
        bool m_bDownInWindow;

        wxTimer m_timerScroll;
        int m_nScrollOffset;

        size_t m_nIndex;

        static const long ID_SCROLLING;

        wxSize m_szGap;
        uiRect m_uiScroll;
        unsigned int m_nTopElement;

        static const unsigned short SCROLL_HORIZONTAL_RIGHT = 3;
        static const unsigned short SCROLL_RETURN  = 4;
        static const unsigned short SCROLL_VERTICAL_END  = 5;


};

inline bool operator<(std::list<std::shared_ptr<advElement>>::iterator it1, std::list<std::shared_ptr<advElement>>::iterator it2)
{
    return ((*it1)->GetSort() < (*it2)->GetSort());
}

inline bool compare(std::shared_ptr<advElement> pFirst, std::shared_ptr<advElement> pSecond)
{
    return pFirst->GetSort() < pSecond->GetSort();
}

DECLARE_EXPORTED_EVENT_TYPE(WXEXPORT, wxEVT_LISTADV_SELECTED,-1)
DECLARE_EXPORTED_EVENT_TYPE(WXEXPORT, wxEVT_LISTADV_HELD,-1)
DECLARE_EXPORTED_EVENT_TYPE(WXEXPORT, wxEVT_LISTADV_PAGED,-1)
DECLARE_EXPORTED_EVENT_TYPE(WXEXPORT, wxEVT_LISTADV_MOVED,-1)



