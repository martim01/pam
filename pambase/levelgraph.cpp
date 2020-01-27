#include "levelgraph.h"
#include <wx/dc.h>
#include <wx/dcbuffer.h>
#include <wx/log.h>
#include "uirect.h"

using namespace std;

BEGIN_EVENT_TABLE(LevelGraph, pmControl)
    EVT_PAINT(LevelGraph::OnPaint)
    EVT_SIZE(LevelGraph::OnSize)
END_EVENT_TABLE()

 wxIMPLEMENT_DYNAMIC_CLASS(LevelGraph, pmControl);



LevelGraph::LevelGraph(wxWindow *parent, wxWindowID id, const wxPoint& pos, const wxSize& size, size_t nDataSize, double dMax, double dMin) : pmControl(),
    m_nDataSize(nDataSize),
    m_dMax(dMax),
    m_dMin(dMin)
{
    Create(parent, id, pos, size);
}

bool LevelGraph::Create(wxWindow *parent, wxWindowID id, const wxPoint& pos, const wxSize& size)
{
    wxSize szInit(size);
    wxSize bestSize = DoGetBestSize();
    if(size.x<=0)
        szInit.SetWidth(bestSize.x);
    if(size.y <= 0)
        szInit.SetHeight(bestSize.y);

    if(!wxWindow::Create(parent,id,pos,szInit,wxWANTS_CHARS, wxT("peak")))
        return false;

    SetMinSize(size);

    SetBackgroundStyle(wxBG_STYLE_CUSTOM);


    return true;
}

LevelGraph::~LevelGraph()
{

}


void LevelGraph::OnPaint(wxPaintEvent& event)
{
    wxAutoBufferedPaintDC dc(this);
    wxBrush br(*wxBLACK);
    dc.SetBrush(br);
    dc.SetPen(*wxTRANSPARENT_PEN);
    dc.DrawRectangle(GetClientRect());


    //draw the zones
    dc.SetPen(*wxTRANSPARENT_PEN);
    for(list<zone>::iterator itZone = m_lstZones.begin(); itZone != m_lstZones.end(); ++itZone)
    {
        double dMin = (*itZone).dMin*m_dResolution;
        double dHeight = (*itZone).dMax*m_dResolution-dMin;

        dc.SetBrush(wxBrush((*itZone).clrZone));
        dc.DrawRectangle(0, dMin, GetClientSize().x, dHeight);
    }

    for(map<double, wxPen>::iterator itLine = m_mLines.begin(); itLine != m_mLines.end(); ++itLine)
    {
        double dY = GetClientRect().GetBottom()-((itLine->first-m_dMin)*m_dResolution);
        dc.SetPen(itLine->second);

        dc.DrawLine(0, dY, GetClientSize().x, dY);
    }


    dc.SetPen(wxPen(GetForegroundColour()));

    dc.SetFont(GetFont());
    uiRect rectMax(wxRect(GetClientRect().GetLeft(), 0, 50, 20));
    uiRect rectMin(wxRect(GetClientRect().GetLeft(), GetClientRect().GetBottom()-20, 50, 20));

    for(map<wxString, graph>::iterator itGraph = m_mGraphs.begin(); itGraph != m_mGraphs.end(); ++itGraph)
    {
        if(itGraph->second.bShow && itGraph->second.lstPeaks.empty() == false)
        {
            dc.SetPen(itGraph->second.clrLine);
            double dY_old(0);

            int x = GetClientRect().GetWidth()-1;
            for(list<double>::reverse_iterator itPeak = itGraph->second.lstPeaks.rbegin(); itPeak != itGraph->second.lstPeaks.rend(); ++itPeak)
            {
                double dPeak = (*itPeak)-itGraph->second.dMin;
                double dTop = max((double)GetClientRect().GetTop(), GetClientRect().GetBottom()-(itGraph->second.dResolution*dPeak));
                dc.DrawLine(x+1, dY_old, x, dTop);
                dY_old=dTop;
                x--;
            }
            if(itGraph->second.bShowRange)
            {
                rectMax.Draw(dc, wxString::Format(wxT("%.2f"), itGraph->second.dMax), uiRect::BORDER_NONE);
                rectMin.Draw(dc, wxString::Format(wxT("%.2f"), itGraph->second.dMin), uiRect::BORDER_NONE);
            }
        }
    }







    dc.SetPen(wxPen(wxColour(200,200,200)));
    dc.DrawLine(0,0, GetClientRect().GetRight(), 0);
    dc.DrawLine(0,GetClientRect().GetBottom(), GetClientRect().GetWidth(), GetClientRect().GetBottom());

    dc.SetPen(wxPen(wxColour(120,120,120), 1));
    dc.DrawLine(0,0, 0, GetClientRect().GetBottom());
    dc.DrawLine(GetClientRect().GetRight(),0, GetClientRect().GetRight(), GetClientRect().GetBottom());
}

void LevelGraph::AddGraph(const wxString& sName, const wxColour& clr)
{
    m_mGraphs.insert(make_pair(sName, graph(clr)));
}

