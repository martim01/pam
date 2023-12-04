#include "wxlogoutput.h"
#include <wx/log.h>


wxDEFINE_EVENT(wxEVT_PMLOG, wxCommandEvent);

void wxLogOutput::Flush(pml::enumLevel eLogLevel, const std::stringstream&  logStream, const std::string& sPrefix)
{
    if(m_pHandler && eLogLevel >= m_eLevel)
    {
        wxString sLog(logStream.str());
        sLog.Trim();
        sLog.Trim(false);


        if(sLog.empty() == false)
        {

            wxCommandEvent* pEvent = new wxCommandEvent(wxEVT_PMLOG);
            pEvent->SetInt(eLogLevel);
            pEvent->SetString(wxString(sPrefix)+"\t"+sLog);
            wxQueueEvent(m_pHandler, pEvent);
        }
    }
}
