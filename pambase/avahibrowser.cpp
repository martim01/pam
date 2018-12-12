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
    pBrowser->ClientCallback(pClient, state);
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
    m_pThreadedPoll(0),
    m_pClient(0),
    m_pTypeBrowser(0),
    m_bBrowsing(false)

{
    Connect(wxID_ANY, wxEVT_BROWSE_FINISHED, (wxObjectEventFunction)&ServiceBrowser::OnStop);
}

ServiceBrowser::~ServiceBrowser()
{
    Stop();
    DeleteAllServices();
}

void ServiceBrowser::DeleteAllServices()
{
    for(map<wxString, dnsService*>::iterator itService = m_mServices.begin(); itService != m_mServices.end(); ++itService)
    {
        delete itService->second;
    }
    m_mServices.clear();
}


bool ServiceBrowser::StartBrowser(const std::set<wxString>& setServices)
{
    m_setServices = setServices;
    int error;
    int ret = 1;
    /* Allocate main loop object */
    if (!(m_pThreadedPoll = avahi_threaded_poll_new()))
    {
        wmLog::Get()->Log(wxT("Failed to create Threaded poll object."));
        return false;
    }

    /* Allocate a new client */
    avahi_client_new(avahi_threaded_poll_get(m_pThreadedPoll), AVAHI_CLIENT_NO_FAIL, client_callback, reinterpret_cast<void*>(this), &error);
    avahi_threaded_poll_start(m_pThreadedPoll);

    return true;
}

void ServiceBrowser::Stop()
{
    if(m_pThreadedPoll)
    {
        avahi_threaded_poll_stop(m_pThreadedPoll);
    }

    for(set<AvahiServiceBrowser*>::iterator itBrowser = m_setBrowser.begin(); itBrowser != m_setBrowser.end(); ++itBrowser)
    {
        avahi_service_browser_free((*itBrowser));
    }

    if(m_pTypeBrowser)
    {
        avahi_service_type_browser_free(m_pTypeBrowser);
        m_pTypeBrowser = 0;
    }
    if(m_pClient)
    {
        avahi_client_free(m_pClient);
        m_pClient = 0;
    }
    if(m_pThreadedPoll)
    {
        avahi_threaded_poll_free(m_pThreadedPoll);
        m_pThreadedPoll = 0;

        if(m_pHandler)
        {   //only send if we are actually stopping not already stopped and now deleting
            wxCommandEvent event(wxEVT_BROWSE_FINISHED);
            wxPostEvent(m_pHandler, event);
        }
    }
    m_nWaitingOn = 0;
}


bool ServiceBrowser::Start(AvahiClient* pClient)
{
    if(!m_bBrowsing)
    {
        m_pClient = pClient;
        if(!(m_pTypeBrowser = avahi_service_type_browser_new(pClient, AVAHI_IF_UNSPEC, AVAHI_PROTO_UNSPEC, NULL, (AvahiLookupFlags)0, type_callback, reinterpret_cast<void*>(this))))
        {
            LogError(wxT("Failed to create service type browser"));
            return false;
        }
        m_nWaitingOn = 1;

    }
    return true;
}

void ServiceBrowser::ClientCallback(AvahiClient * pClient, AvahiClientState state)
{
    wmLog::Get()->Log(wxT("ClientCallback"));
    switch (state)
    {
        case AVAHI_CLIENT_FAILURE:
            wmLog::Get()->Log(wxT("ClientCallback: failure"));
            if (avahi_client_errno(pClient) == AVAHI_ERR_DISCONNECTED)
            {
                int error;
                /* We have been disconnected, so let reconnect */
                wmLog::Get()->Log(wxT("Avahi: Disconnected, reconnecting ..."));

                avahi_client_free(pClient);
                m_pClient = NULL;

                DeleteAllServices();

                if (!(avahi_client_new(avahi_threaded_poll_get(m_pThreadedPoll), AVAHI_CLIENT_NO_FAIL, client_callback, reinterpret_cast<void*>(this), &error)))
                {
                    LogError(wxT("Failed to create client object: "));
                    Stop();

                }
            }
            else
            {
                LogError(wxT("Server connection failure"));
                Stop();
            }
            break;
        case AVAHI_CLIENT_S_REGISTERING:
        case AVAHI_CLIENT_S_RUNNING:
        case AVAHI_CLIENT_S_COLLISION:
            wmLog::Get()->Log(wxT("S_"));
            if (!m_bBrowsing)
            {
                Start(pClient);
            }
            break;
        case AVAHI_CLIENT_CONNECTING:
            wmLog::Get()->Log(wxT("Waiting for daemon ..."));
            break;
    }

}

