#include "releaseelement.h"
#include <wx/dc.h>
#include <wx/log.h>
#include "log.h"
#include <wx/tokenzr.h>


ReleaseElement::ReleaseElement(wxDC& dc, unsigned int nWidth, const wxString& sMessage) : m_nHeader(0)
{
    wxRect rect(0,0,0,0);

    //We use # ## ### etc for headers

    size_t nPos = 0;
    for(; nPos < sMessage.length(); nPos++)
    {
        if(sMessage[nPos] != '#')
        {
            break;
        }
        else
        {
            m_nHeader++;
        }
    }

    if(nPos != 0)
    {
        sMessage = sMessage.Mid(nPos);
    }
    sMessage.Trim().Trim(false);


    m_nHeight = 0;

    dc.SetFont(GetFont(dc));

    wxSize sz = uiRect::GetSizeOfText(dc, sMessage, wxRect(0,0,nWidth, 20), true);
    if(sz.y > 25)
    {
        sz.y+=20;
    }
    else
    {
        sz.y+=2;
    }
    CreateHitRect(sz.y, sMessage);

    m_nHeight = sz.y;

    SetMinSize(nWidth, 1);
    SetMaxSize(nWidth, m_nHeight);

    m_rectEnclosing = wxRect(0, 0, nWidth, m_nHeight);

}

wxFont ReleaseElement::GetFont(wxDC& dc)
{
    auto fnt = dc.GetFont();
    if(m_nHeader != 0)
    {
        fnt.SetPointSize(fnt.GetPointSize()+(8-m_nHeader));
    }
    return fnt;
}



void ReleaseElement::Destroy()
{

}

void ReleaseElement::SelectSubElement(size_t nSub)
{

}

void ReleaseElement::EnableSubElement(size_t nSub, bool bEnable)
{

}

bool ReleaseElement::SubElementDown(const wxPoint& pnt)
{
    return false;
}

bool ReleaseElement::SubElementUp(const wxPoint& pnt, bool bIn)
{
    return false;
}


bool ReleaseElement::SubElementMove(const wxPoint& pnt)
{
    return true;
}

int ReleaseElement::SubElementHeld()
{
    return false;
}

void ReleaseElement::Draw(wxDC& dc, bool bSelected)
{
    dc.SetFont(GetFont(dc));

    for(auto pairRect : m_mHitRects)
    {
        pairRect.second.Draw(dc, uiRect::BORDER_NONE);
    }
}

void ReleaseElement::CreateHitRect(size_t nId, int nHeight, const wxString& sLine)
{
    auto& rect = m_mHitRects.insert(std::make_pair(nId, uiRect(wxRect(0,0,0,0)))).first->second;
    rect.SetGradient(0);

    if(m_nHeader == 1)
    {
        rect.SetBackgroundColour(wxColour(0,50,150));
        rect.SetForegroundColour(*wxWHITE);
    }
    else if(m_nHeader == 2)
    {
        rect.SetBackgroundColour(wxColour(0,50,150));
        rect.SetForegroundColour(*wxWHITE);
    }
    else
    {
        rect.SetBackgroundColour(*wxWHITE);
        rect.SetForegroundColour(*wxBLACK);
    }
    rect.SetWidth(m_rectEnclosing.GetWidth());
    rect.SetTextAlign(wxALIGN_CENTER_VERTICAL | wxALIGN_LEFT);
    rect.SetHeight(nHeight);
    rect.SetLabel(sLine);
}

void ReleaseElement::ElementMoved()
{
    m_mHitRects[0].SetRect(m_rectEnclosing);
}

