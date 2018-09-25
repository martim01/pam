#include <wx/string.h>
#include <wx/process.h>
#include <wx/log.h>
#include <wx/msgdlg.h>
#include "settings.h"
#include "networkcontrol.h"


NetworkControl& NetworkControl::Get()
{
    static NetworkControl nc;
    return nc;
}

const wxString& NetworkControl::GetAddress() const
{
    return m_sAddress;
}

const wxString& NetworkControl::GetGateway() const
{
    return m_sGateway;
}

unsigned long NetworkControl::GetMask() const
{
    return m_nMask;
}



#ifdef __WXMSW__
#include <winsock2.h>
#include <ws2tcpip.h>
#include <iphlpapi.h>
//#include <mstcpip.h>
#include <stdio.h>
#include <winnt.h>

wxString NetworkControl::SetupNetworking(const wxString& sAddress, unsigned long nMask, const wxString& sGateway)
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

#else
#include <wx/textfile.h>
#include <iostream>
#include <fstream>

const wxString NetworkControl::STR_INTERFACE = wxT("interface eth0");
const wxString NetworkControl::STR_ADDRESS = wxT("static ip_address=");
const wxString NetworkControl::STR_GATEWAY = wxT("static routers=");
const wxString NetworkControl::STR_DNS = wxT("static domain_name_servers=");

wxString NetworkControl::SetupNetworking(const wxString& sAddress, unsigned long nMask, wxString sGateway)
{
    if(sGateway == wxT("..."))
    {
        sGateway = wxEmptyString;
    }

    wxTextFile configFile;
    bool bEth0(false);
    bool bDHCP((sAddress.empty() == true));

    if(configFile.Open(wxT("/etc/dhcpcd.conf")))
    {
        if(bDHCP)
        {
            for(int i = 0; i < configFile.GetLineCount(); i++)
            {
                if(configFile.GetLine(i).Left(STR_INTERFACE.length()) == STR_INTERFACE)
                {
                    bEth0 = true;
                    configFile.GetLine(i) = wxT("#") + STR_INTERFACE;
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
            for(int i = 0; i < configFile.GetLineCount(); i++)
            {
                if(configFile.GetLine(i).Left(STR_INTERFACE.length()) == STR_INTERFACE)
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
                configFile.AddLine(STR_INTERFACE);
                configFile.AddLine(wxString::Format(wxT("%s%s/%d"), STR_ADDRESS.c_str(), sAddress.c_str(), nMask));

                configFile.AddLine(STR_GATEWAY+sGateway);
            }
        }

        std::ofstream outFile;
        outFile.open("/etc/dhcpcd.conf", std::ofstream::out | std::ofstream::trunc);
        if(outFile.is_open())
        {
            for(int i = 0; i < configFile.GetLineCount(); i++)
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

bool NetworkControl::DeleteNetworking()
{
    return true;
}


void NetworkControl::GetCurrentSettings()
{

    bool bEth0(false);
    wxTextFile configFile;
    if(configFile.Open(wxT("/etc/dhcpcd.conf")))
    {
        for(int i = 0; i < configFile.GetLineCount(); i++)
        {
            if(configFile.GetLine(i).Left(STR_INTERFACE.length()) == STR_INTERFACE)
            {
                bEth0 = true;
            }
            else if(configFile.GetLine(i).Left(9) == wxT("interface"))
            {
                bEth0 = false;
            }
            else if(bEth0)
            {
                if(configFile.GetLine(i).Left(STR_ADDRESS.length()) == STR_ADDRESS)
                {
                    m_sAddress = configFile.GetLine(i).AfterFirst(wxT('=')).BeforeFirst(wxT('/'));
                    configFile.GetLine(i).AfterFirst(wxT('/')).ToULong(&m_nMask);
                }
                else if(configFile.GetLine(i).Left(STR_GATEWAY.length()) == STR_GATEWAY)
                {
                    m_sGateway = configFile.GetLine(i).AfterFirst(wxT('='));
                }

            }

        }
    }
}

#endif // __WXMSW__



wxString NetworkControl::ConvertMaskToAddress(unsigned long nMask)
{
    switch(nMask)
    {
       case 32:
           return wxT("255.255.255.255");

        case 31:
                return wxT("255.255.255.254");
        case 30:
                return wxT("255.255.255.252");
        case 29:
                return wxT("255.255.255.248");
        case 28:
                return wxT("255.255.255.240");
        case 27:
                return wxT("255.255.255.224");
        case 26:
                return wxT("255.255.255.192");
        case 25:
                return wxT("255.255.255.128");
        case 24:
                return wxT("255.255.255.0");
        case 23:
                return wxT("255.255.254.0");
        case 22:
                return wxT("255.255.252.0");
        case 21:
                return wxT("255.255.248.0");
        case 20:
                return wxT("255.255.240.0");
        case 19:
                return wxT("255.255.224.0");
        case 18:
                return wxT("255.255.192.0");
        case 17:
                return wxT("255.255.128.0");
        case 16:
                return wxT("255.255.0.0");
        case 15:
                return wxT("255.254.0.0");
        case 14:
                return wxT("255.252.0.0");
        case 13:
                return wxT("255.248.0.0");
        case 12:
                return wxT("255.240.0.0");
        case 11:
                return wxT("255.224.0.0");
        case 10:
                return wxT("255.192.0.0");
        case 9:
                return wxT("255.128.0.0");
        case 8:
                return wxT("255.0.0.0");
        case 7:
                return wxT("254.0.0.0");
        case 6:
                return wxT("252.0.0.0");
        case 5:
                return wxT("248.0.0.0");
        case 4:
                return wxT("240.0.0.0");
        case 3:
                return wxT("224.0.0.0");
        case 2:
                return wxT("192.0.0.0");
        case 1:
                return wxT("128.0.0.0");
    }
    return wxEmptyString;
}
