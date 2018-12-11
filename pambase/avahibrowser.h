#pragma once
#ifdef __WXGNU__
#include "dnssdbrowser.h"
#include <wx/thread.h>
#include <avahi-client/client.h>
#include <avahi-client/lookup.h>
#include <avahi-common/simple-watch.h>
#include <avahi-common/malloc.h>
#include <avahi-common/error.h>
#include <set>
#include <map>

static void client_callback(AvahiClient * pClient, AvahiClientState state, AVAHI_GCC_UNUSED void * userdata);
static void type_callback(AvahiServiceTypeBrowser* stb, AvahiIfIndex interface, AvahiProtocol protocol, AvahiBrowserEvent event, const char* type, const char* domain, AvahiLookupResultFlags flags, void* userdata);
static void resolve_callback(AvahiServiceResolver *r, AVAHI_GCC_UNUSED AvahiIfIndex interface, AVAHI_GCC_UNUSED AvahiProtocol protocol, AvahiResolverEvent event,const char *name, const char *type, const char *domain, const char *host_name, const AvahiAddress *address, uint16_t port, AvahiStringList *txt,AvahiLookupResultFlags flags,AVAHI_GCC_UNUSED void* userdata);
static void browse_callback(AvahiServiceBrowser *b, AvahiIfIndex interface, AvahiProtocol protocol, AvahiBrowserEvent event, const char *name, const char *type, const char *domain, AVAHI_GCC_UNUSED AvahiLookupResultFlags flags, void* userdata);
// CServiceBrowserDlg dialog
class ServiceBrowser : public wxThread
{
// Construction
    public:
        ServiceBrowser(wxEvtHandler* pHandler);
        ~ServiceBrowser();


        bool StartBrowser();

        void ClientCallback(AvahiClientState state);
        void TypeCallback(AvahiIfIndex interface, AvahiProtocol protocol, AvahiBrowserEvent event, const char* type, const char* domain);
        void BrowseCallback(AvahiServiceBrowser* pBrowser, AvahiIfIndex interface, AvahiProtocol protocol, AvahiBrowserEvent event, const char *name, const char *type, const char *domain);
        void ResolveCallback(AvahiServiceResolver* pResolver, AvahiResolverEvent event,const char *name, const char *type, const char *domain, const char *host_name, const AvahiAddress *address, uint16_t port, AvahiStringList *txt);


    protected:
        void* Entry();

        void LogError(const wxString& sError);


        wxEvtHandler* m_pHandler;
        AvahiSimplePoll* m_pSimplePoll;
        AvahiClient * m_pClient;
        AvahiServiceTypeBrowser* m_pTypeBrowser;
        bool m_bLoop;
        std::set<AvahiServiceBrowser*> m_setBrowser;

        std::map<wxString, dnsService*> m_mServices;
};


#endif // __WXGNU__
