#include "historygraph.h"
#include <wx/dc.h>
#include <wx/dcbuffer.h>
#include <wx/log.h>
#include "uirect.h"
#include <iostream>
#include "log.h"
#include <algorithm>
using namespace std;

BEGIN_EVENT_TABLE(HistoryGraph, pmControl)
    EVT_PAINT(HistoryGraph::OnPaint)
    EVT_SIZE(HistoryGraph::OnSize)
END_EVENT_TABLE()

 wxIMPLEMENT_DYNAMIC_CLASS(HistoryGraph, pmControl);



HistoryGraph::HistoryGraph(wxWindow *parent, wxWindowID id, const wxPoint& pos, const wxSize& size) : pmControl() , m_itMaster(m_mGraphs.end())
{
    Create(parent, id, pos, size);
        m_bBarChart = true;
    m_rectGraph = wxRect(GetClientRect().GetLeft()+5, GetClientRect().GetTop()+5, GetClientRect().GetWidth()-45, GetClientRect().GetBottom()-25);
}

void HistoryGraph::SetRightAxisWidth(unsigned int nWidth)
{
    m_rectGraph = wxRect(GetClientRect().GetLeft()+5, GetClientRect().GetTop()+5, GetClientRect().GetWidth()-(nWidth+5), GetClientRect().GetBottom()-25);
}

bool HistoryGraph::Create(wxWindow *parent, wxWindowID id, const wxPoint& pos, const wxSize& size)
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

void HistoryGraph::ShowBarGraph(bool bShow)
{
    m_bBarChart = bShow;
    Refresh();
}

HistoryGraph::~HistoryGraph()
{

}


void HistoryGraph::OnPaint(wxPaintEvent& event)
{
    wxAutoBufferedPaintDC dc(this);
    wxBrush br(*wxBLACK);
    dc.SetBrush(br);
    dc.SetPen(*wxTRANSPARENT_PEN);
    dc.DrawRectangle(GetClientRect());

    //draw the axis
    //draiw the graphs
    bool bFirst(true);
    for(auto pairGraph : m_mGraphs)
    {
        if(pairGraph.second.bShow && pairGraph.second.lstPeaks.empty() == false)
        {
            if(bFirst)
            {
                DrawAxis(dc, pairGraph.second);
                bFirst = false;
            }
            if(m_bBarChart)
            {
                DrawBarChart(dc, pairGraph.second);
            }
            else
            {
                DrawLineGraph(dc, pairGraph.second);
            }
        }
    }

}
void HistoryGraph::DrawLineGraph(wxDC& dc, const graph& aGraph)
{
    dc.SetPen(aGraph.clrLine);
    double dY_old(0);

    int x = m_rectGraph.GetRight();
    int xOld = m_rectGraph.GetRight();
    auto now  = std::chrono::system_clock::now();


    double dMin =           m_itMaster != m_mGraphs.end() ? m_itMaster->second.dMin : aGraph.dMin;
    double dResolution =    m_itMaster != m_mGraphs.end() ? m_itMaster->second.dResolution : aGraph.dResolution;
    unsigned int nPixels =  m_itMaster != m_mGraphs.end() ? m_itMaster->second.nPixels : aGraph.nPixels;
    unsigned int nInterval= m_itMaster != m_mGraphs.end() ? m_itMaster->second.nInterval : aGraph.nInterval;

    for(list<graphPoint>::const_reverse_iterator itPeak = aGraph.lstPeaks.rbegin(); itPeak != aGraph.lstPeaks.rend(); ++itPeak)
    {
        double dPeak = (*itPeak).first;
        dPeak-= dMin;

        double dTop = max((double)m_rectGraph.GetTop(), m_rectGraph.GetBottom()-(dResolution*dPeak));

        auto diff = std::chrono::duration_cast<std::chrono::microseconds>(now-(*itPeak).second);
        int64_t nMicroseconds = diff.count();
        nMicroseconds *= nPixels;
        nMicroseconds /= nInterval;

        x = m_rectGraph.GetRight()-(nMicroseconds);

        if(itPeak == aGraph.lstPeaks.rbegin())
        {
            dY_old = dTop;
        }
        dc.SetBrush(wxBrush(wxColour(aGraph.clrLine.Red()/2,aGraph.clrLine.Green()/2,aGraph.clrLine.Blue()/2)));

        dc.SetPen(*wxTRANSPARENT_PEN);
        dc.DrawCircle(wxPoint(x,dTop), 2);
        dc.SetPen(aGraph.clrLine);
        if(aGraph.bConnect)
        {
            dc.DrawLine(x, dTop, xOld, dY_old);
        }

        dY_old=dTop;
        xOld = x;
    }
}

