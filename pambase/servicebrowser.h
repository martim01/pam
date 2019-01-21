#pragma once
#ifdef __WXMSW__
#include <string>
#include <wx/wx.h>
#include "dns_sd.h"
#include <wx/event.h>
#include <unordered_map>
#include <unordered_set>
#include <wx/timer.h>
#include <map>
#include <list>
#include "dnssdbrowser.h"



static void DNSSD_API IterateServiceTypes( DNSServiceRef sdRef, DNSServiceFlags flags, uint32_t interfaceIndex, DNSServiceErrorType errorCode, const char *serviceName, const char *regtype,const char *replyDomain, void *context );
static void DNSSD_API IterateServiceInstances( DNSServiceRef sdRef, DNSServiceFlags flags, uint32_t interfaceIndex, DNSServiceErrorType errorCode, const char *serviceName, const char *regtype,const char *replyDomain, void *context );
static void DNSSD_API ResolveInstance(  DNSServiceRef sdRef, DNSServiceFlags flags, uint32_t interfaceIndex, DNSServiceErrorType errorCode, const char *fullname, const char *hosttarget, uint16_t port, uint16_t txtLen, const unsigned char *txtRecord, void *context );
static void DNSSD_API GetAddress( DNSServiceRef sdRef, DNSServiceFlags flags, uint32_t interfaceIndex, DNSServiceErrorType errorCode, const char *hostname, const struct sockaddr *address,uint32_t ttl, void *context );


// CServiceBrowserDlg dialog
class wxServiceBrowser : public wxEvtHandler
{
// Construction
public:
	wxServiceBrowser(wxEvtHandler* pHandler);
    ~wxServiceBrowser();
	bool StartBrowser(const std::set<wxString>& setServices);


	std::map<wxString, dnsService*> m_mServices;

    void DNSSD_API IterateTypes( DNSServiceRef sdRef, DNSServiceFlags flags, uint32_t interfaceIndex, DNSServiceErrorType errorCode, const char *serviceName, const char *regtype,const char *replyDomain, void *context );
	void DNSSD_API IterateInstances( DNSServiceRef sdRef, DNSServiceFlags flags, uint32_t interfaceIndex, DNSServiceErrorType errorCode, const char *serviceName, const char *regtype,const char *replyDomain, void *context );
    void DNSSD_API Resolve(  DNSServiceRef sdRef, DNSServiceFlags flags, uint32_t interfaceIndex, DNSServiceErrorType errorCode, const char *fullname, const char *hosttarget, uint16_t port, uint16_t txtLen, const unsigned char *txtRecord, void *context );
    void DNSSD_API Address( DNSServiceRef sdRef, DNSServiceFlags flags, uint32_t interfaceIndex, DNSServiceErrorType errorCode, const char *hostname, const struct sockaddr *address,uint32_t ttl, void *context );

    protected:

        std::map<DNSServiceRef,int> m_mClientToFd;
        std::map<DNSServiceRef,wxString> m_mServRefToString;
        std::map<DNSServiceRef,dnsInstance*> m_mInstances;
        std::unordered_set<std::string> m_setServiceTypes;
        std::unordered_set<std::string> m_setServiceInstances;


	void OnTimer(wxTimerEvent& event);
    std::set<wxString> m_setServices;

	wxEvtHandler* m_pHandler;
	wxTimer m_timerBrowser;
};
#endif


