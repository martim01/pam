#include <wx/string.h>
#include <wx/process.h>
#include <wx/log.h>
#include <wx/msgdlg.h>
#include "settings.h"
#include "networkcontrol.h"


NetworkControl* NetworkControl::Get()
{
    static NetworkControl nc;
    return &nc;
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
wxString Settings::SetupNetworking()
{
    wxString sCommand;
    bool bDHCP = (Settings::Get()->Read(wxT("/network/DHCP"), wxT("0")) == wxT("1"));
    //wxExecute(wxT("sudo iplink set eth0 down"));
    if(bDHCP == false)
    {
        sCommand.Printf(wxT("sudo ifconfig eth0 %s netmask %s"),Settings::Get().Read(wxT("Network"), wxT("Address"),wxEmptyString).c_str(),Settings::Get().Read(wxT("Network"), wxT("Subnet"),wxEmptyString).c_str());
        wxExecute(sCommand);

        sCommand.Printf(wxT("sudo route add default gw %s eth0"), Settings::Get().Read(wxT("Network"), wxT("Gateway"),wxEmptyString).c_str());
        wxExecute(sCommand);
    }
    else
    {
        wxExecute(wxT("sudo ifconfig eth0 0.0.0.0 netmask 0.0.0.0"));
    }
   // wxExecute(wxT("iplink set eth0 up"));

   return wxEmptyString;
}

bool NetworkControl::DeleteNetworking()
{
    return true;
}

#endif // __WXMSW__




