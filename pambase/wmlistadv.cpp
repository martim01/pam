#include "wmlistadv.h"
#include <wx/window.h>
#include <wx/dcbuffer.h>
#include <wx/log.h>
#include "wmslidewnd.h"
#include "icons/right16.xpm"
#include "icons/left16.xpm"
#include "icons/up16.xpm"
#include "icons/down16.xpm"
#include <algorithm>


using namespace std;

const long wmListAdv::ID_SCROLLING     = wxNewId();
const long wmListAdv::ID_HOLDING       = wxNewId();



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

    m_pDown = m_pSelected = m_pTop = nullptr;
    WorkoutScrollPosition();

    m_timerScroll.SetOwner(this, ID_SCROLLING);

    m_nStyle = nStyle;
    m_nScrollAllowed = nScroll;
    m_nIndex = 0;

    m_nScrollAllowed = SCROLL_VERTICAL;
    m_szGap = szGap;

    m_timerHold.SetOwner(this, ID_HOLDING);

    m_bDownInWindow = false;


    Connect(wxID_ANY, wxEVT_ADVELM_RESIZE, (wxObjectEventFunction)&wmListAdv::OnResizeElement);
    Connect(ID_HOLDING, wxEVT_TIMER, (wxObjectEventFunction)&wmListAdv::OnHolding);


    m_uiScroll.SetBackgroundColour(wxColour(100,150,100));

    return true;
}


wxSize wmListAdv::DoGetBestSize() const
{
    return wxSize(100,600);
}


void wmListAdv::OnPaint(wxPaintEvent& event)
{
    wxBufferedPaintDC dc(this);
    Draw(dc);

}

void wmListAdv::SetSize(const wxSize& size)
{
    wxWindow::SetSize(size);

}

void wmListAdv::OnResizeElement(wxCommandEvent& event)
{
    CreateRects();
    Refresh();
}

void wmListAdv::OnSize(wxSizeEvent& event)
{
    CreateRects();
    Refresh();
}

void wmListAdv::OnLeftDown(wxMouseEvent& event)
{
    if(!HasCapture())
    {
        CaptureMouse();
    }
    m_nScrolling = SCROLL_NONE;
    m_timerScroll.Stop();
    m_bDownInWindow = true;

    m_pntMouse = event.GetPosition();


    for(auto pElement : m_lstFilteredElements)
    {
        if(pElement->LeftDown(event) >=0)
        {
            m_pDown = pElement;
            m_timerHold.Start(50);
            Refresh();
            Update();
            break;
        }
    }
    event.Skip();
}

void wmListAdv::SelectSubElement(size_t nIndex, size_t nSub)
{
    auto pElement = GetElement(nIndex);
    if(pElement)
    {
        m_pSelected = pElement;
        pElement->SelectSubElement(nSub);
    }
}

wxRect wmListAdv::GetSubElementRect(size_t nIndex, size_t nSub)
{
    auto pElement = GetElement(nIndex);
    if(pElement)
    {
        return pElement->GetSubRect(nSub);
    }
    return wxRect(0,0,0,0);
}

