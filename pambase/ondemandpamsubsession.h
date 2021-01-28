#pragma once
#include <wx/event.h>
#include "OnDemandServerMediaSubsession.hh"
#include "dlldefine.h"

class PamUsageEnvironment;


struct rrqos
{
    unsigned int nSR_RRTime;
    unsigned int nFirstPacketNumber;
    int64_t nOctets;
    int64_t nPackets;
    unsigned int nJitter;
    unsigned int nLastPacketNumber;
    unsigned int nLastSRTime;
    timeval tvLastTimeReceived;
    timeval tvLastButOneTimeReceived;
    u_int8_t nPacketLossRatio;
    unsigned int nPacketsLostBetweenRR;
    unsigned int nPacketsReceivedSinceLastRR;
    unsigned int nRoundTripDelay;
    unsigned int nSSRC;
    timeval tvCreated;
    unsigned int nTotNumPacketsLost;
    std::string sLastFromAddress;

};


class PAMBASE_IMPEXPORT OnDemandPamSubsession: public OnDemandServerMediaSubsession
{
    public:

        virtual std::string GetSDP() { return m_sSDP;}
        virtual std::string GetStreamName()=0;

        RTPSink* GetSink() {return m_pSink;}




    protected: // we're a virtual base class
        OnDemandPamSubsession(wxEvtHandler* pHandler, PamUsageEnvironment& env, portNumBits initialPortNum  = 6970);
        virtual ~OnDemandPamSubsession();

        char const* getAuxSDPLine(RTPSink* rtpSink, FramedSource* inputSource) override;

        void deleteStream(unsigned clientSessionId, void*& streamToken) override;

        void getStreamParameters(unsigned clientSessionId,
				   netAddressBits clientAddress,
                                   Port const& clientRTPPort,
                                   Port const& clientRTCPPort,
				   int tcpSocketNum,
                                   unsigned char rtpChannelId,
                                   unsigned char rtcpChannelId,
                                   netAddressBits& destinationAddress,
				   u_int8_t& destinationTTL,
                                   Boolean& isMulticast,
                                   Port& serverRTPPort,
                                   Port& serverRTCPPort,
                                   void*& streamToken) override;







        wxEvtHandler* m_pHandler;
        long m_nConnections;
        std::string m_sSDP;
        RTPSink* m_pSink;
        PamUsageEnvironment& m_env;

        rrqos m_qos;
        unsigned int m_nQOSMeasurementUSecs;
        unsigned int m_nQOSIntervalUSecs;

    private:
};



