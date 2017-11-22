#include "wmIpEdit.h"

#include <wx/dc.h>
#include <wx/dcclient.h>
#include <wx/dcbuffer.h>
#include <map>
#include <wx/settings.h>
#include <wx/caret.h>
#include <wx/string.h>
#include <wx/app.h>
#include <wx/log.h>
#include "wmfocusmanager.h"
#include "wmedit.h"

using namespace std;


IMPLEMENT_DYNAMIC_CLASS(wmIpEdit, wxControl)

BEGIN_EVENT_TABLE(wmIpEdit, wxControl)
    EVT_PAINT(wmIpEdit::OnPaint)
    EVT_SIZE(wmIpEdit::OnSize)
    EVT_MOUSE_EVENTS(wmIpEdit::OnMouse)
    EVT_MOUSE_CAPTURE_LOST(wmIpEdit::OnCaptureLost)
    EVT_SET_FOCUS(wmIpEdit::OnSetFocus)
    EVT_KILL_FOCUS(wmIpEdit::OnKillFocus)
    EVT_CHAR(wmIpEdit::OnChar)
END_EVENT_TABLE()

wmIpEdit::wmIpEdit() : wxControl(),
    m_bFocus(false),
    m_pCaret(0),
    m_nInsert(0),
    m_bInsert(true)
{
}

wmIpEdit::~wmIpEdit()
{
    //dtor
}


wmIpEdit::wmIpEdit(wxWindow * pParent, wxWindowID id, const wxString& value, const wxPoint& pos, const wxSize& size,long style, const wxValidator& validator, const wxString& name) : wxControl(),
    m_bFocus(false),
    m_pCaret(0),
    m_nInsert(0),
    m_bInsert(true)
{

    Create(pParent, id,  pos, size);
}


wmIpEdit::wmIpEdit(wxWindow* pParent, wxWindowID id, const wxPoint& pos,const wxSize& size) :
    m_bFocus(false),
    m_pCaret(0),
    m_nInsert(0),
    m_bInsert(true)
{
    Create(pParent, id, pos,size);
}

bool wmIpEdit::Create(wxWindow* pParent, wxWindowID id, const wxPoint& pos, const wxSize& size, long nStyle)
{


    wxSize szInit(size);
    wxSize bestSize = DoGetBestSize();
    if(size.x<=0)
        szInit.SetWidth(bestSize.x);
    if(size.y <= 0)
        szInit.SetHeight(bestSize.y);


    if(!wxControl::Create(pParent,id,pos,szInit, wxWANTS_CHARS ,wxDefaultValidator,wxEmptyString))
        return false;

    SetSize(szInit);

    SetInitialSize(GetSize());


    SetInsertPos(m_sText.length());


    SetBackgroundStyle(wxBG_STYLE_CUSTOM);

    m_uiText.SetRect(GetClientRect());
    m_uiText.SetGradient(false);
    m_uiText.SetTextAlign(wxALIGN_LEFT | wxALIGN_CENTER_VERTICAL);
    m_uiText.SetBackgroundColour(*wxWHITE);
    m_uiText.SetForegroundColour(*wxBLACK);



    wxClientDC dc(this);
    m_bInsert = true;
    SetInsertPos(0);
    ConvertToDateTime();

    wmFocusManager::Get()->AddControl(this);

    m_clrBackground[0] = *wxWHITE;
    m_clrBackground[1] = *wxWHITE;
    m_clrForeground[0] = *wxBLACK;
    m_clrForeground[1] = *wxBLACK;
    m_nBorderStyle[0] = uiRect::BORDER_DOWN;
    m_nBorderStyle[1] = uiRect::BORDER_DOWN;

    m_vType.resize(15);
    m_vType[0] = 1;
    m_vType[1] = 1;
    m_vType[2] = 1;
    m_vType[3] = 0;
    m_vType[4] = 1;
    m_vType[5] = 1;
    m_vType[6] = 1;
    m_vType[7] = 0;
    m_vType[8] = 1;
    m_vType[9] = 1;
    m_vType[10] = 1;
    m_vType[11] = 0;
    m_vType[12] = 1;
    m_vType[14] = 1;
    m_vType[14] = 1;

    m_sText = "   .   .   .   ";

    return true;
}

wxSize wmIpEdit::DoGetBestSize() const
{
    return wxSize(150,25);
}

