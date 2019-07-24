#include "wmswitcherpanel.h"
#include <algorithm>
#include <wx/dcbuffer.h>
#include <wx/settings.h>
#include <wx/log.h>
#include <wx/bookctrl.h>

using namespace std;

const long wmSwitcherPanel::ID_SCROLLING     = wxNewId();

BEGIN_EVENT_TABLE(wmSwitcherPanel, pmPanel)
    EVT_SIZE(wmSwitcherPanel::OnSize)
    EVT_PAINT(wmSwitcherPanel::OnPaint)
    EVT_LEFT_DOWN(wmSwitcherPanel::OnLeftDown)
    EVT_LEFT_UP(wmSwitcherPanel::OnLeftUp)
    EVT_TIMER(ID_SCROLLING, wmSwitcherPanel::OnScroll)
END_EVENT_TABLE()

#ifdef WXSPAM
IMPLEMENT_DYNAMIC_CLASS(wmSwitcherPanel, pmPanel)
#else
wxIMPLEMENT_DYNAMIC_CLASS(wmSwitcherPanel, pmPanel);
#endif // WXSPAM

wmSwitcherPanel::wmSwitcherPanel() : pmPanel()
, m_nSelectedPage(0)
, m_nPageNameStyle(PAGE_NONE)
, m_nStyle(0)
{

}

wmSwitcherPanel::~wmSwitcherPanel()
{
    //dtor
}

wmSwitcherPanel::wmSwitcherPanel(wxWindow* pParent, wxWindowID id, const wxPoint& pos, const wxSize& size, long nStyle, const wxString& sName)
: pmPanel()
, m_nSelectedPage(0)
, m_nPageNameStyle(PAGE_NONE)
{
    Create(pParent,id,pos,size,nStyle);
}

bool wmSwitcherPanel::Create(wxWindow* pParent, wxWindowID id, const wxPoint& pos,  const wxSize& size, long nStyle)
{
   if(!wxPanel::Create(pParent,id,pos,size,nStyle,wxEmptyString))
        return false;

    SetBackgroundStyle(wxBG_STYLE_CUSTOM);
    m_nStyle = nStyle;

    m_timerScroll.SetOwner(this, ID_SCROLLING);
    m_nScrolling = SCROLL_NONE;
    if((nStyle & STYLE_NOSWIPE) != 0)
    {
        m_nScrollAllowed = SCROLL_NONE;
    }
    else if((nStyle & STYLE_VERTICAL) ==0)
    {
        m_nScrollAllowed = SCROLL_HORIZONTAL;
    }
    else
    {
        m_nScrollAllowed = SCROLL_VERTICAL;
    }

    m_bDownInWindow = false;


    m_nSwipeHeight = 10;
    m_nNameHeight = 30;
    m_nSwipePage = 0;

    return true;
}

void wmSwitcherPanel::DisableSwipe(bool bDisable)
{
    if(bDisable)
    {
        m_nScrollAllowed = SCROLL_NONE;
    }
    else if((m_nStyle & STYLE_VERTICAL) ==0)
    {
        m_nScrollAllowed = SCROLL_HORIZONTAL;
    }
    else
    {
        m_nScrollAllowed = SCROLL_VERTICAL;
    }
}

void wmSwitcherPanel::AddPage(wxWindow* pPage, const wxString& sName, bool bSelect)
{

    InsertPage(m_vPages.size(),pPage,sName,bSelect);
}

bool wmSwitcherPanel::DisableAnimation(bool bDisable)
{
    if(bDisable && (m_nStyle & STYLE_NOANIMATION)==0)
    {
        m_nStyle |= STYLE_NOANIMATION;
    }
    else if(!bDisable && (m_nStyle & STYLE_NOANIMATION))
    {
        m_nStyle -= STYLE_NOANIMATION;
    }
    return true;
}

void wmSwitcherPanel::AdvanceSelection(bool bForward, bool bLoop)
{
    size_t nPage = m_nSelectedPage;
    if(bForward)
        nPage++;
    else if(nPage != 0)
        nPage--;
    else if(!m_vPages.empty())
        nPage = m_vPages.size()-1;

    if(nPage >= m_vPages.size() && bLoop)
    {
        nPage = 0;
    }
    if(nPage < m_vPages.size())
        ChangeSelection(nPage);
}

void wmSwitcherPanel::DeleteAllPages()
{
    m_vPages.clear();
}

