#include <wx/string.h>
#include <wx/process.h>
#include <wx/log.h>
#include <wx/msgdlg.h>
#include "settings.h"
#include "networkcontrol.h"

#include <wx/tokenzr.h>
#include "log.h"
using namespace std;

const wxString NetworkControl::STR_MASK[33] = {"0.0.0.0", "128.0.0.0", "192.0.0.0", "224.0.0.0", "240.0.0.0", "248.0.0.0", "252.0.0.0", "254.0.0.0", "255.0.0.0", "255.128.0.0", "255.192.0.0", "255.224.0.0","255.240.0.0","255.248.0.0","255.252.0.0","255.254.0.0","255.255.0.0","255.255.128.0","255.255.192.0","255.255.224.0","255.255.240.0","255.255.248.0","255.255.252.0","255.255.254.0","255.255.255.0","255.255.255.128","255.255.255.192","255.255.255.224","255.255.255.240","255.255.255.248","255.255.255.252","255.255.255.254", "255.255.255.255"};

NetworkControl &NetworkControl::Get()
{
    static NetworkControl nc;
    return nc;
}

wxString NetworkControl::GetAddress(const wxString &sInterface) const
{
    if (sInterface.CmpNoCase("any") == 0)
    {
        return "0.0.0.0";
    }

    map<wxString, networkInterface>::const_iterator itInterface = m_mInterfaces.find(sInterface);
    if (itInterface != m_mInterfaces.end())
    {
        return itInterface->second.sAddress;
    }
    return wxEmptyString;
}

wxString NetworkControl::GetInterface(const wxString &sAddress) const
{
    if (sAddress == ("0.0.0.0."))
    {
        return "Any";
    }

    for (const auto &pairInterface : m_mInterfaces)
    {
        if (pairInterface.second.sAddress == sAddress)
        {
            return pairInterface.first;
        }
    }
    return "";
}

wxString NetworkControl::GetGateway(const wxString &sInterface) const
{
    map<wxString, networkInterface>::const_iterator itInterface = m_mInterfaces.find(sInterface);
    if (itInterface != m_mInterfaces.end())
    {
        return itInterface->second.sGateway;
    }
    return wxEmptyString;
}

unsigned long NetworkControl::GetMask(const wxString &sInterface) const
{
    map<wxString, networkInterface>::const_iterator itInterface = m_mInterfaces.find(sInterface);
    if (itInterface != m_mInterfaces.end())
    {
        return itInterface->second.nMask;
    }
    return 0;
}

#ifdef __WXMSW__
#include <winsock2.h>
#include <ws2tcpip.h>
#include <iphlpapi.h>
// #include <mstcpip.h>
#include <stdio.h>
#include <winnt.h>

