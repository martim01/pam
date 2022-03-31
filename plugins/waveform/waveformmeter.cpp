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

    if(!m_vChannels.empty())
    {

        int nGap =GetClientSize().y/(m_vChannels.size())/2;

        dc.SetPen(*wxWHITE_PEN);

        unsigned int nYStart = nGap-m_dResolution;
        unsigned int nYStop = nGap+m_dResolution;
        unsigned char nId = m_vChannels[0].nId;
        for(size_t i = 0; i < m_vChannels.size(); i++)
        {
            bool bSameGroup(false);
            if(i+1 < m_vChannels.size())
            {
                bSameGroup = (m_vChannels[i+1].nId == m_vChannels[i].nId);
            }

            if(!bSameGroup)
            {
                dc.SetPen(*wxWHITE_PEN);
                dc.DrawLine(0,nYStart, 0, nYStop);
                nYStop += nGap*2;
                nYStart = nYStop+1 - (m_dResolution*2.0);
            }
            else
            {
                nYStop += m_dResolution*2.0+2;
            }
        }
    }
}


void waveformMeter::OnSize(wxSizeEvent& event)
{
    WorkoutResolution();
}

void waveformMeter::WorkoutResolution()
{
    if(m_vChannels.size() != 0)
    {
        m_dResolution = static_cast<double>(GetClientSize().y-40);
        m_dResolution /= m_vChannels.size()*2.0;
    }
    Refresh();
}

void waveformMeter::SetAudioData(const timedbuffer* pBuffer)
{
    std::vector<std::pair<float, float>> vRange(m_vChannels.size(), {-1.0,1.0});
	for(size_t i = 0; i < pBuffer->GetBufferSize(); i++)
    {
        vRange[i%m_vChannels.size()].first = max(vRange[i%m_vChannels.size()].first, pBuffer->GetBuffer()[i]);
        vRange[i%m_vChannels.size()].second = min(vRange[i%m_vChannels.size()].second, pBuffer->GetBuffer()[i]);
    }

    wxMemoryDC dc;
    wxBitmap bmp(1,GetClientSize().y);
    dc.SelectObject(bmp);

    dc.SetBackground(*wxBLACK_BRUSH);
    dc.Clear();

    int nGap =bmp.GetHeight()/(m_vChannels.size())/2;
    int nCenter = nGap;

    for(size_t i = 0; i < vRange.size(); i++)
    {
        if(m_vChannels[i].type == subsession::enumChannel::LEFT)
        {
            dc.SetPen(*wxRED_PEN);
        }
        else if(m_vChannels[i].type == subsession::enumChannel::RIGHT)
        {
            dc.SetPen(*wxGREEN_PEN);
        }
        else
        {
            dc.SetPen(wxPen(wxColour(80,90,166)));
        }
        dc.DrawLine(0, (vRange[i].first*m_dResolution) + nCenter, 0, (vRange[i].second*m_dResolution)+nCenter);

        bool bSameGroup(false);
        if(i+1 < vRange.size())
        {
            bSameGroup = (m_vChannels[i+1].nId == m_vChannels[i].nId);
        }

        //draw the surrounds
        if(i == 0 || m_vChannels[i-1].nId != m_vChannels[i].nId)
        {
            dc.SetPen(*wxWHITE_PEN);
        }
        else
        {
            dc.SetPen(*wxGREY_PEN);
        }
        dc.DrawLine(0,nCenter-m_dResolution, 1, nCenter-m_dResolution);

        dc.SetPen(*wxGREY_PEN);
        dc.DrawLine(0,nCenter, 1, nCenter);

        if(!bSameGroup)
        {
            dc.SetPen(*wxWHITE_PEN);
        }
        else
        {
            dc.SetPen(*wxGREY_PEN);
        }
        dc.DrawLine(0,nCenter+m_dResolution, 1, nCenter+m_dResolution);


        if(bSameGroup)
        {
            nCenter += m_dResolution*2.0+2;
        }
        else
        {
            nCenter += nGap*2;
        }
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
        m_vChannels = aSession.GetCurrentSubsession()->vChannels;
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

