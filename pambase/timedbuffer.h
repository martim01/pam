#pragma once
#include "dlldefine.h"
#include <wx/datetime.h>
#ifdef __WXGNU__
#include <sys/time.h>
#else
#include "winsock2.h"
#include "windows.h"
#include <wx/msw/winundef.h>
#endif // __WXGNU__

typedef std::pair<unsigned int, unsigned int> pairTime_t;

#ifdef  __WXMSW__
#ifndef _SYS_TIME_H_
static int gettimeofday(struct timeval * tp, struct timezone * tzp)
{
    // Note: some broken versions only have 8 trailing zero's, the correct epoch has 9 trailing zero's
    // This magic number is the number of 100 nanosecond intervals since January 1, 1601 (UTC)
    // until 00:00:00 January 1, 1970
    static const uint64_t EPOCH = ((uint64_t) 116444736000000000ULL);

    SYSTEMTIME  system_time;
    FILETIME    file_time;
    uint64_t    time;

    GetSystemTime( &system_time );
    SystemTimeToFileTime( &system_time, &file_time );
    time =  ((uint64_t)file_time.dwLowDateTime )      ;
    time += ((uint64_t)file_time.dwHighDateTime) << 32;

    tp->tv_sec  = (long) ((time - EPOCH) / 10000000L);
    tp->tv_usec = (long) (system_time.wMilliseconds * 1000);
    return 0;
}
#endif

static int gettimeofday(pairTime_t& tp, struct timezone * tzp)
{
    // Note: some broken versions only have 8 trailing zero's, the correct epoch has 9 trailing zero's
    // This magic number is the number of 100 nanosecond intervals since January 1, 1601 (UTC)
    // until 00:00:00 January 1, 1970
    static const uint64_t EPOCH = ((uint64_t) 116444736000000000ULL);

    SYSTEMTIME  system_time;
    FILETIME    file_time;
    uint64_t    time;

    GetSystemTime( &system_time );
    SystemTimeToFileTime( &system_time, &file_time );
    time =  ((uint64_t)file_time.dwLowDateTime )      ;
    time += ((uint64_t)file_time.dwHighDateTime) << 32;

    tp.first  = (long) ((time - EPOCH) / 10000000L);
    tp.second = (long) (system_time.wMilliseconds * 1000);
    return 0;
}
#else
static int gettimeofday(pairTime_t& tp, struct timezone * tzp)
{
    timeval tv;
    gettimeofday(&tv, NULL);
    tp.first = tv.tv_sec;
    tp.second = tv.tv_usec;
    return 0;
}
#endif



class PAMBASE_IMPEXPORT timedbuffer
{
    public:
    timedbuffer(unsigned int nBs) :
	m_nBufferSize(nBs),
    m_nTimestamp(0),
    m_nDuration(0),
    m_nBufferDepth(0),
    m_dPlaybackLatency(0.0)
	{
        gettimeofday(m_tvStamp,NULL);

        m_tvTransmissionStamp = m_tvStamp;
        m_tvPlayback = m_tvStamp;

        m_pBuffer = new float[m_nBufferSize];
    }

    timedbuffer(unsigned int nBs, const pairTime_t& tv, unsigned int nTimestamp) :
        m_nBufferSize(nBs),
        m_nTimestamp(nTimestamp),
        m_tvStamp(tv),
        m_tvPlayback(tv),
		m_nDuration(0),
		m_nBufferDepth(0),
		m_dPlaybackLatency(0.0)
        {
            m_pBuffer = new float[m_nBufferSize];
        }

		

    ~timedbuffer()
    {
        delete[] m_pBuffer;
    }

    void SetBuffer(const float* pBuff)
    {
        memcpy(m_pBuffer, pBuff, m_nBufferSize*sizeof(float));
    }

    const float* GetBuffer() const
    {
        return m_pBuffer;
    }

    float* GetWritableBuffer()
    {
        return m_pBuffer;
    }

    const pairTime_t& GetTimeVal() const
    {
        return m_tvStamp;
    }

    void SetTimeVal(const pairTime_t& tv)
    {
        m_tvStamp = tv;
    }

    unsigned int GetBufferSize() const
    {
        return m_nBufferSize;
    }

    void Silence()
    {
        memset(m_pBuffer, 0, m_nBufferSize*sizeof(float));
    }

    void SetDuration(unsigned int nDuration)
    {
        m_nDuration = nDuration;
    }

    unsigned int GetDuration() const
    {
        return m_nDuration;
    }
    void SetTransmissionTime(const pairTime_t& tv)
    {
        m_tvTransmissionStamp = tv;
    }

    const pairTime_t& GetTransmissionTime() const
    {
        return m_tvTransmissionStamp;
    }

    void SetTimestamp(unsigned int nTimestamp)
    {
        m_nTimestamp = nTimestamp;
    }

    unsigned int GetTimestamp() const
    {
        return m_nTimestamp;
    }

    void SetPlaybackOffset(double dMicro)
    {
        m_tvPlayback.first = m_tvStamp.first;
        m_tvPlayback.second = m_tvStamp.second + dMicro;
        while(m_tvPlayback.second >= 1000000)
        {
            m_tvPlayback.first++;
            m_tvPlayback.second -= 1000000;
        }
        m_dPlaybackLatency = dMicro;
    }

    double GetPlaybackLatency() const
    {
        return m_dPlaybackLatency;
    }
    const pairTime_t& GetPlaybackTime() const
    {
        return m_tvPlayback;
    }

    void SetBufferDepth(unsigned int nDepth)
    {
        m_nBufferDepth = nDepth;
    }
    unsigned int GetBufferDepth() const
    {
        return m_nBufferDepth;
    }



    private:

    unsigned int m_nBufferSize;
    unsigned int m_nTimestamp;

    pairTime_t m_tvStamp;
    pairTime_t m_tvPlayback;
    pairTime_t m_tvTransmissionStamp;
    float* m_pBuffer;

    unsigned int m_nDuration;
    unsigned int m_nBufferDepth;
    double m_dPlaybackLatency;
};


