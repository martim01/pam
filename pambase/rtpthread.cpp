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
	m_dDelay0(std::numeric_limits<double>::lowest()),
	m_dTSDFMax(std::numeric_limits<double>::lowest()),
	m_dTSDFMin(std::numeric_limits<double>::max()),
    m_dTSDF(0),
	m_nTSDFCount(0),
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

    if(m_source.sType.CmpNoCase(wxT("rtsp")) == 0)
    {
        pmlLog() << "RTP Client\tconnect using RTSP";
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
        pmlLog() << "RTP Client\tconnect using SIP";
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
        pmlLog() << "RTP Client\tconnect using SDP";
        m_sDescriptor = m_source.sSDP.AfterFirst('\n').ToStdString();
        StreamFromSDP();
    }


    pmlLog() << "RTP Client\tStream closed";
    delete[] m_pCurrentBuffer;

    wxCommandEvent* pEvent = new wxCommandEvent(wxEVT_RTP_SESSION_CLOSED);
    pEvent->SetInt(m_source.nIndex);
    pEvent->SetString(m_source.sDetails);
    wxQueueEvent(m_pHandler, pEvent);

    // @todo do we need to delete clients etc?
    if(m_pRtspClient)
    {
        pmlLog(pml::LOG_TRACE) << "RTPThread::Entry\t" << "Shutdown stream";
        shutdownStream(m_pRtspClient, 0);
        m_pRtspClient = nullptr;
    }


    return 0;

}

void RtpThread::StreamFromSDP()
{

    pmlLog() << m_sDescriptor;


    m_pSession = Smpte2110MediaSession::createNew(*m_penv, m_sDescriptor.c_str());
    if (m_pSession == NULL)
    {
        pmlLog(pml::LOG_ERROR) << "RTP Client\tFailed to create a MediaSession object from the SDP description: " << m_penv->getResultMsg();
        return;
    }
    else
    {
        pmlLog(pml::LOG_DEBUG) << "RTP Client\tCreated MediaSession object";
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
                pmlLog(pml::LOG_WARN) << "RTP Client\tAudio subsession, but 0 channels defined";
            }
        }
        else if (strcmp(pSubsessionCount->codecName(), "RAW") == 0)
        {
            nCountVideo++;
        }
    }
    pmlLog(pml::LOG_DEBUG) << "RTP Client\tNumber of AES67 Subsessions: " << nCountAudio;
    pmlLog(pml::LOG_DEBUG) << "RTP Client\tNumber of Video Subsessions: " << nCountVideo;

    if(nCountAudio == 0)
    {
        pmlLog(pml::LOG_WARN) << "RTP Client\tNo AES67 subsessions. Exit";
        return;
    }
    MediaSubsessionIterator iter(*m_pSession);
    Smpte2110MediaSubsession* subsession = NULL;
    while ((subsession = dynamic_cast<Smpte2110MediaSubsession*>(iter.next())) != NULL)
    {
        if (!subsession->initiate (0))
        {
            pmlLog(pml::LOG_WARN) << "RTP Client\tFailed to initiate the subsession: " << m_penv->getResultMsg();
        }
        else
        {
            subsession->sink = wxSink::createNew(*m_penv, *subsession, this);
            pmlLog(pml::LOG_DEBUG) << "RTP Client\tInitiated the subsession: ";
            if (subsession->rtcpIsMuxed())
            {
                pmlLog(pml::LOG_DEBUG) << "client port " << subsession->clientPortNum();
            }
            else
            {
                pmlLog(pml::LOG_DEBUG) << "client ports " << subsession->clientPortNum() << "-" << subsession->clientPortNum()+1;
            }

            pmlLog(pml::LOG_DEBUG) << "RTP Client\tSessionId: " << subsession->GetEndpoint();
            if (subsession->sink == NULL)
            {
                pmlLog(pml::LOG_ERROR) << "RTP Client\tFailed to create a data sink for the subsession: " << m_penv->getResultMsg();
            }
            else
            {
                pmlLog(pml::LOG_DEBUG) << "RTP Client\tCreated a data sink for the subsession";

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
        pmlLog(pml::LOG_ERROR) << "RTP Client\tFailed to create a RTSP client for URL \"" << sUrl.ToStdString() << "\": " << (*m_penv).getResultMsg();
        return false;
    }
    pmlLog() << "RTSP Send Options";
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
        pmlLog(pml::LOG_ERROR) << "RTP Client\tFailed to create a RTSP client for URL \"" << sUrl.ToStdString() << "\": " << (*m_penv).getResultMsg();
        return false;
    }
    m_pSipClient->GetSDPDescription();
    return true;
}

