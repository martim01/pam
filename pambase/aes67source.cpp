#include "aes67mediasession.h"
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
      wxPtp::Get().ResyncToMaster(0);
      #endif // PTPMONKEY
      WorkoutLastEpoch();
  }


void Aes67Source::WorkoutLastEpoch()
{
    double dTime = -static_cast<double>(m_nSyncTimestamp)/static_cast<double>(m_nTimestampFrequency); //this is number of seconds before Epoch that the stream started...
    //dTime *= 1000000.0;

    //get the current time
    timeval tvNow;
    #ifdef PTPMONKEY

    if(wxPtp::Get().IsSyncedToMaster(0))
    {
        tvNow = wxPtp::Get().GetPtpTime(0); //need to set the domain
        m_bEpochWorkedOut = true;
    }
    else
    {

        gettimeofday(&tvNow, nullptr);
    }
    #else
    gettimeofday(&tvNow, nullptr);
    m_bEpochWorkedOut = true;
    #endif // PTPMONKEY

    double dNow = static_cast<double>(tvNow.tv_sec);//*1000000.0;
    dNow += static_cast<double>(tvNow.tv_usec) / 1000000.0;

    double dSecPerCycle = (4294967296.0/static_cast<double>(m_nTimestampFrequency));

    while((dTime+dSecPerCycle) < dNow)
    {
        dTime += dSecPerCycle;
    }

    double dInt, dDec;
    dDec = modf(dTime, &dInt);

    m_tvLastEpoch.first = dInt;
    m_tvLastEpoch.second = (dDec*1000000.0);


}


pairTime_t Aes67Source::GetTransmissionTime()
{

    double dSeconds = static_cast<double>(fCurPacketRTPTimestamp)/static_cast<double>(m_nTimestampFrequency);

    if(!m_bEpochWorkedOut)
    {
        WorkoutLastEpoch();
    }
    pairTime_t tvTransmision;
    tvTransmision.first = m_tvLastEpoch.first;
    tvTransmision.second = m_tvLastEpoch.second;

    double dInt, dDec;
    dDec = modf(dSeconds, &dInt);
    tvTransmision.first += dInt;
    tvTransmision.second += dDec*1000000.0;

    if(tvTransmision.second > 1000000.0)
    {
        tvTransmision.second -= 1000000.0;
        tvTransmision.first ++;
    }


    return tvTransmision;
}



unsigned int Aes67Source::GetExpectedTimestamp(const pairTime_t& tv)
{
    double dSec = tv.first;
    double dUSec  = tv.second;

    double dTime = dSec + (dUSec/1000000.0);

    //work out timestamp cycles since then
    double dCycles = static_cast<double>(m_nTimestampFrequency)*dTime;
    //add the epoch timestamp
    double dTimestamp = static_cast<double>(m_nSyncTimestamp)+dCycles;
    int64_t nTimestamp = dTimestamp;

    return static_cast<unsigned int>(nTimestamp % 4294967296);
}
