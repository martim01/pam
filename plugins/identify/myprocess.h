#pragma once
#include <wx/process.h>
#include <wx/string.h>
#include <map>

class MyProcess : public wxProcess
{
public:
    MyProcess(wxEvtHandler* pHandler, const wxString& sCmd)
        : wxProcess(pHandler), m_sCmd(sCmd)
    {
        m_pHandler = pHandler;
        Redirect();
    }

    wxString GetResultAttribute(const wxString& sSAttribute);

    // instead of overriding this virtual function we might as well process the
    // event from it in the frame class - this might be more convenient in some
    // cases
    virtual void OnTerminate(int pid, int status);
    bool GetInput();

protected:
    wxEvtHandler *m_pHandler;
    wxString m_sCmd;

    std::map<wxString, wxString> m_mResult;

};


DECLARE_EXPORTED_EVENT_TYPE(WXEXPORT, wxEVT_PROCESS_FINISHED,-1)
