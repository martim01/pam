#include "wmtimeedit.h"

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


const unsigned int wmTimeEdit::DATE_NONE       =   0;
const unsigned int wmTimeEdit::DATE_DDMMYYYY   =   1;
const unsigned int wmTimeEdit::DATE_MMDDYYYY   =   2;
const unsigned int wmTimeEdit::DATE_YYYYMMDD   =   3;
const unsigned int wmTimeEdit::DATE_DDMMYY     =   4;
const unsigned int wmTimeEdit::DATE_MMDDYY     =   5;
const unsigned int wmTimeEdit::DATE_YYMMDD     =   6;

const unsigned int wmTimeEdit::TIME_NONE    =   0;
const unsigned int wmTimeEdit::TIME_HM      =   1;
const unsigned int wmTimeEdit::TIME_HMS     =   2;
const unsigned int wmTimeEdit::TIME_HMSmS   =   3;
const unsigned int wmTimeEdit::TIME_HMSFr   =   4;

const unsigned int wmTimeEdit::SKIP      = 0;
const unsigned int wmTimeEdit::DAY_UNIT  = 1;
const unsigned int wmTimeEdit::DAY_TEN   = 2;
const unsigned int wmTimeEdit::MONTH_UNIT= 3;
const unsigned int wmTimeEdit::MONTH_TEN = 4;
const unsigned int wmTimeEdit::YEAR_UNIT = 5;
const unsigned int wmTimeEdit::YEAR_TEN  = 6;
const unsigned int wmTimeEdit::YEAR_CEN  = 7;
const unsigned int wmTimeEdit::YEAR_MIL  = 8;
const unsigned int wmTimeEdit::HOUR_TEN  = 9;
const unsigned int wmTimeEdit::HOUR_UNIT = 10;
const unsigned int wmTimeEdit::MINUTE_TEN= 11;
const unsigned int wmTimeEdit::MINUTE_UNIT=12;
const unsigned int wmTimeEdit::SECOND_TEN =13;
const unsigned int wmTimeEdit::SECOND_UNIT=14;
const unsigned int wmTimeEdit::MILLI_HUND =16;
const unsigned int wmTimeEdit::MILLI_TEN  =17;
const unsigned int wmTimeEdit::MILLI_UNIT =18;
const unsigned int wmTimeEdit::FRAME_TEN  =19;
const unsigned int wmTimeEdit::FRAME_UNIT =20;


const wxString wmTimeEdit::DDMMYYYY = wxT("DDMMYYYY");
const wxString wmTimeEdit::MMDDYYYY = wxT("MMDDYYYY");
const wxString wmTimeEdit::YYYYMMDD = wxT("YYYYMMDD");
const wxString wmTimeEdit::DDMMYY = wxT("DDMMYY");
const wxString wmTimeEdit::MMDDYY = wxT("MMDDYY");
const wxString wmTimeEdit::YYMMDD = wxT("YYMMDD");
const wxString wmTimeEdit::HM = wxT("HM");
const wxString wmTimeEdit::HMS = wxT("HMS");
const wxString wmTimeEdit::HMSmS = wxT("HMSmS");
const wxString wmTimeEdit::HMSFr = wxT("HMSFr");


IMPLEMENT_DYNAMIC_CLASS(wmTimeEdit, wxControl)

BEGIN_EVENT_TABLE(wmTimeEdit, wxControl)
    EVT_PAINT(wmTimeEdit::OnPaint)
    EVT_SIZE(wmTimeEdit::OnSize)
    EVT_MOUSE_EVENTS(wmTimeEdit::OnMouse)
    EVT_MOUSE_CAPTURE_LOST(wmTimeEdit::OnCaptureLost)
    EVT_SET_FOCUS(wmTimeEdit::OnSetFocus)
    EVT_KILL_FOCUS(wmTimeEdit::OnKillFocus)
    EVT_CHAR(wmTimeEdit::OnChar)
END_EVENT_TABLE()

wmTimeEdit::wmTimeEdit() : wxControl(),
    m_bFocus(false),
    m_pCaret(0),
    m_nInsert(0),
    m_bInsert(true)
{
}

wmTimeEdit::~wmTimeEdit()
{
    //dtor
}


