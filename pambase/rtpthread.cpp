#include "rtpthread.h"
#include "rtsputil.h"
#include "audio.h"
#include <iostream>
#include <wx/log.h>
#include "PamUsageEnvironment.h"
#include "PamTaskScheduler.h"
#include "timedbuffer.h"
#include "smpte2110mediasession.h"
#include "aes67mediasession.h"
#include "wxsink.h"
#include "audioevent.h"
#include "GroupsockHelper.hh"
#include "wxptp.h"
#include "aes67source.h"
#include "log.h"
#include "settings.h"
#include "rtpframeevent.h"

using namespace std;

//// A function that outputs a string that identifies each subsession (for debugging output).  Modify this if you wish:
UsageEnvironment& operator<<(UsageEnvironment& env, const Smpte2110MediaSubsession& subsession)
{
    return env << subsession.mediumName() << "/" << subsession.codecName();
}

bool Tv1LessThanTv2(const timeval& tv1, const timeval& tv2)
{
    return (tv1.tv_sec < tv2.tv_sec || (tv1.tv_sec == tv2.tv_sec && tv1.tv_usec < tv2.tv_usec));
}



DEFINE_EVENT_TYPE(wxEVT_QOS_UPDATED)
DEFINE_EVENT_TYPE(wxEVT_RTP_SESSION)
DEFINE_EVENT_TYPE(wxEVT_RTP_SESSION_CLOSED)
DEFINE_EVENT_TYPE(wxEVT_SDP)


RtpThread::RtpThread(wxEvtHandler* pHandler, const wxString& sReceivingInterface, const wxString& sProg, const AoIPSource& source, unsigned int nBufferSize, bool bSaveSDPOnly) :
    wxThread(wxTHREAD_JOINABLE),
    m_pHandler(pHandler),
    m_sProgName(sProg),
    m_source(source),
    m_nBufferSize(nBufferSize),
    m_bSaveSDP(bSaveSDPOnly)
{
    m_eventLoopWatchVariable = 0;
    m_pCondition = new wxCondition(m_mutex);
    m_nInputChannels = 2;
    m_sReceivingInterface = sReceivingInterface;
    //set the receivinginterface to eth0 or whatever the user choose
   // if(sReceivingInterface.empty() == false)
   // {
   //     ReceivingInterfaceAddr = our_inet_addr(std::string(sReceivingInterface.mb_str()).c_str());
   // }
   // else
   // {
        ReceivingInterfaceAddr = INADDR_ANY;

    //}
}


void* RtpThread::Entry()
{
    m_nSampleRate = 48000;
    // Begin by setting up our usage environment:
    TaskScheduler* scheduler = PamTaskScheduler::createNew();
    m_penv = PamUsageEnvironment::createNew(*scheduler, m_pHandler);

    if(m_source.sType.CmpNoCase(wxT("rtsp")) == 0)
    {
        pmlLog(pml::LOG_INFO, "pam::rtpclient") << "connect using RTSP";
        if(DoRTSP())
        {
            while(m_eventLoopWatchVariable == 0)
            {
                m_penv->taskScheduler().doEventLoop(&m_eventLoopWatchVariable);
            }
        }
    }
    else if(m_source.sType.CmpNoCase(wxT("sip")) == 0)
    {
        pmlLog(pml::LOG_INFO, "pam::rtpclient") << "connect using SIP";
        if(DoSIP())
        {
            while(m_eventLoopWatchVariable == 0)
            {
                m_penv->taskScheduler().doEventLoop(&m_eventLoopWatchVariable);
            }
        }
    }
    else
    {
        pmlLog(pml::LOG_INFO, "pam::rtpclient") << "connect using SDP";
        m_sDescriptor = m_source.sSDP.AfterFirst('\n').ToStdString();
        StreamFromSDP();
    }


    pmlLog(pml::LOG_INFO, "pam::rtpclient") << "Stream closed";

    wxCommandEvent* pEvent = new wxCommandEvent(wxEVT_RTP_SESSION_CLOSED);
    pEvent->SetInt(m_source.nIndex);
    pEvent->SetString(m_source.sDetails);
    wxQueueEvent(m_pHandler, pEvent);

    // @todo do we need to delete clients etc?
    if(m_pRtspClient)
    {
        pmlLog(pml::LOG_DEBUG, "pam::rtpclient") << "Shutdown stream";
        shutdownStream(m_pRtspClient, 0);
        m_pRtspClient = nullptr;
    }


    return 0;

}

