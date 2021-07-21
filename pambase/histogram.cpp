#include "histogram.h"
#include <wx/dc.h>
#include <wx/dcbuffer.h>
#include <wx/log.h>
#include "uirect.h"
#include <algorithm>
#include <iostream>

using namespace std;

BEGIN_EVENT_TABLE(Histogram, pmControl)
    EVT_PAINT(Histogram::OnPaint)
    EVT_SIZE(Histogram::OnSize)
    EVT_LEFT_DOWN(Histogram::OnLeftDown)
    EVT_LEFT_UP(Histogram::OnLeftUp)
    EVT_MOTION(Histogram::OnMouseMove)
END_EVENT_TABLE()

 wxIMPLEMENT_DYNAMIC_CLASS(Histogram, pmControl);



Histogram::Histogram(wxWindow *parent, wxWindowID id, const wxPoint& pos, const wxSize& size) : pmControl()
{
    Create(parent, id, pos, size);
    m_bMouseDown = false;
}

bool Histogram::Create(wxWindow *parent, wxWindowID id, const wxPoint& pos, const wxSize& size)
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

    m_rectGraph = wxRect(GetClientRect().GetLeft()+40, GetClientRect().GetTop()+5, GetClientRect().GetWidth()-45, GetClientRect().GetBottom()-25);

    return true;
}

Histogram::~Histogram()
{

}


