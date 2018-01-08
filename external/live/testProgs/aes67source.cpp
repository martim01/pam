#include "aes67mediasession.h"
#include "UsageEnvironment.hh"
#include "aes67source.h"
#include <string>
#include <cmath>

#include <cmath>

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

Aes67Source*
Aes67Source::createNew(UsageEnvironment& env,
			   Groupsock* RTPgs,
			   unsigned char rtpPayloadFormat,
			   unsigned rtpTimestampFrequency,
			   char const* mimeTypeString,
			   unsigned offset, Boolean doNormalMBitRule,
			   unsigned int nSync)
{
  return new Aes67Source(env, RTPgs, rtpPayloadFormat,
			     rtpTimestampFrequency,
			     mimeTypeString, offset, doNormalMBitRule, nSync);
}

Aes67Source
::Aes67Source(UsageEnvironment& env, Groupsock* RTPgs,
		  unsigned char rtpPayloadFormat,
		  unsigned rtpTimestampFrequency,
		  char const* mimeTypeString,
		  unsigned offset, Boolean doNormalMBitRule,
		  unsigned int nSync)
  : SimpleRTPSource(env, RTPgs, rtpPayloadFormat, rtpTimestampFrequency, mimeTypeString,offset, doNormalMBitRule),
  m_nTimestampFrequency(rtpTimestampFrequency),
  m_nSyncTimestamp(nSync)
  {
      WorkoutLastEpoch();
  }


void Aes67Source::WorkoutLastEpoch()
{
    double dTime = -static_cast<double>(m_nSyncTimestamp)/static_cast<double>(m_nTimestampFrequency); //this is number of seconds before Epoch that the stream started...
    //dTime *= 1000000.0;

    //get the current time
    timeval tvNow;
    gettimeofday(&tvNow, 0);


    double dNow = static_cast<double>(tvNow.tv_sec);//*1000000.0;
    dNow += static_cast<double>(tvNow.tv_usec) / 1000000.0;

    double dSecPerCycle = (4294967296.0/static_cast<double>(m_nTimestampFrequency));

    while((dTime+dSecPerCycle) < dNow)
    {
        dTime += dSecPerCycle;
    }

    double dInt, dDec;
    dDec = modf(dTime, &dInt);

    m_tvLastEpoch.tv_sec = dInt;
    m_tvLastEpoch.tv_usec = (dDec*1000000.0);

    /*

    double dNow = static_cast<double>(tvNow.tv_sec);
    dNow += static_cast<double>(tvNow.tv_usec) / 1000000.0;
    dNow += dTime;

    //divide answer by 2^32
    double dSecPerCycle = (4294967296.0/static_cast<double>(m_nTimestampFrequency));
    unsigned int nCycles = static_cast<unsigned int>(dNow / dSecPerCycle);

    //now workout when the time was
    dTime = static_cast<double>(nCycles)*dSecPerCycle;

    double dInt, dDec;
    dDec = modf(dTime, &dInt);

    m_tvLastEpoch.tv_sec = dInt;
    m_tvLastEpoch.tv_usec = (dDec*1000000.0);
    */
}


timeval Aes67Source::GetTransmissionTime()
{

    double dSeconds = static_cast<double>(fCurPacketRTPTimestamp)/static_cast<double>(m_nTimestampFrequency);

    timeval tvTransmision;
    tvTransmision.tv_sec = m_tvLastEpoch.tv_sec;
    tvTransmision.tv_usec = m_tvLastEpoch.tv_usec;

    double dInt, dDec;
    dDec = modf(dSeconds, &dInt);
    tvTransmision.tv_sec += dInt;
    tvTransmision.tv_usec += dDec*1000000.0;

    return tvTransmision;

 /*       timeval tvTransmission;
    gettimeofday(&tvTransmission,0);

    unsigned int nExpected = GetExpectedTimestamp(tvTransmission);

    double dSamples;
    dSamples = nExpected - fCurPacketRTPTimestamp;

    //divide the samples by the frequency to give us the number of seconds difference
    dSamples /= static_cast<double>(m_nTimestampFrequency);
    dSamples *= 1000000.0;  //in usec

    int64_t nTime = tvTransmission.tv_sec;
    nTime *= 1000000;
    nTime += tvTransmission.tv_usec;

    nTime -= dSamples;

    tvTransmission.tv_sec = nTime / 1000000;
    tvTransmission.tv_usec = nTime % 1000000;

    return tvTransmission;

    */
}



unsigned int Aes67Source::GetExpectedTimestamp(const timeval& tv)
{
    double dSec = tv.tv_sec;
    double dUSec  = tv.tv_usec;

    double dTime = dSec + (dUSec/1000000.0);

    //work out timestamp cycles since then
    double dCycles = static_cast<double>(m_nTimestampFrequency)*dTime;
    //add the epoch timestamp
    double dTimestamp = static_cast<double>(m_nSyncTimestamp)+dCycles;
    int64_t nTimestamp = dTimestamp;

    return static_cast<unsigned int>(nTimestamp % 4294967296);
}
