#include "historygraph.h"
#include <wx/dc.h>
#include <wx/dcbuffer.h>
#include <wx/log.h>
#include "uirect.h"

using namespace std;

BEGIN_EVENT_TABLE(HistoryGraph, pmControl)
    EVT_PAINT(HistoryGraph::OnPaint)
    EVT_SIZE(HistoryGraph::OnSize)
END_EVENT_TABLE()

 wxIMPLEMENT_DYNAMIC_CLASS(HistoryGraph, pmControl);



HistoryGraph::HistoryGraph(wxWindow *parent, wxWindowID id, const wxPoint& pos, const wxSize& size) : pmControl()
{
    Create(parent, id, pos, size);

    m_rectGraph = wxRect(GetClientRect().GetLeft()+5, GetClientRect().GetTop()+5, GetClientRect().GetWidth()-45, GetClientRect().GetBottom()-25);
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
                dc.SetTextForeground(*wxWHITE);
                //we draw the axis and range for the first graph and not the others....
                dc.SetPen(wxPen(GetForegroundColour()));
                dc.DrawLine(m_rectGraph.GetBottomLeft(), m_rectGraph.GetBottomRight());
                dc.DrawLine(m_rectGraph.GetBottomRight(), m_rectGraph.GetTopRight());

                double dRange = pairGraph.second.dMax-pairGraph.second.dMin;
                wxString sMax(wxString::Format("%g", pairGraph.second.dMax));
                wxSize szText = dc.GetTextExtent(sMax);
                dc.DrawText(sMax, m_rectGraph.GetRight()+2, m_rectGraph.GetTop()-szText.y/2);
                dc.DrawLine(m_rectGraph.GetLeft(), m_rectGraph.GetTop(), m_rectGraph.GetRight()+2, m_rectGraph.GetTop());

                wxString sMin(wxString::Format("%g", pairGraph.second.dMin));
                szText = dc.GetTextExtent(sMin);
                dc.DrawText(sMin, m_rectGraph.GetRight()+2, m_rectGraph.GetBottom()-szText.y/2);


                wxString sUpper(wxString::Format("%g", pairGraph.second.dMax-dRange/4));
                szText = dc.GetTextExtent(sUpper);
                dc.DrawText(sUpper, m_rectGraph.GetRight()+2, m_rectGraph.GetTop()+m_rectGraph.GetHeight()/4-szText.y/2);
                dc.DrawLine(m_rectGraph.GetLeft(), m_rectGraph.GetTop()+m_rectGraph.GetHeight()/4, m_rectGraph.GetRight()+2, m_rectGraph.GetTop()+m_rectGraph.GetHeight()/4);

                wxString sLower(wxString::Format("%g", pairGraph.second.dMin+dRange/4));
                szText = dc.GetTextExtent(sLower);
                dc.DrawText(sLower, m_rectGraph.GetRight()+2, m_rectGraph.GetBottom()-m_rectGraph.GetHeight()/4-szText.y/2);
                dc.DrawLine(m_rectGraph.GetLeft(), m_rectGraph.GetBottom()-m_rectGraph.GetHeight()/4, m_rectGraph.GetRight()+2, m_rectGraph.GetBottom()-m_rectGraph.GetHeight()/4);

                wxString sMid(wxString::Format("%g", pairGraph.second.dMin+dRange/2));
                szText = dc.GetTextExtent(sMid);
                dc.DrawText(sMid, m_rectGraph.GetRight()+2, m_rectGraph.GetTop()+m_rectGraph.GetHeight()/2-szText.y/2);
                dc.DrawLine(m_rectGraph.GetLeft(), m_rectGraph.GetTop()+m_rectGraph.GetHeight()/2, m_rectGraph.GetRight()+2, m_rectGraph.GetTop()+m_rectGraph.GetHeight()/2);

                for(int x = m_rectGraph.GetRight(); x >= m_rectGraph.GetLeft(); x-= 40)
                {
                    dc.DrawLine(x, m_rectGraph.GetBottom(), x, m_rectGraph.GetBottom()+3);

                    wxString sTime;
                    if(pairGraph.second.nIntervalDefault >= 1e6)
                    {
                        sTime.Printf("-%gs", static_cast<double>(pairGraph.second.nIntervalDefault/1e6)*(m_rectGraph.GetRight()-x));
                    }
                    else if(pairGraph.second.nIntervalDefault >= 1e3)
                    {
                        sTime.Printf("-%gms", static_cast<double>(pairGraph.second.nIntervalDefault/1e3)*(m_rectGraph.GetRight()-x));
                    }
                    else
                    {
                        sTime.Printf("-%uus", pairGraph.second.nIntervalDefault*(m_rectGraph.GetRight()-x));
                    }
                    szText = dc.GetTextExtent(sTime);
                    dc.DrawText(sTime, x-szText.GetWidth()/2, m_rectGraph.GetBottom()+2);
                }
                bFirst = false;
            }


            dc.SetPen(pairGraph.second.clrLine);
            double dY_old(0);

            int x = m_rectGraph.GetRight()-1;
            for(list<double>::reverse_iterator itPeak = pairGraph.second.lstPeaks.rbegin(); itPeak != pairGraph.second.lstPeaks.rend(); ++itPeak)
            {
                double dPeak = (*itPeak);
                dPeak-= (pairGraph.second.dMin);

                double dTop = max((double)m_rectGraph.GetTop(), m_rectGraph.GetBottom()-(pairGraph.second.dResolution*dPeak));

                if(itPeak == pairGraph.second.lstPeaks.rbegin())
                {
                    dY_old = dTop;
                }

                dc.DrawLine(x+1, dY_old, x, dTop);

                dY_old=dTop;
                x--;
            }



        }
    }



}

