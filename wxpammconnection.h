#include <wx/ipc.h>

class PammConnection : public wxConnection
{
public:
    PammConnection() : wxConnection(){}
    ~PammConnection(){}

};
