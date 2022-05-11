#pragma once
#include <wx/event.h>
#include "ondemandpamsubsession.h"
#include "AudioRTPSink.hh"
#include "liveaudiosource.h"
#include <wx/thread.h>
#include <set>
#include <vector>
#include <string>
class timedbuffer;


class AES67RTPSink : public AudioRTPSink
{

    public:
      static AES67RTPSink* createNew(UsageEnvironment& env, Groupsock* RTPgs, unsigned int nFrameSize, unsigned char nRtpPayload,
unsigned char nChannels, const std::string& sMapping, const std::string& sFormat, unsigned short nSampleRate);

    char const* auxSDPLine() override;
    protected:
      AES67RTPSink(UsageEnvironment& env, Groupsock* RTPgs, unsigned int nFrameSize, unsigned char nRtpPayload,
unsigned char nChannels, const std::string& sMapping, const std::string& sFormat, unsigned short nSampleRate);
        // called only by createNew()



      virtual ~AES67RTPSink();

      unsigned int GetEpochTimestamp();

    private: // redefined virtual functions:
        std::string m_sAux;

        std::string m_sMapping;
};


class OnDemandAES67MediaSubsession: public OnDemandPamSubsession
{
    public:
        static OnDemandAES67MediaSubsession* createNew(wxEvtHandler* pHandler, PamUsageEnvironment& env, unsigned char nNumChannels, const std::string& sMapping,
                                                       unsigned char nRtpPayload,
        LiveAudioSource::enumPacketTime ePacketTime, unsigned char nBitsPerSample, unsigned short nSampleRate, portNumBits initialPortNum = 5004);

        void AddSamples(const timedbuffer* pTimedBuffer);
        void FlushQueue();

        void SetRouting(const std::vector<char>& vRouting);

        std::string GetStreamName() override
        {
            return "AES67";
        }

        void DoQoS();
        void BeginQOSMeasurement();

//        std::string GetSDP() override;


    protected: // we're a virtual base class
        OnDemandAES67MediaSubsession(wxEvtHandler* pHandler, PamUsageEnvironment& env, unsigned char nNumChannels, const std::string& sMapping,
unsigned char nRtpPayload,  unsigned char nBitsPerSample, unsigned short nSampleRate, LiveAudioSource::enumPacketTime ePacketTime, portNumBits initialPortNum  = 5004);
        virtual ~OnDemandAES67MediaSubsession();

        // new virtual functions, defined by all subclasses
        FramedSource* createNewStreamSource(unsigned clientSessionId, unsigned& estBitrate) override;
        // "estBitrate" is the stream's estimated bitrate, in kbps
        RTPSink* createNewRTPSink(Groupsock* rtpGroupsock, unsigned char rtpPayloadTypeIfDynamic, FramedSource* inputSource) override;


//        char const* sdpLines() override;

//        unsigned int GetEpochTimestamp(RTPSink* pSink);
        RTCPInstance* createRTCP(Groupsock* RTCPgs, unsigned totSessionBW, unsigned char const* cname, RTPSink* sink);

    private:
        wxMutex m_mutex;
        unsigned char m_nNumberOfChannels;
        std::string m_sMapping;
        unsigned char m_nRtpPayload;
        LiveAudioSource::enumPacketTime m_ePacketTime;
        unsigned char m_nBitsPerSample;
        unsigned short m_nSampleRate;

        LiveAudioSource* m_pSource;


        RTCPInstance* m_pRTCP;

    bool m_bQos;

};

