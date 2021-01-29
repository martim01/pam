#include "ondemandpamsubsession.h"
#include "PamUsageEnvironment.h"
#include "ondemandstreamer.h"
#include <iostream>
#include "log.h"


OnDemandPamSubsession::OnDemandPamSubsession(wxEvtHandler* pHandler, PamUsageEnvironment& env,  portNumBits initialPortNum) :
    OnDemandServerMediaSubsession(env, True, initialPortNum, False),
    m_pHandler(pHandler),
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
    std::cout << rtpSink->auxSDPLine() << std::endl;
    return rtpSink->auxSDPLine();
  }
  std::cout << "NO RTPSink!!!!!" << std::endl;
  return NULL;
}

void OnDemandPamSubsession::getStreamParameters(unsigned clientSessionId, netAddressBits clientAddress, Port const& clientRTPPort, Port const& clientRTCPPort, int tcpSocketNum, unsigned char rtpChannelId, unsigned char rtcpChannelId, netAddressBits& destinationAddress, u_int8_t& destinationTTL, Boolean& isMulticast, Port& serverRTPPort, Port& serverRTCPPort, void*& streamToken)
{
    OnDemandServerMediaSubsession::getStreamParameters(clientSessionId, clientAddress, clientRTPPort, clientRTCPPort, tcpSocketNum, rtpChannelId, rtcpChannelId,destinationAddress, destinationTTL, isMulticast, serverRTPPort, serverRTCPPort, streamToken);

    m_nConnections++;

    if(m_pHandler)
    {
        wxCommandEvent* pEvent = new wxCommandEvent(wxEVT_ODS_CONNECTION);
        pEvent->SetInt(clientSessionId);
        Destinations* pDestinations = (Destinations*)(fDestinationsHashTable->Lookup((char const*)clientSessionId));
        if(pDestinations)
        {
            pEvent->SetString(wxString::FromUTF8(inet_ntoa(pDestinations->addr)));
            pEvent->SetExtraLong(ntohs(pDestinations->rtpPort.num()));
        }
        else
        {
            pEvent->SetString("Unknown");
            pEvent->SetExtraLong(0);
        }
        wxQueueEvent(m_pHandler, pEvent);
    }
}


void OnDemandPamSubsession::deleteStream(unsigned clientSessionId, void*& streamToken)
{
    if(m_pHandler)
    {
        wxCommandEvent* pEvent = new wxCommandEvent(wxEVT_ODS_DISCONNECTION);
        pEvent->SetInt(clientSessionId);
        Destinations* pDestinations = (Destinations*)(fDestinationsHashTable->Lookup((char const*)clientSessionId));
        if(pDestinations)
        {
            pEvent->SetString(wxString::FromUTF8(inet_ntoa(pDestinations->addr)));
            pEvent->SetExtraLong(pDestinations->rtpPort.num());
        }
        else
        {
            pEvent->SetString("Unknown");
            pEvent->SetExtraLong(0);
        }
        wxQueueEvent(m_pHandler, pEvent);
    }
    --m_nConnections;
    OnDemandServerMediaSubsession::deleteStream(clientSessionId, streamToken);
}


