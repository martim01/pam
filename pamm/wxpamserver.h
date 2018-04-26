#pragma once
#include <wx/ipc.h>
#include <wx/datetime.h>

class PamConnection;

class PamServer : public wxServer
{
    public:
        PamServer();
        ~PamServer();

        wxConnectionBase* OnAcceptConnection (const wxString &topic);

        void Disconnect();
        bool IsConnected();

        wxDateTime GetLastPoke();

    private:
        PamConnection* m_pConnection;

};
