#pragma once

class NetworkControl
{
    public:
        static NetworkControl& Get();

        wxString SetupNetworking(const wxString& sAddress, unsigned long nMask, const wxString& sGateway);
        bool DeleteNetworking();

        const wxString& GetAddress() const;
        const wxString& GetGateway() const;
        unsigned long GetMask() const;

        wxString ConvertMaskToAddress(unsigned long nMask);

//        bool HasAdminRights();

    protected:
        NetworkControl() : m_nMask(0), m_nNTEContext(0)
        {
            GetCurrentSettings();
        }

        void GetCurrentSettings();

        unsigned long m_nNTEContext;

        wxString m_sAddress;
        unsigned long m_nMask;
        wxString m_sGateway;
        wxString m_sDNS;

        #ifdef __WXGNU__
        static const wxString STR_ADDRESS;
        static const wxString STR_GATEWAY;
        static const wxString STR_DNS;
        #endif

};
