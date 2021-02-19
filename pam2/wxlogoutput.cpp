#include "wxlogoutput.h"
#include <wx/log.h>


wxDEFINE_EVENT(wxEVT_PMLOG, wxCommandEvent);

void wxLogOutput::Flush(pml::enumLevel eLogLevel, const std::stringstream&  logStream)
{
    if(m_pHandler)
    {
        wxString sLog(logStream.str());
        sLog.Trim();
        sLog.Trim(false);


        if(sLog.empty() == false)
        {

            wxCommandEvent* pEvent = new wxCommandEvent(wxEVT_PMLOG);
            pEvent->SetInt(eLogLevel);
            pEvent->SetString(sLog);
            wxQueueEvent(m_pHandler, pEvent);
        }
    }
}
