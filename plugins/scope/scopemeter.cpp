#include "scopemeter.h"
#include <wx/dc.h>
#include <wx/dcbuffer.h>
#include <wx/log.h>
#include <wx/image.h>
#include "uirect.h"
#include "timedbuffer.h"
#include "scopebuilder.h"
#include "settings.h"
#include <wx/tokenzr.h>
#include <iostream>

const std::array<wxString, 8> Scope::CLR_PLOT = {"#00ff00", "#ff0000", "#0000ff", "#ff8800","#88ff00", "#00ff88", "#0088ff", "#ff0088"};

using namespace std;

BEGIN_EVENT_TABLE(Scope, pmControl)
    EVT_PAINT(Scope::OnPaint)
    EVT_SIZE(Scope::OnSize)
    EVT_LEFT_DOWN(Scope::OnLeftDown)
    EVT_LEFT_DOWN(Scope::OnLeftUp)
    EVT_MOTION(Scope::OnMotion)
END_EVENT_TABLE()

 wxIMPLEMENT_DYNAMIC_CLASS(Scope, pmControl);

Scope::Scope(wxWindow *parent,ScopeBuilder* pBuilder, wxWindowID id, const wxPoint& pos, const wxSize& size) : pmControl(), m_pBuilder(pBuilder), m_nTrigger(0)
{
    Create(parent, id, pos, size);
}

bool Scope::Create(wxWindow *parent, wxWindowID id, const wxPoint& pos, const wxSize& size)
{
    wxSize szInit(size);
    wxSize bestSize(800,480);
    if(size.x<=0)
        szInit.SetWidth(bestSize.x);
    if(size.y <= 0)
        szInit.SetHeight(bestSize.y);

    if(!wxWindow::Create(parent,id,pos,szInit,wxWANTS_CHARS, wxT("scope")))
        return false;

    SetMinSize(size);

    m_nRouting[0] = LEFT;
    m_nRouting[1] = RIGHT;
    SetBackgroundStyle(wxBG_STYLE_CUSTOM);



    m_bMemory = false;

    m_pntSlide = wxPoint(0,0);
    m_nMode = MODE_NORMAL;
    m_bAutotrigger = true;
    m_nFrameRefresh = 4096;
    m_dTrigger = 0.088;
    SetVerticalZoom(0.5);

    m_timerNudge.SetOwner(this);
    Connect(wxID_ANY, wxEVT_TIMER, (wxObjectEventFunction)&Scope::OnTimerNudge);

    SetFont(wxFont(8,wxFONTFAMILY_SWISS,wxFONTSTYLE_NORMAL,wxFONTWEIGHT_NORMAL,false,_T("Arial"),wxFONTENCODING_DEFAULT));


    m_nTrigger = 0;
    return true;
}

Scope::~Scope()
{

}


