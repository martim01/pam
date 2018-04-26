#pragma once
#include <wx/event.h>
#include <wx/ipc.h>
#include <wx/datetime.h>

class PamServer;
class PamConnection : public wxConnection
{
    public:
        PamConnection(PamServer* pServer);
        ~PamConnection(){}

        bool OnPoke (const wxString& sTopic, const wxString& sItem, const void *data, size_t size, wxIPCFormat format);
        bool OnExec(const wxString& sTopic, const wxString& sCommand);
        const wxDateTime& GetLastPoke();
        bool OnDisconnect();
        bool IsConnected();
    private:
        PamServer* m_pServer;
        wxDateTime m_dtLastPoke;
};

DECLARE_EXPORTED_EVENT_TYPE(WXEXPORT, wxEVT_IPC_RESTART,-1)
