#include "uirect.h"
#include <wx/log.h>
#include <wx/tokenzr.h>
#include <algorithm>
#include <iostream>


using namespace std;

uiRect::uiRect(const wxRect& rect, unsigned int nGradient) : m_rectEnclosing(rect),
m_clrBackgroundHigh(wxColour(150,150,150)),
m_clrBackgroundLow(wxColour(120,120,120)),
m_clrBorderHigh(wxColour(190,190,190)),
m_clrBorderLow(wxColour(100,100,100)),
m_clrText(*wxWHITE),
m_nGradient(nGradient),
m_nAlign(wxALIGN_CENTER),
m_nBitmapAlign(wxALIGN_CENTER),
m_pData(nullptr),
m_nData(0),
m_nIndex(0),
m_bClip(true),
m_bWrap(true)
{
    SetTextRect();
    //RectSet();
}

uiRect::~uiRect()
{

}

void uiRect::SetRect(const wxRect& rect)
{
    m_rectEnclosing = rect;
    SetTextRect();
    RectSet();
}

void uiRect::SetRect(int nLeft, int nTop, int nWidth, int nHeight)
{
    m_rectEnclosing = wxRect(nLeft, nTop, nWidth, nHeight);
    SetTextRect();
    RectSet();
}

void uiRect::SetTextRect()
{
    m_rectText = m_rectEnclosing;
    int nX, nY;
    if(m_rectText.GetHeight() > 30)
    {
        nY = 3;
    }
    else
    {
        nY = 1;
    }
    if(m_rectText.GetWidth() > 80)
    {
        nX = 3;
    }
    else
    {
        nX = 1;
    }
    m_rectText.Deflate(nX, nY);
}

void uiRect::Pad(unsigned long nHorizontal, unsigned long nVertical)
{
    m_rectText.Deflate(nHorizontal, nVertical);
}

void uiRect::SetTextAlign(unsigned int nAlign)
{
    m_nAlign = nAlign;
}

void uiRect::SetBitmapAlign(unsigned int nAlign)
{
    m_nBitmapAlign = nAlign;
}

void uiRect::SetBitmap(const wxBitmap& bmp)
{
    m_bmp = bmp;
}

const wxBitmap& uiRect::GetBitmap() const
{
    return m_bmp;
}

void uiRect::SetBackgroundColour(const wxColour &colour, const wxColour& clrLow)
{
    if(colour.IsOk())
    {
        m_clrBackgroundHigh = colour;
    }
    else if(clrLow.IsOk())
    {
        m_clrBackgroundHigh = clrLow;

    }
    if(m_clrBackgroundHigh.IsOk() == false)
    {
        return;
    }
    if(clrLow.IsOk() == false)
    {
        m_clrBackgroundLow = wxColour(max(0, (int)m_clrBackgroundHigh.Red()-20),
                                    max(0, (int)m_clrBackgroundHigh.Green()-20),
                                    max(0, (int)m_clrBackgroundHigh.Blue()-20));
    }
    else
    {

        m_clrBackgroundLow = clrLow;
    }

    if(m_clrBackgroundLow.IsOk() == false)
    {
        return;
    }
    m_clrBorderHigh = wxColour(min(255, (int)m_clrBackgroundHigh.Red()+40),
                               min(255, (int)m_clrBackgroundHigh.Green()+40),
                               min(255, (int)m_clrBackgroundHigh.Blue()+40));
    m_clrBorderLow = wxColour(max(0, (int)m_clrBackgroundLow.Red()-20),
                                  max(0, (int)m_clrBackgroundLow.Green()-20),
                                  max(0, (int)m_clrBackgroundLow.Blue()-20));
}

void uiRect::SetForegroundColour(const wxColour &colour)
{
    m_clrText = colour;
}

void uiRect::Draw(wxDC& dc, unsigned short nState, unsigned char nEdge)
{
    Draw(dc, m_sLabel, nState, nEdge, m_bmp);
    dc.SetPen(wxNullPen);
    dc.SetBrush(wxNullBrush);
}

