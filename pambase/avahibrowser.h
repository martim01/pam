#pragma once
#ifdef __WXGNU__
#include "dnssdbrowser.h"
#include <wx/thread.h>
#include <avahi-client/client.h>
#include <avahi-client/lookup.h>
#include <avahi-common/thread-watch.h>
#include <avahi-common/malloc.h>
#include <avahi-common/error.h>
#include <set>
#include <map>
#include <wx/event.h>
#include <wx/timer.h>

static void client_callback(AvahiClient * pClient, AvahiClientState state, AVAHI_GCC_UNUSED void * userdata);
static void type_callback(AvahiServiceTypeBrowser* stb, AvahiIfIndex interface, AvahiProtocol protocol, AvahiBrowserEvent event, const char* type, const char* domain, AvahiLookupResultFlags flags, void* userdata);
static void resolve_callback(AvahiServiceResolver *r, AVAHI_GCC_UNUSED AvahiIfIndex interface, AVAHI_GCC_UNUSED AvahiProtocol protocol, AvahiResolverEvent event,const char *name, const char *type, const char *domain, const char *host_name, const AvahiAddress *address, uint16_t port, AvahiStringList *txt,AvahiLookupResultFlags flags,AVAHI_GCC_UNUSED void* userdata);
static void browse_callback(AvahiServiceBrowser *b, AvahiIfIndex interface, AvahiProtocol protocol, AvahiBrowserEvent event, const char *name, const char *type, const char *domain, AVAHI_GCC_UNUSED AvahiLookupResultFlags flags, void* userdata);
// CServiceBrowserDlg dialog
class ServiceBrowser : public wxEvtHandler
{
// Construction
    public:
        ServiceBrowser(wxEvtHandler* pHandler);
        ~ServiceBrowser();


        bool StartBrowser(const std::set<wxString>& setServices);

        void ClientCallback(AvahiClient* pClient, AvahiClientState state);
        void TypeCallback(AvahiIfIndex interface, AvahiProtocol protocol, AvahiBrowserEvent event, const char* type, const char* domain);
        void BrowseCallback(AvahiServiceBrowser* pBrowser, AvahiIfIndex interface, AvahiProtocol protocol, AvahiBrowserEvent event, const char *name, const char *type, const char *domain);
        void ResolveCallback(AvahiServiceResolver* pResolver, AvahiResolverEvent event,const char *name, const char *type, const char *domain, const char *host_name, const AvahiAddress *address, uint16_t port, AvahiStringList *txt);


    protected:

        void LogError(const wxString& sError);
        void DeleteAllServices();
        bool Start(AvahiClient* pClient);
        void Stop();
        void CheckStop();
        void OnStop(wxCommandEvent& event);
        wxEvtHandler* m_pHandler;
        AvahiThreadedPoll* m_pThreadedPoll;
        AvahiClient * m_pClient;
        AvahiServiceTypeBrowser* m_pTypeBrowser;

        std::set<wxString> m_setServices;
        bool m_bBrowsing;
        unsigned long m_nWaitingOn;
        std::set<AvahiServiceBrowser*> m_setBrowser;
        std::map<wxString, dnsService*> m_mServices;
};


#endif // __WXGNU__
