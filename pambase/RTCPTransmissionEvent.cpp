#include "RTCPTransmissionEvent.h"

#ifdef WXSPAM
DEFINE_EVENT_TYPE(wxEVT_RTCP_TRANSMISSION)
#else
wxDEFINE_EVENT(wxEVT_RTCP_TRANSMISSION, RTCPTransmissionEvent);
#endif // WXSPAM


RTCPTransmissionEvent::RTCPTransmissionEvent(const RTCPTransmissionEvent& event) : wxCommandEvent(event),
    m_nSR_RRTime(event.GetSRRRTime()),
    m_nFirstPacketNumber(event.GetFirstPacketNumber()),
    m_nOctets(event.GetTotalOctets()),
    m_nPackets(event.GetTotalPackets()),
    m_nJitter(event.GetJitter()),
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
