#include "wxtouchscreenhtml.h"
#include <wx/log.h>
#include <wx/dcclient.h>
#include <wx/dcmemory.h>
#include <wx/dcbuffer.h>

wxTouchScreenHtml::wxTouchScreenHtml() : wxHtmlWindow()
{

}

wxTouchScreenHtml::wxTouchScreenHtml(wxWindow *parent, wxWindowID id, const wxPoint& pos, const wxSize& size, long style, const wxString& name) : wxHtmlWindow(parent, id, pos, size, style, name)
{
    m_bScrollLock = false;
    m_bDown = false;
    SetScrollbars(1,1,1200,1200);
    Connect(wxEVT_LEFT_DOWN, (wxObjectEventFunction)&wxTouchScreenHtml::OnMouseDown);
    Connect(wxEVT_LEFT_UP, (wxObjectEventFunction)&wxTouchScreenHtml::OnMouseUp);
    Connect(wxEVT_MOTION, (wxObjectEventFunction)&wxTouchScreenHtml::OnMouseMove);
    Connect(wxEVT_PAINT, (wxObjectEventFunction)&wxTouchScreenHtml::OnPaint);
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
    event.Skip();
}

void wxTouchScreenHtml::OnMouseMove(wxMouseEvent& event)
{
    if(m_bDown)
    {
        wxClientDC dc(this);
        int xStart,yStart;
        GetViewStart(&xStart, &yStart);

        int x = event.GetPosition().x - m_pntDown.x;
        int y = event.GetPosition().y - m_pntDown.y;


        int xUnit, yUnit;
        GetScrollPixelsPerUnit(&xUnit, &yUnit);
        x/=xUnit;
        y/=yUnit;
        if(y != 0)
        {
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
    wxAutoBufferedPaintDC dc(this);

    if (m_Cell == NULL)
        return;

    int x, y;
    GetViewStart(&x, &y);
    const wxRect rect = GetUpdateRegion().GetBox();
    const wxSize sz = GetClientSize();

    PrepareDC(dc);

    dc.SetBackground(GetBackgroundColour());
    dc.Clear();


    // draw the HTML window contents
    dc.SetMapMode(wxMM_TEXT);
    dc.SetBackgroundMode(wxTRANSPARENT);
    dc.SetLayoutDirection(GetLayoutDirection());

    wxHtmlRenderingInfo rinfo;
    wxDefaultHtmlRenderingStyle rstyle;
    rinfo.SetSelection(m_selection);
    rinfo.SetStyle(&rstyle);
    m_Cell->Draw(dc, 0, 0,
                 y  + rect.GetTop(),
                 y  + rect.GetBottom(),
                 rinfo);

}


void wxTouchScreenHtml::SetPageTouch(const wxString& sCode)
{
    int xView,yView;
    GetViewStart(&xView,&yView);
    SetPage(sCode);
    int x,y;
    GetVirtualSize(&x,&y);
    SetScrollbars(1,1, x,y);
    if(m_bScrollLock)
    {
        Scroll(xView,yView);
    }
    else
    {
        Scroll(0,y-GetClientSize().y);
    }
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
    Scroll(0,std::max(0,y-GetClientSize().y));
}
void wxTouchScreenHtml::PageDown()
{
    int x,y;
    GetViewStart(&x,&y);

    int yBottom;
    GetVirtualSize(&x,&yBottom);
    yBottom-=GetClientSize().y;

    Scroll(0, std::min(yBottom, y+GetClientSize().y));
}
