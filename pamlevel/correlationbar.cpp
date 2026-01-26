#include "correlationbar.h"
#include <wx/dc.h>
#include <wx/dcbuffer.h>
#include <wx/log.h>
#include <wx/image.h>
#include "levelcalculator.h"
#include "timedbuffer.h"
#include "settings.h"
#include "audioalgorithms.h"


BEGIN_EVENT_TABLE(CorrelationBar, pmControl)
    EVT_PAINT(CorrelationBar::OnPaint)
    EVT_SIZE(CorrelationBar::OnSize)
    EVT_LEFT_UP(CorrelationBar::OnLeftUp)
END_EVENT_TABLE()

 wxIMPLEMENT_DYNAMIC_CLASS(CorrelationBar, pmControl);

CorrelationBar::CorrelationBar(wxWindow *parent, wxWindowID id, const wxPoint& pos, const wxSize& size) : pmControl(),
    m_pntPole(0,0),
    m_dResolution(1.0),
    m_dResolutionCorrelation(1.0),
    m_nMode(0),
    m_dMindB(70.0),
    m_nInputChannels(1),
    m_nAxisX(0),
    m_nAxisY(0),
    m_dCorrelation(0),
    m_pBmpCorrelationOut(nullptr),
    m_pBmpCorrelationIn(nullptr),
    m_dtFlash(wxDateTime::UNow())
{
    Create(parent, id, pos, size);

    SetForegroundColour(*wxWHITE);
    SetFont(wxFont(8,wxFONTFAMILY_SWISS,wxFONTSTYLE_NORMAL,wxFONTWEIGHT_NORMAL,false,_T("Arial"),wxFONTENCODING_DEFAULT));

}

bool CorrelationBar::Create(wxWindow *parent, wxWindowID id, const wxPoint& pos, const wxSize& size)
{
    wxSize szInit(size);
    wxSize bestSize(600,481);
    if(size.x<=0)
        szInit.SetWidth(bestSize.x);
    if(size.y <= 0)
        szInit.SetHeight(bestSize.y);

    if(!wxWindow::Create(parent,id,pos,szInit,wxWANTS_CHARS, wxT("Radar")))
        return false;

    SetMinSize(size);

    SetBackgroundStyle(wxBG_STYLE_CUSTOM);

    m_nInputChannels = 2;
    m_nAxisX = 0;
    m_nAxisY = 1;

    CreateRects();



    return true;
}

CorrelationBar::~CorrelationBar()
{
}


void CorrelationBar::OnPaint(wxPaintEvent& event)
{
    wxAutoBufferedPaintDC dc(this);


    dc.SetPen(*wxTRANSPARENT_PEN);
    dc.SetBrush(*wxBLACK_BRUSH);
    dc.DrawRectangle(GetClientRect());

    if(m_bBar)
    {
        DrawBar(dc);
    }
    else
    {
        DrawBox(dc);
    }
}

