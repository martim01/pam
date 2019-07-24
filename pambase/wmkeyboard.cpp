#include "wmkeyboard.h"
#include <wx/window.h>
#include <wx/dcbuffer.h>
#include <wx/log.h>

#include "icons/backspace.xpm"
#include "icons/shift.xpm"
#include "icons/shifted.xpm"
#include "icons/down.xpm"
#include "icons/up.xpm"
#include "icons/left.xpm"
#include "icons/right.xpm"

using namespace std;

BEGIN_EVENT_TABLE(wmKeyboard, pmControl)
    EVT_PAINT(wmKeyboard::OnPaint)
    EVT_SIZE(wmKeyboard::OnSize)
    EVT_LEFT_DOWN(wmKeyboard::OnLeftDown)
    EVT_LEFT_UP(wmKeyboard::OnLeftUp)
    EVT_LEFT_DCLICK(wmKeyboard::OnLeftDown)
    EVT_MOUSE_CAPTURE_LOST(wmKeyboard::OnCaptureLost)
    EVT_TIMER(wxID_ANY, wmKeyboard::OnTimer)
END_EVENT_TABLE()

#ifdef WXSPAM
IMPLEMENT_DYNAMIC_CLASS(wmKeyboard, pmControl)
#else
wxIMPLEMENT_DYNAMIC_CLASS(wmKeyboard, pmControl);
#endif // WXSPAM

wmKeyboard::wmKeyboard() : pmControl()
{

}

wmKeyboard::~wmKeyboard()
{
    if(HasCapture())
    {
        ReleaseMouse();
    }
}

wmKeyboard::wmKeyboard(wxWindow* pParent, wxWindowID id, const wxPoint& pos, const wxSize& size, unsigned int nStyle, bool bHints) : pmControl()
{
    Create(pParent,id,pos,size, nStyle, bHints);
}

bool wmKeyboard::Create(wxWindow* pParent, wxWindowID id, const wxPoint& pos, const wxSize& size,unsigned int nStyle, bool bHints)
{
    wxSize szInit(size);
    wxSize bestSize = DoGetBestSize();
    if(size.x<=0)
        szInit.SetWidth(bestSize.x);
    if(size.y <= 0)
        szInit.SetHeight(bestSize.y);

    if(!wxWindow::Create(pParent,id,pos,szInit,wxWANTS_CHARS, wxEmptyString))
        return false;

    SetFont(wxFont(10,wxFONTFAMILY_SWISS,wxFONTSTYLE_NORMAL,wxFONTWEIGHT_NORMAL,false,_T("Arial"),wxFONTENCODING_DEFAULT));

    SetBackgroundStyle(wxBG_STYLE_CUSTOM);
    SetBackgroundColour(*wxBLACK);

    m_nStyle = nStyle;
    m_bHints = bHints;
    CreateRects();

    m_nMode = MODE_NORMAL;
    m_itDown = m_lstButtons.end();

    m_timerRepeat.SetOwner(this);


    return true;
}

wxSize wmKeyboard::DoGetBestSize() const
{
    return wxSize(600,300);
}



void wmKeyboard::OnPaint(wxPaintEvent& event)
{
    wxBufferedPaintDC dc(this);
    Draw(dc);

}

void wmKeyboard::OnSize(wxSizeEvent& event)
{
    CreateRects();
    Refresh();
}

void wmKeyboard::OnLeftDown(wxMouseEvent& event)
{
    CaptureMouse();

    m_itDown = m_lstButtons.end();
    for(list<button>::iterator itButton = m_lstButtons.begin(); itButton != m_lstButtons.end(); ++itButton)
    {
        if((*itButton).rectEnclosing.Contains(event.GetPosition()))// && (*itButton).bEnabled)
        {
            m_itDown = itButton;

            switch((*itButton).nType)
            {
                case KEY_SHIFT:
                    if(m_nMode == MODE_NORMAL)
                    {
                        m_nMode = MODE_SHIFT;
                    }
                    else if(m_nMode == MODE_SHIFT)
                    {
                        m_nMode = MODE_NORMAL;
                    }
                    else if(m_nMode == MODE_SYMBOL)
                    {
                        m_nMode = MODE_FN;
                    }
                    else
                    {
                        m_nMode = MODE_SYMBOL;
                    }
                    break;
                case KEY_ABC:
                    if(m_nMode == MODE_NORMAL || m_nMode == MODE_SHIFT)
                    {
                        m_nMode = MODE_SYMBOL;
                    }
                    else if(m_nMode == MODE_SYMBOL || m_nMode == MODE_FN)
                    {
                        m_nMode = MODE_NORMAL;
                    }
                    break;
                case KEY_RETURN:
                    SendKeyEvent(wxEVT_KEY_DOWN, WXK_RETURN);
                    SendKeyEvent(wxEVT_CHAR, WXK_RETURN);
                    break;
                case KEY_BACK:
                    SendKeyEvent(wxEVT_KEY_DOWN, WXK_BACK);
                    SendKeyEvent(wxEVT_CHAR, WXK_BACK);
                    m_timerRepeat.Start(250,true);
                    break;
                case KEY_ESCAPE:
                    SendKeyEvent(wxEVT_KEY_DOWN, WXK_ESCAPE);
                    SendKeyEvent(wxEVT_CHAR, WXK_ESCAPE);
                    break;
                case KEY_NAV:
                    SendKeyEvent(wxEVT_KEY_DOWN, (*itButton).nKey[0]);
                    SendKeyEvent(wxEVT_CHAR, (*itButton).nKey[0]);
                    m_timerRepeat.Start(250,true);
                    break;
                default:
                    m_timerRepeat.Start(500,true);
                   // SendKeyEvent(wxEVT_KEY_DOWN);
                    //SendKeyEvent(wxEVT_CHAR);
                    break;
            }

            Refresh();
            Update();
            break;
        }
    }
    //event.Skip();
}

void wmKeyboard::SendKeyEvent(wxEventType evType)
{
    SendKeyEvent(evType,(*m_itDown).nKey[m_nMode]);
 }

void wmKeyboard::SendKeyEvent(wxEventType evType, unsigned int nKeyCode)
{
    wxKeyEvent eventDown(evType);//, GetId());
    eventDown.m_keyCode = nKeyCode;
    eventDown.m_shiftDown = (m_nMode == MODE_SHIFT);
    wxWindow* pWNd = wxWindow::FindFocus();

    if(pWNd)
    {
        wxPostEvent(pWNd,eventDown);
    }
}

