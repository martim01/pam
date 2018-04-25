#include "pamprocess.h"
#include <wx/txtstrm.h>
#include <wx/log.h>
#include <wx/tokenzr.h>

using namespace std;

DEFINE_EVENT_TYPE(wxEVT_PROCESS_FINISHED)

void PamProcess::OnTerminate(int nPid, int nStatus)
{
    if(m_pHandler)
    {
        wxCommandEvent event(wxEVT_PROCESS_FINISHED);
        event.SetInt(nPid);
        event.SetExtraLong(nStatus);
        wxPostEvent(m_pHandler, event);
    }

}


