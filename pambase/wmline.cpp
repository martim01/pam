#include "wmline.h"
#include <wx/dcbuffer.h>
#include <wx/log.h>

using namespace std;


BEGIN_EVENT_TABLE(wmLineWnd, pmControl)
    EVT_PAINT(wmLineWnd::OnPaint)
    EVT_SIZE(wmLineWnd::OnSize)
END_EVENT_TABLE()

#ifdef WXSPAM
IMPLEMENT_DYNAMIC_CLASS(wmLineWnd, pmControl)
#else
 wxIMPLEMENT_DYNAMIC_CLASS(wmLineWnd, pmControl);
#endif // WXSPAM

wmLineWnd::wmLineWnd(wxWindow *parent, wxWindowID id, const wxPoint& pos, const wxSize& size, long nStyle) : pmControl()
{
    Create(parent, id,  pos, size, nStyle);
}



bool wmLineWnd::Create(wxWindow *parent, wxWindowID id, const wxPoint& pos, const wxSize& size, long nStyle)
{
    wxSize szInit(size);
    wxSize bestSize = DoGetBestSize();
    if(size.x<=0)
        szInit.SetWidth(bestSize.x);
    if(size.y <= 0)
        szInit.SetHeight(bestSize.y);


    if(!wxWindow::Create(parent,id,pos,szInit,wxWANTS_CHARS, wxEmptyString))
        return false;


    SetBackgroundStyle(wxBG_STYLE_CUSTOM);

    SetBackgroundColour(GetParent()->GetBackgroundColour());


    return true;
}

wmLineWnd::~wmLineWnd()
{

}

bool wmLineWnd::AddPercentageLine(const wxString& sName, const wmLine& ln)
{
    if(m_mLinePercentages.insert(make_pair(sName, ln)).second)
    {
        Refresh();
        return true;
    }
    return false;
}

bool wmLineWnd::AddPercentageLine(const wxString& sName, const wxPoint& pntStart, const wxPoint& pntEnd, const wxColour& colour, int nWidth, int nPen, unsigned int nLineStyle, unsigned int nStartArrow, unsigned int nEndArrow, bool bDraw, unsigned int nArrowSize)
{

    if(m_mLinePercentages.insert(make_pair(sName, wmLine(pntStart, pntEnd, colour, nWidth, nPen, nLineStyle, nStartArrow, nEndArrow, nArrowSize, bDraw))).second)
    {
        Refresh();
        return true;
    }
    return false;
}


bool wmLineWnd::UpdatePercentageLine(const wxString& sName, const wxPoint& pntStart, const wxPoint& pntEnd, const wxColour& colour, int nWidth, int nPen, unsigned int nLineStyle, unsigned int nStartArrow, unsigned int nEndArrow, bool bDraw, unsigned int nArrowSize)
{
    map<wxString, wmLine>::iterator itLine = m_mLinePercentages.find(sName);
    if(itLine != m_mLinePercentages.end())
    {
        itLine->second.pntStart = pntStart;
        itLine->second.pntEnd = pntEnd;
        itLine->second.clr = colour;
        itLine->second.nWidth = nWidth;
        itLine->second.nPenStyle = nPen;
        itLine->second.nLineStyle = nLineStyle;
        itLine->second.nStartArrow = nStartArrow;
        itLine->second.nEndArrow = nEndArrow;
        itLine->second.nArrowSize = nArrowSize;
        itLine->second.bDraw = bDraw;
        Refresh();
        return true;
    }
    return false;
}

bool wmLineWnd::DeletePercentageLine(const wxString& sName)
{
    map<wxString, wmLine>::iterator itLine = m_mLinePercentages.find(sName);
    if(itLine != m_mLinePercentages.end())
    {
        m_mLinePercentages.erase(sName);
        Refresh();
        return true;
    }
    return false;
}

void wmLineWnd::OnPaint(wxPaintEvent& event)
{
    wxBufferedPaintDC dc(this);
    Draw(dc);
}

void wmLineWnd::OnSize(wxSizeEvent& event)
{
    Refresh();
}