void uiRect::Draw(wxDC& dc, const wxString& sLabel, unsigned short nState, unsigned char nEdge, const wxBitmap& bmp)
{
    wxRect rectFill(m_rectEnclosing);
    
    if(m_nGradient != 0)
    {
        dc.GradientFillLinear(rectFill, m_clrBackgroundHigh, m_clrBackgroundLow, static_cast<wxDirection>(m_nGradient));
    }
    else
    {
        dc.SetPen(*wxTRANSPARENT_PEN);
        dc.SetBrush(wxBrush(m_clrBackgroundHigh));
        dc.DrawRectangle(rectFill);
        dc.SetPen(wxNullPen);
        dc.SetBrush(wxNullBrush);
    }

    switch(nState)
    {
    case BORDER_UP:
        DrawUp(dc, nEdge);
        break;
    case BORDER_DOWN:
        DrawDown(dc, nEdge);
        break;
    case BORDER_FLAT:
        DrawFlat(dc, nEdge);
    }


    wxColour clr(dc.GetTextForeground());
    dc.SetTextForeground(m_clrText);
    dc.SetBrush(*wxTRANSPARENT_BRUSH);


    if(bmp.IsOk())
    {
        int x,y;
        if((m_nBitmapAlign & wxALIGN_CENTER_HORIZONTAL))
        {
            x = m_rectText.GetLeft()+((m_rectText.GetWidth()-bmp.GetWidth())/2);
        }
        else if((m_nBitmapAlign & wxALIGN_RIGHT))
        {
            x = m_rectText.GetRight()-bmp.GetWidth();
        }
        else
        {
            x = m_rectText.GetLeft();
        }

        if((m_nBitmapAlign & wxALIGN_CENTER_VERTICAL))
        {
            y = m_rectText.GetTop()+((m_rectText.GetHeight()-bmp.GetHeight())/2);
        }
        else if((m_nBitmapAlign & wxALIGN_BOTTOM))
        {
            y = m_rectText.GetBottom()-bmp.GetHeight();
        }
        else
        {
            y = m_rectText.GetTop();
        }
        dc.DrawBitmap(bmp,x,y,true);
    }
    if(m_bClip)
    {
        dc.SetClippingRegion(m_rectText);
    }

    DrawText(dc, sLabel, m_rectText, m_nAlign, m_bWrap, true, true, m_bClip);

    if(m_bClip)
    {
        dc.DestroyClippingRegion();
    }
    dc.SetTextForeground(clr);

    dc.SetPen(wxNullPen);
    dc.SetBrush(wxNullBrush);
}

void uiRect::DrawUp(wxDC& dc, unsigned char nEdge)
{
    dc.SetPen(wxPen(m_clrBorderHigh));
    if((nEdge & EDGE_LEFT) != 0)
    {
        dc.DrawLine(m_rectEnclosing.GetBottomLeft(), m_rectEnclosing.GetTopLeft());
    }
    if((nEdge & EDGE_TOP) != 0)
    {
        dc.DrawLine(m_rectEnclosing.GetTopLeft(), m_rectEnclosing.GetTopRight());
    }
    dc.SetPen(wxPen(m_clrBorderLow));
    if((nEdge & EDGE_BOTTOM) != 0)
    {
        dc.DrawLine(m_rectEnclosing.GetBottomLeft(), m_rectEnclosing.GetBottomRight());
    }
    if((nEdge & EDGE_RIGHT) != 0)
    {
        dc.DrawLine(m_rectEnclosing.GetTopRight(), m_rectEnclosing.GetBottomRight());
    }
}

