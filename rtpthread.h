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


class RtpThread : public wxThread
{
    public:
        RtpThread(wxEvtHandler* pHandler, const wxString& sProg, const wxString& sUrl, unsigned int nBufferSize);
        void* Entry();
        void AddFrame(const pairTime_t& timePresentation, unsigned long nFrameSize, u_int8_t* pBuffer, u_int8_t nBits, const pairTime_t& timeTransmission, unsigned int nTimestamp,unsigned int nDuration);

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

        void PassSessionDetails(const wxString& sSessionName, const wxString& sEndpoint, const wxString& sSessionType, const wxString& sMedium, const wxString& sCodec,const wxString& sProtocol, unsigned int nPort, unsigned int nFrequency, unsigned int nNumChannels, unsigned int nSyncTimestamp, const pairTime_t& tvEpoch);

        bool openURL();

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
        unsigned int m_nInputChannels;

        char m_eventLoopWatchVariable;

        bool m_bClosing;

};


DECLARE_EXPORTED_EVENT_TYPE(WXEXPORT, wxEVT_QOS_UPDATED,-1)
DECLARE_EXPORTED_EVENT_TYPE(WXEXPORT, wxEVT_RTP_SESSION,-1)
DECLARE_EXPORTED_EVENT_TYPE(WXEXPORT, wxEVT_RTP_SESSION_CLOSED,-1)