wmTimeEdit::wmTimeEdit(wxWindow * pParent, wxWindowID id, const wxString& value, const wxPoint& pos, const wxSize& size,long style, const wxValidator& validator, const wxString& name) : wxControl(),
    m_bFocus(false),
    m_pCaret(0),
    m_nInsert(0),
    m_bInsert(true)
{
    wxString sDate(value.BeforeFirst(wxT(':')));
    wxString sTime(value.AfterFirst(wxT(':')));

    unsigned int nDate(DATE_NONE);
    unsigned int nTime(TIME_NONE);

    if(sDate == DDMMYYYY)
    {
        nDate = DATE_DDMMYYYY;
    }
    else if (sDate == MMDDYYYY)
    {
        nDate = DATE_MMDDYYYY;
    }
    else if (sDate == YYYYMMDD)
    {
        nDate = DATE_YYYYMMDD;
    }
    else if (sDate == DDMMYY)
    {
        nDate = DATE_DDMMYY;
    }
    else if (sDate == MMDDYY)
    {
        nDate = DATE_MMDDYY;
    }
    else if (sDate == YYMMDD)
    {
        nDate = DATE_YYMMDD;
    }
    if (sTime == HM)
    {
        nTime = TIME_HM;
    }
    else if(sTime == HMS)
    {
        nTime = TIME_HMS;
    }
    else if(sTime == HMSmS)
    {
        nTime = TIME_HMSmS;
    }
    else if(sTime == HMSFr)
    {
        nTime = TIME_HMSFr;
    }

    Create(pParent, id, nDate, nTime, 0, false, wxT('-'), wxT(':'), pos, size);
}


wmTimeEdit::wmTimeEdit(wxWindow* pParent, wxWindowID id, unsigned int nDateFormat, unsigned int nTimeFormat, unsigned int nFrames, bool bTimeFirst, wxChar chDate, wxChar chTime, const wxPoint& pos,const wxSize& size) :
    m_bFocus(false),
    m_pCaret(0),
    m_nInsert(0),
    m_bInsert(true)
{
    Create(pParent, id, nDateFormat,nTimeFormat,nFrames,bTimeFirst,chDate,chTime,pos,size);
}

bool wmTimeEdit::Create(wxWindow* pParent, wxWindowID id, unsigned int nDateFormat, unsigned int nTimeFormat, unsigned int nFrames, bool bTimeFirst, wxChar chDate, wxChar chTime, const wxPoint& pos, const wxSize& size, long nStyle)
{


    m_nTimeFormat = nTimeFormat;
    m_nDateFormat = nDateFormat;

    if(m_nTimeFormat == TIME_NONE && m_nDateFormat == DATE_NONE)
        m_nTimeFormat = TIME_HMS;


    m_nFrames = nFrames;
    m_dFrameTime = 1000.0 / static_cast<double>(m_nFrames);

    if(bTimeFirst)
    {
        SetTimeFormat(chTime);
        SetDateFormat(chDate);
    }
    else
    {
        SetDateFormat(chDate);
        SetTimeFormat(chTime);
    }

    SetDateTime(wxDateTime::UNow());



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
    //ChangeInsertMode(true, dc);
    SetInsertPos(0);
    ConvertToDateTime();

    wmFocusManager::Get()->AddControl(this);

    m_clrBackground[0] = *wxWHITE;
    m_clrBackground[1] = *wxWHITE;
    m_clrForeground[0] = *wxBLACK;
    m_clrForeground[1] = *wxBLACK;
    m_nBorderStyle[0] = uiRect::BORDER_DOWN;
    m_nBorderStyle[1] = uiRect::BORDER_DOWN;

    return true;
}

wxSize wmTimeEdit::DoGetBestSize() const
{
    return wxSize(150,25);
}


void wmTimeEdit::SetDateTime(const wxDateTime& dtTime)
{
    m_dtTimestamp = dtTime;
    if(m_nTimeFormat != TIME_HMSFr)
    {
        m_sText = m_dtTimestamp.Format(m_sFormat);
    }
    else
    {
        wxString sTime(m_dtTimestamp.Format(m_sFormat));
        sTime.Left(m_nMilliPos);

        unsigned int nFrames = static_cast<unsigned int>(static_cast<double>(m_dtTimestamp.GetMillisecond())/ m_dFrameTime);
        m_sText.Printf(wxT("%s%02d%s"), sTime.Left(m_nMilliPos).c_str(), nFrames, sTime.Mid(m_nMilliPos+2).c_str());
    }
    m_nInsert = 0;
    Refresh();
}