void Histogram::OnPaint(wxPaintEvent& event)
{
    wxAutoBufferedPaintDC dc(this);
    wxBrush br(*wxBLACK);
    dc.SetBrush(br);
    dc.SetPen(*wxTRANSPARENT_PEN);
    dc.DrawRectangle(GetClientRect());
    dc.SetFont(GetFont());

    auto itGraph = m_mGraphs.find(m_sGraph);
    if(itGraph != m_mGraphs.end() && itGraph->second.mColumns.empty() == false)
    {
        dc.SetPen(wxPen(wxColour(200,200,200)));

        //work out the axis
        double dMin = /*max(0, static_cast<int>(*/itGraph->second.mColumns.begin()->first/*)-1)*/;
        double dMax = itGraph->second.mColumns.rbegin()->first+1;

        if(itGraph->second.bScrolled == false)
        {
            itGraph->second.dResolutionX = m_rectGraph.GetWidth()/(dMax-dMin);
        }

        double dResolutionX = itGraph->second.dResolutionX * itGraph->second.dResolution;

        unsigned int nMaxY = 0;
        int nMaxCol(0);
        for(auto pairColumn : itGraph->second.mColumns)
        {
            if(nMaxY < pairColumn.second)
            {
                nMaxY = pairColumn.second;
                nMaxCol = pairColumn.first;
            }
        }

        double dResolutionY = m_rectGraph.GetHeight()/static_cast<double>(nMaxY);

        dc.SetTextForeground(*wxWHITE);



        double dPercent = static_cast<double>(nMaxY)/static_cast<double>(itGraph->second.lstPeaks.size())*100.0;
        wxString sPercent(wxString::Format("%.1f%%", dPercent));
        wxSize szPercent = dc.GetTextExtent(sPercent);

        dc.DrawText(sPercent, GetClientRect().GetLeft()+5, m_rectGraph.GetTop()-szPercent.y/2);
        dc.DrawText(wxString::Format("%.1f%%", dPercent*.75), GetClientRect().GetLeft()+5, m_rectGraph.GetTop()+m_rectGraph.GetHeight()/4-szPercent.y/2);
        dc.DrawText(wxString::Format("%.1f%%", dPercent*.5), GetClientRect().GetLeft()+5, m_rectGraph.GetTop()+m_rectGraph.GetHeight()/2-szPercent.y/2);
        dc.DrawText(wxString::Format("%.1f%%", dPercent*.25), GetClientRect().GetLeft()+5, m_rectGraph.GetBottom()-m_rectGraph.GetHeight()/4-szPercent.y/2);


        dc.DrawLine(m_rectGraph.GetLeft()-2, m_rectGraph.GetTop(), m_rectGraph.GetLeft(), m_rectGraph.GetTop());
        dc.DrawLine(m_rectGraph.GetLeft()-2, m_rectGraph.GetTop()+m_rectGraph.GetHeight()/4, m_rectGraph.GetRight(), m_rectGraph.GetTop()+m_rectGraph.GetHeight()/4);
        dc.DrawLine(m_rectGraph.GetLeft()-2, m_rectGraph.GetTop()+m_rectGraph.GetHeight()/2, m_rectGraph.GetRight(), m_rectGraph.GetTop()+m_rectGraph.GetHeight()/2);
        dc.DrawLine(m_rectGraph.GetLeft()-2, m_rectGraph.GetBottom()-m_rectGraph.GetHeight()/4, m_rectGraph.GetRight(), m_rectGraph.GetBottom()-m_rectGraph.GetHeight()/4);

        dc.DrawLine(m_rectGraph.GetTopLeft(), m_rectGraph.GetBottomLeft());
        dc.DrawLine(m_rectGraph.GetBottomLeft(), m_rectGraph.GetBottomRight());


        int nColumnAdd = std::max(1.0, 1.0/dResolutionX);

        unsigned int nWidth = std::max(1.0, dResolutionX);
        uiRect rect(wxRect(-30,0,0,0));
        rect.SetBackgroundColour(*wxBLACK);
        rect.SetForegroundColour(*wxWHITE);

        m_dFirstColumn = -1;
        int nFirstColumnToShow = itGraph->second.dFirstDisplay/itGraph->second.dInterval;

        for(int n = dMin; n <= dMax; n+=nColumnAdd)
        {

            if(n >= nFirstColumnToShow)
            {
                if(m_dFirstColumn < 0)
                {
                    m_dFirstColumn = n*itGraph->second.dInterval;
                }

                int nLeft = dResolutionX*(n-nFirstColumnToShow)+ m_rectGraph.GetLeft();


                if(nLeft > m_rectGraph.GetRight())
                {
                    break;
                }

                int nCenter = (nLeft+nWidth/2);

                wxSize szText = dc.GetTextExtent(wxString::Format("%g", n*itGraph->second.dInterval));
                if(nCenter- szText.x/2 >= rect.GetRight()+5)
                {
                    dc.SetPen(wxPen(wxColour(200,200,200)));
                    dc.DrawLine(nCenter, m_rectGraph.GetBottom()+3, nCenter, m_rectGraph.GetBottom());

                    rect.SetRect(wxRect(nCenter - szText.x/2, m_rectGraph.GetBottom()+3, szText.x,20));
                    rect.Draw(dc, wxString::Format("%g", n*itGraph->second.dInterval), uiRect::BORDER_NONE);
                }
            }
        }

        dc.SetBrush(wxBrush(itGraph->second.clrLine));
        dc.SetPen(wxPen(wxColour(max(itGraph->second.clrLine.Red()-60, 0),
                                 max(itGraph->second.clrLine.Green()-60, 0),
                                 max(itGraph->second.clrLine.Blue()-60, 0))));


        for(auto pairCol : itGraph->second.mColumns)
        {
            if(pairCol.first*itGraph->second.dInterval >= itGraph->second.dFirstDisplay)
            {
                unsigned int nLeft = dResolutionX*(pairCol.first-(itGraph->second.dFirstDisplay/itGraph->second.dInterval))+ m_rectGraph.GetLeft();
                wxRect rect(wxRect(nLeft, m_rectGraph.GetBottom(), nWidth,-std::max(1.0,dResolutionY*pairCol.second)));
                dc.DrawRectangle(rect);

                if(nLeft > m_rectGraph.GetRight())
                    break;
            }
        }



    }

}

void Histogram::AddGraph(const wxString& sName, const wxColour& clr, double dInterval, size_t nMaxEntries)
{
    m_mGraphs.insert(make_pair(sName, graph(clr, dInterval, nMaxEntries)));
}

void Histogram::AddPeak(const wxString& sGraph, double dPeak)
{
    auto itGraph = m_mGraphs.find(sGraph);
    if(itGraph != m_mGraphs.end())
    {
        itGraph->second.lstPeaks.push_back(dPeak);

        double dColumn;
        modf(dPeak/itGraph->second.dInterval, &dColumn);
        auto ins = itGraph->second.mColumns.insert(std::make_pair(dColumn, 1));
        if(ins.second == false)
        {
            ins.first->second++;
        }
        if(!itGraph->second.bScrolled)
        {
            itGraph->second.dFirstDisplay = itGraph->second.mColumns.begin()->first*itGraph->second.dInterval;
        }

        //if we've got more entries erase the oldest one and minus it from the column count
        if(itGraph->second.lstPeaks.size() > itGraph->second.nMaxEntries)
        {
            modf(itGraph->second.lstPeaks.front()/itGraph->second.dInterval, &dColumn);
            auto itCol = itGraph->second.mColumns.find(dColumn);
            if(itCol != itGraph->second.mColumns.end())
            {
                itCol->second--;
            }
            itGraph->second.lstPeaks.pop_front();
        }

        if(sGraph == m_sGraph)
        {
            Refresh();
        }
    }
}

