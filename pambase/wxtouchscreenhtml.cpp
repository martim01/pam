#include "wxtouchscreenhtml.h"
#include <wx/log.h>
#include <wx/dcclient.h>
#include <wx/dcmemory.h>
#include <wx/dcbuffer.h>

wxTouchScreenHtml::wxTouchScreenHtml() : wxHtmlWindow(),
	m_bScrollLock(false),
	m_bDown(false),
	m_nScrollOffset(0)
{

}

wxTouchScreenHtml::wxTouchScreenHtml(wxWindow *parent, wxWindowID id, const wxPoint& pos, const wxSize& size, long style, const wxString& name) : wxHtmlWindow(parent, id, pos, size, style, name)
{
    m_bScrollLock = false;
    m_bDown = false;
    SetScrollbars(1,1,1200,1200);
    m_timerScroll.SetOwner(this, wxNewId());

    Connect(wxEVT_LEFT_DOWN, (wxObjectEventFunction)&wxTouchScreenHtml::OnMouseDown);
    Connect(wxEVT_LEFT_UP, (wxObjectEventFunction)&wxTouchScreenHtml::OnMouseUp);
    Connect(wxEVT_MOTION, (wxObjectEventFunction)&wxTouchScreenHtml::OnMouseMove);
    Connect(wxEVT_PAINT, (wxObjectEventFunction)&wxTouchScreenHtml::OnPaint);
    Connect(m_timerScroll.GetId(), wxEVT_TIMER, (wxObjectEventFunction)&wxTouchScreenHtml::OntimerScroll);
}

void wxTouchScreenHtml::SetScrollLock(bool bLock)
{
    m_bScrollLock = bLock;
}

void wxTouchScreenHtml::OnMouseDown(wxMouseEvent& event)
{
    m_bDown = true;
    m_pntDown = event.GetPosition();
    event.Skip();
}

void wxTouchScreenHtml::OnMouseUp(wxMouseEvent& event)
{
    m_bDown = false;
    if(m_nScrollOffset != 0)
    {
        m_timerScroll.Start(50);
    }
    event.Skip();
}

void wxTouchScreenHtml::OnMouseMove(wxMouseEvent& event)
{
    if(m_bDown)
    {
        int x = event.GetPosition().x - m_pntDown.x;
        int y = event.GetPosition().y - m_pntDown.y;

        int xUnit, yUnit;
        GetScrollPixelsPerUnit(&xUnit, &yUnit);
        x/=xUnit;
        y/=yUnit;

        m_nScrollOffset =  event.GetPosition().y-m_pntDown.y;

        if(y != 0)
        {
            int xStart,yStart;
            GetViewStart(&xStart, &yStart);
            Scroll(xStart-x,yStart-y);
            m_pntDown = event.GetPosition();
            Refresh();
            Update();
        }


    }
    else
    {
        event.Skip();
    }
}


void wxTouchScreenHtml::OnPaint(wxPaintEvent& event)
{
    wxHtmlWindow::OnPaint(event);

}



void wxTouchScreenHtml::LoadFileTouch(const wxString& sFile)
{
    int xView,yView;
    GetViewStart(&xView,&yView);
    LoadFile(sFile);
    int x,y;
    GetVirtualSize(&x,&y);
    SetScrollbars(1,1, x,y);
}

void wxTouchScreenHtml::Home()
{
    Scroll(0,0);
}

void wxTouchScreenHtml::End()
{
    int x,y;
    GetVirtualSize(&x,&y);
    Scroll(0,y-GetClientSize().y);
}

void wxTouchScreenHtml::PageUp()
{
    int x,y;
    GetViewStart(&x,&y);
    int xUnit, yUnit;
    GetScrollPixelsPerUnit(&xUnit, &yUnit);
    x/=xUnit;
    y/=yUnit;

    Scroll(0,std::max(0,y-GetClientSize().y));
}
void wxTouchScreenHtml::PageDown()
{
    int x,y;
    GetViewStart(&x,&y);

    int xUnit, yUnit;
    GetScrollPixelsPerUnit(&xUnit, &yUnit);
    x/=xUnit;
    y/=yUnit;

    int yBottom;
    GetVirtualSize(&x,&yBottom);
    yBottom-=GetClientSize().y;

    Scroll(0, std::min(yBottom, y+GetClientSize().y));
}


void wxTouchScreenHtml::OntimerScroll(wxTimerEvent& event)
{
    int xUnit, yUnit;
    GetScrollPixelsPerUnit(&xUnit, &yUnit);

    int y = m_nScrollOffset/yUnit;

    if(y != 0)
    {
        int xStart,yStart;
        GetViewStart(&xStart, &yStart);
        Scroll(xStart,yStart-y);
        Refresh();
        Update();


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
