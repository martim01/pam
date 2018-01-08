#pragma once

#include <wx/thread.h>
#include <wx/string.h>
#include <wx/event.h>
#include "streamclientstate.h"
#include "ourRTSPClient.h"
#include "wxsink.h"
#include <queue>
#include "session.h"

struct qosData;
class Aes67MediaSession;

class RtpThread : public wxThread
{
    public:
        RtpThread(wxEvtHandler* pHandler, const wxString& sProg, const wxString& sUrl, unsigned int nBufferSize, bool bSaveSDPOnly = false);
        void* Entry();
        void AddFrame(const wxString& sEndpoint, unsigned long nSSRC, const pairTime_t& timePresentation, unsigned long nFrameSize, u_int8_t* pBuffer, u_int8_t nBits, const pairTime_t& timeTransmission, unsigned int nTimestamp,unsigned int nDuration, mExtension_t* pExt);



        void StopStream();

        void SetToClose()
        {
            m_eventLoopWatchVariable = 1;
        }

        void Signal()
        {
            m_pCondition->Signal();
        }

        void QosUpdated(qosData* pData);

        void PassSessionDetails(Aes67MediaSession* pSession);


        bool openURL();

        void SaveSDP(unsigned int nResult, const wxString& sResult);

        pairTime_t ConvertDoubleToPairTime(double dTime);
        wxEvtHandler* m_pHandler;
        wxString m_sProgName;
        wxString m_sUrl;

        unsigned int m_nBufferSize;

        wxMutex m_mutex;
        wxCondition* m_pCondition;


        std::queue<frameBuffer> m_qBufferFrames;

        float* m_pCurrentBuffer;
        double m_dTransmission;
        double m_dPresentation;
        unsigned long m_nTimestamp;
        unsigned long m_nSampleBufferSize;

        UsageEnvironment* m_penv;

        RTSPClient* m_pRtspClient;
        Aes67MediaSession* m_pSession;
        unsigned int m_nInputChannels;

        char m_eventLoopWatchVariable;

        bool m_bClosing;
        bool m_bSaveSDP;
        session m_Session;

};


DECLARE_EXPORTED_EVENT_TYPE(WXEXPORT, wxEVT_QOS_UPDATED,-1)
DECLARE_EXPORTED_EVENT_TYPE(WXEXPORT, wxEVT_RTP_SESSION,-1)
DECLARE_EXPORTED_EVENT_TYPE(WXEXPORT, wxEVT_RTP_SESSION_CLOSED,-1)
DECLARE_EXPORTED_EVENT_TYPE(WXEXPORT, wxEVT_SDP,-1)

