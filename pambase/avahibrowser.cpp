#ifdef __WXGTK__
#include "avahibrowser.h"
//#include <stdio.h>
//#include <assert.h>
//#include <stdlib.h>
//#include <time.h>

#include "wmlogevent.h"

using namespace std;

void client_callback(AvahiClient * pClient, AvahiClientState state, AVAHI_GCC_UNUSED void * userdata)
{
    ServiceBrowser* pBrowser = reinterpret_cast<ServiceBrowser*>(userdata);
    pBrowser->ClientCallback(state);
}

void type_callback(AvahiServiceTypeBrowser* stb, AvahiIfIndex interface, AvahiProtocol protocol, AvahiBrowserEvent event, const char* type, const char* domain, AvahiLookupResultFlags flags, void* userdata)
{
    ServiceBrowser* pBrowser = reinterpret_cast<ServiceBrowser*>(userdata);
    pBrowser->TypeCallback(interface, protocol, event, type, domain);
}

void browse_callback(AvahiServiceBrowser *b, AvahiIfIndex interface, AvahiProtocol protocol, AvahiBrowserEvent event, const char *name, const char *type, const char *domain, AVAHI_GCC_UNUSED AvahiLookupResultFlags flags, void* userdata)
{
    ServiceBrowser* pBrowser = reinterpret_cast<ServiceBrowser*>(userdata);
    pBrowser->BrowseCallback(b, interface, protocol, event, name, type, domain);
}

void resolve_callback(AvahiServiceResolver *r, AVAHI_GCC_UNUSED AvahiIfIndex interface, AVAHI_GCC_UNUSED AvahiProtocol protocol, AvahiResolverEvent event,const char *name, const char *type, const char *domain, const char *host_name, const AvahiAddress *address, uint16_t port, AvahiStringList *txt,AvahiLookupResultFlags flags,AVAHI_GCC_UNUSED void* userdata)
{
    ServiceBrowser* pBrowser = reinterpret_cast<ServiceBrowser*>(userdata);
    pBrowser->ResolveCallback(r, event, name, type, domain,host_name, address,port,txt);

}







ServiceBrowser::ServiceBrowser(wxEvtHandler* pHandler) :
    m_pHandler(pHandler),
    m_pSimplePoll(0),
    m_pClient(0),
    m_pTypeBrowser(0),
    m_bLoop(true)
{

}

ServiceBrowser::~ServiceBrowser()
{
    for(map<wxString, dnsService*>::iterator itService = m_mServices.begin(); itService != m_mServices.end(); ++itService)
    {
        delete itService->second;
    }
}


bool ServiceBrowser::StartBrowser()
{
    int error;
    int ret = 1;
    /* Allocate main loop object */
    if (!(m_pSimplePoll = avahi_simple_poll_new()))
    {
        wmLog::Get()->Log(wxT("Failed to create simple poll object."));
        return false;
    }

    /* Allocate a new client */
    m_pClient = avahi_client_new(avahi_simple_poll_get(m_pSimplePoll), (AvahiClientFlags)0, client_callback, reinterpret_cast<void*>(this), &error);
    /* Check wether creating the client object succeeded */
    if (!m_pClient)
    {
        LogError(wxT("Failed to create client"));
        avahi_simple_poll_free(m_pSimplePoll);
        return false;
    }

    if(!(m_pTypeBrowser = avahi_service_type_browser_new(m_pClient, AVAHI_IF_UNSPEC, AVAHI_PROTO_UNSPEC, NULL, (AvahiLookupFlags)0, type_callback, reinterpret_cast<void*>(this))))
    {
        LogError(wxT("Failed to create service type browser"));
        avahi_client_free(m_pClient);
        avahi_simple_poll_free(m_pSimplePoll);
        return false;

    }

    Run();


    return true;
}

void* ServiceBrowser::Entry()
{
    /* Run the main loop */
    while(TestDestroy() == false && m_bLoop)
    {
        avahi_simple_poll_iterate(m_pSimplePoll, 100);
    }

    for(set<AvahiServiceBrowser*>::iterator itBrowser = m_setBrowser.begin(); itBrowser != m_setBrowser.end(); ++itBrowser)
    {
        avahi_service_browser_free((*itBrowser));
    }

    avahi_service_type_browser_free(m_pTypeBrowser);
    avahi_client_free(m_pClient);
    avahi_simple_poll_free(m_pSimplePoll);
    return NULL;
}

void ServiceBrowser::ClientCallback(AvahiClientState state)
{
    if(m_pClient)
    {
        /* Called whenever the client or server state changes */
        if (state == AVAHI_CLIENT_FAILURE)
        {
            LogError(wxT("Server connection failure"));
            m_bLoop = false;
        }
    }
}

void ServiceBrowser::TypeCallback(AvahiIfIndex interface, AvahiProtocol protocol, AvahiBrowserEvent event, const char* type, const char* domain)
{
    switch(event)
    {
        case AVAHI_BROWSER_NEW:
        {
            wxString sService(wxString::FromAscii(type));
            wmLog::Get()->Log(wxString::Format(wxT("Service %s found"), sService.c_str()));

            m_mServices.insert(make_pair(sService, new dnsService(sService)));

            AvahiServiceBrowser* psb = NULL;

            /* Create the service browser */
            if (!(psb = avahi_service_browser_new(m_pClient, interface, protocol, type, domain, (AvahiLookupFlags)0, browse_callback, reinterpret_cast<void*>(this))))
            {
                LogError(wxT("Failed to create service browser"));
            }
            m_setBrowser.insert(psb);
            break;
        }
    }
}