void wmTimeEdit::SetTimeFormat(wxChar chTime)
{
    wxString sGap;
    if(!m_sFormat.empty())
        sGap = wxT(" ");

    if(sGap.empty() == false)
        m_vType.push_back(SKIP);

    switch(m_nTimeFormat)
    {
        case TIME_HM:
            m_sFormat << sGap << wxT("%H") << chTime << wxT("%M");

            m_vType.push_back(HOUR_TEN);
            m_vType.push_back(HOUR_UNIT);
            m_vType.push_back(SKIP);
            m_vType.push_back(MINUTE_TEN);
            m_vType.push_back(MINUTE_UNIT);
            break;
        case TIME_HMS:
            m_sFormat << sGap << wxT("%H") << chTime << wxT("%M") << chTime << wxT("%S");

            m_vType.push_back(HOUR_TEN);
            m_vType.push_back(HOUR_UNIT);
            m_vType.push_back(SKIP);
            m_vType.push_back(MINUTE_TEN);
            m_vType.push_back(MINUTE_UNIT);
            m_vType.push_back(SKIP);
            m_vType.push_back(SECOND_TEN);
            m_vType.push_back(SECOND_UNIT);
            break;
        case TIME_HMSmS:
            m_sFormat << sGap << wxT("%H") << chTime << wxT("%M") << chTime << wxT("%S") << chTime << wxT("%l");

            m_vType.push_back(HOUR_TEN);
            m_vType.push_back(HOUR_UNIT);
            m_vType.push_back(SKIP);
            m_vType.push_back(MINUTE_TEN);
            m_vType.push_back(MINUTE_UNIT);
            m_vType.push_back(SKIP);
            m_vType.push_back(SECOND_TEN);
            m_vType.push_back(SECOND_UNIT);
            m_vType.push_back(SKIP);
            m_nMilliPos = m_vType.size();
            m_vType.push_back(MILLI_HUND);
            m_vType.push_back(MILLI_TEN);
            m_vType.push_back(MILLI_UNIT);
            break;
        case TIME_HMSFr:
            m_sFormat << sGap << wxT("%H") << chTime << wxT("%M") << chTime << wxT("%S ");

            m_vType.push_back(HOUR_TEN);
            m_vType.push_back(HOUR_UNIT);
            m_vType.push_back(SKIP);
            m_vType.push_back(MINUTE_TEN);
            m_vType.push_back(MINUTE_UNIT);
            m_vType.push_back(SKIP);
            m_vType.push_back(SECOND_TEN);
            m_vType.push_back(SECOND_UNIT);
            m_vType.push_back(SKIP);
            m_nMilliPos = m_vType.size();
            m_vType.push_back(FRAME_TEN);
            m_vType.push_back(FRAME_UNIT);
            break;
    }
}

