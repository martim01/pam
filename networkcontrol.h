#pragma once

class NetworkControl
{
    public:
        static NetworkControl* Get();

        wxString SetupNetworking();
        bool DeleteNetworking();

//        bool HasAdminRights();

    protected:
        NetworkControl() : m_nNTEContext(0)
        {

        }

        unsigned long m_nNTEContext;


};