void ServiceBrowser::TypeCallback(AvahiIfIndex interface, AvahiProtocol protocol, AvahiBrowserEvent event, const char* type, const char* domain)
{
    switch(event)
    {
        case AVAHI_BROWSER_NEW:
            {
                wxString sService(wxString::FromAscii(type));

                if(m_setServices.find(sService) != m_setServices.end())
                {
                    wmLog::Get()->Log(wxString::Format(wxT("Service %s found"), sService.c_str()));
                    m_mServices.insert(make_pair(sService, new dnsService(sService)));

                    AvahiServiceBrowser* psb = NULL;
                    /* Create the service browser */
                    if (!(psb = avahi_service_browser_new(m_pClient, interface, protocol, type, domain, (AvahiLookupFlags)0, browse_callback, reinterpret_cast<void*>(this))))
                    {
                        LogError(wxT("Failed to create service browser"));
                    }
                    else
                    {
                        m_setBrowser.insert(psb);
                        m_nWaitingOn++;
                    }
                }
                else
                {
                    wmLog::Get()->Log(wxString::Format(wxT("Service %s not for us"), sService.c_str()));
                }
            }
            break;
             case AVAHI_BROWSER_REMOVE:
                /* We're dirty and never remove the browser again */
                break;
        case AVAHI_BROWSER_FAILURE:
            LogError(wxT("service_type_browser failed"));
            Stop();
            break;
        case AVAHI_BROWSER_CACHE_EXHAUSTED:
            wmLog::Get()->Log(wxT("(TypeBrowser) CACHE_EXHAUSTED"));
            break;
        case AVAHI_BROWSER_ALL_FOR_NOW:
            wmLog::Get()->Log(wxT("(TypeBrowser) ALL_FOR_NOW"));
            CheckStop();
            break;
    }
}

void ServiceBrowser::BrowseCallback(AvahiServiceBrowser* pBrowser, AvahiIfIndex interface, AvahiProtocol protocol, AvahiBrowserEvent event, const char *name, const char *type, const char *domain)
{
    /* Called whenever a new services becomes available on the LAN or is removed from the LAN */
    switch (event)
    {
        case AVAHI_BROWSER_FAILURE:
            LogError(wxT("Browser"));
            Stop();
            break;
        case AVAHI_BROWSER_NEW:
            {
                wxString sService(wxString::FromAscii(type));
                wxString sName(wxString::FromAscii(name));

                wmLog::Get()->Log(wxString::Format(wxT("(Browser) NEW: service '%s' of type '%s' in domain '%s'"), sName.c_str(), sService.c_str(), wxString::FromAscii(domain).c_str()));

                /* We ignore the returned resolver object. In the callback
                   function we free it. If the server is terminated before
                   the callback function is called the server will free
                   the resolver for us. */
                if (!(avahi_service_resolver_new(m_pClient, interface, protocol, name, type, domain, AVAHI_PROTO_UNSPEC, (AvahiLookupFlags)0, resolve_callback, reinterpret_cast<void*>(this))))
                {
                    LogError(wxString::Format(wxT("Failed to resolve service '%s'"), wxString::FromAscii(name).c_str()));
                }
                else
                {
                    m_nWaitingOn++;
                }
            }
            break;
        case AVAHI_BROWSER_REMOVE:
            wmLog::Get()->Log(wxString::Format(wxT("(Browser) REMOVE: service '%s' of type '%s' in domain '%s"), wxString::FromAscii(name).c_str(), wxString::FromAscii(type).c_str(), wxString::FromAscii(domain).c_str()));
            break;
        case AVAHI_BROWSER_ALL_FOR_NOW:
            {
                wmLog::Get()->Log(wxString::Format("(Browser) ALL_FOR_NOW"));
                set<AvahiServiceBrowser*>::iterator itBrowser = m_setBrowser.find(pBrowser);
                if(itBrowser != m_setBrowser.end())
                {
                    avahi_service_browser_free((*itBrowser));
                    m_setBrowser.erase(itBrowser);
                }
                CheckStop();
            }
            break;
        case AVAHI_BROWSER_CACHE_EXHAUSTED:
            {
                wmLog::Get()->Log(wxString::Format("(Browser) CACHE_EXHAUSTED"));
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
    CheckStop();
}

void ServiceBrowser::CheckStop()
{
    --m_nWaitingOn;
    if(m_nWaitingOn == 0)
    {
        wxCommandEvent event(wxEVT_BROWSE_FINISHED);
        wxPostEvent(this, event);
    }
}

void ServiceBrowser::OnStop(wxCommandEvent& event)
{
    Stop();
}

void ServiceBrowser::LogError(const wxString& sError)
{
    wmLog::Get()->Log(wxString::Format(wxT("%s: %s"),sError.c_str(), wxString::FromAscii(avahi_strerror(avahi_client_errno(m_pClient))).c_str()));
}

#endif // __WXGTK