void wmKeyboard::OnLeftUp(wxMouseEvent& event)
{
    if(HasCapture())
    {
        ReleaseMouse();
    }

    if(m_itDown != m_lstButtons.end())
    {
        if((*m_itDown).rectEnclosing.Contains(event.GetPosition()))
        {
            switch((*m_itDown).nType)
            {
                case KEY_RETURN:
                    SendKeyEvent(wxEVT_KEY_UP, WXK_RETURN);
                    break;
                case KEY_BACK:
                    SendKeyEvent(wxEVT_KEY_UP, WXK_BACK);
                    break;
                case KEY_NAV:
                    SendKeyEvent(wxEVT_KEY_UP, (*m_itDown).nKey[0]);
                    break;
                case KEY_ESCAPE:
                    SendKeyEvent(wxEVT_KEY_UP, WXK_ESCAPE);
                    break;
                case KEY_ABC:
                case KEY_SHIFT:
                    break;
                default:
                    SendKeyEvent(wxEVT_KEY_DOWN);
                    SendKeyEvent(wxEVT_CHAR);
                    SendKeyEvent(wxEVT_KEY_UP);
                    if(m_nMode == MODE_SHIFT)
                    {
                        m_nMode = MODE_NORMAL;
                    }
                    break;
            }
        }
        else //could be swiping
        {
            if(m_nMode == MODE_NORMAL)
            {

                if(event.GetPosition().x >= (*m_itDown).rectEnclosing.GetLeft() && event.GetPosition().y <= (*m_itDown).rectEnclosing.GetRight())
                {
                    if(event.GetPosition().y < (*m_itDown).rectEnclosing.GetTop() && (*m_itDown).sSwipe[SWIPE_UP].empty() == false)
                    {
                        SendKeyEvent(wxEVT_KEY_DOWN, (*m_itDown).sSwipe[SWIPE_UP].GetChar(0));
                        SendKeyEvent(wxEVT_CHAR, (*m_itDown).sSwipe[SWIPE_UP].GetChar(0));
                        SendKeyEvent(wxEVT_KEY_UP, (*m_itDown).sSwipe[SWIPE_UP].GetChar(0));
                    }
                    else if((*m_itDown).sSwipe[SWIPE_DOWN].empty() == false)
                    {
                        SendKeyEvent(wxEVT_KEY_DOWN, (*m_itDown).sSwipe[SWIPE_DOWN].GetChar(0));
                        SendKeyEvent(wxEVT_CHAR, (*m_itDown).sSwipe[SWIPE_DOWN].GetChar(0));
                        SendKeyEvent(wxEVT_KEY_UP, (*m_itDown).sSwipe[SWIPE_DOWN].GetChar(0));
                    }
                }
                else if(event.GetPosition().y >= (*m_itDown).rectEnclosing.GetTop() && event.GetPosition().x <= (*m_itDown).rectEnclosing.GetBottom())
                {
                    if(event.GetPosition().x < (*m_itDown).rectEnclosing.GetLeft() && (*m_itDown).sSwipe[SWIPE_LEFT].empty() == false)
                    {
                        SendKeyEvent(wxEVT_KEY_DOWN, (*m_itDown).sSwipe[SWIPE_LEFT].GetChar(0));
                        SendKeyEvent(wxEVT_CHAR, (*m_itDown).sSwipe[SWIPE_LEFT].GetChar(0));
                        SendKeyEvent(wxEVT_KEY_UP, (*m_itDown).sSwipe[SWIPE_LEFT].GetChar(0));
                    }
                    else if((*m_itDown).sSwipe[SWIPE_RIGHT].empty() == false)
                    {
                        SendKeyEvent(wxEVT_KEY_DOWN, (*m_itDown).sSwipe[SWIPE_RIGHT].GetChar(0));
                        SendKeyEvent(wxEVT_CHAR, (*m_itDown).sSwipe[SWIPE_RIGHT].GetChar(0));
                        SendKeyEvent(wxEVT_KEY_UP, (*m_itDown).sSwipe[SWIPE_RIGHT].GetChar(0));
                    }
                }
            }
        }
        m_itDown = m_lstButtons.end();
        Refresh();
        m_timerRepeat.Stop();
    }
}

void wmKeyboard::OnCaptureLost(wxMouseCaptureLostEvent& event)
{
    m_itDown = m_lstButtons.end();
    m_timerRepeat.Stop();
    Refresh();
}

void wmKeyboard::OnTimer(wxTimerEvent& event)
{
    switch((*m_itDown).nType)
    {
        case KEY_SHIFT:
        case KEY_ESCAPE:
        case KEY_ABC:
        case KEY_RETURN:
            break;
        case KEY_BACK:
        case KEY_NAV:
            SendKeyEvent(wxEVT_CHAR);
            m_timerRepeat.Start(50,true);
            break;
        default:
            SendKeyEvent(wxEVT_KEY_DOWN, (*m_itDown).sSwipe[SWIPE_UP].GetChar(0));
            SendKeyEvent(wxEVT_CHAR, (*m_itDown).sSwipe[SWIPE_UP].GetChar(0));
            SendKeyEvent(wxEVT_KEY_UP, (*m_itDown).sSwipe[SWIPE_UP].GetChar(0));
            m_itDown = m_lstButtons.end();
            Refresh();
           break;
    }


}


void wmKeyboard::Draw(wxDC& dc)
{
    dc.SetFont(GetFont());

    dc.SetBackgroundMode(wxTRANSPARENT);
    dc.SetPen(*wxTRANSPARENT_PEN);
    dc.SetBrush(wxBrush(GetBackgroundColour()));
    dc.DrawRectangle(GetClientRect());


    dc.SetPen(*wxWHITE_PEN);
    dc.SetTextForeground(GetForegroundColour());
    for(list<button>::iterator itButton = m_lstButtons.begin(); itButton != m_lstButtons.end(); ++itButton)
    {
        wxColour clrButtonHigh, clrButtonLow;
        wxColour clrPenHigh, clrPenLow;
        if(itButton == m_itDown)
        {
            clrButtonLow = wxColour(158,177,253);
            clrButtonHigh = wxColour(128,157,223);
            clrPenLow = wxColour(208,227,255);
            clrPenHigh = wxColour(108,137,203);
        }
        else
        {
            switch((*itButton).nType)
            {
                case KEY_OTHER:
                case KEY_SHIFT:
                case KEY_ABC:
                case KEY_BACK:
                case KEY_ESCAPE:
                    clrButtonHigh = wxColour(80,80,80);
                    clrButtonLow =  wxColour(60,60,60);
                    clrPenHigh = wxColour(120,120,120);
                    clrPenLow = wxColour(30,30,30);
                    break;
                case KEY_RETURN:
                    clrButtonHigh = wxColour(80,80,160);
                    clrButtonLow =  wxColour(60,60,140);
                    clrPenHigh = wxColour(120,120,200);
                    clrPenLow = wxColour(40,40,120);
                    break;
                default:
                    clrButtonHigh = wxColour(120,120,120);
                    clrButtonLow =  wxColour(90,90,90);
                    clrPenHigh = wxColour(190,190,190);
                    clrPenLow = wxColour(100,100,100);
                    break;
            }
        }
        dc.GradientFillLinear((*itButton).rectEnclosing, clrButtonHigh, clrButtonLow);
        dc.SetPen(wxPen(clrPenHigh));

        dc.SetClippingRegion((*itButton).rectEnclosing);
        if((*itButton).bmpKey[m_nMode].IsOk())
        {
            int nLeft = (*itButton).rectEnclosing.GetLeft()+((*itButton).rectEnclosing.GetWidth() - (*itButton).bmpKey[m_nMode].GetWidth())/2;
            int nTop = (*itButton).rectEnclosing.GetTop()+((*itButton).rectEnclosing.GetHeight() - (*itButton).bmpKey[m_nMode].GetHeight())/2;

            dc.DrawBitmap((*itButton).bmpKey[m_nMode], nLeft, nTop, true);
        }
        else
        {
            dc.DrawLabel((*itButton).sKey[m_nMode], (*itButton).rectEnclosing, wxALIGN_CENTER);

            if(m_nMode == MODE_NORMAL)
            {
                dc.SetTextForeground(*wxLIGHT_GREY);
                for(short nSwipe = 0; nSwipe < 4; ++nSwipe)
                {
                    dc.DrawLabel((*itButton).sSwipe[nSwipe], (*itButton).rectSwipe[nSwipe], wxALIGN_CENTER);
                }
                dc.SetTextForeground(GetForegroundColour());
            }
        }
        dc.DestroyClippingRegion();
        dc.DrawLine((*itButton).rectEnclosing.GetBottomLeft(), (*itButton).rectEnclosing.GetTopLeft());
        dc.DrawLine((*itButton).rectEnclosing.GetTopLeft(), (*itButton).rectEnclosing.GetTopRight());

        dc.SetPen(wxPen(clrPenLow));
        dc.DrawLine((*itButton).rectEnclosing.GetBottomLeft(), (*itButton).rectEnclosing.GetBottomRight());
        dc.DrawLine((*itButton).rectEnclosing.GetTopRight(), (*itButton).rectEnclosing.GetBottomRight());

    }
}



