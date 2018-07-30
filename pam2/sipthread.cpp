#include "sipthread.h"
#include "rtsputil.h"
#include "audio.h"
#include <iostream>
#include <wx/log.h>
#include "PamUsageEnvironment.h"
#include "PamTaskScheduler.h"
#include "timedbuffer.h"
using namespace std;


SipThread::SipThread(wxEvtHandler* pHandler, const wxString& sProg, const wxString& sUrl, unsigned int nBufferSize) :
    m_pHandler(pHandler),
    m_sProgName(sProg),
    m_sUrl(sUrl),
    m_nBufferSize(nBufferSize),
    m_nSamplesBuffered(0),
    m_pCurrentBuffer(0),
    m_pRtspClient(0)
{
    m_eventLoopWatchVariable = 0;
    m_pCondition = new wxCondition(m_mutex);
}


void* SipThread::Entry()
{

    // Begin by setting up our usage environment:
    TaskScheduler* scheduler = PamTaskScheduler::createNew();
    m_penv = PamUsageEnvironment::createNew(*scheduler);


    // There are argc-1 URLs: argv[1] through argv[argc-1].  Open and start streaming each one:
    if(openURL())
    {
        // All subsequent activity takes place within the event loop:
        while(TestDestroy() == false)
        {
            m_penv->taskScheduler().doEventLoop(&m_eventLoopWatchVariable);
        }

        if(m_pRtspClient)
        {
            dynamic_cast<ourRTSPClient*>(m_pRtspClient)->SetHandler(0);
            shutdownStream(m_pRtspClient, 0);
            m_pRtspClient = 0;
        }
    // This function call does not return, unless, at some point in time, "eventLoopWatchVariable" gets set to something non-zero.
    }

    delete[] m_pCurrentBuffer;
    (*m_penv) << "************************************** Closing thread \n";


    return 0;

}


bool SipThread::openURL()
{

    // First, trim any directory prefixes from "applicationName":
    m_pAES67Client = AES67Client::createNew(env, 96, "L24", verbosityLevel, "pam");
    if (ourClient == NULL)
    {
        *env << "Failed to create " << clientProtocolName << " client: " << env->getResultMsg() << "\n";
        shutdown();
    }

    setUserAgentString(userAgent);

    if (sendOptionsRequest)
    {
        // Begin by sending an "OPTIONS" command:
        getOptions(continueAfterOPTIONS);
    }
    else
    {
        // Next, get a SDP description for the stream:
        getSDPDescription(continueAfterDESCRIBE);
    }

    return true;
}


void SipThread::getSDPDescription(RTSPClient::responseHandler* afterFunc)
{
    extern char* proxyServerName;
    if (proxyServerName != NULL)
    {
        // Tell the SIP client about the proxy:
        NetAddressList addresses(proxyServerName);
        if (addresses.numAddresses() == 0)
        {
            m_pAESClient->envir() << "Failed to find network address for \"" << proxyServerName << "\"\n";
        }
        else
        {
            NetAddress address = *(addresses.firstAddress());
            unsigned proxyServerAddress  = *(unsigned*)(address.data());
            extern unsigned short proxyServerPortNum;
            if (proxyServerPortNum == 0)
            {
                proxyServerPortNum = 5060; // default
            }

            ourAES67Client->setProxyServer(proxyServerAddress, proxyServerPortNum);
        }
    }

    unsigned short desiredPortNum;
    unsigned short clientStartPortNum = desiredPortNum;
    if (clientStartPortNum == 0)
    {
        clientStartPortNum = 5000; // default
    }

    m_pAESClient->setClientStartPortNum(clientStartPortNum);

    char const* streamURL;
    result = ourAES67Client->invite(streamURL);

    int resultCode = result == NULL ? -1 : 0;
    afterFunc(NULL, resultCode, strDup(result));
}


void SipThread::AddFrame(const timeval& timePresentation, unsigned long nFrameSize, u_int8_t* pFrameBuffer, u_int8_t nBytesPerSample, double dNPT)
{

    m_qBufferFrames.push(frameBuffer(timePresentation, nFrameSize, pFrameBuffer, nBytesPerSample));

    m_nSamplesBuffered += ((nFrameSize/nBytesPerSample)/2);     // @todo assuming stereo here...


    if(m_pCurrentBuffer == 0)
    {
        m_pCurrentBuffer = new float[m_nBufferSize*2];
        m_nSampleBufferSize = 0;
    }

    if(nBytesPerSample == 2)    //16 bit
    {
        for(int i = 0; i < nFrameSize; i+=2)
        {
            int nSample = (static_cast<int>(pFrameBuffer[i+1]) << 16) | (static_cast<int>(pFrameBuffer[i]) << 24);
            float dSample = static_cast<float>(nSample);
            dSample /= 2147483648.0;


            m_pCurrentBuffer[m_nSampleBufferSize] = dSample;
            ++m_nSampleBufferSize;

            if(m_nSampleBufferSize == m_nBufferSize*2)   //filled up buffer
            {

                timedbuffer* pTimedBuffer = new timedbuffer(m_nBufferSize*2);
                pTimedBuffer->SetBuffer(m_pCurrentBuffer);
                pTimedBuffer->SetTimeVal(timePresentation);
                pTimedBuffer->SetNPT(dNPT);

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


            m_pCurrentBuffer[m_nSampleBufferSize] = dSample;
            ++m_nSampleBufferSize;

            if(m_nSampleBufferSize == m_nBufferSize*2)   //filled up buffer
            {

                timedbuffer* pTimedBuffer = new timedbuffer(m_nBufferSize*2);
                pTimedBuffer->SetTimeVal(timePresentation);
                pTimedBuffer->SetBuffer(m_pCurrentBuffer);
                pTimedBuffer->SetNPT(dNPT);

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


bool SipThread::ReadyToRead(unsigned long nMinSamplesBuffered)
{
    wxMutexLocker ml(m_mutex);

    return (m_nSamplesBuffered >= nMinSamplesBuffered);
}


frameBuffer SipThread::ReadNextBuffer()
{
    frameBuffer aFrame;

    wxMutexLocker ml(m_mutex);

    if(m_qBufferFrames.empty() == false)
    {
        aFrame = m_qBufferFrames.front();
        m_qBufferFrames.pop();
        m_nSamplesBuffered -= ((aFrame.nFrameSize/aFrame.nBytesPerSample)/aFrame.nChannels); // @todo assuming stereo here...
    }

    return aFrame;
}


void SipThread::StopStream()
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

void SipThread::QosUpdated(qosData* pData)
{
    if(m_pHandler)
    {
        wxCommandEvent event(wxEVT_QOS_UPDATED);
        event.SetClientData((void*)pData);
        wxPostEvent(m_pHandler, event);
    }
}


void SipThread::PassSessionDetails(const wxString& sSessionName, const wxString& sSessionType, const wxString& sMedium, const wxString& sCodec,const wxString& sProtocol, unsigned int nPort, unsigned int nFrequency, unsigned int nNumChannels)
{
    wxLogDebug(wxT("Frequency = %d"), nFrequency);
    if(m_pHandler)
    {
        session* pSession = new session(sSessionName, sSessionType.Before(wxT('\n')), sMedium, sCodec, sProtocol, nPort, nFrequency, nNumChannels);

        wxCommandEvent event(wxEVT_RTP_SESSION);
        event.SetClientData((void*)pSession);
        wxPostEvent(m_pHandler, event);
    }
}

