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
        std::shared_ptr<timedbuffer> pBuffer(nullptr);
        if(m_mutex.IsOk())
        {
            m_mutex.Lock();
            if(m_queueBuffer.empty() == false)
            {
                pBuffer = FilterBuffer(m_queueBuffer.front());
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
        m_queueBuffer.push(std::make_shared<timedbuffer>(*pBuffer));
    }

}


std::shared_ptr<timedbuffer> RecordThread::CopyBuffer(const timedbuffer* pBuffer)
{
    auto pThreadBuffer = std::make_shared<timedbuffer>(*pBuffer);
    return pThreadBuffer;
}

std::shared_ptr<timedbuffer> RecordThread::FilterBuffer(std::shared_ptr<timedbuffer> pBuffer)
{
    //if we are recording everything then do the faster copy rather than filter
    if(m_vChannels.size() == pBuffer->GetNumberOfChannels())
    {
        return CopyBuffer(pBuffer.get());
    }

    //just get the samples for the channels we want to record
    auto pFilter = std::make_shared<timedbuffer>(pBuffer->GetBufferSizePerChannel()*m_vChannels.size(), m_vChannels.size());

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
        m_queueBuffer.pop();
    }
}
