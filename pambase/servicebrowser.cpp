#ifdef __WXMSW__
#include "servicebrowser.h"

#include <winsock2.h>
#include <ws2tcpip.h>
#include <iphlpapi.h>
#include <iostream>

using namespace std;



void DNSSD_API IterateServiceTypes( DNSServiceRef sdRef, DNSServiceFlags flags, uint32_t interfaceIndex, DNSServiceErrorType errorCode, const char *serviceName, const char *regtype, const char *replyDomain, void *context )
{
    wxLogDebug(wxT("IterateServiceTypes"));
	wxServiceBrowser* pBrowser = (wxServiceBrowser *) context;
    pBrowser->IterateTypes(sdRef, flags, interfaceIndex, errorCode, serviceName, regtype, replyDomain, context);
}


void DNSSD_API IterateServiceInstances( DNSServiceRef sdRef, DNSServiceFlags flags, uint32_t interfaceIndex, DNSServiceErrorType errorCode, const char *serviceName, const char *regtype, const char *replyDomain, void *context )
{
    wxLogDebug(wxT("IterateServiceInstances"));
	wxServiceBrowser* pBrowser = (wxServiceBrowser *) context;
	pBrowser->IterateInstances(sdRef, flags, interfaceIndex, errorCode, serviceName, regtype, replyDomain, context );
}

void DNSSD_API ResolveInstance( DNSServiceRef sdRef, DNSServiceFlags flags, uint32_t interfaceIndex, DNSServiceErrorType errorCode, const char *fullname, const char *hosttarget, uint16_t port, uint16_t txtLen, const unsigned char *txtRecord, void *context )
{
    wxLogDebug(wxT("ResolveInstance"));
	wxServiceBrowser* pBrowser = (wxServiceBrowser *) context;
	pBrowser->Resolve(sdRef, flags, interfaceIndex, errorCode, fullname, hosttarget, port, txtLen, txtRecord, context );
}


void DNSSD_API GetAddress( DNSServiceRef sdRef, DNSServiceFlags flags, uint32_t interfaceIndex, DNSServiceErrorType errorCode, const char *hostname, const struct sockaddr *address, uint32_t ttl, void *context )
{
    wxLogDebug(wxT("GetAddress"));
	wxServiceBrowser* pBrowser = (wxServiceBrowser *) context;
	pBrowser->Address(sdRef, flags, interfaceIndex, errorCode, hostname, address, ttl, context );
}




wxServiceBrowser::wxServiceBrowser(wxEvtHandler* pHandler)
	: m_pHandler(pHandler)
{

	m_timerBrowser.SetOwner(this, wxNewId());
	Connect(m_timerBrowser.GetId(), wxEVT_TIMER, (wxObjectEventFunction)&wxServiceBrowser::OnTimer);

}


bool wxServiceBrowser::StartBrowser(const std::set<wxString>& setServices)
{
    m_setServices = setServices;
	DNSServiceRef client = NULL;
	DNSServiceErrorType err = DNSServiceBrowse(&client, 0, 0, "_services._dns-sd._udp", "", IterateServiceTypes, this );
	if ( err == 0 )
	{
        m_mClientToFd[client] = DNSServiceRefSockFD(client);
		m_timerBrowser.Start(250,false);
		return true;
	}
	wxLogDebug(wxT("Failed to start browser"));
    return false;
}

void wxServiceBrowser::OnTimer(wxTimerEvent& event)
{
    int count = 0;
	for ( ; ; )
	{
        if ( m_mClientToFd.size() == 0 )
		{
            m_timerBrowser.Stop();

            if(m_pHandler)
            {
                wxCommandEvent eventFinished(wxEVT_BROWSE_FINISHED);
                wxPostEvent(m_pHandler, eventFinished);
            }
            break;
        }
		fd_set readfds;
		FD_ZERO(&readfds);
		int nfds = 0;
		for ( auto itClient = m_mClientToFd.cbegin() ; itClient != m_mClientToFd.cend() ; itClient++ )
		{
			FD_SET(itClient->second, &readfds);
			std::cout << itClient->second << std::endl;
			nfds = max((int)itClient->second, nfds);
		}


		std::cout << "SeviceBrowser: Start select: fd =" << m_mClientToFd.size() << " nfds =" << nfds << std::endl;
		struct timeval tv = { 0, 1000 };

		//mDNSPosixGetFDSet(m, &nfds, &readfds, &tv);
		int result = select(nfds, &readfds, (fd_set*)NULL, (fd_set*)NULL, &tv);

		if ( result > 0 )
		{
		    std::cout << "SeviceBrowser: Select done"  << std::endl;
            //
            // While iterating through the loop, the callback functions might delete
            // the client pointed to by the current iterator, so I have to increment
            // it BEFORE calling DNSServiceProcessResult
            //
			for ( auto itClient = m_mClientToFd.cbegin() ; itClient != m_mClientToFd.cend() ; )
			{
                auto jj = itClient++;
				if (FD_ISSET(jj->second, &readfds) )
				{
					DNSServiceErrorType err = DNSServiceProcessResult(jj->first);
                    if ( ++count > 10 )
					{
						break;
					}
				}
			}
		}
		else
		{
		    std::cout << "Result = " << result << std::endl;
			break;
		}
        if ( count > 10 )
        {
			break;
		}
	}
}

