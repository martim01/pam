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

wxString NetworkControl::SetupNetworking()
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


    iaIPAddress = inet_addr(Settings::Get().Read(wxT("Network"), wxT("Address") ,wxEmptyString).mb_str());
    iaIPMask = inet_addr(Settings::Get().Read(wxT("Network"), wxT("Subnet"),wxEmptyString).mb_str());


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

#else
#include <wx/textfile.h>
#include <iostream>
#include <fstream>

const wxString NetworkControl::STR_ADDRESS = wxT("static ip_address=");
const wxString NetworkControl::STR_GATEWAY = wxT("static routers=");
const wxString NetworkControl::STR_DNS = wxT("static domain_name_servers=");

wxString NetworkControl::SetupNetworking(const wxString& sAddress, unsigned long nMask, const wxString& sGateway)
{

    wxTextFile configFile;
    bool bEth0(false);
    bool bDHCP((sAddress.empty() == true));

    if(configFile.Open(wxT("/etc/dhcpcd.conf")))
    {
        if(bDHCP)
        {
            for(int i = 0; i < configFile.GetLineCount(); i++)
            {
                if(configFile.GetLine(i) == wxT("interface eth0"))
                {
                    bEth0 = true;
                    configFile.GetLine(i) == wxT("#interface eth0");
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
                if(configFile.GetLine(i) == wxT("interface eth0"))
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
                    else if(configFile.GetLine(i).Left(STR_GATEWAY.length()) == STR_GATEWAY)
                    {
                        configFile.GetLine(i) = STR_GATEWAY+sGateway;
                    }
                }
            }

            if(bReplace == false)
            {
                configFile.AddLine(wxT("interace eth0"));
                configFile.AddLine(STR_ADDRESS+sAddress);
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
            if(configFile.GetLine(i) == wxT("interface eth0"))
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




