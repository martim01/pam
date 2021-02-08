#pragma once
#include "MediaSession.hh"
#include "SimpleRTPSource.hh"
#include "timedbuffer.h"


class Aes67Source: public SimpleRTPSource {
public:
  static Aes67Source* createNew(UsageEnvironment& env, Groupsock* RTPgs,
				    unsigned char rtpPayloadFormat,
				    unsigned rtpTimestampFrequency,
				    char const* mimeTypeString,
				    unsigned offset = 0,
				    Boolean doNormalMBitRule = True,
				    unsigned int nSyncTime=0);
    u_int32_t GetRTPTimestamp() const { return fCurPacketRTPTimestamp; }
    timeval GetTransmissionTime();

    const timeval& GetLastEpoch()
    {
        return m_tvLastEpoch;
    }

    unsigned int GetSyncTimestamp()
    {
        return m_nSyncTimestamp;
    }

    unsigned int GetFrequency()
    {
        return m_nTimestampFrequency;
    }

    unsigned int GetExpectedTimestamp(const timeval& tv);

    void WorkoutLastEpoch();

protected:
  Aes67Source(UsageEnvironment& env, Groupsock* RTPgs,
		  unsigned char rtpPayloadFormat,
		  unsigned rtpTimestampFrequency,
		  char const* mimeTypeString, unsigned offset,
		  Boolean doNormalMBitRule,
		  unsigned int nSyncTime);
      // called only by createNew()

private:


    timeval m_tvLastEpoch;
    bool m_bEpochWorkedOut;

    unsigned int m_nTimestampFrequency;
    unsigned int m_nSyncTimestamp;

};

