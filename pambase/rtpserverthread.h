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
class Smpte2110MediaSession;
class timedbuffer;

static void afterPlaying(void* pClientData);

class PAMBASE_IMPEXPORT RtpServerThread : public wxThread
{
    public:
        RtpServerThread(wxEvtHandler* pHandler, const wxString& sRTSP, unsigned int nRTSPPort, const wxString& sMulticast, unsigned int RTPPort, LiveAudioSource::enumPacketTime ePacketTime);

        bool SetupStream(const wxString& sStreamName, const wxString& sInfo, const wxString& sDescription);

        void* Entry();

        void StopStream();
        void AddSamples(const timedbuffer* pTimedBuffer);


        void FlushQueue();


    private:

        bool StartStream();
        void CloseStream();


        wxMutex m_mutex;
        wxEvtHandler* m_pHandler;
        wxString m_sRTSP;
        unsigned int m_nRTSPPort;
        wxString m_sMulticast;
        unsigned int m_nRTPPort;
        LiveAudioSource::enumPacketTime m_ePacketTime;

        UsageEnvironment* m_penv;
        char m_eventLoopWatchVariable;

        LiveAudioSource* m_pSource;
        SimpleRTPSink* m_pSink;
        RTCPInstance* m_pRtcpInstance;
        Groupsock* m_pRtpGroupsock;
        RTSPServer* m_pRtspServer;
        ServerMediaSession* m_pSms;

        bool m_bStreaming;
};