void wmIpEdit::OnChar(wxKeyEvent& event)
{
    if(m_bFocus)
    {

        switch(event.GetKeyCode())
        {
            case WXK_BACK:
            case WXK_LEFT:
            case WXK_NUMPAD_DELETE:
            case WXK_NUMPAD_LEFT:
                if(GetInsertPos() > 0)
                {
                    if(m_vType[GetInsertPos()-1] == 0)
                        SetInsertPos(GetInsertPos()-2);
                    else
                        SetInsertPos(GetInsertPos()-1);
                }
                break;
            case WXK_RIGHT:
            case WXK_NUMPAD_RIGHT:
                if(GetInsertPos()+1 < m_sText.length())
                {
                    if(m_vType[GetInsertPos()+1] == 0)
                        SetInsertPos(GetInsertPos()+2);
                    else
                        SetInsertPos(GetInsertPos()+1);
                }
                break;
            case WXK_UP:
            case WXK_NUMPAD_UP:
            case WXK_DOWN:
            case WXK_NUMPAD_DOWN:
                break;
            case WXK_HOME:
            case WXK_NUMPAD_HOME:
                SetInsertPos(0);
                break;
            case WXK_END:
            case WXK_NUMPAD_END:
                SetInsertPos(14);
                break;
            case WXK_RETURN:
            case WXK_NUMPAD_ENTER:
                {
                wxCommandEvent eventEnter(wxEVT_COMMAND_TEXT_ENTER, GetId());
                eventEnter.SetString(m_sText);
                wxPostEvent(GetEventHandler(), eventEnter);
                }
                break;
            case WXK_TAB:
                {
                    wxCommandEvent event(wxEVT_TEXT_TAB, GetId());
                    event.SetString(m_sText);
                    wxPostEvent(GetEventHandler(), event);
                }
                break;
            case WXK_ESCAPE:
                {
                wxCommandEvent event(wxEVT_TEXT_ESCAPE, GetId());
                event.SetString(m_sText);
                wxPostEvent(GetParent(), event);
                }
                break;
            case WXK_NUMPAD0:   //324
            case WXK_NUMPAD1:
            case WXK_NUMPAD2:
            case WXK_NUMPAD3:
            case WXK_NUMPAD4:
            case WXK_NUMPAD5:
            case WXK_NUMPAD6:
            case WXK_NUMPAD7:
            case WXK_NUMPAD8:
            case WXK_NUMPAD9:
                Char(static_cast<wxChar>(event.GetKeyCode()-276));
                break;
            default:
                Char(static_cast<wxChar>(event.GetKeyCode()));

        }


        if(GetInsertPos() >= m_sText.length())
        {
            SetInsertPos(m_sText.length()-1);
        }

        wxCommandEvent eventUp(wxEVT_COMMAND_TEXT_UPDATED, GetId());
        eventUp.SetString(m_sText);
        if(GetEventHandler())
        {
            GetEventHandler()->ProcessEvent(eventUp);
        }

        Refresh();
    }
    event.Skip();
}

bool wmIpEdit::Enable(bool bEnable)
{
    bool bReturn(wxControl::Enable(bEnable));
    Refresh();
    return bReturn;
}


