#pragma once
#include <wx/event.h>
#include <map>
#include "liveMedia.hh"
#include "BasicUsageEnvironment.hh"
#include "timedbuffer.h"
#include <memory>

class RtpThread;
class Smpte2110MediaSubsession;


bool operator<(const timeval& t1, const timeval& t2);

typedef std::map<unsigned char, unsigned char*> mExtension_t;



struct rtpFrame
{
    wxString sEndpoint;
    wxString sGroup;
    unsigned long nSSRC = 0;

    timeval timePresentation;
    timeval timeTransmission;
    unsigned int nTimestamp = 0;
    int nTimestampDifference = 0;
    
    unsigned long nFrameSize = 0;
    u_int8_t* pBuffer = nullptr;
    u_int8_t nBytesPerSample = 0;
        
    mExtension_t* mExt = nullptr;
};

class wxSink: public MediaSink
{
public:
    static wxSink* createNew(UsageEnvironment& env,
                                MediaSubsession& subsession, // identifies the kind of data that's being received
                                RtpThread* pHandler,
                                char const* streamId = NULL); // identifies the stream itself (optional)

private:
    wxSink(UsageEnvironment& env, MediaSubsession& subsession, RtpThread* pHandler,char const* streamId);
    // called only by "createNew()"
    virtual ~wxSink();

    static void afterGettingFrame(void* clientData, unsigned frameSize,
                                  unsigned numTruncatedBytes,
                                  timeval presentationTime,
                                  unsigned durationInMicroseconds);
    void afterGettingFrame(unsigned frameSize, unsigned numTruncatedBytes,
                           const timeval& tvPresentation, unsigned durationInMicroseconds);


    void rtpExtensionCallback(unsigned int nDefinedByProfile, unsigned char* pExtHdrData, unsigned nExtHdrDataLen, struct timeval& presentationTime, unsigned short nRtpSeqNo, unsigned nRtpTimestamp, bool bRtpMarkerBitSet);

    static void rtpExtensionCallback(unsigned int definedByProfile, unsigned char* extHdrData, unsigned extHdrDataLen, struct timeval& presentationTime, unsigned short rtpSeqNo, unsigned rtpTimestamp, bool rtpMarkerBitSet, void* pPriv);




private:
    // redefined virtual functions:
    virtual Boolean continuePlaying();

    u_int8_t* fReceiveBuffer;
    Smpte2110MediaSubsession* m_pSubsession;
    RtpThread* m_pHandler;
    char* fStreamId;

    unsigned int m_nLastTimestamp = 0;

    std::map<timeval, mExtension_t*> m_mExtension;

};


