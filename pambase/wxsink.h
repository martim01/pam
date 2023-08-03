#pragma once
#include <wx/event.h>
#include <map>
#include "liveMedia.hh"
#include "BasicUsageEnvironment.hh"
#include "timedbuffer.h"
#include <memory>
#include "rtpframeevent.h"

class RtpThread;
class Smpte2110MediaSubsession;


bool operator<(const timeval& t1, const timeval& t2);


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

    double m_dTSDFMax = std::numeric_limits<double>::lowest();
	double m_dTSDFMin = std::numeric_limits<double>::max();
    double m_dTSDF = std::numeric_limits<double>::lowest();
	double m_nTSDFCount = 0;
	double m_dDelay0 = std::numeric_limits<double>::lowest();
    timeval m_tvDelay0;

};