void HistoryGraph::DrawBarChart(wxDC& dc, const graph& aGraph)
{
    double dMin =           m_itMaster != m_mGraphs.end() ? m_itMaster->second.dMin : aGraph.dMin;
    double dResolution =    m_itMaster != m_mGraphs.end() ? m_itMaster->second.dResolution : aGraph.dResolution;
    unsigned int nPixels =  m_itMaster != m_mGraphs.end() ? m_itMaster->second.nPixels : aGraph.nPixels;
    unsigned int nInterval= m_itMaster != m_mGraphs.end() ? m_itMaster->second.nInterval : aGraph.nInterval;

    dc.SetBrush(wxBrush(aGraph.clrLine));
    dc.SetPen(wxPen(wxColour(max(aGraph.clrLine.Red()-60, 0),
                                 max(aGraph.clrLine.Green()-60, 0),
                                 max(aGraph.clrLine.Blue()-60, 0))));

    auto now  = std::chrono::system_clock::now();
    int nOld = m_rectGraph.GetRight();
    for(list<graphPoint>::const_reverse_iterator itPeak = aGraph.lstPeaks.rbegin(); itPeak != aGraph.lstPeaks.rend(); ++itPeak)
    {
        double dPeak = (*itPeak).first;
        dPeak-= (dMin);

        double dTop = max((double)m_rectGraph.GetTop(), m_rectGraph.GetBottom()-(dResolution*dPeak));

        auto diff = std::chrono::duration_cast<std::chrono::microseconds>(now-(*itPeak).second);
        int64_t nMicroseconds = diff.count();
        nMicroseconds *= nPixels;
        nMicroseconds /= nInterval;

        int x = m_rectGraph.GetRight()-(nMicroseconds);

        dc.DrawRectangle(x, static_cast<int>(dTop), nOld-x, m_rectGraph.GetBottom()-static_cast<int>(dTop));

        nOld = x;
    }
}


