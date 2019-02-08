#include "senderbuttonfactory.h"
#include <set>
#include <wx/log.h>

using namespace std;


const wxColour uiSender::CLR_STATE[5] = { wxColour(128,128,128), wxColour(255,255,0), wxColour(100,150,100), wxColour(100,255,100), wxColour(255,128,0)};
const wxString uiSender::STR_STATE[5] = { wxEmptyString, wxT("Asked"), wxT("Replied"), wxT("Active"), wxT("Error") };

uiSender::uiSender(const wxRect& rect, unsigned int nGradient) : uiRect(rect, nGradient)
{
}

void uiSender::Draw(wxDC& dc, const wxString& sLabel, unsigned short nState, const wxBitmap& bmp)
{
    Draw(dc,nState);
}

void uiSender::Draw(wxDC& dc, unsigned short nState)
{
    m_uiSender.Draw(dc, m_sLabel, nState);
    m_uiState.Draw(dc, nState);
}

void uiSender::RectSet()
{
    m_uiState.SetTextAlign(wxALIGN_CENTER);
    m_uiSender.SetTextAlign(wxALIGN_CENTER);
    m_uiState.SetRect(m_rectEnclosing.GetLeft(), m_rectEnclosing.GetTop(), m_rectEnclosing.GetWidth(), (40*m_rectEnclosing.GetHeight())/100);
    m_uiSender.SetRect(m_rectEnclosing.GetLeft(), m_uiState.GetBottom(), m_rectEnclosing.GetWidth(), m_rectEnclosing.GetHeight()-m_uiState.GetHeight());

    m_uiState.SetForegroundColour(*wxBLACK);
}

void uiSender::SetState(enumState eState)
{
    m_uiState.SetBackgroundColour(CLR_STATE[eState]);
    m_uiState.SetLabel(STR_STATE[eState]);

    if(eState == STATE_ACTIVE)
    {
        m_uiSender.SetBackgroundColour(wxColour(0,128,0));
    }
    else
    {
        m_uiSender.SetBackgroundColour(wxColour(128,128,128));
    }

}
