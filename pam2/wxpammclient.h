#include <wx/ipc.h>

class PammConnection;

class PammClient : public wxClient
{
    public:
        PammClient();
        virtual ~PammClient();
        bool Connect(const wxString& sHost, const wxString& sService, const wxString& sTopic);
        void Disconnect();
        wxConnectionBase *OnMakeConnection();
        bool IsConnected() { return m_pConnection != NULL; };

        void Poke();
        bool Restart();
private:

    PammConnection* m_pConnection;

};