void RtpThread::StreamFromSDP()
{

    pmlLog(pml::LOG_INFO, "pam::rtpclient") << m_sDescriptor;

    m_pSession = Smpte2110MediaSession::createNew(*m_penv, m_sDescriptor.c_str());
    if (m_pSession == nullptr)
    {
        pmlLog(pml::LOG_ERROR, "pam::rtpclient") << "Failed to create a MediaSession object from the SDP description: " << m_penv->getResultMsg();
        return;
    }
    else
    {
        pmlLog(pml::LOG_DEBUG, "pam::rtpclient") << "Created MediaSession object";
    }

    //count number of subsessions
    unsigned int nCountAudio(0);
    MediaSubsessionIterator iterCount(*m_pSession);
    MediaSubsession* pSubsessionCount = nullptr;
    while ((pSubsessionCount = iterCount.next()) != nullptr)
    {
        if (strcmp(pSubsessionCount->codecName(), "L16") == 0 || strcmp(pSubsessionCount->codecName(), "L24") == 0
        || strcmp(pSubsessionCount->codecName(), "AM824") == 0) // 16 or 24-bit linear audio (RFC 3190) or 2110-31
        {
            if(pSubsessionCount->numChannels() > 0)
            {
                nCountAudio++;
            }
            else
            {
                pmlLog(pml::LOG_WARN, "pam::rtpclient") << "Audio subsession, but 0 channels defined";
            }
        }
    }
    pmlLog(pml::LOG_DEBUG, "pam::rtpclient") << "Number of AES67 Subsessions: " << nCountAudio;
    pmlLog(pml::LOG_DEBUG, "pam::rtpclient") << "Streams = " << m_pSession->GetGroups().size();

    if(nCountAudio == 0)
    {
        pmlLog(pml::LOG_WARN, "pam::rtpclient") << "No AES67 subsessions. Exit";
        return;
    }


    MediaSubsessionIterator iter(*m_pSession);
    Smpte2110MediaSubsession* pSubsession = nullptr;
    while ((pSubsession = dynamic_cast<Smpte2110MediaSubsession*>(iter.next())) != nullptr)
    {
        if((m_pSession->GetGroups().find(pSubsession->GetGroup()) != m_pSession->GetGroups().end())) // duplicate streams
        {
            if (!pSubsession->initiate (0))
            {
                pmlLog(pml::LOG_WARN, "pam::rtpclient") << "Failed to initiate the subsession: " << m_penv->getResultMsg();
            }
            else
            {
                CreateSink(pSubsession);
            }
        }
    }
    PassSessionDetails(m_pSession);
    beginQOSMeasurement(*m_penv, m_pSession, this);



    while(m_eventLoopWatchVariable == 0)
    {
        m_penv->taskScheduler().doEventLoop(&m_eventLoopWatchVariable);
    }
}

void RtpThread::CreateSink(Smpte2110MediaSubsession* pSubsession)
{
    pSubsession->sink = wxSink::createNew(*m_penv, *pSubsession, this);
    pmlLog(pml::LOG_DEBUG, "pam::rtpclient") << "Initiated the subsession: ";
    if (pSubsession->rtcpIsMuxed())
    {
        pmlLog(pml::LOG_DEBUG, "pam::rtpclient") << "client port " << pSubsession->clientPortNum();
    }
    else
    {
        pmlLog(pml::LOG_DEBUG, "pam::rtpclient") << "client ports " << pSubsession->clientPortNum() << "-" << pSubsession->clientPortNum()+1;
    }
    pmlLog(pml::LOG_DEBUG, "pam::rtpclient") << "SessionId: " << pSubsession->GetEndpoint();

    if (pSubsession->sink == nullptr)
    {
        pmlLog(pml::LOG_ERROR, "pam::rtpclient") << "Failed to create a data sink for the subsession: " << m_penv->getResultMsg();
    }
    else
    {
        pmlLog(pml::LOG_DEBUG, "pam::rtpclient") << "Created a data sink for the subsession";
        pSubsession->sink->startPlaying(*pSubsession->readSource(), nullptr, nullptr);
    }
}

