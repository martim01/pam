#include "wmlabel.h"
#include <wx/dcbuffer.h>
#include <wx/log.h>

using namespace std;

const int wmLabel::ID_TIMER_FLASH  = wxNewId();

BEGIN_EVENT_TABLE(wmLabel, pmControl)
    EVT_PAINT(wmLabel::OnPaint)
    EVT_SIZE(wmLabel::OnSize)
    EVT_TIMER(ID_TIMER_FLASH, wmLabel::OnFlash)
END_EVENT_TABLE()

#ifdef WXSPAM
IMPLEMENT_DYNAMIC_CLASS(wmLabel, pmControl)
#else
wxIMPLEMENT_DYNAMIC_CLASS(wmLabel, pmControl);
#endif


wmLabel::wmLabel() : pmControl(),
	m_nStyle(0),
	m_nHoldCount(0),
	m_nHoldTrigger(0),
	m_bFlashState(false),
	m_nBorderState(0),
	m_nState(0),
	m_bChecked(false),
	m_nBitmapAlign(0)
{

}


wmLabel::wmLabel(wxWindow *parent, wxWindowID id, const wxString& sLabel, const wxPoint& pos, const wxSize& size, long nStyle,const wxString& name) : pmControl(),
	m_nStyle(0),
	m_nHoldCount(0),
	m_nHoldTrigger(0),
	m_bFlashState(false),
	m_nBorderState(0),
	m_nState(0),
	m_bChecked(false),
	m_nBitmapAlign(0)
{
    Create(parent, id, sLabel, pos, size, nStyle, name);
}

bool wmLabel::Create(wxWindow *parent, wxWindowID id, const wxString& sLabel , const wxPoint& pos, const wxSize& size, long nStyle, const wxString& name)
{
    wxSize szInit(size);
    wxSize bestSize = DoGetBestSize();
    if(size.x<=0)
        szInit.SetWidth(bestSize.x);
    if(size.y <= 0)
        szInit.SetHeight(bestSize.y);


    if(!wxWindow::Create(parent,id,pos,szInit,wxWANTS_CHARS, wxEmptyString))
        return false;



    SetBackgroundStyle(wxBG_STYLE_CUSTOM);
    SetFont(wxFont(10,wxFONTFAMILY_SWISS,wxFONTSTYLE_NORMAL,wxFONTWEIGHT_NORMAL,false,_T("Arial"),wxFONTENCODING_DEFAULT));

    m_nBorderState = uiRect::BORDER_DOWN;

    m_timerFlash.SetOwner(this, ID_TIMER_FLASH);
    m_bFlashState = false;wmLabel

    CreateRects();
    m_uiRect.SetLabel(sLabel);
    m_uiRect.SetTextAlign(wxALIGN_CENTER);
    SetBackgroundColour(GetParent()->GetBackgroundColour());
    SetForegroundColour(GetParent()->GetForegroundColour());


    return true;
}

void wmLabel::CreateRects()
{
    m_uiRect.SetRect(GetClientRect());


}

wmLabel::wmLabel( wxWindow* parent, wxWindowID id, const wxBitmap& bitmap, const wxPoint& pos, const wxSize& size, long style,  const wxString& name)  : pmControl(),
	m_nStyle(0),
	m_nHoldCount(0),
	m_nHoldTrigger(0),
	m_bFlashState(false),
	m_nBorderState(0),
	m_nState(0),
	m_bChecked(false),
	m_nBitmapAlign(0)
{
    Create(parent, id, bitmap, pos, size, style,  name);
}

bool  wmLabel::Create(wxWindow* parent, wxWindowID id, const wxBitmap& bitmap, const wxPoint& pos, const wxSize& size, long style, const wxString& name)
{
    m_bmp = bitmap;

    return Create(parent, id, wxEmptyString, pos, size, style, name);
}

wmLabel::~wmLabel()
{

}

void wmLabel::SetLabel(const wxString& sLabel)
{
    m_uiRect.SetLabel(sLabel);
    Refresh();
}

void wmLabel::SetTextAlign(unsigned int nAlign)
{
    m_uiRect.SetTextAlign(nAlign);
    Refresh();
}

void wmLabel::SetBitmapAlign(unsigned int nAlign)
{
    m_nBitmapAlign = nAlign;
    Refresh();
}

void wmLabel::SetBorderState(unsigned short nState)
{
    m_nBorderState = nState;
    Refresh();
}


void wmLabel::OnPaint(wxPaintEvent& event)
{
    if(IsShown())
    {
        wxBufferedPaintDC dc(this);
        Draw(dc);
    }
}

void wmLabel::OnSize(wxSizeEvent& event)
{
    CreateRects();
    Refresh();
}


void wmLabel::Draw(wxDC& dc)
{
    dc.SetFont(GetFont());

    unsigned int nState(m_nState);

    if(m_bFlashState)
    {
        m_uiRect.SetBackgroundColour(m_clrFlashBack);
        m_uiRect.SetForegroundColour(m_clrFlashText);
        m_uiRect.Draw(dc, m_uiRect.GetLabel(), m_nBorderState, m_bmpFlash);
    }
    else
    {
        m_uiRect.SetBackgroundColour(GetBackgroundColour());
        m_uiRect.SetForegroundColour(GetForegroundColour());
        m_uiRect.Draw(dc, m_uiRect.GetLabel(), m_nBorderState, m_bmp);
    }

}

void wmLabel::SetBitmap(const wxBitmap& bitmap)
{
    m_bmp = bitmap;
}

void wmLabel::SetBitmapFlash(const wxBitmap& bitmap)
{
   m_bmpFlash = bitmap;
}

const wxBitmap& wmLabel::GetBitmap() const
{
    return m_bmp;
}

const wxBitmap& wmLabel::GetBitmapFlash() const
{
    return m_bmpFlash;
}


void wmLabel::SetColourFlash(const wxColour &colour)
{
    m_clrFlashBack = colour;
    Refresh();
}


void wmLabel::SetTextColourFlash(const wxColour &colour)
{
    m_clrFlashText = colour;
    Refresh();
}

void wmLabel::OnFlash(wxTimerEvent& event)
{
    m_bFlashState = !m_bFlashState;
    Refresh();
}

void wmLabel::Flash(bool bFlash, unsigned int nInterval)
{
    m_bFlashState = false;
    if(bFlash)
    {
        m_timerFlash.Start(nInterval);
    }
    else
    {
        m_timerFlash.Stop();
    }
}

bool wmLabel::SetBackgroundColour(const wxColour &colour)
{
    wxWindow::SetBackgroundColour(colour);
    Refresh();
    return true;
}

bool wmLabel::SetForegroundColour(const wxColour &colour)
{
    wxWindow::SetForegroundColour(colour);
    Refresh();
    return true;
}


uiRect& wmLabel::GetUiRect()
{
    return m_uiRect;
}
