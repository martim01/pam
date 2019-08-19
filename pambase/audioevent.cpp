#include "audioevent.h"
#include "timedbuffer.h"

wxDEFINE_EVENT(wxEVT_DATA, AudioEvent);

AudioEvent::AudioEvent() : wxCommandEvent(wxEVT_DATA, wxID_ANY),
m_pBuffer(0),
m_nCreator(0),
m_nFrameCount(0),
m_nSampleRate(0),
m_nStatus(OK)
{

}

AudioEvent::AudioEvent(timedbuffer* pBuffer, int nCreator, unsigned long nFrameCount, unsigned long nSampleRate, bool bUnderrun, bool bOverrun) : wxCommandEvent(wxEVT_DATA,wxID_ANY),
m_pBuffer(pBuffer),
m_nCreator(nCreator),
m_nFrameCount(nFrameCount),
m_nSampleRate(nSampleRate),
m_nStatus(OK)
{
    if(bUnderrun)
    {
        m_nStatus = UNDERRUN;
    }
    else if(bOverrun)
    {
        m_nStatus = OVERRUN;
    }
}


AudioEvent::AudioEvent(const AudioEvent& event) : wxCommandEvent(event),
m_pBuffer(event.GetBuffer()),
m_nCreator(event.GetCreator()),
m_nFrameCount(event.GetFrameCount()),
m_nSampleRate(event.GetSampleRate()),
m_nStatus(event.GetStatus())
{


}

timedbuffer* AudioEvent::GetBuffer() const
{
    return m_pBuffer;
}

int AudioEvent::GetCreator() const
{
    return m_nCreator;
}

unsigned long AudioEvent::GetFrameCount() const
{
    return m_nFrameCount;
}

unsigned long AudioEvent::GetSampleRate() const
{
    return m_nSampleRate;
}

int AudioEvent::GetStatus() const
{
    return m_nStatus;
}