bool RtpThread::DoRTSP()
{
    // Begin by creating a "RTSPClient" object.  Note that there is a separate "RTSPClient" object for each stream that we wish
    // to receive (even if more than stream uses the same "rtsp://" URL).
    wxString sUrl(m_source.sDetails);
    sUrl.Replace(wxT(" "), wxT("%20"));
    m_pRtspClient = ourRTSPClient::createNew((*m_penv), sUrl.mb_str(), this, 1, m_sProgName.mb_str());
    if (m_pRtspClient == nullptr)
    {
        pmlLog(pml::LOG_ERROR, "pam::rtpclient") << "Failed to create a RTSP client for URL \"" << sUrl.ToStdString() << "\": " << (*m_penv).getResultMsg();
        return false;
    }
    pmlLog(pml::LOG_INFO, "pam::rtpclient") << "RTSP Send Options";
    m_pRtspClient->sendOptionsCommand(continueAfterOPTIONS);

    return true;
}

bool RtpThread::DoSIP()
{
    wxString sUrl(m_source.sDetails);
    sUrl.Replace(wxT(" "), wxT("%20"));
    m_pSipClient = ourSIPClient::createNew((*m_penv), sUrl.mb_str(), this, 1, m_sProgName.mb_str());
    if (m_pRtspClient == nullptr)
    {
        pmlLog(pml::LOG_ERROR, "pam::rtpclient") << "Failed to create a RTSP client for URL \"" << sUrl.ToStdString() << "\": " << (*m_penv).getResultMsg();
        return false;
    }
    m_pSipClient->GetSDPDescription();
    return true;
}

timeval RtpThread::ConvertDoubleToPairTime(double dTime)
{
    double dInt;
    auto dDec = modf(dTime, &dInt);
    return {static_cast<time_t>(dInt), static_cast<__suseconds_t>(dDec*1000000.0)};
}

std::pair<float, unsigned char> RtpThread::ConvertFrameBufferToSample(u_int8_t* pFrameBuffer, u_int8_t nBytesPerSample)
{
    int nSample = 0;
    unsigned char nUser = 0;
    if(nBytesPerSample == 2)
    {
        nSample = (static_cast<int>(pFrameBuffer[1]) << 16) | (static_cast<int>(pFrameBuffer[0]) << 24);
    }
    else if(nBytesPerSample == 3)
    {
        nSample = (static_cast<int>(pFrameBuffer[2]) << 8) | (static_cast<int>(pFrameBuffer[1]) << 16) | (static_cast<int>(pFrameBuffer[0]) << 24);
    }
    else if(nBytesPerSample == 4)
    {   //@todo need to check which 3 of the 4 we uses
        nSample = (static_cast<int>(pFrameBuffer[3]) << 8) | (static_cast<int>(pFrameBuffer[2]) << 16) | (static_cast<int>(pFrameBuffer[1]) << 24);

        nUser = pFrameBuffer[0];
    }
    return {static_cast<float>(nSample)/ 2147483648.0, nUser};
}

void RtpThread::AddFrame(std::shared_ptr<const rtpFrame> pFrame)
{
    if(m_bClosing || m_Session.GetCurrentSubsession() == m_Session.lstSubsession.end() || m_nInputChannels==0)
    {
        return;
    }

    auto itGroup = m_mRedundantBuffers.find(pFrame->sGroup);
    if(itGroup == m_mRedundantBuffers.end())
    {
        itGroup = m_mRedundantBuffers.insert({pFrame->sGroup, std::list<std::shared_ptr<const rtpFrame>>()}).first;
        m_mStreamUsage.try_emplace(pFrame->sGroup, 0);
    }
    itGroup->second.push_back(pFrame);

    auto itReceived = m_mFramesReceived.find(pFrame->sGroup);
    if(itReceived == m_mFramesReceived.end())
    {
        itReceived = m_mFramesReceived.try_emplace(pFrame->sGroup,0).first;
    }
    ++itReceived->second;

    //send out an rtpframe event 20 times a second... every 50ms

    if(m_pHandler && itReceived->second%(14400 / pFrame->nFrameSize) == 0)
    {
       auto pEvent = new RtpFrameEvent(pFrame);
       wxQueueEvent(m_pHandler, pEvent);
    }

    HandleFrameAdded();
}

