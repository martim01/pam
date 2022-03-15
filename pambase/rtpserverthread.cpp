#include "rtpserverthread.h"
#include "PamTaskScheduler.h"
#include "PamUsageEnvironment.h"
#include <wx/log.h>
#include "settings.h"
#include "ondemandstreamer.h"
#include "log.h"
#include "iomanager.h"


RtpServerThread::RtpServerThread(wxEvtHandler* pHandler, const std::set<wxEvtHandler*>& setRTCPHandlers, const wxString& sRTSP, unsigned int nRTSPPort, const wxString& sSourceIp, unsigned int nRTPPort, bool bSSM, LiveAudioSource::enumPacketTime ePacketTime) :
    wxThread(wxTHREAD_JOINABLE),
    m_pHandler(pHandler),
    m_setRTCPHandlers(setRTCPHandlers),
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
    m_bRTCP = (Settings::Get().Read("Server", "RTCP", 0) != 0);
}

void* RtpServerThread::Entry()
{
    m_mutex.Lock();

    //NetAddressList sendingddresses(m_sRTSP.ToStdString().c_str());
    //copyAddress(SendingInterfaceAddr, sendingddresses.firstAddress());


    SendingInterfaceAddr = inet_addr(std::string(m_sRTSP.mb_str()).c_str());
    TaskScheduler* scheduler = PamTaskScheduler::createNew();
    m_penv = PamUsageEnvironment::createNew(*scheduler, m_pHandler);

    if(!CreateStream())
    {
        m_mutex.Unlock();
        return NULL;
    }

    m_mutex.Unlock();

    while(m_eventLoopWatchVariable == 0)
    {
        m_penv->taskScheduler().doEventLoop(&m_eventLoopWatchVariable);
    }

    CloseStream();
    return NULL;
}




