#include "wmlist.h"
#include <wx/window.h>
#include <wx/dcbuffer.h>
#include <wx/log.h>
#include "wmslidewnd.h"
#include "icons/right16.xpm"
#include "icons/left16.xpm"
#include "icons/up16.xpm"
#include "icons/down16.xpm"

using namespace std;

const long wmList::ID_HOLDING       = wxNewId();
const long wmList::ID_SCROLLING     = wxNewId();
const long wmList::ID_FLASHING     = wxNewId();

BEGIN_EVENT_TABLE(wmList, pmControl)
    EVT_PAINT(wmList::OnPaint)
    EVT_SIZE(wmList::OnSize)
    EVT_LEFT_DOWN(wmList::OnLeftDown)
    EVT_LEFT_UP(wmList::OnLeftUp)
    EVT_MOTION(wmList::OnMouseMove)
    EVT_MOUSE_CAPTURE_LOST(wmList::OnCaptureLost)
    EVT_TIMER(ID_HOLDING, wmList::OnHolding)
    EVT_TIMER(ID_SCROLLING, wmList::OnScroll)
    EVT_TIMER(ID_FLASHING, wmList::OnFlash)
    EVT_SET_FOCUS(wmList::OnFocus)
END_EVENT_TABLE()

#ifdef WXSPAM
IMPLEMENT_DYNAMIC_CLASS(wmList, pmControl)
DEFINE_EVENT_TYPE(wxEVT_LIST_SELECTED)
DEFINE_EVENT_TYPE(wxEVT_LIST_HELD)
DEFINE_EVENT_TYPE(wxEVT_LIST_PAGED)
DEFINE_EVENT_TYPE(wxEVT_LIST_SLID)
#else
wxIMPLEMENT_DYNAMIC_CLASS(wmList, pmControl);
wxDEFINE_EVENT(wxEVT_LIST_SELECTED, wxCommandEvent);
wxDEFINE_EVENT(wxEVT_LIST_HELD, wxCommandEvent);
wxDEFINE_EVENT(wxEVT_LIST_PAGED, wxCommandEvent);
wxDEFINE_EVENT(wxEVT_LIST_SLID, wxCommandEvent);
#endif // WXSPAM

wmList::wmList() : pmControl()
{

}

wmList::~wmList()
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
    delete m_pFactory;
}

wmList::wmList(wxWindow* pParent, wxWindowID id, const wxPoint& pos, const wxSize& size, unsigned int nStyle, unsigned short nScroll, const wxSize& szButtons, unsigned int nColumns, const wxSize& szGap) : pmControl()
{
    Create(pParent,id,pos,size, nStyle, nScroll, szButtons, nColumns,szGap);
}



bool wmList::Create(wxWindow* pParent, wxWindowID id, const wxPoint& pos, const wxSize& size, unsigned int nStyle, unsigned short nScroll, const wxSize& szButtons, unsigned int nColumns, const wxSize& szGap)
{
    wxSize szInit(size);
    wxSize bestSize = DoGetBestSize();
    if(size.x<=0)
        szInit.SetWidth(bestSize.x);
    if(size.y <= 0)
        szInit.SetHeight(bestSize.y);

    if(!wxWindow::Create(pParent,id,pos,szInit,wxWANTS_CHARS, wxEmptyString))
        return false;



    m_nGradient = wxEAST;

    SetBackgroundStyle(wxBG_STYLE_CUSTOM);
    SetBackgroundColour(*wxWHITE);

    //m_itSelected = m_lstButtons.end();
    m_itDown = m_lstButtons.end();
    m_itTop = m_lstButtons.end();

    m_timerHold.SetOwner(this, ID_HOLDING);
    m_timerScroll.SetOwner(this, ID_SCROLLING);
    m_timerFlash.SetOwner(this, ID_FLASHING);
    m_nFlashTime = 500;

    m_nStyle = nStyle;
    m_nScrollAllowed = nScroll;
    m_nIndex = 0;
    m_clrButton = wxColour(150,150,150);
    m_clrSelectedButton = wxColour(168,187,243);
    m_clrPressedButton = wxColour(36,227,58);
    m_clrDisabledButton = wxColour(128,128,128);

    m_clrText = *wxWHITE;
    m_clrText = *wxWHITE;
    m_clrTextSelected = *wxWHITE;
    m_clrTextFlash = *wxWHITE;


    m_szButtons = szButtons;
    m_nColumns = nColumns;
    m_szGap = szGap;

    if(m_szButtons.GetHeight() <= 0)
    {
        m_szButtons.SetHeight(40);
    }
    if(m_szButtons.GetWidth() > 0)
    {
        m_nColumns = (GetClientRect().GetWidth()-4-m_szGap.GetWidth())/(m_szButtons.GetWidth()+m_szGap.GetWidth());
    }
    m_pFactory = 0;

    m_nBorderStyle = uiRect::BORDER_UP;
    m_nTextAlign = wxALIGN_CENTER;
    m_nBitmapAlign = wxALIGN_CENTER;

    m_bDownInWindow = false;

    SetDefaultFactory();
    SetFont(wxFont(10,wxFONTFAMILY_SWISS,wxFONTSTYLE_NORMAL,wxFONTWEIGHT_NORMAL,false,_T("Arial"),wxFONTENCODING_DEFAULT));
    m_pSlideWnd = 0;
    m_nSensitivity = 25;
    return true;
}

void wmList::SetSwipeSensitivity(unsigned long nPixels)
{
    m_nSensitivity = nPixels;
}

void wmList::SetDefaultFactory()
{
//    wxLogDebug(wxT("wmList::SetDefaultFactory"));
    SetButtonFactory(new wmButtonFactory());
}

wxSize wmList::DoGetBestSize() const
{
    return wxSize(100,600);
}

void wmList::SetBorderStyle(unsigned short nStyle)
{
    m_nBorderStyle = nStyle;
    Refresh();
}


void wmList::OnPaint(wxPaintEvent& event)
{
    wxBufferedPaintDC dc(this);
    Draw(dc, m_itTop);

}

void wmList::SetSize(const wxSize& size)
{
    wxWindow::SetSize(size);

    CreateRects();
}

void wmList::OnSize(wxSizeEvent& event)
{
    CreateRects();
    Refresh();
}

void wmList::OnLeftDown(wxMouseEvent& event)
{
    CaptureMouse();
    m_nScrolling = SCROLL_NONE;
    m_nSliding = SLID_NONE;
    m_timerScroll.Stop();
    m_bDownInWindow = true;
    m_nSwipeCount = 0;
    m_nSwipeTotal = 0;

    m_pntMouse = event.GetPosition();
    m_itDown = m_lstButtons.end();
    for(list<button*>::iterator itButton = m_itTop; itButton != m_lstButtons.end(); ++itButton)
    {
        if((*itButton)->pUi->Contains(event.GetPosition()))// && (*itButton)->bEnabled)
        {
            if((*itButton)->nEnabled == wmENABLED)
            {
                m_itDown = itButton;
                m_clrDown = (*itButton)->clrPressed;
                wxRect rect((*itButton)->pUi->GetRect());
                //CreateSlideBitmap();
                if((m_nStyle & STYLE_HOLD))
                {
                    m_timerHold.Start(50);
                }
                m_nHoldCount = 0;
                Refresh();
                Update();

            }
            break;
        }
    }

    event.Skip();
}