void wmIpEdit::Char(wxChar chChar)
{

    if(chChar == wxT('.'))
    {
        //move on to next block
    }

    int nCharInsert = GetInsertPos();

    switch(m_vType[GetInsertPos()])
	{
        case HOUR_TEN:
            if(chChar > wxT('2'))	//2
                return;
            if(chChar == wxT('2') && m_sText.GetChar(GetInsertPos()+1) > wxT('3'))
                m_sText.SetChar(GetInsertPos()+1, wxT('3'));
            break;
        case HOUR_UNIT:
            if(m_sText.GetChar(GetInsertPos()-1) == wxT('2') && chChar > wxT('3'))	//24
                return;
            SetInsertPos(GetInsertPos()+1); //avoid the :
            break;
        case MINUTE_TEN:
            if(chChar > wxT('5'))	//5
                return;
            break;
        case SECOND_TEN:
            if(chChar > wxT('5'))	//5
                return;
            break;
        case MINUTE_UNIT:
        case SECOND_UNIT:
        case MILLI_UNIT:
        case YEAR_UNIT:
            SetInsertPos(GetInsertPos()+1);
            break;
        case FRAME_TEN:
            if((unsigned int)chChar > (wxT('0')+(m_nFrames/10)))
                return;
            if(((unsigned int)chChar == (wxT('0')+(m_nFrames/10))) && ((unsigned int)m_sText.GetChar(GetInsertPos()+1) >= (wxT('0')+(m_nFrames%10))))
                m_sText.SetChar(GetInsertPos()+1, (wxT('0')+(m_nFrames%10)-1));
            break;
        case FRAME_UNIT:
            if(((unsigned int)m_sText.GetChar(GetInsertPos()-1) == (wxT('0')+(m_nFrames/10))) &&  ((unsigned int)chChar >= (wxT('0')+(m_nFrames%10))))
                return;
            SetInsertPos(GetInsertPos()+1);
            break;
        case DAY_TEN:
            if(chChar > MaxDayTen())
                return;
            if(chChar == MaxDayTen() && m_sText.GetChar(GetInsertPos()+1) > MaxDayUnit())
                m_sText.SetChar(GetInsertPos()+1, MaxDayUnit());
            break;
        case DAY_UNIT:
            if(m_sText.GetChar(GetInsertPos()-1) == MaxDayTen() && chChar > MaxDayUnit())
                return;
            SetInsertPos(GetInsertPos()+1);
            break;
        case MONTH_TEN:
            if(chChar > wxT('1'))
                return;
            if(chChar == wxT('1') && m_sText.GetChar(GetInsertPos()+1) > wxT('2'))
                m_sText.SetChar(GetInsertPos()+1, wxT('2'));
            break;
        case MONTH_UNIT:
            if(m_sText.GetChar(GetInsertPos()-1) == wxT('1') && chChar > wxT('2'))
                return;
            SetInsertPos(GetInsertPos()+1);
            break;
        case YEAR_CEN:
        case YEAR_MIL:
        case YEAR_TEN:
        case MILLI_HUND:
        case MILLI_TEN:

            break;
        default:
            return;
	}

	m_sText.SetChar(nCharInsert,chChar);
    CheckDays();
	SetInsertPos(GetInsertPos()+1);


    ConvertToDateTime();


}

void wmIpEdit::CheckDays()
{
    unsigned long nDay, nMonth, nYear, nTen, nUnit;

    if(m_nDateFormat != DATE_NONE)
    {
        m_sText.Mid(m_nDayPos,2).ToULong(&nDay);
        m_sText.Mid(m_nMonthPos,2).ToULong(&nMonth);
        m_sText.Mid(m_nYearPos,4).ToULong(&nYear);
        nTen = m_nDayPos;
        nUnit = m_nDayPos+1;
        if(nDay > wxDateTime::GetNumberOfDays(wxDateTime::Month(nMonth-1), nYear, wxDateTime::Gregorian))
        {
            wxString sDay(wxString::Format(wxT("%02d"),wxDateTime::GetNumberOfDays(wxDateTime::Month(nMonth-1), nYear, wxDateTime::Gregorian)));
            m_sText.SetChar(nTen, sDay.GetChar(0));
            m_sText.SetChar(nUnit, sDay.GetChar(1));
        }
    }

}


