//#include "aes67mediasession.h"
#include "UsageEnvironment.hh"
#include "aes67source.h"
#include <string>
#include <cmath>
#include <wx/datetime.h>
#include <wx/log.h>
#include "wxptp.h"

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
  m_bEpochWorkedOut(false),
  m_nTimestampFrequency(rtpTimestampFrequency),
  m_nSyncTimestamp(nSync)
  {
      #ifdef PTPMONKEY
      wxPtp::Get().ResyncToMaster();
      #endif // PTPMONKEY
      WorkoutLastEpoch();
  }


void Aes67Source::WorkoutLastEpoch()
{
    double dTime = -static_cast<double>(m_nSyncTimestamp)/static_cast<double>(m_nTimestampFrequency); //this is number of seconds before Epoch that the stream started...
    //dTime *= 1000000.0;

    //get the current time
    timeval tvNow;
    gettimeofday(&tvNow, nullptr);

    m_bEpochWorkedOut = true;
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


}


timeval Aes67Source::GetTransmissionTime()
{

    double dSeconds = static_cast<double>(fCurPacketRTPTimestamp)/static_cast<double>(m_nTimestampFrequency);

    if(!m_bEpochWorkedOut)
    {
        WorkoutLastEpoch();
    }
    timeval tvTransmision;
    tvTransmision.tv_sec = m_tvLastEpoch.tv_sec;
    tvTransmision.tv_usec = m_tvLastEpoch.tv_usec;

    double dInt, dDec;
    dDec = modf(dSeconds, &dInt);
    tvTransmision.tv_sec += dInt;
    tvTransmision.tv_usec += dDec*1000000.0;

    if(tvTransmision.tv_usec > 1000000.0)
    {
        tvTransmision.tv_usec -= 1000000.0;
        tvTransmision.tv_sec ++;
    }


    return tvTransmision;
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