void wmTimeEdit::SetDateFormat(wxChar chDate)
{
    wxString sGap;
    if(!m_sFormat.empty())
        sGap = wxT(" ");

    if(sGap.empty() == false)
        m_vType.push_back(SKIP);

    switch(m_nDateFormat)
    {
        case DATE_DDMMYYYY:
            m_sFormat << sGap << wxT("%d") << chDate << wxT("%m") << chDate << wxT("%Y");

            m_nDayPos = m_vType.size();
            m_vType.push_back(DAY_TEN);
            m_vType.push_back(DAY_UNIT);
            m_vType.push_back(SKIP);

            m_nMonthPos = m_vType.size();
            m_vType.push_back(MONTH_TEN);
            m_vType.push_back(MONTH_UNIT);
            m_vType.push_back(SKIP);

            m_nYearPos = m_vType.size();
            m_vType.push_back(YEAR_MIL);
            m_vType.push_back(YEAR_CEN);
            m_vType.push_back(YEAR_TEN);
            m_vType.push_back(YEAR_UNIT);
            break;
        case DATE_MMDDYYYY:

            m_sFormat << sGap << wxT("%m") << chDate << wxT("%d") << chDate << wxT("%Y");

            m_nMonthPos = m_vType.size();
            m_vType.push_back(MONTH_TEN);
            m_vType.push_back(MONTH_UNIT);
            m_vType.push_back(SKIP);

            m_nDayPos = m_vType.size();
            m_vType.push_back(DAY_TEN);
            m_vType.push_back(DAY_UNIT);
            m_vType.push_back(SKIP);

            m_nYearPos = m_vType.size();
            m_vType.push_back(YEAR_MIL);
            m_vType.push_back(YEAR_CEN);
            m_vType.push_back(YEAR_TEN);
            m_vType.push_back(YEAR_UNIT);
            break;
        case DATE_YYYYMMDD:
            m_sFormat << sGap << wxT("%Y") << chDate << wxT("%m") << chDate << wxT("%d");

            m_nYearPos = m_vType.size();
            m_vType.push_back(YEAR_MIL);
            m_vType.push_back(YEAR_CEN);
            m_vType.push_back(YEAR_TEN);
            m_vType.push_back(YEAR_UNIT);
            m_vType.push_back(SKIP);

            m_nMonthPos = m_vType.size();
            m_vType.push_back(MONTH_TEN);
            m_vType.push_back(MONTH_UNIT);
            m_vType.push_back(SKIP);

            m_nDayPos = m_vType.size();
            m_vType.push_back(DAY_TEN);
            m_vType.push_back(DAY_UNIT);
            break;
        case DATE_DDMMYY:
            m_sFormat << sGap << wxT("%d") << chDate << wxT("%m") << chDate << wxT("%y");

            m_nDayPos = m_vType.size();
            m_vType.push_back(DAY_TEN);
            m_vType.push_back(DAY_UNIT);
            m_vType.push_back(SKIP);

            m_nMonthPos = m_vType.size();
            m_vType.push_back(MONTH_TEN);
            m_vType.push_back(MONTH_UNIT);
            m_vType.push_back(SKIP);

            m_nYearPos = m_vType.size();
            m_vType.push_back(YEAR_TEN);
            m_vType.push_back(YEAR_UNIT);
            break;
        case DATE_MMDDYY:
            m_sFormat << sGap << wxT("%m") << chDate << wxT("%d") << chDate << wxT("%y");

            m_nMonthPos = m_vType.size();
            m_vType.push_back(MONTH_TEN);
            m_vType.push_back(MONTH_UNIT);
            m_vType.push_back(SKIP);

            m_nDayPos = m_vType.size();
            m_vType.push_back(DAY_TEN);
            m_vType.push_back(DAY_UNIT);
            m_vType.push_back(SKIP);

            m_nYearPos = m_vType.size();
            m_vType.push_back(YEAR_TEN);
            m_vType.push_back(YEAR_UNIT);
            break;
        case DATE_YYMMDD:
            m_sFormat << sGap << wxT("%y") << chDate << wxT("%m") << chDate << wxT("%d");

            m_nYearPos = m_vType.size();
            m_vType.push_back(YEAR_TEN);
            m_vType.push_back(YEAR_UNIT);
            m_vType.push_back(SKIP);

            m_nMonthPos = m_vType.size();
            m_vType.push_back(MONTH_TEN);
            m_vType.push_back(MONTH_UNIT);
            m_vType.push_back(SKIP);

            m_nDayPos = m_vType.size();
            m_vType.push_back(DAY_TEN);
            m_vType.push_back(DAY_UNIT);
            break;
    }
}

void wmTimeEdit::OnChar(wxKeyEvent& event)
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
                    if(m_vType[GetInsertPos()-1] == SKIP)
                        SetInsertPos(GetInsertPos()-2);
                    else
                        SetInsertPos(GetInsertPos()-1);
                }
                break;
            case WXK_RIGHT:
            case WXK_NUMPAD_RIGHT:
                if(GetInsertPos()+1 < m_sText.length())
                {
                    if(m_vType[GetInsertPos()+1] == SKIP)
                        SetInsertPos(GetInsertPos()+2);
                    else
                        SetInsertPos(GetInsertPos()+1);
                }
                break;
            case WXK_UP:
            case WXK_NUMPAD_UP:
                MoveTime(1);
                break;
            case WXK_DOWN:
            case WXK_NUMPAD_DOWN:
                MoveTime(-1);
                break;
            case WXK_HOME:
            case WXK_NUMPAD_HOME:
                SetInsertPos(0);
                break;
            case WXK_END:
            case WXK_NUMPAD_END:
                SetInsertPos(m_sText.length()-1);
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

bool wmTimeEdit::Enable(bool bEnable)
{
    bool bReturn(wxControl::Enable(bEnable));
    Refresh();
    return bReturn;
}


