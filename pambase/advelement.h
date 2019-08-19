#pragma once
#include <wx/event.h>
#include <wx/dc.h>
#include "uirect.h"
#include "dlldefine.h"
#include <map>


class PAMBASE_IMPEXPORT advElement : public wxEvtHandler
{
    public:
        advElement(const wxRect& rect=wxRect(0,0,0,0))  :  m_nIndex(0), m_pParent(0), m_rectEnclosing(rect), m_sizeMin(rect.GetWidth(), rect.GetHeight()), m_sizeMax(rect.GetWidth(), rect.GetHeight()), m_nHoldCount(0)
        {
            m_itDown = m_mHitRects.end();
            m_itSelected = m_mHitRects.end();
            CreateHitRects();
        }

        virtual ~advElement(){}

        virtual void Destroy()=0;

        void SetParent(wxWindow* pParent)
        {
            m_pParent = pParent;
        }

        void DrawMe(wxDC& dc, bool bSelected);

        int LeftDown(wxMouseEvent& event);
        int LeftUp(wxMouseEvent& event, bool bScrolling);
        int MouseMove(wxMouseEvent& event);
        int MouseHeld();

        wxRect GetSubRect(unsigned long nSub) const;

        virtual int GetSort()
        {
            return m_nIndex;
        }

        /** @brief Set the initial enclosing rectangle
        *   @param rect the enclosing rectangle
        **/
        void SetRect(const wxRect& rect);

        /** @brief Set the initial enclosing rectangle
        *   @param nLeft
        *   @param nTop
        *   @param nWidth
        *   @param nHeight
        **/
        void SetRect(int nLeft, int nTop, int nWidth, int nHeight);


        /** @brief Set the min enclosing rectangle
        *   @param rect the enclosing rectangle
        **/
        void SetMinSize(const wxSize& sz);

        /** @brief Set the min enclosing rectangle
        *   @param nWidth
        *   @param nHeight
        **/
        void SetMinSize(int nWidth, int nHeight);


        /** @brief Set the max enclosing rectangle
        *   @param rect the enclosing rectangle
        **/
        void SetMaxSize(const wxSize& sz);

        /** @brief Set the max enclosing rectangle
        *   @param nWidth
        *   @param nHeight
        **/
        void SetMaxSize(int nWidth, int nHeight);


        /** @brief Set the index of the rectangle
        *   @param nIndex
        **/
        void SetIndex(size_t nIndex);

        /** @brief Get the index of the rectangle
        *   @return <i>size_t</i>
        **/
        size_t GetIndex() const;

        /** @brief Get the left point of the rectangle
        *   @return <i>int</i>
        **/
        int GetLeft() const;

        /** @brief Get the right point of the rectangle
        *   @return <i>int</i>
        **/
        int GetRight() const;

        /** @brief Get the height of the rectangle
        *   @return <i>int</i>
        **/
        int GetHeight() const;

        /** @brief Get the width of the rectangle
        *   @return <i>int</i>
        **/
        int GetWidth() const;

        /** @brief Get the top point of the rectangle
        *   @return <i>int</i>
        **/
        int GetTop() const;

        /** @brief Get the bottom point of the rectangle
        *   @return <i>int</i>
        **/
        int GetBottom() const;

        /** @brief Get the center wxPoint of the rectangle
        *   @return <i>wxPoint</i>
        **/
        wxPoint GetCenter() const;

        /** @brief Offset the rectangle
        *   @param x the amount to offset horizontally
        *   @param y the amount of offset vertically
        **/
        void Offset(int x, int y);

        /** @brief Set the left point of the rectangle
        *   @param n the point
        **/
        void SetLeft(int n);

        /** @brief Set the right point of the rectangle
        *   @param n the point
        **/
        void SetRight(int n);

        /** @brief Set the top point of the rectangle
        *   @param n the point
        **/
        void SetTop(int n);

        /** @brief Set the bottom point of the rectangle
        *   @param n the point
        **/
        void SetBottom(int n);

        /** @brief Set the width of the rectangle
        *   @param n the width
        **/
        void SetWidth(int n);

        /** @brief Set the height of the rectangle
        *   @param n the height
        **/
        void SetHeight(int n);

        /** @brief Get the enclosing rectangle
        *   @return <i>wxRect</i>
        **/
        const wxRect& GetRect() const
        { return m_rectEnclosing; }


        ///< @brief gets the min size of the element
        const wxSize& GetMinSize() const;

        ///< @brief gets the max size of the element
        const wxSize& GetMaxSize() const;

        wxSize GetSize() const;

        virtual void* GetClientData()
        {
            return wxEvtHandler::GetClientData();
        }
        virtual void SelectSubElement(size_t nSub);

        void EnableSubElement(size_t nSub, bool bEnable);

    protected:

        static const unsigned int ELEMENT_DISABLED = 0;
        static const unsigned int ELEMENT_ENABLED = 1;
        void RefreshMe();
        void ResizeMe();
        void GrowMe(bool bGrow, bool bAnimate=true) const;
        virtual bool SubElementDown(const wxPoint& pnt)=0;
        virtual bool SubElementUp(const wxPoint& pnt, bool bIn)=0;
        virtual bool SubElementMove(const wxPoint& pnt)=0;
        virtual int SubElementHeld()=0;
        virtual void Draw(wxDC& dc, bool bSelected);

        virtual void CreateHitRects(){};
        virtual void ElementMoved()=0;



        size_t m_nIndex;
        wxWindow* m_pParent;
        wxRect m_rectEnclosing;
        wxSize m_sizeMin;
        wxSize m_sizeMax;

        unsigned int m_nHoldCount;

        std::map<unsigned int, uiRect> m_mHitRects;
        std::map<unsigned int, uiRect>::iterator m_itDown;
        std::map<unsigned int, uiRect>::iterator m_itSelected;
};

DECLARE_EXPORTED_EVENT_TYPE(WXEXPORT, wxEVT_ADVELM_RESIZE,-1)
DECLARE_EXPORTED_EVENT_TYPE(WXEXPORT, wxEVT_ADVELM_GROW,-1)
