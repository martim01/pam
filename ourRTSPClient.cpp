#include "ourRTSPClient.h"


ourRTSPClient* ourRTSPClient::createNew(UsageEnvironment& env, char const* rtspURL,RtpThread* pThread,
                                        int verbosityLevel, char const* applicationName, portNumBits tunnelOverHTTPPortNum)
{
    return new ourRTSPClient(env, rtspURL, pThread, verbosityLevel, applicationName, tunnelOverHTTPPortNum);
}

ourRTSPClient::ourRTSPClient(UsageEnvironment& env, char const* rtspURL,RtpThread* pThread,
                             int verbosityLevel, char const* applicationName, portNumBits tunnelOverHTTPPortNum)
    : RTSPClient(env,rtspURL, verbosityLevel, applicationName, tunnelOverHTTPPortNum, -1),
    m_pHandler(pThread),
    m_nSubsessions(0)
{
}

ourRTSPClient::~ourRTSPClient()
{
}

void ourRTSPClient::CountSubSession()
{
    m_nSubsessions++;
}

size_t ourRTSPClient::GetSubSessionCount()
{
    return m_nSubsessions;
}



