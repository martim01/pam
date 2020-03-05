#pragma once
#include <string>
#include "liveMedia.hh"
#include "BasicUsageEnvironment.hh"
#include "streamclientstate.h"
#include "SIPClient.hh"
#include <wx/string.h>

class RtpThread;
class wxString;

class ourSIPClient
{
public:
    static ourSIPClient* createNew(UsageEnvironment& env,  const wxString& sipURL, RtpThread* pThread, int verbosityLevel = 0, char const* applicationName = NULL);



    RtpThread* GetHandler()
    {
        return m_pHandler;
    }

    void SetHandler(RtpThread* pThread)
    {
        m_pHandler = pThread;
    }

    void CountSubSession();
    size_t GetSubSessionCount();

    //void PlaySubsession(const wxString& sId, responseHandler* responseHandler);
    //void PauseSubsession(const wxString& sId, responseHandler* responseHandler);

    void GetSDPDescription();
    void SetupSubsession(MediaSubsession* subsession);

    StreamClientState scs;

protected:
    ourSIPClient(UsageEnvironment& env, const wxString& sipURL, RtpThread* pThread,int verbosityLevel, char const* applicationName);
    // called only by createNew();
    virtual ~ourSIPClient();

    SIPClient* m_pClient;
    RtpThread* m_pHandler;
    wxString m_sStreamURL;
    size_t m_nSubsessions;




};

