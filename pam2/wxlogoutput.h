#pragma once
#include "log.h"
#include <wx/event.h>

class wxLogOutput : public pml::LogOutput
{
    public:
        wxLogOutput(wxEvtHandler* pHandler) : m_pHandler(pHandler){}
        void Flush(pml::enumLevel eLogLevel, const std::stringstream&  logStream, const std::string& sPrefix) override;
    private:
        wxEvtHandler* m_pHandler;
};


wxDECLARE_EXPORTED_EVENT(WXEXPORT, wxEVT_PMLOG, wxCommandEvent);
