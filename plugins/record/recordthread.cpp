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
        std::shared_ptr<const timedbuffer> pBuffer(nullptr);
        if(m_mutex.IsOk())
        {
            m_mutex.Lock();
            if(m_queueBuffer.empty() == false)
            {
                pBuffer = m_queueBuffer.front();
                m_queueBuffer.pop();
            }
            m_mutex.Unlock();
            if(pBuffer)
            {
                m_sf.WriteAudio(pBuffer);
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


std::shared_ptr<const timedbuffer> RecordThread::CopyBuffer(const timedbuffer* pBuffer)
{
    auto pThreadBuffer = std::make_shared<const timedbuffer>(pBuffer->GetBufferSize(), pBuffer->GetNumberOfChannels());
    pThreadBuffer->SetBuffer(pBuffer->GetBuffer());
    return pThreadBuffer;
}


void RecordThread::ClearQueue()
{
    while(m_queueBuffer.empty() == false)
    {
        m_queueBuffer.pop();
    }
}
