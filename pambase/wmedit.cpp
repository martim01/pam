#include "wmedit.h"

#include <wx/dc.h>
#include <wx/dcclient.h>
#include <wx/dcbuffer.h>
#include <map>
#include <wx/settings.h>
#include <wx/caret.h>
#include <wx/string.h>
#include <wx/app.h>
#include <wx/log.h>
#include <wx/tokenzr.h>
#include "wmfocusmanager.h"

using namespace std;


DEFINE_EVENT_TYPE(wxEVT_TEXT_ESCAPE)
DEFINE_EVENT_TYPE(wxEVT_TEXT_TAB)
DEFINE_EVENT_TYPE(wxEVT_TEXT_BACKSPACE)

#ifdef WXSPAM
IMPLEMENT_DYNAMIC_CLASS(wmEdit, pmControl)
#else
wxIMPLEMENT_DYNAMIC_CLASS(wmEdit, pmControl);
#endif // WXSPAM


BEGIN_EVENT_TABLE(wmEdit, pmControl)
    EVT_PAINT(wmEdit::OnPaint)
    EVT_SIZE(wmEdit::OnSize)
    EVT_MOUSE_EVENTS(wmEdit::OnMouse)
    EVT_MOUSE_CAPTURE_LOST(wmEdit::OnCaptureLost)
    EVT_SET_FOCUS(wmEdit::OnSetFocus)
    EVT_KILL_FOCUS(wmEdit::OnKillFocus)
    EVT_CHAR(wmEdit::OnChar)
    //EVT_KEY_DOWN(wmEdit::OnKeyDown)
    //EVT_KEY_UP(wmEdit::OnKeyUp)
END_EVENT_TABLE()

wmEdit::wmEdit() : pmControl(),
    m_nStyle(0),
    m_bFocus(false),
    m_pCaret(0),
    m_nInsert(0),
    m_bInsert(true),
    m_nValidation(0),
    m_nMaxCharacters(-1)
{
}

wmEdit::~wmEdit()
{
    if(HasCapture())
    {
        ReleaseMouse();
    }
}


wmEdit::wmEdit(wxWindow * pParent, wxWindowID id, const wxString& value, const wxPoint& pos, const wxSize& size,long style, const wxValidator& validator, const wxString& name) : pmControl(),
    m_nStyle(style),
    m_bFocus(false),
    m_pCaret(0),
    m_nInsert(0),
    m_bInsert(true),
    m_nValidation(0),
    m_nMaxCharacters(-1)
{

    Create(pParent, id, value, pos, size, style);
}



bool wmEdit::Create(wxWindow* pParent, wxWindowID id,const wxString& sValue, const wxPoint& pos, const wxSize& size, long nStyle)
{
    m_nStyle = nStyle;
    m_sText = sValue;

    wxSize szInit(size);
    wxSize bestSize = DoGetBestSize();
    if(size.x<=0)
        szInit.SetWidth(bestSize.x);
    if(size.y <= 0)
        szInit.SetHeight(bestSize.y);



    if(!wxWindow::Create(pParent,id,pos,szInit, wxWANTS_CHARS))
        return false;

    SetSize(szInit);

    SetInitialSize(GetSize());

    SetFont(wxFont(10,wxFONTFAMILY_SWISS,wxFONTSTYLE_NORMAL,wxFONTWEIGHT_NORMAL,false,_T("Arial"),wxFONTENCODING_DEFAULT));
    SetInsertPos(m_sText.length());


    SetBackgroundStyle(wxBG_STYLE_CUSTOM);

    m_uiText.SetRect(GetClientRect());
    m_uiText.SetGradient(false);
    m_uiText.SetTextAlign(wxALIGN_LEFT | wxALIGN_CENTER_VERTICAL);

    m_clrBackground[0] = *wxWHITE;
    m_clrBackground[1] = *wxWHITE;
    m_clrForeground[0] = *wxBLACK;
    m_clrForeground[1] = *wxBLACK;
    m_nBorderStyle[0] = uiRect::BORDER_DOWN;
    m_nBorderStyle[1] = uiRect::BORDER_DOWN;

    wxClientDC dc(this);
    ChangeInsertMode(true, dc);
    SetInsertPos(0);

    m_bCapitalize = ((nStyle & wxTE_CAPITALIZE));

    wmFocusManager::Get()->AddControl(this);

//    m_pScroller = new wmScroller(GetEventHandler(), wxSize(20,20));
    return true;
}

