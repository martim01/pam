#include "recordthread.h"
#include "settings.h"
#include "timedbuffer.h"
#include <wx/log.h>

using namespace std;


RecordThread::RecordThread() : wxThread(wxTHREAD_JOINABLE), m_bLoop(true)
{

}

bool RecordThread::Init(const wxString& sFilename, unsigned int nChannels, unsigned int nSampleRate, unsigned int nBitRate)
{
    return m_sf.OpenToWrite(sFilename, nChannels, nSampleRate, nBitRate);
}

void RecordThread::Stop()
{
    m_bLoop = false;
}

void* RecordThread::Entry()
{
    while(m_bLoop)
    {
        timedbuffer* pBuffer(0);
        if(m_mutex.IsOk())
        {
            m_mutex.Lock();
            if(m_queueBuffer.empty() == false)
            {
                pBuffer = CopyBuffer(m_queueBuffer.front());
                delete m_queueBuffer.front();
                m_queueBuffer.pop();
            }
            m_mutex.Unlock();
            if(pBuffer)
            {
                m_sf.WriteAudio(pBuffer);
                delete pBuffer;
            }
            else
            {
                wxMilliSleep(100);  //sleep for 100ms
            }
        }
    }
    ClearQueue();
    return NULL;
}

void RecordThread::AddToBuffer(const timedbuffer* pBuffer)
{
    if(m_mutex.IsOk())
    {
        wxMutexLocker ml(m_mutex);
        m_queueBuffer.push(CopyBuffer(pBuffer));
    }

}


timedbuffer* RecordThread::CopyBuffer(const timedbuffer* pBuffer)
{
    timedbuffer* pThreadBuffer = new timedbuffer(pBuffer->GetBufferSize());
    pThreadBuffer->SetBuffer(pBuffer->GetBuffer());
    return pThreadBuffer;
}


void RecordThread::ClearQueue()
{
    while(m_queueBuffer.empty() == false)
    {
        delete m_queueBuffer.front();
        m_queueBuffer.pop();
    }
}
