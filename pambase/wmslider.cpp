#include "wmslider.h"
#include <wx/dcbuffer.h>
#include <wx/log.h>

using namespace std;

BEGIN_EVENT_TABLE(wmSlider, pmControl)
    EVT_MOTION(wmSlider::OnMouseMove)
    EVT_LEFT_DOWN(wmSlider::OnLeftDown)
    EVT_LEFT_UP(wmSlider::OnLeftUp)
    EVT_MOUSE_CAPTURE_LOST(wmSlider::OnCaptureLost)
    EVT_PAINT(wmSlider::OnPaint)
END_EVENT_TABLE()

#ifdef WXSPAM
IMPLEMENT_DYNAMIC_CLASS(wmSlider, pmControl)
DEFINE_EVENT_TYPE(wxEVT_SLIDER_MOVE)
#else
wxIMPLEMENT_DYNAMIC_CLASS(wmSlider, pmControl);
wxDEFINE_EVENT(wxEVT_SLIDER_MOVE, wxCommandEvent);
#endif // WXSPAM

wmSlider::wmSlider() : pmControl(),
	m_bDown(false),
	m_dResolution(0),
	m_dMin(0),
	m_dMax(0),
	m_dPosition(0)
{
}

wmSlider::wmSlider(wxWindow *parent, wxWindowID id, const wxString& sLabel, const wxPoint& pos, const wxSize& size, long nStyle, const wxString& name) : pmControl(),
	m_bDown(false),
	m_dResolution(0),
	m_dMin(0),
	m_dMax(0),
	m_dPosition(0)
{

    wxSize szInit(size);
    wxSize bestSize = DoGetBestSize();
    if(size.x<=0)
        szInit.SetWidth(bestSize.x);
    if(size.y <= 0)
        szInit.SetHeight(bestSize.y);

    wxWindow::Create(parent,id,pos,szInit,wxWANTS_CHARS, name);

    SetMinSize(size);


    SetBackgroundStyle(wxBG_STYLE_CUSTOM);

    SetPosition(pos);
    SetSize(size);

    CreateRects();
}




wmSlider::~wmSlider()
{

}

void wmSlider::OnSize(wxSizeEvent& event)
{
    CreateRects();
    Init(m_dMin, m_dMax, m_dPosition);
    Refresh();
}

void wmSlider::CreateRects()
{
    m_uiSlider.SetRect(25,10,GetClientSize().x-50, GetClientSize().y-20);
    m_uiSlider.SetGradient(wxSOUTH);
    m_uiButton.SetRect(0,0,50,GetClientSize().y);
    m_uiButton.SetGradient(wxEAST);
}

void wmSlider::OnPaint(wxPaintEvent& event)
{
    wxBufferedPaintDC dc(this);
    Draw(dc);
}

void wmSlider::Draw(wxDC& dc)
{
    dc.SetPen(*wxTRANSPARENT_PEN);
    dc.SetBrush(*wxBLACK_BRUSH);
    dc.DrawRectangle(GetClientRect());


    m_uiSlider.Draw(dc, uiRect::BORDER_DOWN);
    m_uiButton.Draw(dc, uiRect::BORDER_UP);


}

void wmSlider::OnMouseMove(wxMouseEvent& event)
{
    if(event.LeftIsDown() && m_bDown)
    {
        MoveSlider(event.GetPosition().x);
    }
}

void wmSlider::OnLeftDown(wxMouseEvent& event)
{
    CaptureMouse();

    if(m_uiButton.Contains(event.GetPosition()))// && (*itButton).bEnabled)
    {
        m_bDown = true;
    }
    else
    {
        //jump to position
        MoveSlider(event.GetPosition().x);
    }
}

void wmSlider::MoveSlider(int x)
{

    m_dPosition = static_cast<double>(x);
    m_dPosition /= m_dResolution;
    m_dPosition = max(m_dMin, min(m_dMax, m_dPosition));
    m_uiButton.SetRect((m_dResolution*m_dPosition)-25, 0, 50, GetClientSize().y);
    Refresh();


    wxCommandEvent event(wxEVT_SLIDER_MOVE);
    event.SetId(GetId());
    wxPostEvent(GetEventHandler(), event);
}

void wmSlider::SetSliderPosition(double dPosition, bool bEvent)
{
    m_dPosition = max(m_dMin, min(m_dMax, dPosition));
    m_uiButton.SetRect((m_dResolution*m_dPosition)-25, 0, 50, GetClientSize().y);
    Refresh();


    if(bEvent)
    {
        wxCommandEvent event(wxEVT_SLIDER_MOVE);
        event.SetId(GetId());
        wxPostEvent(GetEventHandler(), event);
    }
}


void wmSlider::Init(double dMin, double dMax, double dStart)
{
    m_dMin = dMin;
    m_dMax = dMax;
    m_dPosition = dStart;

    m_dResolution = static_cast<double>(GetClientSize().x)/ (m_dMax-m_dMin);

    m_uiButton.SetRect((m_dResolution*m_dPosition)-25, 0, 50, GetClientSize().y);


    Refresh();

}


void wmSlider::OnLeftUp(wxMouseEvent& event)
{

    m_bDown = false;
    if(HasCapture())
    {
        ReleaseMouse();
    }

    Refresh();
}

void wmSlider::OnCaptureLost(wxMouseCaptureLostEvent& event)
{
    m_bDown = false;
    Refresh();
}

bool wmSlider::SetSliderColour(const wxColour &colour)
{
    m_uiSlider.SetBackgroundColour(colour);
    Refresh();
}

bool wmSlider::SetButtonColour(const wxColour &colour)
{
    m_uiButton.SetBackgroundColour(colour);
    Refresh();
}
