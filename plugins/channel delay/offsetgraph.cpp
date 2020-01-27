#include "offsetgraph.h"
#include <wx/dc.h>
#include <wx/dcbuffer.h>
#include <wx/log.h>
#include <wx/image.h>
#include "uirect.h"
#include <algorithm>
#include "settings.h"


using namespace std;

BEGIN_EVENT_TABLE(OffsetGraph, pmControl)
    EVT_PAINT(OffsetGraph::OnPaint)
    EVT_SIZE(OffsetGraph::OnSize)
END_EVENT_TABLE()
 wxIMPLEMENT_DYNAMIC_CLASS(OffsetGraph, pmControl);


OffsetGraph::OffsetGraph(wxWindow *parent, wxWindowID id, const wxPoint& pos, const wxSize& size) : pmControl(), m_nMaxSamples(1), m_nOffset(0), m_nLastOffset(-1)
{
    Create(parent, id, pos, size);
}

bool OffsetGraph::Create(wxWindow *parent, wxWindowID id, const wxPoint& pos, const wxSize& size)
{
    wxSize szInit(size);
    wxSize bestSize(801,481);
    if(size.x<=0)
        szInit.SetWidth(bestSize.x);
    if(size.y <= 0)
        szInit.SetHeight(bestSize.y);

    if(!wxWindow::Create(parent,id,pos,szInit,wxWANTS_CHARS, wxT("OffsetGraph")))
        return false;

    SetMinSize(size);

    SetBackgroundStyle(wxBG_STYLE_CUSTOM);

    m_bCalculating = false;
    m_dSampleRate = 48.0;


    SetFont(wxFont(8,wxFONTFAMILY_SWISS,wxFONTSTYLE_NORMAL,wxFONTWEIGHT_NORMAL,false,_T("Arial"),wxFONTENCODING_DEFAULT));

    return true;
}

OffsetGraph::~OffsetGraph()
{

}

void OffsetGraph::SetSampleRate(unsigned long nSampleRate)
{
    m_dSampleRate = static_cast<double>(nSampleRate) / 1000.0;
}

void OffsetGraph::OnPaint(wxPaintEvent& event)
{
    //wxAutoBufferedPaintDC dc(this);
    wxAutoBufferedPaintDC dc(this);


    wxBrush br(*wxBLACK);
    dc.SetBrush(br);
    dc.DrawRectangle(GetClientRect());

    dc.SetPen(*wxWHITE_PEN);
    dc.DrawRectangle(m_rectGrid);

    dc.SetFont(GetFont());

    int nCenter =(m_rectGrid.GetLeft()+m_rectGrid.GetWidth()/2);

    dc.SetPen(wxPen(wxColour(120,120,120)));
    double dGap = m_rectGrid.GetWidth()/10;

    for(int i = 0; i < 10; i++)
    {
        uiRect rectLead(wxRect(m_rectGrid.GetLeft()+(dGap*i)-30, m_rectGrid.GetBottom()+20, 60, 25));
        rectLead.SetBackgroundColour(*wxBLACK);
        rectLead.Draw(dc, wxString::Format(wxT("%.0fms"), (m_dTimeFrame/5)*i), uiRect::BORDER_NONE);


        dc.SetPen(wxPen(wxColour(120,120,120),1));
        dc.DrawLine(m_rectGrid.GetLeft()+(dGap*i), m_rectGrid.GetTop(), m_rectGrid.GetLeft()+(dGap*i), rectLead.GetTop());
    }



    dc.GradientFillLinear(m_rectSamplesToGet, wxColour(200,200,0), wxColour(0,255,0));
    dc.SetPen(*wxWHITE_PEN);
    dc.SetBrush(*wxTRANSPARENT_BRUSH);
    dc.DrawRectangle(m_rectSamplesToGet);
    dc.SetPen(*wxTRANSPARENT_PEN);
    dc.SetBrush(*wxBLACK_BRUSH);
    dc.DrawRectangle(m_rectSamples);

    int y = m_rectGrid.GetTop();

    double dX = m_dResolution * static_cast<double>(m_nOffset);
    double dL, dR;



    if(dX < 0)   //this means R is delayed to L
    {
        dL = m_rectGrid.GetLeft();
        dR = m_rectGrid.GetLeft()-dX;

    }
    else
    {
        dL = m_rectGrid.GetLeft()+dX;
        dR = m_rectGrid.GetLeft();

    }

    dc.SetPen(*wxGREEN_PEN);
    dc.DrawLine(dL, m_rectGrid.GetTop()-5, dL, m_rectGrid.GetBottom());
    uiRect rectLeft(wxRect(dL-15, 0, 30, 20));
    rectLeft.SetBackgroundColour(*wxBLACK);
    rectLeft.SetForegroundColour(*wxGREEN);
    rectLeft.Draw(dc, m_sChannel[0], uiRect::BORDER_NONE);


    dc.SetPen(*wxRED_PEN);
    dc.DrawLine(dR, m_rectGrid.GetTop()-5, dR, m_rectGrid.GetBottom());
    uiRect rectRight(wxRect(dR-15, 0, 30, 20));
    rectRight.SetBackgroundColour(*wxBLACK);
    rectRight.SetForegroundColour(*wxRED);
    rectRight.Draw(dc, m_sChannel[1], uiRect::BORDER_NONE);


    dc.SetFont(GetFont());
    uiRect uiSamples(wxRect(m_rectGrid.GetLeft(), GetClientRect().GetBottom()-30, 200,25));
    uiSamples.Draw(dc, m_sSamples, uiRect::BORDER_NONE);

    uiRect uiTimeframe(wxRect(uiSamples.GetRight()+10, GetClientRect().GetBottom()-30, 200,25));
    uiTimeframe.Draw(dc, m_sTimeframe, uiRect::BORDER_NONE);



    if(m_bCalculating)
    {
        uiRect uiCalc(wxRect(m_rectSamplesToGet.GetLeft()-120, 0, 100, 20));
        uiCalc.SetBackgroundColour(wxColour(100,0,0));
        uiCalc.Draw(dc, wxT("Calculating"), uiRect::BORDER_FLAT);
    }
}

