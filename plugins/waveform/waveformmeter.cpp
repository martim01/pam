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
    dc.SetBackground(*wxBLACK_BRUSH);
    dc.Clear();


    int x = 0;
    for(const auto& bmp : m_lstWaveform)
    {
        dc.DrawBitmap(bmp, x,0);
        ++x;
    }
}


void waveformMeter::OnSize(wxSizeEvent& event)
{
    WorkoutResolution();
}

void waveformMeter::WorkoutResolution()
{
    if(m_nChannels != 0)
    {
        m_dResolution = static_cast<double>(GetClientSize().y-20);
        m_dResolution /= m_nChannels*2.0;
    }
    Refresh();
}

void waveformMeter::SetAudioData(const timedbuffer* pBuffer)
{
    std::vector<std::pair<float, float>> vRange(m_nChannels, {-1.0,1.0});
	for(size_t i = 0; i < pBuffer->GetBufferSize(); i++)
    {
        vRange[i%m_nChannels].first = max(vRange[i%m_nChannels].first, pBuffer->GetBuffer()[i]);
        vRange[i%m_nChannels].second = min(vRange[i%m_nChannels].second, pBuffer->GetBuffer()[i]);
    }

    wxMemoryDC dc;
    wxBitmap bmp(1,GetClientSize().y);
    dc.SelectObject(bmp);

    dc.SetBackground(*wxBLACK_BRUSH);
    dc.Clear();


    int nGap =bmp.GetHeight()/(m_nChannels)/2;
    int nCenter = nGap;

    for(const auto& pairRange : vRange)
    {
        dc.SetPen(*wxWHITE_PEN);
        dc.DrawLine(0, (pairRange.first*m_dResolution) + nCenter, 0, (pairRange.second*m_dResolution)+nCenter);

        dc.SetPen(*wxGREY_PEN);
        dc.DrawLine(0,nCenter-m_dResolution, 1, nCenter-m_dResolution);
        dc.DrawLine(0,nCenter, 1, nCenter);
        dc.DrawLine(0,nCenter+m_dResolution, 1, nCenter+m_dResolution);

        nCenter += nGap*2;
    }
    dc.SelectObject(wxNullBitmap);

    m_lstWaveform.push_back(std::move(bmp));
    if(m_lstWaveform.size() > GetClientSize().x)
    {
        m_lstWaveform.pop_front();
    }

    Refresh();
}


void waveformMeter::InputSession(const session& aSession)
{
    m_lstWaveform.clear();
    if(aSession.GetCurrentSubsession() != aSession.lstSubsession.end())
    {
        m_nChannels = (std::min((unsigned int)8 ,aSession.GetCurrentSubsession()->nChannels));
    }
    WorkoutResolution();
}

void waveformMeter::OutputChannels(const std::vector<char>& vChannels)
{

}



void waveformMeter::OnLeftUp(wxMouseEvent& event)
{
	m_pBuilder->Maximize((GetSize().x<=600));

}

