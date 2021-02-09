#pragma once

#include <wx/event.h>
#include "dlldefine.h"
#include <wx/string.h>
#include <wx/datetime.h>

class RTCPTransmissionEvent;

#ifdef WXSPAM
DECLARE_EXPORTED_EVENT_TYPE(WXEXPORT, wxEVT_RTCP_TRANSMISSION, -1)
#else
wxDECLARE_EXPORTED_EVENT(PAMBASE_IMPEXPORT, wxEVT_RTCP_TRANSMISSION,RTCPTransmissionEvent);
#endif
class RTPTransmissionStats;


class PAMBASE_IMPEXPORT RTCPTransmissionEvent : public wxCommandEvent
{


public:

    RTCPTransmissionEvent(RTPTransmissionStats* pStats, int nFrequency);
    /** @brief	Copy Constructor
    *   @param event a wxNIEvent
    **/
    RTCPTransmissionEvent(const RTCPTransmissionEvent& event);

    /** @brief	Destructor
    **/
    virtual ~RTCPTransmissionEvent(){}

    /** @brief	Creates a copy of the wxNIEvent
    *   @return <i>wxNIEvent</i>
    **/
    virtual wxEvent *Clone() const { return new RTCPTransmissionEvent(*this); }

    //wxDECLARE_DYNAMIC_CLASS(SettingEvent)

    unsigned int GetSRRRTime() const { return m_nSR_RRTime; }
    unsigned int GetFirstPacketNumber() const  { return m_nFirstPacketNumber; }
    int64_t GetTotalOctets() const  { return m_nOctets; }
    int64_t GetTotalPackets() const  { return m_nPackets; }
    double GetJitter() const  { return m_dJitter; }
    unsigned int GetLastPacketNumber() const  { return m_nLastPacketNumber; }
    unsigned int GetLastSRTime() const  { return m_nLastSRTime; }
    const wxDateTime& GetLastReceivedTime() const  { return m_dtLastReceived; }
    u_int8_t GetPacketLossRatio() const  { return m_nPacketLossRatio; }
    unsigned int GetPacketsLostBetweenRR() const  { return m_nPacketsLostBetweenRR; }
    unsigned int GetPacketsSinceLastRR() const  { return m_nPacketsReceivedSinceLastRR; }
    unsigned int GetRoundTripDelay() const  { return m_nRoundTripDelay; }
    unsigned int GetSSRC() const  { return m_nSSRC; }
    const wxDateTime& GetFirstReceivedTime() const  { return m_dtCreated; }
    unsigned int GetTotalPacketsLost() const  { return m_nTotNumPacketsLost; }
    const wxString& GetFromAddress() const  { return m_sLastFromAddress; }
    unsigned int GetRTCPPort() const {return  m_nRTCPPort;   }

protected:

    unsigned int m_nSR_RRTime;
    unsigned int m_nFirstPacketNumber;
    int64_t m_nOctets;
    int64_t m_nPackets;
    double m_dJitter;
    unsigned int m_nLastPacketNumber;
    unsigned int m_nLastSRTime;
    wxDateTime m_dtLastReceived;
    u_int8_t m_nPacketLossRatio;
    unsigned int m_nPacketsLostBetweenRR;
    unsigned int m_nPacketsReceivedSinceLastRR;
    unsigned int m_nRoundTripDelay;
    unsigned int m_nSSRC;
    wxDateTime m_dtCreated;
    unsigned int m_nTotNumPacketsLost;
    wxString m_sLastFromAddress;
    unsigned int m_nRTCPPort;
};

typedef void (wxEvtHandler::*RTCPTransmissionEventFunction)(RTCPTransmissionEvent&);



