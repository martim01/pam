#include "sdpelement.h"
#include <wx/dc.h>
#include <wx/log.h>
#include <wx/dc.h>
#include <wx/tokenzr.h>
#include <iostream>


SdpElement::SdpElement(wxDC& dc, unsigned int nWidth, wxString sMessage)
{
    wxRect rect(0,0,0,0);

    m_rectEnclosing.SetWidth(nWidth);

    m_nHeight = 0;
    sMessage.Trim();
    wxSize sz = uiRect::GetSizeOfText(dc, sMessage, wxRect(0,0,nWidth, 20), true);
    CreateHitRect(0, sz.y+2, sMessage);
    m_nHeight += sz.y+2;

    SetMinSize(nWidth, -1);
    SetMaxSize(nWidth, m_nHeight);

    m_rectEnclosing = wxRect(0, 0, nWidth, m_nHeight);

}



void SdpElement::Destroy()
{

}

void SdpElement::SelectSubElement(size_t nSub)
{

}

void SdpElement::EnableSubElement(size_t nSub, bool bEnable)
{

}

bool SdpElement::SubElementDown(const wxPoint& pnt)
{
    return false;
}

bool SdpElement::SubElementUp(const wxPoint& pnt, bool bIn)
{
    return false;
}


bool SdpElement::SubElementMove(const wxPoint& pnt)
{
    return true;
}

int SdpElement::SubElementHeld()
{
    return false;
}

void SdpElement::Draw(wxDC& dc, bool bSelected)
{
    for(auto pairRect : m_mHitRects)
    {
        pairRect.second.Draw(dc, uiRect::BORDER_NONE);
    }
}

void SdpElement::CreateHitRect(size_t nId, int nHeight, const wxString& sLine)
{
    auto& rect = m_mHitRects.insert(std::make_pair(nId, uiRect(wxRect(0,0,0,0)))).first->second;
    rect.SetGradient(0);

    rect.SetBackgroundColour(*wxBLACK);
    rect.SetForegroundColour(*wxWHITE);

    rect.SetWidth(m_rectEnclosing.GetWidth());
    rect.SetTextAlign(wxALIGN_CENTER_VERTICAL | wxALIGN_LEFT);
    rect.SetHeight(nHeight);
    rect.SetLabel(sLine);
}

void SdpElement::ElementMoved()
{
    int nTop = m_rectEnclosing.GetTop();
    for(auto& pairRect : m_mHitRects)
    {
        pairRect.second.SetRect(m_rectEnclosing.GetLeft()+2, nTop, pairRect.second.GetWidth(), pairRect.second.GetHeight());
        nTop = pairRect.second.GetBottom()+1;
    }
}


