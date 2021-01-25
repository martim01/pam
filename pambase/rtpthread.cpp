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
using namespace std;

//// A function that outputs a string that identifies each subsession (for debugging output).  Modify this if you wish:
UsageEnvironment& operator<<(UsageEnvironment& env, const Smpte2110MediaSubsession& subsession)
{
    return env << subsession.mediumName() << "/" << subsession.codecName();
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
    m_pCurrentBuffer(nullptr),
	m_dTransmission(0),
	m_dPresentation(0),
	m_dDelay0(0),
	m_dTSDFMax(0),
	m_dTSDFMin(0),
	m_nSampleRate(48000),
	m_nTimestampErrors(0),
	m_nTimestampErrorsTotal(0),
	m_nTimestamp(0),
	m_nSampleBufferSize(0),
	m_penv(nullptr),
    m_pRtspClient(nullptr),
    m_pSipClient(nullptr),
    m_pSession(nullptr),
    m_bClosing(false),
    m_bSaveSDP(bSaveSDPOnly),
    m_nQosMeasurementIntervalMS(1000)
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

    wxString sProtocol(m_source.sDetails.BeforeFirst(wxT(':')));
    if(sProtocol.CmpNoCase(wxT("rtsp")) == 0)
    {
        pml::Log::Get() << "RTP: connect using RTSP" << std::endl;
        if(DoRTSP())
        {
            while(m_eventLoopWatchVariable == 0)
            {
                m_penv->taskScheduler().doEventLoop(&m_eventLoopWatchVariable);
            }
        }
    }
    else if(sProtocol.CmpNoCase(wxT("sip")) == 0)
    {
        pml::Log::Get() << "RTP:connect using SIP" << std::endl;
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
        pml::Log::Get() << "RTP: connect using SDP" << std::endl;
        m_sDescriptor = m_source.sSDP.AfterFirst('\n').ToStdString();
        StreamFromSDP();
    }

    pml::Log::Get() << "RTP: Stream closed" << std::endl;
    delete[] m_pCurrentBuffer;

    wxCommandEvent* pEvent = new wxCommandEvent(wxEVT_RTP_SESSION_CLOSED);
    pEvent->SetInt(m_source.nIndex);
    pEvent->SetString(m_source.sDetails);
    wxQueueEvent(m_pHandler, pEvent);

    // @todo do we need to delete clients etc?


    return 0;

}