void wmKeyboard::CreateRects()
{
    switch(m_nStyle)
    {
    case KBS_NORMAL:
        CreateNormal();
        break;
    case KBS_CAPS:
        CreateCaps();
        break;
    case KBS_NUMPAD:
        CreateNumpad();
        break;
    case KBS_PHONE:
        CreatePhone();
        break;
    case KBS_NAVIGATE:
        CreateNavigate();
        break;
    case KBS_CALC:
        CreateCalc();
    }
}

void wmKeyboard::CreateNormal()
{
    int nTotalHeight(CreateHintRect());



    m_lstButtons.clear();
    int nWidth = (GetClientRect().GetWidth()-40)/11;
    nWidth *= 11;
    int nOffset = (GetClientRect().GetWidth()-40-nWidth)/2;

    int nHeight= (nTotalHeight-14)/5;
    nHeight *= 5;
    int nOffsetH = ((nTotalHeight-12)-nHeight)/2;


    wxRect rectButton(nOffset,nOffsetH, (GetClientRect().GetWidth()-40)/11, (nTotalHeight-12)/5);
    rectButton.Offset(rectButton.GetWidth()/2,0);
    m_lstButtons.push_back(button(wxT("1"), wxT("1"), wxT("F1"), wxT("F1"), rectButton));
    rectButton.Offset(rectButton.GetWidth()+4,0);
    m_lstButtons.push_back(button(wxT("2"), wxT("2"), wxT("F2"), wxT("F2"), rectButton));
    rectButton.Offset(rectButton.GetWidth()+4,0);
    m_lstButtons.push_back(button(wxT("3"), wxT("3"), wxT("F3"), wxT("F3"), rectButton));
    rectButton.Offset(rectButton.GetWidth()+4,0);
    m_lstButtons.push_back(button(wxT("4"), wxT("4"), wxT("F4"), wxT("F4"), rectButton));
    rectButton.Offset(rectButton.GetWidth()+4,0);
    m_lstButtons.push_back(button(wxT("5"), wxT("5"), wxT("F5"), wxT("F5"), rectButton));
    rectButton.Offset(rectButton.GetWidth()+4,0);
    m_lstButtons.push_back(button(wxT("6"), wxT("6"), wxT("F6"), wxT("F6"), rectButton));
    rectButton.Offset(rectButton.GetWidth()+4,0);
    m_lstButtons.push_back(button(wxT("7"), wxT("7"), wxT("F7"), wxT("F7"), rectButton));
    rectButton.Offset(rectButton.GetWidth()+4,0);
    m_lstButtons.push_back(button(wxT("8"), wxT("8"), wxT("F8"), wxT("F8"), rectButton));
    rectButton.Offset(rectButton.GetWidth()+4,0);
    m_lstButtons.push_back(button(wxT("9"), wxT("9"), wxT("F9"), wxT("F9"), rectButton));
    rectButton.Offset(rectButton.GetWidth()+4,0);
    m_lstButtons.push_back(button(wxT("0"), wxT("0"), wxT("F10"), wxT("F10"), rectButton));
    rectButton.Offset(rectButton.GetWidth()+4,0);

    rectButton.SetLeft(nOffset);
    rectButton.SetTop(rectButton.GetBottom()+4);

    m_lstButtons.push_back(button(wxT("q"), wxT("Q"), wxT("1"), wxT("F11"), rectButton));
    rectButton.Offset(rectButton.GetWidth()+4,0);

    m_lstButtons.push_back(button(wxT("w"), wxT("W"), wxT("2"), wxT("F12"), rectButton));
    rectButton.Offset(rectButton.GetWidth()+4,0);

    m_lstButtons.push_back(button(wxT("e"), wxT("E"), wxT("3"), wxT("F13"), rectButton));
    rectButton.Offset(rectButton.GetWidth()+4,0);

    m_lstButtons.push_back(button(wxT("r"), wxT("R"), wxT("4"), wxT("F14"), rectButton));
    rectButton.Offset(rectButton.GetWidth()+4,0);

    m_lstButtons.push_back(button(wxT("t"), wxT("T"), wxT("5"), wxT("F15"), rectButton));
    rectButton.Offset(rectButton.GetWidth()+4,0);

    m_lstButtons.push_back(button(wxT("y"), wxT("Y"), wxT("6"), wxT("F16"), rectButton));
    rectButton.Offset(rectButton.GetWidth()+4,0);

    m_lstButtons.push_back(button(wxT("u"), wxT("U"), wxT("7"), wxT("F17"), rectButton));
    rectButton.Offset(rectButton.GetWidth()+4,0);

    m_lstButtons.push_back(button(wxT("i"), wxT("I"), wxT("8"), wxT("F18"), rectButton));
    rectButton.Offset(rectButton.GetWidth()+4,0);

    m_lstButtons.push_back(button(wxT("o"), wxT("O"), wxT("9"), wxT("F19"), rectButton));
    rectButton.Offset(rectButton.GetWidth()+4,0);

    m_lstButtons.push_back(button(wxT("p"), wxT("P"), wxT("0"), wxT("F20"), rectButton));

    rectButton.Offset(rectButton.GetWidth()+4,0);

    m_lstButtons.push_back(button(wxBitmap(backspace_xpm), wxBitmap(backspace_xpm), wxBitmap(backspace_xpm), wxBitmap(backspace_xpm), WXK_BACK, rectButton, KEY_BACK));

    int nRight = rectButton.GetRight();

    rectButton = wxRect((rectButton.GetWidth()/2)+nOffset,rectButton.GetBottom()+4, rectButton.GetWidth(), rectButton.GetHeight());
    m_lstButtons.push_back(button(wxT("a"), wxT("A"), wxT("#"), wxT("F21"), rectButton));
    rectButton.Offset(rectButton.GetWidth()+4,0);
    m_lstButtons.push_back(button(wxT("s"), wxT("S"), wxT("@"), wxT("F22"), rectButton));
    rectButton.Offset(rectButton.GetWidth()+4,0);
    m_lstButtons.push_back(button(wxT("d"), wxT("D"), wxT("%"), wxT("F23"), rectButton));
    rectButton.Offset(rectButton.GetWidth()+4,0);
    m_lstButtons.push_back(button(wxT("f"), wxT("F"), wxT("&"), wxT("F24"), rectButton));
    rectButton.Offset(rectButton.GetWidth()+4,0);
    m_lstButtons.push_back(button(wxT("g"), wxT("G"), wxT("*"), wxT(""), rectButton));
    rectButton.Offset(rectButton.GetWidth()+4,0);
    m_lstButtons.push_back(button(wxT("h"), wxT("H"), wxT("-"), wxT(""), rectButton));
    rectButton.Offset(rectButton.GetWidth()+4,0);
    m_lstButtons.push_back(button(wxT("j"), wxT("J"), wxT("+"), wxT(""), rectButton));
    rectButton.Offset(rectButton.GetWidth()+4,0);
    m_lstButtons.push_back(button(wxT("k"), wxT("K"), wxT("("), wxT(""), rectButton));
    rectButton.Offset(rectButton.GetWidth()+4,0);
    m_lstButtons.push_back(button(wxT("l"), wxT("L"), wxT(")"), wxT(""), rectButton));
    rectButton.Offset(rectButton.GetWidth()+4,0);
    m_lstButtons.push_back(button(wxT("Go"), wxT("Go"), wxT("Go"), wxT("Go"), rectButton, KEY_RETURN));
    m_lstButtons.back().rectEnclosing.SetRight(nRight);

    rectButton.SetLeft(nOffset);
    rectButton.SetTop(rectButton.GetBottom()+4);

    m_lstButtons.push_back(button(wxT("Shft"), wxT("SHFT"), wxT("Fn"), wxT("?123"), rectButton, KEY_SHIFT));
    m_lstButtons.back().bmpKey[0] = wxBitmap(shift_xpm);
    m_lstButtons.back().bmpKey[1] = wxBitmap(shifted_xpm);

    rectButton.Offset(rectButton.GetWidth()+4,0);
    m_lstButtons.push_back(button(wxT("z"), wxT("Z"), wxT("<"), wxT(""), rectButton));
    rectButton.Offset(rectButton.GetWidth()+4,0);
    m_lstButtons.push_back(button(wxT("x"), wxT("X"), wxT(">"), wxT(""), rectButton));
    rectButton.Offset(rectButton.GetWidth()+4,0);
    m_lstButtons.push_back(button(wxT("c"), wxT("C"), wxT("="), wxT(""), rectButton));
    rectButton.Offset(rectButton.GetWidth()+4,0);
    m_lstButtons.push_back(button(wxT("v"), wxT("V"), wxT("'"), wxT(""), rectButton));
    rectButton.Offset(rectButton.GetWidth()+4,0);
    m_lstButtons.push_back(button(wxT("b"), wxT("B"), wxT(";"), wxT(""), rectButton));
    rectButton.Offset(rectButton.GetWidth()+4,0);
    m_lstButtons.push_back(button(wxT("n"), wxT("N"), wxT(","), wxT(""), rectButton));
    rectButton.Offset(rectButton.GetWidth()+4,0);
    m_lstButtons.push_back(button(wxT("m"), wxT("M"), wxT("."), wxT(""), rectButton));
    rectButton.Offset(rectButton.GetWidth()+4,0);
    m_lstButtons.push_back(button(wxT(","), wxT(","), wxT("!"), wxT(""), rectButton));
    rectButton.Offset(rectButton.GetWidth()+4,0);
    m_lstButtons.push_back(button(wxT("."), wxT("."), wxT("?"), wxT(""), rectButton));
    rectButton.Offset(rectButton.GetWidth()+4,0);
    m_lstButtons.push_back(button(wxT("Shft"), wxT("SHFT"), wxT("Fn"), wxT("?123"), rectButton, KEY_SHIFT));
    m_lstButtons.back().bmpKey[0] = wxBitmap(shift_xpm);
    m_lstButtons.back().bmpKey[1] = wxBitmap(shifted_xpm);


    rectButton.SetLeft(nOffset);
    rectButton.SetTop(rectButton.GetBottom()+4);

    m_lstButtons.push_back(button(wxT("?123"), wxT("?123"), wxT("ABC"), wxT("ABC"), rectButton, KEY_ABC));
    rectButton.Offset(rectButton.GetWidth()+4,0);
    m_lstButtons.push_back(button(wxT(":"), wxT(":"), wxT(":"), wxT(":"), rectButton, KEY_BLANK));
    rectButton.Offset(rectButton.GetWidth()+4,0);
    m_lstButtons.push_back(button(wxT("/"), wxT("/"), wxT("/"), wxT("/"), rectButton, KEY_OTHER));
    rectButton.Offset(rectButton.GetWidth()+4,0);
    wxRect rectSpace(rectButton.GetLeft(),rectButton.GetTop(),rectButton.GetWidth()*5+16,rectButton.GetHeight());
    m_lstButtons.push_back(button(wxT(" "), wxT(" "), wxT(" "), wxT(" "), rectSpace));


    rectButton.SetLeft(rectSpace.GetRight()+4);
    m_lstButtons.push_back(button(wxT("-"),wxT("-"), wxT("\""), wxT(""), rectButton, KEY_OTHER));
    rectButton.Offset(rectButton.GetWidth()+4,0);
    m_lstButtons.push_back(button(wxT("_"), wxT("/"),wxT("/"), wxT(""), rectButton, KEY_OTHER));
    rectButton.Offset(rectButton.GetWidth()+4,0);
    m_lstButtons.push_back(button(wxT("Clr"), wxT("Clr"),wxT("Clr"), wxT(""), rectButton, KEY_ESCAPE));

    Refresh();
}


