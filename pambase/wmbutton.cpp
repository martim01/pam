#include "wmbutton.h"
#include <wx/dcbuffer.h>
#include <wx/log.h>
#include <algorithm>

using namespace std;

const int wmButton::ID_TIMER_HOLD   = wxNewId();
const int wmButton::ID_TIMER_FLASH  = wxNewId();

BEGIN_EVENT_TABLE(wmButton, pmControl)
    EVT_PAINT(wmButton::OnPaint)
    EVT_SIZE(wmButton::OnSize)
    EVT_LEFT_DOWN(wmButton::OnLeftDown)
    EVT_LEFT_UP(wmButton::OnLeftUp)
    EVT_LEFT_DCLICK(wmButton::OnLeftDClick)
    EVT_MOUSE_CAPTURE_LOST(wmButton::OnCaptureLost)
    EVT_MOTION(wmButton::OnMouseMove)
    EVT_TIMER(ID_TIMER_HOLD, wmButton::OnHolding)
    EVT_TIMER(ID_TIMER_FLASH, wmButton::OnFlash)
END_EVENT_TABLE()

#ifdef WXSPAM
IMPLEMENT_DYNAMIC_CLASS(wmButton, pmControl)
#else
wxIMPLEMENT_DYNAMIC_CLASS(wmButton, pmControl);
#endif // WXSPAM

DEFINE_EVENT_TYPE(wxEVT_BUTTON_PRESSED)
DEFINE_EVENT_TYPE(wxEVT_BUTTON_RELEASED)
DEFINE_EVENT_TYPE(wxEVT_BUTTON_HELD)


wmButton::wmButton() : pmControl()
	, m_nStyle(0)
	, m_nHoldCount(0)
	, m_nHoldTrigger(0)
	, m_bFlashState(false)
	, m_nBorderState(0)
	, m_nState(0)
	, m_bChecked(false)
	, m_nBitmapAlign(0)
	, m_bToggleLook(false)
	, m_dToggleWidth(0.0)
	{
	}

wmButton::wmButton(wxWindow *parent, wxWindowID id, const wxString& sLabel, const wxPoint& pos, const wxSize& size, long nStyle,const wxValidator& validator, const wxString& name) : pmControl()
	, m_nStyle(0)
	, m_nHoldCount(0)
	, m_nHoldTrigger(0)
	, m_bFlashState(false)
	, m_nBorderState(0)
	, m_nState(0)
	, m_bChecked(false)
	, m_nBitmapAlign(0)
	, m_bToggleLook(false)
	, m_dToggleWidth(0.0)
{
    Create(parent, id, sLabel, pos, size, nStyle, validator, name);
}

bool wmButton::Create(wxWindow *parent, wxWindowID id, const wxString& sLabel , const wxPoint& pos, const wxSize& size, long nStyle,const wxValidator& validator, const wxString& name)
{
    m_bToggleLook = false;
//    m_pScroller = new wmScroller(GetEventHandler(), m_uiRect.GetSize());

    wxSize szInit(size);
    wxSize bestSize = DoGetBestSize();
    if(size.x<=0)
        szInit.SetWidth(bestSize.x);
    if(size.y <= 0)
        szInit.SetHeight(bestSize.y);

    if(!wxWindow::Create(parent,id,pos,szInit,wxWANTS_CHARS, name))
        return false;

    SetMinSize(size);

    m_nStyle = nStyle;



    SetBackgroundStyle(wxBG_STYLE_CUSTOM);


    m_nBorderState = uiRect::BORDER_UP;
    m_nState = STATE_NORMAL;

    m_timerHold.SetOwner(this, ID_TIMER_HOLD);
    m_timerFlash.SetOwner(this, ID_TIMER_FLASH);


    CreateRects();
    m_uiRect.SetLabel(sLabel);
    m_uiRect.SetTextAlign(wxALIGN_CENTER);
    SetBackgroundColour(m_uiRect.GetBackgroundColour());
    SetForegroundColour(m_uiRect.GetForegroundColour());

    m_bChecked = false;
    m_bFlashState = false;

    m_nHoldTrigger = 10;

    SetFont(wxFont(10,wxFONTFAMILY_SWISS,wxFONTSTYLE_NORMAL,wxFONTWEIGHT_NORMAL,false,_T("Arial"),wxFONTENCODING_DEFAULT));

    return true;
}