void HistoryGraph::AddGraph(const wxString& sName, const wxColour& clr, unsigned int nIntervalMicroSeconds, bool bShow)
{
    m_mGraphs.insert(make_pair(sName, graph(clr, nIntervalMicroSeconds, bShow)));
}

void HistoryGraph::AddPeak(const wxString& sGraph, double dPeak)
{
    auto itGraph = m_mGraphs.find(sGraph);
    if(itGraph != m_mGraphs.end())
    {
        itGraph->second.lstPeaks.push_back(dPeak);

        if(itGraph->second.lstPeaks.size() > m_rectGraph.GetWidth())
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

void HistoryGraph::AutoRange(graph& aGraph)
{
    if(aGraph.lstPeaks.empty() == false)
    {
        aGraph.dMin = std::min(aGraph.dMin, aGraph.lstPeaks.back());
        aGraph.dMax = std::max(aGraph.dMax, aGraph.lstPeaks.back());
        aGraph.dResolution = static_cast<double>(m_rectGraph.GetHeight())/((aGraph.dMax)-(aGraph.dMin));
    }
}




void HistoryGraph::OnSize(wxSizeEvent& event)
{
    Refresh();
}

void HistoryGraph::ClearGraphs()
{
    for(map<wxString, graph>::iterator itGraph = m_mGraphs.begin(); itGraph != m_mGraphs.end(); ++itGraph)
    {
        itGraph->second.lstPeaks.clear();
        itGraph->second.dMax = std::numeric_limits<double>::lowest();
        itGraph->second.dMin = std::numeric_limits<double>::max();
        itGraph->second.dResolution = static_cast<double>(GetClientSize().y);
    }
    Refresh();
}

void HistoryGraph::DeleteAllGraphs()
{
    m_mGraphs.clear();
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
    map<wxString, graph>::iterator itGraph = m_mGraphs.find(sGraph);
    if(itGraph != m_mGraphs.end())
    {
        itGraph->second.bShow = bShow;
    }
    Refresh();
}

