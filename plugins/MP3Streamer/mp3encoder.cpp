#include "mp3encoder.h"
#include <iostream>
#include <chrono>
#include <array>
#include "log.h"
#include <wx/utils.h>

MP3Encoder::MP3Encoder() : m_pLame(0), m_nTimeBuffer(0), m_nBytesWritten(0), m_nBuffersWritten(0), m_nLeastIndex(0)
{
}

MP3Encoder::~MP3Encoder()
{
    StopLame();
}

bool MP3Encoder::StopLame()
{
    if(m_pLame)
    {
        lame_close(m_pLame);
        m_pLame = 0;
        FlushBuffer();
    }
    return true;
}

bool MP3Encoder::InitLame(const MP3Parameters& params)
{
    m_params = params;
    m_pLame = lame_init();
    if(m_pLame == nullptr)
    {
        return false;
    }

    lame_set_num_channels(m_pLame,m_params.nChannels);
    lame_set_in_samplerate(m_pLame,m_params.nSampleRate);
    lame_set_brate(m_pLame,m_params.nBitRate);
    lame_set_mode(m_pLame,STEREO);
    lame_set_quality(m_pLame,m_params.nQuality);   /* 2=high  5 = medium  7=low */

    m_params.nBytesPerFrame = ((1152 * m_params.nBitRate*1000) / m_params.nSampleRate)/8;
    m_params.nDurationUs =  (1152*1e6)/(static_cast<double>(m_params.nSampleRate));

    int ret_code = lame_init_params(m_pLame);
    if(ret_code <0)
    {
        return false;
    }


    return true;
}



void MP3Encoder::AddSamples(const float* pBuffer, size_t nSize)
{
    if(nSize == 0)
        return;

    if(m_lstFrames.empty())
    {
        m_lstFrames.push_back(mp3frame(m_nBuffersWritten, m_params.nBytesPerFrame));
        ++m_nBuffersWritten;
        m_nBytesWritten = 0;
    }


    unsigned int nBufferSize = 7200+(0.625*nSize);
    std::vector<unsigned char> mp3Buffer(nBufferSize);

    int nBytesOut = lame_encode_buffer_interleaved_ieee_float(m_pLame, pBuffer, nSize/2, mp3Buffer.data(), nBufferSize);

    if(nBytesOut > 0)
    {
        for(size_t i = 0; i < nBytesOut; i++)
        {
            if(m_nBytesWritten == m_params.nBytesPerFrame)
            {   //add new frame
                m_lstFrames.push_back(mp3frame(m_nBuffersWritten, m_params.nBytesPerFrame));

                if(m_mReader.empty())
                {
                    m_nLeastIndex = m_nBuffersWritten;
                }
                ++m_nBuffersWritten;

                std::lock_guard<std::mutex> lg(m_mutex);
                while(m_lstFrames.front().nIndex < m_nLeastIndex && m_lstFrames.size() > m_params.nBuffer)
                {
                    m_lstFrames.pop_front();
                }


                m_nBytesWritten = 0;
            }

            if(m_nBytesWritten < m_params.nBytesPerFrame)
            {
                m_lstFrames.back().vData[m_nBytesWritten] = mp3Buffer[i];
                m_nBytesWritten++;
            }
        }
    }
    else if(nBytesOut < 0)
    {
        pmlLog(pml::LOG_ERROR) << "MP3Streamer\t" << "Unable to encode audio data";
    }
}

int MP3Encoder::RegisterForFrames()
{
    std::lock_guard<std::mutex> lg(m_mutex);
    if(m_lstFrames.size() < 2)
    {
        pmlLog(pml::LOG_WARN) << "MP3Streamer\t" << "Not enough frames";
        return -1;
    }
    return m_mReader.insert(std::make_pair(wxNewId(), m_lstFrames.begin())).first->first;

}

const mp3frame& MP3Encoder::GetNextFrame(int nRegistrationId)
{
    auto itReader = m_mReader.find(nRegistrationId);
    if(itReader != m_mReader.end() && itReader->second != m_lstFrames.end())
    {
        ++(itReader->second);
        if(itReader->second != m_lstFrames.end())
        {
            WorkoutLeastIndex();

            return *(itReader->second);
        }
        else
        {
            return m_empty;
        }
    }
    else
    {
        return m_empty;
    }
}

void MP3Encoder::WorkoutLeastIndex()
{
    int64_t nLeast(-1);
    int nId;
    for(auto pairR : m_mReader)
    {
        if(nLeast < 0)
        {
            nLeast = pairR.second->nIndex;
            nId = pairR.first;
        }
        else
        {
            nId = pairR.first;
            nLeast = std::min(nLeast, pairR.second->nIndex);
        }
    }
    m_nLeastIndex = nLeast;
}

void MP3Encoder::FlushBuffer()
{
    m_lstFrames.clear();
    m_mReader.clear();
    m_nLeastIndex = m_nBuffersWritten;

}

void MP3Encoder::Unregister(int nRegistrationId)
{
    m_mReader.erase(nRegistrationId);
    WorkoutLeastIndex();
}
