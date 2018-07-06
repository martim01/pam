#pragma once

#include <wx/event.h>

#include "liveMedia.hh"
#include "BasicUsageEnvironment.hh"
#include "streamclientstate.h"


class rtspSaveClient: public RTSPClient
{
public:
    static rtspSaveClient* createNew(UsageEnvironment& env, char const* rtspURL, wxEvtHandler* pHandler,
                                    int verbosityLevel = 0,
                                    char const* applicationName = NULL,
                                    portNumBits tunnelOverHTTPPortNum = 0);




    void SetHandler(wxEvtHandler* pHandler)
    {
        m_pHandler = pHandler;
    }

    void SaveSDP(unsigned int nResult, const wxString& sResult);


protected:
    rtspSaveClient(UsageEnvironment& env, char const* rtspURL, wxEvtHandler* pHandler,
                  int verbosityLevel, char const* applicationName, portNumBits tunnelOverHTTPPortNum);
    // called only by createNew();
    virtual ~rtspSaveClient(){}

    wxEvtHandler* m_pHandler;
public:
    StreamClientState scs;

};

wxDECLARE_EXPORTED_EVENT(WXEXPORT, wxEVT_SDP, wxCommandEvent);