void wmSwitcherPanel::DeletePage(size_t nPage)
{
    if(nPage < m_vPages.size())
        m_vPages.erase(m_vPages.begin()+nPage);
}

void wmSwitcherPanel::DeletePage(const wxString& sName)
{
    vector<page>::iterator it = m_vPages.begin();
    while(it != m_vPages.end())
    {
        if((*it).sPage == sName)
        {
            m_vPages.erase(it);
            break;
        }
        ++it;
    }
}

size_t wmSwitcherPanel::GetPageCount()
{
    return m_vPages.size();
}

size_t wmSwitcherPanel::GetSelection()
{
    return m_nSelectedPage;
}

wxString wmSwitcherPanel::GetSelectionName()
{
    if(m_nSelectedPage < m_vPages.size())
    {
        return m_vPages[m_nSelectedPage].sPage;
    }
    return wxEmptyString;
}

void wmSwitcherPanel::InsertPage(size_t nPage, wxWindow* pPage, const wxString& sName, bool bSelect)
{
    if(!pPage)
        return ;


    if(!bSelect && m_vPages.empty())
        bSelect = true;

    pPage->SetPosition(wxPoint(0, m_nSwipeHeight+m_nNameHeight));
    pPage->SetSize(wxSize(GetClientSize().GetWidth(), GetClientSize().GetHeight()-(m_nSwipeHeight+m_nNameHeight+4)));

    if(nPage < m_vPages.size())
    {
        m_vPages.insert(m_vPages.begin()+nPage,page(sName,pPage));
    }
    else
    {
        m_vPages.push_back(page(sName,pPage));
        nPage = m_vPages.size()-1;
    }

    if(bSelect)
    {
        ChangeSelection(nPage);
    }
    else
    {
        pPage->Show(false);
    }

    Refresh();
    Update();

}
size_t wmSwitcherPanel::ChangeSelection(size_t nPage)
{
    if(nPage >= m_vPages.size() || nPage == m_nSelectedPage)
        return nPage;

    if((m_nStyle & STYLE_NOANIMATION))
    {
        m_nSwipePage = nPage;
        size_t nReturn = m_nSelectedPage;
        SwipeFinished();

        #ifdef _pambase_
        wxBookCtrlEvent event(wxEVT_NOTEBOOK_PAGE_CHANGED, GetId());
        event.SetSelection(nReturn);
        event.SetEventObject(this);

        //HandleWindowEvent(event);
        GetEventHandler()->ProcessEvent(event);
        #endif // __pambase__

        return nReturn;
    }
    else if((m_nStyle & STYLE_VERTICAL) ==0)
    {
        ResetCurrentSwipe();

        m_nSwipePage = nPage;
        m_nSwipeLeft = 0;
        if(nPage < m_nSelectedPage)
        {
            m_vPages[m_nSwipePage].pPanel->SetPosition(wxPoint(-GetClientSize().GetWidth(), m_nSwipeHeight+m_nNameHeight+4));
            m_vPages[m_nSwipePage].pPanel->Show();
            m_nScrolling = SCROLL_HORIZONTAL;

            m_nSwipeOffset = max(1, (GetClientRect().GetWidth())/10);
        }
        else
        {
            m_vPages[m_nSwipePage].pPanel->SetPosition(wxPoint(GetClientSize().GetWidth(), m_nSwipeHeight+m_nNameHeight+4));
            m_vPages[m_nSwipePage].pPanel->Show();
            m_nScrolling = SCROLL_HORIZONTAL_RIGHT;

            m_nSwipeOffset = min(-1, -(GetClientRect().GetWidth())/10);
        }
        m_timerScroll.Start(10);

        #ifdef _pambase_
        wxBookCtrlEvent event(wxEVT_NOTEBOOK_PAGE_CHANGED, GetId());
        event.SetSelection(m_nSelectedPage);
        GetEventHandler()->ProcessEvent(event);
        #endif // __pambase__
        return m_nSelectedPage;
    }
    else
    {
        ResetCurrentSwipe();

        m_nSwipePage = nPage;
        m_nSwipeLeft = 0;
        if(nPage < m_nSelectedPage)
        {
            m_vPages[m_nSwipePage].pPanel->SetPosition(wxPoint(0, -GetClientSize().GetHeight()+m_nSwipeHeight+m_nNameHeight+4));
            m_vPages[m_nSwipePage].pPanel->Show();
            m_nScrolling = SCROLL_VERTICAL_DOWN;

            m_nSwipeOffset = max(1, (GetClientRect().GetHeight())/10);
        }
        else
        {
            m_vPages[m_nSwipePage].pPanel->SetPosition(wxPoint(0, GetClientSize().GetHeight()+m_nSwipeHeight+m_nNameHeight+4));
            m_vPages[m_nSwipePage].pPanel->Show();
            m_nScrolling = SCROLL_VERTICAL;

            m_nSwipeOffset = min(-1, -(GetClientRect().GetHeight())/10);
        }
        m_timerScroll.Start(10);

        #ifdef _pambase_
        wxBookCtrlEvent event(wxEVT_NOTEBOOK_PAGE_CHANGED, GetId());
        event.SetSelection(m_nSelectedPage);
        GetEventHandler()->ProcessEvent(event);
        #endif // __pambase__
        return m_nSelectedPage;
    }


}