void wmListAdv::OnLeftUp(wxMouseEvent& event)
{
    m_bDownInWindow = false;
    m_timerHold.Stop();

    bool bPressedElement(false);
    if(m_pDown)
    {
        int nSubElement = m_pDown->LeftUp(event,(m_nScrolling!=SCROLL_NONE));
        if(nSubElement >=0)
        {
            wxCommandEvent event(wxEVT_LISTADV_SELECTED, GetId());
            event.SetInt(m_pDown->GetIndex());
            event.SetClientData(reinterpret_cast<void*>(m_pDown->GetClientData()));
            event.SetExtraLong(nSubElement);
            wxPostEvent(GetEventHandler(), event);
            bPressedElement = true;
            m_pSelected = m_pDown;
        }
        m_pDown = nullptr;
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

    if(HasCapture())
    {
        ReleaseMouse();
    }
    event.Skip();
}


void wmListAdv::OnMouseMove(wxMouseEvent& event)
{
    if(event.LeftIsDown() && m_bDownInWindow && m_lstFilteredElements.empty() == false)
    {
        if(m_nScrollAllowed != SCROLL_NONE)
        {
            m_nScrollOffset =  event.GetPosition().y-m_pntMouse.y;

            if(m_nScrolling == SCROLL_NONE && m_nScrollAllowed == SCROLL_VERTICAL)
            {
                if(m_lstFilteredElements.back()->GetBottom() > GetClientSize().GetHeight() || m_lstFilteredElements.front()->GetTop() < 0)    //only scroll if more that on screen
                {
                    if(m_nScrollOffset < -5 || m_nScrollOffset > 5)
                    {
                        m_nScrolling = SCROLL_VERTICAL;
                        m_timerHold.Stop();
                    }
                }
            }
            if(m_nScrolling == SCROLL_VERTICAL)
            {
                ScrollVertical(m_nScrollOffset);
            }
        }
    }

    m_pntMouse = event.GetPosition();

    event.Skip();
}


bool wmListAdv::ScrollVertical(int nYDiff)
{
    int nMove(0);
    if(nYDiff != 0)
    {
        if(nYDiff > 0)
        {
            nMove = min(nYDiff, 2-m_lstFilteredElements.front()->GetTop());
        }
        else
        {
            nMove = -min(-nYDiff, m_lstFilteredElements.back()->GetBottom()-(GetClientRect().GetBottom()-2));
        }

        if(nMove != 0)
        {
            WorkoutScrollPosition();
            m_pTop = nullptr;
            for(auto pElement : m_lstFilteredElements)
            {
                pElement->Offset(0, nMove);
                if(m_pTop == nullptr && pElement->GetBottom() >= 0)
                {
                    m_pTop = pElement;
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
    }

void wmListAdv::OnCaptureLost(wxMouseCaptureLostEvent& event)
{
    m_pDown = nullptr;
    m_timerHold.Stop();
    Refresh();
}

void wmListAdv::OnHolding(wxTimerEvent& event)
{
    if(m_pDown)
    {
        int nSubElement = m_pDown->MouseHeld();
        if(nSubElement != 0)
        {
            wxCommandEvent event(wxEVT_LISTADV_HELD, GetId());
            event.SetInt(m_pDown->GetIndex());
            event.SetClientData(reinterpret_cast<void*>(m_pDown->GetClientData()));
            event.SetExtraLong(nSubElement);
            wxPostEvent(GetEventHandler(), event);

            m_pSelected = m_pDown;
            m_pDown = nullptr;
            Refresh();
        }
    }

}


void wmListAdv::Draw(wxDC& dc)
{
    dc.SetFont(GetFont());


    dc.SetBackgroundMode(wxTRANSPARENT);
    dc.SetPen(*wxTRANSPARENT_PEN);
    dc.SetBrush(wxBrush(GetBackgroundColour()));
    dc.DrawRectangle(GetClientRect());

    dc.SetPen(*wxBLACK_PEN);
    dc.SetTextForeground(GetForegroundColour());


    unsigned int nColumn(0);
    unsigned int nElements(0);

    for(auto pElement : m_lstFilteredElements)
    {
        if(pElement->GetBottom() > 0)
        {
            DrawElement(dc, pElement);
            nElements++;
        }

        //only draw to the end of the screen...
        if(pElement->GetBottom() >= GetClientRect().GetBottom())
        {
            break;
        }
    }

    double dPosition = static_cast<double>(nElements)/static_cast<double>(m_lstFilteredElements.size());
    dPosition *= static_cast<double>(GetClientSize().y);
    m_uiScroll.SetHeight(dPosition);
    m_uiScroll.Draw(dc, uiRect::BORDER_UP);
}

void wmListAdv::DrawElement(wxDC& dc, std::shared_ptr<advElement> pElement)
{
    pElement->DrawMe(dc, (pElement==m_pSelected));
}

bool wmListAdv::RemoveElement(size_t nIndex)
{
    Freeze();

    for(auto itElement = m_lstFilteredElements.begin(); itElement != m_lstFilteredElements.end(); ++itElement)
    {
        if((*itElement)->GetIndex() == nIndex)
        {
            bool bTop(false);
            if(m_pTop == (*itElement))
            {
                if(itElement != m_lstFilteredElements.begin())
                {
                    auto itTop = itElement;
                    --itTop;
                    m_pTop   = (*itTop);
                }
                else
                {
                    bTop = true;
                }
            }
            if(m_pDown == (*itElement))
            {
                m_pDown = nullptr;
            }
            if(m_pSelected == (*itElement))
            {
                m_pSelected = nullptr;
            }
            (*itElement)->Destroy();

            m_lstFilteredElements.erase(itElement);

//            if(m_lstFilteredElements.empty() == false)
//            {
//                if(bTop)
//                {
//                    m_pTop = m_lstFilteredElements.front());
//                    WorkoutScrollPosition();
//                }
//
//                CreateRects();
//                while(m_itTop != m_lstElements.begin() && (*m_itTop)->GetTop() > 2)
//                {
//                    --m_itTop;
//                }
//
//                int nMove = 2-(*m_itTop)->GetTop();
//                for(list<advElement*>::iterator itButton = m_lstElements.begin(); itButton != m_lstElements.end(); ++itButton)
//                {
//                    (*itButton)->Offset(0, nMove);
//                }
//            }
//            else
//            {
//                m_pTop = nullptr;
//                WorkoutScrollPosition();
//            }
            Thaw();
            Refresh();
            return true;
        }
    }
    return false;
}

size_t wmListAdv::AddElement(std::shared_ptr<advElement> pElement, bool bEnd, std::function<bool(std::shared_ptr<advElement>)> pFilter)
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

    if(!pFilter || pFilter(pElement))
    {
        if(bEnd)
        {
            m_lstFilteredElements.push_back(pElement);
        }
        else
        {
            m_lstFilteredElements.push_front(pElement);
        }


        if(!m_pTop)
        {
            m_pTop = m_lstFilteredElements.front();
            WorkoutScrollPosition();
        }

        if(IsFrozen() == false)
        {
            CreateRects();
            Refresh();
        }
    }
    return m_nIndex;
}

void wmListAdv::Thaw()
{
    wxWindow::Thaw();
}

void wmListAdv::Clear()
{
    for(auto pElement : m_lstElements)
    {
        pElement->Destroy();
    }
    m_lstElements.clear();
    m_lstFilteredElements.clear();

    m_nIndex = 0;
    m_pDown = nullptr;
    m_pSelected = nullptr;
    m_pTop = nullptr;
    WorkoutScrollPosition();
    Refresh();
}

std::shared_ptr<advElement> wmListAdv::GetElement(size_t nIndex)
{
    auto itElement = std::find_if(m_lstFilteredElements.begin(), m_lstFilteredElements.end(), [nIndex](std::shared_ptr<advElement> pElement) { return pElement->GetIndex() == nIndex;});
    if(itElement != m_lstFilteredElements.end())
    {
        return *itElement;
    }
    return nullptr;

}

std::shared_ptr<advElement> wmListAdv::GetFirstElement(void* pData)
{
    auto itElement = std::find_if(m_lstFilteredElements.begin(), m_lstFilteredElements.end(), [pData](std::shared_ptr<advElement> pElement) { return pElement->GetClientData() == pData;});
    if(itElement != m_lstFilteredElements.end())
    {
        return *itElement;
    }
    return nullptr;
}

size_t wmListAdv::FindFirstElement(void* pData)
{
    auto itElement = std::find_if(m_lstFilteredElements.begin(), m_lstFilteredElements.end(), [pData](std::shared_ptr<advElement> pElement) { return pElement->GetClientData() == pData;});
    if(itElement != m_lstFilteredElements.end())
    {
        return (*itElement)->GetIndex();
    }
    return (size_t)-1;
}

void wmListAdv::ShowElement(size_t nIndex, Position pos)
{
    auto pElement = GetElement(nIndex);
    if(pElement)
    {
        ShowElement(pElement, pos);
    }
}


void wmListAdv::ShowElement(std::shared_ptr<advElement> pElement, Position pos)
{
    ///< @todo Do ShowButton for horizontal scroll
    int nDiff(0);
    switch(pos)
    {
        case TOP:
            nDiff = -pElement->GetTop();
            if(m_lstFilteredElements.front()->GetTop() + nDiff > 0)
            {
                nDiff = -m_lstFilteredElements.front()->GetTop();
            }
            break;
        case BOTTOM:
            nDiff = GetClientRect().GetBottom()-pElement->GetBottom();
            break;
        case MIDDLE:
            nDiff = (GetClientRect().GetBottom()-pElement->GetBottom())-(GetClientRect().GetHeight()/2);
            if(m_lstFilteredElements.front()->GetTop() + nDiff > 0)
            {
                nDiff = -m_lstFilteredElements.front()->GetTop();
            }
            break;
        case ONSCREEN:
            if(GetClientRect().Contains(pElement->GetCenter()))
            {
                nDiff = 0;
            }
            else if(pElement->GetTop() < 0)
            {
                nDiff = -pElement->GetTop();
            }
            else
            {
                nDiff = GetClientRect().GetBottom()-pElement->GetBottom();
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
    return m_lstFilteredElements.size();
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
    if(m_lstFilteredElements.empty() == false)
    {
        auto pElement = m_lstFilteredElements.front();
        int nButton = pElement->GetTop();
        int nDiff = min(GetClientSize().GetHeight(), -nButton);
        ScrollVertical(nDiff);
    }

}


void wmListAdv::ShowNextPage()
{
    if(m_lstFilteredElements.empty() == false)
    {
        list<std::shared_ptr<advElement>>::iterator itButton = m_lstFilteredElements.end();
        --itButton;
        int nButton = (*itButton)->GetBottom();
        int nDiff = min(GetClientSize().GetHeight(), nButton - GetClientRect().GetHeight());
        ScrollVertical(-nDiff);
    }
}


void wmListAdv::CreateRects()
{
    int nTop = -1;
    for(auto pElement : m_lstFilteredElements)
    {
        if(nTop == -1)
        {
            nTop = pElement->GetTop();
        }
        if(nTop < 2)
        {
            nTop = 2;
        }

        pElement->SetTop(nTop);
        nTop += pElement->GetHeight();
        nTop += m_szGap.GetHeight();

    }
}


const list<std::shared_ptr<advElement>>& wmListAdv::GetElements() const
{
    return m_lstFilteredElements;
}


void wmListAdv::WorkoutScrollPosition()
{
    size_t nTopElement = 0;
    for(auto pElement : m_lstFilteredElements)
    {
        if(pElement == m_pTop)
        {
            break;
        }
        ++nTopElement;
    }
    double dPosition = static_cast<double>(nTopElement)/static_cast<double>(m_lstFilteredElements.size());
    dPosition *= static_cast<double>(GetClientSize().y);
    m_uiScroll.SetRect(GetClientRect().GetRight()-5, dPosition,5,30);



}


void wmListAdv::Filter(std::function<bool(std::shared_ptr<advElement>)> pFilter)
{
    bool bSelected(false);
    bool bTop(false);

    m_lstFilteredElements.clear();
    for(auto pElement : m_lstElements)
    {
        if(!pFilter || pFilter(pElement))
        {
            m_lstFilteredElements.push_back(pElement);
            if(pElement == m_pSelected)
            {
                bSelected = true;
            }
            if(pElement == m_pTop)
            {
                bTop = true;
            }
        }
    }

    if(!bSelected)
    {
        m_pSelected = nullptr;
    }
    if(!bTop)
    {
        m_pTop = m_lstFilteredElements.front();
    }
    CreateRects();
    WorkoutScrollPosition();
    Refresh();
}