void wmLineWnd::Draw(wxDC& dc)
{
    dc.SetFont(GetFont());

    dc.SetBackgroundMode(wxTRANSPARENT);
    dc.SetPen(*wxTRANSPARENT_PEN);
    dc.SetBrush(wxBrush(GetBackgroundColour()));
    dc.DrawRectangle(GetClientRect());

    wxSize szClient = GetClientSize();
    double dWidth = szClient.GetX();
    double dHeight = szClient.GetY();

    wxPoint pntStart, pntEnd;
    for(map<wxString, wmLine>::iterator itLine = m_mLinePercentages.begin(); itLine != m_mLinePercentages.end(); ++itLine)
    {
        if(itLine->second.bDraw)
        {
            dc.SetPen(wxPen(itLine->second.clr, itLine->second.nWidth, itLine->second.nPenStyle));
            dc.SetBrush(wxBrush(itLine->second.clr));

            CalculatePoints(dWidth, dHeight, itLine->second, pntStart, pntEnd);
            dc.DrawLine(pntStart, pntEnd);

            switch((itLine->second.nStartArrow))
            {
                case wmLine::LINEEND_ARROW:
                    DrawArrow(dc, pntStart, pntEnd, (itLine->second.nArrowSize),false);
                    break;
                case wmLine::LINEEND_SARROW:
                    DrawArrow(dc, pntStart, pntEnd, (itLine->second.nArrowSize), true);
                    break;
                case wmLine::LINEEND_SQUARE:
                    DrawSquare(dc, pntStart, pntEnd, (itLine->second.nArrowSize));
                    break;
                case wmLine::LINEEND_CIRCLE:
                    DrawCircle(dc, pntStart, pntEnd, (itLine->second.nArrowSize));
                    break;
            }

            switch((itLine->second.nEndArrow))
            {
                case wmLine::LINEEND_ARROW:
                    DrawArrow(dc, pntEnd, pntStart, (itLine->second.nArrowSize),false);
                    break;
                case wmLine::LINEEND_SARROW:
                    DrawArrow(dc, pntEnd, pntStart, (itLine->second.nArrowSize), true);
                    break;
                case wmLine::LINEEND_SQUARE:
                    DrawSquare(dc, pntEnd, pntStart, (itLine->second.nArrowSize));
                    break;
                case wmLine::LINEEND_CIRCLE:
                    DrawCircle(dc, pntEnd, pntStart, (itLine->second.nArrowSize));
                    break;
            }
        }
    }
}

void wmLineWnd::Rotate(const wxPoint& ptPoint, const wxPoint& ptBase, unsigned int nPoints, wxPoint pnts[])
{
    double dTheta = atan2(-(double)(ptBase.y - ptPoint.y), (double)(ptBase.x - ptPoint.x));
    //rotate and translate
    for(int i = 0; i < nPoints; i++)
    {
        double x = pnts[i].x;
        double y = pnts[i].y;

        pnts[i].x = (cos(dTheta)*x)+(sin(dTheta)*y) + ptPoint.x;
        pnts[i].y = (cos(dTheta)*y)-(sin(dTheta)*x) + ptPoint.y;
    }
}

void wmLineWnd::DrawArrow(wxDC& dc, const wxPoint& ptPoint, const wxPoint& ptBase, int nSize, bool bFill)
{
    //create arrow shape
    wxPoint pts[3];
    pts[0] = wxPoint(0,0);
    pts[1] = wxPoint(nSize, -nSize/2);
    pts[2] = wxPoint(nSize, nSize/2);

    Rotate(ptPoint, ptBase, 3, pts);

    if(!bFill)
    {
        dc.DrawLine(pts[0], pts[1]);
        dc.DrawLine(pts[0], pts[2]);
    }
    else
    {
        dc.DrawPolygon(3,pts);
    }
}

void wmLineWnd::DrawSquare(wxDC& dc, const wxPoint& ptPoint, const wxPoint& ptBase, int nSize)
{

    //create arrow shape
    wxPoint pts[4];
    pts[0] = wxPoint(0,0);
    pts[1] = wxPoint(nSize/2, -nSize/2);
    pts[2] = wxPoint(nSize, 0);
    pts[3] = wxPoint(nSize/2, nSize/2);


    Rotate(ptPoint, ptBase, 4, pts);


    dc.DrawPolygon(4,pts);
}

void wmLineWnd::DrawCircle(wxDC& dc, const wxPoint& ptPoint, const wxPoint& ptBase, int nSize)
{
    wxPoint pnt[1];
    pnt[0] = wxPoint(nSize,0);
    Rotate(ptPoint, ptBase, 1, pnt);
    //dc.DrawCircle(ptPoint,nSize);
    dc.DrawCircle(pnt[0],nSize);
}


void wmLineWnd::CalculatePoints(double dWidth, double dHeight, const wmLine& line, wxPoint& pntStart, wxPoint& pntEnd)
{
    double dStartX = dWidth*(static_cast<double>(line.pntStart.x)/100.0);
    double dStartY = dHeight*(static_cast<double>(line.pntStart.y)/100.0);

    double dEndX = dWidth*(static_cast<double>(line.pntEnd.x)/100.0);
    double dEndY = dHeight*(static_cast<double>(line.pntEnd.y)/100.0);

    pntStart.x = dStartX;
    pntStart.y = dStartY;

    pntEnd.x = dEndX;
    pntEnd.y = dEndY;

}

bool wmLineWnd::SetBackgroundColour(const wxColour &colour)
{
    wxWindow::SetBackgroundColour(colour);
    Refresh();
    return true;
}


bool wmLineWnd::HideLine(const wxString& sName, bool bHide)
{
    map<wxString, wmLine>::iterator itLine = m_mLinePercentages.find(sName);
    if(itLine != m_mLinePercentages.end())
    {
        itLine->second.bDraw = !bHide;
        Refresh();
        return true;
    }
    return false;
}