void wmEdit::ChangeInsertMode(bool bInsert)
{
    wxClientDC dc(this);
    ChangeInsertMode(bInsert, dc);
}

void wmEdit::SetNonFocusedStyle(const wxColour& clrBack, const wxColour& clrText, unsigned int nBorder)
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

void wmEdit::SetFocusedStyle(const wxColour& clrBack, const wxColour& clrText, unsigned int nBorder)
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

void wmEdit::SetBorderStyle(unsigned long nNonFocused, unsigned long nFocused)
{
    m_nBorderStyle[0] = nNonFocused;
    m_nBorderStyle[1]= nFocused;
    Refresh();
}

void wmEdit::SetFocusedBackground(const wxColour& clr)
{
    if(clr.IsOk())
    {
        m_clrBackground[1] = clr;
        Refresh();
    }
}

void wmEdit::SetFocusedForeground(const wxColour& clr)
{
    if(clr.IsOk())
    {
        m_clrForeground[1] = clr;
        Refresh();
    }
}

wxSize wmEdit::DoGetBestSize() const
{
    return wxSize(150,25);
}

void wmEdit::SetValue(const wxString& sValue)
{
    if(m_bCapitalize)
    {
       m_sText = sValue.Upper();
    }
    else
    {
        m_sText = sValue;
    }
    m_nInsert = m_sText.length();
    if((m_nStyle & wxTE_PASSWORD))
    {
        CreatePassword();
    }
    Refresh();
}

void wmEdit::Capitalize(bool bCapitalize)
{
    m_bCapitalize = bCapitalize;

}

void wmEdit::OnChar(wxKeyEvent& event)
{
    if(IsEnabled() == false)
        return;


    event.ResumePropagation(1);
    wxClientDC dc(this);
    if(m_bFocus)
    {

        switch(event.GetKeyCode())
        {
        case 8:		//backspace
            DeleteChar();
            break;
        case 9:		//tab
            if((m_nStyle & wxTE_PROCESS_TAB)  && m_sText.length() < m_nMaxCharacters)
            {
                UpdateText(AddChar(wxT('\t')));
                ++m_nInsert;
            }
            else
            {
                wxCommandEvent eventTab(wxEVT_TEXT_TAB, GetId());
                eventTab.SetString(m_sText);
                eventTab.SetInt(event.ShiftDown());
                wxPostEvent(GetParent(), eventTab);
            }

            break;
        case 13:	//return
            if((m_nStyle & wxTE_PROCESS_ENTER)  && m_sText.length() < m_nMaxCharacters)
            {
                UpdateText(AddChar(wxT('\n')));
                ++m_nInsert;
            }
            {
                wxCommandEvent eventUp(wxEVT_COMMAND_TEXT_ENTER, GetId());
                eventUp.SetString(m_sText);
                wxPostEvent(GetParent(), eventUp);
            }
            break;
        case WXK_DOWN:
        case WXK_NUMPAD_DOWN:
            break;
        case WXK_UP:
        case WXK_NUMPAD_UP:
            break;
        case WXK_LEFT:
        case WXK_NUMPAD_LEFT:
            if(m_nInsert >0)
                --m_nInsert;
            break;
        case WXK_RIGHT:
        case WXK_NUMPAD_RIGHT:
            if(m_nInsert < m_sText.length())
                ++m_nInsert;
            break;
        case WXK_ESCAPE:		//clears the screen
            UpdateText(wxEmptyString);
            {
                wxCommandEvent event(wxEVT_TEXT_ESCAPE, GetId());
                event.SetString(m_sText);
                wxPostEvent(GetParent(), event);
            }
            m_nInsert = 0;
            break;
        case WXK_PAGEDOWN:
        case WXK_NUMPAD_PAGEDOWN:
            //PageDown();
            break;
        case WXK_PAGEUP:
        case WXK_NUMPAD_PAGEUP:
            //PageUp();
            break;
        case WXK_HOME:
        case WXK_NUMPAD_HOME:
            m_nInsert = 0;
            //Home();
            break;
        case WXK_END:
        case WXK_NUMPAD_END:
            m_nInsert = m_sText.length();
            //End();
            break;
        case WXK_DELETE:
        case WXK_NUMPAD_DELETE:
            if(m_nInsert < m_sText.length())
            {
                wxString sStr(m_sText.Left(m_nInsert));
                sStr += m_sText.Mid(m_nInsert+1);
                UpdateText(sStr);
            }
            break;
        case WXK_INSERT:
        case WXK_NUMPAD_INSERT:
            ChangeInsertMode(!m_bInsert, dc);
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
            if(Validate(static_cast<wxChar>(event.GetKeyCode()-276)) && (m_sText.length() < m_nMaxCharacters || m_bInsert))
            {
                UpdateText(AddChar(static_cast<wxChar>(event.GetKeyCode()-276)));
                ++m_nInsert;
            }
            break;
        default:
            if(Validate(static_cast<wxChar>(event.GetKeyCode())) && (m_sText.length() < m_nMaxCharacters || m_bInsert))
            {
                UpdateText(AddChar(static_cast<wxChar>(event.GetKeyCode())));

                ++m_nInsert;
            }

        }



        CreatePassword();




    	Refresh();
    	Update();
    }
    event.Skip();
}

