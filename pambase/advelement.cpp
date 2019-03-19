#include "advelement.h"
#include <wx/log.h>
#include <wx/window.h>


using namespace std;

DEFINE_EVENT_TYPE(wxEVT_ADVELM_RESIZE)
DEFINE_EVENT_TYPE(wxEVT_ADVELM_GROW)

wxSize advElement::GetSize() const
{
    return m_rectEnclosing.GetSize();
}


void advElement::SetRect(const wxRect& rect)
{
    m_rectEnclosing = rect;

    if(m_sizeMin.GetWidth() == 0 && m_sizeMin.GetHeight() == 0) //not set yet
    {
        m_sizeMin = m_rectEnclosing.GetSize();
    }

    if(m_sizeMax.GetWidth() == 0 && m_sizeMax.GetHeight() == 0) //not set yet
    {
        m_sizeMax = m_rectEnclosing.GetSize();
    }

    ElementMoved();
}

void advElement::SetRect(int nLeft, int nTop, int nWidth, int nHeight)
{
    m_rectEnclosing = wxRect(nLeft, nTop, nWidth, nHeight);

    if(m_sizeMin.GetWidth() == 0 && m_sizeMin.GetHeight() == 0) //not set yet
    {
        m_sizeMin = m_rectEnclosing.GetSize();
    }

    if(m_sizeMax.GetWidth() == 0 && m_sizeMax.GetHeight() == 0) //not set yet
    {
        m_sizeMax = m_rectEnclosing.GetSize();
    }

    ElementMoved();
}

void advElement::DrawMe(wxDC& dc, bool bSelected)
{
    wxDCClipper clip(dc, m_rectEnclosing);
    Draw(dc,bSelected);
}
void advElement::Draw(wxDC& dc, bool bSelected)
{
    dc.SetPen(*wxBLACK_PEN);
    dc.SetBrush(*wxTRANSPARENT_BRUSH);
    dc.GradientFillLinear(m_rectEnclosing, wxColour(220,220,220), wxColour(180,180,180), wxEAST);
    dc.DrawRectangle(m_rectEnclosing);
    dc.SetPen(wxNullPen);
    dc.SetBrush(wxNullBrush);
}


int advElement::GetLeft() const
{
    return m_rectEnclosing.GetLeft();
}

int advElement::GetRight() const
{
    return m_rectEnclosing.GetRight();
}

int advElement::GetHeight() const
{
    return m_rectEnclosing.GetHeight();
}

int advElement::GetWidth() const
{
    return m_rectEnclosing.GetWidth();
}

int advElement::GetTop() const
{
    return m_rectEnclosing.GetTop();
}

int advElement::GetBottom() const
{
    return m_rectEnclosing.GetBottom();
}

wxPoint advElement::GetCenter() const
{
    return wxPoint(m_rectEnclosing.GetLeft()+ m_rectEnclosing.GetWidth()/2,
                   m_rectEnclosing.GetTop()+ m_rectEnclosing.GetHeight()/2);

}

void advElement::Offset(int x, int y)
{
    m_rectEnclosing.Offset(x,y);

    ElementMoved();

//    for(map<unsigned int, uiRect>::iterator itHit = m_mHitRects.begin(); itHit != m_mHitRects.end(); ++itHit)
//    {
//        itHit->second.Offset(x,y);
//    }
    //ElementMoved();
}

void advElement::SetLeft(int n)
{
    m_rectEnclosing.SetLeft(n);

    ElementMoved();

}

void advElement::SetRight(int n)
{
    m_rectEnclosing.SetRight(n);

    ElementMoved();

}

void advElement::SetTop(int n)
{
    m_rectEnclosing.SetTop(n);

    ElementMoved();

}

void advElement::SetBottom(int n)
{
    m_rectEnclosing.SetBottom(n);

    ElementMoved();
}

void advElement::SetWidth(int n)
{
    m_rectEnclosing.SetWidth(n);

    ElementMoved();
}

void advElement::SetHeight(int n)
{
    m_rectEnclosing.SetHeight(n);

    ElementMoved();

}