void CorrelationBar::DrawBar(wxDC& dc)
{
    dc.SetPen(*wxWHITE_PEN);
    dc.DrawRectangle(m_rectCorrelation);
    dc.DrawRectangle(m_rectBalance);

    dc.DrawBitmap(*m_pBmpCorrelationOut, m_rectCorrelation.GetLeft()+1, m_rectCorrelation.GetTop()+1);
    dc.DrawBitmap(*m_pBmpCorrelationIn, wxPoint(m_rectCorrelation.GetLeft()+m_rectCorrelation.GetWidth()/2, m_rectCorrelation.GetTop()+1));


    auto dCorrelation = WorkoutCorrelation();


    dc.SetBrush(*wxBLACK_BRUSH);
    dc.SetPen(*wxTRANSPARENT_PEN);
    dc.DrawRectangle(m_rectCorrelation.GetLeft()+m_rectCorrelation.GetWidth()/2+dCorrelation*m_dResolutionCorrelation, m_rectCorrelation.GetTop()+1, 3, m_rectCorrelation.GetHeight()-2);



    dc.SetBrush(*wxTRANSPARENT_BRUSH);

    dc.SetPen(wxPen(wxColour(0,0,0), 1));
    dc.DrawLine(m_rectCorrelation.GetLeft()+1+m_rectCorrelation.GetWidth()/4, m_rectCorrelation.GetTop()+1, m_rectCorrelation.GetLeft()+1+m_rectCorrelation.GetWidth()/4, m_uiCorrelation.GetBottom()-1);
    dc.DrawLine(m_rectCorrelation.GetLeft()+1+m_rectCorrelation.GetWidth()/2, m_rectCorrelation.GetTop()+1, m_rectCorrelation.GetLeft()+1+m_rectCorrelation.GetWidth()/2, m_uiCorrelation.GetBottom()-1);
    dc.DrawLine(3*(m_rectCorrelation.GetLeft()+1+m_rectCorrelation.GetWidth())/4, m_rectCorrelation.GetTop()+1, 3*(m_rectCorrelation.GetLeft()+1+m_rectCorrelation.GetWidth())/4, m_uiCorrelation.GetBottom()-1);


    uiRect rectLabel(wxRect(m_rectCorrelation.GetLeft()-5, m_rectCorrelation.GetBottom()+5, 20,20));
    rectLabel.SetBackgroundColour(*wxBLACK);
    rectLabel.Draw(dc, wxT("-1"), uiRect::BORDER_NONE);

    rectLabel.SetRect(m_rectCorrelation.GetLeft()+1+m_rectCorrelation.GetWidth()/4-10, m_rectCorrelation.GetBottom()+5, 20,20);
    rectLabel.Draw(dc, wxT("-.5"), uiRect::BORDER_NONE);

    rectLabel.SetRect(m_rectCorrelation.GetLeft()+1+m_rectCorrelation.GetWidth()/2-10, m_rectCorrelation.GetBottom()+5, 20,20);
    rectLabel.Draw(dc, wxT("0"), uiRect::BORDER_NONE);

    rectLabel.SetRect(3*(m_rectCorrelation.GetLeft()+1+m_rectCorrelation.GetWidth())/4-10, m_rectCorrelation.GetBottom()+5, 20,20);
    rectLabel.Draw(dc, wxT(".5"), uiRect::BORDER_NONE);

    rectLabel.SetRect(m_rectCorrelation.GetRight()-10, m_rectCorrelation.GetBottom()+5, 20,20);
    rectLabel.Draw(dc, wxT("1"), uiRect::BORDER_NONE);

    m_uiCorrelation.Draw(dc, wxString::Format(wxT("%.2f"), dCorrelation), uiRect::BORDER_NONE);

}

void CorrelationBar::DrawBox(wxDC& dc)
{
    
    uiRect rect(GetClientRect());

    auto dCorrelation = WorkoutCorrelation();
    if(dCorrelation > m_dThreshold)
    {
        rect.SetBackgroundColour(wxColour(0,128,0));
        rect.SetForegroundColour(*wxWHITE);
        auto fnt = dc.GetFont();
        dc.SetFont(wxFont(12,wxFONTFAMILY_SWISS,wxFONTSTYLE_NORMAL,wxFONTWEIGHT_BOLD,false,_T("Arial"),wxFONTENCODING_DEFAULT));
        rect.Draw(dc, "PHASE OK", uiRect::BORDER_FLAT);
        dc.SetFont(fnt);
    }
    else
    {
        rect.SetBackgroundColour(m_bFlash ? wxColour(255,0,0) : wxColour(128,0,0));
        rect.SetForegroundColour(*wxWHITE);
        auto fnt = dc.GetFont();
        dc.SetFont(wxFont(12,wxFONTFAMILY_SWISS,wxFONTSTYLE_NORMAL,wxFONTWEIGHT_BOLD,false,_T("Arial"),wxFONTENCODING_DEFAULT));
        rect.Draw(dc, "OUT OF PHASE", uiRect::BORDER_FLAT);
        dc.SetFont(fnt);

        auto dtNow = wxDateTime::UNow();
        if(dtNow > m_dtFlash+wxTimeSpan(0,0,0,500))
        {
            m_bFlash = !m_bFlash;
            m_dtFlash = dtNow;
        }
    }
}

void CorrelationBar::WorkoutBalance(const timedbuffer* pBuffer)
{

    m_lstCorrelation.push_back(PearsonCorrelation(pBuffer, m_nInputChannels, m_nAxisX, m_nAxisY));
    if(m_lstCorrelation.size() > 40)
    {
        m_lstCorrelation.pop_front();
    }

}



void CorrelationBar::OnSize(wxSizeEvent& event)
{
    CreateRects();
    Refresh();
}

