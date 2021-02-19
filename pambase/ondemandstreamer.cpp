#include "ondemandstreamer.h"
#include "PamTaskScheduler.h"
#include "liveMedia.hh"
#include "BasicUsageEnvironment.hh"
#include <thread>
#include <wx/log.h>
#include "PamUsageEnvironment.h"
#include "GroupsockHelper.hh"
#include <chrono>
#include <iomanip>
#include <sstream>
#include "ondemandpamsubsession.h"
#include "log.h"
#include "pamRTSPServer.h"

wxDEFINE_EVENT(wxEVT_ODS_ANNOUNCE, wxCommandEvent);
wxDEFINE_EVENT(wxEVT_ODS_CONNECTION, wxCommandEvent);
wxDEFINE_EVENT(wxEVT_ODS_DISCONNECTION, wxCommandEvent);
wxDEFINE_EVENT(wxEVT_ODS_FINISHED, wxCommandEvent);




OnDemandStreamer::OnDemandStreamer(const std::set<wxEvtHandler*>& setRTSPHandlers, const std::set<wxEvtHandler*>& setRTCPHandlers,  const wxString& sRTSPAddress, unsigned short nRtspPort) :
    wxThread(wxTHREAD_JOINABLE),
    m_setRTSPHandlers(setRTSPHandlers),
    m_setRTCPHandlers(setRTCPHandlers),
    m_sRtspAddress(sRTSPAddress),
    m_nRtspPort(nRtspPort),
    m_pSubsession(nullptr),
    m_eventLoopWatchVariable(0),
    m_pSMS(nullptr)
{
        // Begin by setting up our usage environment:
    m_pScheduler = PamTaskScheduler::createNew();
    m_pEnv = PamUsageEnvironment::createNew(*m_pScheduler,nullptr);
}



void* OnDemandStreamer::Entry()
{
    // Create the RTSP server:
//    SendingInterfaceAddr = our_inet_addr(std::string(m_sRtspAddress.mb_str()).c_str());

    PamRTSPServer* rtspServer = PamRTSPServer::createNew(*m_pEnv, m_nRtspPort, NULL);
    if (rtspServer == NULL)
    {
        pml::Log(pml::LOG_ERROR) << "RTP Server\tFailed to create RTSP server (unicast): " << m_pEnv->getResultMsg();
        return NULL;
    }

    if(m_pSubsession == nullptr)
    {
        pml::Log(pml::LOG_ERROR) << "RTP Server\tFailed to create RTSP server (unicast): No subsession defined";
        return NULL;
    }

    char const* descriptionString = wxGetHostName().c_str() + " PAM_"+m_pSubsession->GetStreamName();
    std::string sStreamName = "by-name/PAM_"+m_pSubsession->GetStreamName();

    pml::Log(pml::LOG_DEBUG) << "RTP Server\tStreamName = '" << sStreamName << "'";

    m_pSubsession->SetRTCPHandlers(m_setRTCPHandlers);
    m_pSubsession->SetRTSPHandlers(m_setRTSPHandlers);

    m_pSMS = ServerMediaSession::createNew(*m_pEnv, sStreamName.c_str(), sStreamName.c_str(), descriptionString);

    m_pSMS->addSubsession(m_pSubsession);
    rtspServer->addServerMediaSession(m_pSMS);


    AnnounceStream(rtspServer, m_pSMS, sStreamName);

    //GetSDP();

    // Also, attempt to create a HTTP server for RTSP-over-HTTP tunneling.
    // Try first with the default HTTP port (80), and then with the alternative HTTP
    // port numbers (8000 and 8080).

    if (rtspServer->setUpTunnelingOverHTTP(80) || rtspServer->setUpTunnelingOverHTTP(8000) || rtspServer->setUpTunnelingOverHTTP(8080))
    {
        pml::Log(pml::LOG_INFO) << "RTP Server\t(We use port " << rtspServer->httpServerPortNum() << " for optional RTSP-over-HTTP tunneling.)";
    }
    else
    {
        pml::Log(pml::LOG_WARN) << "RTP Server\t(RTSP-over-HTTP tunneling is not available.)";
    }


    while(m_eventLoopWatchVariable == 0)
    {
        m_pEnv->taskScheduler().doEventLoop(&m_eventLoopWatchVariable);
    }

    //gracefully close
    rtspServer->closeAllClientSessionsForServerMediaSession(sStreamName.c_str());
    Medium::close(rtspServer);

    return NULL;
}


void OnDemandStreamer::Stop()
{
    m_eventLoopWatchVariable = 1;
}

void OnDemandStreamer::AnnounceStream(RTSPServer* rtspServer, ServerMediaSession* sms,  const std::string& sStreamName)
{
    char* url = rtspServer->rtspURL(sms);


    UsageEnvironment& env = rtspServer->envir();
    pml::Log(pml::LOG_INFO) << "RTP Server\tPlay this stream using the URL \"" << url << "\"";

    wxMutexLocker ml(m_mutex);
    for(auto pHandler : m_setRTSPHandlers)
    {
        wxCommandEvent* pEvent = new wxCommandEvent(wxEVT_ODS_ANNOUNCE);
        pEvent->SetString(wxString::FromUTF8(url));
        wxQueueEvent(pHandler, pEvent);
    }

    delete[] url;
}


const std::string& OnDemandStreamer::GetSDP()
{
    return m_sSDP;
}

