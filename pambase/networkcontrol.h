#pragma once
#include <map>

struct networkInterface
{
    networkInterface() : nMask(0), bStatic(false), bWireless(false), bConnected(false){}
    wxString sAddress;
    unsigned long nMask;
    wxString sGateway;
    wxString sDNS;
    bool bStatic;
    bool bWireless;
    bool bConnected;
    wxString sEssid;
    wxString sPassword;
};

struct wifi_cell
{
    wifi_cell() : bEncryption(false), nNetwork(-1){}
    wxString sESSID;
    bool bEncryption;
    wxString sEncType;
    wxString sPassword;
    long nNetwork;
};

class NetworkControl
{
    public:
        static NetworkControl& Get();

        wxString SetupNetworking(const wxString& sInterface, const wxString& sAddress, unsigned long nMask, wxString sGateway, bool bDHCP);
        bool DeleteNetworking();

        wxString GetAddress(const wxString& sInterface) const;
        wxString GetGateway(const wxString& sInterface) const;
        unsigned long GetMask(const wxString& sInterface) const;
        wxString GetInterface(const wxString& sAddress) const;

        wxString ConvertMaskToAddress(unsigned long nMask);
        unsigned long ConvertAddressToMask(wxString sAddress);

        void ChangeWiFiNetwork(const wxString& sAccessPoint, const wxString& sPassword, const wxString& sInterface);
        std::map<wxString, networkInterface>::const_iterator GetInterfaceBegin();
        std::map<wxString, networkInterface>::const_iterator GetInterfaceEnd();
        std::map<wxString, networkInterface>::const_iterator FindInterface(wxString sInterface);

        std::set<wxString> GetInterfaceNames();
//        bool HasAdminRights();
        void CheckConnection(const wxString& sInterface);

        std::map<wxString, wifi_cell>::const_iterator GetWiFiCellBegin();
        std::map<wxString, wifi_cell>::const_iterator GetWiFiCellEnd();
        std::map<wxString, wifi_cell>::const_iterator FindWiFiCell(const wxString& sSsid);
        void ScanWiFi(const wxString& sInterface);


    protected:
        NetworkControl() : m_nNTEContext(0)
        {
            GetCurrentSettings();
        }

        void GetCurrentSettings();
        void CheckConnection(const wxString& sInterface, networkInterface& anInterface);
        unsigned long m_nNTEContext;


        std::map<wxString, wifi_cell> m_mCells;




        std::map<wxString, networkInterface> m_mInterfaces;

        #ifdef __WXGNU__
        static const wxString STR_INTERFACE;
        static const wxString STR_ADDRESS;
        static const wxString STR_GATEWAY;
        static const wxString STR_DNS;
        #endif

        static const wxString STR_MASK[33];
};
