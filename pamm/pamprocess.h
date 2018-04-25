#pragma once
#include <wx/process.h>
#include <wx/string.h>
#include <map>

class PamProcess : public wxProcess
{
public:
    PamProcess(wxEvtHandler* pHandler)
        : wxProcess(pHandler)
    {
        m_pHandler = pHandler;
        Redirect();
    }

    // instead of overriding this virtual function we might as well process the
    // event from it in the frame class - this might be more convenient in some
    // cases
    virtual void OnTerminate(int pid, int status);

protected:
    wxEvtHandler *m_pHandler;

};


DECLARE_EXPORTED_EVENT_TYPE(WXEXPORT, wxEVT_PROCESS_FINISHED,-1)