void RtpThread::StreamFromSDP()
{

    pml::Log::Get() << m_sDescriptor << std::endl;


    m_pSession = Smpte2110MediaSession::createNew(*m_penv, m_sDescriptor.c_str());
    if (m_pSession == NULL)
    {
        pml::Log::Get(pml::Log::LOG_ERROR) << "Failed to create a MediaSession object from the SDP description: " << m_penv->getResultMsg() << std::endl;
        return;
    }
    else
    {
        pml::Log::Get() << "Created MediaSession object" << std::endl;
    }

    //count number of subsessions
    unsigned int nCountAudio(0);
    unsigned int nCountVideo(0);
    MediaSubsessionIterator iterCount(*m_pSession);
    MediaSubsession* pSubsessionCount = NULL;
    while ((pSubsessionCount = iterCount.next()) != NULL)
    {
        if (strcmp(pSubsessionCount->codecName(), "L16") == 0 || strcmp(pSubsessionCount->codecName(), "L24") == 0) // 16 or 24-bit linear audio (RFC 3190)
        {
            if(pSubsessionCount->numChannels() > 0)
                nCountAudio++;
            else
            {
                pml::Log::Get(pml::Log::LOG_WARN) << "Audio subsession, but 0 channels defined" << std::endl;
            }
        }
        else if (strcmp(pSubsessionCount->codecName(), "RAW") == 0)
        {
            nCountVideo++;
        }
    }
    pml::Log::Get() << "Number of AES67 Subsessions: " << nCountAudio << std::endl;
    pml::Log::Get() << "Number of Video Subsessions: " << nCountVideo << std::endl;

    if(nCountAudio == 0)
    {
        pml::Log::Get(pml::Log::LOG_WARN) << "No AES67 subsessions. Exit" << std::endl;
        return;
    }
    MediaSubsessionIterator iter(*m_pSession);
    Smpte2110MediaSubsession* subsession = NULL;
    while ((subsession = dynamic_cast<Smpte2110MediaSubsession*>(iter.next())) != NULL)
    {
        if (!subsession->initiate (0))
        {
            pml::Log::Get(pml::Log::LOG_WARN) << "Failed to initiate the subsession: " << m_penv->getResultMsg() << std::endl;
        }
        else
        {
            subsession->sink = wxSink::createNew(*m_penv, *subsession, this);
            pml::Log::Get() << "Initiated the subsession (";
            if (subsession->rtcpIsMuxed())
            {
                pml::Log::Get() << "client port " << subsession->clientPortNum();
            }
            else
            {
                pml::Log::Get() << "client ports " << subsession->clientPortNum() << "-" << subsession->clientPortNum()+1;
            }
            pml::Log::Get() << ")" << std::endl;

            pml::Log::Get() << "SessionId: " << subsession->GetEndpoint() << std::endl;
            if (subsession->sink == NULL)
            {
                pml::Log::Get(pml::Log::LOG_ERROR) << "Failed to create a data sink for the subsession: " << m_penv->getResultMsg() << std::endl;
            }
            else
            {
                pml::Log::Get() << "Created a data sink for the subsession" << std::endl;

                if(m_pRtspClient)
                {   //@todo do we need any setup here??
//                    m_pRtspClient->SetupSubsession(subsession);
                }
                else if(m_pSipClient)
                {
                    m_pSipClient->SetupSubsession(subsession);
                }
                // @todo do we need to send a Start Playing /ACK here??

                // @todo move the startPlaying to later??
                subsession->sink->startPlaying(*subsession->readSource(), NULL, NULL);
                beginQOSMeasurement(*m_penv, m_pSession, this);
            }
        }
    }
    PassSessionDetails(m_pSession);

    while(m_eventLoopWatchVariable == 0)
    {
        m_penv->taskScheduler().doEventLoop(&m_eventLoopWatchVariable);
    }
}

bool RtpThread::DoRTSP()
{
    // Begin by creating a "RTSPClient" object.  Note that there is a separate "RTSPClient" object for each stream that we wish
    // to receive (even if more than stream uses the same "rtsp://" URL).
    wxString sUrl(m_source.sDetails);
    sUrl.Replace(wxT(" "), wxT("%20"));
    m_pRtspClient = ourRTSPClient::createNew((*m_penv), sUrl.mb_str(), this, 1, m_sProgName.mb_str());
    if (m_pRtspClient == NULL)
    {
        pml::Log::Get(pml::Log::LOG_ERROR) << "Failed to create a RTSP client for URL \"" << sUrl.ToStdString() << "\": " << (*m_penv).getResultMsg() << std::endl;
        return false;
    }

    m_pRtspClient->sendOptionsCommand(continueAfterOPTIONS);
    //m_pRtspClient->sendDescribeCommand(continueAfterDESCRIBE);

    return true;
}

bool RtpThread::DoSIP()
{
    wxString sUrl(m_source.sDetails);
    sUrl.Replace(wxT(" "), wxT("%20"));
    m_pSipClient = ourSIPClient::createNew((*m_penv), sUrl.mb_str(), this, 1, m_sProgName.mb_str());
    if (m_pRtspClient == NULL)
    {
        pml::Log::Get(pml::Log::LOG_ERROR) << "Failed to create a RTSP client for URL \"" << sUrl.ToStdString() << "\": " << (*m_penv).getResultMsg() << std::endl;
        return false;
    }
    m_pSipClient->GetSDPDescription();
    return true;
}

pairTime_t RtpThread::ConvertDoubleToPairTime(double dTime)
{
    double dInt, dDec;
    dDec = modf(dTime, &dInt);
    return make_pair(static_cast<unsigned int>(dInt), static_cast<unsigned int>(dDec*1000000.0));
}