void uiRect::DrawDown(wxDC& dc, unsigned char nEdge)
{
    dc.SetPen(wxPen(m_clrBorderLow));
    if((nEdge & EDGE_LEFT) != 0)
    {
        dc.DrawLine(m_rectEnclosing.GetBottomLeft(), m_rectEnclosing.GetTopLeft());
    }
    if((nEdge & EDGE_TOP) != 0)
    {
        dc.DrawLine(m_rectEnclosing.GetTopLeft(), m_rectEnclosing.GetTopRight());
    }
    dc.SetPen(wxPen(m_clrBorderHigh));
    if((nEdge & EDGE_BOTTOM) != 0)
    {
        dc.DrawLine(m_rectEnclosing.GetBottomLeft(), m_rectEnclosing.GetBottomRight());
    }
    if((nEdge & EDGE_RIGHT) != 0)
    {
        dc.DrawLine(m_rectEnclosing.GetTopRight(), m_rectEnclosing.GetBottomRight());
    }

}

void uiRect::DrawFlat(wxDC& dc, unsigned char nEdge)
{
    dc.SetPen(wxPen(m_clrBorderLow));
    if((nEdge & EDGE_LEFT) != 0)
    {
        dc.DrawLine(m_rectEnclosing.GetBottomLeft(), m_rectEnclosing.GetTopLeft());
    }
    if((nEdge & EDGE_TOP) != 0)
    {
        dc.DrawLine(m_rectEnclosing.GetTopLeft(), m_rectEnclosing.GetTopRight());
    }
    if((nEdge & EDGE_BOTTOM) != 0)
    {
        dc.DrawLine(m_rectEnclosing.GetBottomLeft(), m_rectEnclosing.GetBottomRight());
    }
    if((nEdge & EDGE_RIGHT) != 0)
    {
        dc.DrawLine(m_rectEnclosing.GetTopRight(), m_rectEnclosing.GetBottomRight());
    }
}

void uiRect::ClipText(bool bClip)
{
    m_bClip = bClip;
}

void uiRect::SetWrap(bool bWrap)
{
    m_bWrap = bWrap;
}



wxSize uiRect::GetSizeOfText(wxDC& dc, const wxString& sText, const wxRect& rect, bool bWrap)
{
    return DrawText(dc,sText, rect, wxALIGN_TOP | wxALIGN_LEFT, bWrap, false, true, false);
}

wxSize uiRect::DrawText(wxDC& dc,const wxString& sText, const wxRect& rect, int nAlign, bool bWrap, bool bDraw, bool bClip, bool bClippingRegion)
{
    bWrap = true;

    if(bDraw && bClippingRegion)
        dc.SetClippingRegion(rect);

    wxString sOutput;
    wxSize szReturn;

    bool bOverflow(false);

    wxStringTokenizer stzLine(sText,"\r\n",wxTOKEN_RET_EMPTY_ALL);
    while(stzLine.HasMoreTokens() && !bOverflow)
    {
        wxString sLine(stzLine.GetNextToken());
        wxString sOutLine;
        if(sLine.empty())   //simply a \n
        {
            szReturn.y += dc.GetTextExtent(sText).GetHeight();
            szReturn.x = 0;
            if(bClip && szReturn.y > rect.GetHeight())
            {
            //    sOutLine = "...";
                bOverflow = true;
            }
        }
        else
        {
            if(bWrap)
            {
                wxStringTokenizer stz(sLine," ,.!{}[]",wxTOKEN_RET_DELIMS);
                szReturn.x = 0;
                int nToken(0);
                while(stz.HasMoreTokens())
                {
                    wxString sWord(stz.GetNextToken());
                    wxSize szExtent;
                    szExtent = dc.GetTextExtent(sWord);

                    szReturn.x += szExtent.GetWidth();

                    if(szReturn.x > rect.GetWidth() && nToken != 0)    //we don't move down one if we are the first word on the line
                    {
                        szReturn.y += dc.GetTextExtent(sOutLine).GetHeight();

                        sOutput += sOutLine;
                        sOutput += "\n";
                        sOutLine = "";

                        szReturn.x = dc.GetTextExtent(sWord).GetWidth();
                        nToken=0;
                    }

                    sOutLine += sWord;
                    nToken++;
                }
            }
            else
            {
                sOutLine = sLine;
            }

            szReturn.y += dc.GetTextExtent(sOutLine).GetHeight();
            if(bClip && szReturn.y > rect.GetHeight())
            {
                if(sOutput.empty() == false)
                {
                    if(sOutput.GetChar(sOutput.length()-1) == wxT('\n'))
                    {
                        sOutput = sOutput.Left(sOutput.length()-1);
                    }
                }
            }
        }
        sOutput += sOutLine;
        if(stzLine.HasMoreTokens()) //if more tokens then we need to add a \n
        {
            sOutput += "\n";
        }
    }



    if(bDraw)
    {
       // dc.DrawRectangle(rect);
        dc.DrawLabel(sOutput,rect,nAlign);
        if(bClippingRegion)
            dc.DestroyClippingRegion();
    }



    return szReturn;
}