void LevelGraph::AddPeak(const wxString& sGraph, double dPeak)
{
    map<wxString, graph>::iterator itGraph = m_mGraphs.find(sGraph);
    if(itGraph != m_mGraphs.end())
    {
        itGraph->second.dDataSetTotal += dPeak;
        itGraph->second.dDataSetMax = max(dPeak,itGraph->second.dDataSetMax);
        itGraph->second.nDataSize++;

        if(itGraph->second.nDataSize == m_nDataSize)
        {
            ProcessDataSet(itGraph->second);
        }
    }
}

void LevelGraph::ProcessDataSet(graph& aGraph)
{
    //currently we just show the max
    double dPeak(GetDataSetMax(aGraph));
    aGraph.nDataSize = 0;
    aGraph.dDataSetMax = -120.0;
    aGraph.dDataSetTotal = 0.0;

    aGraph.lstPeaks.push_back(dPeak);
    if(aGraph.lstPeaks.size() > GetClientRect().GetWidth())
    {
        aGraph.lstPeaks.pop_front();
    }

    Refresh();

}
double LevelGraph::GetDataSetMax(graph& aGraph)
{
    return aGraph.dDataSetMax;

}


void LevelGraph::OnSize(wxSizeEvent& event)
{
    m_dResolution = static_cast<double>(GetClientSize().y)/(m_dMax-m_dMin);
}

void LevelGraph::ClearGraphs()
{
    for(map<wxString, graph>::iterator itGraph = m_mGraphs.begin(); itGraph != m_mGraphs.end(); ++itGraph)
    {
        itGraph->second.lstPeaks.clear();
        itGraph->second.dDataSetTotal =0.0;
        itGraph->second.dDataSetMax= -120.0;
        itGraph->second.nDataSize=0;

        itGraph->second.dMax = 1.0;
        itGraph->second.dMin = 0.0;
        itGraph->second.dResolution = static_cast<double>(GetClientSize().y);
    }
    Refresh();
}

void LevelGraph::DeleteAllGraphs()
{
    m_mGraphs.clear();
    Refresh();
}

void LevelGraph::HideAllGraphs()
{
    for(map<wxString, graph>::iterator itGraph = m_mGraphs.begin(); itGraph != m_mGraphs.end(); ++itGraph)
    {
        itGraph->second.bShow = false;
    }
}


void LevelGraph::ShowGraph(const wxString& sGraph, bool bShow)
{
    map<wxString, graph>::iterator itGraph = m_mGraphs.find(sGraph);
    if(itGraph != m_mGraphs.end())
    {
        itGraph->second.bShow = bShow;
    }
    Refresh();
}

void LevelGraph::ShowRange(const wxString& sGraph, bool bShow)
{
    map<wxString, graph>::iterator itGraph = m_mGraphs.find(sGraph);
    if(itGraph != m_mGraphs.end())
    {
        itGraph->second.bShowRange = bShow;
    }
    Refresh();
}

void LevelGraph::AddZone(double dMin, double dMax, const wxColour& clr)
{
    m_lstZones.push_back(zone(dMin, dMax, RGBA_Blend(clr, wxColour(0,0,0), 0.15)));
}

void LevelGraph::ClearZones()
{
    m_lstZones.clear();
}


wxColour LevelGraph::RGBA_Blend(wxColour clrFore, wxColour clrBack, double dAlpha)
{
    double dNotAlpha = (1.0-dAlpha);
    int ored = (dNotAlpha * static_cast<double>(clrBack.Red())) + (dAlpha * static_cast<double>(clrFore.Red()));
    int ogreen= (dNotAlpha * static_cast<double>(clrBack.Green())) + (dAlpha * static_cast<double>(clrFore.Green()));
    int oblue= (dNotAlpha * static_cast<double>(clrBack.Blue())) + (dAlpha * static_cast<double>(clrFore.Blue()));
    return wxColour(ored, ogreen, oblue);
}


void LevelGraph::SetLimit(const wxString& sGraph, double dMax, double dMin)
{
    map<wxString, graph>::iterator itGraph = m_mGraphs.find(sGraph);
    if(itGraph != m_mGraphs.end())
    {
        itGraph->second.dMax = dMax;
        itGraph->second.dMin = dMin;
        itGraph->second.dResolution = static_cast<double>(GetClientSize().y)/(dMax-dMin);
        Refresh();
    }
}

void LevelGraph::AddLine(double dPosition, const wxPen& penStyle)
{
    m_mLines[dPosition] = penStyle;

}

void LevelGraph::ClearLines()
{
    m_mLines.clear();
}


pair<double,double> LevelGraph::GetRange(const wxString& sGraph)
{
    map<wxString, graph>::iterator itGraph = m_mGraphs.find(sGraph);
    if(itGraph != m_mGraphs.end())
    {
        return make_pair(itGraph->second.dMin, itGraph->second.dMax);
    }
    return make_pair(0.0,0.0);
}