wxString NetworkControl::SetupNetworking(const wxString &sInterface, const wxString &sAddress, unsigned long nMask, wxString sGateway, bool bDHCP)
{
    wxString sCommand;
    if (m_nNTEContext != 0)
    {
        if (DeleteNetworking() == false)
        {
            return "Could not delete previous address";
        }
    }

    PMIB_IPADDRTABLE pIPAddrTable;
    DWORD dwSize = 0;
    DWORD dwRetVal = 0;
    IN_ADDR IPAddr;
    DWORD ifIndex;

    /* IPv4 address and subnet mask we will be adding */
    UINT iaIPAddress;
    UINT iaIPMask;

    /* Variables where handles to the added IP are returned */

    ULONG NTEInstance = 0;

    /* Variables used to return error message */
    LPVOID lpMsgBuf;

    iaIPAddress = inet_addr(sAddress.mb_str());
    //    iaIPMask = inet_addr(nMask);

    // Before calling AddIPAddress we use GetIpAddrTable to get
    // an adapter to which we can add the IP.
    pIPAddrTable = (MIB_IPADDRTABLE *)malloc(sizeof(MIB_IPADDRTABLE));
    if (pIPAddrTable == NULL)
    {
        return "No network adaptor found";
    }
    else
    {
        dwSize = 0;
        // Make an initial call to GetIpAddrTable to get the
        // necessary size into the dwSize variable
        if (GetIpAddrTable(pIPAddrTable, &dwSize, 0) == ERROR_INSUFFICIENT_BUFFER)
        {
            free(pIPAddrTable);
            pIPAddrTable = (MIB_IPADDRTABLE *)malloc(dwSize);
        }
        if (pIPAddrTable == NULL)
        {
            return "Memory allocation failed for GetIpAddrTable\n";
        }
    }
    // Make a second call to GetIpAddrTable to get the
    // actual data we want
    if ((dwRetVal = GetIpAddrTable(pIPAddrTable, &dwSize, 0)) == NO_ERROR)
    {
        // Save the interface index to use for adding an IP address
        ifIndex = pIPAddrTable->table[0].dwIndex;
        IPAddr.S_un.S_addr = (u_long)pIPAddrTable->table[0].dwAddr;
        IPAddr.S_un.S_addr = (u_long)pIPAddrTable->table[0].dwMask;
        IPAddr.S_un.S_addr = (u_long)pIPAddrTable->table[0].dwBCastAddr;
    }
    else
    {
        if (pIPAddrTable)
        {
            free(pIPAddrTable);
        }
        return "Could not find adaptor";
    }

    if (pIPAddrTable)
    {
        free(pIPAddrTable);
        pIPAddrTable = NULL;
    }

    if ((dwRetVal = AddIPAddress(iaIPAddress, iaIPMask, ifIndex, &m_nNTEContext, &NTEInstance)) == NO_ERROR)
    {
        return wxEmptyString;
    }

    if (FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS, NULL, dwRetVal, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPTSTR)&lpMsgBuf, 0, NULL))
    {
        wxString str;
        str.Printf("%s", lpMsgBuf);
        LocalFree(lpMsgBuf);
        return str;
    }

    return "Unknown error";
}

bool NetworkControl::DeleteNetworking()
{
    DWORD dwRetVal;
    if ((dwRetVal = DeleteIPAddress(m_nNTEContext)) == NO_ERROR)
    {
        m_nNTEContext = 0;
        return true;
    }
    return false;
}

void NetworkControl::GetCurrentSettings()
{
}
void NetworkControl::ChangeWiFiNetwork(const wxString &sAccessPoint, const wxString &sPassword, const wxString &sInterface)
{
}
void NetworkControl::CheckConnection(const wxString &sInterface)
{
}

#else
#include <wx/textfile.h>
#include <iostream>
#include <fstream>

const wxString NetworkControl::STR_INTERFACE = "iface";
const wxString NetworkControl::STR_ADDRESS = "address";
const wxString NetworkControl::STR_GATEWAY = "gateway";
const wxString NetworkControl::STR_DNS = "dns-nameservers";

wxString NetworkControl::SetupNetworking(const wxString &sInterface, const wxString &sAddress, unsigned long nMask, wxString sGateway, bool bDHCP)
{
    if (sGateway == "...")
    {
        sGateway = wxEmptyString;
    }

    if(auto itInterface = m_mInterfaces.find(sInterface); itInterface != m_mInterfaces.end())
    {
        itInterface->second.bStatic = (!bDHCP);
        itInterface->second.sAddress = sAddress;
        itInterface->second.sGateway = sGateway;
        itInterface->second.nMask = nMask;
    }

    
    if (wxTextFile configFile; configFile.Open("/usr/local/etc/pam2/network.cfg"))
    {
        configFile.Clear();
    
        for(const auto& [sName, interface] : m_mInterfaces)
        {
            configFile.AddLine("auto "+sName);
            configFile.AddLine("allow-hotplug "+sName);
            configFile.AddLine(STR_INTERFACE+" "+sName+" inet "+(interface.bStatic ? "static" : "dhcp"));
            if(interface.bStatic)
            {
                configFile.AddLine(wxString::Format("    address %s/%u", interface.sAddress.c_str(), interface.nMask));
                configFile.AddLine(wxString::Format("    gateway %s", interface.sGateway.c_str()));
            }
        }
        pmlLog(pml::LOG_INFO, "pam::network") << "Attempt to save new network config";
        if( configFile.Write() == false)
        {
             pmlLog(pml::LOG_ERROR, "pam::network") << "Failed to save new network config";
             return "Could not write file";
        }
        else
        {
             GetCurrentSettings();
             pmlLog(pml::LOG_INFO, "pam::network") << "Restart networking";
             wxExecute("sudo service networking restart");
        }
        return wxEmptyString;
    }
    
    return "Could not read file";
}