wxString wmSwitcherPanel::ChangeSelection(const wxString& sName)
{
    //wxLogDebug(wxT("wmSwitcherPanel::ChangeSelection: %s"), sName.c_str());
    vector<page>::iterator it = m_vPages.begin();
    size_t nCount = 0;
    while(it != m_vPages.end())
    {
        if((*it).sPage.CmpNoCase(sName) == 0)
        {
            size_t nBefore = ChangeSelection(nCount);
            if(nBefore < m_vPages.size())
                return m_vPages[nBefore].sPage;
            break;
        }
        ++nCount;
        ++it;
    }
    return wxEmptyString;
}

void wmSwitcherPanel::OnSize(wxSizeEvent& event)
{
    ResizePages();
    event.Skip();
}

void wmSwitcherPanel::ResizePages()
{
    for(size_t i = 0; i < m_vPages.size(); i++)
    {
        m_vPages[i].pPanel->SetPosition(wxPoint(0, m_nSwipeHeight+m_nNameHeight));
        m_vPages[i].pPanel->SetSize(wxSize(GetClientSize().GetWidth(), GetClientSize().GetHeight()-(m_nSwipeHeight+m_nNameHeight)));
    }
    Refresh();
}


void wmSwitcherPanel::OnPaint(wxPaintEvent& event)
{
    wxAutoBufferedPaintDC dc(this);
    dc.SetTextForeground(GetForegroundColour());
    dc.SetFont(GetFont());

    if(m_vPages.empty())
    {
        dc.SetBrush(*wxRED_BRUSH);
        dc.DrawRectangle(0,0,GetClientSize().GetWidth(), GetClientSize().GetHeight());
        dc.DrawLabel(wxT("No panels added"), wxRect(0,0,GetClientSize().GetWidth(), GetClientSize().GetHeight()), wxALIGN_CENTER);
        dc.SetBrush(wxNullBrush);
    }
    else
    {
        dc.SetPen(*wxTRANSPARENT_PEN);
        dc.SetBrush(*wxBLACK_BRUSH);//wxBrush(GetBackgroundColour()));
        dc.DrawRectangle(0,0,GetClientSize().GetWidth(), GetClientSize().GetHeight());

        int nGap = 0;
        if(m_nPageNameStyle == PAGE_BUTTON)
        {
            nGap = (m_vPages.size()-1)*5;
        }
        int nWidth = min(100, int((GetClientSize().GetWidth()-nGap-4)/m_vPages.size()));

        if(m_nNameHeight > 0)
        {
            for(size_t i = 0; i < m_vPages.size(); i++)
            {
                if(m_nPageNameStyle == PAGE_BUTTON)
                {
                    m_vPages[i].uiLabel.SetTextAlign(wxALIGN_CENTER);
                    m_vPages[i].uiLabel.SetRect((nWidth+5)*i+2,2,nWidth, m_nNameHeight);
                }
                else
                {
                    m_vPages[i].uiLabel.SetRect(nWidth*i+2,2,nWidth, m_nNameHeight);
                    m_vPages[i].uiLabel.SetTextAlign(wxALIGN_LEFT | wxALIGN_CENTER_VERTICAL);
                }

                int nState;
                if(i == m_nSwipePage && (m_bDownInWindow || m_nPageNameStyle == PAGE_BUTTON))
                {
                    m_vPages[i].uiLabel.SetBackgroundColour(wxColour(255,175,10));
                    nState = uiRect::BORDER_DOWN;
                }
                else
                {
                    m_vPages[i].uiLabel.SetBackgroundColour(wxColour(140,145,150));
                    nState = uiRect::BORDER_UP;
                }

                switch(m_nPageNameStyle)
                {
                    case PAGE_FLAT:
                        m_vPages[i].uiLabel.Draw(dc, m_vPages[i].sPage, uiRect::BORDER_FLAT);
                        break;
                    case PAGE_BUTTON:

                        m_vPages[i].uiLabel.Draw(dc, m_vPages[i].sPage, nState);
                        break;
                    case PAGE_NONE:
                        dc.SetClippingRegion(m_vPages[i].uiLabel.GetRect());
                        dc.DrawLabel(m_vPages[i].sPage, m_vPages[i].uiLabel.GetRect());
                        dc.DestroyClippingRegion();
                }
            }
        }
        if(m_nSwipeHeight > 0)
        {
            dc.SetPen(*wxTRANSPARENT_PEN);
            int nOffset = (m_nSwipeLeft*nWidth)/(GetClientSize().GetWidth());
            m_rectSwipe = wxRect(nWidth*m_nSelectedPage-nOffset,m_nNameHeight,nWidth, m_nSwipeHeight);
            dc.SetBrush(wxBrush(wxColour(255,175,10)));
            dc.DrawRectangle(m_rectSwipe);
        }
    }
}


