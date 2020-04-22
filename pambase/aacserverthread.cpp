#include "mp3serverthread.h"
#include "PamTaskScheduler.h"
#include "PamUsageEnvironment.h"
#include <wx/log.h>
#include "settings.h"

MP3ServerThread::MP3ServerThread(wxEvtHandler* pHandler, const wxString& sRTSP, unsigned int nRTSPPort, const wxString& sSourceIp, unsigned int nRTPPort, bool bSSM) :
    m_pHandler(pHandler),
    m_sRTSP(sRTSP),
    m_nRTSPPort(nRTSPPort),
    m_sSourceIp(sSourceIp),
    m_nRTPPort(nRTPPort),
    m_bSSM(bSSM),
    m_ePacketTime(ePacketTime),
	m_penv(0),
    m_eventLoopWatchVariable(0),
    m_pSource(0),
    m_pSink(0),
    m_pRtcpInstance(0),
    m_pRtpGroupsock(0),
    m_pRtspServer(0),
    m_bStreaming(false)
{

}

void* MP3ServerThread::Entry()
{
    SendingInterfaceAddr = our_inet_addr(std::string(m_sRTSP.mb_str()).c_str());

    TaskScheduler* scheduler = PamTaskScheduler::createNew();
    m_penv = PamUsageEnvironment::createNew(*scheduler, m_pHandler);
    CreateStream();

    while(m_eventLoopWatchVariable == 0)
    {
        m_penv->taskScheduler().doEventLoop(&m_eventLoopWatchVariable);
    }
    CloseStream();
    return NULL;
}



bool MP3ServerThread::CreateStream()
{

    wxMutexLocker lock(m_mutex);
    // Open the file as a 'WAV' file:
    m_pSource = MP3LiveSource::createNew(m_pHandler, m_mutex, *m_penv);

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
        return false;
    }

    const unsigned char ttl = 255;

    const Port rtpPort(m_nRTPPort);

    m_pRtpGroupsock = new Groupsock(*m_penv, destinationAddress, rtpPort, ttl);
    if(m_bSSM)
    {
        m_pRtpGroupsock->multicastSendOnly(); // we're a SSM source
    }

    // Create an appropriate audio RTP sink (using "SimpleRTPSink") from the RTP 'groupsock':
    m_pSink = MPEG1or2AudioRTPSink::createNew(*m_penv, m_pRtpGroupsock);


    // Create and start a RTSP server to serve this stream:
    m_pRtspServer = RTSPServer::createNew(*m_penv, m_nRTSPPort);
    if (m_pRtspServer == NULL)
    {
        *m_penv << "Failed to create RTSP server: " << m_penv->getResultMsg() << "\n";
        return true;
    }
    ServerMediaSession* sms = ServerMediaSession::createNew(*m_penv, "PAM", "MP3", "");
    PassiveServerMediaSubsession* pSmss =  PassiveServerMediaSubsession::createNew(*m_pSink, NULL);
    sms->addSubsession(pSmss);
    m_pRtspServer->addServerMediaSession(sms);

    // Finally, start the streaming:
    *m_penv << "Beginning streaming...\n";
    *m_penv << m_pRtspServer->rtspURL(sms) << "\n";


    m_pSink->startPlaying(*m_pSource, afterPlaying, reinterpret_cast<void*>(this));

    wxLogDebug(wxT("Mp3Server::CreateStream() Done"));
    m_bStreaming = true;


    return true;
}


void afterPlaying(void* pClientData)
{
    MP3ServerThread* pThread = reinterpret_cast<MP3ServerThread*>(pClientData);
    pThread->StopStream();
}

void MP3ServerThread::CloseStream()
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

void MP3ServerThread::StopStream()
{
    wxLogDebug(wxT("MP3ServerThread::StopStream"));
    m_eventLoopWatchVariable = 1;
}



void MP3ServerThread::AddSamples(const timedbuffer* pTimedBuffer)
{
    wxMutexLocker lock(m_mutex);
    if(m_pSource && m_bStreaming)
    {
        m_pSource->AddSamples(pTimedBuffer);
    }
}

void MP3ServerThread::FlushQueue()
{
    wxMutexLocker lock(m_mutex);
    if(m_pSource && m_bStreaming)
    {
        m_pSource->FlushQueue();
    }
}
