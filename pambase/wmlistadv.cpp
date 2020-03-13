#include "wmlistadv.h"
#include <wx/window.h>
#include <wx/dcbuffer.h>
#include <wx/log.h>
#include "wmslidewnd.h"
#include "icons/right16.xpm"
#include "icons/left16.xpm"
#include "icons/up16.xpm"
#include "icons/down16.xpm"

using namespace std;

const long wmListAdv::ID_SCROLLING     = wxNewId();
const long wmListAdv::ID_HOLDING       = wxNewId();
const long wmListAdv::ID_GROW          = wxNewId();


BEGIN_EVENT_TABLE(wmListAdv, pmControl)
    EVT_PAINT(wmListAdv::OnPaint)
    EVT_SIZE(wmListAdv::OnSize)
    EVT_LEFT_DOWN(wmListAdv::OnLeftDown)
    EVT_LEFT_UP(wmListAdv::OnLeftUp)
    EVT_MOTION(wmListAdv::OnMouseMove)
    EVT_MOUSE_CAPTURE_LOST(wmListAdv::OnCaptureLost)
    EVT_TIMER(ID_SCROLLING, wmListAdv::OnScroll)
    EVT_SET_FOCUS(wmListAdv::OnFocus)
END_EVENT_TABLE()

DEFINE_EVENT_TYPE(wxEVT_LISTADV_SELECTED)
DEFINE_EVENT_TYPE(wxEVT_LISTADV_PAGED)
DEFINE_EVENT_TYPE(wxEVT_LISTADV_MOVED)
DEFINE_EVENT_TYPE(wxEVT_LISTADV_HELD)

wmListAdv::wmListAdv() : pmControl()
{

}

wmListAdv::~wmListAdv()
{
    if(HasCapture())
    {
        ReleaseMouse();
    }
    if(m_pSlideWnd)
    {
        m_pSlideWnd->Destroy();
    }

    Clear();
}

wmListAdv::wmListAdv(wxWindow* pParent, wxWindowID id, const wxPoint& pos, const wxSize& size, unsigned int nStyle, unsigned short nScroll, const wxSize& szButtons, unsigned int nColumns, const wxSize& szGap) : pmControl()
{
    Create(pParent,id,pos,size, nStyle, nScroll, szButtons, nColumns,szGap);
}



bool wmListAdv::Create(wxWindow* pParent, wxWindowID id, const wxPoint& pos, const wxSize& size, unsigned int nStyle, unsigned short nScroll, const wxSize& szButtons, unsigned int nColumns, const wxSize& szGap)
{
    wxSize szInit(size);
    wxSize bestSize = DoGetBestSize();
    if(size.x<=0)
        szInit.SetWidth(bestSize.x);
    if(size.y <= 0)
        szInit.SetHeight(bestSize.y);

    if(!pmControl::Create(pParent,id,pos,szInit,wxWANTS_CHARS, wxEmptyString))
        return false;


    SetBackgroundStyle(wxBG_STYLE_CUSTOM);
    SetBackgroundColour(*wxWHITE);

    //m_itSelected = m_lstElements.end();
    m_itDown = m_lstElements.end();
    m_itSelected = m_lstElements.end();
    m_itTop = m_lstElements.end();
    WorkoutScrollPosition();

    m_timerScroll.SetOwner(this, ID_SCROLLING);
    m_timerGrow.SetOwner(this, ID_GROW);

    m_nStyle = nStyle;
    m_nScrollAllowed = nScroll;
    m_nIndex = 0;

    m_nScrollAllowed = SCROLL_VERTICAL;
//    m_nColumns = nColumns;
    m_szGap = szGap;

    m_timerHold.SetOwner(this, ID_HOLDING);

    m_bDownInWindow = false;

    m_pSlideWnd = 0;
    m_nSensitivity = 25;

    Connect(wxID_ANY, wxEVT_ADVELM_RESIZE, (wxObjectEventFunction)&wmListAdv::OnResizeElement);
    Connect(wxID_ANY, wxEVT_ADVELM_GROW, (wxObjectEventFunction)&wmListAdv::OnGrowElement);
    Connect(ID_HOLDING, wxEVT_TIMER, (wxObjectEventFunction)&wmListAdv::OnHolding);
    Connect(ID_GROW, wxEVT_TIMER, (wxObjectEventFunction)&wmListAdv::OnGrowTimer);


    m_pGrowElement = 0;
    m_uiScroll.SetBackgroundColour(wxColour(100,150,100));

    return true;
}

void wmListAdv::SetSwipeSensitivity(unsigned long nPixels)
{
    m_nSensitivity = nPixels;
}