void wmKeyboard::CreateCaps()
{
    int nTotalHeight(CreateHintRect());

    m_lstButtons.clear();
    int nWidth = (GetClientRect().GetWidth()-40)/11;
    nWidth *= 11;
    int nOffset = (GetClientRect().GetWidth()-40-nWidth)/2;

    int nHeight= (nTotalHeight-14)/5;
    nHeight *= 5;
    int nOffsetH = ((nTotalHeight-12)-nHeight)/2;


    wxRect rectButton(nOffset,nOffsetH, (GetClientRect().GetWidth()-40)/11, (nTotalHeight-12)/5);
    rectButton.Offset(rectButton.GetWidth()/2,0);
    m_lstButtons.push_back(button(wxT("1"), wxT("1"), wxT("F1"), wxT("F1"), rectButton));
    rectButton.Offset(rectButton.GetWidth()+4,0);
    m_lstButtons.push_back(button(wxT("2"), wxT("2"), wxT("F2"), wxT("F2"), rectButton));
    rectButton.Offset(rectButton.GetWidth()+4,0);
    m_lstButtons.push_back(button(wxT("3"), wxT("3"), wxT("F3"), wxT("F3"), rectButton));
    rectButton.Offset(rectButton.GetWidth()+4,0);
    m_lstButtons.push_back(button(wxT("4"), wxT("4"), wxT("F4"), wxT("F4"), rectButton));
    rectButton.Offset(rectButton.GetWidth()+4,0);
    m_lstButtons.push_back(button(wxT("5"), wxT("5"), wxT("F5"), wxT("F5"), rectButton));
    rectButton.Offset(rectButton.GetWidth()+4,0);
    m_lstButtons.push_back(button(wxT("6"), wxT("6"), wxT("F6"), wxT("F6"), rectButton));
    rectButton.Offset(rectButton.GetWidth()+4,0);
    m_lstButtons.push_back(button(wxT("7"), wxT("7"), wxT("F7"), wxT("F7"), rectButton));
    rectButton.Offset(rectButton.GetWidth()+4,0);
    m_lstButtons.push_back(button(wxT("8"), wxT("8"), wxT("F8"), wxT("F8"), rectButton));
    rectButton.Offset(rectButton.GetWidth()+4,0);
    m_lstButtons.push_back(button(wxT("9"), wxT("9"), wxT("F9"), wxT("F9"), rectButton));
    rectButton.Offset(rectButton.GetWidth()+4,0);
    m_lstButtons.push_back(button(wxT("0"), wxT("0"), wxT("F10"), wxT("F10"), rectButton));
    rectButton.Offset(rectButton.GetWidth()+4,0);

    rectButton.SetLeft(nOffset);
    rectButton.SetTop(rectButton.GetBottom()+4);

    m_lstButtons.push_back(button(wxT("Q"), wxT("Q"), wxT("1"), wxT("F11"), rectButton));
    rectButton.Offset(rectButton.GetWidth()+4,0);

    m_lstButtons.push_back(button(wxT("W"), wxT("W"), wxT("2"), wxT("F12"), rectButton));
    rectButton.Offset(rectButton.GetWidth()+4,0);

    m_lstButtons.push_back(button(wxT("E"), wxT("E"), wxT("3"), wxT("F13"), rectButton));
    rectButton.Offset(rectButton.GetWidth()+4,0);

    m_lstButtons.push_back(button(wxT("R"), wxT("R"), wxT("4"), wxT("F14"), rectButton));
    rectButton.Offset(rectButton.GetWidth()+4,0);

    m_lstButtons.push_back(button(wxT("T"), wxT("T"), wxT("5"), wxT("F15"), rectButton));
    rectButton.Offset(rectButton.GetWidth()+4,0);

    m_lstButtons.push_back(button(wxT("Y"), wxT("Y"), wxT("6"), wxT("F16"), rectButton));
    rectButton.Offset(rectButton.GetWidth()+4,0);

    m_lstButtons.push_back(button(wxT("U"), wxT("U"), wxT("7"), wxT("F17"), rectButton));
    rectButton.Offset(rectButton.GetWidth()+4,0);

    m_lstButtons.push_back(button(wxT("I"), wxT("I"), wxT("8"), wxT("F18"), rectButton));
    rectButton.Offset(rectButton.GetWidth()+4,0);

    m_lstButtons.push_back(button(wxT("O"), wxT("O"), wxT("9"), wxT("F19"), rectButton));
    rectButton.Offset(rectButton.GetWidth()+4,0);

    m_lstButtons.push_back(button(wxT("P"), wxT("P"), wxT("0"), wxT("F20"), rectButton));

    rectButton.Offset(rectButton.GetWidth()+4,0);

    m_lstButtons.push_back(button(wxBitmap(backspace_xpm), wxBitmap(backspace_xpm), wxBitmap(backspace_xpm), wxBitmap(backspace_xpm), WXK_BACK, rectButton, KEY_BACK));

    int nRight = rectButton.GetRight();

    rectButton = wxRect((rectButton.GetWidth()/2)+nOffset,rectButton.GetBottom()+4, rectButton.GetWidth(), rectButton.GetHeight());
    m_lstButtons.push_back(button(wxT("A"), wxT("A"), wxT("#"), wxT("F21"), rectButton));
    rectButton.Offset(rectButton.GetWidth()+4,0);
    m_lstButtons.push_back(button(wxT("S"), wxT("S"), wxT("@"), wxT("F22"), rectButton));
    rectButton.Offset(rectButton.GetWidth()+4,0);
    m_lstButtons.push_back(button(wxT("D"), wxT("D"), wxT("%"), wxT("F23"), rectButton));
    rectButton.Offset(rectButton.GetWidth()+4,0);
    m_lstButtons.push_back(button(wxT("F"), wxT("F"), wxT("&"), wxT("F24"), rectButton));
    rectButton.Offset(rectButton.GetWidth()+4,0);
    m_lstButtons.push_back(button(wxT("G"), wxT("G"), wxT("*"), wxT(""), rectButton));
    rectButton.Offset(rectButton.GetWidth()+4,0);
    m_lstButtons.push_back(button(wxT("H"), wxT("H"), wxT("-"), wxT(""), rectButton));
    rectButton.Offset(rectButton.GetWidth()+4,0);
    m_lstButtons.push_back(button(wxT("J"), wxT("J"), wxT("+"), wxT(""), rectButton));
    rectButton.Offset(rectButton.GetWidth()+4,0);
    m_lstButtons.push_back(button(wxT("K"), wxT("K"), wxT("("), wxT(""), rectButton));
    rectButton.Offset(rectButton.GetWidth()+4,0);
    m_lstButtons.push_back(button(wxT("L"), wxT("L"), wxT(")"), wxT(""), rectButton));
    rectButton.Offset(rectButton.GetWidth()+4,0);
    m_lstButtons.push_back(button(wxT("Go"), wxT("Go"), wxT("Go"), wxT("Go"), rectButton, KEY_RETURN));
    m_lstButtons.back().rectEnclosing.SetRight(nRight);

    rectButton.SetLeft(nOffset);
    rectButton.SetTop(rectButton.GetBottom()+4);

    m_lstButtons.push_back(button(wxT("Shft"), wxT("SHFT"), wxT("Fn"), wxT("?123"), rectButton, KEY_SHIFT));
    m_lstButtons.back().bmpKey[0] = wxBitmap(shift_xpm);
    m_lstButtons.back().bmpKey[1] = wxBitmap(shifted_xpm);

    rectButton.Offset(rectButton.GetWidth()+4,0);
    m_lstButtons.push_back(button(wxT("Z"), wxT("Z"), wxT("<"), wxT(""), rectButton));
    rectButton.Offset(rectButton.GetWidth()+4,0);
    m_lstButtons.push_back(button(wxT("X"), wxT("X"), wxT(">"), wxT(""), rectButton));
    rectButton.Offset(rectButton.GetWidth()+4,0);
    m_lstButtons.push_back(button(wxT("C"), wxT("C"), wxT("="), wxT(""), rectButton));
    rectButton.Offset(rectButton.GetWidth()+4,0);
    m_lstButtons.push_back(button(wxT("V"), wxT("V"), wxT("'"), wxT(""), rectButton));
    rectButton.Offset(rectButton.GetWidth()+4,0);
    m_lstButtons.push_back(button(wxT("B"), wxT("B"), wxT(";"), wxT(""), rectButton));
    rectButton.Offset(rectButton.GetWidth()+4,0);
    m_lstButtons.push_back(button(wxT("N"), wxT("N"), wxT(","), wxT(""), rectButton));
    rectButton.Offset(rectButton.GetWidth()+4,0);
    m_lstButtons.push_back(button(wxT("M"), wxT("M"), wxT("."), wxT(""), rectButton));
    rectButton.Offset(rectButton.GetWidth()+4,0);
    m_lstButtons.push_back(button(wxT(","), wxT(","), wxT("!"), wxT(""), rectButton));
    rectButton.Offset(rectButton.GetWidth()+4,0);
    m_lstButtons.push_back(button(wxT("."), wxT("."), wxT("?"), wxT(""), rectButton));
    rectButton.Offset(rectButton.GetWidth()+4,0);
    m_lstButtons.push_back(button(wxT("Shft"), wxT("SHFT"), wxT("Fn"), wxT("?123"), rectButton, KEY_SHIFT));
    m_lstButtons.back().bmpKey[0] = wxBitmap(shift_xpm);
    m_lstButtons.back().bmpKey[1] = wxBitmap(shifted_xpm);


    rectButton.SetLeft(nOffset);
    rectButton.SetTop(rectButton.GetBottom()+4);

    m_lstButtons.push_back(button(wxT("?123"), wxT("?123"), wxT("ABC"), wxT("ABC"), rectButton, KEY_ABC));
    rectButton.Offset(rectButton.GetWidth()+4,0);
    m_lstButtons.push_back(button(wxT("@"), wxT("@"), wxT("/"), wxT("@"), rectButton, KEY_BLANK));
    rectButton.Offset(rectButton.GetWidth()+4,0);
    m_lstButtons.push_back(button(wxT("["), wxT("["), wxT(":"), wxT(""), rectButton, KEY_OTHER));
    rectButton.Offset(rectButton.GetWidth()+4,0);
    wxRect rectSpace(rectButton.GetLeft(),rectButton.GetTop(),rectButton.GetWidth()*5+16,rectButton.GetHeight());
    m_lstButtons.push_back(button(wxT(" "), wxT(" "), wxT(" "), wxT(" "), rectSpace));


    rectButton.SetLeft(rectSpace.GetRight()+4);
    m_lstButtons.push_back(button(wxT("-"),wxT("-"), wxT("\""), wxT(""), rectButton, KEY_OTHER));
    rectButton.Offset(rectButton.GetWidth()+4,0);
    m_lstButtons.push_back(button(wxT("_"), wxT("/"),wxT("/"), wxT(""), rectButton, KEY_OTHER));
    rectButton.Offset(rectButton.GetWidth()+4,0);
    m_lstButtons.push_back(button(wxT("Clr"), wxT("Clr"),wxT("Clr"), wxT(""), rectButton, KEY_ESCAPE));

    Refresh();
}

