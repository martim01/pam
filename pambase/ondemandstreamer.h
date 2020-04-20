#pragma once
#include <wx/thread.h>
#include <wx/event.h>
#include "dlldefine.h"
#include "OnDemandServerMediaSubsession.hh"
class PamUsageEnvironment;
class RTSPServer;
class ServerMediaSession;

class PAMBASE_IMPEXPORT OnDemandStreamer : public wxThread
{
    public:
        OnDemandStreamer(wxEvtHandler* pHandler, unsigned short nRTSPPort);
        PamUsageEnvironment* envir()
        {
            return m_pEnv;

        }

        void SetSubsession(OnDemandServerMediaSubsession* pSubsession)
        {
            m_pSubsession = pSubsession;
        }

        void* Entry();
        void Stop();
    protected:
        void AnnounceStream(RTSPServer* rtspServer, ServerMediaSession* sms,  char const* streamName);

        wxEvtHandler* m_pHandler;
        unsigned short m_nRtspPort;
        OnDemandServerMediaSubsession* m_pSubsession;

        TaskScheduler* m_pScheduler;
        PamUsageEnvironment* m_pEnv;
        char m_eventLoopWatchVariable;
};

wxDECLARE_EXPORTED_EVENT(PAMBASE_IMPEXPORT, wxEVT_ODS_ANNOUNCE, wxCommandEvent);
wxDECLARE_EXPORTED_EVENT(PAMBASE_IMPEXPORT, wxEVT_ODS_CONNECTION, wxCommandEvent);
wxDECLARE_EXPORTED_EVENT(PAMBASE_IMPEXPORT, wxEVT_ODS_DISCONNECTION, wxCommandEvent);