void wmIpEdit::MoveTime(int nWay)
{
    wxDateTime dt(m_dtTimestamp);
    switch(m_vType[GetInsertPos()])
    {
        case HOUR_TEN:
            if(nWay > 0)
            {
                if(dt.GetHour() < 14)
                    dt.Add(wxTimeSpan(nWay*10,0,0,0));
                else if(dt.GetHour() < 20)
                    dt.Add(wxTimeSpan(nWay*14,0,0,0));
                else
                    dt.Add(wxTimeSpan(nWay*4,0,0,0));
            }
            else
            {
                if(dt.GetHour() > 10)
                    dt.Add(wxTimeSpan(nWay*10,0,0,0));
                else if (dt.GetHour() < 4)
                    dt.Add(wxTimeSpan(nWay*4,0,0,0));
                else
                    dt.Add(wxTimeSpan(nWay*14,0,0,0));
            }
            break;
        case HOUR_UNIT:
            dt.Add(wxTimeSpan(nWay,0,0,0));
            break;
        case MINUTE_TEN:
            dt.Add(wxTimeSpan(0,nWay*10,0,0));
            break;
        case MINUTE_UNIT:
            dt.Add(wxTimeSpan(0,nWay,0,0));
            break;
        case SECOND_TEN:
            dt.Add(wxTimeSpan(0,0,nWay*10,0));
            break;
        case SECOND_UNIT:
            dt.Add(wxTimeSpan(0,0,nWay,0));
            break;
        case MILLI_UNIT:
            dt.Add(wxTimeSpan(0,0,0,nWay));
            break;
        case MILLI_TEN:
            dt.Add(wxTimeSpan(0,0,0,nWay*10));
            break;
        case MILLI_HUND:
            dt.Add(wxTimeSpan(0,0,0,nWay*100));
            break;
        case FRAME_TEN:
            if(nWay > 0)
            {
                dt.Add(wxTimeSpan(0,0,0,static_cast<unsigned int>(m_dFrameTime*10.0)));
            }
            else
            {
                dt.Subtract(wxTimeSpan(0,0,0,static_cast<unsigned int>(m_dFrameTime*10.0)));
            }
            break;
        case FRAME_UNIT:
            if(nWay > 0)
            {
                dt.Add(wxTimeSpan(0,0,0,static_cast<unsigned int>(m_dFrameTime)));
            }
            else
            {
                dt.Subtract(wxTimeSpan(0,0,0,static_cast<unsigned int>(m_dFrameTime)));
            }
            break;
        case DAY_TEN:
            if(nWay > 0)
            {
                int nMax = wxDateTime::GetNumberOfDays(dt.GetMonth(), dt.GetYear(), wxDateTime::Gregorian);
                if(dt.GetDay() + 10 <= nMax)
                {
                    dt += wxDateSpan(0,0,0,10);
                }
                else
                {
                    dt+= wxDateSpan(0,0,0,nMax-20);
                }

            }
            else
            {
                wxDateTime dtPrev = dt - wxDateSpan(0,1,0,0);
                int nMax = wxDateTime::GetNumberOfDays(dtPrev.GetMonth(), dtPrev.GetYear(), wxDateTime::Gregorian);
                if(dt.GetDay() - 10 > 0)
                {
                    dt -= wxDateSpan(0,0,0,10);
                }
                else
                {
                    dt -= wxDateSpan(0,0,0,nMax-20);

                }
            }
            break;
        case DAY_UNIT:
            dt += wxDateSpan(0,0,0,nWay);
            break;
        case MONTH_TEN:
            if(nWay > 0)
            {
                if(dt.GetMonth() < 2)
                {
                    dt += wxDateSpan(0,10,0,0);
                }
                else if(dt.GetMonth() > 9)
                {
                    dt += wxDateSpan(0,2,0,0);
                }
                else
                {
                    dt += wxDateSpan(0,12,0,0);
                }
            }
            else
            {
                if(dt.GetMonth() > 9)
                {
                    dt -= wxDateSpan(0,10,0,0);
                }
                else if(dt.GetMonth() < 2)
                {
                    dt -= wxDateSpan(0,2,0,0);
                }
                else
                {
                    dt -= wxDateSpan(0,12,0,0);
                }
            }
            break;
        case MONTH_UNIT:
            dt += wxDateSpan(0,nWay,0,0);
            break;
        case YEAR_MIL:
            dt += wxDateSpan(nWay*1000,0,0,0);
            break;
        case YEAR_CEN:
            dt += wxDateSpan(nWay*100,0,0,0);
            break;
        case YEAR_TEN:
            dt += wxDateSpan(nWay*10,0,0,0);
            break;
        case YEAR_UNIT:
            dt += wxDateSpan(nWay,0,0,0);
            break;
    }


    SetString(dt);


    ConvertToDateTime();
    Refresh();
}


void wmIpEdit::SetString(const wxDateTime& dt)
{
    if(m_nTimeFormat != TIME_HMSFr)
    {
        m_sText = dt.Format(m_sFormat);
    }
    else
    {
        wxString sTime(dt.Format(m_sFormat));
        sTime.Left(m_nMilliPos);

        unsigned int nFrames = static_cast<unsigned int>(static_cast<double>(dt.GetMillisecond())/ m_dFrameTime);
        m_sText = wxString::Format(wxT("%s%02d%s"), sTime.Left(m_nMilliPos).c_str(), nFrames, sTime.Mid(m_nMilliPos+2).c_str());
    }
}



