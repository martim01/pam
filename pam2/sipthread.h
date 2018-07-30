#pragma once
#inclue "rtpthread.h"

class SipThread : public wxThread
{
    public:
        SipThread(wxEvtHandler* pHandler, const wxString& sProg, const wxString& sUrl, unsigned int nBufferSize);
        void* Entry();
        void AddFrame(const timeval& timePresentation, unsigned long nFrameSize, u_int8_t* pBuffer, u_int8_t nBits, double dNPT);

        bool ReadyToRead(unsigned long nMinSamplesBuffered);
        frameBuffer ReadNextBuffer();

        void StopStream();

        void SetToClose()
        {
            m_eventLoopWatchVariable = 1;
        }

        void QosUpdated(qosData* pData);

        void PassSessionDetails(const wxString& sSessionName, const wxString& sSessionType, const wxString& sMedium, const wxString& sCodec,const wxString& sProtocol, unsigned int nPort, unsigned int nFrequency, unsigned int nNumChannels);

        bool openURL();
        wxEvtHandler* m_pHandler;
        wxString m_sProgName;
        wxString m_sUrl;

        unsigned int m_nBufferSize;

        wxMutex m_mutex;
        wxCondition* m_pCondition;


        std::queue<frameBuffer> m_qBufferFrames;
        unsigned long m_nSamplesBuffered;

        float* m_pCurrentBuffer;
        unsigned long m_nSampleBufferSize;

        UsageEnvironment* m_penv;

        AES67Client* m_pAES67Client;

        char m_eventLoopWatchVariable;



};