float RtpThread::ConvertFrameBufferToSample(u_int8_t* pFrameBuffer, u_int8_t nBytesPerSample)
{
    int nSample(0);
    if(nBytesPerSample == 2)
    {
        nSample = (static_cast<int>(pFrameBuffer[1]) << 16) | (static_cast<int>(pFrameBuffer[0]) << 24);
    }
    else if(nBytesPerSample == 3)
    {
        nSample = (static_cast<int>(pFrameBuffer[2]) << 8) | (static_cast<int>(pFrameBuffer[1]) << 16) | (static_cast<int>(pFrameBuffer[0]) << 24);
    }
    return static_cast<float>(nSample)/ 2147483648.0;
}

void RtpThread::AddFrame(const wxString& sEndpoint, unsigned long nSSRC, const pairTime_t& timePresentation, unsigned long nFrameSize, u_int8_t* pFrameBuffer, u_int8_t nBytesPerSample, const pairTime_t& timeTransmission, unsigned int nTimestamp,unsigned int nDuration, int nTimestampDifference, mExtension_t* pExt)
{
    if(m_bClosing || m_Session.GetCurrentSubsession() == m_Session.lstSubsession.end() || m_Session.GetCurrentSubsession()->sSourceAddress != sEndpoint)
        return;

    if(m_pCurrentBuffer == 0)
    {
        m_pCurrentBuffer = new float[m_nBufferSize*m_nInputChannels];
        m_nSampleBufferSize = 0;
    }

    #ifdef PTPMONKEY
    timeval tv = wxPtp::Get().GetPtpOffset(0);
    double dOffset = tv.tv_sec + (static_cast<double>(tv.tv_usec))/1000000.0;
    #else
    double dOffset = 0.0;
    #endif

    for(int i = 0; i < nFrameSize; i+=nBytesPerSample)
    {
        float dSample(ConvertFrameBufferToSample(&pFrameBuffer[i], nBytesPerSample));

        if(m_nSampleBufferSize == 0)
        {
            m_dTransmission = timeTransmission.first + (static_cast<double>(timeTransmission.second))/1000000.0;
            m_dPresentation = (timePresentation.first + (static_cast<double>(timePresentation.second))/1000000.0) - dOffset;
            //is the timestamp what we'd expect??
            m_nTimestamp = nTimestamp;

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

        m_pCurrentBuffer[m_nSampleBufferSize] = dSample;
        ++m_nSampleBufferSize;

        if(m_nSampleBufferSize == m_nBufferSize*m_nInputChannels)   //filled up buffer
        {
            timedbuffer* pTimedBuffer = new timedbuffer(m_nBufferSize*m_nInputChannels, ConvertDoubleToPairTime(m_dPresentation), m_nTimestamp);
            pTimedBuffer->SetBuffer(m_pCurrentBuffer);
            pTimedBuffer->SetTransmissionTime(ConvertDoubleToPairTime(m_dTransmission));
            pTimedBuffer->SetDuration(nDuration);
            AudioEvent* pEvent = new AudioEvent(pTimedBuffer, AudioEvent::RTP, m_nBufferSize, m_nSampleRate, false, false);
            wxQueueEvent(m_pHandler, pEvent);
            m_nSampleBufferSize = 0;
        }
    }



    //QOS
    if(m_nInputChannels*nBytesPerSample != 0)
    {
        unsigned int nExpectedDifference = nFrameSize/(m_nInputChannels*nBytesPerSample);
        if(nExpectedDifference != nTimestampDifference)
        {
            m_nTimestampErrors++;
            m_nTimestampErrorsTotal++;
        }
    }

    double dTSDF = (timePresentation.first*1000000.0 + (static_cast<double>(timePresentation.second))) - (dOffset*1000000.0);
    dTSDF -= (timeTransmission.first*1000000.0 + (static_cast<double>(timeTransmission.second)));

    dTSDF += m_dDelay0;

    m_dTSDFMax = max(m_dTSDFMax, dTSDF);
    m_dTSDFMin = min(m_dTSDFMin, dTSDF);

}



void RtpThread::StopStream()
{
    pml::Log::Get(pml::Log::LOG_INFO) << "Stop Stream " << std::endl;
    if(m_pRtspClient)
    {
        shutdownStream(m_pRtspClient, 0);
        m_pRtspClient = 0;
    }
    else
    {
        m_eventLoopWatchVariable = 1;
    }

}

void RtpThread::QosUpdated(qosData* pData)
{

    pData->nTimestampErrors = m_nTimestampErrors;
    pData->nTimestampErrorsTotal = m_nTimestampErrorsTotal;
    pData->dTSDF = (m_dTSDFMax-m_dTSDFMin);
    if(m_pHandler)
    {
        wxCommandEvent* pEvent = new wxCommandEvent(wxEVT_QOS_UPDATED);
        pEvent->SetClientData((void*)pData);
        wxQueueEvent(m_pHandler, pEvent);
    }

    m_nTimestampErrors = 0;
    //set out transmission0 and presentation0 ready for next lot of ts-df
    m_dDelay0 = (m_dTransmission-m_dPresentation)*1e6;
    m_dTSDFMax = -10000000000000;
    m_dTSDFMin = 10000000000000;

}


void RtpThread::PassSessionDetails(Smpte2110MediaSession* pSession)
{
    m_Session = session();

    m_Session.sName = wxString::FromUTF8(pSession->sessionName());
    m_Session.sRawSDP = pSession->GetRawSDP();
    m_Session.sType = wxString::FromUTF8(pSession->mediaSessionType());
    m_Session.refClock = pSession->GetRefClock();
    m_Session.sDescription = wxString::FromUTF8(pSession->sessionDescription());
    m_Session.sGroups = pSession->GetGroupDup();


    MediaSubsessionIterator iterSub(*pSession);
    Smpte2110MediaSubsession* pSubsession = NULL;
    while ((pSubsession = dynamic_cast<Smpte2110MediaSubsession*>(iterSub.next())) != NULL)
    {
        refclk clock = pSubsession->GetRefClock();
        m_Session.lstSubsession.push_back(subsession(wxString::FromUTF8(pSubsession->sessionId()),
                                                     wxString::FromUTF8(pSubsession->GetEndpoint()),
                                                     wxString::FromUTF8(pSubsession->mediumName()),
                                                     wxString::FromUTF8(pSubsession->codecName()),
                                                     wxString::FromUTF8(pSubsession->protocolName()),
                                                     pSubsession->clientPortNum(),
                                                     pSubsession->rtpTimestampFrequency(),
                                                     pSubsession->numChannels(),
                                                     wxEmptyString,  /* @todo this is the channel list from SMPTE2110 */
                                                     pSubsession->GetSyncTime(),
                                                     pSubsession->GetLastEpoch(),
                                                     pSubsession->GetRefClock()));
        #ifdef PTPMONKEY
        if(pSubsession->GetRefClock().sType.CmpNoCase(wxT("PTP")) == 0)
        {
            wxPtp::Get().RunDomain(std::string(m_sReceivingInterface.mb_str()), pSubsession->GetRefClock().nDomain);
        }
        #endif // PTPMONKEY

        pml::Log::Get(pml::Log::LOG_DEBUG) << "Subsession sync: " << pSubsession->GetSyncTime() << std::endl;
    }

    m_Session.SetCurrentSubsession();
    if(m_Session.GetCurrentSubsession() != m_Session.lstSubsession.end())
    {
        m_nSampleRate = m_Session.GetCurrentSubsession()->nSampleRate;
        m_nInputChannels = min((unsigned int)256 ,m_Session.GetCurrentSubsession()->nChannels);
    }
    else
    {
        m_nSampleRate = 48000;
        m_nInputChannels = 0;
        pml::Log::Get(pml::Log::LOG_ERROR) << "No Input Channels" << std::endl;
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
    wxMutexLocker ml(m_mutex);
    return m_nQosMeasurementIntervalMS;
}

void RtpThread::MasterClockChanged()
{

    if(m_pSession)
    {
        MediaSubsessionIterator iter(*m_pSession);
        Smpte2110MediaSubsession* subsession = NULL;
        while ((subsession = dynamic_cast<Smpte2110MediaSubsession*>(iter.next())) != NULL)
        {
            Aes67Source* pSource = dynamic_cast<Aes67Source*>(subsession->readSource());
            if(pSource)
            {

                pSource->WorkoutLastEpoch();

            }
        }
    }
}
