#include "ondemandstreamer.h"
#include "PamTaskScheduler.h"
#include "liveMedia.hh"
#include "BasicUsageEnvironment.hh"
#include <thread>
#include <wx/log.h>
#include "PamUsageEnvironment.h"


wxDEFINE_EVENT(wxEVT_ODS_ANNOUNCE, wxCommandEvent);
wxDEFINE_EVENT(wxEVT_ODS_CONNECTION, wxCommandEvent);
wxDEFINE_EVENT(wxEVT_ODS_DISCONNECTION, wxCommandEvent);




OnDemandStreamer::OnDemandStreamer(wxEvtHandler* pHandler, unsigned short nRtspPort) :
    m_pHandler(pHandler),
    m_nRtspPort(nRtspPort),
    m_pSubsession(0),
    m_eventLoopWatchVariable(0)
{
    // Begin by setting up our usage environment:
    m_pScheduler = PamTaskScheduler::createNew();
    m_pEnv = PamUsageEnvironment::createNew(*m_pScheduler,nullptr);
}



void* OnDemandStreamer::Entry()
{
    // Create the RTSP server:
    RTSPServer* rtspServer = RTSPServer::createNew(*m_pEnv, m_nRtspPort, NULL);
    if (rtspServer == NULL)
    {
        *m_pEnv << "Failed to create RTSP server: " << m_pEnv->getResultMsg() << "\n";
        return NULL;
    }

    char const* descriptionString = "Session streamed by \"testOnDemandRTSPServer\"";

    char const* streamName = "PAM";
    ServerMediaSession* sms = ServerMediaSession::createNew(*m_pEnv, streamName, streamName, descriptionString);

    sms->addSubsession(m_pSubsession);
    rtspServer->addServerMediaSession(sms);

    AnnounceStream(rtspServer, sms, streamName);

    // Also, attempt to create a HTTP server for RTSP-over-HTTP tunneling.
    // Try first with the default HTTP port (80), and then with the alternative HTTP
    // port numbers (8000 and 8080).

    if (rtspServer->setUpTunnelingOverHTTP(80) || rtspServer->setUpTunnelingOverHTTP(8000) || rtspServer->setUpTunnelingOverHTTP(8080))
    {
        *m_pEnv << "\n(We use port " << rtspServer->httpServerPortNum() << " for optional RTSP-over-HTTP tunneling.)\n";
    }
    else
    {
        *m_pEnv << "\n(RTSP-over-HTTP tunneling is not available.)\n";
    }

    while(m_eventLoopWatchVariable == 0)
    {
        m_pEnv->taskScheduler().doEventLoop(&m_eventLoopWatchVariable);
    }

    return NULL;
}

void OnDemandStreamer::Stop()
{
    m_eventLoopWatchVariable = 1;
}

void OnDemandStreamer::AnnounceStream(RTSPServer* rtspServer, ServerMediaSession* sms,  char const* streamName)
{
    char* url = rtspServer->rtspURL(sms);


    UsageEnvironment& env = rtspServer->envir();
    env << "\n\"" << streamName << "\" stream\n";
    env << "Play this stream using the URL \"" << url << "\"\n";

    if(m_pHandler)
    {
        wxCommandEvent* pEvent = new wxCommandEvent(wxEVT_ODS_ANNOUNCE);
        pEvent->SetString(wxString::FromUTF8(url));
        wxQueueEvent(m_pHandler, pEvent);
    }

    delete[] url;
}