void Scope::OnPaint(wxPaintEvent& event)
{
    wxAutoBufferedPaintDC dc(this);

    wxBrush br(*wxBLACK);
    dc.SetBrush(br);
    dc.DrawRectangle(GetClientRect());

    wxDC* pDc = &dc;


    wxMemoryDC memDC;
    if(!m_bMemory)
    {
        wxBitmap bmp(m_rectGrid.GetWidth(),m_rectGrid.GetHeight());
        memDC.SelectObject(bmp);
        memDC.SetBrush(br);
        memDC.DrawRectangle(wxRect(0,0, bmp.GetWidth(), bmp.GetHeight()));
    }
    else
    {
        memDC.SelectObject(m_bmpScreen);
    }


    wxPoint pntCenter(m_rectGrid.GetWidth()/2, m_rectGrid.GetHeight()/2);
    memDC.SetPen(wxPen(wxColour(120,120,120),1));



    float dCursorAmp(0);

    for(const auto& graph : m_vPlot)
    {
        if(graph.bPlot)
        {
            memDC.SetPen(graph.clr);

            float x = 0;
            int x_old(0), y_old(pntCenter.y);
            float dy_old = -80;

            for(const auto& sample : graph.lstBuffer)
            {
                float y;
                int nY;
                if(x > (m_dSampleX-1)*m_dStep && x < (m_dSampleX + 1)*m_dStep)
                {
                    dCursorAmp = sample;
                }

                y = sample*m_dResolution;
                y += graph.nOffset;
                nY = pntCenter.y-y;


                int nX = min(static_cast<int>(x), m_rectGrid.GetWidth());

                if((x_old != nX || y_old != nY) && nX+m_pntSlide.x < m_rectGrid.GetRight())
                {
                    memDC.DrawLine(x_old+m_pntSlide.x, y_old+m_pntSlide.y, nX+m_pntSlide.x,nY+m_pntSlide.y);
                }
                x_old = nX;
                y_old = nY;
                x+=m_dStep;
            }
        }
    }
    dc.Blit(m_rectGrid.GetLeft(),m_rectGrid.GetTop(),m_rectGrid.GetWidth(), m_rectGrid.GetHeight(), &memDC,0,0);

    float dGap = static_cast<float>(m_rectGrid.GetHeight())/16.0;
    for(float i = 0; i < 8; i++)
    {
        float dY = dGap * i;

        if(i == 0)
        {
            dc.SetPen(wxPen(wxColour(190,190,190),1, wxSOLID));
            dc.DrawLine(m_rectGrid.GetLeft(),pntCenter.y-dY, m_rectGrid.GetRight(), pntCenter.y-dY);
            uiRect rectLabel(wxRect(0, pntCenter.y-dY-10, m_rectGrid.GetLeft(), 20));
            rectLabel.Draw(dc, wxT("0"), uiRect::BORDER_NONE);
        }
        else
        {
            dc.SetPen(wxPen(wxColour(120,120,120),1));
            dc.DrawLine(m_rectGrid.GetLeft(),pntCenter.y-dY, m_rectGrid.GetRight(), pntCenter.y-dY);
            dc.DrawLine(m_rectGrid.GetLeft(),pntCenter.y+dY, m_rectGrid.GetRight(), pntCenter.y+dY);
            double dValue = (i*m_dVerticalZoom/8.0);

            uiRect rectLabel(wxRect(0, pntCenter.y-dY-10, m_rectGrid.GetLeft(), 20));
            rectLabel.Draw(dc, wxString::Format(wxT("%.3f"), dValue), uiRect::BORDER_NONE);

            uiRect rectLabel2(wxRect(0, pntCenter.y+dY-10, m_rectGrid.GetLeft(), 20));
            rectLabel2.Draw(dc, wxString::Format(wxT("%.3f"), -dValue), uiRect::BORDER_NONE);
        }
    }
    dc.SetPen(wxPen(wxColour(200,50,50),1));
    dc.DrawLine(m_rectGrid.GetLeft(), pntCenter.y-(m_dResolution*m_dTrigger), m_rectGrid.GetRight(),pntCenter.y-(m_dResolution*m_dTrigger));

    dc.SetPen(wxPen(wxColour(120,120,120),1));
    for(int i = 0; i < 10; i++)
    {
        dc.DrawLine(m_rectGrid.GetLeft()+(m_rectGrid.GetWidth()/10)*i,m_rectGrid.GetTop(), m_rectGrid.GetLeft()+(m_rectGrid.GetWidth()/10)*i, m_rectGrid.GetBottom());

    }




    double dMs = (1/48.0)*static_cast<double>(m_nFrameRefresh/2);
    uiRect uiLabel(wxRect(m_rectGrid.GetLeft(),GetClientSize().y-22, 80,20));
    uiLabel.SetBackgroundColour(wxColour(80,80,120));
    uiLabel.Draw(dc, wxString::Format(wxT("%.02f ms"), dMs), uiRect::BORDER_NONE);

    uiLabel.SetRect(wxRect(uiLabel.GetRight()+10, uiLabel.GetTop(), 50,20));
    if(m_bAutotrigger)
    {
        uiLabel.SetBackgroundColour(wxColour(0,100,0));
        uiLabel.Draw(dc, wxT("Auto"), uiRect::BORDER_NONE);
    }
    uiLabel.SetRect(wxRect(uiLabel.GetRight()+10, uiLabel.GetTop(), 50,20));
    if(m_bMemory)
    {
        uiLabel.SetBackgroundColour(wxColour(100,0,0));
        uiLabel.Draw(dc, wxT("Mem"), uiRect::BORDER_NONE);
    }
    uiLabel.SetBackgroundColour(wxColour(80,80,120));

    uiLabel.SetRect(wxRect(uiLabel.GetRight()+10, uiLabel.GetTop(), 200,20));
    if(m_vMaxY.size() > m_nTrigger)
    {
        uiLabel.Draw(dc, wxString::Format(wxT("%.05f [%.05f]"), m_vMaxY[m_nTrigger], m_dTrigger), uiRect::BORDER_NONE);
    }
    uiLabel.SetRect(wxRect(uiLabel.GetRight()+10, uiLabel.GetTop(), 80,20));
    if(m_pntSlide.x != 0 || m_pntSlide.y != 0)
    {
        uiLabel.SetBackgroundColour(wxColour(120,0,0));
        uiLabel.Draw(dc, wxString::Format(wxT("(%d,%d)"), m_pntSlide.x, m_pntSlide.y), uiRect::BORDER_NONE);
    }

    dMs /= 10;
    int nDiv= m_rectGrid.GetWidth()/10;
    for(int i = 0; i < 10; i++)
    {
        uiRect uiLabel(wxRect(m_rectGrid.GetLeft() + nDiv*i-(nDiv/2)+4, m_rectGrid.GetBottom()+5, nDiv-8, 25));

        uiLabel.Draw(dc, wxString::Format(wxT("%.2fms"), dMs*static_cast<double>(i)), uiRect::BORDER_NONE);
    }



    if(m_nMode != MODE_NORMAL)
    {
        m_uiExit.Draw(dc, wxT("Exit"), uiRect::BORDER_UP);
        if(m_nMode == MODE_CURSOR)
        {
            dc.SetPen(wxPen(wxColour(255,100,00), 1));
            dc.DrawLine(m_dSampleX*m_dStep  + m_rectGrid.GetLeft(), m_rectGrid.GetTop(), m_dSampleX*m_dStep + m_rectGrid.GetLeft(), m_rectGrid.GetBottom());

            m_uiValue.Draw(dc, wxString::Format(wxT("%.3f"), dCursorAmp), uiRect::BORDER_NONE);
            m_uiTime.Draw(dc, wxString::Format(wxT("%.2f ms"), static_cast<double>(m_dSampleX)/48.0), uiRect::BORDER_NONE);
            m_uiNudgeDown.Draw(dc,wxT("-"), (m_nNudge==DOWN)? uiRect::BORDER_DOWN : uiRect::BORDER_UP);;
            m_uiNudgeUp.Draw(dc,wxT("+"), (m_nNudge==UP)? uiRect::BORDER_DOWN : uiRect::BORDER_UP);
        }
    }

}