void NetworkControl::ChangeWiFiNetwork(const wxString &sAccessPoint, const wxString &sPassword, const wxString &sInterface)
{
    map<wxString, wifi_cell>::iterator itCell = m_mCells.find(sAccessPoint);
    if (itCell != m_mCells.end())
    {
        wxArrayString asResult;
        if (itCell->second.nNetwork == -1)
        {

            wxExecute(wxString::Format("wpa_cli -i %s add_network", sInterface.c_str()), asResult);
            if (asResult.GetCount() > 0)
            {
                for (size_t i = 0; i < asResult.GetCount(); i++)
                {
                    if (asResult[i].ToLong(&itCell->second.nNetwork))
                    {
                        break;
                    }
                }
            }
        }

        if (itCell->second.nNetwork != -1)
        {
            wxExecute(wxString::Format("wpa_cli -i %s set_network %d ssid '\"%s\"'", sInterface.c_str(), itCell->second.nNetwork, sAccessPoint.c_str()), asResult);
            if (asResult.GetCount() == 0 || asResult[0] == "FAIL")
            {
                pmlLog(pml::LOG_WARN, "pam::networkcontrol") << "Unable to change WiFi";
                return;
            }
            if (itCell->second.bEncryption)
            {
                wxExecute(wxString::Format("wpa_cli -i %s set_network %d psk '\"%s\"'", sInterface.c_str(), itCell->second.nNetwork, sPassword.c_str()), asResult);
                if (asResult.GetCount() == 0 || asResult[0] == "FAIL")
                {
                    pmlLog(pml::LOG_WARN, "pam::networkcontrol") << "Unable to change WiFi password";
                    return;
                }
                if (itCell->second.sEncType.Find("WPA") != wxNOT_FOUND)
                {
                    wxExecute(wxString::Format("wpa_cli -i %s set_network %d key_mgmt WPA-PSK", sInterface.c_str(), itCell->second.nNetwork), asResult);
                    if (asResult.GetCount() == 0 || asResult[0] == "FAIL")
                    {
                        pmlLog(pml::LOG_WARN, "pam::networkcontrol") << "Unable to change WiFi key management";
                        return;
                    }
                }
            }
            wxExecute(wxString::Format("wpa_cli -i %s enable_network %d", sInterface.c_str(), itCell->second.nNetwork), asResult);
            if (asResult.GetCount() == 0 || asResult[0] == "FAIL")
            {
                pmlLog(pml::LOG_WARN, "pam::networkcontrol") << "Unable to enable WiFi network";
                return;
            }

            wxExecute(wxString::Format("wpa_cli -i %s save_config", sInterface.c_str()), asResult);
            if (asResult.GetCount() == 0 || asResult[0] == "FAIL")
            {
                pmlLog(pml::LOG_WARN, "pam::networkcontrol") << "Unable to save WiFi config";
                return;
            }
            // wxExecute("wpa_cli reconfigure");

            wxExecute(wxString::Format("wpa_cli -i %s select_network %d", sInterface.c_str(), itCell->second.nNetwork), asResult);

            pmlLog(pml::LOG_INFO, "pam::network") << "WiFi '" << itCell->first.c_str() << "' Setup: network " << itCell->second.nNetwork;
            Settings::Get().Write("WiFi", sAccessPoint, sPassword);
        }
        else
        {
            pmlLog(pml::LOG_WARN, "pam::networkcontrol") << "Unable to add WiFi network";
        }
    }
}