wxSize wmListAdv::DoGetBestSize() const
{
    return wxSize(100,600);
}


void wmListAdv::OnPaint(wxPaintEvent& event)
{
    wxBufferedPaintDC dc(this);
    Draw(dc, m_itTop);

}

void wmListAdv::SetSize(const wxSize& size)
{
    wxWindow::SetSize(size);

}

void wmListAdv::OnResizeElement(wxCommandEvent& event)
{
    wxLogDebug(wxT("resize"));
    CreateRects();
    Refresh();
}

void wmListAdv::OnGrowElement(wxCommandEvent& event)
{

    if(m_pGrowElement != 0)
    {
        m_timerGrow.Stop();
        if(m_bGrowing)
        {
            m_pGrowElement->SetRect(m_pGrowElement->GetRect().GetLeft(), m_pGrowElement->GetRect().GetTop(), m_pGrowElement->GetMaxSize().GetWidth(), m_pGrowElement->GetMaxSize().GetHeight());

        }
        else
        {
            m_pGrowElement->SetRect(m_pGrowElement->GetRect().GetLeft(), m_pGrowElement->GetRect().GetTop(), m_pGrowElement->GetMinSize().GetWidth(), m_pGrowElement->GetMinSize().GetHeight());
        }
        GrowBelow();

        m_pGrowElement = 0;
    }


    list<advElement*>::iterator itElement = GetElement(event.GetId());
    if(itElement != m_lstElements.end())
    {
        m_pGrowElement = (*itElement);
        m_bGrowing = (event.GetInt() != 0);
        if(event.GetExtraLong() == 0)   //if set to 0 then don't animate
        {
            if(m_bGrowing)
            {
                m_pGrowElement->SetRect(m_pGrowElement->GetRect().GetLeft(), m_pGrowElement->GetRect().GetTop(), m_pGrowElement->GetMaxSize().GetWidth(), m_pGrowElement->GetMaxSize().GetHeight());

            }
            else
            {
                m_pGrowElement->SetRect(m_pGrowElement->GetRect().GetLeft(), m_pGrowElement->GetRect().GetTop(), m_pGrowElement->GetMinSize().GetWidth(), m_pGrowElement->GetMinSize().GetHeight());
            }
            GrowBelow();

            m_pGrowElement = 0;
        }
        else
        {
            m_timerGrow.Start(30,true);
        }
    }
}

void wmListAdv::OnGrowTimer(wxTimerEvent& event)
{
    if(m_pGrowElement != 0)
    {
        if(m_bGrowing)
        {
            if(m_pGrowElement->GetRect().GetHeight() < m_pGrowElement->GetMaxSize().GetHeight())
            {

                m_pGrowElement->SetHeight(m_pGrowElement->GetHeight()+max(1, (m_pGrowElement->GetMaxSize().GetHeight()-m_pGrowElement->GetRect().GetHeight())/3));

                if(m_pGrowElement->GetRect().GetBottom() > GetClientRect().GetBottom())
                {
                    ScrollVertical((GetClientRect().GetBottom()-m_pGrowElement->GetRect().GetBottom()));
                }
//                wxLogDebug(wxT("Grow %d"), (m_pGrowElement->GetHeight()));
                m_timerGrow.Start(30,true);
            }
            else
            {
                m_pGrowElement = 0;
            }
        }
        else
        {
            if(m_pGrowElement->GetRect().GetHeight() > m_pGrowElement->GetMinSize().GetHeight())
            {
                m_pGrowElement->SetHeight(m_pGrowElement->GetHeight()-max(1, (m_pGrowElement->GetRect().GetHeight()-m_pGrowElement->GetMinSize().GetHeight())/3));


                m_timerGrow.Start(30,true);
            }
            else
            {
                m_pGrowElement = 0;
            }
        }
        GrowBelow();
        if(!m_bGrowing) //shrinking
        {
            if((*m_lstElements.front()).GetRect().GetTop() < 0 && (*m_lstElements.back()).GetRect().GetBottom() < GetClientRect().GetBottom())
            {
                ScrollVertical((*m_lstElements.front()).GetRect().GetTop());
            }
        }

        Refresh();
    }
}


void wmListAdv::OnSize(wxSizeEvent& event)
{
    CreateRects();
    Refresh();
}