void wmButton::CreateRects()
{
    m_uiRect.SetRect(GetClientRect());

    double dButtonWidth = GetClientRect().GetWidth()*(m_dToggleWidth/100.0);
    double dLabelWidth = (GetClientRect().GetWidth()-dButtonWidth)/2;

    m_uiToggleLeft.SetRect(GetClientRect().GetLeft(), GetClientRect().GetTop(), dLabelWidth, GetClientRect().GetHeight());
    m_uiGroove.SetRect(m_uiToggleLeft.GetRight(), m_uiToggleLeft.GetTop()+2, dButtonWidth, GetClientRect().GetHeight()-4);
    m_uiToggleRight.SetRect(m_uiGroove.GetRight(), GetClientRect().GetTop(), GetClientRect().GetRight()-m_uiGroove.GetRight()+1, GetClientRect().GetHeight());

    m_uiToggleLeft.Pad(5,0);
    m_uiToggleRight.Pad(5,0);

    m_uiToggleLeft.SetTextAlign(wxALIGN_CENTER_VERTICAL | wxALIGN_RIGHT);
    m_uiToggleRight.SetTextAlign(wxALIGN_CENTER_VERTICAL | wxALIGN_LEFT);

//    m_pScroller->ChangeLimits(m_uiRect.GetSize());

}

wmButton::wmButton( wxWindow* parent, wxWindowID id, const wxBitmap& bitmap, const wxPoint& pos, const wxSize& size, long style, const wxValidator& validator, const wxString& name) : pmControl()
{
    Create(parent, id, bitmap, pos, size, style, validator, name);
}

bool  wmButton::Create(wxWindow* parent, wxWindowID id, const wxBitmap& bitmap, const wxPoint& pos, const wxSize& size, long style, const wxValidator& validator, const wxString& name)
{
    m_bmp[STATE_NORMAL] = bitmap;
    return Create(parent, id, wxEmptyString, pos, size, style, validator, name);
}

wmButton::~wmButton()
{
    if(HasCapture())
    {
        ReleaseMouse();
    }
}

void wmButton::SetLabel(const wxString& sLabel)
{
    m_uiRect.SetLabel(sLabel);
    Refresh();
}

void wmButton::SetTextAlign(unsigned int nAlign)
{
    m_uiRect.SetTextAlign(nAlign);
    Refresh();
}

void wmButton::SetBitmapAlign(unsigned int nAlign)
{
    m_nBitmapAlign = nAlign;
    m_uiRect.SetBitmapAlign(nAlign);
    Refresh();
}

bool wmButton::SetBackgroundColour(const wxColour &colour)
{
    for(int i = 0; i <= 4; i++)
    {
        m_clrBackground[i] = colour;
    }
    Refresh();
    return true;
}
bool wmButton::SetForegroundColour(const wxColour &colour)
{
    for(int i = 0; i <= 4; i++)
    {
        m_clrForeground[i] = colour;
    }
    Refresh();
    return true;
}

void wmButton::SetBorderState(unsigned short nState)
{
    m_nBorderState = nState;
    Refresh();
}


void wmButton::OnPaint(wxPaintEvent& event)
{
    wxBufferedPaintDC dc(this);

    if(!m_bToggleLook)
    {
        Draw(dc);
    }
    else
    {
        DrawToggle(dc);
    }
}

void wmButton::OnSize(wxSizeEvent& event)
{
    CreateRects();
    Refresh();
}

void wmButton::OnLeftDClick(wxMouseEvent& event)
{
    OnLeftDown(event);
    //OnLeftUp(event);
}

