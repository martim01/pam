#pragma once

#include <wx/thread.h>
#include <wx/string.h>
#include <wx/event.h>
#include "liveMedia.hh"
#include "GroupsockHelper.hh"
#include "BasicUsageEnvironment.hh"
#include "AES67ServerMediaSubsession.h"
#include "liveaudiosource.h"
#include "dlldefine.h"

struct qosData;
class timedbuffer;

static void afterPlaying(void* pClientData);

class PAMBASE_IMPEXPORT MP3ServerThread : public wxThread
{
    public:
        MP3ServerThread(wxEvtHandler* pHandler, const wxString& sRTSP, unsigned int nRTSPPort, const wxString& sSourceIp, unsigned int RTPPort, bool bSSM);
        void* Entry();

        void StopStream();
        void AddSamples(const timedbuffer* pTimedBuffer);


        void FlushQueue();


    private:

        bool CreateStream();
        void CloseStream();


        wxMutex m_mutex;
        wxEvtHandler* m_pHandler;
        wxString m_sRTSP;
        unsigned int m_nRTSPPort;
        wxString m_sSourceIp;
        unsigned int m_nRTPPort;
        bool m_bSSM;

        UsageEnvironment* m_penv;
        char m_eventLoopWatchVariable;

        MP3LiveSource* m_pSource;
        MPEG1or2AudioRTPSink* m_pSink;
        RTCPInstance* m_pRtcpInstance;
        Groupsock* m_pRtpGroupsock;
        RTSPServer* m_pRtspServer;

        bool m_bStreaming;
};