void wmListAdv::OnLeftDown(wxMouseEvent& event)
{
    CaptureMouse();
    m_nScrolling = SCROLL_NONE;
    m_timerScroll.Stop();
    m_bDownInWindow = true;
    m_nSwipeCount = 0;
    m_nSwipeTotal = 0;

    m_pntMouse = event.GetPosition();
    m_itDown = m_lstElements.end();

    if(m_lstElements.empty() == false)
    {
        for(list<advElement*>::iterator itButton = m_itTop; itButton != m_lstElements.end(); ++itButton)
        {
            if((*itButton)->LeftDown(event) >=0)
            {
                m_itDown = itButton;
                m_timerHold.Start(50);
                Refresh();
                Update();
            }
        }
    }
    event.Skip();
}

void wmListAdv::SelectSubElement(size_t nIndex, size_t nSub)
{
    list<advElement*>::iterator itElm = GetElement(nIndex);
    if(itElm != m_lstElements.end())
    {
        m_itSelected = itElm;
        (*itElm)->SelectSubElement(nSub);
    }
}

wxRect wmListAdv::GetSubElementRect(size_t nIndex, size_t nSub)
{
    list<advElement*>::const_iterator itElm = GetElement(nIndex);
    if(itElm != m_lstElements.end())
    {
        return (*itElm)->GetSubRect(nSub);
    }
    return wxRect(0,0,0,0);
}

void wmListAdv::OnLeftUp(wxMouseEvent& event)
{
    m_bDownInWindow = false;
    m_timerHold.Stop();

    bool bPressedElement(false);
    if(m_itDown != m_lstElements.end())
    {
        int nSubElement = (*m_itDown)->LeftUp(event,(m_nScrolling!=SCROLL_NONE));
        if(nSubElement >=0)
        {
            wxCommandEvent event(wxEVT_LISTADV_SELECTED, GetId());
            event.SetInt((*m_itDown)->GetIndex());
            event.SetClientData(reinterpret_cast<void*>((*m_itDown)->GetClientData()));
            event.SetExtraLong(nSubElement);
            wxPostEvent(GetEventHandler(), event);
            bPressedElement = true;
            m_itSelected = m_itDown;
        }
        m_itDown = m_lstElements.end();
        Refresh();
    }

    if(!bPressedElement)
    {
        wxCommandEvent event(wxEVT_LISTADV_SELECTED, GetId());
        event.SetInt(-1);
        event.SetClientData(0);
        event.SetExtraLong(-1);
        wxPostEvent(GetEventHandler(), event);
    }

    if(m_nScrolling == SCROLL_VERTICAL)
    {
        if(m_nScrollOffset != 0)
        {
            m_timerScroll.Start(50);
        }
        Refresh();
    }
    else if(m_nScrolling == SCROLL_HORIZONTAL_RIGHT)
    {

        if(-(m_nSwipeTotal/max(1, m_nSwipeCount)) > 60 || (-m_nSwipeLeft > GetClientRect().GetWidth()/3))
        {
            m_nSwipeOffset = min(-1, -(GetClientRect().GetWidth()+m_nSwipeLeft)/10);
        }
        else if(m_nSwipeLeft < 0)
        {
            m_nSwipeOffset = max(2, -(m_nSwipeLeft/10));
            m_nScrolling = SCROLL_RETURN;
        }
        else
        {
            m_nSwipeOffset = -m_nSwipeLeft;
            m_nScrolling = SCROLL_RETURN;
        }
        m_timerScroll.Start(10);
    }
    else if(m_nScrolling == SCROLL_HORIZONTAL)
    {
        if((m_nSwipeTotal/max(1, m_nSwipeCount)) > 60 || m_nSwipeLeft > GetClientRect().GetWidth()/3)
        {
            m_nSwipeOffset = max(1, (GetClientRect().GetWidth()-m_nSwipeLeft)/10);
        }
        else if(m_nSwipeLeft > 0)
        {
            m_nSwipeOffset = min(-2, -(m_nSwipeLeft/10));
            m_nScrolling = SCROLL_RETURN;
        }
        else
        {
            m_nSwipeOffset = -m_nSwipeLeft;
            m_nScrolling = SCROLL_RETURN;
        }
        m_timerScroll.Start(10);
    }
    if(m_pSlideWnd)
    {
        m_pSlideWnd->Hide();
    }
    if(HasCapture())
    {
        ReleaseMouse();
    }
    event.Skip();
}