bool wmEdit::Enable(bool bEnable)
{
    bool bReturn(wxWindow::Enable(bEnable));
    if(!bEnable)
    {
        HideCaret();
    }
    //else
    //{
    //    CreateCaret();
    //}
    Refresh();
    return bReturn;
}

void wmEdit::OnPaint(wxPaintEvent& event)
{

    wxAutoBufferedPaintDC dc(this);
    dc.SetFont(GetFont());
    if(IsEnabled())
    {
        m_uiText.SetBackgroundColour(m_clrBackground[m_bFocus]);
        m_uiText.SetForegroundColour(m_clrForeground[m_bFocus]);
    }
    else
    {
        m_uiText.SetBackgroundColour(wxColour(180,180,180));
        m_uiText.SetForegroundColour(m_clrForeground[false]);
    }

    if((m_nStyle & wxTE_PASSWORD) == 0)
    {
        m_uiText.Draw(dc, m_sText, m_nBorderStyle[m_bFocus]);
        SetCaretPos(dc, m_sText);
    }
    else
    {
        m_uiText.Draw(dc, m_sPassword, m_nBorderStyle[m_bFocus]);
        SetCaretPos(dc, m_sPassword);
    }



}

void wmEdit::OnSize(wxSizeEvent& event)
{
    m_uiText.SetRect(GetClientRect());
    Refresh();
}

void wmEdit::OnMouse(wxMouseEvent& event)
{
    if(event.LeftDown())
    {
        SetFocus();
    }
    else if(event.LeftUp())
    {
        wxCommandEvent event(wxEVT_COMMAND_BUTTON_CLICKED, GetId());
        event.SetEventObject(this);
        event.SetString(m_sText);
        event.SetClientData(m_uiText.GetClientData());
        event.SetExtraLong(m_uiText.GetIntData());
        wxPostEvent(GetEventHandler(), event);

    }
    else
        event.Skip();
}

void wmEdit::OnCaptureLost(wxMouseCaptureLostEvent& event)
{
    ReleaseMouse();
}

void wmEdit::OnSetFocus(wxFocusEvent& event)
{
    m_bFocus = true;

    CreateCaret();
    Refresh();
}

void wmEdit::OnKillFocus(wxFocusEvent& event)
{
    if(wmFocusManager::Get()->AllowedFocus(event.GetWindow()))
    {
        HideCaret();
        m_bFocus = false;
        Refresh();
    }
    else
    {
        SetFocus();
    }
}


void wmEdit::CreateCaret()
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
    if(!m_pCaret->IsVisible())
    {
        m_pCaret->Show();
        SetCaretPos(dc, wxT(""));
    }
}

void wmEdit::HideCaret()
{
    if(m_pCaret && m_pCaret->IsVisible())
        m_pCaret->Hide();
}

void wmEdit::ReleaseCaret()
{
    m_pCaret = 0;
}

void wmEdit::ChangeInsertMode(bool bInsert, wxDC& dc)
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

wxSize wmEdit::GetCaretSize(wxDC& dc)
{
    dc.SetFont(GetFont());
    wxSize sz;
    return dc.GetTextExtent(wxT("A"));

}

void wmEdit::SetCaretPos(wxDC& dc, const wxString& sText)
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

void wmEdit::SetInsertPos(int nPos)
{
    m_nInsert = nPos;
    Refresh();
}

unsigned int wmEdit::GetInsertPos() const
{
    return m_nInsert;
}

bool wmEdit::Insert() const
{
    return m_bInsert;
}
wxPoint wmEdit::GetCaretPos() const
{
    if(m_pCaret)
    {
        return m_pCaret->GetPosition();
    }
    return GetClientRect().GetTopLeft();
}