void wmKeyboard::CreateNumpad()
{
    int nTotalHeight(CreateHintRect());

    m_lstButtons.clear();
    int nWidth = (GetClientRect().GetWidth()-12)/3;
    int nWidthUp = nWidth*3;
    int nOffset = (GetClientRect().GetWidth()-12-nWidthUp)/2;

    int nHeight= (nTotalHeight-14)/4;
    int nHeightUp = nHeight*4;
    int nOffsetH = ((nTotalHeight-12)-nHeightUp)/2;


    wxRect rectButton(nOffset,nOffsetH, nWidth, nHeight);
    m_lstButtons.push_back(button(wxT("7"), wxT("7"), wxT("Home"), wxT("Home"), rectButton));
    m_lstButtons.back().nKey[MODE_SYMBOL] = WXK_HOME;
    m_lstButtons.back().nKey[MODE_FN] = WXK_HOME;

    rectButton.Offset(rectButton.GetWidth()+4,0);
    m_lstButtons.push_back(button(wxT("8"), wxT("8"), wxT(""), wxT(""), rectButton));
    m_lstButtons.back().bmpKey[MODE_SYMBOL] = wxBitmap(up_xpm);
    m_lstButtons.back().bmpKey[MODE_FN] = wxBitmap(up_xpm);
    m_lstButtons.back().nKey[MODE_SYMBOL] = WXK_UP;
    m_lstButtons.back().nKey[MODE_FN] = WXK_UP;


    rectButton.Offset(rectButton.GetWidth()+4,0);
    m_lstButtons.push_back(button(wxT("9"), wxT("9"), wxT("Page\nUp"), wxT("Page\nUp"), rectButton));
    m_lstButtons.back().nKey[MODE_SYMBOL] = WXK_PAGEUP;
    m_lstButtons.back().nKey[MODE_FN] = WXK_PAGEUP;
    rectButton.Offset(rectButton.GetWidth()+4,0);

    rectButton.SetLeft(nOffset);
    rectButton.SetTop(rectButton.GetBottom()+4);
    m_lstButtons.push_back(button(wxT("4"), wxT("4"), wxT(""), wxT(""), rectButton));
    m_lstButtons.back().bmpKey[MODE_SYMBOL] = wxBitmap(left_xpm);
    m_lstButtons.back().bmpKey[MODE_FN] = wxBitmap(left_xpm);
    m_lstButtons.back().nKey[MODE_SYMBOL] = WXK_LEFT;
    m_lstButtons.back().nKey[MODE_FN] = WXK_LEFT;

    rectButton.Offset(rectButton.GetWidth()+4,0);
    m_lstButtons.push_back(button(wxT("5"), wxT("5"), wxT("5"), wxT("5"), rectButton));
    rectButton.Offset(rectButton.GetWidth()+4,0);
    m_lstButtons.push_back(button(wxT("6"), wxT("6"), wxT("6"), wxT("6"), rectButton));
    m_lstButtons.back().bmpKey[MODE_SYMBOL] = wxBitmap(right_xpm);
    m_lstButtons.back().bmpKey[MODE_FN] = wxBitmap(right_xpm);
    m_lstButtons.back().nKey[MODE_SYMBOL] = WXK_RIGHT;
    m_lstButtons.back().nKey[MODE_FN] = WXK_RIGHT;

    rectButton.Offset(rectButton.GetWidth()+4,0);

    rectButton.SetLeft(nOffset);
    rectButton.SetTop(rectButton.GetBottom()+4);
    m_lstButtons.push_back(button(wxT("1"), wxT("1"), wxT("End"), wxT("End"), rectButton));
    m_lstButtons.back().nKey[MODE_SYMBOL] = WXK_END;
    m_lstButtons.back().nKey[MODE_FN] = WXK_END;
    rectButton.Offset(rectButton.GetWidth()+4,0);
    m_lstButtons.push_back(button(wxT("2"), wxT("2"), wxT("2"), wxT("2"), rectButton));
    m_lstButtons.back().bmpKey[MODE_SYMBOL] = wxBitmap(down_xpm);
    m_lstButtons.back().bmpKey[MODE_FN] = wxBitmap(down_xpm);
    m_lstButtons.back().nKey[MODE_SYMBOL] = WXK_DOWN;
    m_lstButtons.back().nKey[MODE_FN] = WXK_DOWN;

    rectButton.Offset(rectButton.GetWidth()+4,0);
    m_lstButtons.push_back(button(wxT("3"), wxT("3"), wxT("Page\nDown"), wxT("Page\nDown"), rectButton));
    m_lstButtons.back().nKey[MODE_SYMBOL] = WXK_PAGEDOWN;
    m_lstButtons.back().nKey[MODE_FN] = WXK_PAGEDOWN;
    rectButton.Offset(rectButton.GetWidth()+4,0);

    rectButton.SetLeft(nOffset);
    rectButton.SetTop(rectButton.GetBottom()+4);
    m_lstButtons.push_back(button(wxT("0"), wxT("0"), wxT("0"), wxT("0"), rectButton));
    rectButton.Offset(rectButton.GetWidth()+4,0);
    m_lstButtons.push_back(button(wxT("Go"), wxT("Go"), wxT("Go"), wxT("Go"), rectButton,KEY_RETURN));
    rectButton.Offset(rectButton.GetWidth()+4,0);
    m_lstButtons.push_back(button(wxT("Nav"), wxT("Nav"), wxT("Nav"), wxT("Num"), rectButton, KEY_ABC));
    rectButton.Offset(rectButton.GetWidth()+4,0);

}