void wmListAdv::OnMouseMove(wxMouseEvent& event)
{
    if(event.LeftIsDown() && m_bDownInWindow && m_lstElements.empty() == false)
    {
        if(m_nScrollAllowed != SCROLL_NONE)
        {
            m_nSwipeOffset =  event.GetPosition().x-m_pntMouse.x;
            m_nScrollOffset =  event.GetPosition().y-m_pntMouse.y;
            m_nSwipeTotal += m_nSwipeOffset;
            ++m_nSwipeCount;

            if(m_nScrolling == SCROLL_NONE)
            {
                if(m_nScrollAllowed == SCROLL_VERTICAL)
                {
                    if(m_lstElements.back()->GetBottom() > GetClientSize().GetHeight() || m_lstElements.front()->GetTop() < 0)    //only scroll if more that on screen
                    {
                        if(m_nScrollOffset < -5 || m_nScrollOffset > 5)
                        {
                            m_nScrolling = SCROLL_VERTICAL;
                            m_timerHold.Stop();
                        }
                    }
                }
                else if(m_nScrollAllowed == SCROLL_HORIZONTAL)
                {
                    if(m_nSwipeOffset > m_nSensitivity)
                    {
                        set<std::list<advElement*>::iterator>::iterator itPage = m_setPages.find(m_itTop);
                        if(itPage != m_setPages.end())
                        {
                            if(itPage != m_setPages.begin())
                            {
                                --itPage;
                                m_itSwipe = itPage;
                                m_nScrolling = SCROLL_HORIZONTAL;
                            }
                            else
                            {
                                m_itSwipe = m_setPages.end();
                                --m_itSwipe;
                                m_nScrolling = SCROLL_HORIZONTAL;
                            }
                        }
                        m_timerHold.Stop();
                    }
                    else if(m_nSwipeOffset < -m_nSensitivity)
                    {
                        set<std::list<advElement*>::iterator>::iterator itPage = m_setPages.find(m_itTop);
                        if(itPage != m_setPages.end())
                        {
                            ++itPage;
                            if(itPage != m_setPages.end())
                            {
                                m_itSwipe = itPage;
                                m_nScrolling = SCROLL_HORIZONTAL_RIGHT;
                            }
                            else
                            {
                                m_itSwipe = m_setPages.begin();
                                m_nScrolling = SCROLL_HORIZONTAL_RIGHT;
                            }
                            m_timerHold.Stop();
                        }
                    }
                    if(m_nScrolling == SCROLL_HORIZONTAL || m_nScrolling == SCROLL_HORIZONTAL_RIGHT)
                    {

                        CreateSwipeBitmaps();

                        m_nSwipeLeft = m_nSwipeOffset;
                        DrawHorizontalScroll();
                    }
                }

            }
            if(m_nScrolling == SCROLL_VERTICAL)
            {
                ScrollVertical(m_nScrollOffset);
            }
            else if(m_nScrolling == SCROLL_HORIZONTAL || m_nScrolling == SCROLL_HORIZONTAL_RIGHT)
            {
                ScrollHorizontal(m_nSwipeOffset);
            }
        }
    }

    m_pntMouse = event.GetPosition();

    event.Skip();
}

void wmListAdv::CreateSwipeBitmaps()
{
    m_bmpSwipe[0].SetWidth(GetClientSize().x);
    m_bmpSwipe[0].SetHeight(GetClientSize().y);
    m_bmpSwipe[1].SetWidth(GetClientSize().x);
    m_bmpSwipe[1].SetHeight(GetClientSize().y);

    //copy the current screen view in to the current bitmap
    wxMemoryDC memDC;
    wxBitmap bmpTemp(GetClientRect().GetWidth(), GetClientRect().GetHeight());
    memDC.SelectObject(bmpTemp);
    Draw(memDC, m_itTop);
    memDC.SelectObject(wxNullBitmap);
    m_bmpSwipe[0] = bmpTemp;

    memDC.SelectObject(bmpTemp);
    Draw(memDC, (*m_itSwipe));
    memDC.SelectObject(wxNullBitmap);
    m_bmpSwipe[1] = bmpTemp;
}

bool wmListAdv::ScrollVertical(int nYDiff)
{
    int nMove(0);
    if(nYDiff != 0)
    {
        if(nYDiff > 0)
        {
            nMove = min(nYDiff, 2-m_lstElements.front()->GetTop());
        }
        else
        {
            nMove = -min(-nYDiff, m_lstElements.back()->GetBottom()-(GetClientRect().GetBottom()-2));
        }

        if(nMove != 0)
        {
            m_itTop = m_lstElements.end();
            WorkoutScrollPosition();
            for(list<advElement*>::iterator itButton = m_lstElements.begin(); itButton != m_lstElements.end(); ++itButton)
            {
                (*itButton)->Offset(0, nMove);
                if(m_itTop == m_lstElements.end() && (*itButton)->GetBottom() >= 0)
                {
                    m_itTop = itButton;
                    WorkoutScrollPosition();
                }
            }
            Refresh();
            Update();

            wxCommandEvent event(wxEVT_LISTADV_MOVED, GetId());
            wxPostEvent(GetEventHandler(), event);
        }
        else
        {
            wxClientDC dc(this);
            dc.SetPen(wxPen(wxColour(255,30,20),10, wxSOLID));
            if(nYDiff > 0)
            {
                dc.DrawLine(GetClientRect().GetTopLeft(), GetClientRect().GetTopRight());
            }
            else
            {
                dc.DrawLine(GetClientRect().GetBottomLeft(), GetClientRect().GetBottomRight());
            }
        }
    }

    return (nMove != 0);
}