void Histogram::ChangeGranularity(const wxString& sGraph, int nWhich)
{
    auto itGraph = m_mGraphs.find(sGraph);
    if(itGraph != m_mGraphs.end())
    {
        switch(nWhich)
        {
            case DECREASE:
                itGraph->second.dInterval*=2;
                break;
            case RESET:
                itGraph->second.dInterval = itGraph->second.dIntervalDefault;
                break;
            case INCREASE:
                itGraph->second.dInterval/=2;
                break;
        }
        WorkoutColumns(itGraph->second);
    }
}

void Histogram::ChangeResolution(const wxString& sGraph, int nWhich)
{
    auto itGraph = m_mGraphs.find(sGraph);
    if(itGraph != m_mGraphs.end())
    {
        switch(nWhich)
        {
            case DECREASE:
                itGraph->second.dResolution/=2;
                break;
            case RESET:
                itGraph->second.dResolution = 1.0;
                break;
            case INCREASE:
                itGraph->second.dResolution*=2;
                break;
        }
        Refresh();
    }
}

void Histogram::OnSize(wxSizeEvent& event)
{

}

void Histogram::ClearGraph(graph& aGraph)
{
    aGraph.mColumns.clear();
    aGraph.lstPeaks.clear();
}

void Histogram::ClearGraphs()
{
    for(auto& pairGraph : m_mGraphs)
    {
        ClearGraph(pairGraph.second);
    }
    Refresh();
}

void Histogram::ClearGraph(const wxString& sGraph)
{
    auto itGraph = m_mGraphs.find(sGraph);
    if(itGraph != m_mGraphs.end())
    {
        ClearGraph(itGraph->second);
        Refresh();
    }
}

void Histogram::DeleteAllGraphs()
{
    m_mGraphs.clear();
    m_sGraph = "";
    Refresh();
}

void Histogram::HideAllGraphs()
{
    m_sGraph = "";
    Refresh();
}


void Histogram::ShowGraph(const wxString& sGraph)
{
    m_sGraph = sGraph;
    Refresh();

}

void Histogram::WorkoutColumns(graph& aGraph)
{
    aGraph.mColumns.clear();
    for(auto dPeak : aGraph.lstPeaks)
    {
        double dColumn;
        modf(dPeak/aGraph.dInterval, &dColumn);
        auto ins = aGraph.mColumns.insert(std::make_pair(dColumn, 1));
        if(ins.second == false)
        {
            ins.first->second++;
        }
    }
    Refresh();
}

void Histogram::OnLeftDown(wxMouseEvent& event)
{
    auto itGraph = m_mGraphs.find(m_sGraph);
    if(itGraph != m_mGraphs.end())
    {
        m_pntMouse = event.GetPosition();
        m_bMouseDown = true;
        itGraph->second.dFirstDisplay = m_dFirstColumn;
    }
    CaptureMouse();
}

void Histogram::OnLeftUp(wxMouseEvent& event)
{
    m_bMouseDown = false;
    ReleaseMouse();
}

void Histogram::OnMouseMove(wxMouseEvent& event)
{
    if(m_bMouseDown)
    {
        int nDiff = event.GetPosition().x  - m_pntMouse.x;

        auto itGraph = m_mGraphs.find(m_sGraph);
        if(itGraph != m_mGraphs.end())
        {
            itGraph->second.bScrolled = true;
            double dMin = max(0, static_cast<int>(itGraph->second.mColumns.begin()->first)-1);
            double dMax = itGraph->second.mColumns.rbegin()->first+1;
            double dResolutionX = m_rectGraph.GetWidth()/(dMax-dMin);
            dResolutionX *= itGraph->second.dResolution;

            int nColumns = nDiff/dResolutionX;
            itGraph->second.dFirstDisplay = max(0.0, itGraph->second.dFirstDisplay-(nColumns*itGraph->second.dInterval));
            Refresh();
        }
        m_pntMouse = event.GetPosition();
    }

}