void HistoryGraph::DrawAxis(wxDC& dc, const graph& aGraph)
{
    double dMax =           m_itMaster != m_mGraphs.end() ? m_itMaster->second.dMax : aGraph.dMax;
    double dMin =           m_itMaster != m_mGraphs.end() ? m_itMaster->second.dMin : aGraph.dMin;
    double dResolution =    m_itMaster != m_mGraphs.end() ? m_itMaster->second.dResolution : aGraph.dResolution;
    unsigned int nPixels =  m_itMaster != m_mGraphs.end() ? m_itMaster->second.nPixels : aGraph.nPixels;
    unsigned int nInterval= m_itMaster != m_mGraphs.end() ? m_itMaster->second.nInterval : aGraph.nInterval;
    unsigned int nIntervalDefault= m_itMaster != m_mGraphs.end() ? m_itMaster->second.nIntervalDefault : aGraph.nIntervalDefault;
    wxString sUnits = m_itMaster != m_mGraphs.end() ? m_itMaster->second.sUnits : aGraph.sUnits;

    dc.SetTextForeground(*wxWHITE);
    //we draw the axis and range for the first graph and not the others....
    dc.SetPen(wxPen(GetForegroundColour()));
    dc.DrawLine(m_rectGraph.GetBottomLeft(), m_rectGraph.GetBottomRight());
    dc.DrawLine(m_rectGraph.GetBottomRight(), m_rectGraph.GetTopRight());

    double dRange = dMax-dMin;
    wxString sMax(wxString::Format("%g%s", dMax, sUnits.c_str()));
    wxSize szText = dc.GetTextExtent(sMax);
    dc.DrawText(sMax, m_rectGraph.GetRight()+2, m_rectGraph.GetTop()-szText.y/2);
    dc.DrawLine(m_rectGraph.GetLeft(), m_rectGraph.GetTop(), m_rectGraph.GetRight()+2, m_rectGraph.GetTop());

    wxString sMin(wxString::Format("%g%s", dMin, sUnits.c_str()));
    szText = dc.GetTextExtent(sMin);
    dc.DrawText(sMin, m_rectGraph.GetRight()+2, m_rectGraph.GetBottom()-szText.y/2);


    wxString sUpper(wxString::Format("%g%s", dMax-dRange/4, sUnits.c_str()));
    szText = dc.GetTextExtent(sUpper);
    dc.DrawText(sUpper, m_rectGraph.GetRight()+2, m_rectGraph.GetTop()+m_rectGraph.GetHeight()/4-szText.y/2);
    dc.DrawLine(m_rectGraph.GetLeft(), m_rectGraph.GetTop()+m_rectGraph.GetHeight()/4, m_rectGraph.GetRight()+2, m_rectGraph.GetTop()+m_rectGraph.GetHeight()/4);

    wxString sLower(wxString::Format("%g%s", dMin+dRange/4, sUnits.c_str()));
    szText = dc.GetTextExtent(sLower);
    dc.DrawText(sLower, m_rectGraph.GetRight()+2, m_rectGraph.GetBottom()-m_rectGraph.GetHeight()/4-szText.y/2);
    dc.DrawLine(m_rectGraph.GetLeft(), m_rectGraph.GetBottom()-m_rectGraph.GetHeight()/4, m_rectGraph.GetRight()+2, m_rectGraph.GetBottom()-m_rectGraph.GetHeight()/4);

    wxString sMid(wxString::Format("%g%s", dMin+dRange/2, sUnits.c_str()));
    szText = dc.GetTextExtent(sMid);
    dc.DrawText(sMid, m_rectGraph.GetRight()+2, m_rectGraph.GetTop()+m_rectGraph.GetHeight()/2-szText.y/2);
    dc.DrawLine(m_rectGraph.GetLeft(), m_rectGraph.GetTop()+m_rectGraph.GetHeight()/2, m_rectGraph.GetRight()+2, m_rectGraph.GetTop()+m_rectGraph.GetHeight()/2);

    for(int x = m_rectGraph.GetRight(); x >= m_rectGraph.GetLeft(); x-= 40)
    {
        int nDiff = (m_rectGraph.GetRight()-x);
        nDiff /=nPixels;

        wxString sTime;
        if(nIntervalDefault >= 1e2)
        {
            sTime.Printf("-%gs", static_cast<double>(nIntervalDefault/1e6)*nDiff);
        }
        else if(nIntervalDefault >= 1e5)
        {
            sTime.Printf("-%gms", static_cast<double>(nIntervalDefault/1e3)*nDiff);
        }
        else
        {
            sTime.Printf("-%uus", nIntervalDefault*nDiff);
        }
        szText = dc.GetTextExtent(sTime);

        dc.DrawText(sTime, x-szText.GetWidth()/2, m_rectGraph.GetBottom()+3);
        //dc.SetPen(wxPen(GetForegroundColour()));
        dc.DrawLine(x, m_rectGraph.GetBottom(), x, m_rectGraph.GetBottom()+3);
    }
}


void HistoryGraph::AddGraph(const wxString& sName, const wxColour& clr, unsigned int nIntervalMicroSeconds, bool bShow, bool bConnect)
{
    m_mGraphs.insert(make_pair(sName, graph(clr, nIntervalMicroSeconds, bShow,bConnect)));
}

void HistoryGraph::AddPeak(const wxString& sGraph, double dPeak)
{
    auto now = std::chrono::system_clock::now();

    AddPeak(sGraph, dPeak, now);
}
void HistoryGraph::AddPeak(const wxString& sGraph, double dPeak, const std::chrono::time_point<std::chrono::system_clock>& tp)
{
    auto itGraph = m_mGraphs.find(sGraph);
    if(itGraph != m_mGraphs.end())
    {
        itGraph->second.lstPeaks.push_back(std::make_pair(dPeak,tp));

        auto micro = std::chrono::duration_cast<std::chrono::microseconds>(tp.time_since_epoch() - itGraph->second.lstPeaks.front().second.time_since_epoch());
        int64_t nMicroseconds = micro.count();
        nMicroseconds *= itGraph->second.nPixels;
        nMicroseconds /= itGraph->second.nInterval;

        if(nMicroseconds> m_rectGraph.GetWidth())
        {
            itGraph->second.lstPeaks.pop_front();
        }

        AutoRange(itGraph->second);

        if(itGraph->second.bShow)
        {
            Refresh();
        }
    }
}

