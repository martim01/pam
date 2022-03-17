#include "recordthread.h"
#include "settings.h"
#include "timedbuffer.h"
#include <wx/log.h>
#include <iostream>

using namespace std;


RecordThread::RecordThread() : wxThread(wxTHREAD_JOINABLE), m_bLoop(true)
{

}

bool RecordThread::Init(const wxString& sFilename, const std::vector<unsigned char>& vChannels, unsigned int nSampleRate, unsigned int nBitRate)
{
    m_vChannels = vChannels;


    return m_sf.OpenToWrite(sFilename, vChannels.size(), nSampleRate, nBitRate);
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
                pBuffer = FilterBuffer(m_queueBuffer.front());
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
    timedbuffer* pThreadBuffer = new timedbuffer(pBuffer->GetBufferSize(), pBuffer->GetNumberOfChannels());
    pThreadBuffer->SetBuffer(pBuffer->GetBuffer());
    return pThreadBuffer;
}

timedbuffer* RecordThread::FilterBuffer(const timedbuffer* pBuffer)
{
    //if we are recording everything then do the faster copy rather than filter
    if(m_vChannels.size() == pBuffer->GetNumberOfChannels())
    {
        return CopyBuffer(pBuffer);
    }

    //just get the samples for the channels we want to record
    timedbuffer* pFilter = new timedbuffer(pBuffer->GetBufferSizePerChannel()*m_vChannels.size(), m_vChannels.size());

    size_t nCount = 0;
    for(size_t i = 0; i < pBuffer->GetBufferSize(); i+= pBuffer->GetNumberOfChannels())
    {
        for(auto nChannel : m_vChannels)
        {
            pFilter->GetWritableBuffer()[nCount] = pBuffer->GetBuffer()[i+nChannel];
            ++nCount;
        }
    }
    return pFilter;
}

void RecordThread::ClearQueue()
{
    while(m_queueBuffer.empty() == false)
    {
        delete m_queueBuffer.front();
        m_queueBuffer.pop();
    }
}
