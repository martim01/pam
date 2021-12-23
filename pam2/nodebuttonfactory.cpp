#include "nodebuttonfactory.h"
#include <set>
#include <wx/log.h>

using namespace std;


const wxColour uiNode::CLR_BAD          = wxColour(250,50,50);
const wxColour uiNode::CLR_REG_BACK[4] = { wxColour(180,180,200),wxColour(50,250,50),wxColour(50,250,50), wxColour(50,250,50) };
const wxColour uiNode::CLR_REG_TEXT[4]  = { *wxBLACK, wxBLACK,*wxWHITE,*wxRED};


uiNode::uiNode(const wxRect& rect, unsigned int nGradient) : uiRect(rect, nGradient), m_bOK(true), m_eState(REG_NONE)
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

void uiNode::SetRegisterState(enumRegState eState)
{
   m_eState = eState;
   SetColour();
}

void uiNode::SetColour()
{
    wxColour clrBack(CLR_BAD);
    wxColour clrText(*wxBLACK);
    if(m_bOK)
    {
        clrBack = CLR_REG_BACK[m_eState];
        clrText = CLR_REG_TEXT[m_eState];
    }

    m_uiUrl.SetBackgroundColour(clrBack);
    m_uiUrl.SetForegroundColour(clrText);
    m_uiPriority.SetBackgroundColour(clrBack);
    m_uiPriority.SetForegroundColour(clrText);
    m_uiVersion.SetBackgroundColour(clrBack);
    m_uiVersion.SetForegroundColour(clrText);
}

void uiNode::SetLabel(const wxString& sLabel)
{
    m_uiUrl.SetLabel(sLabel);

}

void uiNode::SetPriority(unsigned short nPriority)
{
    m_uiPriority.SetLabel(wxString::Format("%u", nPriority));
}

#ifdef __NMOS__
void uiNode::SetVersion(const pml::nmos::ApiVersion& version)
{
    m_uiVersion.SetLabel(version.GetVersionAsString());
}
#endif
const wxString& uiNode::GetLabel() const
{
    return m_uiUrl.GetLabel();
}
