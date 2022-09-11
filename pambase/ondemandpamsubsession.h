#pragma once
#include <wx/event.h>
#include "OnDemandServerMediaSubsession.hh"
#include "dlldefine.h"
#include <set>
class PamUsageEnvironment;




class PAMBASE_IMPEXPORT OnDemandPamSubsession: public OnDemandServerMediaSubsession
{
    public:

        virtual std::string GetSDP() { return m_sSDP;}
        virtual std::string GetStreamName()=0;

        RTPSink* GetSink() {return m_pSink;}


        void SetRTCPHandlers(std::set<wxEvtHandler*>& setHandlers) { m_setRTCPHandlers = setHandlers;}
        void SetRTSPHandlers(std::set<wxEvtHandler*>& setHandlers) { m_setRTSPHandlers = setHandlers;}



    protected: // we're a virtual base class
        OnDemandPamSubsession(wxEvtHandler* pAudioHandler, PamUsageEnvironment& env, portNumBits initialPortNum  = 6970);
        virtual ~OnDemandPamSubsession();

        char const* getAuxSDPLine(RTPSink* rtpSink, FramedSource* inputSource) override;

        void deleteStream(unsigned clientSessionId, void*& streamToken) override;

        void getStreamParameters(unsigned clientSessionId,
				   struct sockaddr_storage const& clientAddress,
                                   Port const& clientRTPPort,
                                   Port const& clientRTCPPort,
				   int tcpSocketNum,
                                   unsigned char rtpChannelId,
                                   unsigned char rtcpChannelId,
                                   TLSState* tlsState, // in (used if TCP)
                                   struct sockaddr_storage& destinationAddress,
				   u_int8_t& destinationTTL,
                                   Boolean& isMulticast,
                                   Port& serverRTPPort,
                                   Port& serverRTCPPort,
                                   void*& streamToken) override;




        wxEvtHandler* m_pAudioHandler;
        long m_nConnections;
        std::string m_sSDP;
        RTPSink* m_pSink;
        PamUsageEnvironment& m_env;



        std::set<wxEvtHandler*> m_setRTSPHandlers;
        std::set<wxEvtHandler*> m_setRTCPHandlers;

    private:
};



