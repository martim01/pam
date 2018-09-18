#include "maxmingraph.h"
#include <wx/dc.h>
#include <wx/dcbuffer.h>
#include <wx/log.h>
#include <wx/image.h>
#include "uirect.h"
#include "settings.h"
#include "levelsbuilder.h"
#include "wmlogevent.h"
//#include "testlog.h"

using namespace std;

BEGIN_EVENT_TABLE(MaxMinGraph, wxWindow)
    EVT_PAINT(MaxMinGraph::OnPaint)
    EVT_SIZE(MaxMinGraph::OnSize)
END_EVENT_TABLE()



MaxMinGraph::MaxMinGraph(wxWindow *parent,LevelsBuilder* pBuilder, wxWindowID id, const wxPoint& pos, const wxSize& size) :
    m_pBuilder(pBuilder),
    m_dMaxRange(80.0),
    m_dLastLevel(-80.0)
{
    Create(parent, id, pos, size);
}

bool MaxMinGraph::Create(wxWindow *parent, wxWindowID id, const wxPoint& pos, const wxSize& size)
{
    wxSize szInit(size);
    wxSize bestSize(50,200);
    if(size.x<=0)
        szInit.SetWidth(bestSize.x);
    if(size.y <= 0)
        szInit.SetHeight(bestSize.y);

    if(!wxWindow::Create(parent,id,pos,szInit,wxWANTS_CHARS, wxT("MaxMinGraph")))
        return false;

    SetMinSize(size);

    SetBackgroundStyle(wxBG_STYLE_CUSTOM);

    m_uiCurrent.SetBackgroundColour(wxColour(91,91,0));
    m_uiMax.SetBackgroundColour(wxColour(91,91,0));
    m_uiMin.SetBackgroundColour(wxColour(91,91,0));
    m_uiRange.SetBackgroundColour(wxColour(91,91,0));

    m_uiCurrent.SetForegroundColour(*wxWHITE);
    m_uiMax.SetForegroundColour(*wxWHITE);
    m_uiMin.SetForegroundColour(*wxWHITE);
    m_uiRange.SetForegroundColour(*wxWHITE);

    m_uiCurrent.SetGradient(0);
    m_uiMax.SetGradient(0);
    m_uiMin.SetGradient(0);
    m_uiRange.SetGradient(0);

    SetSize(size);
    SetPosition(pos);

    if(Settings::Get().HideCursor())
    {
        SetCursor(wxCURSOR_BLANK);
    }

    SetFont(wxFont(8,wxFONTFAMILY_SWISS,wxFONTSTYLE_NORMAL,wxFONTWEIGHT_NORMAL,false,_T("Arial"),wxFONTENCODING_DEFAULT));
    return true;
}

MaxMinGraph::~MaxMinGraph()
{

}


void MaxMinGraph::OnPaint(wxPaintEvent& event)
{
    //wxAutoBufferedPaintDC dc(this);
    wxAutoBufferedPaintDC dc(this);

    dc.SetFont(GetFont());

    wxBrush br(*wxBLACK);
    dc.SetBrush(br);
    dc.SetPen(*wxTRANSPARENT_PEN);
    dc.DrawRectangle(GetClientRect());
    dc.SetPen(*wxWHITE_PEN);
    dc.DrawRectangle(m_rectGraph);

    if(m_dMin < m_dMax)
    {
        double dMax(70.0+m_dMax);
        double dMin(70.0+m_dMin);
        double dCurrent(70.0+m_dCurrent);

        dc.SetPen(*wxTRANSPARENT_PEN);
        dc.SetBrush(wxBrush(wxColour(220,180,180)));
        dc.DrawRectangle(wxRect(1, m_rectGraph.GetBottom()-1-(dMax*m_dResolution), m_rectGraph.GetWidth()-2, (dMax-dMin)*m_dResolution));

        dc.SetPen(*wxBLUE);
        dc.DrawLine(1, m_rectGraph.GetBottom()-1-dCurrent*m_dResolution, m_rectGraph.GetWidth()-2, m_rectGraph.GetBottom()-1-dCurrent*m_dResolution);
    }

    m_uiCurrent.Draw(dc, uiRect::BORDER_DOWN);
    m_uiMax.Draw(dc, uiRect::BORDER_DOWN);
    m_uiMin.Draw(dc, uiRect::BORDER_DOWN);
    m_uiRange.Draw(dc, uiRect::BORDER_DOWN);
}

void MaxMinGraph::OnSize(wxSizeEvent& event)
{
    m_rectGraph = GetClientRect();
    m_rectGraph.SetBottom(m_rectGraph.GetBottom()-85);

    m_uiCurrent.SetRect(wxRect(m_rectGraph.GetLeft(), m_rectGraph.GetBottom()+2, m_rectGraph.GetWidth(), 20));
    m_uiMax.SetRect(wxRect(m_uiCurrent.GetLeft(), m_uiCurrent.GetBottom()+2, m_uiCurrent.GetWidth(), 20));
    m_uiMin.SetRect(wxRect(m_uiMax.GetLeft(), m_uiMax.GetBottom()+2, m_uiMax.GetWidth(), 20));
    m_uiRange.SetRect(wxRect(m_uiMin.GetLeft(), m_uiMin.GetBottom()+2, m_uiMin.GetWidth(), 20));

    m_dResolution = static_cast<double>(m_rectGraph.GetHeight()-2)/70.0;
    Refresh();
}


void MaxMinGraph::SetLevels(double dMax, double dMin, double dCurrent, bool bConvertToDb)
{
    if(bConvertToDb)
    {
        m_dMax = max(-70.0, 20*log10(dMax));
        m_dMin = max(-70.0, 20*log10(dMin));
        m_dCurrent = 20*log10(dCurrent);
    }
    else
    {
        m_dMax = max(dMax, dMin);
        m_dMin = min(dMin, dMax);
        m_dCurrent = dCurrent;
    }

    double dRange(m_dMax-m_dMin);

    m_uiCurrent.SetLabel(wxString::Format(wxT("%.1fdB"), m_dCurrent));
    m_uiMax.SetLabel(wxString::Format(wxT("%.1fdB"), m_dMax));
    m_uiMin.SetLabel(wxString::Format(wxT("%.1fdB"),m_dMin));
    m_uiRange.SetLabel(wxString::Format(wxT("%.1fdB"), dRange));


    if(m_dCurrent != m_dLastLevel && m_pBuilder->ReadSetting(wxT("Monitor"),0) == 0)
    {
        if(m_pBuilder->IsLogActive())
        {
            wmLog::Get()->Log(wxString::Format(wxT("**TESTS** Levels - level changed from %.2fdB to %.2fdB"), m_dLastLevel, m_dCurrent));
        }
        m_uiCurrent.SetBackgroundColour(wxColour(255,100,100));
    }
    else
    {
        m_uiCurrent.SetBackgroundColour(wxColour(91,91,0));
    }

    if(dRange > m_dMaxRange && m_pBuilder->ReadSetting(wxT("Monitor"),0) == 1)
    {
        if(m_pBuilder->IsLogActive())
        {
            wmLog::Get()->Log(wxString::Format(wxT("**TESTS** Levels - range %.2fdB > max set %.2fdB"), dRange, m_dMaxRange));
        }
        m_uiRange.SetBackgroundColour(wxColour(255,100,100));
    }
    else
    {
        m_uiRange.SetBackgroundColour(wxColour(91,91,0));
    }

    m_dLastLevel = m_dCurrent;
    Refresh();
}


void MaxMinGraph::SetMaxRange(double dRange)
{
    m_dMaxRange = dRange;
}
