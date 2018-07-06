#include "ourRTSPClient.h"
#include <wx/string.h>

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


void ourRTSPClient::PlaySubsession(const wxString& sId, responseHandler* responseHandler)
{

    MediaSubsessionIterator iterCount(*scs.session);
    MediaSubsession* pSubsession = NULL;
    while ((pSubsession = iterCount.next()) != NULL)
    {
        if(strcmp(pSubsession->sessionId(), sId.mb_str()) == 0)
        {
            break;
        }
    }
    if(pSubsession)
    {
//        envir << *((RTSPClient)this) << "Send Play Command for \"" <<scs.subsession->sessionId()<< "\" subsession \n";
        sendPlayCommand(*pSubsession, responseHandler);
    }
}

void ourRTSPClient::PauseSubsession(const wxString& sId, responseHandler* responseHandler)
{
    scs.iter = new MediaSubsessionIterator(*scs.session);

    scs.subsession = scs.iter->next();
    while (scs.subsession == NULL && strcmp(scs.subsession->sessionId(),sId.mb_str()) != 0)
    {
        scs.subsession =scs.iter->next();
    }
    if(scs.subsession != NULL)
    {
  //      envir << *((RTSPClient)this) << "Send Pause Command for \"" <<scs.subsession->sessionId()<< "\" subsession \n";
        sendPauseCommand(*scs.subsession, responseHandler);
    }
}

