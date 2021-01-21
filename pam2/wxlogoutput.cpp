#include "wxlogoutput.h"
#include <wx/log.h>


wxDEFINE_EVENT(wxEVT_PMLOG, wxCommandEvent);

void wxLogOutput::Flush(int nLogLevel, const std::stringstream&  logStream)
{
    if(m_pHandler)
    {
        wxString sLog(logStream.str());
        sLog.Trim();
        sLog.Trim(false);
        wxCommandEvent* pEvent = new wxCommandEvent(wxEVT_PMLOG);
        pEvent->SetInt(nLogLevel);
        pEvent->SetString(sLog);
        wxQueueEvent(m_pHandler, pEvent);
    }

    #ifndef NDEBUG
        std::cout << "LOGOUTPUT:     " << logStream.str() << std::endl;
    #endif // __WXDEBUG__

}
