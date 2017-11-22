#include "maxmingraph.h"
#include <wx/dc.h>
#include <wx/dcbuffer.h>
#include <wx/log.h>
#include <wx/image.h>
#include "uirect.h"
#include "testlog.h"

using namespace std;

BEGIN_EVENT_TABLE(MaxMinGraph, wxWindow)
    EVT_PAINT(MaxMinGraph::OnPaint)
    EVT_SIZE(MaxMinGraph::OnSize)
END_EVENT_TABLE()



MaxMinGraph::MaxMinGraph(wxWindow *parent, wxWindowID id, const wxPoint& pos, const wxSize& size)
{
    Create(parent, id, pos, size);
}

bool MaxMinGraph::Create(wxWindow *parent, wxWindowID id, const wxPoint& pos, const wxSize& size)
{
    wxSize szInit(size);
    wxSize bestSize(200,50);
    if(size.x<=0)
        szInit.SetWidth(bestSize.x);
    if(size.y <= 0)
        szInit.SetHeight(bestSize.y);

    if(!wxWindow::Create(parent,id,pos,szInit,wxWANTS_CHARS, wxT("MaxMinGraph")))
        return false;

    SetMinSize(size);

    SetBackgroundStyle(wxBG_STYLE_CUSTOM);

    #ifndef __WXMSW__
    SetCursor(wxCURSOR_BLANK);
    #endif // __WXMSW__

    return true;
}

MaxMinGraph::~MaxMinGraph()
{

}


void MaxMinGraph::OnPaint(wxPaintEvent& event)
{
    //wxAutoBufferedPaintDC dc(this);
    wxAutoBufferedPaintDC dc(this);


    wxBrush br(*wxBLACK);
    dc.SetBrush(br);
    dc.SetPen(*wxWHITE_PEN);
    dc.DrawRectangle(GetClientRect());

    if(m_dMin < m_dMax)
    {
        dc.SetPen(*wxTRANSPARENT_PEN);
        dc.SetBrush(wxBrush(wxColour(220,180,180)));
        dc.DrawRectangle(wxRect(1+m_dMin*m_dResolution, 1, (m_dMax-m_dMin)*m_dResolution, GetClientRect().GetHeight()-2));

        dc.SetPen(*wxBLUE);
        dc.DrawLine(1+m_dCurrent*m_dResolution, 1, 1+m_dCurrent*m_dResolution, GetClientRect().GetBottom()-2);
    }

}

void MaxMinGraph::OnSize(wxSizeEvent& event)
{
    m_dResolution = static_cast<double>(GetClientSize().x-2);
    Refresh();
}


void MaxMinGraph::SetLevels(double dMax, double dMin, double dCurrent)
{
    m_dMax = dMax;
    m_dMin = dMin;
    m_dCurrent = dCurrent;

    Refresh();
}