void wmKeyboard::CreatePhone()
{
    int nTotalHeight(CreateHintRect());

    m_lstButtons.clear();
    int nWidth = (GetClientRect().GetWidth()-12)/3;
    int nWidthUp = nWidth*3;
    int nOffset = (GetClientRect().GetWidth()-12-nWidthUp)/2;

    int nHeight= (nTotalHeight-14)/4;
    int nHeightUp = nHeight*4;
    int nOffsetH = ((nTotalHeight-12)-nHeightUp)/2;


    wxRect rectButton(nOffset,nOffsetH, nWidth, nHeight);
    m_lstButtons.push_back(button(wxT("1"), wxT("1"), wxT("1"), wxT("1"), rectButton));
    rectButton.Offset(rectButton.GetWidth()+4,0);
    m_lstButtons.push_back(button(wxT("2"), wxT("2"), wxT("2"), wxT("2"), rectButton));
    rectButton.Offset(rectButton.GetWidth()+4,0);
    m_lstButtons.push_back(button(wxT("3"), wxT("3"), wxT("3"), wxT("3"), rectButton));
    rectButton.Offset(rectButton.GetWidth()+4,0);

    rectButton.SetLeft(nOffset);
    rectButton.SetTop(rectButton.GetBottom()+4);
    m_lstButtons.push_back(button(wxT("4"), wxT("4"), wxT("4"), wxT("4"), rectButton));
    rectButton.Offset(rectButton.GetWidth()+4,0);
    m_lstButtons.push_back(button(wxT("5"), wxT("5"), wxT("5"), wxT("5"), rectButton));
    rectButton.Offset(rectButton.GetWidth()+4,0);
    m_lstButtons.push_back(button(wxT("6"), wxT("6"), wxT("6"), wxT("6"), rectButton));
    rectButton.Offset(rectButton.GetWidth()+4,0);

    rectButton.SetLeft(nOffset);
    rectButton.SetTop(rectButton.GetBottom()+4);
    m_lstButtons.push_back(button(wxT("7"), wxT("7"), wxT("7"), wxT("7"), rectButton));
    rectButton.Offset(rectButton.GetWidth()+4,0);
    m_lstButtons.push_back(button(wxT("8"), wxT("8"), wxT("8"), wxT("8"), rectButton));
    rectButton.Offset(rectButton.GetWidth()+4,0);
    m_lstButtons.push_back(button(wxT("9"), wxT("9"), wxT("9"), wxT("9"), rectButton));
    rectButton.Offset(rectButton.GetWidth()+4,0);

    rectButton.SetLeft(nOffset);
    rectButton.SetTop(rectButton.GetBottom()+4);
    m_lstButtons.push_back(button(wxT("#"), wxT("#"), wxT("#"), wxT("#"), rectButton));
    rectButton.Offset(rectButton.GetWidth()+4,0);
    m_lstButtons.push_back(button(wxT("0"), wxT("0"), wxT("0"), wxT("0"), rectButton));
    rectButton.Offset(rectButton.GetWidth()+4,0);
    m_lstButtons.push_back(button(wxT("<-"), wxT("<-"), wxT("<-"), wxT("<-"), rectButton, KEY_BACK));
    rectButton.Offset(rectButton.GetWidth()+4,0);
}

