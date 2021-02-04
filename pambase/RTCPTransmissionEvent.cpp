#include "RTCPTransmissionEvent.h"
#include "RTPSink.hh"

#ifdef WXSPAM
DEFINE_EVENT_TYPE(wxEVT_RTCP_TRANSMISSION)
#else
wxDEFINE_EVENT(wxEVT_RTCP_TRANSMISSION, RTCPTransmissionEvent);
#endif // WXSPAM




RTCPTransmissionEvent::RTCPTransmissionEvent(RTPTransmissionStats* pStats, int nFrequency) : wxCommandEvent(wxEVT_RTCP_TRANSMISSION, wxID_ANY)
{

    m_nSR_RRTime = pStats->diffSR_RRTime();
    m_nFirstPacketNumber = pStats->firstPacketNumReported();

    u_int32_t nOctHigh,  nOctLow,  nPacketHigh, nPacketLow;
    pStats->getTotalOctetCount(nOctHigh, nOctLow);
    pStats->getTotalPacketCount(nPacketHigh, nPacketLow);
    m_nOctets = nOctHigh;
    m_nOctets = m_nOctets << 32;
    m_nOctets += nOctLow;
    m_nPackets = nPacketHigh;
    m_nPackets = m_nPackets << 32;
    m_nPackets += nPacketLow;

    m_dJitter = static_cast<double>(pStats->jitter())/static_cast<double>(nFrequency);
    m_dJitter *= 1000.0; //into ms;
    char addr[256];
    if(pStats->lastFromAddress().sin_family == AF_INET)
    {
        inet_ntop(AF_INET, &pStats->lastFromAddress().sin_addr, &addr[0], 256);
    }
    else if(pStats->lastFromAddress().sin_family == AF_INET6)
    {
        inet_ntop(AF_INET6, &pStats->lastFromAddress().sin_addr, &addr[0], 256);
    }
    m_sLastFromAddress = wxString(std::string(addr));

    m_nLastPacketNumber = pStats->lastPacketNumReceived();
    m_nLastSRTime = pStats->lastSRTime();
    m_dtLastReceived = wxDateTime(time_t(pStats->lastTimeReceived().tv_sec));
    m_dtLastReceived.SetMillisecond(pStats->lastTimeReceived().tv_usec/1000);
    m_nPacketLossRatio = pStats->packetLossRatio();
    m_nPacketsLostBetweenRR = pStats->packetsLostBetweenRR();
    m_nPacketsReceivedSinceLastRR = pStats->packetsReceivedSinceLastRR();
    m_nRoundTripDelay = (static_cast<double>(pStats->roundTripDelay())/65536.0)*1000000;
    m_nSSRC = pStats->SSRC();
    m_dtCreated = wxDateTime(time_t(pStats->timeCreated().tv_sec));
    m_dtCreated.SetMillisecond(pStats->timeCreated().tv_usec/1000);
    m_nTotNumPacketsLost = pStats->totNumPacketsLost();

}


RTCPTransmissionEvent::RTCPTransmissionEvent(const RTCPTransmissionEvent& event) : wxCommandEvent(event),
    m_nSR_RRTime(event.GetSRRRTime()),
    m_nFirstPacketNumber(event.GetFirstPacketNumber()),
    m_nOctets(event.GetTotalOctets()),
    m_nPackets(event.GetTotalPackets()),
    m_dJitter(event.GetJitter()),
    m_nLastPacketNumber(event.GetLastPacketNumber()),
    m_nLastSRTime(event.GetLastSRTime()),
    m_dtLastReceived(event.GetLastReceivedTime()),
    m_nPacketLossRatio(event.GetPacketLossRatio()),
    m_nPacketsLostBetweenRR(event.GetPacketsLostBetweenRR()),
    m_nPacketsReceivedSinceLastRR(event.GetPacketsSinceLastRR()),
    m_nRoundTripDelay(event.GetRoundTripDelay()),
    m_nSSRC(event.GetSSRC()),
    m_dtCreated(event.GetFirstReceivedTime()),
    m_nTotNumPacketsLost(event.GetTotalPacketsLost()),
    m_sLastFromAddress(event.GetFromAddress())
{

}
