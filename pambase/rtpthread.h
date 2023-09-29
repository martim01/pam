#pragma once

#include <wx/thread.h>
#include <wx/string.h>
#include <wx/event.h>
#include "streamclientstate.h"
#include "ourRTSPClient.h"
#include "ourSIPClient.h"
#include "wxsink.h"
#include <queue>
#include "session.h"
#include "dlldefine.h"
#include "aoipsourcemanager.h"
#include <atomic>
#include <vector>
#include <list>
#include <map>

struct qosData;
class Smpte2110MediaSession;

class PAMBASE_IMPEXPORT RtpThread : public wxThread
{
    public:
        RtpThread(wxEvtHandler* pHandler, const wxString& sReceivingInterface, const wxString& sProg, const AoIPSource& source, unsigned int nBufferSize, bool bSaveSDPOnly = false);
        void* Entry();
        void AddFrame(std::shared_ptr<const rtpFrame> pFrame);

        void MasterClockChanged();



        void SetToClose()
        {
            m_eventLoopWatchVariable = 1;
        }


        void StreamFromSDP();

        void SetQosMeasurementIntervalMS(unsigned long nMilliseconds);
        unsigned long GetQosMeasurementIntervalMS();

        void Signal()
        {
            m_pCondition->Signal();
        }

        void SaveSDP(unsigned int nResult, const std::string& sResult);

        void QosUpdated(qosData* pData);






        void PassSessionDetails(Smpte2110MediaSession* pSession);

        std::pair<float, unsigned char> ConvertFrameBufferToSample(u_int8_t* pFrameBuffer, u_int8_t nBytesPerSample);

    protected:

        friend void shutdownStream(RTSPClient* rtspClient, int exitCode);
        void StopStream();

        bool DoRTSP();
        bool DoSIP();

        void StreamShutdown();


        timeval ConvertDoubleToPairTime(double dTime);

        void CreateSink(Smpte2110MediaSubsession* pSubsession);

        void HandleFrameAdded();

        void ConvertFrameToTimedBuffer(std::shared_ptr<const rtpFrame> pFrame);
        void WorkoutFirstFrame();
        void WorkoutNextFrame();

        wxEvtHandler* m_pHandler = nullptr;
        wxString m_sProgName;
        AoIPSource m_source;

        unsigned int m_nBufferSize = 4096;

        wxMutex m_mutex;
        wxCondition* m_pCondition = nullptr;

        wxString m_sReceivingInterface;

        std::vector<float> m_vCurrentBuffer;
        std::vector<unsigned char> m_vCurrentUserBits;

        
        double m_dTransmission = 0.0;
        double m_dPresentation = 0.0;

    
        unsigned int m_nSampleRate = 48000;
        unsigned int m_nTimestampErrors = 0;
        unsigned int m_nTimestampErrorsTotal = 0;

        unsigned long m_nTimestamp = 0;
        unsigned long m_nSampleBufferSize = 0;

        UsageEnvironment* m_penv = nullptr;

        RTSPClient* m_pRtspClient = nullptr;
        ourSIPClient* m_pSipClient = nullptr;
        Smpte2110MediaSession* m_pSession = nullptr;
        unsigned int m_nInputChannels = 2;

        char m_eventLoopWatchVariable= 0;

        bool m_bClosing = false;
        bool m_bSaveSDP = false;
        session m_Session;

        std::string m_sDescriptor;
        unsigned long m_nQosMeasurementIntervalMS = 1000;


        std::map<wxString, std::list<std::shared_ptr<const rtpFrame>>> m_mRedundantBuffers;
        std::map<wxString, uint64_t> m_mStreamUsage;
        std::map<wxString, uint64_t> m_mFramesReceived;
        uint64_t m_nTotalFramesPlayed = 0;
        bool m_bFrameExtracted = false;
        timeval m_tvLastExtracted;
        size_t m_nRedundantBufferQueue = 10;

        bool m_bFirstDebug = true;
};


DECLARE_EXPORTED_EVENT_TYPE(PAMBASE_IMPEXPORT, wxEVT_QOS_UPDATED,-1)
DECLARE_EXPORTED_EVENT_TYPE(PAMBASE_IMPEXPORT, wxEVT_RTP_SESSION,-1)
DECLARE_EXPORTED_EVENT_TYPE(PAMBASE_IMPEXPORT, wxEVT_RTP_SESSION_CLOSED,-1)
DECLARE_EXPORTED_EVENT_TYPE(PAMBASE_IMPEXPORT, wxEVT_SDP,-1)
DECLARE_EXPORTED_EVENT_TYPE(PAMBASE_IMPEXPORT, wxEVT_RTP_SESSION_EPOCH,-1)