bool wmSwitcherPanel::ScrollHorizontal(int nXDiff)
{
    if(nXDiff != 0)
    {
        int nCurrent = m_nSwipeLeft;

        m_nSwipeLeft += nXDiff;

        m_vPages[m_nSelectedPage].pPanel->SetPosition(wxPoint(m_vPages[m_nSelectedPage].pPanel->GetPosition().x+nXDiff,m_nSwipeHeight+m_nNameHeight));
        m_vPages[m_nSwipePage].pPanel->SetPosition(wxPoint(m_vPages[m_nSwipePage].pPanel->GetPosition().x+nXDiff,m_nSwipeHeight+m_nNameHeight));

        if(m_nScrolling == SCROLL_HORIZONTAL || m_nScrolling == SCROLL_HORIZONTAL_RIGHT)
        {
            if(m_nSwipeLeft < -GetClientSize().GetWidth() || m_nSwipeLeft > GetClientSize().GetWidth())
            {
                SwipeFinished();
                return false;
            }
        }
        else if(m_nScrolling == SCROLL_RETURN)
        {
            if((nCurrent <= 0 && m_nSwipeLeft >=0) || (nCurrent >= 0 && m_nSwipeLeft <=0))
            {
                ResetCurrentSwipe();
                return false;
            }
        }

        DrawScroll();
        return true;
    }
    return false;

}

void wmSwitcherPanel::DrawScroll()
{
    Refresh();
    Update();
}


bool wmSwitcherPanel::ScrollVertical(int nYDiff)
{
    if(nYDiff != 0)
    {
        int nCurrent = m_nSwipeLeft;

        m_nSwipeLeft += nYDiff;

        m_vPages[m_nSelectedPage].pPanel->SetPosition(wxPoint(0,m_vPages[m_nSelectedPage].pPanel->GetPosition().y+nYDiff));
        m_vPages[m_nSwipePage].pPanel->SetPosition(wxPoint(0,m_vPages[m_nSwipePage].pPanel->GetPosition().y+nYDiff));

        if(m_nScrolling == SCROLL_VERTICAL || m_nScrolling == SCROLL_VERTICAL_DOWN)
        {
            if(m_nSwipeLeft < -GetClientSize().GetHeight() || m_nSwipeLeft > GetClientSize().GetHeight())
            {
                SwipeFinished();
                return false;
            }
        }
        else if(m_nScrolling == SCROLL_RETURN)
        {
            if((nCurrent <= 0 && m_nSwipeLeft >=0) || (nCurrent >= 0 && m_nSwipeLeft <=0))
            {
                ResetCurrentSwipe();
                return false;
            }
        }

        DrawScroll();
        return true;
    }
    return false;

}


void wmSwitcherPanel::OnScroll(wxTimerEvent& event)
{
    if(m_nScrolling == SCROLL_HORIZONTAL || m_nScrolling == SCROLL_HORIZONTAL_RIGHT)
    {
        ScrollHorizontal(m_nSwipeOffset);
    }
    else if(m_nScrolling == SCROLL_VERTICAL || m_nScrolling == SCROLL_VERTICAL_DOWN)
    {
        ScrollVertical(m_nSwipeOffset);
    }
    else if(m_nScrolling == SCROLL_RETURN)
    {
        if((m_nStyle & STYLE_VERTICAL)== 0)
        {
            ScrollHorizontal(m_nSwipeOffset);
        }
        else
        {
            ScrollVertical(m_nSwipeOffset);
        }
    }
}