void RtpThread::WorkoutFirstFrame()
{
    //if we've not yet extracted from the buffer then get the one with the earliest transmission time
    gettimeofday(&m_tvLastExtracted, nullptr);
    size_t nMaxReceived = 0;

    for(const auto& pairBuffer : m_mRedundantBuffers)
    {
        nMaxReceived = std::max(nMaxReceived, pairBuffer.second.size());
        if(pairBuffer.second.empty() == false && Tv1LessThanTv2(pairBuffer.second.front()->timeTransmission, m_tvLastExtracted))
        {
            m_nTimestamp = pairBuffer.second.front()->nTimestamp-1;
            m_tvLastExtracted = pairBuffer.second.front()->timeTransmission;
        }
    }
    if(nMaxReceived > m_nRedundantBufferQueue)  //we don't move on from this until at least one stream has sent (10) frames
    {
        m_bFrameExtracted = true;
    }
}

void RtpThread::HandleFrameAdded()
{
    if(m_mRedundantBuffers.size() == 1) //no redundant frames so no point buffering
    {
        if(m_mRedundantBuffers.begin()->second.empty() == false)
        {
            ConvertFrameToTimedBuffer(m_mRedundantBuffers.begin()->second.front());
            m_mRedundantBuffers.begin()->second.pop_front();
        }
    }
    else if(m_bFrameExtracted == false)
    {
        WorkoutFirstFrame();
    }
    else
    {
        WorkoutNextFrame();
    }
}

void RtpThread::WorkoutNextFrame()
{
    auto bExtracted = false;
    for(const auto& pairBuffer : m_mRedundantBuffers)
    {
        for(auto itFrame = pairBuffer.second.begin(); itFrame != pairBuffer.second.end();)
        {
            if(Tv1LessThanTv2((*itFrame)->timeTransmission, m_tvLastExtracted))
            {   //delete frame if transmitted earlier than the last one we extracted
                auto itDelete = itFrame;
                ++itFrame;
                pairBuffer.second.erase(itDelete);
            }
            else if((*itFrame)->nTimestamp == m_nTimestamp+1)   //this is the timestamp we want
            {
                if(!bExtracted)
                {
                    m_tvLastExtracted = (*itFrame)->timeTransmission;
                    ConvertFrameToTimedBuffer(*itFrame);
                    bExtracted = true;
                }
                pairBuffer.second.erase(itFrame);
                break;
            }
            else
            {
                ++itFrame;
            }
        }
    }
}

