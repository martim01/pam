#include "ondemandmp3servermediasubsession.h"
#include "mp3livesource.h"
#include "MPEG1or2AudioRTPSink.hh"
#include "PamUsageEnvironment.h"
#include "ondemandstreamer.h"

OnDemandMP3MediaSubsession* OnDemandMP3MediaSubsession::createNew(wxEvtHandler* pHandler, PamUsageEnvironment& env, MP3Encoder& encoder,  portNumBits initialPortNum)
{
    return new OnDemandMP3MediaSubsession(pHandler, env, encoder, initialPortNum);
}

OnDemandMP3MediaSubsession::OnDemandMP3MediaSubsession(wxEvtHandler* pHandler, PamUsageEnvironment& env, MP3Encoder& encoder,  portNumBits initialPortNum) :
    OnDemandPamSubsession(pHandler, env, initialPortNum),
    m_Encoder(encoder)
    {

    }
OnDemandMP3MediaSubsession::~OnDemandMP3MediaSubsession()
{

}

FramedSource* OnDemandMP3MediaSubsession::createNewStreamSource(unsigned clientSessionId, unsigned& estBitrate)
{
    estBitrate = 320;
    MP3LiveSource* pSource = MP3LiveSource::createNew(m_Encoder, envir());



    return pSource;

}

RTPSink* OnDemandMP3MediaSubsession::createNewRTPSink(Groupsock* rtpGroupsock, unsigned char rtpPayloadTypeIfDynamic, FramedSource* inputSource)
{

    return MPEG1or2AudioRTPSink::createNew(envir(), rtpGroupsock);
}

