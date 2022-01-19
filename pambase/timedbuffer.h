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

#endif



class PAMBASE_IMPEXPORT timedbuffer
{
    public:
    timedbuffer(unsigned int nBs, int nChannels) :
	m_nBufferSize(nBs),
    m_nTimestamp(0),
    m_nDuration(0),
    m_nBufferDepth(0),
    m_dPlaybackLatency(0.0),
    m_nChannels(nChannels)
	{
        gettimeofday(&m_tvStamp,NULL);

        m_tvTransmissionStamp = m_tvStamp;
        m_tvPlayback = m_tvStamp;

        m_pBuffer = new float[m_nBufferSize];
    }

    timedbuffer(unsigned int nBs, const timeval& tv, unsigned int nTimestamp, int nChannels) :
        m_nBufferSize(nBs),
        m_nTimestamp(nTimestamp),
        m_tvStamp(tv),
        m_tvPlayback(tv),
		m_nDuration(0),
		m_nBufferDepth(0),
		m_dPlaybackLatency(0.0),
        m_nChannels(nChannels)
        {
            m_pBuffer = new float[m_nBufferSize];
        }

	timedbuffer(const timedbuffer& tbuffer) :
		m_nBufferSize(tbuffer.GetBufferSize()),
		m_nTimestamp(tbuffer.GetTimestamp()),
		m_tvStamp(tbuffer.GetTimeVal()),
		m_tvPlayback(tbuffer.GetPlaybackTime()),
		m_tvTransmissionStamp(tbuffer.GetTransmissionTime()),
		m_pBuffer(new float[tbuffer.GetBufferSize()]),
		m_nDuration(tbuffer.GetDuration()),
		m_nBufferDepth(tbuffer.GetBufferDepth()),
		m_dPlaybackLatency(tbuffer.GetPlaybackLatency()),
        m_nChannels(tbuffer.GetNumberOfChannels())
		{
			memcpy(m_pBuffer, tbuffer.GetBuffer(), tbuffer.GetBufferSize());
		}

	timedbuffer& operator=(const timedbuffer& tbuffer)
	{
		if(&tbuffer != this)
		{
			m_nBufferSize  = tbuffer.GetBufferSize();
			m_nTimestamp = tbuffer.GetTimestamp();
			m_tvStamp = tbuffer.GetTimeVal();
			m_tvPlayback = tbuffer.GetPlaybackTime();
			m_tvTransmissionStamp = tbuffer.GetTransmissionTime();

			m_pBuffer = new float[tbuffer.GetBufferSize()];
			memcpy(m_pBuffer, tbuffer.GetBuffer(), tbuffer.GetBufferSize());
			m_nDuration = tbuffer.GetDuration();
			m_nBufferDepth = tbuffer.GetBufferDepth();
			m_dPlaybackLatency = tbuffer.GetPlaybackLatency();
            m_nChannels = tbuffer.GetNumberOfChannels();
		}
		return *this;
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

    const timeval& GetTimeVal() const
    {
        return m_tvStamp;
    }

    void SetTimeVal(const timeval& tv)
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
    void SetTransmissionTime(const timeval& tv)
    {
        m_tvTransmissionStamp = tv;
    }

    const timeval& GetTransmissionTime() const
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
        m_tvPlayback.tv_sec = m_tvStamp.tv_sec;
        m_tvPlayback.tv_usec = m_tvStamp.tv_usec + dMicro;
        while(m_tvPlayback.tv_usec >= 1000000)
        {
            m_tvPlayback.tv_sec++;
            m_tvPlayback.tv_usec -= 1000000;
        }
        m_dPlaybackLatency = dMicro;
    }

    double GetPlaybackLatency() const
    {
        return m_dPlaybackLatency;
    }
    const timeval& GetPlaybackTime() const
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

    unsigned int GetNumberOfChannels() const
    {   return m_nChannels; }

    unsigned int GetBufferSizePerChannel() const
    {
        return m_nBufferSize/m_nChannels;
    }

    private:

    unsigned int m_nBufferSize;
    unsigned int m_nTimestamp;

    timeval m_tvStamp;
    timeval m_tvPlayback;
    timeval m_tvTransmissionStamp;
    float* m_pBuffer;

    unsigned int m_nDuration;
    unsigned int m_nBufferDepth;
    double m_dPlaybackLatency;
    unsigned int m_nChannels;
};