bool wmIpEdit::ConvertToDateTime()
{
    wxDateTime dt;

    switch(m_nTimeFormat)
    {

        case TIME_HMSFr:
        {
            unsigned long nFrames;
            m_sText.Mid(m_nMilliPos,3).ToULong(&nFrames);

            wxString sTime(m_sText.Left(m_nMilliPos-1));
            sTime += m_sText.Mid(m_nMilliPos+2);

            if(dt.ParseFormat(sTime, m_sFormat, wxDateTime::UNow()))
            {
                if(m_nDateFormat == DATE_NONE && dt < wxDateTime::UNow())
                    dt += wxDateSpan(0,0,0,1);

                dt.SetMillisecond(static_cast<unsigned int>(static_cast<double>(nFrames)*m_dFrameTime));
                m_dtTimestamp = dt;
                return true;
            }
            return false;
        }
        break;
        case TIME_HMSmS:
        {
            unsigned long nMilliseconds;
            m_sText.Mid(m_nMilliPos,3).ToULong(&nMilliseconds);


            wxString sTime(m_sText.Left(m_nMilliPos-1));
            sTime += m_sText.Mid(m_nMilliPos+3);

            wxString sFormat(m_sFormat);
            sFormat.Replace(wxT(":%l"), wxT(""));


            if(dt.ParseFormat(sTime, sFormat, wxDateTime::UNow()))
            {
                if(m_nDateFormat == DATE_NONE && dt < wxDateTime::UNow())
                    dt += wxDateSpan(0,0,0,1);
                dt.SetMillisecond(nMilliseconds);
                m_dtTimestamp = dt;
                return true;
            }
            return false;
        }
        break;
        default:
            if(dt.ParseFormat(m_sText, m_sFormat, wxDateTime::UNow()))
            {
                if(m_nDateFormat == DATE_NONE && dt < wxDateTime::UNow())
                {
                    dt += wxDateSpan(0,0,0,1);
                }
                else if((m_nDateFormat == DATE_DDMMYY || m_nDateFormat == DATE_MMDDYY || m_nDateFormat == DATE_YYMMDD) && dt.GetYear() < 2000)
                {
                    dt += wxDateSpan(100,0,0,0);
                }
                m_dtTimestamp = dt;
                return true;
            }
    }

    return false;


}

void wmIpEdit::OnPaint(wxPaintEvent& event)
{
    wxAutoBufferedPaintDC dc(this);
    dc.SetFont(GetFont());

    m_uiText.SetBackgroundColour(m_clrBackground[m_bFocus]);
    m_uiText.SetForegroundColour(m_clrForeground[m_bFocus]);
    m_uiText.Draw(dc, m_sText, m_nBorderStyle[m_bFocus]);


    SetCaretPos(dc, m_sText);
}

void wmIpEdit::OnSize(wxSizeEvent& event)
{
    m_uiText.SetRect(GetClientRect());
    Refresh();
}

void wmIpEdit::OnMouse(wxMouseEvent& event)
{
    if(event.LeftDown())
    {
        if(!HasCapture())
            CaptureMouse();
        SetFocus();
    }
    else if(event.LeftUp())
    {
        if(HasCapture())
            ReleaseMouse();

    }
    else
        event.Skip();
}

void wmIpEdit::OnCaptureLost(wxMouseCaptureLostEvent& event)
{
}

void wmIpEdit::OnSetFocus(wxFocusEvent& event)
{
    m_bFocus = true;

    CreateCaret();
    Refresh();
}

void wmIpEdit::OnKillFocus(wxFocusEvent& event)
{
    HideCaret();

    m_bFocus = false;
    Refresh();
}


wxChar wmIpEdit::MaxDayTen()
{
    return wxT('0') + (wxDateTime::GetNumberOfDays(m_dtTimestamp.GetMonth(), m_dtTimestamp.GetYear(), wxDateTime::Gregorian) / 10);
}


wxChar wmIpEdit::MaxDayUnit()
{
    return wxT('0') + (wxDateTime::GetNumberOfDays(m_dtTimestamp.GetMonth(), m_dtTimestamp.GetYear(), wxDateTime::Gregorian) % 10);
}




void wmIpEdit::CreateCaret()
{
    wxClientDC dc(this);

    if(m_pCaret == 0)
    {
        if(GetCaret() == 0)
        {
            wxSize sz(GetCaretSize(dc));
            if(m_bInsert)
            {
                m_pCaret = new wxCaret(this,sz.GetWidth(),1);
            }
            else
            {
                m_pCaret = new wxCaret(this,1,sz.GetHeight());
            }
            SetCaret(m_pCaret);
        }
        else
        {
            m_pCaret = GetCaret();
        }
        SetCaretPos(dc, wxT(""));
    }
    if(m_pCaret && !m_pCaret->IsVisible())
    {
        m_pCaret->Show();
        SetCaretPos(dc, wxT(""));
    }
}

