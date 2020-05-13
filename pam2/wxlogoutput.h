#pragma once
#include "log.h"
#include <wx/event.h>

class wxLogOutput : public pml::LogOutput
{
    public:
        wxLogOutput(wxEvtHandler* pHandler) : m_pHandler(pHandler){}
        void Flush(int nLogLevel, const std::stringstream&  logStream);
    private:
        wxEvtHandler* m_pHandler;
};


wxDECLARE_EXPORTED_EVENT(WXEXPORT, wxEVT_PMLOG, wxCommandEvent);