bool NetworkControl::DeleteNetworking()
{
    return true;
}

void NetworkControl::GetCurrentSettings()
{
    m_mInterfaces.clear();
    // run ifconfig to get list of interfaces

    // run iwconfig interface to find out whether wireless and essid
    // then read config file to see if static
    wxArrayString asInterfaces;
    long nResult = wxExecute("sh -c \"ifconfig | grep flags \"", asInterfaces);

    for (size_t nLine = 0; nLine < asInterfaces.GetCount(); nLine++)
    {
        asInterfaces[nLine] = asInterfaces[nLine].BeforeFirst(':');
        if (asInterfaces[nLine] != "lo")
        {
            networkInterface anInterface;
            // if have inet entry then connected - else not

            CheckConnection(asInterfaces[nLine], anInterface);
            m_mInterfaces.insert(make_pair(asInterfaces[nLine], anInterface));
        }
    }

    map<wxString, networkInterface>::iterator itInterface = m_mInterfaces.end();
    wxTextFile configFile;
    if (configFile.Open("/usr/local/etc/pam2/network.cfg"))
    {
        for (size_t i = 0; i < configFile.GetLineCount(); i++)
        {
            auto sLine = configFile.GetLine(i).Trim(false);

            if (sLine.Left(STR_INTERFACE.length()) == STR_INTERFACE)
            {
                auto as = wxStringTokenize(sLine, " ");
                if (as.GetCount() > DHCP_STATIC)
                {
                    itInterface = m_mInterfaces.find(as[ETH]);
                    if (itInterface != m_mInterfaces.end())
                    {
                        itInterface->second.bStatic = (as[DHCP_STATIC] != "dhcp");
                    }
                }
            }
            else if (itInterface != m_mInterfaces.end())
            {
                if (sLine.Left(STR_ADDRESS.length()) == STR_ADDRESS && itInterface->second.bStatic)
                {
                    itInterface->second.sAddress = sLine.AfterFirst(' ').BeforeFirst('/');
                    sLine.AfterFirst('/').ToULong(&itInterface->second.nMask);
                }
                else if (sLine.Left(STR_GATEWAY.length()) == STR_GATEWAY)
                {
                    itInterface->second.sGateway = sLine.AfterFirst(' ');
                }
            }
        }
    }

    // update any settings that depend on the interface/ip address
    // Settings::Get().Write("Server","RTSP_Address", GetAddress(Settings::Get().Read("Server", "RTSP_Interface", "eth0")));
    // Settings::Get().Write("RemoteApi","Interface", NetworkControl::Get().GetAddress(Settings::Get().Read("RemoteApi", "_Interface", "eth0")));
}
void NetworkControl::CheckConnection(const wxString &sInterface)
{
}
void NetworkControl::CheckConnection(const wxString &sInterface, networkInterface &anInterface)
{
    wxArrayString asOutput;
    wxExecute(wxString::Format("sh -c \"ifconfig %s | grep netmask\"", sInterface.c_str()), asOutput);
    if (asOutput.GetCount() != 0)
    {
        anInterface.bConnected = true;
        anInterface.sAddress = asOutput[0].AfterFirst('t').BeforeFirst('n');
        anInterface.sAddress.Trim().Trim(false);
        wxString sMask = asOutput[0].AfterFirst('k').BeforeFirst('b');
        sMask.Trim().Trim(false);
        anInterface.nMask = ConvertAddressToMask(sMask);
    }
    else
    {
        anInterface.bConnected = false;
        anInterface.sAddress.clear();
    }
    wxArrayString asOutput2;
    wxExecute(wxString::Format("sh -c \"iwconfig %s | grep ESSID\"", sInterface.c_str()), asOutput2);
    if (asOutput2.GetCount() == 0 || asOutput2[0].Find("no wireless") != wxNOT_FOUND)
    {
        anInterface.bWireless = false;
    }
    else
    {
        anInterface.bWireless = true;
        anInterface.sEssid = asOutput2[0].AfterFirst('"').BeforeFirst('"');
    }
}
#endif // __WXMSW__