bool wmListAdv::ScrollHorizontal(int nXDiff)
{
    if(nXDiff != 0)
    {
        int nCurrent = m_nSwipeLeft;
        m_nSwipeLeft += nXDiff;

        if(m_nScrolling == SCROLL_HORIZONTAL || m_nScrolling == SCROLL_HORIZONTAL_RIGHT)
        {
            if(m_nSwipeLeft < -GetClientSize().GetWidth() || m_nSwipeLeft > GetClientSize().GetWidth())
            {
                m_itTop = (*m_itSwipe);
                WorkoutScrollPosition();
                m_nScrolling = SCROLL_NONE;
                Refresh();
                Update();

                wxCommandEvent event(wxEVT_LISTADV_PAGED, GetId());
                event.SetInt(GetCurrentPageNumber());
                wxPostEvent(GetEventHandler(), event);

                return false;
            }
        }
        else if(m_nScrolling == SCROLL_RETURN)
        {
            if((nCurrent <= 0 && m_nSwipeLeft >=0) || (nCurrent >= 0 && m_nSwipeLeft <=0))
            {
                m_nScrolling = SCROLL_NONE;
                Refresh();
                Update();
                return false;
            }
        }

        DrawHorizontalScroll();
        return true;
    }
    return false;


}

void wmListAdv::DrawHorizontalScroll()
{
    wxClientDC dc(this);
    if(m_nSwipeLeft > 0)
    {
        dc.DrawBitmap(m_bmpSwipe[1],m_nSwipeLeft-m_bmpSwipe[1].GetWidth(), 0);
    }
    else
    {
        dc.DrawBitmap(m_bmpSwipe[1],m_bmpSwipe[1].GetWidth()+m_nSwipeLeft, 0);
    }
    dc.DrawBitmap(m_bmpSwipe[0],m_nSwipeLeft, 0);

    //dc.DestroyClippingRegion();
}


void wmListAdv::OnScroll(wxTimerEvent& event)
{
    if(m_nScrolling == SCROLL_VERTICAL)
    {
        if(ScrollVertical(m_nScrollOffset))
        {
            if(m_nScrollOffset < 0)
            {
                m_nScrollOffset++;
            }
            else if(m_nScrollOffset > 0)
            {
                --m_nScrollOffset;
            }
            else
            {
                m_timerScroll.Stop();
            }
        }
        else
        {
            m_timerScroll.Stop();
        }
    }
    else if(m_nScrolling == SCROLL_HORIZONTAL || m_nScrolling == SCROLL_RETURN || m_nScrolling == SCROLL_HORIZONTAL_RIGHT)
    {
        if(ScrollHorizontal(m_nSwipeOffset) == false)
        {
            m_timerScroll.Stop();
        }
    }
}

void wmListAdv::OnCaptureLost(wxMouseCaptureLostEvent& event)
{
    m_itDown = m_lstElements.end();
    m_timerHold.Stop();
    Refresh();
}

void wmListAdv::OnHolding(wxTimerEvent& event)
{
    if(m_itDown != m_lstElements.end())
    {
        int nSubElement = (*m_itDown)->MouseHeld();
        if(nSubElement != 0)
        {
            wxLogDebug(wxT("Held"));
            wxCommandEvent event(wxEVT_LISTADV_HELD, GetId());
            event.SetInt((*m_itDown)->GetIndex());
            event.SetClientData(reinterpret_cast<void*>((*m_itDown)->GetClientData()));
            event.SetExtraLong(nSubElement);
            wxPostEvent(GetEventHandler(), event);

            m_itSelected = m_itDown;
            m_itDown = m_lstElements.end();
            Refresh();
        }
    }

}