void wmTimeEdit::Char(wxChar chChar)
{

	if(chChar < wxT('0') || chChar > wxT('9'))	//numbers only
		return;

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

void wmTimeEdit::CheckDays()
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


void wmTimeEdit::MoveTime(int nWay)
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


void wmTimeEdit::SetString(const wxDateTime& dt)
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



bool wmTimeEdit::ConvertToDateTime()
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

void wmTimeEdit::OnPaint(wxPaintEvent& event)
{
    wxAutoBufferedPaintDC dc(this);
    dc.SetFont(GetFont());

    m_uiText.SetBackgroundColour(m_clrBackground[m_bFocus]);
    m_uiText.SetForegroundColour(m_clrForeground[m_bFocus]);
    m_uiText.Draw(dc, m_sText, m_nBorderStyle[m_bFocus]);


    SetCaretPos(dc, m_sText);
}

void wmTimeEdit::OnSize(wxSizeEvent& event)
{
    m_uiText.SetRect(GetClientRect());
    Refresh();
}

void wmTimeEdit::OnMouse(wxMouseEvent& event)
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

void wmTimeEdit::OnCaptureLost(wxMouseCaptureLostEvent& event)
{
}

void wmTimeEdit::OnSetFocus(wxFocusEvent& event)
{
    m_bFocus = true;

    CreateCaret();
    Refresh();
}

void wmTimeEdit::OnKillFocus(wxFocusEvent& event)
{
    HideCaret();

    m_bFocus = false;
    Refresh();
}


wxChar wmTimeEdit::MaxDayTen()
{
    return wxT('0') + (wxDateTime::GetNumberOfDays(m_dtTimestamp.GetMonth(), m_dtTimestamp.GetYear(), wxDateTime::Gregorian) / 10);
}


wxChar wmTimeEdit::MaxDayUnit()
{
    return wxT('0') + (wxDateTime::GetNumberOfDays(m_dtTimestamp.GetMonth(), m_dtTimestamp.GetYear(), wxDateTime::Gregorian) % 10);
}




void wmTimeEdit::CreateCaret()
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

void wmTimeEdit::HideCaret()
{
    if(m_pCaret && m_pCaret->IsVisible())
        m_pCaret->Hide();
}

void wmTimeEdit::ReleaseCaret()
{
    m_pCaret = 0;
}

void wmTimeEdit::ChangeInsertMode(bool bInsert, wxDC& dc)
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

wxSize wmTimeEdit::GetCaretSize(wxDC& dc)
{
    dc.SetFont(GetFont());
    wxSize sz;
    return dc.GetTextExtent(wxT("A"));

}

void wmTimeEdit::SetCaretPos(wxDC& dc, const wxString& sText)
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

void wmTimeEdit::SetInsertPos(int nPos)
{
    m_nInsert = nPos;
}

unsigned int wmTimeEdit::GetInsertPos() const
{
    return m_nInsert;
}

bool wmTimeEdit::Insert() const
{
    return m_bInsert;
}
wxPoint wmTimeEdit::GetCaretPos() const
{
    if(m_pCaret)
    {
        return m_pCaret->GetPosition();
    }
    return GetClientRect().GetTopLeft();
}

bool wmTimeEdit::SetBackgroundColour(const wxColour& clr)
{
    m_clrBackground[0] = clr;
    Refresh();
}

bool wmTimeEdit::SetForegroundColour(const wxColour& clr)
{
    m_clrForeground[0] = clr;
    Refresh();
}

void wmTimeEdit::SetNonFocusedStyle(const wxColour& clrBack, const wxColour& clrText, unsigned int nBorder)
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

void wmTimeEdit::SetFocusedStyle(const wxColour& clrBack, const wxColour& clrText, unsigned int nBorder)
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

void wmTimeEdit::SetBorderStyle(unsigned long nNonFocused, unsigned long nFocused)
{
    m_nBorderStyle[0] = nNonFocused;
    m_nBorderStyle[1]= nFocused;
    Refresh();
}

void wmTimeEdit::SetFocusedBackground(const wxColour& clr)
{
    if(clr.IsOk())
    {
        m_clrBackground[1] = clr;
        Refresh();
    }
}

void wmTimeEdit::SetFocusedForeground(const wxColour& clr)
{
    if(clr.IsOk())
    {
        m_clrForeground[1] = clr;
        Refresh();
    }
}

void wmTimeEdit::SetTextAlign(unsigned long nAlign)
{
    m_uiText.SetTextAlign(nAlign);
    Refresh();
}
