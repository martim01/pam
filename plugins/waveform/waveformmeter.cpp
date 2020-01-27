#include "waveformmeter.h"
#include <wx/dc.h>
#include <wx/dcbuffer.h>
#include <wx/log.h>
#include "uirect.h"
#include "waveformbuilder.h"
#include "settings.h"
#include "timedbuffer.h"


using namespace std;

BEGIN_EVENT_TABLE(waveformMeter, pmControl)
    EVT_PAINT(waveformMeter::OnPaint)
    EVT_SIZE(waveformMeter::OnSize)
    EVT_LEFT_UP(waveformMeter::OnLeftUp)
END_EVENT_TABLE()


 wxIMPLEMENT_DYNAMIC_CLASS(waveformMeter, pmControl);



waveformMeter::waveformMeter(wxWindow *parent, waveformBuilder* pBuilder, wxWindowID id, const wxPoint& pos, const wxSize& size) : pmControl(),
    m_pBuilder(pBuilder)
{
    Create(parent, id, pos, size);

}

bool waveformMeter::Create(wxWindow *parent, wxWindowID id, const wxPoint& pos, const wxSize& size)
{
    wxSize szInit(size);
    wxSize bestSize = DoGetBestSize();
    if(size.x<=0)
        szInit.SetWidth(bestSize.x);
    if(size.y <= 0)
        szInit.SetHeight(bestSize.y);

    if(!wxWindow::Create(parent,id,pos,szInit,wxWANTS_CHARS, wxT("fft")))
        return false;

    SetMinSize(size);

    SetBackgroundStyle(wxBG_STYLE_CUSTOM);


    return true;
}

waveformMeter::~waveformMeter()
{

}


void waveformMeter::OnPaint(wxPaintEvent& event)
{
    wxAutoBufferedPaintDC dc(this);
    wxBrush br(*wxBLACK);
    dc.SetBrush(br);
    dc.DrawRectangle(GetClientRect());

    int x = 0;
    int nLeft = GetClientRect().GetTop() + (GetClientSize().y/4);
    int nRight = GetClientRect().GetBottom() - (GetClientSize().y/4);

    dc.SetPen(*wxRED_PEN);
    for(list<pair<double,double> >::iterator itWave = m_lstWaveformLeft.begin(); itWave != m_lstWaveformLeft.end(); ++itWave)
    {
        dc.DrawLine(x, ((*itWave).first*m_dResolution) + nLeft, x, ((*itWave).second*m_dResolution)+nLeft);
        ++x;
    }

    x = 0;
    dc.SetPen(*wxGREEN_PEN);
    for(list<pair<double,double> >::iterator itWave = m_lstWaveformRight.begin(); itWave != m_lstWaveformRight.end(); ++itWave)
    {
        dc.DrawLine(x, ((*itWave).first*m_dResolution) + nRight, x, ((*itWave).second*m_dResolution)+nRight);
        ++x;
    }

    dc.SetPen(wxPen(*wxWHITE, 1));
    dc.DrawLine(0,nLeft-m_dResolution, GetClientSize().x, nLeft-m_dResolution);
    dc.DrawLine(0,nLeft, GetClientSize().x, nLeft);
    dc.DrawLine(0,nLeft+m_dResolution, GetClientSize().x, nLeft+m_dResolution);
    dc.DrawLine(0,nRight-m_dResolution, GetClientSize().x, nRight-m_dResolution);
    dc.DrawLine(0,nRight, GetClientSize().x, nRight);
    dc.DrawLine(0,nRight+m_dResolution, GetClientSize().x, nRight+m_dResolution);


}


void waveformMeter::OnSize(wxSizeEvent& event)
{
    m_dResolution = static_cast<double>(GetClientSize().y-20);
    m_dResolution /= 4;
    Refresh();

}

void waveformMeter::SetAudioData(const timedbuffer* pBuffer)
{
    float dPoint[2][2] = {{-1.0,1.0}, {-1.0,1.0}};


	for(size_t i = 0; i < pBuffer->GetBufferSize(); i++)
    {

        dPoint[i%2][0]  = max(dPoint[i%2][0], pBuffer->GetBuffer()[i]);
        dPoint[i%2][1]  = min(dPoint[i%2][1], pBuffer->GetBuffer()[i]);
    }
    m_lstWaveformLeft.push_back(make_pair(dPoint[0][0],dPoint[0][1]));
    m_lstWaveformRight.push_back(make_pair(dPoint[1][0],dPoint[1][1]));

    if(m_lstWaveformLeft.size() > GetClientSize().x)
    {
        m_lstWaveformLeft.pop_front();
        m_lstWaveformRight.pop_front();
    }
    Refresh();
}


void waveformMeter::InputSession(const session& aSession)
{

}

void waveformMeter::OutputChannels(const std::vector<char>& vChannels)
{

}



void waveformMeter::OnLeftUp(wxMouseEvent& event)
{
	m_pBuilder->Maximize((GetSize().x<=600));

}