wxString NetworkControl::ConvertMaskToAddress(unsigned long nMask)
{
    if (nMask < 33)
    {
        return STR_MASK[nMask];
    }
    return wxEmptyString;
}

unsigned long NetworkControl::ConvertAddressToMask(wxString sAddress)
{
    for (unsigned int i = 0; i < 33; i++)
    {
        if (STR_MASK[i] == sAddress)
            return i;
    }
    return 0;
}

map<wxString, networkInterface>::const_iterator NetworkControl::GetInterfaceBegin()
{
    return m_mInterfaces.begin();
}

map<wxString, networkInterface>::const_iterator NetworkControl::GetInterfaceEnd()
{
    return m_mInterfaces.end();
}

map<wxString, networkInterface>::const_iterator NetworkControl::FindInterface(wxString sInterface)
{
    return m_mInterfaces.find(sInterface);
}

std::map<wxString, wifi_cell>::const_iterator NetworkControl::GetWiFiCellBegin()
{
    return m_mCells.begin();
}
std::map<wxString, wifi_cell>::const_iterator NetworkControl::GetWiFiCellEnd()
{
    return m_mCells.end();
}

std::map<wxString, wifi_cell>::const_iterator NetworkControl::FindWiFiCell(const wxString &sSsid)
{
    return m_mCells.find(sSsid);
}

void NetworkControl::ScanWiFi(const wxString &sInterface)
{
    m_mCells.clear();

    wxExecute(wxString::Format("wpa_cli -i %s scan", sInterface.c_str()));

    wxArrayString asResults;
    wxExecute(wxString::Format("wpa_cli scan_results"), asResults);
    if (asResults.GetCount() > 0)
    {
        for (size_t nLine = 1; nLine < asResults.GetCount(); nLine++)
        {

            wxArrayString asLine(wxStringTokenize(asResults[nLine], "\t"));
            if (asLine.GetCount() > 4)
            {
                wifi_cell aCell;
                aCell.sESSID = asLine[4];
                size_t nStart = asLine[3].Find("WPA");
                if (nStart == wxNOT_FOUND)
                {
                    nStart = asLine[3].Find("WEP");
                }

                if (nStart != wxNOT_FOUND)
                {
                    aCell.bEncryption = true;
                    aCell.sEncType = asLine[3].Mid(nStart).BeforeFirst(']');
                }
                else
                {
                    aCell.bEncryption = false;
                }
                m_mCells.insert(make_pair(aCell.sESSID, aCell));
            }
        }

        wxExecute(wxString::Format("wpa_cli list_networks"), asResults);
        for (size_t nLine = 0; nLine < asResults.GetCount(); nLine++)
        {

            wxArrayString asLine(wxStringTokenize(asResults[nLine], "\t"));

            if (asLine.GetCount() >= 2)
            {
                map<wxString, wifi_cell>::iterator itCell = m_mCells.find(asLine[1]);
                if (itCell != m_mCells.end())
                {

                    asLine[0].ToLong(&itCell->second.nNetwork);
                }
            }
        }
    }
}

std::set<wxString> NetworkControl::GetInterfaceNames()
{
    std::set<wxString> setInt;
    for (const auto &pairInt : m_mInterfaces)
    {
        setInt.insert(pairInt.first);
    }
    return setInt;
}
