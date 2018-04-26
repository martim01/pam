#include "wxpamconnection.h"
#include "wxpamserver.h"
#include <wx/app.h>

DEFINE_EVENT_TYPE(wxEVT_IPC_RESTART)

PamConnection::PamConnection(PamServer* pServer) : wxConnection(),
    m_pServer(pServer),
    m_dtLastPoke(wxDateTime::Now())
{

}

bool PamConnection::OnPoke(const wxString& sTopic, const wxString& sItem, const void *data, size_t size, wxIPCFormat format)
{
    m_dtLastPoke = wxDateTime::Now();
    return true;
}

const wxDateTime& PamConnection::GetLastPoke()
{
    return m_dtLastPoke;
}


bool PamConnection::OnDisconnect()
{
    m_pServer->Disconnect();
    return true;
}


bool PamConnection::OnExec(const wxString& sTopic, const wxString& sCommand)
{
    if(sTopic == wxT("Restart") || sCommand == wxT("Restart"))
    {
        wxCommandEvent* pEvent = new wxCommandEvent(wxEVT_IPC_RESTART);
        wxQueueEvent((wxEvtHandler*)wxTheApp->GetTopWindow(), pEvent);
    }
}
