#include "nodebuttonfactory.h"
#include <set>
#include <wx/log.h>

using namespace std;


const wxColour uiNode::CLR_BAD      = wxColour(150,50,50);
const wxColour uiNode::CLR_OK       = wxColour(50,50,150);
const wxColour uiNode::CLR_SELECTED = wxColour(50,150,50);


uiNode::uiNode(const wxRect& rect, unsigned int nGradient) : uiRect(rect, nGradient), m_bOK(true), m_bSelected(false)
{
    SetColour();
}

void uiNode::Draw(wxDC& dc, const wxString& sLabel, unsigned short nState, const wxBitmap& bmp)
{

    //uiRect::Draw(dc, sLabel, nState);
    m_uiUrl.Draw(dc, uiRect::BORDER_NONE);
    m_uiPriority.Draw(dc, uiRect::BORDER_NONE);
    m_uiVersion.Draw(dc, uiRect::BORDER_NONE);
}

void uiNode::Draw(wxDC& dc, unsigned short nState)
{
    Draw(dc,"",nState);
}

void uiNode::RectSet()
{
    m_uiUrl.SetTextAlign(wxALIGN_LEFT);
    m_uiPriority.SetTextAlign(wxALIGN_LEFT);
    m_uiVersion.SetTextAlign(wxALIGN_RIGHT);


    m_uiUrl.SetRect(m_rectEnclosing.GetLeft(), m_rectEnclosing.GetTop(), (7*m_rectEnclosing.GetWidth())/10, m_rectEnclosing.GetHeight());
    m_uiPriority.SetRect(m_uiUrl.GetRight(), m_rectEnclosing.GetTop(), m_rectEnclosing.GetWidth()/10, m_rectEnclosing.GetHeight());
    m_uiVersion.SetRect(m_uiPriority.GetRight(), m_rectEnclosing.GetTop(), m_rectEnclosing.GetWidth()-m_uiPriority.GetRight(), m_rectEnclosing.GetHeight());


}

void uiNode::SetOK(bool bOK)
{
    m_bOK = bOK;
    SetColour();
}

void uiNode::Select(bool bSelected)
{
   m_bSelected = bSelected;
   SetColour();
}

void uiNode::SetColour()
{
    wxColour clrBack(CLR_BAD);
    if(m_bOK)
    {
        if(m_bSelected)
        {
            clrBack = CLR_SELECTED;
        }
        else
        {
            clrBack = CLR_OK;
        }
    }

    m_uiUrl.SetBackgroundColour(clrBack);
    m_uiPriority.SetBackgroundColour(clrBack);
    m_uiVersion.SetBackgroundColour(clrBack);
}

void uiNode::SetLabel(const wxString& sLabel)
{
    m_uiUrl.SetLabel(sLabel);

}

void uiNode::SetPriority(unsigned short nPriority)
{
    m_uiPriority.SetLabel(wxString::Format("%u", nPriority));
}

void uiNode::SetVersion(const pml::nmos::ApiVersion& version)
{
    m_uiVersion.SetLabel(version.GetVersionAsString());
}

const wxString& uiNode::GetLabel() const
{
    return m_uiUrl.GetLabel();
}
