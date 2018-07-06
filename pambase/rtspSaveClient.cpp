#include "rtspSaveClient.h"

wxDEFINE_EVENT(wxEVT_SDP, wxCommandEvent);

rtspSaveClient* rtspSaveClient::createNew(UsageEnvironment& env, char const* rtspURL,wxEvtHandler* pHandler,
                                        int verbosityLevel, char const* applicationName, portNumBits tunnelOverHTTPPortNum)
{
    return new rtspSaveClient(env, rtspURL, pHandler, verbosityLevel, applicationName, tunnelOverHTTPPortNum);
}

rtspSaveClient::rtspSaveClient(UsageEnvironment& env, char const* rtspURL,wxEvtHandler* pHandler,
                             int verbosityLevel, char const* applicationName, portNumBits tunnelOverHTTPPortNum)
    : RTSPClient(env,rtspURL, verbosityLevel, applicationName, tunnelOverHTTPPortNum, -1),
    m_pHandler(pHandler)
{
}



void rtspSaveClient::SaveSDP(unsigned int nResult, const wxString& sResult)
{
    if(m_pHandler)
    {
        wxCommandEvent* pEvent = new wxCommandEvent(wxEVT_SDP);
        pEvent->SetClientData(reinterpret_cast<void*>(this));
        pEvent->SetString(sResult);
        pEvent->SetInt(nResult);
        wxQueueEvent(m_pHandler, pEvent);
    }
}