void wmButton::OnLeftDown(wxMouseEvent& event)
{
    CaptureMouse();

//    m_pScroller->MouseDown(event.GetPosition());


    if(m_uiRect.Contains(event.GetPosition()))// && (*itButton).bEnabled)
    {
        SendEvent(wxEVT_BUTTON_PRESSED);
        m_nBorderState = uiRect::BORDER_DOWN;

        if(m_nState != STATE_ARMED)
        {
            m_nState = STATE_SELECTED;
        }

        m_clrDown[0] = m_clrForeground[STATE_SELECTED];
        m_clrDown[1] = m_clrBackground[STATE_SELECTED];

        if((m_nStyle & STYLE_HOLD) || (m_nStyle & STYLE_REPEAT))
        {
            m_timerHold.Start(50);
        }
        m_nHoldCount = 0;
        Refresh();
        Update();
    }
}

void wmButton::ToggleSelection(bool bSelected, bool bEvent)
{
    if((m_nStyle & STYLE_SELECT))
    {
        if(bSelected != m_bChecked)
        {
            if(bSelected)
            {
                m_nBorderState = uiRect::BORDER_DOWN;
                m_nState = STATE_SELECTED;
                m_bChecked = true;
                m_clrDown[0] = m_clrForeground[STATE_SELECTED];
                m_clrDown[1] = m_clrBackground[STATE_SELECTED];
            }
            else
            {
                m_nBorderState = uiRect::BORDER_UP;
                m_nState = STATE_NORMAL;
                m_bChecked = false;
            }
            if(bEvent)
            {
                SendEvent(wxEVT_COMMAND_BUTTON_CLICKED);
            }
            Refresh();
        }
    }
}

void wmButton::OnLeftUp(wxMouseEvent& event)
{
    m_timerHold.Stop();
    SendEvent(wxEVT_BUTTON_RELEASED);


    if(m_uiRect.Contains(event.GetPosition()))
    {
        if((m_nStyle & STYLE_SELECT))
        {
            if(!m_bChecked)
            {
                m_nBorderState = uiRect::BORDER_DOWN;
                m_nState = STATE_SELECTED;
                m_bChecked = true;
            }
            else
            {
                m_nBorderState = uiRect::BORDER_UP;
                m_nState = STATE_NORMAL;
                m_bChecked = false;
            }
            if(m_nState == STATE_ARMED || (m_nStyle & STYLE_ARM)==0)
            {
                SendEvent(wxEVT_COMMAND_BUTTON_CLICKED);
            }
        }
        else
        {
            if(m_nState == STATE_ARMED || (m_nStyle & STYLE_ARM)==0)
            {
                SendEvent(wxEVT_COMMAND_BUTTON_CLICKED);
            }
            m_nBorderState = uiRect::BORDER_UP;
            if((m_nStyle & STYLE_ARM) && m_nState != STATE_ARMED)
            {
                m_nState = STATE_ARMED;
                m_timerHold.Start(1000,true);
            }
            else
            {
                m_nState = STATE_NORMAL;
            }

        }

     }
    else
    {
        //we've scrolled out over the button so lets tell
//        m_pScroller->MouseUp(event.GetPosition());

        m_nBorderState = uiRect::BORDER_UP;
    }
    if(HasCapture())
    {
        ReleaseMouse();
    }

    Refresh();

}

void wmButton::OnHolding(wxTimerEvent& event)
{

    if((m_nStyle & STYLE_HOLD))
    {
        m_clrDown[0] = wxColour(min(255, m_clrDown[0].Red()+20),
                            min(255, m_clrDown[0].Green()+20),
                            min(255, m_clrDown[0].Blue()+20));

        m_clrDown[1] = wxColour(min(255, m_clrDown[1].Red()+20),
                            min(255, m_clrDown[1].Green()+20),
                            min(255, m_clrDown[1].Blue()+20));


        Refresh();
        Update();
        m_nHoldCount++;
        if(m_nHoldCount > m_nHoldTrigger)
        {
            SendEvent(wxEVT_BUTTON_HELD);
            m_timerHold.Stop();
            m_nHoldCount = 0;
            if(HasCapture())
            {
                ReleaseMouse();
            }
        }
    }
    if((m_nStyle & STYLE_REPEAT))
    {
        SendEvent(wxEVT_COMMAND_BUTTON_CLICKED);
    }

    if((m_nStyle & STYLE_ARM))
    {
        m_nState = STATE_NORMAL;
        Refresh();
        Update();
    }
}

