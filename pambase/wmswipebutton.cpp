#include "wmswipebutton.h"
#include <wx/dcbuffer.h>
#include <wx/log.h>

using namespace std;

BEGIN_EVENT_TABLE(wmSwipeButton, wmButton)
    EVT_MOTION(wmSwipeButton::OnMouseMove)
    EVT_LEFT_DOWN(wmSwipeButton::OnLeftDown)
    EVT_LEFT_UP(wmSwipeButton::OnLeftUp)
    EVT_LEFT_DCLICK(wmSwipeButton::OnLeftDClick)
    EVT_MOUSE_CAPTURE_LOST(wmSwipeButton::OnCaptureLost)
END_EVENT_TABLE()

 wxIMPLEMENT_DYNAMIC_CLASS(wmSwipeButton, wmButton);

wmSwipeButton::wmSwipeButton(wxWindow *parent, wxWindowID id, const wxString& sLabel, const wxPoint& pos, const wxSize& size, long nStyle,const wxValidator& validator, const wxString& name) :
    wmButton(parent, id, sLabel, pos, size, nStyle, validator, name)
{
    m_bVertical = ((nStyle & STYLE_VERTICAL) != 0);
    CreateRects();
}



wmSwipeButton::wmSwipeButton( wxWindow* parent, wxWindowID id, const wxBitmap& bitmap, const wxPoint& pos, const wxSize& size, long style, const wxValidator& validator, const wxString& name) :
    wmButton(parent, id, bitmap, pos, size, style, validator, name)
{
    m_bVertical = ((style & STYLE_VERTICAL) != 0);
    CreateRects();
}


wmSwipeButton::~wmSwipeButton()
{

}

void wmSwipeButton::CreateRects()
{
    if(!m_bVertical)
    {
        m_uiRect.SetRect(wxRect(10, 0, 80,GetClientSize().GetHeight()));
    }
    else
    {
        m_uiRect.SetRect(wxRect(0, 10, GetClientSize().GetWidth(),40));
    }
    m_uiRect.SetTextAlign(wxALIGN_CENTER);
}

void wmSwipeButton::Draw(wxDC& dc)
{
    dc.SetPen(*wxTRANSPARENT_PEN);
    dc.SetBrush(wxBrush(GetParent()->GetBackgroundColour()));
    dc.DrawRectangle(GetClientRect());
    dc.SetBrush(wxNullBrush);
    dc.SetPen(wxNullPen);


    dc.SetFont(GetFont());

    wxBitmap bmp;
    if(m_bmp[m_nState].IsOk())
    {
        bmp = m_bmp[m_nState];
    }
    else if(m_bmp[STATE_NORMAL].IsOk())
    {
        bmp = m_bmp[STATE_NORMAL];
    }
    if(IsEnabled() == false)
    {
        m_uiRect.SetBackgroundColour(m_clrBackground[STATE_DISABLED]);
    }
    else if(m_clrBackground[m_nState].IsOk())
    {
        m_uiRect.SetBackgroundColour(m_clrBackground[m_nState]);
    }
    if(m_clrForeground[m_nState].IsOk())
    {
        m_uiRect.SetForegroundColour(m_clrForeground[m_nState]);
    }


    if(!m_bVertical)
    {
        m_uiSwitchLine.SetRect(wxRect(0, m_uiRect.GetCenter().y-3, m_uiRect.GetLeft(),6));
    }
    else
    {
        m_uiSwitchLine.SetRect(wxRect(m_uiRect.GetCenter().x-3, 0, 6,m_uiRect.GetTop()));
    }

    m_uiSwitchLine.SetBackgroundColour(m_uiRect.GetBackgroundColour());
    m_uiSwitchLine.Draw(dc, wxEmptyString, uiRect::BORDER_DOWN);

    if(!m_bVertical)
    {
        m_uiSwitchLine.SetRect(wxRect(m_uiRect.GetRight(), m_uiRect.GetCenter().y-3, GetClientSize().GetWidth()-m_uiRect.GetRight(),6));
    }
    else
    {
        m_uiSwitchLine.SetRect(wxRect(m_uiRect.GetCenter().x-3, m_uiRect.GetBottom(), 6,GetClientSize().GetHeight()-m_uiRect.GetBottom()));
    }

    m_uiSwitchLine.SetBackgroundColour(wxColour(140,140,140));
    m_uiSwitchLine.Draw(dc, wxEmptyString, uiRect::BORDER_DOWN);


    m_uiRect.Draw(dc, m_uiRect.GetLabel(), m_nBorderState, uiRect::EDGE_ALL, bmp);

}

void wmSwipeButton::OnMouseMove(wxMouseEvent& event)
{
    if(event.LeftIsDown() && m_nBorderState == uiRect::BORDER_DOWN)
    {
        if(!m_bVertical)
        {
            m_uiRect.Offset(event.GetPosition().x - m_pntMouse.x,0);
            if(m_uiRect.GetLeft() >= GetClientSize().GetWidth()-40)
            {
                SendEvent(wxEVT_COMMAND_BUTTON_CLICKED);
            }
            else if(m_uiRect.GetLeft() <= 0)
            {
                m_uiRect.SetLeft(10);
            }
            Refresh();
        }
        else
        {
            m_uiRect.Offset(0, event.GetPosition().y - m_pntMouse.y);
            if(m_uiRect.GetTop() >= GetClientSize().GetHeight()-40)
            {
                SendEvent(wxEVT_COMMAND_BUTTON_CLICKED);
                m_uiRect.SetTop(10);
            }
            else if(m_uiRect.GetTop() <= 0)
            {
                m_uiRect.SetTop(10);
            }
            Refresh();
        }
    }
    m_pntMouse = event.GetPosition();
}

void wmSwipeButton::OnLeftDown(wxMouseEvent& event)
{
    if(!HasCapture())
    {
        CaptureMouse();
    }
    m_pntMouse = event.GetPosition();

    if(m_uiRect.Contains(event.GetPosition()))// && (*itButton).bEnabled)
    {
        SendEvent(wxEVT_BUTTON_PRESSED);
        m_nBorderState = uiRect::BORDER_DOWN;

        m_nState = STATE_SELECTED;

        m_clrDown[0] = wxColour(36,227,58);
        m_clrDown[1] = wxColour(16,207,38);

        m_nHoldCount = 0;
        Refresh();
        Update();
    }
}



void wmSwipeButton::OnLeftDClick(wxMouseEvent& event)
{
    OnLeftDown(event);
}

void wmSwipeButton::OnLeftUp(wxMouseEvent& event)
{
    SendEvent(wxEVT_BUTTON_RELEASED);

    if(m_uiRect.Contains(event.GetPosition()))
    {
        m_nBorderState = uiRect::BORDER_UP;
        m_nState = STATE_NORMAL;

     }
    else
    {
        m_nBorderState = uiRect::BORDER_UP;
    }
    if(!m_bVertical)
    {
        m_uiRect.SetLeft(10);
    }
    else
    {
        m_uiRect.SetTop(10);
    }

    if(HasCapture())
    {
        ReleaseMouse();
    }

    Refresh();
}

void wmSwipeButton::OnCaptureLost(wxMouseCaptureLostEvent& event)
{
    m_nBorderState = uiRect::BORDER_UP;
    Refresh();
    if(!m_bVertical)
    {
        m_uiRect.SetLeft(10);
    }
    else
    {
        m_uiRect.SetTop(10);
    }
}