bool RtpServerThread::CreateStream()
{
    pmlLog() << "RTP Server: CreateStream -> " << m_sSourceIp;

    char const* mimeType = "L24";
    unsigned char payloadFormatCode = Settings::Get().Read("Server", "RTPMap", 96);

    // Create 'groupsocks' for RTP:
    struct sockaddr_storage destinationAddress;
    destinationAddress.ss_family = AF_INET;
    if(m_sSourceIp.empty() == false)
    {
        NetAddressList destinationAddresses(m_sSourceIp.ToStdString().c_str());
        copyAddress(destinationAddress, destinationAddresses.firstAddress());
    }
    else if(m_bSSM)
    {

        ((sockaddr_in&)destinationAddress).sin_addr.s_addr= chooseRandomIPv4SSMAddress(*m_penv);   //THIS IS THE MULTICAST ADDRESS
    }
    else
    {
        m_bStreaming = false;
        pmlLog(pml::LOG_ERROR) << "RTP Server: CreateStream No SourceIp and not multicast";
        return false;
    }

    const unsigned char ttl = 255;

    const Port rtpPort(m_nRTPPort);
    const Port rtcpPort(m_nRTPPort+1);
    m_pRtpGroupsock = new Groupsock(*m_penv, destinationAddress, rtpPort, ttl);
    if(m_bRTCP)
    {
        m_pRtcpGroupsock = new Groupsock(*m_penv, destinationAddress, rtcpPort, ttl);
    }

    if(m_bSSM)
    {
        m_pRtpGroupsock->multicastSendOnly(); // we're a SSM source
        if(m_bRTCP)
        {
            m_pRtcpGroupsock->multicastSendOnly(); // we're a SSM source
        }
    }


    m_pSource = LiveAudioSource::createNew(m_pHandler, m_mutex, *m_penv, Settings::Get().Read("Server", "Channels", 2), m_ePacketTime,
    Settings::Get().Read("Server", "Bits", 24),Settings::Get().Read("Server", "SampleRate", 48000));

    // Create an appropriate audio RTP sink (using "SimpleRTPSink") from the RTP 'groupsock':
    m_pSink = SimpleRTPSink::createNew(*m_penv, m_pRtpGroupsock, payloadFormatCode, m_pSource->samplingFrequency(), "audio", mimeType, m_pSource->numChannels());

    // @todo do we need to set this here as well as in the source??
    m_pSink->setPacketSizes(m_pSource->GetPreferredFrameSize()+12,m_pSource->GetPreferredFrameSize()+12);

    // Create and start a RTSP server to serve this stream:
    pmlLog() << "RTP Server: Create RTSP Server on port " << m_nRTSPPort;
    m_pRtspServer = PamRTSPServer::createNew(*m_penv, m_nRTSPPort);

    // Create (and start) a 'RTCP instance' for this RTP sink:
    if(m_bRTCP)
    {
        const unsigned int nEstimatedSessionBandwidth =( (m_pSource->samplingFrequency()*m_pSource->numChannels() * 3) + 500)/1000; // in kbps; for RTCP b/w share
        const unsigned int nMaxCNAMElen = 100;
        unsigned char CNAME[nMaxCNAMElen+1];
        gethostname((char*)CNAME, nMaxCNAMElen);
        CNAME[nMaxCNAMElen] = '\0'; // just in case

        m_pRtcpInstance = RTCPInstance::createNew(*m_penv, m_pRtcpGroupsock, nEstimatedSessionBandwidth, CNAME, m_pSink, NULL, True);
    }


    bool bOk(true);
    if (m_pRtspServer == nullptr)
    {
        pmlLog(pml::LOG_ERROR) << "RTP Server\tFailed to create RTSP server: " << m_penv->getResultMsg();
        bOk = false;
    }
    if(m_pRtcpInstance == nullptr && m_bRTCP)
    {
        pmlLog(pml::LOG_ERROR) << "RTP Server\tFailed to create RTCP Instance: " << m_penv->getResultMsg();
        bOk = false;
    }
    if(!bOk)
    {
        Medium::close(m_pSink);
        Medium::close(m_pSource);

        m_pSource= nullptr;
        m_pSink = nullptr;
        m_bStreaming = false;
        delete m_pRtpGroupsock;
        m_pRtpGroupsock = nullptr;
        if(m_pRtcpGroupsock)
        {
            delete m_pRtcpGroupsock;
        }
        m_pRtcpGroupsock = nullptr;
        return false;
    }


    wxString sStream = "by-name/"+ IOManager::Get().GetDnsSdService();

    ServerMediaSession* sms = ServerMediaSession::createNew(*m_penv, sStream, nullptr, "PAM AES67", m_bSSM);
    AES67ServerMediaSubsession* pSmss =  AES67ServerMediaSubsession::createNew(m_setRTCPHandlers, *m_pSink, m_pRtcpInstance, m_ePacketTime, GetChannelMapping());
    sms->addSubsession(pSmss);
    m_pRtspServer->addServerMediaSession(sms);

    char* pSDP = sms->generateSDPDescription(AF_INET);
    m_sSDP = std::string(pSDP);
    delete[] pSDP;


    // Finally, start the streaming:
    pmlLog(pml::LOG_INFO) << "RTP Server\tBeginning streaming..." << m_pRtspServer->rtspURL(sms);
    pmlLog(pml::LOG_INFO) << m_sSDP;



    m_pSink->startPlaying(*m_pSource, afterPlaying, reinterpret_cast<void*>(this));


    m_bStreaming = true;
   // Settings::Get().Write(wxT("AoIP"), wxT("Epoch"), pSmss->GetEpochTimestamp());

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
    pmlLog(pml::LOG_INFO) << "RTP Server\t...done streaming";

    // End by closing the media:
    Medium::close(m_pRtspServer);
    Medium::close(m_pSink);
    delete m_pRtpGroupsock;
    Medium::close(m_pSource);
    if(m_pRtcpInstance)
    {
        Medium::close(m_pRtcpInstance);
        delete m_pRtcpGroupsock;
    }
    m_pSource = nullptr;
    m_pSink = nullptr;
    m_pRtpGroupsock = nullptr;
    m_pRtspServer = nullptr;
    m_pRtcpInstance = nullptr;
    m_pRtcpGroupsock = nullptr;

}

void RtpServerThread::StopStream()
{

    m_eventLoopWatchVariable = 1;
}

void RtpServerThread::SetChannelMapping(const std::vector<char>& vMapping)
{
    m_pSource->SetChannelMapping(vMapping);
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
