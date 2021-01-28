#pragma once
#include <wx/thread.h>
#include <wx/event.h>
#include "dlldefine.h"
#include "OnDemandServerMediaSubsession.hh"


class PamUsageEnvironment;
class RTSPServer;
class ServerMediaSession;
class OnDemandPamSubsession;



class PAMBASE_IMPEXPORT OnDemandStreamer : public wxThread
{
    public:
        OnDemandStreamer(wxEvtHandler* pHandler, const wxString& sRTSPAddress, unsigned short nRTSPPort);
        PamUsageEnvironment* envir()
        {
            return m_pEnv;

        }

        void SetSubsession(OnDemandPamSubsession* pSubsession)
        {
            m_pSubsession = pSubsession;
        }

        void* Entry();
        void Stop();


        const std::string& GetSDP();
    protected:
        void AnnounceStream(RTSPServer* rtspServer, ServerMediaSession* sms,  const std::string& sStreamName);



        wxEvtHandler* m_pHandler;
        wxString m_sRtspAddress;
        unsigned short m_nRtspPort;
        OnDemandPamSubsession* m_pSubsession;

        TaskScheduler* m_pScheduler;
        PamUsageEnvironment* m_pEnv;
        char m_eventLoopWatchVariable;

        std::string m_sSDP;
        ServerMediaSession* m_pSMS;


};

wxDECLARE_EXPORTED_EVENT(PAMBASE_IMPEXPORT, wxEVT_ODS_ANNOUNCE, wxCommandEvent);
wxDECLARE_EXPORTED_EVENT(PAMBASE_IMPEXPORT, wxEVT_ODS_CONNECTION, wxCommandEvent);
wxDECLARE_EXPORTED_EVENT(PAMBASE_IMPEXPORT, wxEVT_ODS_DISCONNECTION, wxCommandEvent);
wxDECLARE_EXPORTED_EVENT(PAMBASE_IMPEXPORT, wxEVT_ODS_FINISHED, wxCommandEvent);