void HistoryGraph::SetLine(const wxString& sName, double dStart, const std::chrono::time_point<std::chrono::system_clock>& tpStart, double dEnd, const std::chrono::time_point<std::chrono::system_clock>& tpEnd)
{
    auto itGraph = m_mGraphs.find(sName);
    if(itGraph != m_mGraphs.end())
    {
        itGraph->second.lstPeaks.clear();
        itGraph->second.lstPeaks.push_back(graphPoint(dStart, tpStart));
        itGraph->second.lstPeaks.push_back(graphPoint(dEnd, tpEnd));

        AutoRange(itGraph->second);

        if(itGraph->second.bShow)
        {
            Refresh();
        }
    }
}

void HistoryGraph::AutoRange(graph& aGraph)
{
    if(aGraph.lstPeaks.empty() == false)
    {
        aGraph.dMin = std::min(aGraph.dMin, aGraph.lstPeaks.back().first);
        aGraph.dMax = std::max(aGraph.dMax, aGraph.lstPeaks.back().first);
        aGraph.dResolution = static_cast<double>(m_rectGraph.GetHeight())/((aGraph.dMax)-(aGraph.dMin));
    }
}




void HistoryGraph::OnSize(wxSizeEvent& event)
{
    Refresh();
}

void HistoryGraph::ClearGraphs()
{
    for(auto& pairGraph : m_mGraphs)
    {
        ClearGraph(pairGraph.second);
    }

    Refresh();
}

void HistoryGraph::ClearGraph(graph& aGraph)
{
    aGraph.lstPeaks.clear();
    aGraph.dMax = std::numeric_limits<double>::lowest();
    aGraph.dMin = std::numeric_limits<double>::max();
    aGraph.dResolution = static_cast<double>(GetClientSize().y);
}

void HistoryGraph::ClearGraph(const wxString& sGraph)
{
    auto itGraph = m_mGraphs.find(sGraph);
    if(itGraph != m_mGraphs.end())
    {
        ClearGraph(itGraph->second);
        Refresh();
    }
}

void HistoryGraph::DeleteAllGraphs()
{
    m_mGraphs.clear();
    m_itMaster = m_mGraphs.end();
    Refresh();
}

void HistoryGraph::HideAllGraphs()
{
    for(map<wxString, graph>::iterator itGraph = m_mGraphs.begin(); itGraph != m_mGraphs.end(); ++itGraph)
    {
        itGraph->second.bShow = false;
    }
}


void HistoryGraph::ShowGraph(const wxString& sGraph, bool bShow)
{
    auto itGraph = m_mGraphs.find(sGraph);
    if(itGraph != m_mGraphs.end())
    {
        itGraph->second.bShow = bShow;
    }
    Refresh();
}

void HistoryGraph::ChangeInterval(const wxString& sGraph, unsigned int nIntervalMicroSeconds)
{
    auto itGraph = m_mGraphs.find(sGraph);
    if(itGraph != m_mGraphs.end())
    {
        itGraph->second.nInterval = nIntervalMicroSeconds;
    }
    Refresh();
}

void HistoryGraph::ChangeResolution(const wxString& sGraph, unsigned int nPixels)
{
    auto itGraph = m_mGraphs.find(sGraph);
    if(itGraph != m_mGraphs.end())
    {
        itGraph->second.nPixels = nPixels;
    }
}

void HistoryGraph::SetGraphUnits(const wxString& sGraph, const wxString& sUnits)
{
    auto itGraph = m_mGraphs.find(sGraph);
    if(itGraph != m_mGraphs.end())
    {
        itGraph->second.sUnits = sUnits;
        Refresh();
    }
}

void HistoryGraph::WorkoutMinMax(graph& aGraph)
{
    aGraph.dMax = std::numeric_limits<double>::lowest();
    aGraph.dMin = std::numeric_limits<double>::max();

    for(auto dPeak : aGraph.lstPeaks)
    {
        aGraph.dMax = std::max(aGraph.dMax, dPeak.first);
        aGraph.dMin = std::min(aGraph.dMin, dPeak.first);
    }
}

void HistoryGraph::RecalculateRange(const wxString& sGraph)
{
    auto itGraph = m_mGraphs.find(sGraph);
    if(itGraph != m_mGraphs.end())
    {
        WorkoutMinMax(itGraph->second);
        if(itGraph->second.bShow)
        {
            Refresh();
        }
    }
    else
    {
        pmlLog(pml::LOG_DEBUG) << "HistoryGraph::RecalculateRange: Could not find graph '" << sGraph << "'";
    }
}

void HistoryGraph::SetMasterGraph(const wxString& sGraph)
{
    m_itMaster =m_mGraphs.find(sGraph);
    Refresh();
}

