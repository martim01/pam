#pragma once
#include <wx/event.h>

#include "liveMedia.hh"
#include "BasicUsageEnvironment.hh"
#include "timedbuffer.h"

class RtpThread;
class Aes67MediaSubsession;

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
                           const pairTime_t& presentationTime, unsigned durationInMicroseconds);

private:
    // redefined virtual functions:
    virtual Boolean continuePlaying();

private:
    u_int8_t* fReceiveBuffer;
    Aes67MediaSubsession* m_pSubsession;
    RtpThread* m_pHandler;
    char* fStreamId;

    unsigned int m_nLastTimestamp;

};