void Scope::OnSize(wxSizeEvent& event)
{
    m_rectGrid = wxRect(50, 0, GetClientRect().GetWidth()-50, GetClientRect().GetHeight()-50);

    m_dResolution = static_cast<float>(m_rectGrid.GetHeight())/(2*m_dVerticalZoom);

    m_uiExit.SetRect(wxRect(GetClientRect().GetRight()-100, 5, 100,50));
    m_uiExit.SetBackgroundColour(wxColour(120,0,0));

    m_uiValue.SetRect(m_uiExit.GetLeft()-120, m_uiExit.GetTop(), 100, m_uiExit.GetHeight());

    m_uiTime.SetRect(m_uiValue.GetLeft()-180, m_uiValue.GetTop(), 100, m_uiValue.GetHeight());
    m_uiNudgeUp.SetRect(m_uiTime.GetRight()+5, m_uiValue.GetTop(), 50, m_uiValue.GetHeight());
    m_uiNudgeDown.SetRect(m_uiTime.GetLeft()-55, m_uiValue.GetTop(), 50, m_uiValue.GetHeight());

    m_uiNudgeDown.SetBackgroundColour(wxColour(100,100,160));
    m_uiNudgeUp.SetBackgroundColour(wxColour(100,100,160));

    m_dStep = static_cast<float>(m_rectGrid.GetWidth())/static_cast<float>(m_nFrameRefresh/2);

    ResetMemory();
    Refresh();
}


