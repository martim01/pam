#include <wx/string.h>
#include <wx/process.h>
#include <wx/log.h>
#include <wx/msgdlg.h>
#include "settings.h"
#include "networkcontrol.h"
#include "wmlogevent.h"
#include <wx/tokenzr.h>

using namespace std;


const wxString NetworkControl::STR_MASK[33] = {wxT("0.0.0.0"), wxT("128.0.0.0"), wxT("192.0.0.0"), wxT("224.0.0.0"), wxT("240.0.0.0"), wxT("248.0.0.0"), wxT("252.0.0.0"), wxT("254.0.0.0"), wxT("255.0.0.0"), wxT("255.128.0.0"), wxT("255.192.0.0"), wxT("255.224.0.0"),wxT("255.240.0.0"),wxT("255.248.0.0"),wxT("255.252.0.0"),wxT("255.254.0.0"),wxT("255.255.0.0"),wxT("255.255.128.0"),wxT("255.255.192.0"),wxT("255.255.224.0"),wxT("255.255.240.0"),wxT("255.255.248.0"),wxT("255.255.252.0"),wxT("255.255.254.0"),wxT("255.255.255.0"),wxT("255.255.255.128"),wxT("255.255.255.192"),wxT("255.255.255.224"),wxT("255.255.255.240"),wxT("255.255.255.248"),wxT("255.255.255.252"),wxT("255.255.255.254"),wxT("255.255.255.255")};

NetworkControl& NetworkControl::Get()
{
    static NetworkControl nc;
    return nc;
}

wxString NetworkControl::GetAddress(const wxString& sInterface) const
{
    map<wxString, networkInterface>::const_iterator itInterface = m_mInterfaces.find(sInterface);
    if(itInterface != m_mInterfaces.end())
    {
        return itInterface->second.sAddress;
    }
    return wxEmptyString;
}

wxString NetworkControl::GetGateway(const wxString& sInterface) const
{
    map<wxString, networkInterface>::const_iterator itInterface = m_mInterfaces.find(sInterface);
    if(itInterface != m_mInterfaces.end())
    {
        return itInterface->second.sGateway;
    }
    return wxEmptyString;
}

unsigned long NetworkControl::GetMask(const wxString& sInterface) const
{
    map<wxString, networkInterface>::const_iterator itInterface = m_mInterfaces.find(sInterface);
    if(itInterface != m_mInterfaces.end())
    {
        return itInterface->second.nMask;
    }
    return 0;
}



#ifdef __WXMSW__
#include <winsock2.h>
#include <ws2tcpip.h>
#include <iphlpapi.h>
//#include <mstcpip.h>
#include <stdio.h>
#include <winnt.h>

