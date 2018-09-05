#include "rtpthread.h"
#include "rtsputil.h"
#include "audio.h"
#include <iostream>
#include <wx/log.h>
#include "PamUsageEnvironment.h"
#include "PamTaskScheduler.h"
#include "timedbuffer.h"
#include "smpte2110mediasession.h"
#include "wxsink.h"
#include "audioevent.h"

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

RtpThread::RtpThread(wxEvtHandler* pHandler, const wxString& sProg, const wxString& sUrl, unsigned int nBufferSize, bool bSaveSDPOnly) :
    m_pHandler(pHandler),
    m_sProgName(sProg),
    m_sUrl(sUrl),
    m_nBufferSize(nBufferSize),
    m_pCurrentBuffer(0),
    m_pRtspClient(0),
    m_bClosing(false),
    m_bSaveSDP(bSaveSDPOnly),
    m_nQosMeasurementIntervalMS(1000)
{
    m_eventLoopWatchVariable = 0;
    m_pCondition = new wxCondition(m_mutex);

}


void* RtpThread::Entry()
{
    std::cout << "RtpThread: 1" << std::endl;
    // Begin by setting up our usage environment:
    TaskScheduler* scheduler = PamTaskScheduler::createNew();
    m_penv = PamUsageEnvironment::createNew(*scheduler, m_pHandler);
    std::cout << "RtpThread: 2" << std::endl;

    if(m_sUrl.Before(wxT(':')) == wxT("rtsp"))
    {
        std::cout << "RtpThread: 3" << std::endl;
        if(openURL())
        {
            std::cout << "RtpThread: 4" << std::endl;
            // All subsequent activity takes place within the event loop:
            while(TestDestroy() == false && m_eventLoopWatchVariable == 0)
            {
                m_penv->taskScheduler().doEventLoop(&m_eventLoopWatchVariable);
            }

            wxLogDebug(wxT("TEST DESTROY = true"));
            m_bClosing = true;
            if(m_pRtspClient && m_eventLoopWatchVariable == 0)  //0 means stream has shutdown and is telling us to stop
            {
                shutdownStream(m_pRtspClient, 0);
            }
            std::cout << "RtpThread: 6" << std::endl;
        }
    }
    else
    {

        wxString sDescriptor(m_sUrl.AfterFirst(wxT('[')).BeforeFirst(wxT(']')));
        sDescriptor.Replace(wxT("`"), wxT("\n"));
        sDescriptor = sDescriptor.AfterFirst(wxT('\n'));
        string sSDP(sDescriptor.mb_str());

        *m_penv << "\nUsing SDP passed via SAP \n" << sSDP.c_str() << "\n";

        Smpte2110MediaSession* pSession = Smpte2110MediaSession::createNew(*m_penv, sSDP.c_str());
        if (pSession == NULL)
        {
            *m_penv << "Failed to create a MediaSession object from the SDP description: " << m_penv->getResultMsg() << "\n";
            return 0;
        }
        else
        {
            *m_penv << "Created MediaSession object\n";
        }

        //count number of subsessions
        unsigned int nCountAudio(0);
        unsigned int nCountVideo(0);
        MediaSubsessionIterator iterCount(*pSession);
        MediaSubsession* pSubsessionCount = NULL;
        while ((pSubsessionCount = iterCount.next()) != NULL)
        {
            if (strcmp(pSubsessionCount->codecName(), "L16") == 0 || strcmp(pSubsessionCount->codecName(), "L24") == 0) // 16 or 24-bit linear audio (RFC 3190)
            {
                nCountAudio++;
            }
            else if (strcmp(pSubsessionCount->codecName(), "RAW") == 0)
            {
                nCountVideo++;
            }
        }
        *m_penv << "---------------------------------------\n";
        *m_penv << "Number of AES67 Subsessions: " << nCountAudio << "\n";
        *m_penv << "Number of Video Subsessions: " << nCountVideo << "\n";
        *m_penv << "---------------------------------------\n";

        MediaSubsessionIterator iter(*pSession);
        Smpte2110MediaSubsession* subsession = NULL;
        while ((subsession = dynamic_cast<Smpte2110MediaSubsession*>(iter.next())) != NULL)
        {
            if (!subsession->initiate (0))
            {
                *m_penv << "Failed to initiate the subsession: " << m_penv->getResultMsg() << "\n";
            }
            else
            {
                subsession->sink = wxSink::createNew(*m_penv, *subsession, this);
                *m_penv << "Initiated the \"" << *subsession << "\" subsession (";
                if (subsession->rtcpIsMuxed())
                {
                    *m_penv << "client port " << subsession->clientPortNum();
                }
                else
                {
                    *m_penv << "client ports " << subsession->clientPortNum() << "-" << subsession->clientPortNum()+1;
                }
                *m_penv << ")\n";

                *m_penv << "SessionId: " << subsession->GetEndpoint() << "\n";
                if (subsession->sink == NULL)
                {
                    *m_penv << "Failed to create a data sink for the subsession: " << m_penv->getResultMsg() << "\n";
                }
                else
                {
                    *m_penv << "Created a data sink for the \"" << *subsession << "\" subsession\n";

                    // @todo move the startPlaying to later??
                    subsession->sink->startPlaying(*subsession->readSource(), NULL, NULL);
                    beginQOSMeasurement(*m_penv, pSession, this);
                }
            }
        }
        PassSessionDetails(pSession);

        while(TestDestroy() == false && m_eventLoopWatchVariable == 0)
        {
            m_penv->taskScheduler().doEventLoop(&m_eventLoopWatchVariable);
        }
    }


    delete[] m_pCurrentBuffer;

    wxLogDebug(wxT("SEND Close event %s"), m_sUrl.c_str());
    wxCommandEvent* pEvent = new wxCommandEvent(wxEVT_RTP_SESSION_CLOSED);
    pEvent->SetString(m_sUrl);
    wxQueueEvent(m_pHandler, pEvent);



    return 0;

}


