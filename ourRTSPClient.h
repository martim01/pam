#pragma once


#include "liveMedia.hh"
#include "BasicUsageEnvironment.hh"
#include "streamclientstate.h"


class RtpThread;

class ourRTSPClient: public RTSPClient
{
public:
    static ourRTSPClient* createNew(UsageEnvironment& env, char const* rtspURL, RtpThread* pThread,
                                    int verbosityLevel = 0,
                                    char const* applicationName = NULL,
                                    portNumBits tunnelOverHTTPPortNum = 0);



    RtpThread* GetHandler()
    {
        return m_pHandler;
    }

    void SetHandler(RtpThread* pThread)
    {
        m_pHandler = pThread;
    }


protected:
    ourRTSPClient(UsageEnvironment& env, char const* rtspURL, RtpThread* pThread,
                  int verbosityLevel, char const* applicationName, portNumBits tunnelOverHTTPPortNum);
    // called only by createNew();
    virtual ~ourRTSPClient();

    RtpThread* m_pHandler;
public:
    StreamClientState scs;

};