void wmKeyboard::CreateNavigate()
{
    int nTotalHeight(CreateHintRect());

    m_lstButtons.clear();
    int nWidth = (GetClientRect().GetWidth()-12)/3;
    int nWidthUp = nWidth*3;
    int nOffset = (GetClientRect().GetWidth()-12-nWidthUp)/2;

    int nHeight= (nTotalHeight-14)/4;
    int nHeightUp = nHeight*4;
    int nOffsetH = ((nTotalHeight-12)-nHeightUp)/2;


    wxRect rectButton(nOffset,nOffsetH, nWidth, nHeight);
    //m_lstButtons.push_back(button(wxT("Ins"), wxT("1"), wxT("1"), wxT("1"), rectButton));
    rectButton.Offset(rectButton.GetWidth()+4,0);
    m_lstButtons.push_back(button(wxT("Home"), wxT("2"), wxT("2"), wxT("2"), rectButton, KEY_NAV));
    m_lstButtons.back().nKey[0] = WXK_HOME;
    rectButton.Offset(rectButton.GetWidth()+4,0);
    m_lstButtons.push_back(button(wxT("Page\nUp"), wxT("3"), wxT("3"), wxT("3"), rectButton, KEY_NAV));
    m_lstButtons.back().nKey[0] = WXK_PAGEUP;
    rectButton.Offset(rectButton.GetWidth()+4,0);

    rectButton.SetLeft(nOffset);
    rectButton.SetTop(rectButton.GetBottom()+4);
    //m_lstButtons.push_back(button(wxT("4"), wxT("4"), wxT("4"), wxT("4"), rectButton));
    rectButton.Offset(rectButton.GetWidth()+4,0);
    m_lstButtons.push_back(button(wxT("End"), wxT("5"), wxT("5"), wxT("5"), rectButton, KEY_NAV));
    m_lstButtons.back().nKey[0] = WXK_END;
    rectButton.Offset(rectButton.GetWidth()+4,0);
    m_lstButtons.push_back(button(wxT("Page\nDown"), wxT("6"), wxT("6"), wxT("6"), rectButton, KEY_NAV));
    m_lstButtons.back().nKey[0] = WXK_PAGEDOWN;
    rectButton.Offset(rectButton.GetWidth()+4,0);

    rectButton.SetLeft(nOffset);
    rectButton.SetTop(rectButton.GetBottom()+4);
    //m_lstButtons.push_back(button(wxT("7"), wxT("7"), wxT("7"), wxT("7"), rectButton));
    rectButton.Offset(rectButton.GetWidth()+4,0);
    m_lstButtons.push_back(button(wxBitmap(up_xpm), wxBitmap(up_xpm), wxBitmap(up_xpm), wxBitmap(up_xpm), WXK_UP, rectButton, KEY_NAV));
    rectButton.Offset(rectButton.GetWidth()+4,0);
    rectButton.Offset(rectButton.GetWidth()+4,0);

    rectButton.SetLeft(nOffset);
    rectButton.SetTop(rectButton.GetBottom()+4);
    m_lstButtons.push_back(button(wxBitmap(left_xpm), wxBitmap(left_xpm), wxBitmap(left_xpm), wxBitmap(left_xpm), WXK_LEFT, rectButton, KEY_NAV));
    rectButton.Offset(rectButton.GetWidth()+4,0);
    m_lstButtons.push_back(button(wxBitmap(down_xpm), wxBitmap(down_xpm), wxBitmap(down_xpm), wxBitmap(down_xpm), WXK_DOWN, rectButton, KEY_NAV));
    rectButton.Offset(rectButton.GetWidth()+4,0);
    m_lstButtons.push_back(button(wxBitmap(right_xpm), wxBitmap(right_xpm), wxBitmap(right_xpm), wxBitmap(right_xpm), WXK_RIGHT, rectButton, KEY_NAV));
    rectButton.Offset(rectButton.GetWidth()+4,0);
}

