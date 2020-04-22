#include "ondemandaes67mediasubsession.h"
#include "liveaudiosource.h"

#include "wxsink.h"

OnDemandAES67MediaSubsession* OnDemandAES67MediaSubsession::createNew(wxEvtHandler* pHandler, PamUsageEnvironment& env, unsigned char nNumChannels,LiveAudioSource::enumPacketTime ePacketTime, portNumBits initialPortNum)
{
    return new OnDemandAES67MediaSubsession(pHandler, env, nNumChannels, ePacketTime, initialPortNum);
}

OnDemandAES67MediaSubsession::OnDemandAES67MediaSubsession(wxEvtHandler* pHandler, PamUsageEnvironment& env, unsigned char nNumChannels, LiveAudioSource::enumPacketTime ePacketTime, portNumBits initialPortNum ) :
    OnDemandPamSubsession(pHandler, env, initialPortNum),
  m_nNumberOfChannels(nNumChannels),
  m_ePacketTime(ePacketTime),
  m_pSource(nullptr)

{

}
OnDemandAES67MediaSubsession::~OnDemandAES67MediaSubsession()
{

}


FramedSource* OnDemandAES67MediaSubsession::createNewStreamSource(unsigned clientSessionId, unsigned& estBitrate)
{
    m_pSource = LiveAudioSource::createNew(m_pHandler, m_mutex, envir(), m_nNumberOfChannels, m_ePacketTime);
}


RTPSink* OnDemandAES67MediaSubsession::createNewRTPSink(Groupsock* rtpGroupsock, unsigned char rtpPayloadTypeIfDynamic, FramedSource* inputSource)
{
    return SimpleRTPSink::createNew(envir(), rtpGroupsock, 96, 48000, "audio", "L24", m_nNumberOfChannels);
}

void OnDemandAES67MediaSubsession::AddSamples(const timedbuffer* pTimedBuffer)
{
    wxMutexLocker lock(m_mutex);
    if(m_pSource)
    {
        m_pSource->AddSamples(pTimedBuffer);
    }
}

void OnDemandAES67MediaSubsession::FlushQueue()
{
    wxMutexLocker lock(m_mutex);
    if(m_pSource)
    {
        m_pSource->FlushQueue();
    }
}
