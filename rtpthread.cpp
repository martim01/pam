#include "rtpthread.h"
#include "rtsputil.h"
#include "audio.h"
#include <iostream>
#include <wx/log.h>
#include "PamUsageEnvironment.h"
#include "PamTaskScheduler.h"
#include "timedbuffer.h"
#include "aes67mediasession.h"
#include "wxsink.h"

using namespace std;

//// A function that outputs a string that identifies each subsession (for debugging output).  Modify this if you wish:
UsageEnvironment& operator<<(UsageEnvironment& env, const Aes67MediaSubsession& subsession)
{
    return env << subsession.mediumName() << "/" << subsession.codecName();
}


DEFINE_EVENT_TYPE(wxEVT_QOS_UPDATED)
DEFINE_EVENT_TYPE(wxEVT_RTP_SESSION)
DEFINE_EVENT_TYPE(wxEVT_RTP_SESSION_CLOSED)

RtpThread::RtpThread(wxEvtHandler* pHandler, const wxString& sProg, const wxString& sUrl, unsigned int nBufferSize) :
    m_pHandler(pHandler),
    m_sProgName(sProg),
    m_sUrl(sUrl),
    m_nBufferSize(nBufferSize),
    m_pCurrentBuffer(0),
    m_pRtspClient(0),
    m_bClosing(false)
{
    m_eventLoopWatchVariable = 0;
    m_pCondition = new wxCondition(m_mutex);
}


void* RtpThread::Entry()
{

    // Begin by setting up our usage environment:
    TaskScheduler* scheduler = PamTaskScheduler::createNew();
    m_penv = PamUsageEnvironment::createNew(*scheduler, m_pHandler);


    if(m_sUrl.Before(wxT(':')) == wxT("rtsp"))
    {
        if(openURL())
        {
            // All subsequent activity takes place within the event loop:
            while(TestDestroy() == false && m_eventLoopWatchVariable == 0)
            {
                m_penv->taskScheduler().doEventLoop(&m_eventLoopWatchVariable);
            }
            m_bClosing = true;
            if(m_pRtspClient && m_eventLoopWatchVariable == 0)  //0 means stream has shutdown and is telling us to stop
            {
                shutdownStream(m_pRtspClient, 0);
            }
        }
    }
    else
    {

        wxString sDescriptor(m_sUrl.AfterFirst(wxT('[')).BeforeFirst(wxT(']')));
        sDescriptor.Replace(wxT("\\n"), wxT("\n"));
        sDescriptor = sDescriptor.AfterFirst(wxT('\n'));
        string sSDP(sDescriptor.mb_str());

        *m_penv << "\nUsing SDP passed via SAP \n" << sSDP.c_str() << "\n";

        Aes67MediaSession* session = Aes67MediaSession::createNew(*m_penv, sSDP.c_str());
        if (session == NULL)
        {
            *m_penv << "Failed to create a MediaSession object from the SDP description: " << m_penv->getResultMsg() << "\n";
            return 0;
        }
        else
        {
            *m_penv << "Created MediaSession object\n";
        }

        MediaSubsessionIterator iter(*session);
        Aes67MediaSubsession* subsession = NULL;
        while ((subsession = dynamic_cast<Aes67MediaSubsession*>(iter.next())) != NULL)
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
                if (subsession->sink == NULL)
                {
                    *m_penv << "Failed to create a data sink for the subsession: " << m_penv->getResultMsg() << "\n";
                }
                else
                {
                    *m_penv << "Created a data sink for the \"" << *subsession << "\" subsession\n";

                    PassSessionDetails(wxString::FromAscii(session->sessionName()), subsession->GetEndpoint(), wxString::FromAscii(session->mediaSessionType()), wxString::FromAscii(subsession->mediumName()), wxString::FromAscii(subsession->codecName()), wxString::FromAscii(subsession->protocolName()), subsession->clientPortNum(), subsession->rtpTimestampFrequency(), subsession->numChannels(), subsession->GetSyncTime(), subsession->GetLastEpoch());

                    subsession->sink->startPlaying(*subsession->readSource(), NULL, NULL);
                    beginQOSMeasurement(*m_penv, session, this);
                }
            }
        }

        while(TestDestroy() == false && m_eventLoopWatchVariable == 0)
        {
            m_penv->taskScheduler().doEventLoop(&m_eventLoopWatchVariable);
        }
    }


    delete[] m_pCurrentBuffer;

    wxCommandEvent event(wxEVT_RTP_SESSION_CLOSED);
    event.SetString(m_sUrl);
    wxPostEvent(m_pHandler, event);



    return 0;

}