bool uiRect::Contains(const wxPoint& pnt) const
{
    return m_rectEnclosing.Contains(pnt);
}

int uiRect::GetLeft() const
{
    return m_rectEnclosing.GetLeft();
}

int uiRect::GetRight() const
{
    return m_rectEnclosing.GetRight();
}

int uiRect::GetHeight() const
{
    return m_rectEnclosing.GetHeight();
}

int uiRect::GetWidth() const
{
    return m_rectEnclosing.GetWidth();
}

int uiRect::GetTop() const
{
    return m_rectEnclosing.GetTop();
}

int uiRect::GetBottom() const
{
    return m_rectEnclosing.GetBottom();
}

wxPoint uiRect::GetCenter() const
{
    return wxPoint(m_rectEnclosing.GetLeft()+ m_rectEnclosing.GetWidth()/2,
                   m_rectEnclosing.GetTop()+ m_rectEnclosing.GetHeight()/2);

}

void uiRect::Offset(int x, int y)
{
    m_rectEnclosing.Offset(x,y);
    SetTextRect();
    RectSet();
}

void uiRect::SetLeft(int n)
{
    m_rectEnclosing.SetLeft(n);
    SetTextRect();
    RectSet();
}

void uiRect::SetRight(int n)
{
    m_rectEnclosing.SetRight(n);
    SetTextRect();
    RectSet();
}

void uiRect::SetTop(int n)
{
    m_rectEnclosing.SetTop(n);
    SetTextRect();
    RectSet();
}

void uiRect::SetBottom(int n)
{
    m_rectEnclosing.SetBottom(n);
    SetTextRect();
    RectSet();
}

void uiRect::SetWidth(int n)
{
    m_rectEnclosing.SetWidth(n);
    SetTextRect();
    RectSet();
}

void uiRect::SetHeight(int n)
{
    m_rectEnclosing.SetHeight(n);
    SetTextRect();
    RectSet();
}

void uiRect::SetClientData(void* pData)
{
    m_pData = pData;
}
void* uiRect::GetClientData() const
{
    return m_pData;
}

void uiRect::SetLabel(const wxString& sLabel)
{
    m_sLabel = sLabel;
}

const wxString& uiRect::GetLabel() const
{
    return m_sLabel;
}

void uiRect::SetIntData(int nData)
{
    m_nData = nData;
}

int uiRect::GetIntData() const
{
    return m_nData;
}

void uiRect::SetIndex(size_t nIndex)
{
    m_nIndex = nIndex;
}

size_t uiRect::GetIndex() const
{
    return m_nIndex;
}


uiRect::uiRect(const uiRect& ui) :
m_rectEnclosing(ui.GetRect()),
m_rectText(ui.GetTextRect()),
m_clrText(ui.GetForegroundColour()),
m_nGradient(ui.GetGradient()),
m_nAlign(ui.GetTextAlign()),
m_nBitmapAlign(ui.GetBitmapAlign()),
m_pData(ui.GetClientData()),
m_nData(ui.GetIntData()),
m_sLabel(ui.GetLabel()),
m_bmp(ui.GetBitmap()),
m_nIndex(ui.GetIndex())
{
    SetBackgroundColour(ui.GetBackgroundColour());
}


wxSize uiRect::GetSize() const
{
    return m_rectEnclosing.GetSize();
}
