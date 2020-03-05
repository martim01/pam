#include "ourSIPClient.h"
#include <wx/string.h>
#include <wx/log.h>
#include "rtpthread.h"
#include <cstdio>


static char* getLine(char* startOfLine) {
  // returns the start of the next line, or NULL if none
  for (char* ptr = startOfLine; *ptr != '\0'; ++ptr) {
    if (*ptr == '\r' || *ptr == '\n') {
      // We found the end of the line
      *ptr++ = '\0';
      if (*ptr == '\n') ++ptr;
      return ptr;
    }
  }

  return NULL;
}



ourSIPClient* ourSIPClient::createNew(UsageEnvironment& env,  const wxString& sipURL, RtpThread* pThread, int verbosityLevel, char const* applicationName)
{
    return new ourSIPClient(env, sipURL, pThread, verbosityLevel, applicationName);
}

ourSIPClient::ourSIPClient(UsageEnvironment& env,  const wxString& sipURL , RtpThread* pThread,
                             int verbosityLevel, char const* applicationName) :
    m_pClient(SIPClient::createNew(env, 96, "L24", verbosityLevel, applicationName)),
    m_pHandler(pThread),
    m_sStreamURL(sipURL),
    m_nSubsessions(0)
{

}

ourSIPClient::~ourSIPClient()
{
}

void ourSIPClient::CountSubSession()
{
    m_nSubsessions++;
}

size_t ourSIPClient::GetSubSessionCount()
{
    return m_nSubsessions;
}

/*
void ourSIPClient::PlaySubsession(const wxString& sId, responseHandler* responseHandler)
{

    MediaSubsessionIterator iterCount(*scs.session);
    MediaSubsession* pSubsession = NULL;
    while ((pSubsession = iterCount.next()) != NULL)
    {
        wxLogDebug(wxString::Format(wxT("Subsession = %s"), wxString::FromAscii(pSubsession->sessionId()).c_str()));

        if(sId == wxT("0") || strcmp(pSubsession->sessionId(), sId.mb_str()) == 0)
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

void ourSIPClient::PauseSubsession(const wxString& sId, responseHandler* responseHandler)
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
*/

void ourSIPClient::GetSDPDescription()
{

    unsigned short clientStartPortNum = 0;
    if (clientStartPortNum == 0)
        clientStartPortNum = 8000; // default

    m_pClient->setClientStartPortNum(clientStartPortNum);

    char* result = m_pClient->invite(m_sStreamURL.mb_str());


    int resultCode = result == NULL ? -1 : 0;
    if(resultCode != 0)
    {
        m_pClient->envir() << "Failed to get a SDP description for the URL \"" << m_sStreamURL.mb_str() << "\": " << result << "\n";
        delete[] result;
    }
    else
    {
        wxString sResult(wxString::FromUTF8(result));
        delete[] result;

        m_pHandler->SaveSDP(resultCode, sResult);
    }

}

void ourSIPClient::SetupSubsession(MediaSubsession* subsession)
{
    subsession->setSessionId("mumble"); // anything that's non-NULL will work

    // Parse the "Transport:" header parameters:
    // We do not send audio, but we need port for RTCP
    char* serverAddressStr;
    portNumBits serverPortNum;
    unsigned char rtpChannelId, rtcpChannelId;

    rtpChannelId = rtcpChannelId = 0xff;
    serverPortNum = 0;
    serverAddressStr = NULL;

    char* sdp = strDup(m_pClient->getInviteSdpReply());

    char* lineStart;
    char* nextLineStart = sdp;
    while (1)
    {
        lineStart = nextLineStart;
        if (lineStart == NULL)
        {
            break;
        }
        nextLineStart = getLine(lineStart);

        char* toTagStr = strDupSize(lineStart);

        if (sscanf(lineStart, "m=audio %[^/\r\n]", toTagStr) == 1)
        {
            sscanf(toTagStr, "%hu", &serverPortNum);
        }
        else if (sscanf(lineStart, "c=IN IP4 %[^/\r\n]", toTagStr) == 1)
        {
            serverAddressStr = strDup(toTagStr);
        }
        delete[] toTagStr;
    }

    if(sdp != NULL)
    {
        delete[] sdp;
    }

    delete[] subsession->connectionEndpointName();
    subsession->connectionEndpointName() = serverAddressStr;
    subsession->serverPortNum = serverPortNum;
    subsession->rtpChannelId = rtpChannelId;
    subsession->rtcpChannelId = rtcpChannelId;

    // Set the RTP and RTCP sockets' destination address and port from the information in the SETUP response (if present):
    netAddressBits destAddress = subsession->connectionEndpointAddress();
    if (destAddress != 0)
    {
        subsession->setDestinations(destAddress);
    }

}
