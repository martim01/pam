#include "rtpserverthread.h"
#include "PamTaskScheduler.h"
#include "PamUsageEnvironment.h"
#include <wx/log.h>


RtpServerThread::RtpServerThread(wxEvtHandler* pHandler, unsigned int nRTSPPort, const wxString& sMulticast, unsigned int nRTPPort, LiveAudioSource::enumPacketTime ePacketTime) :
    m_pHandler(pHandler),
    m_nRTSPPort(nRTSPPort),
    m_sMulticast(sMulticast),
    m_nRTPPort(nRTPPort),
    m_ePacketTime(ePacketTime),
    m_eventLoopWatchVariable(0),
    m_pSource(0),
    m_pSink(0),
    m_pRtcpInstance(0),
    m_pRtpGroupsock(0),
    m_pRtspServer(0),
    m_bStreaming(false)
{

}

void* RtpServerThread::Entry()
{
    wxLogDebug(wxT("RtpServerThread::Entry() %d"), m_ePacketTime);
    TaskScheduler* scheduler = PamTaskScheduler::createNew();
    m_penv = PamUsageEnvironment::createNew(*scheduler, m_pHandler);
    CreateStream();

    while(m_eventLoopWatchVariable == 0)
    {
        m_penv->taskScheduler().doEventLoop(&m_eventLoopWatchVariable);
    }
    CloseStream();
    wxLogDebug(wxT("RtpServerThread::Entry() Finished"));
    return NULL;
}



bool RtpServerThread::CreateStream()
{
    wxLogDebug(wxT("RtpServerThread::CreateStream()"));

    wxMutexLocker lock(m_mutex);
    // Open the file as a 'WAV' file:
    m_pSource = LiveAudioSource::createNew(m_pHandler, m_mutex, *m_penv, 2, m_ePacketTime);

    char const* mimeType = "L24";
    unsigned char payloadFormatCode = 96; // by default, unless a static RTP payload type can be used

    // Create 'groupsocks' for RTP and RTCP:
    struct in_addr destinationAddress;
    destinationAddress.s_addr = chooseRandomIPv4SSMAddress(*m_penv);   //THIS IS THE MULTICAST ADDRESS

    const unsigned char ttl = 255;

    const Port rtpPort(m_nRTPPort);

    m_pRtpGroupsock = new Groupsock(*m_penv, destinationAddress, rtpPort, ttl);
    m_pRtpGroupsock->multicastSendOnly(); // we're a SSM source

    // Create an appropriate audio RTP sink (using "SimpleRTPSink") from the RTP 'groupsock':
    m_pSink = SimpleRTPSink::createNew(*m_penv, m_pRtpGroupsock, payloadFormatCode, m_pSource->samplingFrequency(), "audio", mimeType, m_pSource->numChannels());

    // @todo do we need to set this here as well as in the source??
    m_pSink->setPacketSizes(m_pSource->GetPreferredFrameSize()+12,m_pSource->GetPreferredFrameSize()+12);   //THIS IS FOR 72 byte package

    // Create and start a RTSP server to serve this stream:
    m_pRtspServer = RTSPServer::createNew(*m_penv, m_nRTSPPort);
    if (m_pRtspServer == NULL)
    {
        *m_penv << "Failed to create RTSP server: " << m_penv->getResultMsg() << "\n";
        return true;
    }
    ServerMediaSession* sms = ServerMediaSession::createNew(*m_penv, "liveStream", "AES67", "", True/*SSM*/);
    sms->addSubsession(AES67ServerMediaSubsession::createNew(*m_pSink, NULL));
    m_pRtspServer->addServerMediaSession(sms);

    // Finally, start the streaming:
    *m_penv << "Beginning streaming...\n";
    *m_penv << m_pRtspServer->rtspURL(sms) << "\n";

    m_pSink->startPlaying(*m_pSource, afterPlaying, reinterpret_cast<void*>(this));

    wxLogDebug(wxT("RtpServerThread::CreateStream() Done"));
    m_bStreaming = true;

    return true;
}


void afterPlaying(void* pClientData)
{
    RtpServerThread* pThread = reinterpret_cast<RtpServerThread*>(pClientData);
    pThread->StopStream();
}

void RtpServerThread::CloseStream()
{
    wxMutexLocker lock(m_mutex);
    *m_penv << "...done streaming\n";

    // End by closing the media:
    Medium::close(m_pRtspServer);
    Medium::close(m_pSink);
    delete m_pRtpGroupsock;
    Medium::close(m_pSource);

    m_pSource = 0;
    m_pSink = 0;
    m_pRtpGroupsock = 0;
    m_pRtspServer = 0;

}

void RtpServerThread::StopStream()
{
    wxLogDebug(wxT("RTPServerThread::StopStream"));
    m_eventLoopWatchVariable = 1;
}



void RtpServerThread::AddSamples(const timedbuffer* pTimedBuffer)
{
    wxMutexLocker lock(m_mutex);
    if(m_pSource && m_bStreaming)
    {
        m_pSource->AddSamples(pTimedBuffer);
    }
}

void RtpServerThread::FlushQueue()
{
    wxMutexLocker lock(m_mutex);
    if(m_pSource && m_bStreaming)
    {
        m_pSource->FlushQueue();
    }
}