void RtpThread::ConvertFrameToTimedBuffer(std::shared_ptr<const rtpFrame> pFrame)
{
    if(m_vCurrentBuffer.empty())
    {
        m_vCurrentBuffer.resize(m_nBufferSize*m_nInputChannels);
        m_vCurrentUserBits.resize(m_vCurrentBuffer.size());
        m_nSampleBufferSize = 0;
    }

    double dOffset = 0.0;

    //pml::LogStream ls;

    for(int i = 0; i < pFrame->nFrameSize; i+=pFrame->nBytesPerSample)
    {
        auto sample = ConvertFrameBufferToSample(&(pFrame->pBuffer[i]), pFrame->nBytesPerSample);

         //ls << std::dec << sample.first << std::hex << "[" << (int)pFrame->pBuffer[i] << (int)pFrame->pBuffer[i+1] << (int)pFrame->pBuffer[i+2]  << "],";

        if(m_nSampleBufferSize == 0)
        {
            m_dTransmission = pFrame->timeTransmission.tv_sec + (static_cast<double>(pFrame->timeTransmission.tv_usec))/1000000.0;
            m_dPresentation = (pFrame->timePresentation.tv_sec + (static_cast<double>(pFrame->timePresentation.tv_usec))/1000000.0) - dOffset;
            //is the timestamp what we'd expect??
            m_nTimestamp = pFrame->nTimestamp;

        }
        else if(i%m_nInputChannels ==0)
        {
            ++m_nTimestamp; //timestamp goes up 1 per sample
            if(m_nSampleRate == 0)
            {
                 m_nSampleRate = 48000;
            }
            m_dTransmission += (1.0 / static_cast<double>(m_nSampleRate));
            m_dPresentation += (1.0 / static_cast<double>(m_nSampleRate)); //@todo assuming 48K here
        }

        m_vCurrentBuffer[m_nSampleBufferSize] = sample.first;
        m_vCurrentUserBits[m_nSampleBufferSize] = sample.second;
        ++m_nSampleBufferSize;

        if(m_nSampleBufferSize == m_vCurrentBuffer.size())   //filled up buffer
        {
            auto pTimedBuffer = new timedbuffer(ConvertDoubleToPairTime(m_dPresentation), m_nTimestamp, m_nInputChannels, m_vCurrentBuffer, m_vCurrentUserBits);

            pTimedBuffer->SetTransmissionTime(ConvertDoubleToPairTime(m_dTransmission));
            pTimedBuffer->SetDuration(pFrame->nFrameSize);
            auto pEvent = new AudioEvent(pTimedBuffer, AudioEvent::RTP, m_nBufferSize, m_nSampleRate, false, false);
            wxQueueEvent(m_pHandler, pEvent);
            m_nSampleBufferSize = 0;
        }
    }
    //QOS
    if(m_nInputChannels*pFrame->nBytesPerSample != 0)
    {
        auto nExpectedDifference = pFrame->nFrameSize/(m_nInputChannels*pFrame->nBytesPerSample);
        if(nExpectedDifference != pFrame->nTimestampDifference)
        {
            m_nTimestampErrors++;
            m_nTimestampErrorsTotal++;
        }
    }

    //m_bFirstDebug = false;


    //keep track of which streams frames come from
    ++m_mStreamUsage[pFrame->sGroup];
    ++m_nTotalFramesPlayed;
}



void RtpThread::StopStream()
{
    if(m_pRtspClient)
    {
        pmlLog(pml::LOG_INFO, "pam::rtpclient") << "Stop Stream ";
        pmlLog(pml::LOG_DEBUG, "pam::rtpclient") << "Shutdown stream";
        shutdownStream(m_pRtspClient, 0);
        m_pRtspClient = nullptr;
    }
    else
    {
        m_eventLoopWatchVariable = 1;
    }

}

void RtpThread::QosUpdated(qosData* pData)
{
    auto itStream = m_mStreamUsage.find(pData->sStream);
    if(itStream != m_mStreamUsage.end())
    {
        pData->nFramesUsed = itStream->second;
    }
    auto itReceived = m_mFramesReceived.find(pData->sStream);
    if(itReceived != m_mFramesReceived.end())
    {
        pData->nFramesReceived = itReceived->second;
    }
    auto itBuffer = m_mRedundantBuffers.find(pData->sStream);
    if(itBuffer != m_mRedundantBuffers.end())
    {
        pData->nBufferSize = itBuffer->second.size();
    }
    pData->nTotalFrames = m_nTotalFramesPlayed;

    pData->nTimestampErrors = m_nTimestampErrors;
    pData->nTimestampErrorsTotal = m_nTimestampErrorsTotal;
    if(m_pHandler)
    {
        wxCommandEvent* pEvent = new wxCommandEvent(wxEVT_QOS_UPDATED);
        pEvent->SetClientData((void*)pData);
        wxQueueEvent(m_pHandler, pEvent);
    }

    m_nTimestampErrors = 0;

}


