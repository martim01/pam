#pragma once
#include "MediaSession.hh"
#include "SimpleRTPSource.hh"
#include "timedbuffer.h"
#include "aes67source.h"


class RawVideoSource: public Aes67Source {
public:
  static RawVideoSource* createNew(UsageEnvironment& env, Groupsock* RTPgs,
				    unsigned char rtpPayloadFormat,
				    unsigned rtpTimestampFrequency,
				    char const* mimeTypeString,
				    unsigned offset = 0,
				    Boolean doNormalMBitRule = True,
				    unsigned int nSyncTime=0);
    u_int32_t GetRTPTimestamp() const { return fCurPacketRTPTimestamp; }
    pairTime_t GetTransmissionTime();

    Boolean processSpecialHeader(BufferedPacket* packet,unsigned& resultSpecialHeaderSize);

protected:
  RawVideoSource(UsageEnvironment& env, Groupsock* RTPgs,
		  unsigned char rtpPayloadFormat,
		  unsigned rtpTimestampFrequency,
		  char const* mimeTypeString, unsigned offset,
		  Boolean doNormalMBitRule,
		  unsigned int nSyncTime);
      // called only by createNew()


};


