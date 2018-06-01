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
#include <wx/tokenzr.h>

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






    wxClientDC dc(this);
    m_bInsert = true;
    SetInsertPos(0);

    wmFocusManager::Get()->AddControl(this);

    m_clrBackground[0] = *wxWHITE;
    m_clrBackground[1] = *wxWHITE;
    m_clrForeground[0] = *wxBLACK;
    m_clrForeground[1] = *wxBLACK;
    m_nBorderStyle[0] = uiRect::BORDER_DOWN;
    m_nBorderStyle[1] = uiRect::BORDER_DOWN;

    m_vType.resize(15);
    m_vType[0] = HUNDREDS;
    m_vType[1] = TENS;
    m_vType[2] = UNITS;
    m_vType[3] = DOT;
    m_vType[4] = HUNDREDS;
    m_vType[5] = TENS;
    m_vType[6] = UNITS;
    m_vType[7] = 0;
    m_vType[8] = HUNDREDS;
    m_vType[9] = TENS;
    m_vType[10] = UNITS;
    m_vType[11] = 0;
    m_vType[12] = HUNDREDS;
    m_vType[14] = TENS;
    m_vType[14] = UNITS;

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

    int nCharInsert = GetInsertPos();
    if(chChar == wxT('.'))
    {
        switch(m_vType[GetInsertPos()])
        {
            case HUNDREDS:
                nCharInsert += 4;
                break;
            case TENS:
                nCharInsert += 3;
                break;
            case UNITS:
                nCharInsert += 2;
                break;
        }

        if(nCharInsert > m_vType.size())
        {
            nCharInsert = 0;
        }
        SetInsertPos(nCharInsert);
    }
    else if(chChar >= wxT('0') && chChar <= wxT('9'))
    {

        switch(m_vType[GetInsertPos()])
        {
            case HUNDREDS:
                if(chChar > wxT('2'))	//2
                {
                    return;
                }
                if(chChar == wxT('2') && m_sText.GetChar(GetInsertPos()+1) > wxT('5'))
                {
                    m_sText.SetChar(GetInsertPos()+1, wxT('5'));
                }
                if(chChar == wxT('2') && m_sText.GetChar(GetInsertPos()+1) == wxT('5') && m_sText.GetChar(GetInsertPos()+2) > wxT('5'))
                {
                    m_sText.SetChar(GetInsertPos()+1, wxT('5'));
                }
                break;
            case TENS:
                if(m_sText.GetChar(GetInsertPos()-1) == wxT('2'))
                {
                    if(chChar > wxT('5'))	//24
                    {
                        return;
                    }
                    else if(chChar == wxT('5') &&  m_sText.GetChar(GetInsertPos()+1) > wxT('5'))
                    {
                        m_sText.SetChar(GetInsertPos()+1, wxT('5'));
                    }
                }
                break;
            case UNITS:
                if(m_sText.GetChar(GetInsertPos()-2) == wxT('2') && m_sText.GetChar(GetInsertPos()-1) == 5 && chChar > wxT('5'))
                {
                    return;
                }
                SetInsertPos(GetInsertPos()+1);
                break;
            default:
                return;
        }

        m_sText.SetChar(nCharInsert,chChar);
        SetInsertPos(GetInsertPos()+1);
    }
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
    unsigned int nWidth = GetClientRect().GetWidth()/4;
    for(int i = 0; i < 4; i++)
    {
        m_uiText[i].SetRect(nWidth*i, GetClientRect().GetTop(), nWidth, GetClientRect().GetHeight());
        m_uiText[i].SetGradient(false);
        m_uiText[i].SetTextAlign(wxALIGN_LEFT | wxALIGN_CENTER_VERTICAL);
        m_uiText[i].SetBackgroundColour(*wxWHITE);
        m_uiText[i].SetForegroundColour(*wxBLACK);
    }
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

void wmIpEdit::SetValue(const wxString& sIpAddress)
{
    //@todo check it is ok
    if(sIpAddress.empty())
    {
        m_sText = "   .   .   .   ";
    }
    else
    {
        wxArrayString asTest = wxStringTokenize(sIpAddress, wxT("."));
        if(asTest.GetCount() == 4)
        {
            unsigned long nCheck;
            for(int i = 0; i < 4; i++)
            {
                if(asTest[i].ToULong(&nCheck) == false || nCheck > 255)
                {
                    m_sText = "   .   .   .   ";
                    Refresh();
                    return;
                }
            }
        }
        m_sText = sIpAddress;
    }
    Refresh();
}
