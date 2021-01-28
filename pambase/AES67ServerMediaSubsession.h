#pragma once
#include "ServerMediaSession.hh"

class AES67ServerMediaSubsession: public ServerMediaSubsession
{
    public:
        static AES67ServerMediaSubsession* createNew(RTPSink& rtpSink, RTCPInstance* rtcpInstance, int nPacketTime);

        unsigned int GetEpochTimestamp();
    protected:
        AES67ServerMediaSubsession(RTPSink& rtpSink, RTCPInstance* rtcpInstance, int nPacketTime);
        // called only by createNew();
        virtual ~AES67ServerMediaSubsession();

        virtual Boolean rtcpIsMuxed();


      virtual char const* sdpLines();
        virtual void getStreamParameters(unsigned clientSessionId, netAddressBits clientAddress, Port const& clientRTPPort,Port const& clientRTCPPort, int tcpSocketNum, unsigned char rtpChannelId, unsigned char rtcpChannelId,netAddressBits& destinationAddress, u_int8_t& destinationTTL,Boolean& isMulticast, Port& serverRTPPort, Port& serverRTCPPort,void*& streamToken);
      virtual void startStream(unsigned clientSessionId, void* streamToken, TaskFunc* rtcpRRHandler, void* rtcpRRHandlerClientData, unsigned short& rtpSeqNum, unsigned& rtpTimestamp,    ServerRequestAlternativeByteHandler* serverRequestAlternativeByteHandler, void* serverRequestAlternativeByteHandlerClientData);

      virtual float getCurrentNPT(void* streamToken);
      virtual void getRTPSinkandRTCP(void* streamToken, RTPSink const*& rtpSink, RTCPInstance const*& rtcp);
      virtual void deleteStream(unsigned clientSessionId, void*& streamToken);

    protected:
        char* fSDPLines;

    private:
      RTPSink& fRTPSink;
      RTCPInstance* fRTCPInstance;
        int m_nPacketTime;

        //HashTable* fClientRTCPSourceRecords; // indexed by client session id; used to implement RTCP "RR" handling

};


