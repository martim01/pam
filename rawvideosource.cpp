#include "aes67mediasession.h"
#include "UsageEnvironment.hh"
#include "rawvideosource.h"
#include <string>
#include <cmath>
#include <cmath>
#include <wx/datetime.h>
#include <wx/log.h>


RawVideoSource*
RawVideoSource::createNew(UsageEnvironment& env,
			   Groupsock* RTPgs,
			   unsigned char rtpPayloadFormat,
			   unsigned rtpTimestampFrequency,
			   char const* mimeTypeString,
			   unsigned offset, Boolean doNormalMBitRule,
			   unsigned int nSync)
{
  return new RawVideoSource(env, RTPgs, rtpPayloadFormat,
			     rtpTimestampFrequency,
			     mimeTypeString, offset, doNormalMBitRule, nSync);
}

RawVideoSource
::RawVideoSource(UsageEnvironment& env, Groupsock* RTPgs,
		  unsigned char rtpPayloadFormat,
		  unsigned rtpTimestampFrequency,
		  char const* mimeTypeString,
		  unsigned offset, Boolean doNormalMBitRule,
		  unsigned int nSync)
  : Aes67Source(env, RTPgs, rtpPayloadFormat, rtpTimestampFrequency, mimeTypeString,offset, doNormalMBitRule, nSync)
  {
      //WorkoutLastEpoch();
  }



Boolean RawVideoSource::processSpecialHeader(BufferedPacket* packet,unsigned& resultSpecialHeaderSize)
{
    unsigned char* pHeader = packet->data();
    unsigned int nPacketSize = packet->dataSize();

    unsigned int nPlace(0);
    //first 16 bits are extended sequence high
    unsigned short nSequenceHigh = (static_cast<short>(pHeader[nPlace]) << 8) | pHeader[nPlace+1];
    //LooP
    nPlace += 2;
    bool bMore(true);
    do
    {
        //next 16 bits are number of bytes of SRD
        unsigned short nSRDLength = (static_cast<short>(pHeader[nPlace]) << 8) | pHeader[nPlace+1];
        nPlace+=2;

        //next bit is  field identifier
        bool bField = ((pHeader[nPlace] | 0x80)!=0);

        //next 15 bits are SRD row id
        unsigned short nSRDRowId = (static_cast<short>(pHeader[nPlace]|0x7F) << 8) | pHeader[nPlace+1];
        nPlace+=2;

        //next bit is 1 if there is more than one row = C
        bMore = ((pHeader[nPlace] | 0x80)!=0);

        //next 15 bits are SRD offset
        unsigned short nSRDRowOffset = (static_cast<short>(pHeader[nPlace]|0x7F) << 8) | pHeader[nPlace+1];
        nPlace += 2;

    }while(bMore); //Loop if C==1 and we are smaller than packet size

    resultSpecialHeaderSize = nPlace;
    return True;
}