timeval RtpThread::ConvertDoubleToPairTime(double dTime)
{
    double dInt, dDec;
    dDec = modf(dTime, &dInt);
    return {static_cast<time_t>(dInt), static_cast<__suseconds_t>(dDec*1000000.0)};
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

void RtpThread::AddFrame(const wxString& sEndpoint, unsigned long nSSRC, const timeval& timePresentation, unsigned long nFrameSize, u_int8_t* pFrameBuffer,
u_int8_t nBytesPerSample, const timeval& timeTransmission,
unsigned int nTimestamp,unsigned int nDuration, int nTimestampDifference, mExtension_t* pExt)
{
    if(m_bClosing || m_Session.GetCurrentSubsession() == m_Session.lstSubsession.end() || m_Session.GetCurrentSubsession()->sSourceAddress != sEndpoint)
        return;

    if(m_pCurrentBuffer == 0)
    {
        m_pCurrentBuffer = new float[m_nBufferSize*m_nInputChannels];
        m_nSampleBufferSize = 0;
    }

    double dOffset = 0.0;

    for(int i = 0; i < nFrameSize; i+=nBytesPerSample)
    {
        float dSample(ConvertFrameBufferToSample(&pFrameBuffer[i], nBytesPerSample));

        if(m_nSampleBufferSize == 0)
        {
            m_dTransmission = timeTransmission.tv_sec + (static_cast<double>(timeTransmission.tv_usec))/1000000.0;
            m_dPresentation = (timePresentation.tv_sec + (static_cast<double>(timePresentation.tv_usec))/1000000.0) - dOffset;
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
            timedbuffer* pTimedBuffer = new timedbuffer(m_nBufferSize*m_nInputChannels, ConvertDoubleToPairTime(m_dPresentation), m_nTimestamp, m_nInputChannels);
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

    int nFramesPerSec = (m_nSampleRate*m_nInputChannels*nBytesPerSample)/nFrameSize;



    timeval tvSub;
    timersub(&timePresentation, &timeTransmission, &tvSub);
    double dTSDF = (static_cast<double>(tvSub.tv_sec)*1000000.0)+tvSub.tv_usec;

    if(m_dDelay0 == std::numeric_limits<double>::lowest() || m_nTSDFCount == nFramesPerSec)
    {
        m_dTSDF = m_dTSDFMax-m_dTSDFMin;
        m_dDelay0 = dTSDF;

        m_dTSDFMax = std::numeric_limits<double>::lowest();
        m_dTSDFMin = std::numeric_limits<double>::max();

        m_nTSDFCount = 1;

    }
    else
    {
        dTSDF -= m_dDelay0;
       // dTSDF *= 1000000.0;
        m_dTSDFMax = max(m_dTSDFMax, dTSDF);
        m_dTSDFMin = min(m_dTSDFMin, dTSDF);
        m_nTSDFCount++;
    }
}



void RtpThread::StopStream()
{
    if(m_pRtspClient)
    {
        pmlLog(pml::LOG_INFO) << "RTP Client\tStop Stream ";
        pmlLog(pml::LOG_TRACE) <<  "RTPThread::StopStream\t" << "Shutdown stream";
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

    pData->nTimestampErrors = m_nTimestampErrors;
    pData->nTimestampErrorsTotal = m_nTimestampErrorsTotal;
    pData->dTSDF = m_dTSDF;
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
        timeval tvEpoch = pSubsession->GetLastEpoch();
        std::vector<std::pair<unsigned char, wxString>> vChannels(pSubsession->numChannels());
        for(size_t i = 0; i < pSubsession->numChannels(); i++)
        {
            vChannels[i] = std::make_pair(pSubsession->GetChannelGrouping()[i], pSubsession->GetChannelName(i));
        }

        m_Session.lstSubsession.push_back(subsession(wxString::FromUTF8(pSubsession->sessionId()),
                                                     wxString::FromUTF8(pSubsession->GetEndpoint()),
                                                     wxString::FromUTF8(pSubsession->mediumName()),
                                                     wxString::FromUTF8(pSubsession->codecName()),
                                                     wxString::FromUTF8(pSubsession->protocolName()),
                                                     pSubsession->clientPortNum(),
                                                     pSubsession->rtpTimestampFrequency(),
                                                     vChannels,
                                                     pSubsession->GetSyncTime(),
                                                     tvEpoch,
                                                     pSubsession->GetRefClock()));
        #ifdef PTPMONKEY
        if(pSubsession->GetRefClock().sType.CmpNoCase(wxT("PTP")) == 0)
        {
            wxPtp::Get().RunDomain(std::string(m_sReceivingInterface.mb_str()), pSubsession->GetRefClock().nDomain);
        }
        #endif // PTPMONKEY

        pmlLog(pml::LOG_DEBUG) << "RTP Client\tSubsession sync: " << pSubsession->GetSyncTime();
    }

    m_Session.SetCurrentSubsession();
    if(m_Session.GetCurrentSubsession() != m_Session.lstSubsession.end())
    {
        m_nSampleRate = m_Session.GetCurrentSubsession()->nSampleRate;
        m_nInputChannels = min((unsigned int)256 ,m_Session.GetCurrentSubsession()->nChannels);
        pmlLog() << "RTP Client\t" << m_nInputChannels << " channels at " << m_nSampleRate << " Hz";
    }
    else
    {
        m_nSampleRate = 48000;
        m_nInputChannels = 0;
        pmlLog(pml::LOG_ERROR) << "RTP Client\tNo Input Channels";
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


void RtpThread::StreamShutdown()
{
    m_pRtspClient = nullptr;    //to stop us calling shutdownStream again
}