void wmKeyboard::CreateCalc()
{
    int nTotalHeight(CreateHintRect());

    m_lstButtons.clear();
    int nWidth = (GetClientRect().GetWidth()-20)/4;
    int nWidthUp = nWidth*4;
    int nOffset = (GetClientRect().GetWidth()-12-nWidthUp)/2;

    int nHeight= (nTotalHeight-14)/4;
    int nHeightUp = nHeight*4;
    int nOffsetH = ((nTotalHeight-12)-nHeightUp)/2;


    wxRect rectButton(nOffset,nOffsetH, nWidth, nHeight);
    m_lstButtons.push_back(button(wxT("7"), wxT("7"), wxT("Home"), wxT("Home"), rectButton));

    rectButton.Offset(rectButton.GetWidth()+4,0);
    m_lstButtons.push_back(button(wxT("8"), wxT("8"), wxT(""), wxT(""), rectButton));

    rectButton.Offset(rectButton.GetWidth()+4,0);
    m_lstButtons.push_back(button(wxT("9"), wxT("9"), wxT("Page\nUp"), wxT("Page\nUp"), rectButton));

    rectButton.Offset(rectButton.GetWidth()+4,0);
    m_lstButtons.push_back(button(wxT("Clr"), wxT("Clr"),wxT("Clr"), wxT(""), rectButton, KEY_ESCAPE));

    rectButton.SetLeft(nOffset);
    rectButton.SetTop(rectButton.GetBottom()+4);
    m_lstButtons.push_back(button(wxT("4"), wxT("4"), wxT(""), wxT(""), rectButton));

    rectButton.Offset(rectButton.GetWidth()+4,0);
    m_lstButtons.push_back(button(wxT("5"), wxT("5"), wxT("5"), wxT("5"), rectButton));

    rectButton.Offset(rectButton.GetWidth()+4,0);
    m_lstButtons.push_back(button(wxT("6"), wxT("6"), wxT("6"), wxT("6"), rectButton));

    rectButton.Offset(rectButton.GetWidth()+4,0);
    m_lstButtons.push_back(button(wxT("<-"), wxT("<-"),wxT("<-"), wxT(""), rectButton, KEY_BACK));

    rectButton.Offset(rectButton.GetWidth()+4,0);

    rectButton.SetLeft(nOffset);
    rectButton.SetTop(rectButton.GetBottom()+4);
    m_lstButtons.push_back(button(wxT("1"), wxT("1"), wxT("End"), wxT("End"), rectButton));

    rectButton.Offset(rectButton.GetWidth()+4,0);
    m_lstButtons.push_back(button(wxT("2"), wxT("2"), wxT("2"), wxT("2"), rectButton));

    rectButton.Offset(rectButton.GetWidth()+4,0);
    m_lstButtons.push_back(button(wxT("3"), wxT("3"), wxT("Page\nDown"), wxT("Page\nDown"), rectButton));

    rectButton.Offset(rectButton.GetWidth()+4,0);
    m_lstButtons.push_back(button(wxT("Go"), wxT("Go"), wxT("Go"), wxT("Go"), wxRect(rectButton.GetLeft(), rectButton.GetTop(), rectButton.GetWidth(), rectButton.GetHeight()*2+4), KEY_RETURN));

    rectButton.SetLeft(nOffset);
    rectButton.SetTop(rectButton.GetBottom()+4);
    m_lstButtons.push_back(button(wxT("0"), wxT("0"), wxT("0"), wxT("0"), rectButton));
    rectButton.Offset(rectButton.GetWidth()+4,0);
    m_lstButtons.push_back(button(wxT("."), wxT("."), wxT("."), wxT("."), rectButton));
    rectButton.Offset(rectButton.GetWidth()+4,0);
    m_lstButtons.push_back(button(wxT("-"), wxT("-"), wxT("-"), wxT("-"), rectButton));
    rectButton.Offset(rectButton.GetWidth()+4,0);

}



unsigned int wmKeyboard::CreateHintRect()
{
    if(m_bHints)
    {
        int nWidth(GetClientSize().GetWidth()/5);
        m_uiHints[0].SetRect(nWidth,0,nWidth,25);
        m_uiHints[1].SetRect(m_uiHints[0].GetRight(),0,nWidth,25);
        m_uiHints[2].SetRect(m_uiHints[1].GetRight(),0,nWidth,25);
        return GetClientSize().GetHeight()-25;
    }
    return GetClientSize().GetHeight();

}


void wmKeyboard::SetReturnString(const wxString& sReturn)
{
    SetButtonText(KEY_RETURN, sReturn);
}

void wmKeyboard::SetButtonText(unsigned short nButtonType, const wxString& sText)
{
    for(list<button>::iterator itButton = m_lstButtons.begin(); itButton != m_lstButtons.end(); ++itButton)
    {
        if((*itButton).nType == nButtonType)
        {
            for(size_t i = 0; i < 4; i++)
            {
                (*itButton).sKey[i] = sText;
            }
            RefreshRect((*itButton).rectEnclosing);
        }
    }

}

void wmKeyboard::SetEscapeString(const wxString& sEscape)
{
    SetButtonText(KEY_ESCAPE, sEscape);
}

void wmKeyboard::SetBlankString(const wxString& sBlank)
{
    SetButtonText(KEY_BLANK, sBlank);
}
