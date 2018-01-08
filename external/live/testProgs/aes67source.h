#pragma once
#include "MediaSession.hh"
#include "SimpleRTPSource.hh"

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

    timeval GetLastEpoch()
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

protected:
  Aes67Source(UsageEnvironment& env, Groupsock* RTPgs,
		  unsigned char rtpPayloadFormat,
		  unsigned rtpTimestampFrequency,
		  char const* mimeTypeString, unsigned offset,
		  Boolean doNormalMBitRule,
		  unsigned int nSyncTime);
      // called only by createNew()

private:

    void WorkoutLastEpoch();
    timeval m_tvLastEpoch;

    unsigned int m_nTimestampFrequency;
    unsigned int m_nSyncTimestamp;

};

