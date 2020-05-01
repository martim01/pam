#pragma once
#ifdef __WXMSW__
#include "lame.h"
#endif // __WXMSW__
#ifdef __WXGNU__
#include "lame/lame.h"
#endif // __WXGNU__
#include <vector>
#include <list>
#include <mutex>
#include <map>


struct MP3Parameters
{
    MP3Parameters() : nChannels(2), nSampleRate(48000), nBitRate(320), nQuality(2), nBuffer(5){}
    unsigned int nChannels;
    unsigned int nSampleRate;
    unsigned int nBitRate;
    unsigned int nQuality;
    unsigned int nBytesPerFrame;
    unsigned int nDurationUs;
    size_t nBuffer;

};

struct mp3frame
{
    mp3frame(int64_t nI=-1, unsigned int nSize=0) : nIndex(nI), vData(nSize){}
    mp3frame(const mp3frame& fr) : nIndex(fr.nIndex), vData(fr.vData){}

    int64_t nIndex;

    std::vector<unsigned char> vData;

};


class MP3Encoder
{
    public:
        MP3Encoder();
        ~MP3Encoder();
        bool InitLame(const MP3Parameters& params = MP3Parameters());
        bool StopLame();
        unsigned int GetFrameDuration() const
        {
            return m_params.nDurationUs;
        }
        const MP3Parameters& GetParameters() const
        {
            return m_params;
        }

        void AddSamples(const float* pBuffer, size_t nSize);

        // Returns a unique id
        int RegisterForFrames();
        void Unregister(int nRegistrationId);
        const mp3frame& GetNextFrame(int nRegistrationId);


        void FlushBuffer();

    private:

        void WorkoutLeastIndex();
        MP3Parameters m_params;
        lame_global_flags* m_pLame;
        std::list<mp3frame> m_lstFrames;

        unsigned int m_nBytesWritten;
        int m_nTimeBuffer;
        std::mutex m_mutex;

        int64_t m_nBuffersWritten;
        int64_t m_nLeastIndex;
        std::map<int, std::list<mp3frame>::const_iterator > m_mReader;
        mp3frame m_empty;
};