void wmList::OnLeftUp(wxMouseEvent& event)
{

    m_bDownInWindow = false;

    if(m_itDown != m_lstButtons.end())
    {
        m_timerHold.Stop();
        if(m_nScrolling == SCROLL_NONE)
        {
            if((*m_itDown)->pUi->Contains(event.GetPosition()))
            {
                if((*m_itDown)->nEnabled == wmENABLED)
                {
                    SelectButton(m_itDown);
                    Refresh();
                }
            }
            else
            {
                int nSlid(SLID_NONE);
                if(event.GetPosition().y >= (*m_itDown)->pUi->GetTop() && event.GetPosition().y <= (*m_itDown)->pUi->GetBottom())
                {
                    if(event.GetPosition().x < (*m_itDown)->pUi->GetLeft())
                    {
                        nSlid = SLID_LEFT;
                    }
                    else
                    {
                        nSlid = SLID_RIGHT;
                    }
                }
                else if(event.GetPosition().x >= (*m_itDown)->pUi->GetLeft() && event.GetPosition().x <= (*m_itDown)->pUi->GetRight())
                {
                    if(event.GetPosition().y < (*m_itDown)->pUi->GetTop())
                    {
                        nSlid = SLID_UP;
                    }
                    else
                    {
                        nSlid = SLID_DOWN;
                    }
                }
                if(nSlid != SLID_NONE)
                {
                    wxCommandEvent eventSlid(wxEVT_LIST_SLID, GetId());
                    eventSlid.SetEventObject(this);
                    eventSlid.SetString((*m_itDown)->pUi->GetLabel());
                    eventSlid.SetClientData((*m_itDown)->pUi->GetClientData());
                    eventSlid.SetInt((*m_itDown)->pUi->GetIndex());
                    eventSlid.SetExtraLong(nSlid);
                    wxPostEvent(GetEventHandler(), eventSlid);
                }
            }
        }
        m_itDown = m_lstButtons.end();
        Refresh();
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


void wmList::OnMouseMove(wxMouseEvent& event)
{
    if(event.LeftIsDown() && m_bDownInWindow && m_lstButtons.empty() == false)
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
                    if(m_lstButtons.back()->pUi->GetBottom() > GetClientSize().GetHeight() || m_lstButtons.front()->pUi->GetTop() < 0)    //only scroll if more that on screen
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
                        set<std::list<button*>::iterator>::iterator itPage = m_setPages.find(m_itTop);
                        if(itPage != m_setPages.end())
                        {
                            if(itPage != m_setPages.begin())
                            {
                                --itPage;
                                m_itSwipe = itPage;
                                m_nScrolling = SCROLL_HORIZONTAL;
                            }
                            else if((m_nStyle & STYLE_CIRCULAR))
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
                        set<std::list<button*>::iterator>::iterator itPage = m_setPages.find(m_itTop);
                        if(itPage != m_setPages.end())
                        {
                            ++itPage;
                            if(itPage != m_setPages.end())
                            {
                                m_itSwipe = itPage;
                                m_nScrolling = SCROLL_HORIZONTAL_RIGHT;
                            }
                            else if((m_nStyle & STYLE_CIRCULAR))
                            {
                                m_itSwipe = m_setPages.begin();
                                m_nScrolling = SCROLL_HORIZONTAL_RIGHT;
                            }
                        }
                        m_timerHold.Stop();
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
        else
        {
            if(m_itDown != m_lstButtons.end())
            {
                CheckSliding(event.GetPosition());
            }
        }
    }

    m_pntMouse = event.GetPosition();

    event.Skip();
}

void wmList::CheckSliding(wxPoint pnt)
{
    m_nSwipeOffset =  pnt.x-m_pntMouse.x;
    m_nScrollOffset =  pnt.y-m_pntMouse.y;

    if(m_nSliding == SLID_NONE && ((*m_itDown)->nSlideAllowed & SLID_UP) && m_nScrollOffset < -5)
    {
        m_nSliding = SLID_UP;
        CreateSlideBitmap();
        m_timerHold.Stop();
    }

    if(m_nSliding == SLID_NONE && ((*m_itDown)->nSlideAllowed & SLID_DOWN) && m_nScrollOffset > 5)
    {
        m_nSliding = SLID_DOWN;
        CreateSlideBitmap();
        m_timerHold.Stop();
    }

    if(m_nSliding == SLID_NONE && ((*m_itDown)->nSlideAllowed & SLID_RIGHT) && m_nSwipeOffset > 5)
    {
        m_nSliding = SLID_RIGHT;
        CreateSlideBitmap();
        m_timerHold.Stop();
    }
    if(m_nSliding == SLID_NONE && ((*m_itDown)->nSlideAllowed & SLID_LEFT) && m_nSwipeOffset < -5)
    {
        m_nSliding = SLID_LEFT;
        CreateSlideBitmap();
        m_timerHold.Stop();
    }

    if(m_pSlideWnd)
    {
        int x = m_pSlideWnd->GetPosition().x+(pnt.x-m_pntMouse.x);
        int y = m_pSlideWnd->GetPosition().y+(pnt.y-m_pntMouse.y);
        wxPoint pntTL(GetClientRect().GetTopLeft());
        wxPoint pntBR(GetClientRect().GetBottomRight());
        pntTL = ClientToScreen(pntTL);
        pntBR = ClientToScreen(pntBR);
        switch(m_nSliding)
        {
            case SLID_LEFT:
                if(x <= pntBR.x && x+m_pSlideWnd->GetSize().x >= pntTL.x)
                {
                    m_pSlideWnd->SetPosition(wxPoint(x, m_pSlideWnd->GetPosition().y));
                }
                break;
            case SLID_RIGHT:
                if(x >= pntTL.x && x <= pntBR.x)
                {
                    m_pSlideWnd->SetPosition(wxPoint(x, m_pSlideWnd->GetPosition().y));
                }
                break;
            case SLID_UP:
                if(y <= pntBR.y && y+m_pSlideWnd->GetSize().y >= pntTL.y)
                {
                    m_pSlideWnd->SetPosition(wxPoint(m_pSlideWnd->GetPosition().x, y));
                }
                break;
            case SLID_DOWN:
                if(y >= pntTL.y && y <= pntBR.y)
                {
                    m_pSlideWnd->SetPosition(wxPoint(m_pSlideWnd->GetPosition().x, y));
                }
                break;
        }
    }
}

void wmList::CreateSwipeBitmaps()
{
    #ifndef __WXGNU__
    m_pbmpSwipe[0] = new wxBitmap(GetClientSize().x, GetClientSize().y, 32);
    m_pbmpSwipe[1] = new wxBitmap(GetClientSize().x, GetClientSize().y, 32);

    //copy the current screen view in to the current bitmap
    wxMemoryDC memDC;
    //wxBitmap bmpTemp(GetClientRect().GetWidth(), GetClientRect().GetHeight(),24);
    memDC.SelectObject(*m_pbmpSwipe[0]);
    Draw(memDC, m_itTop);
    memDC.SelectObject(wxNullBitmap);
    //m_bmpSwipe[0] = bmpTemp;

    memDC.SelectObject(*m_pbmpSwipe[1]);
    Draw(memDC, (*m_itSwipe));
    memDC.SelectObject(wxNullBitmap);
    //m_bmpSwipe[1] = bmpTemp;
    #endif
}

void wmList::CreateSlideBitmap()
{
    if(m_itDown != m_lstButtons.end())
    {
        //m_bmpSlide.SetSize((*m_itDown)->pUi->GetRect().GetWidth(), (*m_itDown)->pUi->GetRect().GetHeight());
        wxRect rect((*m_itDown)->pUi->GetRect());
        wxString sLabel((*m_itDown)->pUi->GetLabel());
        wxBitmap bmp((*m_itDown)->bmp[0]);

        (*m_itDown)->pUi->Offset(-(*m_itDown)->pUi->GetLeft(), -(*m_itDown)->pUi->GetTop());
        if((*m_itDown)->sSlide != wxEmptyString)
        {
            (*m_itDown)->pUi->SetLabel((*m_itDown)->sSlide);
        }
        (*m_itDown)->bmp[0] = wxNullBitmap;
        m_clrDown = (*m_itDown)->clrButton;

        wxMemoryDC memDC;
        wxBitmap bmpTemp((*m_itDown)->pUi->GetRect().GetWidth(), (*m_itDown)->pUi->GetRect().GetHeight());
        memDC.SelectObject(bmpTemp);

        memDC.SetFont(GetFont());

        memDC.SetBackgroundMode(wxTRANSPARENT);
        memDC.SetPen(*wxTRANSPARENT_PEN);
        memDC.SetBrush(wxBrush(GetBackgroundColour()));
        memDC.SetPen(*wxBLACK_PEN);
        memDC.SetTextForeground(GetForegroundColour());

        DrawButton(memDC, m_itDown);
        memDC.SelectObject(wxNullBitmap);
        //m_bmpSlide = bmpTemp;

        (*m_itDown)->pUi->SetRect(rect);
        (*m_itDown)->pUi->SetLabel(sLabel);
        (*m_itDown)->bmp[0] = wxBitmap(bmp);
        m_clrDown = (*m_itDown)->clrPressed;

        int x = rect.GetLeft();
        int y = rect.GetTop();
        ClientToScreen(&x,&y);

        if(!m_pSlideWnd)
        {
            m_pSlideWnd = new wmSlideWnd(bmpTemp, wxPoint(x,y), wxSize((*m_itDown)->pUi->GetRect().GetWidth(), (*m_itDown)->pUi->GetRect().GetHeight()));
        }
        else
        {
            m_pSlideWnd->SetBitmap(bmpTemp);
            m_pSlideWnd->SetPosition(wxPoint(x,y));
            m_pSlideWnd->SetSize((*m_itDown)->pUi->GetRect().GetSize());
        }
        m_pSlideWnd->Show();
    }

}

bool wmList::ScrollVertical(int nYDiff)
{
    int nMove(0);
    if(nYDiff != 0)
    {
        if(nYDiff > 0)
        {
            nMove = min(nYDiff, 2-m_lstButtons.front()->pUi->GetTop());
        }
        else
        {
            nMove = -min(-nYDiff, m_lstButtons.back()->pUi->GetBottom()-(GetClientRect().GetBottom()-2));
        }

        if(nMove != 0)
        {
            m_itTop = m_lstButtons.end();
            for(list<button*>::iterator itButton = m_lstButtons.begin(); itButton != m_lstButtons.end(); ++itButton)
            {
                (*itButton)->pUi->Offset(0, nMove);
                if(m_itTop == m_lstButtons.end() && (*itButton)->pUi->GetBottom() >= 0)
                {
                    m_itTop = itButton;
                }
            }
            Refresh();
            Update();
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


bool wmList::ScrollHorizontal(int nXDiff)
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
                m_nScrolling = SCROLL_NONE;
                Refresh();
                Update();

                wxCommandEvent event(wxEVT_LIST_PAGED, GetId());
                event.SetClientData((*m_itTop)->pUi->GetClientData());
                event.SetInt(GetCurrentPageNumber());
                wxPostEvent(GetParent(), event);

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

void wmList::DrawHorizontalScroll()
{
    #ifndef __WXGNU__
    wxClientDC dc(this);
    if(m_nSwipeLeft > 0)
    {
        dc.DrawBitmap(*m_pbmpSwipe[1],m_nSwipeLeft-m_pbmpSwipe[1]->GetWidth(), 0);
    }
    else
    {
        dc.DrawBitmap(*m_pbmpSwipe[1],m_pbmpSwipe[1]->GetWidth()+m_nSwipeLeft, 0);
    }
    dc.DrawBitmap(*m_pbmpSwipe[0],m_nSwipeLeft, 0);

    dc.DestroyClippingRegion();

    #endif
}


void wmList::OnScroll(wxTimerEvent& event)
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

void wmList::OnCaptureLost(wxMouseCaptureLostEvent& event)
{
    m_itDown = m_lstButtons.end();
    m_timerHold.Stop();
    Refresh();
}



void wmList::OnHolding(wxTimerEvent& event)
{

    if(m_itDown != m_lstButtons.end())
    {
        m_clrDown = wxColour(min(255, m_clrDown.Red()+20),
                             min(255, m_clrDown.Green()+20),
                             min(255, m_clrDown.Blue()+20));


        m_nHoldCount++;
        wxRect rect((*m_itDown)->pUi->GetRect());
        if(m_nHoldCount > 10)   //hold for 1 second
        {
            wxCommandEvent event(wxEVT_LIST_HELD, GetId());
            event.SetEventObject(this);
            event.SetString((*m_itDown)->pUi->GetLabel());
            event.SetClientData((*m_itDown)->pUi->GetClientData());
            event.SetInt((*m_itDown)->pUi->GetIndex());
            wxPostEvent(GetEventHandler(), event);

            m_timerHold.Stop();
            m_itDown = m_lstButtons.end();
        }


        RefreshRect(rect);
        Update();
    }

}

void wmList::SelectAll(bool bSelect, bool bEvent)
{
    if(bSelect)
    {
        for(list<button*>::iterator itButton = m_lstButtons.begin(); itButton != m_lstButtons.end(); ++itButton)
        {
            set<list<button*>::iterator>::iterator itSelected = m_setitSelected.find(itButton);
            if(itSelected == m_setitSelected.end() && bSelect)  //not already selected so select
            {
                SelectButton(itButton, bEvent);
            }
        }
    }
    else
    {
        m_setitSelected.clear();
        Refresh();
        Update();
    }
}

void wmList::SelectButton(std::list<button*>::iterator itSel, bool bEvent)
{


    if(itSel != m_lstButtons.end() && (*itSel)->nEnabled == wmENABLED)
    {
        //make this here before we possibly invalidate the iterator
        wxCommandEvent event(wxEVT_LIST_SELECTED, GetId());
        event.SetEventObject(this);
        event.SetString((*itSel)->pUi->GetLabel());
        event.SetClientData((*itSel)->pUi->GetClientData());
        event.SetInt((*itSel)->pUi->GetIndex());


        bool bSelected(false);
        if((m_nStyle & STYLE_SELECT))
        {
            if((m_nStyle & STYLE_SELECT_MULTI)==0)
            {
                m_setitSelected.clear();
            }

            if((m_nStyle & STYLE_SELECT_ROW))
            {
                //find the first column
                while((*itSel)->pUi->GetIntData() != 0)
                {
                    --itSel;
                }
                while(true)
                {
                    if(m_setitSelected.insert(itSel).second == false)   //aready there
                    {
                        m_setitSelected.erase(itSel);
                    }
                    else
                    {
                        bSelected = true;
                    }
                    ++itSel;
                    if(itSel == m_lstButtons.end() || (*itSel)->pUi->GetIntData() == 0)
                    {
                        break;
                    }
                }
            }
            else
            {
                if(m_setitSelected.insert(itSel).second == false)   //aready there
                {
                    m_setitSelected.erase(itSel);
                }
                else
                {
                    bSelected = true;
                }
            }
        }
        if(bEvent)
        {

            event.SetExtraLong(bSelected);
            //ProcessWindowEvent(event);
            wxPostEvent(GetEventHandler(), event);
        }
    }
    Refresh();
    Update();
}


void wmList::SelectFirstButton(bool bShow, bool bEvent)
{
    if(m_lstButtons.empty() == false)
    {
        if(bShow)
        {
            ShowButton(m_lstButtons.begin(),ONSCREEN, false);
        }
        SelectButton(m_lstButtons.begin(), bEvent);
    }
}

void wmList::SelectLastButton(bool bShow, bool bEvent)
{
    if(m_lstButtons.empty() == false)
    {
        list<button*>::iterator itButton = m_lstButtons.end();
        --itButton;

        if(bShow)
        {
            ShowButton(itButton,ONSCREEN, false);
        }
        SelectButton(itButton, bEvent);
    }
}

void wmList::SelectNextButton(bool bShow, bool bEvent)
{
    list<button*>::iterator itButton = m_lstButtons.end();

    if(m_lstButtons.empty() == false)
    {
        if(m_setitSelected.empty())
        {
            itButton = m_lstButtons.begin();
        }
        else
        {
            if((m_nStyle & STYLE_SELECT_MULTI)==0)
            {
                itButton = (*m_setitSelected.begin());
                ++itButton;
            }
            else
            {
                set<list<button*>::iterator>::iterator itLast = m_setitSelected.end();
                --itLast;
                itButton = (*itLast);
                ++itButton;
            }
        }
    }
    if(itButton != m_lstButtons.end())
    {
        if(bShow)
        {
            ShowButton(itButton,ONSCREEN, false);
        }
        SelectButton(itButton, bEvent);
    }
}

void wmList::SelectPreviousButton(bool bShow, bool bEvent)
{
    list<button*>::iterator itButton = m_lstButtons.end();
    if(m_lstButtons.empty() == false)
    {
        if(m_setitSelected.empty())
        {
            itButton = m_lstButtons.begin();
        }
        else
        {
            itButton = (*m_setitSelected.begin());
            --itButton;
        }
    }
    if(itButton != m_lstButtons.end())
    {
        if(bShow)
        {
            ShowButton(itButton,ONSCREEN, false);
            SelectButton(itButton, bEvent);
        }
    }
}

void wmList::Draw(wxDC& dc, const std::list<button*>::iterator& itTop)
{
    dc.SetFont(GetFont());

    dc.SetBackgroundMode(wxTRANSPARENT);
    dc.SetPen(*wxTRANSPARENT_PEN);
    dc.SetBrush(wxBrush(GetBackgroundColour()));
    dc.DrawRectangle(GetClientRect());

    dc.SetPen(*wxBLACK_PEN);
    dc.SetTextForeground(GetForegroundColour());


    unsigned int nColumn(0);
    for(list<button*>::iterator itButton = itTop; itButton != m_lstButtons.end(); ++itButton)
    {
        DrawButton(dc, itButton);

        //only draw to the end of the screen...
        if((*itButton)->pUi->GetBottom()+m_szGap.GetHeight() >= GetClientRect().GetBottom())
        {
            nColumn++;
            if(nColumn >= m_nColumns)
            {
                m_itLast = itButton;
                break;
            }
        }
    }
}

void wmList::DrawButton(wxDC& dc, const std::list<button*>::iterator& itButton)
{
    if((*itButton)->nEnabled != wmHIDDEN)
    {
        if((*itButton)->nEnabled != wmDISABLED)
        {
            unsigned int nState(uiRect::BORDER_FLAT);

            //set the bitmap to draw
            (*itButton)->pUi->SetBitmap((*itButton)->bmp[(*itButton)->bFlashing]);

            set<list<button*>::iterator>::iterator itFlash = m_setitFlash.find(itButton);
            if(itFlash != m_setitFlash.end() && (*itButton)->bFlashing)
            {
                (*itButton)->pUi->SetBackgroundColour((*itButton)->clrFlashing);
                (*itButton)->pUi->SetForegroundColour((*itButton)->clrTextFlashing);

                set<list<button*>::iterator>::iterator itSel = m_setitSelected.find(itButton);
                if(itSel != m_setitSelected.end())
                {
                    nState = uiRect::BORDER_DOWN;
                }
                else if(itButton == m_itDown)
                {
                    nState = uiRect::BORDER_DOWN;
                }
                else
                {
                    nState = uiRect::BORDER_UP;
                }

            }
            else
            {
                set<list<button*>::iterator>::iterator itSel = m_setitSelected.find(itButton);
                if(itSel != m_setitSelected.end())
                {
                    (*itButton)->pUi->SetBackgroundColour((*itButton)->clrSelected);
                    (*itButton)->pUi->SetForegroundColour((*itButton)->clrTextSelected);
                    nState = uiRect::BORDER_DOWN;
                }
                else if(itButton == m_itDown)
                {
                    (*itButton)->pUi->SetBackgroundColour(m_clrDown);
                    (*itButton)->pUi->SetForegroundColour((*itButton)->clrText);
                    nState = uiRect::BORDER_DOWN;
                }
                else
                {
                    (*itButton)->pUi->SetBackgroundColour((*itButton)->clrButton);
                    (*itButton)->pUi->SetForegroundColour((*itButton)->clrText);
                    nState = uiRect::BORDER_UP;
                }
            }


            switch(m_nBorderStyle)
            {
                case uiRect::BORDER_DOWN:
                    if(nState == uiRect::BORDER_UP)
                    {
                        nState = uiRect::BORDER_DOWN;
                    }
                    else
                    {
                        nState =uiRect::BORDER_UP;
                    }
                    break;
                case uiRect::BORDER_FLAT:
                    nState = uiRect::BORDER_FLAT;
                    break;
                case uiRect::BORDER_NONE:
                    nState = uiRect::BORDER_NONE;
                    break;
            }

            (*itButton)->pUi->Draw(dc, nState);
        }
        else
        {
            (*itButton)->pUi->SetBackgroundColour(m_clrDisabledButton);
            (*itButton)->pUi->Draw(dc,uiRect::BORDER_FLAT);
        }
    }
}

void wmList::SetGradient(unsigned int nGradient)
{
    if(m_nGradient != nGradient)
    {
        m_nGradient = nGradient;
        for(list<button*>::iterator itButton = m_lstButtons.begin(); itButton != m_lstButtons.end(); ++itButton)
        {
            (*itButton)->pUi->SetGradient(m_nGradient);
        }
        Refresh();
    }
}


size_t wmList::AddButton(const wxString& sButton, const wxBitmap& bmp, void* pData, short nEnabled, const wxColour& clrButton, const wxColour& clrPressed, const wxColour& clrSelected)
{
    if(m_pFactory)
    {
        m_lstButtons.push_back(new button(m_pFactory->CreateButton()));
        m_lstButtons.back()->nEnabled = nEnabled;

        m_lstButtons.back()->pUi->SetLabel(sButton);
        m_lstButtons.back()->pUi->SetBitmap(bmp);
        m_lstButtons.back()->pUi->SetClientData(pData);
        m_lstButtons.back()->pUi->SetTextAlign(m_nTextAlign);
        m_lstButtons.back()->pUi->SetBitmapAlign(m_nBitmapAlign);
        m_lstButtons.back()->pUi->SetGradient(m_nGradient);
        m_lstButtons.back()->pUi->SetIndex(m_nIndex);

        m_lstButtons.back()->bmp[0] = bmp;
        m_lstButtons.back()->bmp[1] = bmp;

        m_nIndex++;

        if(clrButton.IsOk())
        {
            m_lstButtons.back()->clrButton = clrButton;
        }
        else
        {
            m_lstButtons.back()->clrButton = m_clrButton;
        }

        if(clrPressed.IsOk())
        {
            m_lstButtons.back()->clrPressed = clrPressed;
        }
        else
        {
            m_lstButtons.back()->clrPressed = m_clrPressedButton;
        }

        if(clrSelected.IsOk())
        {
            m_lstButtons.back()->clrSelected = clrSelected;
        }
        else
        {
            m_lstButtons.back()->clrSelected = m_clrSelectedButton;
        }
        m_lstButtons.back()->clrFlashing = m_clrFlash;

        m_lstButtons.back()->clrText = m_clrText;
        m_lstButtons.back()->clrTextPressed = m_clrTextPressed;
        m_lstButtons.back()->clrTextSelected = m_clrTextSelected;
        m_lstButtons.back()->clrTextFlashing = m_clrTextFlash;




        if(m_itTop == m_lstButtons.end())
        {
            m_itTop = m_lstButtons.begin();
        }

        if(IsFrozen() == false)
        {
            CreateRects();
        }
        return m_lstButtons.back()->pUi->GetIndex();
    }
    return NOT_FOUND;
}

void wmList::Thaw()
{
    CreateRects();
    wxWindow::Thaw();
}

void wmList::Clear()
{
    for(list<button*>::iterator it = m_lstButtons.begin(); it != m_lstButtons.end(); ++it)
    {
        delete (*it);
    }
    m_nIndex = 0;
    m_lstButtons.clear();
    m_setitFlash.clear();

    m_itDown = m_lstButtons.end();
    m_itTop = m_lstButtons.end();
    m_setitSelected.clear();
    Refresh();
}

void wmList::CreateRects()
{
    if(m_lstButtons.empty() == false)
    {
        m_setPages.clear();

        if((m_nStyle & STYLE_RESIZE_BUTTONS))
        {
            int nHeight = min(40, max(30, GetClientRect().GetHeight()/(static_cast<int>(m_lstButtons.size())+m_szGap.GetHeight())));
            wxRect rect(2,2,(GetClientRect().GetWidth()-4)/m_nColumns, nHeight-1);
            int nColumn(0);
            int nLeft(2);


            for(list<button*>::iterator itButton = m_lstButtons.begin(); itButton != m_lstButtons.end(); ++itButton)
            {
                if(nColumn == 0 && rect.GetTop() == 2 && m_nScrollAllowed == SCROLL_HORIZONTAL)   //first button on page
                {
                    m_setPages.insert(itButton);
                }

                (*itButton)->pUi->SetRect(rect);
                rect.Offset(rect.GetWidth()+m_szGap.GetWidth(),0);

                (*itButton)->pUi->SetIntData(nColumn);

                ++nColumn;
                if(nColumn == m_nColumns)
                {
                    nColumn = 0;
                    rect.Offset(0,rect.GetHeight()+m_szGap.GetHeight());

                    //if we swipe then once we get to the bottom we create a new page
                    if(rect.GetTop() >= GetClientRect().GetBottom())
                    {
                        if(m_nScrollAllowed == SCROLL_HORIZONTAL)
                        {
                            nLeft += GetClientRect().GetWidth();
                            rect.SetTop(2);
                        }
                    }

                    rect.SetLeft(nLeft);
                }
            }
        }
        else
        {
            int nWidth(m_szButtons.GetWidth());
            if(nWidth <= 0)
            {
                nWidth = (GetClientRect().GetWidth()-4-((m_nColumns-1)*m_szGap.GetWidth()))/m_nColumns;
            }

            wxRect rect(2,2,nWidth, m_szButtons.GetHeight());
            int nColumn(0);
            int nLeft(2);
            for(list<button*>::iterator itButton = m_lstButtons.begin(); itButton != m_lstButtons.end(); ++itButton)
            {
                if(nColumn == 0 && rect.GetTop() == 2 && m_nScrollAllowed == SCROLL_HORIZONTAL)   //first button on page
                {
                    m_setPages.insert(itButton);
                }

                (*itButton)->pUi->SetRect(rect);
                (*itButton)->pUi->SetIntData(nColumn);

                rect.Offset(rect.GetWidth()+m_szGap.GetWidth(),0);

                ++nColumn;
                if(nColumn == m_nColumns)
                {
                    nColumn = 0;
                    rect.Offset(0,rect.GetHeight()+m_szGap.GetHeight());

                    //if we swipe then once we get to the bottom we create a new page
                    if(rect.GetTop() >= GetClientRect().GetBottom())
                    {
                        if(m_nScrollAllowed == SCROLL_HORIZONTAL)
                        {
                            rect.SetTop(2);
                        }
                    }

                    rect.SetLeft(nLeft);
                }
            }
        }
    }
}

void wmList::SetButtonHeight(unsigned int nHeight)
{
    m_szButtons.SetHeight(nHeight);
    CreateRects();
    Refresh();
}

void wmList::SelectButton(const wxString& sButton, bool bEvent)
{
    for(list<button*>::iterator itButton = m_lstButtons.begin(); itButton != m_lstButtons.end(); ++itButton)
    {
        if((*itButton)->pUi->GetLabel() == sButton)
        {
            SelectButton(itButton, bEvent);
        }
    }
}


size_t wmList::FindButton(void* pData)
{
    for(list<button*>::iterator itButton = m_lstButtons.begin(); itButton != m_lstButtons.end(); ++itButton)
    {
        if((*itButton)->pUi->GetClientData() == pData)
        {
            return (*itButton)->pUi->GetIndex();
        }
    }
    return NOT_FOUND;
}

size_t wmList::FindButton(const wxString& sLabel)
{
    for(list<button*>::iterator itButton = m_lstButtons.begin(); itButton != m_lstButtons.end(); ++itButton)
    {
        if((*itButton)->pUi->GetLabel() == sLabel)
        {
            return (*itButton)->pUi->GetIndex();
        }
    }
    return NOT_FOUND;
}

void wmList::DeleteButtons(void* pData)
{
    for(list<button*>::iterator itButton = m_lstButtons.begin(); itButton != m_lstButtons.end();)
    {
        if((*itButton)->pUi->GetClientData() == pData)
        {
            list<button*>::iterator itDelete(itButton);
            list<button*>::iterator itDelete2(itButton);
            list<button*>::iterator itDelete3(itButton);
            if(m_itTop == itButton)
            {
                ++m_itTop;
            }
            if(m_itDown == itButton)
            {
                m_itDown = m_lstButtons.end();
            }
            ++itButton;
            m_setitSelected.erase(itDelete);
            m_setitFlash.erase(itDelete2);
			m_lstButtons.erase(itDelete3);

        }
        else
        {
            ++itButton;
        }
    }
    if(!IsFrozen())
    {
        CreateRects();
    }
}

void wmList::SetButtonsColour(void* pData, const wxColour& clr)
{
    for(list<button*>::iterator itButton = m_lstButtons.begin(); itButton != m_lstButtons.end();++itButton)
    {
        if((*itButton)->pUi->GetClientData() == pData)
        {
            (*itButton)->clrButton = clr;
            RefreshRect((*itButton)->pUi->GetRect());
        }
    }
}

void wmList::SetTextButtonsColour(void* pData, const wxColour& clr)
{
    for(list<button*>::iterator itButton = m_lstButtons.begin(); itButton != m_lstButtons.end();++itButton)
    {
        if((*itButton)->pUi->GetClientData() == pData)
        {
            (*itButton)->clrText = clr;
            RefreshRect((*itButton)->pUi->GetRect());
        }
    }
}

void wmList::SetTextAlign(unsigned int nAlign)
{
    m_nTextAlign = nAlign;
    for(list<button*>::iterator itButton = m_lstButtons.begin(); itButton != m_lstButtons.end(); ++itButton)
    {
        (*itButton)->pUi->SetTextAlign(nAlign);
    }
    Refresh();

}

void wmList::SetBitmapAlign(unsigned int nAlign)
{
    m_nBitmapAlign = nAlign;
    for(list<button*>::iterator itButton = m_lstButtons.begin(); itButton != m_lstButtons.end(); ++itButton)
    {
        (*itButton)->pUi->SetBitmapAlign(nAlign);
    }
    Refresh();
}


list<wmList::button*>::iterator wmList::GetButton(size_t nIndex)
{
    list<button*>::iterator itButton = m_lstButtons.begin();
    for(; itButton != m_lstButtons.end(); ++itButton)
    {
        if((*itButton)->pUi->GetIndex() == nIndex)
            break;
    }
    return itButton;
}

list<wmList::button*>::iterator wmList::GetButton(void* pData)
{
    list<button*>::iterator itButton = m_lstButtons.begin();
    for(; itButton != m_lstButtons.end(); ++itButton)
    {
        if((*itButton)->pUi->GetClientData() == pData)
            break;
    }
    return itButton;
}


void wmList::SelectButton(size_t nButton, bool bEvent)
{
    list<button*>::iterator itButton = GetButton(nButton);
    if(itButton != m_lstButtons.end())
    {
        SelectButton(itButton, bEvent);
    }
}




void wmList::SetButtonColour(const wxColour& clr)
{
    m_clrButton = clr;
    for(list<button*>::iterator itButton = m_lstButtons.begin(); itButton != m_lstButtons.end(); ++itButton)
    {
        (*itButton)->clrButton = clr;
    }
    Refresh();
}

void wmList::SetFlashButtonColour(const wxColour& clr)
{
    m_clrFlash = clr;
    for(list<button*>::iterator itButton = m_lstButtons.begin(); itButton != m_lstButtons.end(); ++itButton)
    {
        (*itButton)->clrFlashing = clr;
    }
    Refresh();
}

void wmList::SetFlashButtonBitmap(size_t nButton, const wxBitmap& bmp)
{
    list<button*>::iterator itButton = GetButton(nButton);
    if(itButton != m_lstButtons.end())
    {
        (*itButton)->bmp[true] = bmp;
        RefreshRect((*itButton)->pUi->GetRect());
    }
}

void wmList::SetButtonBitmap(size_t nButton, const wxBitmap& bmp)
{
    list<button*>::iterator itButton = GetButton(nButton);
    if(itButton != m_lstButtons.end())
    {
        (*itButton)->bmp[false] = bmp;
        (*itButton)->pUi->SetBitmap(bmp);
        RefreshRect((*itButton)->pUi->GetRect());
    }
}

void wmList::SetSlideAllowed(size_t nIndex, short nSlide, const wxString& sLabel, bool bShowArrow)
{
    list<button*>::iterator itButton = GetButton(nIndex);
    if(itButton != m_lstButtons.end())
    {
        (*itButton)->nSlideAllowed = nSlide;
        (*itButton)->sSlide = sLabel;
        switch(nSlide)
        {
            case SLID_RIGHT:
                if(bShowArrow)
                {
                    (*itButton)->bmp[0] = wxBitmap(right16_xpm);
                }
                (*itButton)->pUi->SetBitmapAlign(wxALIGN_RIGHT | wxALIGN_BOTTOM);
                break;
            case SLID_LEFT:
                if(bShowArrow)
                {
                    (*itButton)->bmp[0] = wxBitmap(left16_xpm);
                }
                (*itButton)->pUi->SetBitmapAlign(wxALIGN_LEFT | wxALIGN_BOTTOM);
                break;
            case SLID_UP:
                if(bShowArrow)
                {
                    (*itButton)->bmp[0] = wxBitmap(up16_xpm);
                }
                (*itButton)->pUi->SetBitmapAlign(wxALIGN_RIGHT | wxALIGN_TOP);
                break;
            case SLID_DOWN:
                if(bShowArrow)
                {
                    (*itButton)->bmp[0] = wxBitmap(down16_xpm);
                }
                (*itButton)->pUi->SetBitmapAlign(wxALIGN_RIGHT | wxALIGN_BOTTOM);
                break;
        }

        RefreshRect((*itButton)->pUi->GetRect());
    }
}

void wmList::SetButtonColour(size_t nButton, const wxColour& clr)
{
    list<button*>::iterator itButton = GetButton(nButton);
    if(itButton != m_lstButtons.end())
    {
        if(clr.IsOk())
        {
            (*itButton)->clrButton = clr;
        }
        else
        {
            (*itButton)->clrButton = m_clrButton;
        }
        RefreshRect((*itButton)->pUi->GetRect());
    }
}

void wmList::SetFlashButtonColour(size_t nButton, const wxColour& clr)
{
    list<button*>::iterator itButton = GetButton(nButton);
    if(itButton != m_lstButtons.end())
    {
        if(clr.IsOk())
        {
            (*itButton)->clrButton = clr;
        }
        else
        {
            (*itButton)->clrButton = m_clrFlash;
        }
        RefreshRect((*itButton)->pUi->GetRect());
    }
}

void wmList::SetPressedButtonColour(size_t nButton, const wxColour& clr)
{
    list<button*>::iterator itButton = GetButton(nButton);
    if(itButton != m_lstButtons.end())
    {
        if(clr.IsOk())
        {
            (*itButton)->clrPressed = clr;
        }
        else
        {
            (*itButton)->clrPressed = m_clrPressedButton;
        }
        RefreshRect((*itButton)->pUi->GetRect());
    }
}

void wmList::SetSelectedButtonColour(size_t nButton, const wxColour& clr)
{
    list<button*>::iterator itButton = GetButton(nButton);
    if(itButton != m_lstButtons.end())
    {
        if(clr.IsOk())
        {
            (*itButton)->clrSelected = clr;
        }
        else
        {
            (*itButton)->clrSelected = m_clrSelectedButton;
        }
        RefreshRect((*itButton)->pUi->GetRect());
    }
}

void wmList::SetPressedButtonColour(const wxColour& clr)
{
    m_clrPressedButton = clr;
    for(list<button*>::iterator itButton = m_lstButtons.begin(); itButton != m_lstButtons.end(); ++itButton)
    {
        (*itButton)->clrPressed = clr;
    }
    Refresh();
}

void wmList::SetSelectedButtonColour(const wxColour& clr)
{
    m_clrSelectedButton = clr;
    for(list<button*>::iterator itButton = m_lstButtons.begin(); itButton != m_lstButtons.end(); ++itButton)
    {
        (*itButton)->clrSelected = clr;
    }
    Refresh();
}



void wmList::SetTextButtonColour(const wxColour& clr)
{
    m_clrText = clr;
    for(list<button*>::iterator itButton = m_lstButtons.begin(); itButton != m_lstButtons.end(); ++itButton)
    {
        (*itButton)->clrText = clr;
    }
    Refresh();
}

void wmList::SetTextFlashButtonColour(const wxColour& clr)
{
    m_clrTextFlash = clr;
    for(list<button*>::iterator itButton = m_lstButtons.begin(); itButton != m_lstButtons.end(); ++itButton)
    {
        (*itButton)->clrTextFlashing = clr;
    }
    Refresh();
}

void wmList::SetTextButtonColour(size_t nButton, const wxColour& clr)
{
    list<button*>::iterator itButton = GetButton(nButton);
    if(itButton != m_lstButtons.end())
    {
        if(clr.IsOk())
        {
            (*itButton)->clrText = clr;
        }
        else
        {
            (*itButton)->clrText = m_clrText;
        }
        RefreshRect((*itButton)->pUi->GetRect());
    }
}

void wmList::SetTextFlashButtonColour(size_t nButton, const wxColour& clr)
{
    list<button*>::iterator itButton = GetButton(nButton);
    if(itButton != m_lstButtons.end())
    {
        if(clr.IsOk())
        {
            (*itButton)->clrTextFlashing = clr;
        }
        else
        {
            (*itButton)->clrTextFlashing = m_clrTextFlash;
        }
        RefreshRect((*itButton)->pUi->GetRect());
    }
}

void wmList::SetTextPressedButtonColour(size_t nButton, const wxColour& clr)
{
    list<button*>::iterator itButton = GetButton(nButton);
    if(itButton != m_lstButtons.end())
    {
        if(clr.IsOk())
        {
            (*itButton)->clrTextPressed = clr;
        }
        else
        {
            (*itButton)->clrTextPressed = m_clrTextPressed;
        }
        RefreshRect((*itButton)->pUi->GetRect());
    }
}

void wmList::SetTextSelectedButtonColour(size_t nButton, const wxColour& clr)
{
    list<button*>::iterator itButton = GetButton(nButton);
    if(itButton != m_lstButtons.end())
    {
        if(clr.IsOk())
        {
            (*itButton)->clrTextSelected = clr;
        }
        else
        {
            (*itButton)->clrTextSelected = m_clrTextSelected;
        }
        RefreshRect((*itButton)->pUi->GetRect());
    }
}

void wmList::SetTextPressedButtonColour(const wxColour& clr)
{
    m_clrTextPressed = clr;
    for(list<button*>::iterator itButton = m_lstButtons.begin(); itButton != m_lstButtons.end(); ++itButton)
    {
        (*itButton)->clrTextPressed = clr;
    }
    Refresh();
}

void wmList::SetTextSelectedButtonColour(const wxColour& clr)
{
    m_clrTextSelected = clr;
    for(list<button*>::iterator itButton = m_lstButtons.begin(); itButton != m_lstButtons.end(); ++itButton)
    {
        (*itButton)->clrTextSelected = clr;
    }
    Refresh();
}

void wmList::EnableButton(size_t nButton, short nEnable)
{
    list<button*>::iterator itButton = GetButton(nButton);
    if(itButton != m_lstButtons.end())
    {
        (*itButton)->nEnabled = nEnable;
        RefreshRect((*itButton)->pUi->GetRect());
    }
}

bool wmList::Enable(bool bEnable)
{
    for(list<button*>::iterator itButton = m_lstButtons.begin(); itButton != m_lstButtons.end(); ++itButton)
    {
        if(bEnable)
        {
            (*itButton)->nEnabled = wmENABLED;
        }
        else
        {
            (*itButton)->nEnabled = wmDISABLED;
        }
    }
    Refresh();

    return true;
}

size_t wmList::GetSelectionCount() const
{
    return m_setitSelected.size();
}

wxArrayInt wmList::GetSelectedButtons() const
{
    wxArrayInt ai;
    for(set<list<button*>::iterator>::const_iterator itSel = m_setitSelected.begin(); itSel != m_setitSelected.end(); ++itSel)
    {
        ai.Add((*(*itSel))->pUi->GetIndex());
    }
    return ai;
}

bool wmList::IsSelected(size_t nButton) const
{
    for(set<list<button*>::iterator>::const_iterator itSel = m_setitSelected.begin(); itSel != m_setitSelected.end(); ++itSel)
    {
        if((*(*itSel))->pUi->GetIndex() == nButton)
        {
            return true;
        }
    }
    return false;
}


void wmList::SetButtonText(size_t nButton, const wxString& sText)
{
    list<button*>::iterator itButton = GetButton(nButton);
    if(itButton != m_lstButtons.end())
    {
        (*itButton)->pUi->SetLabel(sText);
        RefreshRect((*itButton)->pUi->GetRect());
    }
}

void wmList::SetButtonData(size_t nIndex, void* pData)
{
    list<button*>::iterator itButton = GetButton(nIndex);
    if(itButton != m_lstButtons.end())
    {
        (*itButton)->pUi->SetClientData(pData);
    }
}

void wmList::ShowButton(size_t nIndex, Position pos, bool bSelect)
{
    list<button*>::iterator itButton = GetButton(nIndex);
    if(itButton != m_lstButtons.end())
    {
        ShowButton(itButton, pos, bSelect);
    }
}

void wmList::ShowButton(void* pData, Position pos, bool bSelect)
{
    list<button*>::iterator itButton = GetButton(pData);
    if(itButton != m_lstButtons.end())
    {
        ShowButton(itButton, pos, bSelect);
    }
}

void wmList::ShowButton(list<button*>::iterator itButton, Position pos, bool bSelect)
{
    ///< @todo Do ShowButton for horizontal scroll
    int nDiff(0);
    switch(pos)
    {
        case TOP:
            nDiff = -(*itButton)->pUi->GetTop();
            if(m_lstButtons.front()->pUi->GetTop() + nDiff > 0)
            {
                nDiff = -m_lstButtons.front()->pUi->GetTop();
            }
            break;
        case BOTTOM:
            nDiff = GetClientRect().GetBottom()-(*itButton)->pUi->GetBottom();
            break;
        case MIDDLE:
            nDiff = (GetClientRect().GetBottom()-(*itButton)->pUi->GetBottom())-(GetClientRect().GetHeight()/2);
            if(m_lstButtons.front()->pUi->GetTop() + nDiff > 0)
            {
                nDiff = -m_lstButtons.front()->pUi->GetTop();
            }
            break;
        case ONSCREEN:
            if(GetClientRect().Contains((*itButton)->pUi->GetCenter()))
            {
                nDiff = 0;
            }
            else if((*itButton)->pUi->GetTop() < 0)
            {
                nDiff = -(*itButton)->pUi->GetTop();
            }
            else
            {
                nDiff = GetClientRect().GetBottom()-(*itButton)->pUi->GetBottom();
            }
    }
    if(nDiff != 0)
    {
        ScrollVertical(nDiff);

    }
    if(bSelect)
    {
        SelectButton(itButton, true);
    }
    Refresh();
}

wxString wmList::GetButtonText(size_t nButton) const
{
    list<button*>::const_iterator itButton = m_lstButtons.begin();
    for(; itButton != m_lstButtons.end(); ++itButton)
    {
        if((*itButton)->pUi->GetIndex() == nButton)
            return (*itButton)->pUi->GetLabel();
    }
    return wxEmptyString;
}
void* wmList::GetButtonData(size_t nButton)
{
    list<button*>::iterator itButton = GetButton(nButton);
    if(itButton != m_lstButtons.end())
    {
        return (*itButton)->pUi->GetClientData();
    }
    return 0;
}

void wmList::ClearSelection()
{
    m_setitSelected.clear();
    Refresh();
}

size_t wmList::GetItemCount() const
{
    return m_lstButtons.size();
}

size_t wmList::GetMaxIndex() const
{
    return m_nIndex;
}

void wmList::SetDisabledColour(const wxColour& clr)
{
    m_clrDisabledButton = clr;
    Refresh();
}

void wmList::SetTextDisabledButtonColour(const wxColour& clr)
{
    ///< @todo
}

void wmList::OnFocus(wxFocusEvent& event)
{
    if(event.GetWindow() && event.GetWindow() != this)
    {
        event.GetWindow()->SetFocus();
    }
}

void wmList::SetButtonFactory(wmButtonFactory* pFactory)
{
    if(m_pFactory)
    {
        delete m_pFactory;
    }
    m_pFactory = pFactory;
}

void wmList::OnFlash(wxTimerEvent& event)
{
    for(set<list<button*>::iterator>::iterator itButton = m_setitFlash.begin(); itButton != m_setitFlash.end(); ++itButton)
    {
        (*(*itButton))->bFlashing = !(*(*itButton))->bFlashing;
        RefreshRect((*(*itButton))->pUi->GetRect());
    }
}

void wmList::FlashButton(size_t nIndex, bool bFlash)
{
    list<button*>::iterator itButton = GetButton(nIndex);
    if(itButton != m_lstButtons.end())
    {
        if(bFlash)
        {
            if(m_setitFlash.empty())
            {
                m_timerFlash.Start(m_nFlashTime);
            }
            m_setitFlash.insert(itButton);
        }
        else
        {
            m_setitFlash.erase(itButton);
            if(m_setitFlash.empty())
            {
                m_timerFlash.Stop();
            }
        }
    }
}

wxArrayInt wmList::GetButtonIndexes(bool bIncludeHidden, bool bIncludeDisabled) const
{
    wxArrayInt ai;
    for(list<button*>::const_iterator itButton = m_lstButtons.begin(); itButton != m_lstButtons.end(); ++itButton)
    {
        if((bIncludeHidden || (*itButton)->nEnabled != wmHIDDEN) && (bIncludeDisabled || (*itButton)->nEnabled != wmDISABLED))
        {
            ai.Add((*itButton)->pUi->GetIndex());
        }
    }
    return ai;
}

void wmList::ShowPreviousPage(bool bSelect, bool bEvent)
{
    if(m_nScrollAllowed == SCROLL_HORIZONTAL)
    {
        m_itSwipe = m_setPages.end();
        set<std::list<button*>::iterator>::iterator itPage = m_setPages.find(m_itTop);
        if(itPage != m_setPages.end())
        {

            if(itPage != m_setPages.begin())
            {
                --itPage;
                m_itSwipe = itPage;
                m_nScrolling = SCROLL_HORIZONTAL;
            }
            else if((m_nStyle & STYLE_CIRCULAR))
            {
                m_itSwipe = m_setPages.end();
                --m_itSwipe;
                m_nScrolling = SCROLL_HORIZONTAL;
            }
        }

        m_nSwipeLeft = 0;//GetClientRect().GetWidth();
        m_timerHold.Stop();
        if(m_itSwipe != m_setPages.end())
        {
            m_nSwipeOffset = 50;// max(1, (GetClientRect().GetWidth()-m_nSwipeLeft)/10);
            CreateSwipeBitmaps();
            m_timerScroll.Start(10);
        }
    }
    else
    {
        if(m_lstButtons.empty() == false)
        {
            list<button*>::iterator itButton = m_lstButtons.begin();
            int nButton = (*itButton)->pUi->GetTop();
            int nDiff = min(GetClientSize().GetHeight(), -nButton);
            ScrollVertical(nDiff);
        }
    }

    if(bSelect)
    {
        Update(); //force a redrwaw
        SelectButton(m_itLast, bEvent);
    }


}

void wmList::ShowFirstPage(bool bSelect, bool bEvent)
{
    if(m_nScrollAllowed == SCROLL_HORIZONTAL)
    {
        set<std::list<button*>::iterator>::iterator itPage = m_setPages.find(m_itTop);
        if(itPage != m_setPages.end())
        {
            itPage = m_setPages.begin();
            m_itSwipe = itPage;
            m_nScrolling = SCROLL_HORIZONTAL;
        }
        m_nSwipeLeft = 0;//GetClientRect().GetWidth();
        m_timerHold.Stop();
        m_nSwipeOffset = 50;// max(1, (GetClientRect().GetWidth()-m_nSwipeLeft)/10);
        CreateSwipeBitmaps();
        m_timerScroll.Start(10);
    }
    else
    {
        if(m_lstButtons.empty() == false)
        {
            list<button*>::iterator itButton = m_lstButtons.begin();
            int nButton = (*itButton)->pUi->GetTop();
            int nDiff = min(GetClientSize().GetHeight(), -nButton);
            ScrollVertical(nDiff);
        }
    }

    if(bSelect)
    {
        Update(); //force a redrwaw
        SelectButton(m_itLast, bEvent);
    }

}

void wmList::ShowLastPage(bool bSelect, bool bEvent)
{
    if(m_nScrollAllowed == SCROLL_HORIZONTAL)
    {
        set<std::list<button*>::iterator>::iterator itPage = m_setPages.find(m_itTop);
        if(itPage != m_setPages.end())
        {
            itPage = m_setPages.end();
            --itPage;
            m_itSwipe = itPage;
            m_nScrolling = SCROLL_HORIZONTAL_RIGHT;
        }

        m_timerHold.Stop();
        CreateSwipeBitmaps();
        m_nSwipeOffset = -50;
        m_nSwipeLeft = 0;
        m_timerScroll.Start(10);
    }
    else
    {
        if(m_lstButtons.empty() == false)
        {
            list<button*>::iterator itButton = m_lstButtons.end();
            --itButton;
            int nButton = (*itButton)->pUi->GetBottom();
            int nDiff = min(GetClientSize().GetHeight(), nButton - GetClientRect().GetHeight());
            ScrollVertical(-nDiff);
        }
    }

    if(bSelect)
    {
        Update(); //force a redrwaw
        SelectButton(m_itTop, bEvent);
    }
}

void wmList::ShowNextPage(bool bSelect, bool bEvent)
{
    if(m_nScrollAllowed == SCROLL_HORIZONTAL)
    {
        m_itSwipe = m_setPages.end();

        set<std::list<button*>::iterator>::iterator itPage = m_setPages.find(m_itTop);
        if(itPage != m_setPages.end())
        {
            ++itPage;
            if(itPage != m_setPages.end())
            {
                m_itSwipe = itPage;
                m_nScrolling = SCROLL_HORIZONTAL_RIGHT;
            }
            else if((m_nStyle & STYLE_CIRCULAR))
            {
                m_itSwipe = m_setPages.begin();
                m_nScrolling = SCROLL_HORIZONTAL_RIGHT;
            }
        }

        m_timerHold.Stop();
        m_nSwipeOffset = -50;
        m_nSwipeLeft = 0;
        if(m_itSwipe != m_setPages.end())
        {
            CreateSwipeBitmaps();
            m_timerScroll.Start(10);
        }


    }
    else
    {
        if(m_lstButtons.empty() == false)
        {
            list<button*>::iterator itButton = m_lstButtons.end();
            --itButton;
            int nButton = (*itButton)->pUi->GetBottom();
            int nDiff = min(GetClientSize().GetHeight(), nButton - GetClientRect().GetHeight());
            ScrollVertical(-nDiff);
        }
    }

    if(bSelect)
    {
        Update(); //force a redrwaw
        SelectButton(m_itTop, bEvent);
    }
}

unsigned long wmList::GetCurrentPageNumber() const
{
    unsigned long nCount = 1;
    for(set<list<button*>::iterator>::const_iterator itPage = m_setPages.begin(); itPage != m_setPages.end(); ++itPage)
    {
        if((*itPage) == m_itTop)
            return nCount;

        ++nCount;
    }
    return 0;

}

unsigned long wmList::GetPageCount() const
{
    return m_setPages.size();
}

void wmList::DeleteButton(size_t nIndex)
{
    list<button*>::iterator itButton = GetButton(nIndex);
    if(itButton != m_lstButtons.end())
    {
        if(m_itDown == itButton)
        {
            m_itDown = m_lstButtons.end();
        }
        if(m_itTop == itButton)
        {
            ++m_itTop;
        }
        if(m_itLast == itButton)
        {
            m_itLast = m_lstButtons.end();
        }
        m_setitSelected.erase(itButton);
        m_setitFlash.erase(itButton);
        delete (*itButton);
        m_lstButtons.erase(itButton);
        CreateRects();
        Refresh();
    }
}


wxColour wmList::GetButtonColour(size_t nIndex) const
{
    list<button*>::const_iterator itButton = m_lstButtons.begin();
    for(; itButton != m_lstButtons.end(); ++itButton)
    {
        if((*itButton)->pUi->GetIndex() == nIndex)
        {
            return (*itButton)->clrButton;
        }
    }
    return wxNullColour;
}

wxColour wmList::GetButtonPressedColour(size_t nIndex) const
{
    list<button*>::const_iterator itButton = m_lstButtons.begin();
    for(; itButton != m_lstButtons.end(); ++itButton)
    {
        if((*itButton)->pUi->GetIndex() == nIndex)
        {
            return (*itButton)->clrPressed;
        }
    }
    return wxNullColour;
}

wxColour wmList::GetButtonSelectedColour(size_t nIndex) const
{
    list<button*>::const_iterator itButton = m_lstButtons.begin();
    for(; itButton != m_lstButtons.end(); ++itButton)
    {
        if((*itButton)->pUi->GetIndex() == nIndex)
        {
            return (*itButton)->clrSelected;
        }
    }
    return wxNullColour;
}

wxColour wmList::GetButtonFlashColour(size_t nIndex) const
{
    list<button*>::const_iterator itButton = m_lstButtons.begin();
    for(; itButton != m_lstButtons.end(); ++itButton)
    {
        if((*itButton)->pUi->GetIndex() == nIndex)
        {
            return (*itButton)->clrFlashing;
        }
    }
    return wxNullColour;
}

wxColour wmList::GetButtonTextColour(size_t nIndex) const
{
    list<button*>::const_iterator itButton = m_lstButtons.begin();
    for(; itButton != m_lstButtons.end(); ++itButton)
    {
        if((*itButton)->pUi->GetIndex() == nIndex)
        {
            return (*itButton)->clrText;
        }
    }
    return wxNullColour;
}

wxColour wmList::GetButtonTextPressedColour(size_t nIndex) const
{
    list<button*>::const_iterator itButton = m_lstButtons.begin();
    for(; itButton != m_lstButtons.end(); ++itButton)
    {
        if((*itButton)->pUi->GetIndex() == nIndex)
        {
            return (*itButton)->clrTextPressed;
        }
    }
    return wxNullColour;
}

wxColour wmList::GetButtonTextSelectedColour(size_t nIndex) const
{
    list<button*>::const_iterator itButton = m_lstButtons.begin();
    for(; itButton != m_lstButtons.end(); ++itButton)
    {
        if((*itButton)->pUi->GetIndex() == nIndex)
        {
            return (*itButton)->clrTextSelected;
        }
    }
    return wxNullColour;
}

wxColour wmList::GetButtonTextFlashColour(size_t nIndex) const
{
    list<button*>::const_iterator itButton = m_lstButtons.begin();
    for(; itButton != m_lstButtons.end(); ++itButton)
    {
        if((*itButton)->pUi->GetIndex() == nIndex)
        {
            return (*itButton)->clrTextFlashing;
        }
    }
    return wxNullColour;
}


unsigned short wmList::GetEnabled(size_t nIndex) const
{
    list<button*>::const_iterator itButton = m_lstButtons.begin();
    for(; itButton != m_lstButtons.end(); ++itButton)
    {
        if((*itButton)->pUi->GetIndex() == nIndex)
        {
            return (*itButton)->nEnabled;
        }
    }
    return 0;
}

short wmList::GetSlideDirection(size_t nIndex) const
{
    list<button*>::const_iterator itButton = m_lstButtons.begin();
    for(; itButton != m_lstButtons.end(); ++itButton)
    {
        if((*itButton)->pUi->GetIndex() == nIndex)
        {
            return (*itButton)->nSlideAllowed;
        }
    }
    return 0;
}

wxString wmList::GetSlideText(size_t nIndex) const
{
    list<button*>::const_iterator itButton = m_lstButtons.begin();
    for(; itButton != m_lstButtons.end(); ++itButton)
    {
        if((*itButton)->pUi->GetIndex() == nIndex)
        {
            return (*itButton)->sSlide;
        }
    }
    return wxEmptyString;
}



wxColour wmList::GetButtonsColour() const
{
    return m_clrButton;
}

wxColour wmList::GetButtonsPressedColour() const
{
    return m_clrPressedButton;
}

wxColour wmList::GetButtonsSelectedColour() const
{
    return m_clrSelectedButton;
}

wxColour wmList::GetButtonsFlashColour() const
{
    return m_clrFlash;
}

wxColour wmList::GetButtonsTextColour() const
{
    return m_clrText;
}

wxColour wmList::GetButtonsTextPressedColour() const
{
    return m_clrTextPressed;
}

wxColour wmList::GetButtonsTextSelectedColour() const
{
    return m_clrTextSelected;
}

wxColour wmList::GetButtonsTextFlashColour() const
{
    return m_clrTextFlash;
}


wxColour wmList::GetButtonsDisabledColour() const
{
    return m_clrDisabledButton;
}


void wmList::SetColumns(unsigned int nColumns)
{
    m_nColumns = nColumns;
    CreateRects();
    Refresh();
}


void wmList::SetButtonAuxillaryText(size_t nIndex, const wxString& sText)
{
    list<button*>::iterator itButton = GetButton(nIndex);
    if(itButton != m_lstButtons.end() && (*itButton))
    {
        (*itButton)->sAuxillary = sText;
    }
}

wxString wmList::GetButtonAuxillaryText(size_t nIndex)
{
    list<button*>::iterator itButton = GetButton(nIndex);
    if(itButton != m_lstButtons.end() && (*itButton))
    {
        return (*itButton)->sAuxillary;
    }
    return wxEmptyString;
}


uiRect* wmList::GetButtonuiRect(size_t nButton)
{
    list<button*>::iterator itButton = GetButton(nButton);
    if(itButton != m_lstButtons.end() && (*itButton))
    {
        return (*itButton)->pUi;
    }
    return 0;
}
