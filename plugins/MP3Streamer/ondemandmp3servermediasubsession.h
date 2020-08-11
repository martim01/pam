#pragma once
#include <wx/event.h>
#include "mp3encoder.h"
#include "ondemandpamsubsession.h"
class PamUsageEnvironment;

class OnDemandMP3MediaSubsession: public OnDemandPamSubsession
{
    public:
        static OnDemandMP3MediaSubsession* createNew(wxEvtHandler* pHandler, PamUsageEnvironment& env, MP3Encoder& encoder,  portNumBits initialPortNum = 6970);

        std::string GetStreamName() override;

    protected: // we're a virtual base class
        OnDemandMP3MediaSubsession(wxEvtHandler* pHandler, PamUsageEnvironment& env, MP3Encoder& encoder,  portNumBits initialPortNum  = 6970);
        virtual ~OnDemandMP3MediaSubsession();

        // new virtual functions, defined by all subclasses
        FramedSource* createNewStreamSource(unsigned clientSessionId, unsigned& estBitrate) override;
        // "estBitrate" is the stream's estimated bitrate, in kbps
        RTPSink* createNewRTPSink(Groupsock* rtpGroupsock, unsigned char rtpPayloadTypeIfDynamic, FramedSource* inputSource) override;

    private:
        MP3Encoder& m_Encoder;
};


