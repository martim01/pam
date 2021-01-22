#pragma once
#include "log.h"
#include <wx/event.h>

class wxLogOutput : public pml::LogOutput
{
    public:
        wxLogOutput(wxEvtHandler* pHandler) : m_pHandler(pHandler){}
        void Flush(pml::Log::enumLevel eLogLevel, const std::stringstream&  logStream) override;
    private:
        wxEvtHandler* m_pHandler;
};


wxDECLARE_EXPORTED_EVENT(WXEXPORT, wxEVT_PMLOG, wxCommandEvent);
