#include "wxpamserver.h"
#include "wxpamconnection.h"
#include <wx/log.h>

PamServer::PamServer() : wxServer(),
m_pConnection(0)
{

}

PamServer::~PamServer()
{
    Disconnect();
}

wxConnectionBase* PamServer::OnAcceptConnection(const wxString& topic)
{
    if(!m_pConnection)
    {
        m_pConnection = new PamConnection(this);
    }
    else
    {
        return NULL;
    }

    return m_pConnection;
}

void PamServer::Disconnect()
{
    if (m_pConnection)
    {
        m_pConnection->Disconnect();
        wxDELETE(m_pConnection);
    }
}

bool PamServer::IsConnected()
{
    return (m_pConnection != NULL);
}


wxDateTime PamServer::GetLastPoke()
{
    if(m_pConnection)
    {
        return m_pConnection->GetLastPoke();
    }
    return wxDateTime(wxLongLong(0));
}
