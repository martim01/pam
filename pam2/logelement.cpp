#include "logelement.h"
#include <wx/dc.h>
#include <wx/log.h>
#include "log.h"
#include <wx/tokenzr.h>


LogElement::LogElement(wxDC& dc, unsigned int nWidth, const wxString& sMessage, int nLevel) : m_nLevel(nLevel)
{
    wxRect rect(0,0,0,0);
    auto& rectTime = m_mHitRects.insert(std::make_pair(0, uiRect(rect,0))).first->second;
    rectTime.SetLabel(wxDateTime::UNow().Format(wxT("%H:%M:%S:%l")));
    rectTime.SetTextAlign(wxALIGN_CENTER_VERTICAL | wxALIGN_LEFT);
    rectTime.SetBackgroundColour(*wxBLACK);
    rectTime.SetForegroundColour(*wxWHITE);

    m_rectEnclosing.SetWidth(nWidth);

    m_nHeight = 0;
    wxArrayString asLines(wxStringTokenize(sMessage, "\n"));
    for(size_t i = 0; i < asLines.GetCount(); i++)
    {
        asLines[i].Trim();
        wxSize sz = uiRect::GetSizeOfText(dc, asLines[i], wxRect(0,0,nWidth-COLUMN_TIME, 20), true);
        if(sz.y > 25)
        {
            sz.y+=20;
        }
        else
        {
            sz.y+=2;
        }
        CreateHitRect(i+1, sz.y, asLines[i]);
        m_nHeight += sz.y;
    }
   // m_nHeight = std::max(m_nHeight, (unsigned long)30);

    rectTime.SetHeight(m_nHeight);

    SetMinSize(nWidth, 1);
    SetMaxSize(nWidth, m_nHeight);

    m_rectEnclosing = wxRect(0, 0, nWidth, m_nHeight);

}



void LogElement::Destroy()
{

}

void LogElement::SelectSubElement(size_t nSub)
{

}

void LogElement::EnableSubElement(size_t nSub, bool bEnable)
{

}

bool LogElement::SubElementDown(const wxPoint& pnt)
{
    return false;
}

bool LogElement::SubElementUp(const wxPoint& pnt, bool bIn)
{
    return false;
}


bool LogElement::SubElementMove(const wxPoint& pnt)
{
    return true;
}

int LogElement::SubElementHeld()
{
    return false;
}

void LogElement::Draw(wxDC& dc, bool bSelected)
{
    for(auto pairRect : m_mHitRects)
    {
        pairRect.second.Draw(dc, uiRect::BORDER_NONE);
    }
}

void LogElement::CreateHitRect(size_t nId, int nHeight, const wxString& sLine)
{
    auto& rect = m_mHitRects.insert(std::make_pair(nId, uiRect(wxRect(0,0,0,0)))).first->second;
    rect.SetGradient(0);

    switch(m_nLevel)
    {
        case pml::LOG_TRACE:
            rect.SetBackgroundColour(*wxBLACK);
            rect.SetForegroundColour(wxColour(100,100,100));
            break;
        case pml::LOG_DEBUG:
            rect.SetBackgroundColour(*wxBLACK);
            rect.SetForegroundColour(wxColour(150,150,150));
            break;
        case pml::LOG_INFO:
            rect.SetBackgroundColour(*wxBLACK);
            rect.SetForegroundColour(*wxWHITE);
            break;
        case pml::LOG_WARN:
            rect.SetBackgroundColour(wxColour(255,100,50));
            rect.SetForegroundColour(*wxBLACK);
            break;
        case pml::LOG_ERROR:
            rect.SetBackgroundColour(wxColour(255,50,50));
            rect.SetForegroundColour(*wxBLACK);
            break;
        case pml::LOG_CRITICAL:
            rect.SetBackgroundColour(wxColour(255,0,0));
            rect.SetForegroundColour(*wxWHITE);
            break;
    }
    rect.SetWidth(m_rectEnclosing.GetWidth()-COLUMN_TIME);
    rect.SetTextAlign(wxALIGN_CENTER_VERTICAL | wxALIGN_LEFT);
    rect.SetHeight(nHeight);
    rect.SetLabel(sLine);

}

void LogElement::ElementMoved()
{
    m_mHitRects[0].SetRect(m_rectEnclosing.GetLeft(), m_rectEnclosing.GetTop(), COLUMN_TIME, m_nHeight);
    int nLeft, nTop;
    for(auto& pairRect : m_mHitRects)
    {
        if(pairRect.first == 0)
        {
            pairRect.second.SetRect(m_rectEnclosing.GetLeft(), m_rectEnclosing.GetTop(), COLUMN_TIME, m_nHeight);
            nLeft = pairRect.second.GetRight();
            nTop = m_rectEnclosing.GetTop();
        }
        else
        {
            pairRect.second.SetRect(nLeft+2, nTop, pairRect.second.GetWidth(), pairRect.second.GetHeight());
            nTop = pairRect.second.GetBottom()+1;
        }
    }
}