void CorrelationBar::CreateRects()
{
    m_rectCorrelation = wxRect(5,0, GetClientRect().GetWidth()-10, 20);


    m_uiCorrelation.SetRect(wxRect(m_rectCorrelation.GetLeft()+m_rectCorrelation.GetWidth()/2 -25 , m_rectCorrelation.GetBottom()+25, 50, 20));
    m_uiCorrelation.SetBackgroundColour(*wxBLACK);


    m_dResolution = static_cast<double>((GetClientRect().GetWidth()-4))/(m_dMindB*2.0);
    m_dResolutionCorrelation = static_cast<double>((m_rectCorrelation.GetWidth()-2))/2.0;


    m_pBmpCorrelationOut = std::make_unique<wxBitmap>(m_rectCorrelation.GetWidth()/2-1, m_rectCorrelation.GetHeight()-2);

    m_pBmpCorrelationIn = std::make_unique<wxBitmap>(m_rectCorrelation.GetWidth()/2-1, m_rectCorrelation.GetHeight()-2);

    wxMemoryDC dc;
    dc.SelectObject(*m_pBmpCorrelationOut);
    dc.GradientFillLinear(wxRect(0,0, m_pBmpCorrelationOut->GetWidth(), m_pBmpCorrelationOut->GetHeight()), wxColour(255,0,0), wxColour(255,255,0));

    dc.SelectObject(*m_pBmpCorrelationIn);
    dc.GradientFillLinear(wxRect(0,0, m_pBmpCorrelationIn->GetWidth(), m_pBmpCorrelationIn->GetHeight()), wxColour(255,255,0), wxColour(0,255,0));


    Refresh();
}

double CorrelationBar::WorkoutCorrelation()
{
    double dCorrelation = 0.0;
    for(const auto d : m_lstCorrelation)
    {
        dCorrelation += d;
    }

    dCorrelation /= static_cast<double>(m_lstCorrelation.size());
    return dCorrelation;
}


void CorrelationBar::OnLeftUp(wxMouseEvent& event)
{
    wxPostEvent(GetParent(), event);
}


void CorrelationBar::ClearMeter()
{
    m_lstCorrelation.clear();
    m_dCorrelation = 0.0;
    if(IsShown())
    {
        Refresh();
    }
}


void CorrelationBar::SetAudioData(const timedbuffer* pBuffer)
{
    WorkoutBalance(pBuffer);

    if(IsShown())
    {
        Refresh();
    }
}

void CorrelationBar::SetNumberOfInputChannels(unsigned int nInputChannels)
{
    m_nInputChannels = nInputChannels;
}

void CorrelationBar::SetAxisX(unsigned int nChannel)
{
    m_nAxisX = nChannel;
}

void CorrelationBar::SetAxisY(unsigned int nChannel)
{
    m_nAxisY = nChannel;
}



hsv CorrelationBar::rgb2hsv(rgb in)
{
    hsv         out;
    double      min, max, delta;

    min = in.r < in.g ? in.r : in.g;
    min = min  < in.b ? min  : in.b;

    max = in.r > in.g ? in.r : in.g;
    max = max  > in.b ? max  : in.b;

    out.v = max;                                // v
    delta = max - min;
    if (delta < 0.00001)
    {
        out.s = 0;
        out.h = 0; // undefined, maybe nan?
        return out;
    }
    if( max > 0.0 ) { // NOTE: if Max is == 0, this divide would cause a crash
        out.s = (delta / max);                  // s
    } else {
        // if max is 0, then r = g = b = 0
        // s = 0, h is undefined
        out.s = 0.0;
        out.h = NAN;                            // its now undefined
        return out;
    }
    if( in.r >= max )                           // > is bogus, just keeps compilor happy
        out.h = ( in.g - in.b ) / delta;        // between yellow & magenta
    else
    if( in.g >= max )
        out.h = 2.0 + ( in.b - in.r ) / delta;  // between cyan & yellow
    else
        out.h = 4.0 + ( in.r - in.g ) / delta;  // between magenta & cyan

    out.h *= 60.0;                              // degrees

    if( out.h < 0.0 )
        out.h += 360.0;

    return out;
}


rgb CorrelationBar::hsv2rgb(hsv in)
{
    double      hh, p, q, t, ff;
    long        i;
    rgb         out;

    if(in.s <= 0.0) {       // < is bogus, just shuts up warnings
        out.r = in.v;
        out.g = in.v;
        out.b = in.v;
        return out;
    }
    hh = in.h;
    if(hh >= 360.0) hh = 0.0;
    hh /= 60.0;
    i = (long)hh;
    ff = hh - i;
    p = in.v * (1.0 - in.s);
    q = in.v * (1.0 - (in.s * ff));
    t = in.v * (1.0 - (in.s * (1.0 - ff)));

    switch(i) {
    case 0:
        out.r = in.v;
        out.g = t;
        out.b = p;
        break;
    case 1:
        out.r = q;
        out.g = in.v;
        out.b = p;
        break;
    case 2:
        out.r = p;
        out.g = in.v;
        out.b = t;
        break;

    case 3:
        out.r = p;
        out.g = q;
        out.b = in.v;
        break;
    case 4:
        out.r = t;
        out.g = p;
        out.b = in.v;
        break;
    case 5:
    default:
        out.r = in.v;
        out.g = p;
        out.b = q;
        break;
    }
    return out;
}

void CorrelationBar::SetMode(int nMode)
{
    m_nMode = nMode;
    Refresh();
}