void wmListAdv::Draw(wxDC& dc, const std::list<advElement*>::iterator& itTop)
{
    dc.SetFont(GetFont());


    dc.SetBackgroundMode(wxTRANSPARENT);
    dc.SetPen(*wxTRANSPARENT_PEN);
    dc.SetBrush(wxBrush(GetBackgroundColour()));
    dc.DrawRectangle(GetClientRect());

    dc.SetPen(*wxBLACK_PEN);
    dc.SetTextForeground(GetForegroundColour());


    if(m_lstElements.empty() == false)
    {
        unsigned int nColumn(0);
        unsigned int nElements(0);
        for(list<advElement*>::iterator itButton = itTop; itButton != m_lstElements.end(); ++itButton)
        {
            ++nElements;
            DrawElement(dc, itButton);

            //only draw to the end of the screen...
            if((*itButton)->GetBottom() >= GetClientRect().GetBottom())
            {
                m_itLast = itButton;
                break;
            }
        }

        double dPosition = static_cast<double>(nElements)/static_cast<double>(m_lstElements.size());
        dPosition *= static_cast<double>(GetClientSize().y);
        m_uiScroll.SetHeight(dPosition);
        m_uiScroll.Draw(dc, uiRect::BORDER_UP);
    }
}

void wmListAdv::DrawElement(wxDC& dc, const std::list<advElement*>::iterator& itButton)
{

    (*itButton)->DrawMe(dc, (itButton==m_itSelected));
}

bool wmListAdv::RemoveElement(size_t nIndex)
{
    Freeze();


    for(list<advElement*>::iterator itButton = m_lstElements.begin(); itButton != m_lstElements.end(); ++itButton)
    {
        if((*itButton)->GetIndex() == nIndex)
        {
            bool bTop(false);
            if(m_itTop == itButton)
            {
                if(m_itTop != m_lstElements.begin())
                {
                    --m_itTop;
                }
                else
                {
                    bTop = true;
                }
            }
            if(m_itDown == itButton)
            {
                m_itDown = m_lstElements.end();
            }
            if(m_itSelected == itButton)
            {
                m_itSelected = m_lstElements.end();
            }
            (*itButton)->Destroy();
            delete (*itButton);
            m_lstElements.erase(itButton);

            if(m_lstElements.empty() == false)
            {
                if(bTop)
                {
                    m_itTop = m_lstElements.begin();
                    WorkoutScrollPosition();
                }

                CreateRects();
                while(m_itTop != m_lstElements.begin() && (*m_itTop)->GetTop() > 2)
                {
                    --m_itTop;
                }

                int nMove = 2-(*m_itTop)->GetTop();
                for(list<advElement*>::iterator itButton = m_lstElements.begin(); itButton != m_lstElements.end(); ++itButton)
                {
                    (*itButton)->Offset(0, nMove);
                }
            }
            else
            {
                m_itTop = m_lstElements.end();
                WorkoutScrollPosition();
            }
            Thaw();
            Refresh();
            return true;
        }
    }
    return false;
}

size_t wmListAdv::AddElement(advElement* pElement, bool bEnd)
{
    if(bEnd)
    {
        m_lstElements.push_back(pElement);
    }
    else
    {
        m_lstElements.push_front(pElement);
    }

    pElement->SetParent(this);
    pElement->SetIndex(m_nIndex);

    m_nIndex++;

    if(m_itTop == m_lstElements.end())
    {
        m_itTop = m_lstElements.begin();
        WorkoutScrollPosition();
    }

    if(IsFrozen() == false)
    {
        CreateRects();
        Refresh();
    }

    return m_nIndex;
}

void wmListAdv::Thaw()
{
    wxWindow::Thaw();
}

void wmListAdv::Clear()
{
    for(list<advElement*>::iterator it = m_lstElements.begin(); it != m_lstElements.end(); ++it)
    {
        (*it)->Destroy();
        delete (*it);
    }
    m_nIndex = 0;
    m_lstElements.clear();
    m_itDown = m_lstElements.end();
    m_itSelected = m_lstElements.end();
    m_itTop = m_lstElements.end();
    WorkoutScrollPosition();
    Refresh();
}

advElement* wmListAdv::GetAdvElement(size_t nIndex)
{
    list<advElement*>::iterator itButton = m_lstElements.begin();
    for(; itButton != m_lstElements.end(); ++itButton)
    {
        if((*itButton)->GetIndex() == nIndex)
        {
            return (*itButton);
        }
    }
    return NULL;
}

advElement* wmListAdv::GetFirstAdvElement(void* pData)
{
    list<advElement*>::iterator itButton = m_lstElements.begin();
    for(; itButton != m_lstElements.end(); ++itButton)
    {
        if((*itButton)->GetClientData() == pData)
        {
            return (*itButton);
        }
    }
    return NULL;
}