bool RtpThread::openURL()
{
    // Begin by creating a "RTSPClient" object.  Note that there is a separate "RTSPClient" object for each stream that we wish
    // to receive (even if more than stream uses the same "rtsp://" URL).
    m_sUrl.Replace(wxT(" "), wxT("%20"));
    m_pRtspClient = ourRTSPClient::createNew((*m_penv), m_sUrl.mb_str(), this, 1, m_sProgName.mb_str());
    if (m_pRtspClient == NULL)
    {
        (*m_penv) << "Failed to create a RTSP client for URL \"" << m_sUrl.mb_str() << "\": " << (*m_penv).getResultMsg() << "\n";
        return false;
    }


    // Next, send a RTSP "DESCRIBE" command, to get a SDP description for the stream.
    // Note that this command - like all RTSP commands - is sent asynchronously; we do not block, waiting for a response.
    // Instead, the following function call returns immediately, and we handle the RTSP response later, from within the event loop:
    if(m_bSaveSDP)
    {
        m_pRtspClient->sendDescribeCommand(saveAfterDESCRIBE);
    }
    else
    {
        m_pRtspClient->sendDescribeCommand(continueAfterDESCRIBE);
    }
    return true;
}

pairTime_t RtpThread::ConvertDoubleToPairTime(double dTime)
{
    double dInt, dDec;
    dDec = modf(dTime, &dInt);
    return make_pair(static_cast<unsigned int>(dInt), static_cast<unsigned int>(dDec*1000000.0));
}

void RtpThread::AddFrame(const wxString& sEndpoint, unsigned long nSSRC, const pairTime_t& timePresentation, unsigned long nFrameSize, u_int8_t* pFrameBuffer, u_int8_t nBytesPerSample, const pairTime_t& timeTransmission, unsigned int nTimestamp,unsigned int nDuration, mExtension_t* pExt)
{
    if(!m_bClosing)
    {
        if(m_Session.GetCurrentSubsession() != m_Session.lstSubsession.end() && m_Session.GetCurrentSubsession()->sSourceAddress == sEndpoint)
        {

            if(m_pCurrentBuffer == 0)
            {
                m_pCurrentBuffer = new float[m_nBufferSize*m_nInputChannels];
                m_nSampleBufferSize = 0;
            }


            if(nBytesPerSample == 2)    //16 bit
            {
                for(int i = 0; i < nFrameSize; i+=2)
                {
                    int nSample = (static_cast<int>(pFrameBuffer[i+1]) << 16) | (static_cast<int>(pFrameBuffer[i]) << 24);
                    float dSample = static_cast<float>(nSample);
                    dSample /= 2147483648.0;

                    if(m_nSampleBufferSize == 0)
                    {
                        m_dTransmission = timeTransmission.first + (static_cast<double>(timeTransmission.second))/1000000.0;
                        m_dPresentation = timePresentation.first + (static_cast<double>(timePresentation.second))/1000000.0;

                        m_nTimestamp = nTimestamp;
                    }
                    else
                    {
                        ++m_nTimestamp; //timestamp goes up 1 per sample
                        m_dTransmission += (1.0 / 48000.0); //@todo assuming 48K here
                        m_dPresentation += (1.0 / 48000.0); //@todo assuming 48K here
                    }

                    m_pCurrentBuffer[m_nSampleBufferSize] = dSample;
                    ++m_nSampleBufferSize;

                    if(m_nSampleBufferSize == m_nBufferSize*m_nInputChannels)   //filled up buffer
                    {
                        timedbuffer* pTimedBuffer = new timedbuffer(m_nBufferSize*m_nInputChannels, ConvertDoubleToPairTime(m_dPresentation), m_nTimestamp);
                        pTimedBuffer->SetBuffer(m_pCurrentBuffer);

                        pTimedBuffer->SetTransmissionTime(ConvertDoubleToPairTime(m_dTransmission));
                        pTimedBuffer->SetDuration(nDuration);

                        AudioEvent* pEvent = new AudioEvent(pTimedBuffer, AudioEvent::RTP, m_nBufferSize, 48000, false, false);
                        wxQueueEvent(m_pHandler, pEvent);

                        m_nSampleBufferSize = 0;

                    }
                }
            }
            else if(nBytesPerSample == 3)   //24 bit
            {
                for(int i = 0; i < nFrameSize; i+=3)
                {
                    int nSample = (static_cast<int>(pFrameBuffer[i+2]) << 8) | (static_cast<int>(pFrameBuffer[i+1]) << 16) | (static_cast<int>(pFrameBuffer[i]) << 24);
                    float dSample = static_cast<float>(nSample);
                    dSample /= 2147483648.0;

                    if(m_nSampleBufferSize == 0)
                    {
                        m_dTransmission = timeTransmission.first + (static_cast<double>(timeTransmission.second))/1000000.0;
                        m_dPresentation = timePresentation.first + (static_cast<double>(timePresentation.second))/1000000.0;
                        m_nTimestamp = nTimestamp;

                    }
                    else
                    {
                        ++m_nTimestamp; //timestamp goes up 1 per sample
                        m_dTransmission += (1.0/ 48000.0);
                        m_dPresentation += (1.0/ 48000.0);

                    }

                    m_pCurrentBuffer[m_nSampleBufferSize] = dSample;
                    ++m_nSampleBufferSize;

                    if(m_nSampleBufferSize == m_nBufferSize*m_nInputChannels)   //filled up buffer assuming two channels
                    {

                        timedbuffer* pTimedBuffer = new timedbuffer(m_nBufferSize*m_nInputChannels, ConvertDoubleToPairTime(m_dPresentation), m_nTimestamp);
                        pTimedBuffer->SetBuffer(m_pCurrentBuffer);
                        pTimedBuffer->SetTransmissionTime(ConvertDoubleToPairTime(m_dTransmission));

                        pTimedBuffer->SetDuration(nDuration);

                        AudioEvent* pEvent = new AudioEvent(pTimedBuffer, AudioEvent::RTP, m_nBufferSize, 48000, false, false);

                        wxQueueEvent(m_pHandler, pEvent);

                        m_nSampleBufferSize = 0;
                    }
                }
            }
        }
    }
}



