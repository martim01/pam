#include "releaseelement.h"
#include <wx/dc.h>
#include <wx/log.h>

#include <wx/tokenzr.h>


ReleaseElement::ReleaseElement(wxDC& dc, unsigned int nWidth, wxString sMessage) : m_nHeader(0)
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

    wxFont fnt = dc.GetFont();
    dc.SetFont(GetFont(dc));

    wxSize sz = uiRect::GetSizeOfText(dc, sMessage, wxRect(0,0,nWidth, 20), true);
    sz.y+=2;

    dc.SetFont(fnt);

    CreateHitRect(sz.y, sMessage);

    m_nHeight = sz.y;

    SetMinSize(nWidth, 1);
    SetMaxSize(nWidth, m_nHeight);

    m_rectEnclosing = wxRect(0, 0, nWidth, m_nHeight);

}

wxFont ReleaseElement::GetFont(wxDC& dc)
{
    auto fnt = dc.GetFont();

    switch(m_nHeader)
    {
        case 1:
            fnt.SetPixelSize(fnt.GetPixelSize()*2);
            fnt.SetWeight(wxFONTWEIGHT_BOLD);
            fnt.SetUnderlined(false);
            break;
        case 2:
            fnt.SetPixelSize(fnt.GetPixelSize()*1.5);
            fnt.SetWeight(wxFONTWEIGHT_BOLD);
            fnt.SetUnderlined(false);
            break;
        case 3:
            fnt.SetPixelSize(fnt.GetPixelSize()*1.17);
            fnt.SetWeight(wxFONTWEIGHT_BOLD);
            fnt.SetUnderlined(true);
            break;
        case 4:
            fnt.SetPixelSize(fnt.GetPixelSize());
            fnt.SetWeight(wxFONTWEIGHT_BOLD);
            fnt.SetUnderlined(true);
            break;
        case 5:
            fnt.SetPixelSize(fnt.GetPixelSize()*0.83);
            fnt.SetWeight(wxFONTWEIGHT_BOLD);
            fnt.SetUnderlined(false);
            break;
        case 6:
            fnt.SetPixelSize(fnt.GetPixelSize()*.67);
            fnt.SetWeight(wxFONTWEIGHT_BOLD);
            fnt.SetUnderlined(false);
            break;
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
    wxFont fnt = dc.GetFont();
    dc.SetFont(GetFont(dc));

    for(auto pairRect : m_mHitRects)
    {
        pairRect.second.Draw(dc, uiRect::BORDER_NONE);
    }
    dc.SetFont(fnt);
}

void ReleaseElement::CreateHitRect(int nHeight, const wxString& sLine)
{
    auto& rect = m_mHitRects.insert(std::make_pair(0, uiRect(wxRect(0,0,0,0)))).first->second;
    rect.SetGradient(0);
    rect.SetBackgroundColour(*wxWHITE);

    if(m_nHeader == 1)
    {
        rect.SetForegroundColour(wxColour(255, 166, 99));
    }
    else if(m_nHeader == 2)
    {
        rect.SetForegroundColour(wxColour(73,187,244));
    }
    else
    {
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