size_t wmListAdv::FindFirstElement(void* pData)
{
    list<advElement*>::iterator itButton = m_lstElements.begin();
    for(; itButton != m_lstElements.end(); ++itButton)
    {
        if((*itButton)->GetClientData() == pData)
        {
            return (*itButton)->GetIndex();
        }
    }
    return 0xFFFFFFFF;
}

list<advElement*>::iterator wmListAdv::GetElement(size_t nIndex)
{
    list<advElement*>::iterator itButton = m_lstElements.begin();
    for(; itButton != m_lstElements.end(); ++itButton)
    {
        if((*itButton)->GetIndex() == nIndex)
            break;
    }
    return itButton;
}


void wmListAdv::ShowElement(size_t nIndex, Position pos)
{
    list<advElement*>::iterator itButton = GetElement(nIndex);
    if(itButton != m_lstElements.end())
    {
        ShowElement(itButton, pos);
    }
}


void wmListAdv::ShowElement(list<advElement*>::iterator itButton, Position pos)
{
    ///< @todo Do ShowButton for horizontal scroll
    int nDiff(0);
    switch(pos)
    {
        case TOP:
            nDiff = -(*itButton)->GetTop();
            if(m_lstElements.front()->GetTop() + nDiff > 0)
            {
                nDiff = -m_lstElements.front()->GetTop();
            }
            break;
        case BOTTOM:
            nDiff = GetClientRect().GetBottom()-(*itButton)->GetBottom();
            break;
        case MIDDLE:
            nDiff = (GetClientRect().GetBottom()-(*itButton)->GetBottom())-(GetClientRect().GetHeight()/2);
            if(m_lstElements.front()->GetTop() + nDiff > 0)
            {
                nDiff = -m_lstElements.front()->GetTop();
            }
            break;
        case ONSCREEN:
            if(GetClientRect().Contains((*itButton)->GetCenter()))
            {
                nDiff = 0;
            }
            else if((*itButton)->GetTop() < 0)
            {
                nDiff = -(*itButton)->GetTop();
            }
            else
            {
                nDiff = GetClientRect().GetBottom()-(*itButton)->GetBottom();
            }
    }

    if(GetScreenPosition().y < 0)
    {
        nDiff -= GetScreenPosition().y;
    }


    if(nDiff != 0)
    {
        ScrollVertical(nDiff);

    }
    Refresh();
}

size_t wmListAdv::GetElementCount() const
{
    return m_lstElements.size();
}

void wmListAdv::OnFocus(wxFocusEvent& event)
{
    if(event.GetWindow() && event.GetWindow() != this)
    {
        event.GetWindow()->SetFocus();
    }
}


void wmListAdv::ShowPreviousPage()
{
    if(m_nScrollAllowed == SCROLL_HORIZONTAL)
    {
        set<std::list<advElement*>::iterator>::iterator itPage = m_setPages.find(m_itTop);
        if(itPage != m_setPages.end())
        {
            if(itPage != m_setPages.begin())
            {
                --itPage;
                m_itSwipe = itPage;
                m_nScrolling = SCROLL_HORIZONTAL;
            }
            else
            {
                m_itSwipe = m_setPages.end();
                --m_itSwipe;
                m_nScrolling = SCROLL_HORIZONTAL;
            }
        }
        m_nSwipeLeft = 0;//GetClientRect().GetWidth();
        m_nSwipeOffset = 50;// max(1, (GetClientRect().GetWidth()-m_nSwipeLeft)/10);
        CreateSwipeBitmaps();
        m_timerScroll.Start(10);
    }
    else
    {
        if(m_lstElements.empty() == false)
        {
            list<advElement*>::iterator itButton = m_lstElements.begin();
            int nButton = (*itButton)->GetTop();
            int nDiff = min(GetClientSize().GetHeight(), -nButton);
            ScrollVertical(nDiff);
        }
    }

}


void wmListAdv::ShowNextPage()
{
    if(m_nScrollAllowed == SCROLL_HORIZONTAL)
    {
        set<std::list<advElement*>::iterator>::iterator itPage = m_setPages.find(m_itTop);
        if(itPage != m_setPages.end())
        {
            ++itPage;
            if(itPage != m_setPages.end())
            {
                m_itSwipe = itPage;
                m_nScrolling = SCROLL_HORIZONTAL_RIGHT;
            }
            else
            {
                m_itSwipe = m_setPages.begin();
                m_nScrolling = SCROLL_HORIZONTAL_RIGHT;
            }
        }

        CreateSwipeBitmaps();
        m_nSwipeOffset = -50;
        m_nSwipeLeft = 0;
        m_timerScroll.Start(10);
    }
    else
    {
        if(m_lstElements.empty() == false)
        {
            list<advElement*>::iterator itButton = m_lstElements.end();
            --itButton;
            int nButton = (*itButton)->GetBottom();
            int nDiff = min(GetClientSize().GetHeight(), nButton - GetClientRect().GetHeight());
            ScrollVertical(-nDiff);
        }
    }

}

