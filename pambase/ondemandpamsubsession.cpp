#include "ondemandpamsubsession.h"
#include "PamUsageEnvironment.h"
#include "ondemandstreamer.h"
#include <iostream>
#include "log.h"


OnDemandPamSubsession::OnDemandPamSubsession(wxEvtHandler* pAudioHandler, PamUsageEnvironment& env,  portNumBits initialPortNum) :
    OnDemandServerMediaSubsession(env, True, initialPortNum, False),
    m_pAudioHandler(pAudioHandler),
    m_nConnections(0),
    m_pSink(nullptr),
    m_env(env)
{

}

OnDemandPamSubsession::~OnDemandPamSubsession()
{
}

char const* OnDemandPamSubsession::getAuxSDPLine(RTPSink* rtpSink, FramedSource* /*inputSource*/)
{
  // Default implementation:
  if(rtpSink != nullptr)
  {
    return rtpSink->auxSDPLine();
  }

  return NULL;
}

void OnDemandPamSubsession::getStreamParameters(unsigned clientSessionId,
				   struct sockaddr_storage const& clientAddress,
                                   Port const& clientRTPPort,
                                   Port const& clientRTCPPort,
				   int tcpSocketNum,
                                   unsigned char rtpChannelId,
                                   unsigned char rtcpChannelId,
                                   struct sockaddr_storage& destinationAddress,
				   u_int8_t& destinationTTL,
                                   Boolean& isMulticast,
                                   Port& serverRTPPort,
                                   Port& serverRTCPPort,
                                   void*& streamToken)
{
    OnDemandServerMediaSubsession::getStreamParameters(clientSessionId, clientAddress, clientRTPPort, clientRTCPPort, tcpSocketNum, rtpChannelId, rtcpChannelId,destinationAddress, destinationTTL, isMulticast, serverRTPPort, serverRTCPPort, streamToken);

    m_nConnections++;

    for(auto pHandler : m_setRTSPHandlers)
    {
        wxCommandEvent* pEvent = new wxCommandEvent(wxEVT_ODS_CONNECTION);
        Destinations* pDestinations = (Destinations*)(fDestinationsHashTable->Lookup((char const*)clientSessionId));
        if(pDestinations)
        {
//            pEvent->SetString(wxString::FromUTF8(inet_ntoa(pDestinations->addr))); //@todo fix this ip address
            pEvent->SetInt(ntohs(pDestinations->rtpPort.num()));
            pEvent->SetExtraLong(ntohs(pDestinations->rtcpPort.num()));
            pEvent->SetClientData((void*)clientSessionId);
        }
        else
        {
            pEvent->SetString("Unknown");
            pEvent->SetInt(0);
            pEvent->SetClientData(0);
        }
        wxQueueEvent(pHandler, pEvent);
    }
}


void OnDemandPamSubsession::deleteStream(unsigned clientSessionId, void*& streamToken)
{
    for(auto pHandler : m_setRTSPHandlers)
    {
        wxCommandEvent* pEvent = new wxCommandEvent(wxEVT_ODS_DISCONNECTION);

        Destinations* pDestinations = (Destinations*)(fDestinationsHashTable->Lookup((char const*)clientSessionId));
        if(pDestinations)
        {
          //  pEvent->SetString(wxString::FromUTF8(inet_ntoa(pDestinations->addr)));    // @todo fix this
            pEvent->SetInt(ntohs(pDestinations->rtpPort.num()));
            pEvent->SetExtraLong(ntohs(pDestinations->rtcpPort.num()));
        }
        else
        {
            pEvent->SetString("Unknown");
            pEvent->SetInt(0);
            pEvent->SetExtraLong(0);
        }
        wxQueueEvent(pHandler, pEvent);
    }
    --m_nConnections;
    OnDemandServerMediaSubsession::deleteStream(clientSessionId, streamToken);
}