void Scope::SetTimeFrame(int nFrames)
{
    m_nFrameRefresh = nFrames;
    m_lstBuffer.clear();
    m_dStep = static_cast<float>(m_rectGrid.GetWidth())/static_cast<float>(m_nFrameRefresh/2);
    Refresh();
}

void Scope::SetData(const timedbuffer* pBuffer)
{

    for(int i = 0; i < pBuffer->GetBufferSize(); i++)
    {
        m_lstBuffer.push_back(pBuffer->GetBuffer()[i]);
    }

    if(m_lstBuffer.size() >= m_nFrameRefresh*m_vPlot.size() && m_vPlot.size() != 0)
    {
        bool bShow = WorkoutPlot();

        if(m_bAutotrigger)
        {
            Autotrigger();
            bShow = WorkoutPlot();
        }
        if(bShow)
        {
            Refresh();
        }
        m_lstBuffer.clear();
    }
}


bool Scope::WorkoutPlot()
{
    bool bTriggered(false);
    for(auto& aplot : m_vPlot)
    {
        aplot.lstBuffer.clear();
    }

    std::vector<float> vChannels(m_vPlot.size());
    m_vMaxY = std::vector<float>(m_vPlot.size(), 0.0);
    std::vector<float> vLast(m_vPlot.size(), 0.0);

    auto itSample = m_lstBuffer.begin();

    for(; itSample != m_lstBuffer.end();)
    {
        for(int nChannel = 0; nChannel < vChannels.size(); nChannel++)
        {
            if(itSample != m_lstBuffer.end())
            {
                vChannels[nChannel] = *(itSample);
                m_vMaxY[nChannel] = max(vChannels[nChannel], m_vMaxY[nChannel]);
                ++itSample;
            }
        }

        if(!bTriggered)
        {
            bTriggered = ((m_dTrigger >= 0 && vChannels[m_nTrigger] >= m_dTrigger && vLast[m_nTrigger] < vChannels[m_nTrigger])  ||
                          (m_dTrigger < 0 && vChannels[m_nTrigger] <= m_dTrigger && vLast[m_nTrigger] > vChannels[m_nTrigger]));
        }
        vLast = vChannels;


        if(bTriggered)
        {
            for(size_t i = 0; i < vChannels.size(); i++)
            {
                m_vPlot[i].lstBuffer.push_back(vChannels[i]);
            }
        }
    }



    return bTriggered;
}
void Scope::SetRouting(short nRouting, int nPlot)
{
    m_nRouting[nPlot%2] = nRouting;
}


double Scope::GetTrigger()
{
    return m_dTrigger;
}

void Scope::SetTrigger(double dTrigger)
{
    m_dTrigger = dTrigger;
}

double Scope::GetVerticalZoom()
{
    return m_dVerticalZoom;
}

void Scope::SetVerticalZoom(double dZoom)
{
    m_dVerticalZoom = dZoom;
    m_dResolution = static_cast<float>(m_rectGrid.GetHeight())/(2*m_dVerticalZoom);
}


void Scope::OnLeftDown(wxMouseEvent& event)
{
    if(m_uiExit.Contains(event.GetPosition()))
    {

    }
    else
    {
        m_pntMouse = event.GetPosition();
        if(m_nMode == MODE_CURSOR)
        {
            if(m_uiNudgeDown.Contains(event.GetPosition()))
            {
                m_dSampleX = max(m_dSampleX-1, 0.0);
                m_timerNudge.Start(100);
                m_nNudge = DOWN;
            }
            else if(m_uiNudgeUp.Contains(event.GetPosition()))
            {
                m_dSampleX = min(m_dSampleX+1, static_cast<double>(m_nFrameRefresh)/2.0);
                m_timerNudge.Start(100);
                m_nNudge = UP;
            }
            else
            {
                m_dSampleX = static_cast<double>(m_pntMouse.x - m_rectGrid.GetLeft())/m_dStep;
;            }
        }
    }
    event.Skip();
}