void DNSSD_API wxServiceBrowser::IterateTypes( DNSServiceRef sdRef, DNSServiceFlags flags, uint32_t interfaceIndex, DNSServiceErrorType errorCode, const char *serviceName, const char *regtype, const char *replyDomain, void *context )
{
	// Service types are added to the top level of the tree
	//
	if ( flags & kDNSServiceFlagsAdd && !errorCode )
	{
		std::string r( regtype );
		size_t n = r.find_last_of('.');
		if ( n != std::string::npos )
		{
			r = r.substr(0,n);
		}
		n = r.find_last_of('.');
		if ( n != std::string::npos )
		{
			r = r.substr(0,n);
		}
		std::string service_type = serviceName;
        service_type += '.';
		service_type += r.c_str();
		if(m_setServices.find(wxString::FromAscii(service_type.c_str())) != m_setServices.end())
        {
            auto itService = m_setServiceTypes.find( service_type );
            if ( itService == m_setServiceTypes.end() )
            {
                m_setServiceTypes.insert( service_type );
                dnsService* pService = new dnsService(wxString::FromAscii(service_type.c_str()));

                DNSServiceRef client = NULL;
                DNSServiceErrorType err = DNSServiceBrowse( &client,  0,  0,  service_type.c_str(),  "",  IterateServiceInstances,  context );

                if ( err == 0 )
                {
                    m_mClientToFd[client] = DNSServiceRefSockFD(client);
                    m_mServRefToString[client] = pService->sService;
                    m_mServices.insert(make_pair(pService->sService, pService));
                }
                else
                {
                    wxLogDebug(wxT("Error trying to browse service type: %s"), wxString::FromAscii(service_type.c_str()).c_str());
                }
            }
        }
	}
    if ( !(flags & kDNSServiceFlagsMoreComing ) )
	{
        auto itClient = m_mClientToFd.find( sdRef );
        if ( itClient != m_mClientToFd.end() )
		{
			m_mClientToFd.erase( itClient );
		}
        //m_Tree.Invalidate();
    }
}

void DNSSD_API wxServiceBrowser::IterateInstances( DNSServiceRef sdRef, DNSServiceFlags flags, uint32_t interfaceIndex, DNSServiceErrorType errorCode, const char *serviceName, const char *regtype, const char *replyDomain, void *context )
{
	if ( (flags & kDNSServiceFlagsAdd) && !errorCode )
	{
        auto itServToString = m_mServRefToString.find( sdRef );
        if ( itServToString != m_mServRefToString.end() )
		{
		    auto itService = m_mServices.find(itServToString->second);

            dnsInstance* pInstance = new dnsInstance(wxString::FromAscii(serviceName));
            pInstance->sService = itServToString->second;

            DNSServiceRef client = NULL;
            DNSServiceErrorType err = DNSServiceResolve ( &client, 0, interfaceIndex, serviceName, regtype, replyDomain, ResolveInstance, context );

            wxLogDebug(wxT("Resolving instance of %s %s"), wxString::FromAscii(serviceName).c_str(), wxString::FromAscii(regtype).c_str());

            if ( err == 0 )
			{
                m_mClientToFd[client] = DNSServiceRefSockFD(client);
                itService->second->lstInstances.push_back(pInstance);

                m_mInstances[client] = pInstance;
            }
			else
			{
				wxLogDebug(wxT("Error trying to browse service instance: %s"), wxString::FromAscii(serviceName).c_str() );
            }
        }
        else
        {
			wxLogDebug(wxT("???"));
		}
    }
    if ( !(flags & kDNSServiceFlagsMoreComing ) )
	{
        m_mClientToFd.erase(sdRef);
        m_mServRefToString.erase( sdRef );
    }
}

