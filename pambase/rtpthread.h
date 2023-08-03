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

        float ConvertFrameBufferToSample(u_int8_t* pFrameBuffer, u_int8_t nBytesPerSample);

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

        wxEvtHandler* m_pHandler;
        wxString m_sProgName;
        AoIPSource m_source;

        unsigned int m_nBufferSize;

        wxMutex m_mutex;
        wxCondition* m_pCondition;

        wxString m_sReceivingInterface;

        float* m_pCurrentBuffer;
        double m_dTransmission;
        double m_dPresentation;

        
        unsigned int m_nSampleRate;
        unsigned int m_nTimestampErrors;
        unsigned int m_nTimestampErrorsTotal;

        unsigned long m_nTimestamp;
        unsigned long m_nSampleBufferSize;

        UsageEnvironment* m_penv;

        RTSPClient* m_pRtspClient;
        ourSIPClient* m_pSipClient;
        Smpte2110MediaSession* m_pSession;
        unsigned int m_nInputChannels;

        char m_eventLoopWatchVariable;

        bool m_bClosing;
        bool m_bSaveSDP;
        session m_Session;

        std::string m_sDescriptor;
        unsigned long m_nQosMeasurementIntervalMS;


        std::map<wxString, std::list<std::shared_ptr<const rtpFrame>>> m_mRedundantBuffers;
        std::map<wxString, uint64_t> m_mStreamUsage;
        std::map<wxString, uint64_t> m_mFramesReceived;
        uint64_t m_nTotalFramesPlayed = 0;
        bool m_bFrameExtracted = false;
        timeval m_tvLastExtracted;
        size_t m_nRedundantBufferQueue = 10;
};


DECLARE_EXPORTED_EVENT_TYPE(PAMBASE_IMPEXPORT, wxEVT_QOS_UPDATED,-1)
DECLARE_EXPORTED_EVENT_TYPE(PAMBASE_IMPEXPORT, wxEVT_RTP_SESSION,-1)
DECLARE_EXPORTED_EVENT_TYPE(PAMBASE_IMPEXPORT, wxEVT_RTP_SESSION_CLOSED,-1)
DECLARE_EXPORTED_EVENT_TYPE(PAMBASE_IMPEXPORT, wxEVT_SDP,-1)
DECLARE_EXPORTED_EVENT_TYPE(PAMBASE_IMPEXPORT, wxEVT_RTP_SESSION_EPOCH,-1)

