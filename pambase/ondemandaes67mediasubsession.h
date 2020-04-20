#pragma once
#include <wx/event.h>
#include "OnDemandPamSubsession.h"
#include "liveaudiosource.h"
#include <wx/thread.h>

class OnDemandAES67MediaSubsession: public OnDemandPamSubsession
{
    public:
        static OnDemandAES67MediaSubsession* createNew(wxEvtHandler* pHandler, PamUsageEnvironment& env, unsigned char nNumChannels,LiveAudioSource::enumPacketTime ePacketTime, portNumBits initialPortNum = 6970);
    protected: // we're a virtual base class
        OnDemandAES67MediaSubsession(wxEvtHandler* pHandler, PamUsageEnvironment& env, unsigned char nNumChannels,LiveAudioSource::enumPacketTime ePacketTime, portNumBits initialPortNum  = 6970);
        virtual ~OnDemandAES67MediaSubsession();

        // new virtual functions, defined by all subclasses
        FramedSource* createNewStreamSource(unsigned clientSessionId, unsigned& estBitrate) override;
        // "estBitrate" is the stream's estimated bitrate, in kbps
        RTPSink* createNewRTPSink(Groupsock* rtpGroupsock, unsigned char rtpPayloadTypeIfDynamic, FramedSource* inputSource) override;

    private:
        wxMutex m_mutex;
        unsigned char m_nNumberOfChannels;
        LiveAudioSource::enumPacketTime m_ePacketTime;
};