void DNSSD_API wxServiceBrowser::Resolve( DNSServiceRef sdRef, DNSServiceFlags flags, uint32_t interfaceIndex, DNSServiceErrorType errorCode, const char *fullname, const char *hosttarget, uint16_t port, uint16_t txtLen, const unsigned char *txtRecord, void *context )
{
    if ( !errorCode )
	{
        auto itInstance = m_mInstances.find( sdRef );
        if ( itInstance != m_mInstances.end() )
		{
		    itInstance->second->sHostName = wxString::FromAscii(hosttarget);

            DNSServiceRef client = NULL;
            #ifdef __WXMSW__
			MIB_IFROW IfRow;
			IfRow.dwIndex = interfaceIndex;
			DWORD result  = GetIfEntry ( &IfRow );
			#else
			int result = 0;
            #endif
			wxString sAdapter = _T("Unknown");
			if ( result == 0 )
			{
			    #ifdef __WXMSW__
				sAdapter =wxString::FromAscii( (char*)IfRow.bDescr );
				#endif
                DNSServiceErrorType err = DNSServiceGetAddrInfo( &client, kDNSServiceFlagsTimeout, interfaceIndex, kDNSServiceProtocol_IPv4, hosttarget, GetAddress, context );
                if ( err == 0 )
				{
                    m_mClientToFd[client] = DNSServiceRefSockFD(client);

                    m_mInstances[client] = itInstance->second;

					wxString sTarget = wxString::FromAscii(hosttarget);
                    wxLogDebug( wxT("Looking up %s on %s"), sTarget.c_str(), sAdapter.c_str() );
                }
				else
				{
					wxLogDebug( wxT("Error looking up address info for %s"), wxString::FromAscii( hosttarget).c_str() );
                }
			}
			uint8_t lolo = (port >> 0) & 0xFF;
            uint8_t lohi = (port >> 8) & 0xFF;
			itInstance->second->nPort = (lolo << 8) | (lohi << 0);
			itInstance->second->sInterface = sAdapter;

            if  ( errorCode == 0 )
			{
                size_t pos = 0;
                for ( ; ; )
				{
                    if ( pos >= txtLen )
					{
						break;
					}
                    int length = txtRecord[pos] & 0xff;
                    pos++;
                    if ( length == 0 )
                    {
						break;
					}
					wxString sText;
					for(int i = 0; i < length; i++)
                    {
                        sText << wxString::FromAscii(txtRecord[pos]);
                        ++pos;
                    }
					wxLogDebug(wxT("Pos = %zu, Length = %d, %s"), pos, length, sText.c_str());
					itInstance->second->mTxt[sText.BeforeFirst(wxT('='))] = sText.AfterFirst(wxT('='));
                }
            }
        }
        else
		{
			wxLogDebug( wxT("???") );
		}
	}
    if ( !(flags & kDNSServiceFlagsMoreComing ) )
	{
        auto itClient = m_mClientToFd.find( sdRef );
        if ( itClient != m_mClientToFd.end() )
		{
            m_mClientToFd.erase( itClient );
		}
		m_mInstances.erase(sdRef);
    }
}
void DNSSD_API wxServiceBrowser::Address( DNSServiceRef sdRef, DNSServiceFlags flags, uint32_t interfaceIndex, DNSServiceErrorType errorCode, const char *hostname, const struct sockaddr *address, uint32_t ttl, void *context )
{
    if ( !errorCode )
	{
        auto itInstance = m_mInstances.find( sdRef );
        if ( itInstance != m_mInstances.end() )
		{
		    const sockaddr_in *in = (const sockaddr_in *) address;
            char *ip = inet_ntoa( in->sin_addr );
            itInstance->second->sHostIP = wxString::FromAscii(ip);
        }
		else
        {
			wxLogDebug( wxT("???") );
		}

		if(m_pHandler)
        {
            wxCommandEvent event(wxEVT_BROWSE_RESOLVED);
            event.SetClientData(reinterpret_cast<void*>(itInstance->second));
            wxPostEvent(m_pHandler, event);
        }
    }
    if ( !(flags & kDNSServiceFlagsMoreComing ) )
	{
        m_mClientToFd.erase(sdRef);
        m_mInstances.erase(sdRef);
    }
}


wxServiceBrowser::~wxServiceBrowser()
{
    for(map<wxString, dnsService*>::iterator itService = m_mServices.begin(); itService != m_mServices.end(); ++itService)
    {
        delete itService->second;
    }
}
#endif
