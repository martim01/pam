#include "dnssdbrowser.h"
#include "servicebrowser.h"

DEFINE_EVENT_TYPE(wxEVT_BROWSE_FINISHED)
DEFINE_EVENT_TYPE(wxEVT_BROWSE_RESOLVED)

DNSServiceBrowser::DNSServiceBrowser(wxEvtHandler* pHandler) :
    m_pBrowser(new ServiceBrowser(pHandler))
{

}

DNSServiceBrowser::~DNSServiceBrowser()
{
    delete m_pBrowser;
}

bool DNSServiceBrowser::Start()
{
    return m_pBrowser->StartBrowser();
}


std::map<wxString, dnsService*>::const_iterator DNSServiceBrowser::GetServiceBegin() const
{
    return m_pBrowser->m_mServices.begin();
}

std::map<wxString, dnsService*>::const_iterator DNSServiceBrowser::GetServiceEnd() const
{
    return m_pBrowser->m_mServices.end();
}

std::map<wxString, dnsService*>::const_iterator DNSServiceBrowser::FindService(wxString sService) const
{
    return m_pBrowser->m_mServices.find(sService);
}