void wmButton::OnCaptureLost(wxMouseCaptureLostEvent& event)
{
     m_nBorderState = uiRect::BORDER_UP;
     Refresh();
     m_timerHold.Stop();
}

void wmButton::Draw(wxDC& dc)
{

    dc.SetFont(GetFont());

    unsigned int nState(m_nState);
    if(m_bFlashState)
    {
        nState = STATE_FLASH;
    }

    wxBitmap bmp;
    if(m_bmp[nState].IsOk())
    {
        bmp = m_bmp[nState];
    }
    else if(m_bmp[STATE_NORMAL].IsOk())
    {
        bmp = m_bmp[STATE_NORMAL];
    }

    if(m_bChecked)
    {
        nState = STATE_SELECTED;
    }
    if(nState == STATE_SELECTED)
    {
        m_uiRect.SetBackgroundColour(m_clrDown[1]);
        m_uiRect.SetForegroundColour(m_clrDown[0]);
    }
    else
    {
        m_uiRect.SetBackgroundColour(m_clrBackground[nState]);
        m_uiRect.SetForegroundColour(m_clrForeground[nState]);
    }
    if(m_nState != STATE_DISABLED)
    {
        m_uiRect.Draw(dc, m_uiRect.GetLabel(), m_nBorderState, bmp);
    }
    else
    {
        m_uiRect.Draw(dc, m_uiRect.GetLabel(), uiRect::BORDER_FLAT, bmp);
    }
}


void wmButton::DrawToggle(wxDC& dc)
{
    dc.SetFont(GetFont());

    unsigned int nState(m_nState);

    wxBitmap bmp;
    if(m_bmp[nState].IsOk())
    {
        bmp = m_bmp[nState];
    }
    else if(m_bmp[STATE_NORMAL].IsOk())
    {
        bmp = m_bmp[STATE_NORMAL];
    }


    //draw groove
    dc.SetBackground(wxBrush(wxColour(wxColour(180,180,180))));
    dc.Clear();

    m_uiGroove.SetBackgroundColour(wxColour(140,140,140));
    m_uiGroove.Draw(dc, wxEmptyString, uiRect::BORDER_NONE);

    m_uiToggleLeft.SetBackgroundColour(wxColour(180,180,180));
    m_uiToggleRight.SetBackgroundColour(wxColour(180,180,180));
    m_uiToggleLeft.SetForegroundColour(*wxBLACK);
    m_uiToggleRight.SetForegroundColour(*wxBLACK);
    m_uiToggleLeft.Draw(dc,uiRect::BORDER_NONE);
    m_uiToggleRight.Draw(dc,uiRect::BORDER_NONE);

    if(m_bChecked)
    {
        nState = STATE_SELECTED;
    }

    m_uiRect.SetBackgroundColour(m_clrBackground[STATE_NORMAL]);
    m_uiRect.SetForegroundColour(m_clrForeground[STATE_NORMAL]);

    m_uiRect.SetTop(m_uiGroove.GetTop()+2);
    m_uiRect.SetBottom(m_uiGroove.GetBottom()-2);

    if(m_bChecked)
    {
        m_uiRect.SetLeft(m_uiGroove.GetCenter().x - (m_uiGroove.GetWidth()/4));
        m_uiRect.SetRight(m_uiGroove.GetRight()-2);
    }
    else
    {
        m_uiRect.SetLeft(m_uiGroove.GetLeft()+2);
        m_uiRect.SetRight(m_uiGroove.GetCenter().x+(m_uiGroove.GetWidth()/4));
    }

    if(m_nState != STATE_DISABLED)
    {
        m_uiRect.Draw(dc, m_uiRect.GetLabel(), uiRect::BORDER_UP, bmp);
    }
    else
    {
        m_uiRect.SetBackgroundColour(m_clrBackground[STATE_DISABLED]);
        m_uiRect.SetForegroundColour(m_clrForeground[STATE_DISABLED]);
        m_uiRect.Draw(dc, m_uiRect.GetLabel(), uiRect::BORDER_FLAT, bmp);
    }
}