wxWindow* wmSwitcherPanel::GetPage(size_t nPage)
{
    if(nPage < m_vPages.size())
    {
        return m_vPages[nPage].pPanel;
    }
    return NULL;
}


void wmSwitcherPanel::SetPageNameStyle(unsigned long nStyle)
{
    m_nPageNameStyle = nStyle;
    switch(m_nPageNameStyle)
    {
        case PAGE_HIDDEN:
            m_nNameHeight = 0;
            m_nSwipeHeight = 0;
            break;
        case PAGE_NONE:
            m_nNameHeight = 20;
            m_nSwipeHeight = 10;
            break;
        case PAGE_FLAT:
            m_nNameHeight = 25;
            m_nSwipeHeight = 10;
            break;
        case PAGE_BUTTON:
            m_nNameHeight = 35;
            m_nSwipeHeight = 0;
            break;
    }
    ResizePages();
    Refresh();
}

void wmSwitcherPanel::ResetCurrentSwipe()
{

    if(m_nSwipePage != 0xFFFFFFFF)
    {
        m_timerScroll.Stop();
        for(size_t i = 0; i < m_vPages.size(); i++)
        {
            m_vPages[i].pPanel->SetPosition(wxPoint(0,m_nSwipeHeight+m_nNameHeight));
            if(i != m_nSelectedPage)
            {
                m_vPages[i].pPanel->Show(false);
            }
            else
            {
                m_vPages[i].pPanel->Refresh();
                m_vPages[i].pPanel->Update();
            }
        }

        m_nScrolling = SCROLL_NONE;
        m_nSwipeLeft = 0;
        m_nSwipePage = 0xFFFFFFFF;
        Refresh();
        Update();
    }
}

void wmSwitcherPanel::SwipeFinished()
{
    m_timerScroll.Stop();
    m_vPages[m_nSelectedPage].pPanel->Show(false);
    m_vPages[m_nSelectedPage].pPanel->SetPosition(wxPoint(0,m_nSwipeHeight+m_nNameHeight));
    m_nSelectedPage = m_nSwipePage;
    m_vPages[m_nSelectedPage].pPanel->SetPosition(wxPoint(0,m_nSwipeHeight+m_nNameHeight));
    m_vPages[m_nSelectedPage].pPanel->Show(true);
    m_vPages[m_nSelectedPage].pPanel->Refresh();
    m_nScrolling = SCROLL_NONE;
    m_nSwipeLeft = 0;
    Refresh();
}

void wmSwitcherPanel::SetSwipeDirection(unsigned int nDirection)
{
    if(nDirection == STYLE_VERTICAL)
    {
        if((m_nStyle & STYLE_VERTICAL)==0)
        {
            m_nStyle += STYLE_VERTICAL;
        }
    }
    else
    {
        if((m_nStyle & STYLE_VERTICAL))
        {
            m_nStyle -= STYLE_VERTICAL;
        }
    }
}

wxString wmSwitcherPanel::GetPageText(size_t nPage)
{
    if(nPage < m_vPages.size())
    {
        return m_vPages[nPage].sPage;
    }
    return wxEmptyString;
}



void wmSwitcherPanel::OnLeftDown(wxMouseEvent& event)
{
    CaptureMouse();
    m_bDownInWindow = true;
    m_pntMouse = event.GetPosition();
    m_nSwipePage = 0xFFFFFFFF;
    for(size_t i = 0; i < m_vPages.size(); ++i)
    {
        if(m_vPages[i].uiLabel.Contains(event.GetPosition()))
        {
            m_nSwipePage = i;
            break;
        }
    }

    m_nSwipeCount = 0;
    m_nSwipeTotal = 0;
}

void wmSwitcherPanel::OnLeftUp(wxMouseEvent& event)
{
    m_bDownInWindow = false;
    if(m_nSwipePage != 0xFFFFFFFF && m_vPages[m_nSwipePage].uiLabel.Contains(event.GetPosition()))
    {
        ChangeSelection(m_nSwipePage);
    }

    if(HasCapture())
    {
        ReleaseMouse();
    }
}