void RtpThread::StopStream()
{
    (*m_penv) << "------------------------ Stop Stream \n";
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
    if(m_pHandler)
    {
        wxCommandEvent* pEvent = new wxCommandEvent(wxEVT_QOS_UPDATED);
        pEvent->SetClientData((void*)pData);
        wxQueueEvent(m_pHandler, pEvent);
    }
}


void RtpThread::PassSessionDetails(Smpte2110MediaSession* pSession)
{
    m_Session = session();

    m_Session.sName = wxString::FromAscii(pSession->sessionName());
    m_Session.sRawSDP = pSession->GetRawSDP();
    m_Session.sType = wxString::FromAscii(pSession->mediaSessionType());
    m_Session.refClock = pSession->GetRefClock();
    m_Session.sDescription = wxString::FromAscii(pSession->sessionDescription());
    m_Session.sGroups = pSession->GetGroupDup();


    MediaSubsessionIterator iterSub(*pSession);
    Smpte2110MediaSubsession* pSubsession = NULL;
    while ((pSubsession = dynamic_cast<Smpte2110MediaSubsession*>(iterSub.next())) != NULL)
    {
        refclk clock = pSubsession->GetRefClock();
        wxLogDebug(wxT("Subsession: %s  %s"), wxString::FromAscii(pSubsession->mediumName()).c_str(), wxString::FromAscii(pSubsession->protocolName()).c_str());
        m_Session.lstSubsession.push_back(subsession(wxString::FromAscii(pSubsession->sessionId()),
                                                     wxString::FromAscii(pSubsession->GetEndpoint()),
                                                     wxString::FromAscii(pSubsession->mediumName()),
                                                     wxString::FromAscii(pSubsession->codecName()),
                                                     wxString::FromAscii(pSubsession->protocolName()),
                                                     pSubsession->clientPortNum(),
                                                     pSubsession->rtpTimestampFrequency(),
                                                     pSubsession->numChannels(),
                                                     wxEmptyString,  /* @todo this is the channel list from SMPTE2110 */
                                                     pSubsession->GetSyncTime(),
                                                     pSubsession->GetLastEpoch(),
                                                     pSubsession->GetRefClock()));
    }

    m_Session.SetCurrentSubsession();
    if(m_Session.GetCurrentSubsession() != m_Session.lstSubsession.end())
    {
        m_nInputChannels = min((unsigned int)256 ,m_Session.GetCurrentSubsession()->nChannels);
    }
    else
    {

        m_nInputChannels = 0;
    }

    if(m_pHandler)
    {
        wxCommandEvent* pEvent = new wxCommandEvent(wxEVT_RTP_SESSION);
        pEvent->SetClientData((void*)&m_Session);
        wxQueueEvent(m_pHandler, pEvent);
    }
}


void RtpThread::SaveSDP(unsigned int nResult, const wxString& sResult)
{
    if(m_pHandler)
    {
        wxCommandEvent* pEvent = new wxCommandEvent(wxEVT_SDP);
        pEvent->SetClientData(reinterpret_cast<void*>(this));
        pEvent->SetString(sResult);
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