bool wmEdit::Validate(const wxChar& ch)
{
    switch(m_nValidation)
    {
        case ASCII:
            return (ch >=wxT(' ') && ch <= wxT('~') && AllowChar(ch));
        case ALPHA:
            return (AllowChar(ch) && ((ch >= wxT('A') && ch <= wxT('Z')) || ch == wxT(' ') || (ch >= wxT('a') && ch <= wxT('z'))));
        case ALPHANUMERIC:
            return (AllowChar(ch) && ((ch >= wxT('A') && ch <= wxT('Z')) || ch == wxT(' ') || (ch >= wxT('a') && ch <= wxT('z')) || (ch >= wxT('0') && ch <= wxT('9'))));
        case INTEGER:
        {
            if(m_sText.empty() && (ch == wxT('-') || (int)ch == WXK_NUMPAD_SUBTRACT))
                return true;
            long nCheck;
            return (AllowChar(ch) && AddChar(ch).ToLong(&nCheck));
        }
        case DIGITS:
            return (AllowChar(ch) && ch >= wxT('0') && ch <= wxT('9'));
        case REAL_NUMBER:
        {
            if(m_sText.empty() && (ch == wxT('-') || (int)ch == WXK_NUMPAD_SUBTRACT || ch==wxT('.') || (int)ch==WXK_NUMPAD_DECIMAL))
                return true;

            double dCheck;
            return (AllowChar(ch) && AddChar(ch).ToDouble(&dCheck));
        }
        case IP:
        {
            if((m_sText.empty() || m_sText.GetChar(m_sText.length()-1) == wxT('.')) && ch == wxT('.'))
                return false;

            if(ch==wxT('.') || ch >= wxT('0') && ch <= wxT('9'))
            {
                unsigned long nIP;
                if(m_sText.empty() || (m_sText.ToULong(&nIP) && nIP <= 255))
                {
                    return true;
                }
            }
            return false;
        }
    }
    return true;
}

bool wmEdit::AllowChar(const wxChar& ch)
{
    if(m_setCheck.empty())
        return true;

    set<wxChar>::iterator it = m_setCheck.find(ch);

    if(m_bExclude && it == m_setCheck.end())
        return true;
    if(!m_bExclude && it != m_setCheck.end())
        return true;

    ::wxBell();
    return false;
}

void wmEdit::SetExclude(const wxString& sExclude, bool bExclude)
{
    m_setCheck.clear();
    for(size_t i = 0; i < sExclude.length(); i++)
    {
        m_setCheck.insert(sExclude[i]);
    }
    m_bExclude = bExclude;
}

void wmEdit::CreatePassword()
{
    if(m_nStyle & wxTE_PASSWORD)
    {
        m_sPassword = wxT("");
        for(size_t i = 0; i < m_sText.length(); i++)
            m_sPassword += wxT("*");
    }
}

wxString wmEdit::AddChar(wxChar ch)
{

    wxString sStr(m_sText.Left(m_nInsert));
    sStr << ch;
    if(!m_bInsert)
        sStr << m_sText.Mid(m_nInsert);
    else if(m_nInsert +1 < m_sText.length())
        sStr << m_sText.Mid(m_nInsert+1);
    return sStr;
}


void wmEdit::UpdateText(const wxString& sText)
{
    if(m_bCapitalize)
    {
       m_sText = sText.Upper();
    }
    else
    {
        m_sText = sText;
    }
    wxCommandEvent event(wxEVT_COMMAND_TEXT_UPDATED, GetId());
    event.SetString(m_sText);
    wxPostEvent(GetParent(), event);
}

bool wmEdit::SetBackgroundColour(const wxColour& clr)
{
    m_clrBackground[0] = clr;
    Refresh();
    return true;
}

bool wmEdit::SetForegroundColour(const wxColour& clr)
{
    m_clrForeground[0] = clr;
    Refresh();
    return true;
}


//void wmEdit::OnKeyDown(wxKeyEvent& event)
//{
//    //OnChar(event);
//    event.Skip();
//}
//
//void wmEdit::OnKeyUp(wxKeyEvent& event)
//{
//    event.Skip();
//}


void wmEdit::DeleteChar()
{
    if(m_sText.empty() == false && m_nInsert >0)
    {
        wxString sStr(m_sText.Left(m_nInsert-1));
        sStr += m_sText.Mid(m_nInsert);
        UpdateText(sStr);
        --m_nInsert;
    }
    else if(m_nInsert == 0)
    {
        wxCommandEvent eventUp(wxEVT_TEXT_BACKSPACE, GetId());
        wxPostEvent(GetParent(), eventUp);
    }
}
