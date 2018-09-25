#pragma once
#include <map>

struct networkInterface
{
    networkInterface() : nMask(0), bStatic(false), bWireless(false), bUp(false){}
    wxString sAddress;
    unsigned long nMask;
    wxString sGateway;
    wxString sDNS;
    bool bStatic;
    bool bWireless;
    bool bUp;
    wxString sEssid;
    wxString sPassword;
};

class NetworkControl
{
    public:
        static NetworkControl& Get();

        wxString SetupNetworking(const wxString& sInterface, const wxString& sAddress, unsigned long nMask, wxString sGateway);
        bool DeleteNetworking();

        wxString GetAddress(const wxString& sInterface) const;
        wxString GetGateway(const wxString& sInterface) const;
        unsigned long GetMask(const wxString& sInterface) const;

        wxString ConvertMaskToAddress(unsigned long nMask);
        unsigned long ConvertAddressToMask(wxString sAddress);

        std::map<wxString, networkInterface>::const_iterator GetInterfaceBegin();
        std::map<wxString, networkInterface>::const_iterator GetInterfaceEnd();
        std::map<wxString, networkInterface>::const_iterator FindInterface(wxString sInterface);
//        bool HasAdminRights();

    protected:
        NetworkControl() : m_nNTEContext(0)
        {
            GetCurrentSettings();
        }

        void GetCurrentSettings();

        unsigned long m_nNTEContext;



        std::map<wxString, networkInterface> m_mInterfaces;

        #ifdef __WXGNU__
        static const wxString STR_INTERFACE;
        static const wxString STR_ADDRESS;
        static const wxString STR_GATEWAY;
        static const wxString STR_DNS;
        #endif

        static const wxString STR_MASK[33];
};
