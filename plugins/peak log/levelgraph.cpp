#include "levelgraph.h"
#include <wx/dc.h>
#include <wx/dcbuffer.h>
#include <wx/log.h>


using namespace std;

BEGIN_EVENT_TABLE(LevelGraph, wxWindow)
    EVT_PAINT(LevelGraph::OnPaint)
    EVT_SIZE(LevelGraph::OnSize)
END_EVENT_TABLE()



LevelGraph::LevelGraph(wxWindow *parent, wxWindowID id, const wxPoint& pos, const wxSize& size, size_t nDataSize, double dLimit) :
    m_nDataSize(nDataSize),
    m_dLimit(dLimit)
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

    double dPPM0 = m_dResolution*-34.0;
    for(int i = 0; i < 9; i++)
    {
        if(i != 4)
        {
            dc.SetPen(wxPen(wxColour(100,100,100), 1, wxDOT));
        }
        else
        {
            dc.SetPen(wxPen(wxColour(100,100,160), 1, wxDOT));
        }
        double dY = static_cast<double>(i*4)*m_dResolution;
        dc.DrawLine(0, dY+dPPM0, GetClientSize().x, dY+dPPM0);
    }


    dc.SetPen(wxPen(GetForegroundColour()));


    for(map<wxString, graph>::iterator itGraph = m_mGraphs.begin(); itGraph != m_mGraphs.end(); ++itGraph)
    {
        if(itGraph->second.bShow)
        {
            dc.SetPen(itGraph->second.clrLine);
            double dY_old(0);

            int x = GetClientRect().GetWidth()-1;
            list<double>::iterator itPeak = itGraph->second.lstPeaks.end();
            while(itPeak != itGraph->second.lstPeaks.begin())
            {
                --itPeak;

                dc.DrawLine(x+1, dY_old, x, (m_dResolution*(*itPeak)));
                dY_old=(m_dResolution*(*itPeak));
                x--;
            }
        }
    }





    dc.SetPen(wxPen(wxColour(200,200,200)));
    dc.DrawLine(0,0, GetClientRect().GetRight(), 0);
    dc.DrawLine(0,GetClientRect().GetBottom(), GetClientRect().GetWidth(), GetClientRect().GetBottom());

    dc.SetPen(wxPen(wxColour(200,200,200), 1, wxDOT));
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
    m_dResolution = static_cast<double>(GetClientSize().y)/m_dLimit;
}

void LevelGraph::ClearGraphs()
{
    for(map<wxString, graph>::iterator itGraph = m_mGraphs.begin(); itGraph != m_mGraphs.end(); ++itGraph)
    {
        itGraph->second.lstPeaks.clear();
        itGraph->second.dDataSetTotal =0.0;
        itGraph->second.dDataSetMax= -120.0;
        itGraph->second.nDataSize=0;

    }
    Refresh();
}

void LevelGraph::DeleteAllGraphs()
{
    m_mGraphs.clear();
    Refresh();
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