void Scope::OnTimerNudge(wxTimerEvent& event)
{
    if(m_nNudge == UP)
    {
        m_dSampleX = min(m_dSampleX+1, static_cast<double>(m_nFrameRefresh)/2.0);
    }
    else if(m_nNudge == DOWN)
    {
        m_dSampleX = max(m_dSampleX-1, 0.0);
    }
}

void Scope::OnMotion(wxMouseEvent& event)
{
    if(event.LeftIsDown() && m_nMode == MODE_SLIDE)
    {
        m_pntSlide.x = (event.GetPosition().x-m_pntMouse.x);
        m_pntSlide.y = (event.GetPosition().y-m_pntMouse.y);
        Refresh();
    }
    event.Skip();
}

void Scope::Autotrigger()
{
    m_dTrigger = m_vMaxY[m_nTrigger];
    if(m_dTrigger > 0)
    {
        m_dTrigger -= 0.001;
    }
    else
    {
        m_dTrigger += 0.001;
    }
}

void Scope::ResetMemory()
{
    m_bmpScreen = wxBitmap(m_rectGrid.GetWidth(),m_rectGrid.GetHeight());
    wxMemoryDC memDC;
    memDC.SelectObject(m_bmpScreen);
    memDC.SetBrush(*wxBLACK_BRUSH);
    memDC.DrawRectangle(wxRect(0,0, m_bmpScreen.GetWidth(), m_bmpScreen.GetHeight()));
    Refresh();
}


void Scope::SetMode(unsigned int nMode)
{
    m_nMode = nMode;
    if(m_nMode != MODE_NORMAL)
    {
        m_pBuilder->Maximize(true);
    }
    Refresh();
}

void Scope::TurnoffNudge()
{
    m_timerNudge.Stop();
    m_nNudge = NONE;
    RefreshRect(m_uiNudgeDown.GetRect());
    RefreshRect(m_uiNudgeUp.GetRect());
}


void Scope::OnLeftUp(wxMouseEvent& event)
{
    //m_plstScope_Mode->SelectButton(m_pScope->GetMode(), false);
    if(GetMode() == Scope::MODE_NORMAL)
    {
        if(GetSize().x > 600)
        {
            m_pBuilder->Maximize(false);
        }
        else
        {
            m_pBuilder->Maximize(true);
        }
    }
    else
    {
        if(m_uiExit.Contains(event.GetPosition()))
        {
            m_nMode = MODE_NORMAL;
            m_pBuilder->WriteSetting(wxT("Mode"), 0);
        }

        TurnoffNudge();
    }
}


void Scope::SetNumberOfChannels(unsigned int nChannels)
{
    m_vPlot.clear();
    m_vPlot.resize(nChannels);
    SetPlot(m_pBuilder->ReadSetting("Plot", "0,1"));

    for(size_t i = 0; i < m_vPlot.size(); i++)
    {
        m_vPlot[i].clr = wxColour(m_pBuilder->ReadSetting(wxString::Format("Plot_Colour_%lu", i), CLR_PLOT[i]));
        m_vPlot[i].nOffset = m_pBuilder->ReadSetting(wxString::Format("Plot_Offset_%lu", i), 0);
    }
}


void Scope::SetPlot(const wxString& sPlot)
{

    wxArrayString asPlot = wxStringTokenize(sPlot, ",");
    for(size_t i = 0; i < m_vPlot.size(); i++)
    {
        m_vPlot[i].bPlot = (asPlot.Index(wxString::Format("%lu",i)) != wxNOT_FOUND);

    }
}

void Scope::SetPlotColour(unsigned long nChannel, const wxString& sColour)
{
    wxColour clr(sColour);
    if(nChannel < m_vPlot.size() && clr.IsOk())
    {
        m_vPlot[nChannel].clr = clr;
        Refresh();
    }
}

void Scope::SetPlotOffset(unsigned long nChannel, int nOffset)
{
    if(nChannel < m_vPlot.size())
    {
        m_vPlot[nChannel].nOffset = nOffset;
        Refresh();
    }
}
