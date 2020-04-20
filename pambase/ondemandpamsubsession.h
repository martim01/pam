#pragma once
#include <wx/event.h>
#include "OnDemandServerMediaSubsession.hh"
#include "dlldefine.h"

class PamUsageEnvironment;

class PAMBASE_IMPEXPORT OnDemandPamSubsession: public OnDemandServerMediaSubsession
{
    public:
    protected: // we're a virtual base class
        OnDemandPamSubsession(wxEvtHandler* pHandler, PamUsageEnvironment& env, portNumBits initialPortNum  = 6970);
        virtual ~OnDemandPamSubsession();

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

    protected:
        wxEvtHandler* m_pHandler;
};



