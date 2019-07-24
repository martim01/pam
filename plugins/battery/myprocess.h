#pragma once
#include <wx/process.h>
#include <wx/string.h>
#include <map>
#include "json/json.h"

class MyProcess : public wxProcess
{
public:
    MyProcess(wxEvtHandler* pHandler)
        : wxProcess(pHandler)
    {
        m_pHandler = pHandler;
        Redirect();
    }

    wxString GetResultAttribute(const wxString& sSAttribute);

    virtual void OnTerminate(int pid, int status);

    wxString GetStatus();
    wxString GetPower();
    wxString GetError();
    double GetCharge();
    bool IsProcessOk();

protected:

    bool GetInput();

    wxEvtHandler *m_pHandler;


    Json::Value m_jsStatus;
    Json::Value m_jsCharge;
    bool m_bErrors;
};


DECLARE_EXPORTED_EVENT_TYPE(WXEXPORT, wxEVT_PROCESS_FINISHED,-1)