void ServiceBrowser::BrowseCallback(AvahiServiceBrowser* pBrowser, AvahiIfIndex interface, AvahiProtocol protocol, AvahiBrowserEvent event, const char *name, const char *type, const char *domain)
{
    /* Called whenever a new services becomes available on the LAN or is removed from the LAN */
    switch (event)
    {
        case AVAHI_BROWSER_FAILURE:
            LogError(wxT("Browser"));
            m_bLoop = false;
            break;
        case AVAHI_BROWSER_NEW:
            {
                wxString sService(wxString::FromAscii(type));
                wxString sName(wxString::FromAscii(name));

                wmLog::Get()->Log(wxString::Format(wxT("(Browser) NEW: service '%s' of type '%s' in domain '%s'"), sName.c_str(), sService.c_str(), wxString::FromAscii(domain).c_str()));
//                map<wxString, dnsService>::iterator itService = m_mService.find(sService);
//                if(itService != m_mService.end())
//                {
//                    itService->second->lstInstances.push_back(new dnsInstance(sName));
//                }

                /* We ignore the returned resolver object. In the callback
                   function we free it. If the server is terminated before
                   the callback function is called the server will free
                   the resolver for us. */
                if (!(avahi_service_resolver_new(m_pClient, interface, protocol, name, type, domain, AVAHI_PROTO_UNSPEC, (AvahiLookupFlags)0, resolve_callback, reinterpret_cast<void*>(this))))
                {
                    LogError(wxString::Format(wxT("Failed to resolve service '%s'"), wxString::FromAscii(name).c_str()));
                }
            }
            break;
        case AVAHI_BROWSER_REMOVE:
            wmLog::Get()->Log(wxString::Format(wxT("(Browser) REMOVE: service '%s' of type '%s' in domain '%s"), wxString::FromAscii(name).c_str(), wxString::FromAscii(type).c_str(), wxString::FromAscii(domain).c_str()));
            break;
        case AVAHI_BROWSER_ALL_FOR_NOW:
        case AVAHI_BROWSER_CACHE_EXHAUSTED:
            wmLog::Get()->Log(wxString::Format("(Browser) ALL_FOR_NOW"));
            set<AvahiServiceBrowser*>::iterator itBrowser = m_setBrowser.find(pBrowser);
            if(itBrowser != m_setBrowser.end())
            {
                avahi_service_browser_free((*itBrowser));
                m_setBrowser.erase(itBrowser);
            }
            break;
    }
}

void ServiceBrowser::ResolveCallback(AvahiServiceResolver* pResolver, AvahiResolverEvent event,const char *name, const char *type, const char *domain, const char *host_name, const AvahiAddress *address, uint16_t port, AvahiStringList *txt)
{
    if(pResolver)
    {
        AvahiLookupResultFlags flags =0;
        wxString sName(wxString::FromAscii(name));
        wxString sService(wxString::FromAscii(type));
        wxString sDomain(wxString::FromAscii(domain));
        /* Called whenever a service has been resolved successfully or timed out */
        switch (event)
        {
            case AVAHI_RESOLVER_FAILURE:
                LogError(wxString::Format(wxT("(Resolver) Failed to resolve service '%s' of type '%s' in domain '%s'"), sName.c_str(), sService.c_str(), sDomain.c_str()));
                break;
            case AVAHI_RESOLVER_FOUND:
            {
                char a[AVAHI_ADDRESS_STR_MAX];
                avahi_address_snprint(a, sizeof(a), address);

                map<wxString, dnsService*>::iterator itService = m_mServices.find(sService);
                dnsInstance* pInstance = new dnsInstance(sName);
                pInstance->sHostName = wxString::FromAscii(host_name);
                pInstance->nPort = port;
                pInstance->sHostIP = wxString::FromAscii(a);
                pInstance->sService = sService;

                for(AvahiStringList* pIterator = txt; pIterator; pIterator = avahi_string_list_get_next(pIterator))
                {
                    wxString sPair(wxString::FromAscii(avahi_string_list_get_text(pIterator)));
                    pInstance->mTxt.insert(make_pair(sPair.BeforeFirst(wxT('=')), sPair.AfterFirst(wxT('='))));
                }
                itService->second->lstInstances.push_back(pInstance);
                if(m_pHandler)
                {

                    wxCommandEvent event(wxEVT_BROWSE_RESOLVED);
                    event.SetClientData(reinterpret_cast<void*>(pInstance));
                    wxPostEvent(m_pHandler, event);
                }
                wmLog::Get()->Log(wxString::Format(wxT("Instance %s resolved at %s"), pInstance->sName.c_str(), pInstance->sHostIP.c_str()));

            }
        }
    }
    avahi_service_resolver_free(pResolver);
}

void ServiceBrowser::LogError(const wxString& sError)
{
    wmLog::Get()->Log(wxString::Format(wxT("%s: %s"),sError.c_str(), wxString::FromAscii(avahi_strerror(avahi_client_errno(m_pClient))).c_str()));
}

#endif // __WXGTK
