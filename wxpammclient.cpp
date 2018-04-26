#include "wxpammclient.h"
#include "wxpammconnection.h"
#include <wx/log.h>
#include <wx/utils.h>
#include <wx/datetime.h>

PammClient::PammClient() : wxClient()
{
    m_pConnection = NULL;
}

bool PammClient::Connect(const wxString& sHost, const wxString& sService, const wxString& sTopic)
{
    // suppress the log messages from MakeConnection()
    //wxLogNull nolog;
    wxLogMessage(wxT("Attempt to connect to %s"), sService.c_str());

    m_pConnection = dynamic_cast<PammConnection*>(MakeConnection(wxGetHostName(), sService, sTopic));
    if ( !m_pConnection )
    {
        wxLogMessage(wxT("Failed to connect"));
        return false;
    }


    return true;
}

wxConnectionBase *PammClient::OnMakeConnection()
{
    return new PammConnection();
}

void PammClient::Disconnect()
{
    if (m_pConnection)
    {
        m_pConnection->Disconnect();
        wxDELETE(m_pConnection);
    }
}

PammClient::~PammClient()
{
    Disconnect();
}


void PammClient::Poke()
{
    if(m_pConnection)
    {
        wxString s = wxDateTime::Now().Format();
        m_pConnection->Poke("Date", s);
    }
}

bool PammClient::Restart()
{
    return (m_pConnection && m_pConnection->Execute(wxT("Restart")));
}
