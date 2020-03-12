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

struct qosData;
class Smpte2110MediaSession;

class PAMBASE_IMPEXPORT RtpThread : public wxThread
{
    public:
        RtpThread(wxEvtHandler* pHandler, const wxString& sReceivingInterface, const wxString& sProg, const AoIPSource& source, unsigned int nBufferSize, bool bSaveSDPOnly = false);
        void* Entry();
        void AddFrame(const wxString& sEndpoint, unsigned long nSSRC, const pairTime_t& timePresentation, unsigned long nFrameSize, u_int8_t* pBuffer, u_int8_t nBits, const pairTime_t& timeTransmission, unsigned int nTimestamp,unsigned int nDuration, int nTimestampDifference, mExtension_t* pExt);

        void MasterClockChanged();


        void StopStream();

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

        void QosUpdated(qosData* pData);

        void PassSessionDetails(Smpte2110MediaSession* pSession);

        float ConvertFrameBufferToSample(u_int8_t* pFrameBuffer, u_int8_t nBytesPerSample);

        bool DoRTSP();
        bool DoSIP();

        void SaveSDP(unsigned int nResult, const wxString& sResult);

        pairTime_t ConvertDoubleToPairTime(double dTime);
        wxEvtHandler* m_pHandler;
        wxString m_sProgName;
        AoIPSource m_source;

        unsigned int m_nBufferSize;

        wxMutex m_mutex;
        wxCondition* m_pCondition;

        wxString m_sReceivingInterface;
        std::queue<frameBuffer> m_qBufferFrames;

        float* m_pCurrentBuffer;
        double m_dTransmission;
        double m_dPresentation;

        double m_dDelay0;
        double m_dTSDFMax;
        double m_dTSDFMin;
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

        wxString m_sDescriptor;
        unsigned long m_nQosMeasurementIntervalMS;


};


DECLARE_EXPORTED_EVENT_TYPE(PAMBASE_IMPEXPORT, wxEVT_QOS_UPDATED,-1)
DECLARE_EXPORTED_EVENT_TYPE(PAMBASE_IMPEXPORT, wxEVT_RTP_SESSION,-1)
DECLARE_EXPORTED_EVENT_TYPE(PAMBASE_IMPEXPORT, wxEVT_RTP_SESSION_CLOSED,-1)
DECLARE_EXPORTED_EVENT_TYPE(PAMBASE_IMPEXPORT, wxEVT_SDP,-1)
DECLARE_EXPORTED_EVENT_TYPE(PAMBASE_IMPEXPORT, wxEVT_RPT_SESSION_EPOCH,-1)

