#pragma once
#include "ServerMediaSession.hh"
#include <set>
#include <vector>
class wxEvtHandler;

class AES67ServerMediaSubsession: public ServerMediaSubsession
{
    public:
        static AES67ServerMediaSubsession* createNew(const std::set<wxEvtHandler*>& setRTCPHandlers, RTPSink& rtpSink, RTCPInstance* rtcpInstance, int nPacketTime);

        unsigned int GetEpochTimestamp();

        void DoQoS();
        void BeginQOSMeasurement();

    protected:
        AES67ServerMediaSubsession(const std::set<wxEvtHandler*>& setRTCPHandlers, RTPSink& rtpSink, RTCPInstance* rtcpInstance, int nPacketTime);
        // called only by createNew();
        virtual ~AES67ServerMediaSubsession();

        virtual Boolean rtcpIsMuxed();


      virtual char const* sdpLines(int addressFamily);
        virtual void getStreamParameters(unsigned clientSessionId, // in
				   struct sockaddr_storage const& clientAddress, // in
				   Port const& clientRTPPort, // in
				   Port const& clientRTCPPort, // in
				   int tcpSocketNum, // in (-1 means use UDP, not TCP)
				   unsigned char rtpChannelId, // in (used if TCP)
				   unsigned char rtcpChannelId, // in (used if TCP)
				   struct sockaddr_storage& destinationAddress, // in out
				   u_int8_t& destinationTTL, // in out
				   Boolean& isMulticast, // out
				   Port& serverRTPPort, // out
				   Port& serverRTCPPort, // out
				   void*& streamToken);
      virtual void startStream(unsigned clientSessionId, void* streamToken, TaskFunc* rtcpRRHandler, void* rtcpRRHandlerClientData, unsigned short& rtpSeqNum, unsigned& rtpTimestamp,    ServerRequestAlternativeByteHandler* serverRequestAlternativeByteHandler, void* serverRequestAlternativeByteHandlerClientData);

      virtual float getCurrentNPT(void* streamToken);
      virtual void getRTPSinkandRTCP(void* streamToken, RTPSink const*& rtpSink, RTCPInstance const*& rtcp);
      virtual void deleteStream(unsigned clientSessionId, void*& streamToken);


    protected:
        char* fSDPLines;

    private:
      RTPSink& fRTPSink;
      RTCPInstance* fRTCPInstance;
      std::set<wxEvtHandler*> m_setRTCPHandlers;
        int m_nPacketTime;

        //HashTable* fClientRTCPSourceRecords; // indexed by client session id; used to implement RTCP "RR" handling

};