void wmButton::SendEvent(wxEventType eventType)
{
    wxCommandEvent event(eventType, GetId());
    event.SetEventObject(this);
    event.SetString(m_uiRect.GetLabel());
    event.SetClientData(m_uiRect.GetClientData());
    event.SetExtraLong(m_uiRect.GetIntData());
    event.SetInt(m_bChecked);
    wxPostEvent(GetEventHandler(), event);
}


void wmButton::SetBitmapLabel(const wxBitmap& bitmap)
{
    m_bmp[STATE_NORMAL] = bitmap;
}

void wmButton::SetBitmapDisabled(const wxBitmap& bitmap)
{
    m_bmp[STATE_DISABLED] = bitmap;
}

void wmButton::SetBitmapSelected(const wxBitmap& bitmap)
{
    m_bmp[STATE_SELECTED] = bitmap;
}

void wmButton::SetBitmapArmed(const wxBitmap& bitmap)
{
   m_bmp[STATE_ARMED] = bitmap;
}

void wmButton::SetBitmapFlash(const wxBitmap& bitmap)
{
   m_bmp[STATE_FLASH] = bitmap;
}

const wxBitmap& wmButton::GetBitmapLabel() const
{
    return m_bmp[STATE_NORMAL];
}
const wxBitmap& wmButton::GetBitmapDisabled() const
{
    return m_bmp[STATE_DISABLED];
}

const wxBitmap& wmButton::GetBitmapSelected() const
{
    return m_bmp[STATE_SELECTED];
}

const wxBitmap& wmButton::GetBitmapArmed() const
{
    return m_bmp[STATE_ARMED];
}

const wxBitmap& wmButton::GetBitmapFlash() const
{
    return m_bmp[STATE_FLASH];
}

void wmButton::SetColour(const wxColour &colour)
{
    m_clrBackground[STATE_NORMAL] = colour;
    Refresh();
}

void wmButton::SetColourDisabled(const wxColour &colour)
{
    m_clrBackground[STATE_DISABLED] = colour;
    Refresh();
}

void wmButton::SetColourSelected(const wxColour &colour)
{
    m_clrBackground[STATE_SELECTED] = colour;
    Refresh();
}

void wmButton::SetColourArmed(const wxColour &colour)
{
    m_clrBackground[STATE_ARMED] = colour;
    Refresh();
}

void wmButton::SetColourFlash(const wxColour &colour)
{
    m_clrBackground[STATE_FLASH] = colour;
    Refresh();
}

void wmButton::SetTextColour(const wxColour &colour)
{
    m_clrForeground[STATE_NORMAL] = colour;
    Refresh();
}

void wmButton::SetTextColourDisabled(const wxColour &colour)
{
    m_clrForeground[STATE_DISABLED] = colour;
    Refresh();
}

void wmButton::SetTextColourSelected(const wxColour &colour)
{
    m_clrForeground[STATE_SELECTED] = colour;
    Refresh();
}

void wmButton::SetTextColourArmed(const wxColour &colour)
{
    m_clrForeground[STATE_ARMED] = colour;
    Refresh();
}

void wmButton::SetTextColourFlash(const wxColour &colour)
{
    m_clrForeground[STATE_FLASH] = colour;
    Refresh();
}

bool wmButton::Enable(bool bEnable)
{
    if(bEnable && IsEnabled() == false)
    {
        m_nState = STATE_NORMAL;

    }
    else if(!bEnable && IsEnabled())
    {
        m_nState = STATE_DISABLED;
        Flash(false);
    }
    Refresh();
    return wxWindow::Enable(bEnable);

}

void wmButton::OnFlash(wxTimerEvent& event)
{
    m_bFlashState = !m_bFlashState;
    Refresh();
}

void wmButton::Flash(bool bFlash, unsigned int nInterval)
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


void wmButton::OnMouseMove(wxMouseEvent& event)
{
//    m_pScroller->MouseMove(event.GetPosition());
}


bool wmButton::IsChecked() const
{
    return m_bChecked;
}


void wmButton::SetAuxillaryText(const wxString& sText)
{
    m_sAuxillary = sText;
}

const wxString& wmButton::GetAuxillaryText() const
{
    return m_sAuxillary;
}
