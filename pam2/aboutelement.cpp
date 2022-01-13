#include "AboutElement.h"
#include <wx/dc.h>
#include <wx/log.h>
#include "log.h"
#include <wx/tokenzr.h>

const wxColour AboutElement::CLR_LABEL = wxColour("#49bbf4");
const wxColour AboutElement::CLR_DATA = *wxBLACK;
const wxColour AboutElement::CLR_BACK = *wxWHITE;

AboutElement::AboutElement(wxDC& dc, unsigned int nWidth,const wxString& sName, const wxString& sVersion, const wxString& sDate, const wxString& sTag, const wxString& sBranch)
{
    wxRect rect(0,0,0,0);

    m_rectEnclosing.SetWidth(nWidth);
    auto rectTitle = m_mHitRects.insert({ID_NAME, uiRect(wxRect(0,0,nWidth/2,HEIGHT_TITLE)}).first->second;
    rectTitle.SetLabel(sName);
    rectTitle.SetGradient(0);
    rectTitle.SetTextAlign(wxALIGN_CENTER_VERTICAL | wxALIGN_CENTER_HORIZONTAL);
    rectTitle.SetBackgroundColour(CLR_BACK);
    rectTitle.SetForegroundColour(CLR_LABEL);

    auto rectVersion = m_mHitRects.insert({ID_VERSION, uiRect(wxRect(rectTitle.GetRight(),0,rectTitle.GetWidth(),rectTitle.GetHeight())}).first->second;
    rectVersion.SetLabel(sVersion);
    rectVersion.SetGradient(0);
    rectVersion.SetTextAlign(wxALIGN_CENTER_VERTICAL | wxALIGN_CENTER_HORIZONTAL);
    rectVersion.SetBackgroundColour(CLR_BACK);
    rectVersion.SetForegroundColour(CLR_DATA);

    auto rectDateLabel = m_mHitRects.insert({ID_DATE_LABEL, uiRect(wxRect(0,rectTitle.GetBottom(),nWidth/6,HEIGHT_REST)}).first->second;
    rectDateLabel.SetLabel("Date:");
    rectDateLabel.SetGradient(0);
    rectDateLabel.SetTextAlign(wxALIGN_CENTER_VERTICAL | wxALIGN_RIGHT);
    rectDateLabel.SetBackgroundColour(CLR_BACK);
    rectDateLabel.SetForegroundColour(CLR_LABEL);

    auto rectDate = m_mHitRects.insert({ID_DATE, uiRect(wxRect(rectDateLabel.GetRight(),rectTitle.GetBottom(),nWidth-rectDateLabel.GetWidth(),HEIGHT_REST)}).first->second;
    rectDate.SetLabel(sDate);
    rectDate.SetGradient(0);
    rectDate.SetTextAlign(wxALIGN_CENTER_VERTICAL | wxALIGN_LEFT);
    rectDate.SetBackgroundColour(CLR_BACK);
    rectDate.SetForegroundColour(CLR_DATA);


    auto rectTagLabel = m_mHitRects.insert({ID_TAG_LABEL, uiRect(wxRect(0,rectDateLabel.GetBottom(),nWidth/6,HEIGHT_REST)}).first->second;
    rectTagLabel.SetLabel("Tag:");
    rectTagLabel.SetGradient(0);
    rectTagLabel.SetTextAlign(wxALIGN_CENTER_VERTICAL | wxALIGN_RIGHT);
    rectTagLabel.SetBackgroundColour(CLR_BACK);
    rectTagLabel.SetForegroundColour(CLR_LABEL);

    auto rectTag = m_mHitRects.insert({ID_TAG, uiRect(wxRect(rectTagLabel.GetRight(),rectDateLabel.GetBottom(),(nWidth/2)-rectDateLabel.GetWidth(),HEIGHT_REST)}).first->second;
    rectTag.SetLabel(sTag);
    rectTag.SetGradient(0);
    rectTag.SetTextAlign(wxALIGN_CENTER_VERTICAL | wxALIGN_LEFT);
    rectTag.SetBackgroundColour(CLR_BACK);
    rectTag.SetForegroundColour(CLR_DATA);


    auto rectBranchLabel = m_mHitRects.insert({ID_TAG_LABEL, uiRect(wxRect(rectTag.GetRight(),rectDateLabel.GetBottom(),nWidth/6,HEIGHT_REST)}).first->second;
    rectBranchLabel.SetLabel("Branch:");
    rectBranchLabel.SetGradient(0);
    rectBranchLabel.SetTextAlign(wxALIGN_CENTER_VERTICAL | wxALIGN_RIGHT);
    rectBranchLabel.SetBackgroundColour(CLR_BACK);
    rectBranchLabel.SetForegroundColour(CLR_LABEL);

    auto rectTag = m_mHitRects.insert({ID_TAG, uiRect(wxRect(rectBranchLabel.GetRight(),rectDateLabel.GetBottom(),nWidth-rectBranchLabel.GetRight(),HEIGHT_REST))}).first->second;
    rectTag.SetLabel(sBranch);
    rectTag.SetGradient(0);
    rectTag.SetTextAlign(wxALIGN_CENTER_VERTICAL | wxALIGN_LEFT);
    rectTag.SetBackgroundColour(CLR_BACK);
    rectTag.SetForegroundColour(CLR_DATA);

    SetMinSize(nWidth, 1);
    SetMaxSize(nWidth, 70);

    m_rectEnclosing = wxRect(0, 0, nWidth, 70);
    auto rectEnclosing = m_mHitRects.insert({ID_ENCLOSING, uiRect(m_rectEnclosing)}).first->second;
    rectEnclosing.SetBackgroundColour(CLR_BACK);

}



void AboutElement::Destroy()
{

}

void AboutElement::SelectSubElement(size_t nSub)
{

}

void AboutElement::EnableSubElement(size_t nSub, bool bEnable)
{

}

bool AboutElement::SubElementDown(const wxPoint& pnt)
{
    return false;
}

bool AboutElement::SubElementUp(const wxPoint& pnt, bool bIn)
{
    return false;
}


bool AboutElement::SubElementMove(const wxPoint& pnt)
{
    return true;
}

int AboutElement::SubElementHeld()
{
    return false;
}

void AboutElement::Draw(wxDC& dc, bool bSelected)
{
    m_mHitRects[ID_ENCLOSING].Draw(dc, uiRect::BORDER_FLAT);

    wxFont fnt = dc.GetFont();
    wxFont fntTitle = fnt;
    fntTitle.SetPixelSize(fnt.GetPixelSize()*2);
    dc.SetFont(fntTitle);
    m_mHitRects[ID_TITLE].Draw(dc, uiRect::BORDER_NONE);
    m_mHitRects[ID_VERSION].Draw(dc, uiRect::BORDER_NONE);

    dc.SetFont(fnt);
    m_mHitRects[ID_DATE_LABEL].Draw(dc, uiRect::BORDER_NONE);
    m_mHitRects[ID_DATE].Draw(dc, uiRect::BORDER_NONE);
    m_mHitRects[ID_TAG_LABEL].Draw(dc, uiRect::BORDER_NONE);
    m_mHitRects[ID_TAG].Draw(dc, uiRect::BORDER_NONE);
    m_mHitRects[ID_BRANCH_LABEL].Draw(dc, uiRect::BORDER_NONE);
    m_mHitRects[ID_BRANCH].Draw(dc, uiRect::BORDER_NONE);

}

void AboutElement::ElementMoved()
{
    m_mHitRects[ID_TITLE].SetRect(m_rectEnclosing.GetLeft(), m_rectEnclosing.GetTop(), m_rectEnclosing.GetWidth()/2, HEIGHT_TITLE);
    m_mHitRects[ID_VERSION].SetRect(m_mHitRects[ID_TITLE].GetRight(), m_rectEnclosing.GetTop(), m_rectEnclosing.GetWidth()/2, HEIGHT_TITLE);
    m_mHitRects[ID_DATE_LABEL].SetRect(0, m_mHitRects[ID_TITLE].GetBottom(), m_rectEnclosing.GetWidth()/6, HEIGHT_REST);
    m_mHitRects[ID_DATE].SetRect(m_mHitRects[ID_DATE_LABEL].GetRight(), m_mHitRects[ID_TITLE].GetBottom(), m_rectEnclosing.GetWidth()-m_mHitRects[ID_DATE_LABEL].GetRight(), HEIGHT_REST);

    m_mHitRects[ID_TAG_LABEL].SetRect(0, m_mHitRects[ID_DATE_LABEL].GetBottom(), m_rectEnclosing.GetWidth()/6, HEIGHT_REST);
    m_mHitRects[ID_TAG].SetRect(m_mHitRects[ID_TAG_LABEL].GetRight(), m_mHitRects[ID_DATE_LABEL].GetBottom(), m_rectEnclosing.GetWidth()/2-m_mHitRects[ID_TAG_LABEL].GetRight(), HEIGHT_REST);

    m_mHitRects[ID_BRANCH_LABEL].SetRect((m_mHitRects[ID_TAG].GetRight(), m_mHitRects[ID_DATE_LABEL].GetBottom(), m_rectEnclosing.GetWidth()/6, HEIGHT_REST);
    m_mHitRects[ID_BRANCH].SetRect(m_mHitRects[ID_BRANCH_LABEL].GetRight(), m_mHitRects[ID_DATE_LABEL].GetBottom(), m_rectEnclosing.GetWidth()-m_mHitRects[ID_BRANCH_LABEL].GetRight(), HEIGHT_REST);

    m_mHitRects[ID_ENCLOSING].SetRect(m_rectEnclosing);
}