void RtpThread::PassSessionDetails(Smpte2110MediaSession* pSession)
{
    pmlLog(pml::LOG_DEBUG, "pam::rtpclient") << "PassSessionDetails";

    //make the buffer queue the same size as the number of streams

    m_Session = session();

    m_Session.sName = wxString::FromUTF8(pSession->sessionName());
    m_Session.sRawSDP = pSession->GetRawSDP();
    m_Session.sType = wxString::FromUTF8(pSession->mediaSessionType());
    m_Session.refClock = pSession->GetRefClock();
    m_Session.sDescription = wxString::FromUTF8(pSession->sessionDescription());
    m_Session.setGroups = pSession->GetGroups();


    MediaSubsessionIterator iterSub(*pSession);
    Smpte2110MediaSubsession* pSubsession = nullptr;
    while ((pSubsession = dynamic_cast<Smpte2110MediaSubsession*>(iterSub.next())) != nullptr)
    {
        refclk clock = pSubsession->GetRefClock();
        timeval tvEpoch = pSubsession->GetLastEpoch();

        m_Session.lstSubsession.push_back(subsession(wxString::FromUTF8(pSubsession->sessionId()),
                                                     wxString::FromUTF8(pSubsession->GetEndpoint()),
                                                     pSubsession->GetGroup(),
                                                     wxString::FromUTF8(pSubsession->mediumName()),
                                                     wxString::FromUTF8(pSubsession->codecName()),
                                                     wxString::FromUTF8(pSubsession->protocolName()),
                                                     pSubsession->clientPortNum(),
                                                     pSubsession->rtpTimestampFrequency(),
                                                     pSubsession->GetChannelGrouping(),
                                                     pSubsession->GetSyncTime(),
                                                     tvEpoch,
                                                     pSubsession->GetRefClock()));
        #ifdef PTPMONKEY
        if(pSubsession->GetRefClock().sType.CmpNoCase("PTP") == 0)
        {
            wxPtp::Get().Run(std::string(m_sReceivingInterface.mb_str()), pSubsession->GetRefClock().nDomain, Settings::Get().Read("Time", "Ptp_Mode", 0) ? pml::ptpmonkey::Mode::HYBRID : pml::ptpmonkey::Mode::MULTICAST);
        }
        #endif // PTPMONKEY

        pmlLog(pml::LOG_DEBUG, "pam::rtpclient") << "Subsession sync: " << pSubsession->GetSyncTime();
    }

    m_Session.SetCurrentSubsession();
    if(m_Session.GetCurrentSubsession() != m_Session.lstSubsession.end())
    {
        m_nSampleRate = m_Session.GetCurrentSubsession()->nSampleRate;
        m_nInputChannels = min((unsigned int)8 ,m_Session.GetCurrentSubsession()->nChannels);
        pmlLog(pml::LOG_INFO, "pam::rtpclient") << m_nInputChannels << " channels at " << m_nSampleRate << " Hz";
    }
    else
    {
        m_nSampleRate = 48000;
        m_nInputChannels = 0;
        pmlLog(pml::LOG_ERROR, "pam::rtpclient") << "No Input Channels";
        StopStream();
    }

    if(m_pHandler)
    {
        wxCommandEvent* pEvent = new wxCommandEvent(wxEVT_RTP_SESSION);
        pEvent->SetClientData((void*)&m_Session);
        wxQueueEvent(m_pHandler, pEvent);
    }
}


void RtpThread::SaveSDP(unsigned int nResult, const std::string& sResult)
{
    m_sDescriptor = sResult;

    //Start playing
    StreamFromSDP();

    if(m_pHandler)
    {
        wxCommandEvent* pEvent = new wxCommandEvent(wxEVT_SDP);
        pEvent->SetClientData(reinterpret_cast<void*>(this));
        pEvent->SetString(m_sDescriptor);
        pEvent->SetInt(nResult);
        wxQueueEvent(m_pHandler, pEvent);
    }
}

void RtpThread::SetQosMeasurementIntervalMS(unsigned long nMilliseconds)
{
    wxMutexLocker ml(m_mutex);
    m_nQosMeasurementIntervalMS = nMilliseconds;
}

unsigned long RtpThread::GetQosMeasurementIntervalMS()
{
    return m_nQosMeasurementIntervalMS;
}

void RtpThread::MasterClockChanged()
{

    if(m_pSession)
    {
        MediaSubsessionIterator iter(*m_pSession);
        Smpte2110MediaSubsession* subsession = nullptr;
        while ((subsession = dynamic_cast<Smpte2110MediaSubsession*>(iter.next())) != nullptr)
        {
            auto pSource = dynamic_cast<Aes67Source*>(subsession->readSource());
            if(pSource)
            {
                pSource->WorkoutLastEpoch();
            }
        }
    }
}


void RtpThread::StreamShutdown()
{
    m_pRtspClient = nullptr;    //to stop us calling shutdownStream again
}