void advElement::SetIndex(size_t nIndex)
{
    m_nIndex = nIndex;
}

size_t advElement::GetIndex() const
{
    return m_nIndex;
}


void advElement::SelectSubElement(size_t nSub)
{
    map<unsigned int, uiRect>::iterator itRect = m_mHitRects.find(nSub);
    if(itRect != m_mHitRects.end())
    {
        m_itSelected = itRect;
        RefreshMe();
    }
}

int advElement::LeftDown(wxMouseEvent& event)
{
    if(m_rectEnclosing.Contains(event.GetPosition()) == false)
        return -1;

    m_nHoldCount = 0;
    for(map<unsigned int, uiRect>::iterator itRect = m_mHitRects.begin(); itRect != m_mHitRects.end(); ++itRect)
    {
        if(itRect->second.Contains(event.GetPosition()) && itRect->second.GetIntData() != ELEMENT_DISABLED)
        {
            m_itDown = itRect;
            if(SubElementDown(event.GetPosition()))
            {
                return itRect->first;
            }
            else
            {
                return -1;
            }
        }
    }
    m_itDown = m_mHitRects.end();
    m_itSelected = m_mHitRects.end();
    return -1;


}

int advElement::LeftUp(wxMouseEvent& event, bool bScrolling)
{
    m_nHoldCount = 0;
    if(m_itDown != m_mHitRects.end())
    {
        bool bUp = SubElementUp(event.GetPosition(), m_itDown->second.Contains(event.GetPosition()));
        if(m_itDown->second.Contains(event.GetPosition()) && m_itDown->second.GetIntData() != ELEMENT_DISABLED)
        {
            int nDown = m_itDown->first;
            m_itSelected = m_itDown;
            m_itDown = m_mHitRects.end();
            if(bUp)
            {
                return nDown;
            }
            return -1;
        }
    }
    m_itDown = m_mHitRects.end();
    return -1;
}

int advElement::MouseMove(wxMouseEvent& event)
{
    return m_rectEnclosing.Contains(event.GetPosition());
}

int advElement::MouseHeld()
{
    m_nHoldCount++;
    return SubElementHeld();
}

void advElement::RefreshMe()
{
    if(m_pParent)
    {
        m_pParent->RefreshRect(m_rectEnclosing);
    }
}

void advElement::GrowMe(bool bGrow, bool bAnimate)
{
    if(m_pParent)
    {
        wxCommandEvent event(wxEVT_ADVELM_GROW);
        event.SetId(m_nIndex);
        event.SetInt(bGrow);
        event.SetExtraLong(bAnimate);
        wxPostEvent(m_pParent, event);
    }
}

void advElement::ResizeMe()
{
    if(m_pParent)
    {
        wxCommandEvent event(wxEVT_ADVELM_RESIZE);
        event.SetId(m_nIndex);
        wxPostEvent(m_pParent, event);
    }
}

wxRect advElement::GetSubRect(unsigned long nSub) const
{
    map<unsigned int, uiRect>::const_iterator itHit = m_mHitRects.find(nSub);
    if(itHit != m_mHitRects.end())
    {
        return itHit->second.GetRect();
    }
    return wxRect(0,0,0,0);
}


void advElement::EnableSubElement(size_t nSub, bool bEnable)
{
    map<unsigned int, uiRect>::iterator itHit = m_mHitRects.find(nSub);
    if(itHit != m_mHitRects.end())
    {
        itHit->second.SetIntData(bEnable);
    }
}


void advElement::SetMinSize(const wxSize& sz)
{
    m_sizeMin = sz;
}
void advElement::SetMinSize(int nWidth, int nHeight)
{
    m_sizeMin = wxSize(nWidth, nHeight);
}

void advElement::SetMaxSize(const wxSize& sz)
{
    m_sizeMax = sz;
}

void advElement::SetMaxSize(int nWidth, int nHeight)
{
    m_sizeMax = wxSize(nWidth, nHeight);
}


const wxSize& advElement::GetMinSize() const
{
    return m_sizeMin;
}

const wxSize& advElement::GetMaxSize() const
{
    return m_sizeMax;
}