unsigned long wmListAdv::GetCurrentPageNumber() const
{
    unsigned long nCount = 1;
    for(set<list<advElement*>::iterator>::const_iterator itPage = m_setPages.begin(); itPage != m_setPages.end(); ++itPage)
    {
        if((*itPage) == m_itTop)
            return nCount;

        ++nCount;
    }
    return 0;

}

unsigned long wmListAdv::GetPageCount() const
{
    return m_setPages.size();
}

//void wmListAdv::DeleteButton(size_t nIndex)
//{
//    list<advElement*>::iterator itButton = GetButton(nIndex);
//    if(itButton != m_lstElements.end())
//    {
//        if(m_itDown == itButton)
//        {
//            m_itDown = m_lstElements.end();
//        }
//        if(m_itTop == itButton)
//        {
//            ++m_itTop;
//        }
//        if(m_itLast == itButton)
//        {
//            m_itLast = m_lstElements.end();
//        }
//        m_setitSelected.erase(itButton);
//        m_setitFlash.erase(itButton);
//        m_lstElements.erase(itButton);
//        CreateRects();
//        Refresh();
//    }
//}


void wmListAdv::CreateRects()
{
    m_setPages.clear();
    if(m_lstElements.empty())
    {
        return;
    }

    int nTop = m_lstElements.front()->GetTop();
    for(list<advElement*>::iterator itButton = m_lstElements.begin(); itButton != m_lstElements.end(); ++itButton)
    {

        if(nTop == 2 && m_nScrollAllowed == SCROLL_HORIZONTAL)   //first button on page
        {
            m_setPages.insert(itButton);
        }

        (*itButton)->SetTop(nTop);
        nTop += (*itButton)->GetHeight();
        nTop += m_szGap.GetHeight();

        //if we swipe then once we get to the bottom we create a new page
        if(nTop >= GetClientRect().GetBottom())
        {
            if(m_nScrollAllowed == SCROLL_HORIZONTAL)
            {
                nTop = 2;
            }
        }
    }
}

void wmListAdv::GrowBelow()
{
    //CreateRects();
    //return;

    bool bBelow(false);
    unsigned int nTop;
    for(list<advElement*>::iterator itButton = m_lstElements.begin(); itButton != m_lstElements.end(); ++itButton)
    {
        if(bBelow)
        {
            (*itButton)->SetTop(nTop);
            nTop = (*itButton)->GetBottom()+m_szGap.GetHeight();
        }
        else if((*itButton) == m_pGrowElement)
        {
            nTop = m_pGrowElement->GetBottom()+m_szGap.GetHeight();
            bBelow = true;
        }
    }
    Refresh();
}

void wmListAdv::Sort()
{
    if(m_lstElements.empty() == false)
    {

        Freeze();

        m_lstElements.sort(compare);
        CreateRects();

        while(m_itTop != m_lstElements.begin() && (*m_itTop)->GetTop() > 2)
        {
            --m_itTop;
        }

        int nMove = 2-(*m_itTop)->GetTop();
        for(list<advElement*>::iterator itButton = m_lstElements.begin(); itButton != m_lstElements.end(); ++itButton)
        {
            (*itButton)->Offset(0, nMove);
        }
        Thaw();
        Refresh();

        wxCommandEvent event(wxEVT_LISTADV_MOVED, GetId());
        wxPostEvent(GetEventHandler(), event);
    }
}


list<advElement*>::const_iterator wmListAdv::GetElementBegin() const
{
    return m_lstElements.begin();
}

list<advElement*>::const_iterator wmListAdv::GetElementEnd() const
{
    return m_lstElements.end();
}


void wmListAdv::WorkoutScrollPosition()
{
    size_t nTopElement = 0;
    for(list<advElement*>::iterator itButton = m_lstElements.begin(); itButton != m_lstElements.end(); ++itButton)
    {
        if(itButton == m_itTop)
        {
            break;
        }
        ++nTopElement;
    }
    double dPosition = static_cast<double>(nTopElement)/static_cast<double>(m_lstElements.size());
    dPosition *= static_cast<double>(GetClientSize().y);
    m_uiScroll.SetRect(GetClientRect().GetRight()-5, dPosition,5,30);



}