void OffsetGraph::OnSize(wxSizeEvent& event)
{
    m_rectGrid = wxRect(25, 25, GetClientRect().GetWidth()-40, GetClientRect().GetHeight()-100);

    m_rectSamplesToGet =(wxRect(GetClientRect().GetRight()-120, GetClientRect().GetBottom()-25, 102, 20));



    SetTimeFrame(m_nMaxSamples);

}


void OffsetGraph::SetTimeFrame(int nMaxSamples)
{
    m_nMaxSamples = nMaxSamples/4;
    m_dResolution = static_cast<double>(m_rectGrid.GetWidth())/static_cast<double>(m_nMaxSamples*2);



    m_dTimeFrame = (1.0 / m_dSampleRate)*static_cast<double>(m_nMaxSamples);

    Refresh();
}

void OffsetGraph::AddOffset(int nSample)
{
    m_nOffset = nSample;
    if(m_nOffset < 0)
    {
        m_sSamples.Printf(wxT("%s leads %s by %d samples"), m_sChannel[0].c_str(), m_sChannel[1].c_str(), -m_nOffset);
        m_sTimeframe.Printf(wxT("%s leads %s by %.0f ms"),m_sChannel[0].c_str(), m_sChannel[1].c_str(), -(static_cast<double>(m_nOffset)/m_dSampleRate));
    }
    else
    {
        m_sSamples.Printf(wxT("%s leads %s by %d samples"),m_sChannel[1].c_str(), m_sChannel[0].c_str(), m_nOffset);
        m_sTimeframe.Printf(wxT("%s leads %s by %.0f ms"), m_sChannel[1].c_str(), m_sChannel[0].c_str(), (static_cast<double>(m_nOffset)/m_dSampleRate));

    }
    if(m_nOffset != m_nLastOffset)
    {
        //TestLog::Get()->Log(wxT("ChannelDelay"), m_sTimeframe);
    }
    m_nLastOffset = m_nOffset;
    Refresh();
}

void OffsetGraph::ShowSamplesLeftToGet(unsigned int nSamples)
{
    int nPercent = min(100.0, max(0.0, static_cast<double>(nSamples)/static_cast<double>(m_nMaxSamples*4)*100.0));

    m_rectSamples = wxRect(m_rectSamplesToGet.GetLeft()+(101-nPercent), m_rectSamplesToGet.GetTop()+1, nPercent, m_rectSamplesToGet.GetHeight()-2);
    RefreshRect(m_rectSamplesToGet);
}

void OffsetGraph::SetCalculating(bool bCalc)
{
    m_bCalculating = bCalc;
    RefreshRect(wxRect(m_rectSamplesToGet.GetLeft()-120, 0, 100, 20));
}


void OffsetGraph::SetChannel1Name(const wxString& sChannel1)
{
    m_sChannel[0] = sChannel1;
}

void OffsetGraph::SetChannel2Name(const wxString& sChannel2)
{
    m_sChannel[1] = sChannel2;
}