bool RtpThread::openURL()
{
    // Begin by creating a "RTSPClient" object.  Note that there is a separate "RTSPClient" object for each stream that we wish
    // to receive (even if more than stream uses the same "rtsp://" URL).
    m_pRtspClient = ourRTSPClient::createNew((*m_penv), m_sUrl.mb_str(), this, 1, m_sProgName.mb_str());
    if (m_pRtspClient == NULL)
    {
        (*m_penv) << "Failed to create a RTSP client for URL \"" << m_sUrl.mb_str() << "\": " << (*m_penv).getResultMsg() << "\n";
        return false;
    }


    // Next, send a RTSP "DESCRIBE" command, to get a SDP description for the stream.
    // Note that this command - like all RTSP commands - is sent asynchronously; we do not block, waiting for a response.
    // Instead, the following function call returns immediately, and we handle the RTSP response later, from within the event loop:
    m_pRtspClient->sendDescribeCommand(continueAfterDESCRIBE);

    return true;
}

pairTime_t RtpThread::ConvertDoubleToPairTime(double dTime)
{
    double dInt, dDec;
    dDec = modf(dTime, &dInt);
    return make_pair(static_cast<unsigned int>(dInt), static_cast<unsigned int>(dDec*1000000.0));
}

void RtpThread::AddFrame(const pairTime_t& timePresentation, unsigned long nFrameSize, u_int8_t* pFrameBuffer, u_int8_t nBytesPerSample, const pairTime_t& timeTransmission, unsigned int nTimestamp,unsigned int nDuration)
{

    if(!m_bClosing)
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

                    wxCommandEvent event(wxEVT_DATA);
                    event.SetId(0);
                    event.SetClientData(reinterpret_cast<void*>(pTimedBuffer));
                    event.SetInt(m_nBufferSize);
                    event.SetExtraLong(48000);  //@todo sample rate
                    wxPostEvent(m_pHandler, event);

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

                    wxCommandEvent event(wxEVT_DATA);
                    event.SetId(0);
                    event.SetClientData(reinterpret_cast<void*>(pTimedBuffer));
                    event.SetInt(m_nBufferSize);
                    event.SetExtraLong(48000);  //@todo sample rate
                    wxPostEvent(m_pHandler, event);

                    m_nSampleBufferSize = 0;
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
//    m_mutex.Lock();
//    m_pCondition->Wait();

}

void RtpThread::QosUpdated(qosData* pData)
{
    if(m_pHandler)
    {
        wxCommandEvent event(wxEVT_QOS_UPDATED);
        event.SetClientData((void*)pData);
        wxPostEvent(m_pHandler, event);
    }
}


void RtpThread::PassSessionDetails(const wxString& sSessionName, const wxString& sEndpoint, const wxString& sSessionType, const wxString& sMedium, const wxString& sCodec,const wxString& sProtocol, unsigned int nPort, unsigned int nFrequency, unsigned int nNumChannels, unsigned int nSyncTimestamp, const pairTime_t& tvEpoch)
{
    m_nInputChannels = nNumChannels;
    if(m_pHandler)
    {
        session* pSession = new session(sSessionName, sEndpoint, sSessionType.Before(wxT('\n')), sMedium, sCodec, sProtocol, nPort, nFrequency, nNumChannels, nSyncTimestamp, tvEpoch);


        wxCommandEvent event(wxEVT_RTP_SESSION);
        event.SetClientData((void*)pSession);
        wxPostEvent(m_pHandler, event);
    }
}

