#include "logelement.h"
#include <wx/dc.h>
#include <wx/log.h>

LogElement::LogElement(wxDC& dc, unsigned int nWidth, const wxString& sMessage, bool bTest) : m_bTest(bTest)
{
    wxSize sz = uiRect::GetSizeOfText(dc, sMessage, wxRect(0,0,nWidth-COLUMN_TIME, 25));

    m_nHeight = std::max(sz.y, 30);

    CreateHitRects();
    m_mHitRects[1].SetLabel(sMessage);

    SetMinSize(sz.x, m_nHeight);
    SetMaxSize(sz.x, m_nHeight);

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
    m_mHitRects[0].Draw(dc, uiRect::BORDER_NONE);
    m_mHitRects[1].Draw(dc, uiRect::BORDER_NONE);
}

void LogElement::CreateHitRects()
{
    wxRect rect(0,0,0,0);

    m_mHitRects.clear();
    m_mHitRects.insert(std::make_pair(0, uiRect(rect,0)));
    m_mHitRects.insert(std::make_pair(1, uiRect(rect,0)));

    m_mHitRects[0].SetLabel(wxDateTime::UNow().Format(wxT("%H:%M:%S:%l")));
    m_mHitRects[0].SetTextAlign(wxALIGN_CENTER_VERTICAL | wxALIGN_LEFT);

    if(!m_bTest)
    {
        m_mHitRects[0].SetBackgroundColour(*wxBLACK);
        m_mHitRects[1].SetBackgroundColour(*wxBLACK);
    }
    else
    {
        m_mHitRects[0].SetBackgroundColour(wxColour(255,100,100));
        m_mHitRects[1].SetBackgroundColour(wxColour(255,100,100));
    }
    m_mHitRects[1].SetWidth(m_rectEnclosing.GetWidth()-COLUMN_TIME);

    m_mHitRects[1].SetTextAlign(wxALIGN_CENTER_VERTICAL | wxALIGN_LEFT);


}

void LogElement::ElementMoved()
{
    m_mHitRects[0].SetRect(m_rectEnclosing.GetLeft(), m_rectEnclosing.GetTop(), COLUMN_TIME, m_nHeight);
    m_mHitRects[1].SetRect(m_mHitRects[0].GetRight()+2, m_rectEnclosing.GetTop(), m_rectEnclosing.GetRight()-m_mHitRects[0].GetRight()-2, m_nHeight);
}


