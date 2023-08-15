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
#include <memory>
#include <vector>


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
    m_vBuffer(nBs,0.0),
    m_vUserBits(nBs,0),
    m_nChannels(nChannels)
	{
        gettimeofday(&m_tvStamp,NULL);

        m_tvTransmissionStamp = m_tvStamp;
        m_tvPlayback = m_tvStamp;

    }

    timedbuffer(unsigned int nBs, const timeval& tv, unsigned int nTimestamp, int nChannels) :
        m_nTimestamp(nTimestamp),
        m_tvStamp(tv),
        m_tvPlayback(tv),
        m_vBuffer(nBs,0.0),
        m_vUserBits(nBs,0),
        m_nChannels(nChannels)
        {
        }

    timedbuffer(const timeval& tv, unsigned int nTimestamp, int nChannels, const std::vector<float>& vBuffer, const std::vector<unsigned char>& vUserBits) :
        m_nTimestamp(nTimestamp),
        m_tvStamp(tv),
        m_tvPlayback(tv),
        m_vBuffer(vBuffer),
        m_vUserBits(vUserBits),
        m_nChannels(nChannels)
    {

    }

	timedbuffer(const timedbuffer& tbuffer) :
		m_nTimestamp(tbuffer.GetTimestamp()),
		m_tvStamp(tbuffer.GetTimeVal()),
		m_tvPlayback(tbuffer.GetPlaybackTime()),
		m_tvTransmissionStamp(tbuffer.GetTransmissionTime()),
		m_vBuffer(tbuffer.GetBuffer()),
        m_vUserBits(tbuffer.GetUserBits()),
		m_nDuration(tbuffer.GetDuration()),
		m_nBufferDepth(tbuffer.GetBufferDepth()),
		m_dPlaybackLatency(tbuffer.GetPlaybackLatency()),
        m_nChannels(tbuffer.GetNumberOfChannels())
		{
		}

	timedbuffer& operator=(const timedbuffer& tbuffer)
	{
		if(&tbuffer != this)
		{
			m_nTimestamp = tbuffer.GetTimestamp();
			m_tvStamp = tbuffer.GetTimeVal();
			m_tvPlayback = tbuffer.GetPlaybackTime();
			m_tvTransmissionStamp = tbuffer.GetTransmissionTime();

            m_vBuffer = tbuffer.GetBuffer();
            m_vUserBits = tbuffer.GetUserBits();

			m_nDuration = tbuffer.GetDuration();
			m_nBufferDepth = tbuffer.GetBufferDepth();
			m_dPlaybackLatency = tbuffer.GetPlaybackLatency();
            m_nChannels = tbuffer.GetNumberOfChannels();
		}
		return *this;
	}

    ~timedbuffer()
    {

    }

    void SetBuffer(const float* pBuff)
    {
        m_vBuffer = {pBuff, pBuff+m_vBuffer.size()};
    }

    const std::vector<float>& GetBuffer() const
    {
        return m_vBuffer;
    }

    const std::vector<unsigned char>& GetUserBits() const
    {
        return m_vUserBits;
    }

    std::vector<float>& GetWritableBuffer()
    {
        return m_vBuffer;
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
        return m_vBuffer.size();
    }

    void Silence()
    {
        m_vBuffer = std::vector<float>(m_vBuffer.size(), 0.0);
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
        return m_vBuffer.size()/m_nChannels;
    }

    private:

    unsigned int m_nTimestamp = 0;

    timeval m_tvStamp;
    timeval m_tvPlayback;
    timeval m_tvTransmissionStamp;

    std::vector<float> m_vBuffer;
    std::vector<unsigned char> m_vUserBits;

    unsigned int m_nDuration = 0;
    unsigned int m_nBufferDepth = 0;
    double m_dPlaybackLatency = 0;
    unsigned int m_nChannels = 0;
};