wxString NetworkControl::SetupNetworking(const wxString& sInterface, const wxString& sAddress, unsigned long nMask, wxString sGateway)
{
    wxString sCommand;
    if(m_nNTEContext != 0)
    {
        if(DeleteNetworking() == false)
        {
            return wxT("Could not delete previous address");
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
    pIPAddrTable = (MIB_IPADDRTABLE *) malloc(sizeof (MIB_IPADDRTABLE));
    if (pIPAddrTable == NULL)
    {
        return wxT("No network adaptor found");
    }
    else
    {
        dwSize = 0;
        // Make an initial call to GetIpAddrTable to get the
        // necessary size into the dwSize variable
        if (GetIpAddrTable(pIPAddrTable, &dwSize, 0) == ERROR_INSUFFICIENT_BUFFER)
        {
            free(pIPAddrTable);
            pIPAddrTable = (MIB_IPADDRTABLE *) malloc(dwSize);

        }
        if (pIPAddrTable == NULL)
        {
            return wxT("Memory allocation failed for GetIpAddrTable\n");
        }
    }
    // Make a second call to GetIpAddrTable to get the
    // actual data we want
    if ((dwRetVal = GetIpAddrTable(pIPAddrTable, &dwSize, 0)) == NO_ERROR)
    {
        // Save the interface index to use for adding an IP address
        ifIndex = pIPAddrTable->table[0].dwIndex;
        IPAddr.S_un.S_addr = (u_long) pIPAddrTable->table[0].dwAddr;
        IPAddr.S_un.S_addr = (u_long) pIPAddrTable->table[0].dwMask;
        IPAddr.S_un.S_addr = (u_long) pIPAddrTable->table[0].dwBCastAddr;


    }
    else
    {
        if (pIPAddrTable)
        {
            free(pIPAddrTable);
        }
        return wxT("Could not find adaptor");
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


    if (FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS, NULL, dwRetVal, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),(LPTSTR) & lpMsgBuf, 0, NULL))
    {
        wxString str;
        str.Printf(wxT("%s"), lpMsgBuf);
        LocalFree(lpMsgBuf);
        return str;
    }

    return wxT("Unknown error");

}

bool NetworkControl::DeleteNetworking()
{
    DWORD dwRetVal;
    if ((dwRetVal = DeleteIPAddress(m_nNTEContext)) == NO_ERROR)
    {
        m_nNTEContext  = 0;
        return true;
    }
    return false;
}

void NetworkControl::GetCurrentSettings()
{

}
void NetworkControl::ChangeWiFiNetwork(const wxString& sAccessPoint, const wxString& sPassword, const wxString& sInterface)
{


}
void NetworkControl::CheckConnection(const wxString& sInterface)
{

}

#else
#include <wx/textfile.h>
#include <iostream>
#include <fstream>

const wxString NetworkControl::STR_INTERFACE = wxT("interface");
const wxString NetworkControl::STR_ADDRESS = wxT("static ip_address=");
const wxString NetworkControl::STR_GATEWAY = wxT("static routers=");
const wxString NetworkControl::STR_DNS = wxT("static domain_name_servers=");

wxString NetworkControl::SetupNetworking(const wxString& sInterface, const wxString& sAddress, unsigned long nMask, wxString sGateway)
{
    if(sGateway == wxT("..."))
    {
        sGateway = wxEmptyString;
    }

    wxString sInterfaceLine(wxString::Format(wxT("%s %s"), STR_INTERFACE.c_str(), sInterface.c_str()));
    wxTextFile configFile;
    bool bEth0(false);
    bool bDHCP((sAddress.empty() == true));

    if(configFile.Open(wxT("/etc/dhcpcd.conf")))
    {
        if(bDHCP)
        {
            for(size_t i = 0; i < configFile.GetLineCount(); i++)
            {
                if(configFile.GetLine(i).Left(sInterfaceLine.length()) == sInterfaceLine)
                {
                    bEth0 = true;
                    configFile.GetLine(i) = wxT("#") + sInterfaceLine;
                }
                else if(configFile.GetLine(i).Left(9) == wxT("interface"))
                {
                    bEth0 = false;
                }
                else if(bEth0)
                {
                    if(configFile.GetLine(i).Left(STR_ADDRESS.length()) == STR_ADDRESS)
                    {
                        configFile.GetLine(i) = wxEmptyString;
                    }
                    else if(configFile.GetLine(i).Left(STR_GATEWAY.length()) == STR_GATEWAY)
                    {
                        configFile.GetLine(i) = wxEmptyString;
                    }
                }
            }
        }
        else
        {
            bool bReplace(false);
            for(size_t i = 0; i < configFile.GetLineCount(); i++)
            {
                if(configFile.GetLine(i).Left(sInterfaceLine.length()) == sInterfaceLine)
                {
                    bEth0 = true;
                    bReplace = true;
                }
                else if(configFile.GetLine(i).Left(9) == wxT("interface"))
                {
                    bEth0 = false;
                }
                else if(bEth0)
                {
                    if(configFile.GetLine(i).Left(STR_ADDRESS.length()) == STR_ADDRESS)
                    {
                        configFile.GetLine(i) = wxString::Format(wxT("%s%s/%d"), STR_ADDRESS.c_str(), sAddress.c_str(), nMask);
                    }
                    else if(configFile.GetLine(i).Left(STR_GATEWAY.length()) == STR_GATEWAY )
                    {
                        configFile.GetLine(i) = STR_GATEWAY+sGateway;
                    }
                }
            }

            if(bReplace == false)
            {
                configFile.AddLine(sInterfaceLine);
                configFile.AddLine(wxString::Format(wxT("%s%s/%d"), STR_ADDRESS.c_str(), sAddress.c_str(), nMask));

                configFile.AddLine(STR_GATEWAY+sGateway);
            }
        }

        std::ofstream outFile;
        outFile.open("/etc/dhcpcd.conf", std::ofstream::out | std::ofstream::trunc);
        if(outFile.is_open())
        {
            for(size_t i = 0; i < configFile.GetLineCount(); i++)
            {
                outFile << configFile.GetLine(i).mb_str() << std::endl;
            }
            outFile.close();
            configFile.Close();
            GetCurrentSettings();
            // restart the interfaces
            wxExecute(wxT("sudo service dhcpcd restart"));
            return wxEmptyString;
        }
        else
        {
            configFile.Close();
            return  wxT("Could not save file");
        }
    }

    return wxT("Could not read file");
}

void NetworkControl::ChangeWiFiNetwork(const wxString& sAccessPoint, const wxString& sPassword, const wxString& sInterface)
{
    map<wxString, wifi_cell>::iterator itCell = m_mCells.find(sAccessPoint);
    if(itCell != m_mCells.end())
    {
        wxArrayString asResult;
        if(itCell->second.nNetwork == -1)
        {

            wxExecute(wxString::Format(wxT("wpa_cli -i %s add_network"), sInterface.c_str()), asResult);
            if(asResult.GetCount() > 0)
            {
                for(size_t i = 0; i < asResult.GetCount(); i++)
                {
                    if(asResult[i].ToLong(&itCell->second.nNetwork))
                    {
                        break;
                    }
                }
            }
        }

        if(itCell->second.nNetwork != -1)
        {
            wxExecute(wxString::Format(wxT("wpa_cli -i %s set_network %d ssid '\"%s\"'"), sInterface.c_str(), itCell->second.nNetwork, sAccessPoint.c_str()), asResult);
            if(asResult.GetCount() == 0 || asResult[0] == "FAIL")
            {
                wmLog::Get()->Log(wxT("Unable to change WiFi"));
                return;
            }
            if(itCell->second.bEncryption)
            {
                wxExecute(wxString::Format(wxT("wpa_cli -i %s set_network %d psk '\"%s\"'"), sInterface.c_str(), itCell->second.nNetwork, sPassword.c_str()), asResult);
                if(asResult.GetCount() == 0 || asResult[0] == "FAIL")
                {
                    wmLog::Get()->Log(wxT("Unable to change WiFi passworkd"));
                    return;
                }
                if(itCell->second.sEncType.Find(wxT("WPA")) != wxNOT_FOUND)
                {
                    wxExecute(wxString::Format(wxT("wpa_cli -i %s set_network %d key_mgmt WPA-PSK"), sInterface.c_str(), itCell->second.nNetwork), asResult);
                    if(asResult.GetCount() == 0 || asResult[0] == "FAIL")
                    {
                        wmLog::Get()->Log(wxT("Unable to change WiFi key management"));
                        return;
                    }
                }
            }
            wxExecute(wxString::Format(wxT("wpa_cli -i %s enable_network %d"), sInterface.c_str(), itCell->second.nNetwork), asResult);
            if(asResult.GetCount() == 0 || asResult[0] == "FAIL")
            {
                wmLog::Get()->Log(wxT("Unable to enable WiFi network"));
                return;
            }

            wxExecute(wxString::Format(wxT("wpa_cli -i %s save_config"), sInterface.c_str()), asResult);
            if(asResult.GetCount() == 0 || asResult[0] == "FAIL")
            {
                wmLog::Get()->Log(wxT("Unable to save WiFi config"));
                return;
            }
            //wxExecute(wxT("wpa_cli reconfigure"));

            wxExecute(wxString::Format(wxT("wpa_cli -i %s select_network %d"), sInterface.c_str(), itCell->second.nNetwork), asResult);

            wmLog::Get()->Log(wxString::Format(wxT("WiFi '%s' setup: network %d"), itCell->first.c_str(), itCell->second.nNetwork));
            Settings::Get().Write(wxT("WiFi"), sAccessPoint, sPassword);
        }
        else
        {
            wmLog::Get()->Log(wxT("Unable to add WiFi network"));
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
    //run ifconfig to get list of interfaces

    //run iwconfig interface to find out whether wireless and essid
    //then read config file to see if static
    wxArrayString asInterfaces;
    long nResult = wxExecute(wxT("sh -c \"ifconfig | grep flags \""), asInterfaces);

    for(size_t nLine = 0; nLine < asInterfaces.GetCount(); nLine++)
    {
        asInterfaces[nLine] = asInterfaces[nLine].BeforeFirst(wxT(':'));
        if(asInterfaces[nLine] != wxT("lo"))
        {
            networkInterface anInterface;
            //if have inet entry then connected - else not

            CheckConnection(asInterfaces[nLine], anInterface);
            m_mInterfaces.insert(make_pair(asInterfaces[nLine], anInterface));
        }
    }

    map<wxString, networkInterface>::iterator itInterface = m_mInterfaces.end();
    wxTextFile configFile;
    if(configFile.Open(wxT("/etc/dhcpcd.conf")))
    {
        for(size_t i = 0; i < configFile.GetLineCount(); i++)
        {
            if(configFile.GetLine(i).Left(STR_INTERFACE.length()) == STR_INTERFACE)
            {
                itInterface = m_mInterfaces.find(configFile.GetLine(i).AfterFirst(wxT(' ')));
            }
            else if(itInterface != m_mInterfaces.end())
            {
                if(configFile.GetLine(i).Left(STR_ADDRESS.length()) == STR_ADDRESS)
                {
                    itInterface->second.bStatic = true;
                    itInterface->second.sAddress = configFile.GetLine(i).AfterFirst(wxT('=')).BeforeFirst(wxT('/'));
                    configFile.GetLine(i).AfterFirst(wxT('/')).ToULong(&itInterface->second.nMask);
                }
                else if(configFile.GetLine(i).Left(STR_GATEWAY.length()) == STR_GATEWAY)
                {
                    itInterface->second.sGateway = configFile.GetLine(i).AfterFirst(wxT('='));
                }
            }

        }
    }
}
void NetworkControl::CheckConnection(const wxString& sInterface)
{

}
void NetworkControl::CheckConnection(const wxString& sInterface, networkInterface& anInterface)
{
    wxArrayString asOutput;
    wxExecute(wxString::Format(wxT("sh -c \"ifconfig %s | grep netmask\""), sInterface.c_str()), asOutput);
    if(asOutput.GetCount() != 0)
    {
        anInterface.bConnected = true;
        anInterface.sAddress = asOutput[0].AfterFirst(wxT('t')).BeforeFirst(wxT('n'));
        anInterface.sAddress.Trim().Trim(false);
        wxString sMask = asOutput[0].AfterFirst(wxT('k')).BeforeFirst(wxT('b'));
        sMask.Trim().Trim(false);
        anInterface.nMask = ConvertAddressToMask(sMask);
    }
    else
    {
        anInterface.bConnected = false;
        anInterface.sAddress.clear();
    }
    wxArrayString asOutput2;
    wxExecute(wxString::Format(wxT("sh -c \"iwconfig %s | grep ESSID\""),sInterface.c_str()), asOutput2);
    if(asOutput2.GetCount() == 0 || asOutput2[0].Find(wxT("no wireless")) != wxNOT_FOUND)
    {
        anInterface.bWireless = false;
    }
    else
    {
        anInterface.bWireless = true;
        anInterface.sEssid = asOutput2[0].AfterFirst(wxT('"')).BeforeFirst(wxT('"'));
    }
}
#endif // __WXMSW__


wxString NetworkControl::ConvertMaskToAddress(unsigned long nMask)
{
    if(nMask < 33)
    {
        return STR_MASK[nMask];
    }
    return wxEmptyString;
}


unsigned long NetworkControl::ConvertAddressToMask(wxString sAddress)
{
    for(unsigned int i = 0; i < 33; i++)
    {
        if(STR_MASK[i] == sAddress)
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

std::map<wxString, wifi_cell>::const_iterator NetworkControl::FindWiFiCell(const wxString& sSsid)
{
    return m_mCells.find(sSsid);
}

void NetworkControl::ScanWiFi(const wxString& sInterface)
{
    m_mCells.clear();

    wxExecute(wxString::Format(wxT("wpa_cli -i %s scan"), sInterface.c_str()));

    wxArrayString asResults;
    wxExecute(wxString::Format(wxT("wpa_cli scan_results")), asResults);
    if(asResults.GetCount() > 0)
    {
        for(size_t nLine = 1; nLine < asResults.GetCount(); nLine++)
        {
            wxLogDebug(asResults[nLine]);

            wxArrayString asLine(wxStringTokenize(asResults[nLine], wxT("\t")));
            if(asLine.GetCount() > 4)
            {
                wifi_cell aCell;
                aCell.sESSID = asLine[4];
                size_t nStart = asLine[3].Find(wxT("WPA"));
                if(nStart == wxNOT_FOUND)
                {
                    nStart = asLine[3].Find(wxT("WEP"));
                }

                if(nStart != wxNOT_FOUND)
                {
                    aCell.bEncryption = true;
                    aCell.sEncType = asLine[3].Mid(nStart).BeforeFirst(wxT(']'));
                }
                else
                {
                    aCell.bEncryption = false;
                }
                m_mCells.insert(make_pair(aCell.sESSID, aCell));
            }
        }

        wxExecute(wxString::Format(wxT("wpa_cli list_networks")), asResults);
        for(size_t nLine = 0; nLine < asResults.GetCount(); nLine++)
        {
            wxLogDebug(asResults[nLine]);
            wxArrayString asLine(wxStringTokenize(asResults[nLine], wxT("\t")));
            wxLogDebug(wxT("Split %d"), asLine.GetCount());
            if(asLine.GetCount() >= 2)
            {
                map<wxString, wifi_cell>::iterator itCell = m_mCells.find(asLine[1]);
                if(itCell != m_mCells.end())
                {
                    wxLogDebug(wxT("Cell Found "));
                    asLine[0].ToLong(&itCell->second.nNetwork);
                }
            }
        }
    }
    wxLogDebug(wxT("Scan done-----------------------"));
}