bool wmLineWnd::ChangeStartPercentage(const wxString& sName, const wxPoint& pntStart)
{
    map<wxString, wmLine>::iterator itLine = m_mLinePercentages.find(sName);
    if(itLine != m_mLinePercentages.end())
    {
        itLine->second.pntStart = pntStart;
        Refresh();
        return true;
    }
    return false;
}

bool wmLineWnd::ChangeEndPercentage(const wxString& sName, const wxPoint& pntEnd)
{
    map<wxString, wmLine>::iterator itLine = m_mLinePercentages.find(sName);
    if(itLine != m_mLinePercentages.end())
    {
        itLine->second.pntEnd = pntEnd;
        Refresh();
        return true;
    }
    return false;
}

bool wmLineWnd::ChangeColour(const wxString& sName, const wxColour& clr)
{
    map<wxString, wmLine>::iterator itLine = m_mLinePercentages.find(sName);
    if(itLine != m_mLinePercentages.end())
    {
        itLine->second.clr = clr;
        Refresh();
        return true;
    }
    return false;
}

bool wmLineWnd::ChangeWidth(const wxString& sName, int nWidth)
{
    map<wxString, wmLine>::iterator itLine = m_mLinePercentages.find(sName);
    if(itLine != m_mLinePercentages.end())
    {
        itLine->second.nWidth = nWidth;
        Refresh();
        return true;
    }
    return false;
}

bool wmLineWnd::ChangePenStyle(const wxString& sName, int nStyle)
{
    map<wxString, wmLine>::iterator itLine = m_mLinePercentages.find(sName);
    if(itLine != m_mLinePercentages.end())
    {
        itLine->second.nPenStyle = nStyle;
        Refresh();
        return true;
    }
    return false;
}

wxPoint wmLineWnd::GetStartPercentage(const wxString& sName)
{
    map<wxString, wmLine>::iterator itLine = m_mLinePercentages.find(sName);
    if(itLine != m_mLinePercentages.end())
    {
        return itLine->second.pntStart;
    }
    return wxPoint(-1,-1);
}

wxPoint wmLineWnd::GetEndPercentage(const wxString& sName)
{
    map<wxString, wmLine>::iterator itLine = m_mLinePercentages.find(sName);
    if(itLine != m_mLinePercentages.end())
    {
        return itLine->second.pntEnd;
    }
    return wxPoint(-1,-1);
}

wxColour wmLineWnd::GetColour(const wxString& sName)
{
    map<wxString, wmLine>::iterator itLine = m_mLinePercentages.find(sName);
    if(itLine != m_mLinePercentages.end())
    {
        return itLine->second.clr;
    }
    return wxColour();
}

int wmLineWnd::GetWidth(const wxString& sName)
{
    map<wxString, wmLine>::iterator itLine = m_mLinePercentages.find(sName);
    if(itLine != m_mLinePercentages.end())
    {
        return itLine->second.nWidth;
    }
    return -1;
}

int wmLineWnd::GetPenStyle(const wxString& sName)
{
    map<wxString, wmLine>::iterator itLine = m_mLinePercentages.find(sName);
    if(itLine != m_mLinePercentages.end())
    {
        return itLine->second.nPenStyle;
    }
    return -1;
}

int wmLineWnd::GetStartArrow(const wxString& sName)
{
    map<wxString, wmLine>::iterator itLine = m_mLinePercentages.find(sName);
    if(itLine != m_mLinePercentages.end())
    {
        return itLine->second.nStartArrow;
    }
    return -1;
}

int wmLineWnd::GetEndArrow(const wxString& sName)
{
    map<wxString, wmLine>::iterator itLine = m_mLinePercentages.find(sName);
    if(itLine != m_mLinePercentages.end())
    {
        return itLine->second.nEndArrow;
    }
    return -1;
}

bool wmLineWnd::IsDrawn(const wxString& sName)
{
    map<wxString, wmLine>::iterator itLine = m_mLinePercentages.find(sName);
    if(itLine != m_mLinePercentages.end())
    {
        return itLine->second.bDraw;
    }
    return false;
}

int wmLineWnd::GetArrowSize(const wxString& sName)
{
    map<wxString, wmLine>::iterator itLine = m_mLinePercentages.find(sName);
    if(itLine != m_mLinePercentages.end())
    {
        return itLine->second.nArrowSize;
    }
    return -1;
}

bool wmLineWnd::LineExists(const wxString& sName)
{
    map<wxString, wmLine>::iterator itLine = m_mLinePercentages.find(sName);
    return (itLine != m_mLinePercentages.end());
}

mLines_t::iterator wmLineWnd::GetLinesBegin()
{
    return m_mLinePercentages.begin();
}

mLines_t::iterator wmLineWnd::GetLinesEnd()
{
    return m_mLinePercentages.end();
}