void wmIpEdit::HideCaret()
{
    if(m_pCaret && m_pCaret->IsVisible())
        m_pCaret->Hide();
}

void wmIpEdit::ReleaseCaret()
{
    m_pCaret = 0;
}

void wmIpEdit::ChangeInsertMode(bool bInsert, wxDC& dc)
{
    m_bInsert = bInsert;
    if(m_pCaret)
    {
        wxSize sz(GetCaretSize(dc));
        if(m_bInsert)
        {
            m_pCaret->SetSize(sz.GetWidth(),1);
        }
        else
        {
            m_pCaret->SetSize(1,sz.GetHeight());
        }
    }
}

wxSize wmIpEdit::GetCaretSize(wxDC& dc)
{
    dc.SetFont(GetFont());
    wxSize sz;
    return dc.GetTextExtent(wxT("A"));

}

void wmIpEdit::SetCaretPos(wxDC& dc, const wxString& sText)
{
    if(m_pCaret == NULL || !m_pCaret->IsVisible())
        return;

    dc.SetFont(GetFont());


    if(sText.empty() || m_nInsert == 0)
    {
        wxSize sz = dc.GetTextExtent(wxT("A"));
        int nInsert(0);
        if(m_bInsert)
            nInsert = sz.GetHeight();
        m_pCaret->Move(m_uiText.GetTextRect().GetLeft(),m_uiText.GetTextRect().GetTop()+nInsert+(m_uiText.GetTextRect().GetHeight()-sz.GetHeight())/2);
    }
    else
    {
        wxSize sz = dc.GetTextExtent(sText.Left(m_nInsert));
        int nInsert(0);
        if(m_bInsert)
            nInsert = sz.GetHeight();

        m_pCaret->Move(m_uiText.GetTextRect().GetLeft()+sz.GetWidth(),m_uiText.GetTextRect().GetTop()+nInsert+(m_uiText.GetTextRect().GetHeight()-sz.GetHeight())/2);
    }
}

void wmIpEdit::SetInsertPos(int nPos)
{
    m_nInsert = nPos;
}

unsigned int wmIpEdit::GetInsertPos() const
{
    return m_nInsert;
}

bool wmIpEdit::Insert() const
{
    return m_bInsert;
}
wxPoint wmIpEdit::GetCaretPos() const
{
    if(m_pCaret)
    {
        return m_pCaret->GetPosition();
    }
    return GetClientRect().GetTopLeft();
}

bool wmIpEdit::SetBackgroundColour(const wxColour& clr)
{
    m_clrBackground[0] = clr;
    Refresh();
}

bool wmIpEdit::SetForegroundColour(const wxColour& clr)
{
    m_clrForeground[0] = clr;
    Refresh();
}

void wmIpEdit::SetNonFocusedStyle(const wxColour& clrBack, const wxColour& clrText, unsigned int nBorder)
{
    if(clrBack.IsOk())
    {
        m_clrBackground[0] = clrBack;
    }
    if(clrText.IsOk())
    {
        m_clrForeground[0] = clrText;
    }
    m_nBorderStyle[0] = nBorder;
    Refresh();
}

void wmIpEdit::SetFocusedStyle(const wxColour& clrBack, const wxColour& clrText, unsigned int nBorder)
{
    if(clrBack.IsOk())
    {
        m_clrBackground[1] = clrBack;
    }
    if(clrText.IsOk())
    {
        m_clrForeground[1] = clrText;
    }
    m_nBorderStyle[1] = nBorder;
    Refresh();
}

void wmIpEdit::SetBorderStyle(unsigned long nNonFocused, unsigned long nFocused)
{
    m_nBorderStyle[0] = nNonFocused;
    m_nBorderStyle[1]= nFocused;
    Refresh();
}

void wmIpEdit::SetFocusedBackground(const wxColour& clr)
{
    if(clr.IsOk())
    {
        m_clrBackground[1] = clr;
        Refresh();
    }
}

void wmIpEdit::SetFocusedForeground(const wxColour& clr)
{
    if(clr.IsOk())
    {
        m_clrForeground[1] = clr;
        Refresh();
    }
}

void wmIpEdit::SetTextAlign(unsigned long nAlign)
{
    m_uiText.SetTextAlign(nAlign);
    Refresh();
}
