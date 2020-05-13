#include "rtpserverthread.h"
#include "PamTaskScheduler.h"
#include "PamUsageEnvironment.h"
#include <wx/log.h>
#include "settings.h"
#include "ondemandstreamer.h"


RtpServerThread::RtpServerThread(wxEvtHandler* pHandler, const wxString& sRTSP, unsigned int nRTSPPort, const wxString& sSourceIp, unsigned int nRTPPort, bool bSSM, LiveAudioSource::enumPacketTime ePacketTime) :
    m_pHandler(pHandler),
    m_sRTSP(sRTSP),
    m_nRTSPPort(nRTSPPort),
    m_sSourceIp(sSourceIp),
    m_nRTPPort(nRTPPort),
    m_bSSM(bSSM),
    m_ePacketTime(ePacketTime),
	m_penv(nullptr),
    m_eventLoopWatchVariable(0),
    m_pSource(nullptr),
    m_pSink(nullptr),
    m_pRtcpInstance(nullptr),
    m_pRtpGroupsock(nullptr),
    m_pRtspServer(nullptr),
    m_bStreaming(false)
{

}

void* RtpServerThread::Entry()
{
    m_mutex.Lock();
    SendingInterfaceAddr = our_inet_addr(std::string(m_sRTSP.mb_str()).c_str());
    TaskScheduler* scheduler = PamTaskScheduler::createNew();
    m_penv = PamUsageEnvironment::createNew(*scheduler, m_pHandler);

    if(!CreateStream())
    {
        m_mutex.Unlock();
        SendFinish();
        return NULL;
    }

    m_mutex.Unlock();

    while(m_eventLoopWatchVariable == 0)
    {
        m_penv->taskScheduler().doEventLoop(&m_eventLoopWatchVariable);
    }

    CloseStream();
    SendFinish();
    return NULL;
}


void RtpServerThread::SendFinish()
{
    wxCommandEvent* pEvent = new wxCommandEvent(wxEVT_ODS_FINISHED);
    wxQueueEvent(m_pHandler, pEvent);
}


bool RtpServerThread::CreateStream()
{
    char const* mimeType = "L24";
    unsigned char payloadFormatCode = 96; // by default, unless a static RTP payload type can be used

    // Create 'groupsocks' for RTP:
    struct in_addr destinationAddress;
    if(m_sSourceIp.empty() == false)
    {
        destinationAddress.s_addr = inet_addr(std::string(m_sSourceIp.mb_str()).c_str());
    }
    else if(m_bSSM)
    {
        destinationAddress.s_addr = chooseRandomIPv4SSMAddress(*m_penv);   //THIS IS THE MULTICAST ADDRESS
    }
    else
    {
        m_bStreaming = false;
        return false;
    }

    const unsigned char ttl = 255;

    const Port rtpPort(m_nRTPPort);

    m_pRtpGroupsock = new Groupsock(*m_penv, destinationAddress, rtpPort, ttl);
    if(m_bSSM)
    {
        m_pRtpGroupsock->multicastSendOnly(); // we're a SSM source
    }

    m_pSource = LiveAudioSource::createNew(m_pHandler, m_mutex, *m_penv, 2, m_ePacketTime);

    // Create an appropriate audio RTP sink (using "SimpleRTPSink") from the RTP 'groupsock':
    m_pSink = SimpleRTPSink::createNew(*m_penv, m_pRtpGroupsock, payloadFormatCode, m_pSource->samplingFrequency(), "audio", mimeType, m_pSource->numChannels());

    // @todo do we need to set this here as well as in the source??
    m_pSink->setPacketSizes(m_pSource->GetPreferredFrameSize()+12,m_pSource->GetPreferredFrameSize()+12);

    // Create and start a RTSP server to serve this stream:
    m_pRtspServer = RTSPServer::createNew(*m_penv, m_nRTSPPort);
    if (m_pRtspServer == NULL)
    {
        *m_penv << "Failed to create RTSP server: " << m_penv->getResultMsg() << "\n";
        Medium::close(m_pSink);
        Medium::close(m_pSource);

        m_pSource= nullptr;
        m_pSink = nullptr;
        m_bStreaming = false;
        delete m_pRtpGroupsock;
        m_pRtpGroupsock = nullptr;
        return false;
    }
    ServerMediaSession* sms = ServerMediaSession::createNew(*m_penv, "PAM_AES67", "PAM_AES67", "PAM_AES67", True/*SSM*/);
    AES67ServerMediaSubsession* pSmss =  AES67ServerMediaSubsession::createNew(*m_pSink, NULL, m_ePacketTime);
    sms->addSubsession(pSmss);
    m_pRtspServer->addServerMediaSession(sms);

    char* pSDP = sms->generateSDPDescription();
    m_sSDP = std::string(pSDP);
    delete[] pSDP;

    // Finally, start the streaming:
    *m_penv << "Beginning streaming...\n";
    *m_penv << m_pRtspServer->rtspURL(sms) << "\n";


    m_pSink->startPlaying(*m_pSource, afterPlaying, reinterpret_cast<void*>(this));


    m_bStreaming = true;
    Settings::Get().Write(wxT("AoIP"), wxT("Epoch"), pSmss->GetEpochTimestamp());

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

    m_eventLoopWatchVariable = 1;
}



void RtpServerThread::AddSamples(const timedbuffer* pTimedBuffer)
{
    if(m_bStreaming)
    {
        m_pSource->AddSamples(pTimedBuffer);
    }
}

void RtpServerThread::FlushQueue()
{
    if(m_bStreaming)
    {
        m_pSource->FlushQueue();
    }
}

const std::string& RtpServerThread::GetSDP()
{
    wxMutexLocker lock(m_mutex);
    return m_sSDP;
}
