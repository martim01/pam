#pragma once
#include "dlldefine.h"
#include <wx/event.h>
#include <map>
#include <list>
#include <set>
class wxServiceBrowser;
struct PAMBASE_IMPEXPORT dnsInstance
{
    dnsInstance(){}
    dnsInstance(wxString sN) : sName(sN), nPort(0){}

    wxString sName;
    wxString sHostName;
    wxString sHostIP;
    wxString sService;
    unsigned long nPort;
    wxString sInterface;
    std::map<wxString, wxString> mTxt;

};


struct PAMBASE_IMPEXPORT dnsService
{
    dnsService(){}
    dnsService(wxString ss) : sService(ss){}

    ~dnsService()
    {
        for(std::list<dnsInstance*>::iterator itInstance = lstInstances.begin(); itInstance != lstInstances.end(); ++itInstance)
        {
            delete (*itInstance);
        }
    }

    wxString sService;
    std::list<dnsInstance*> lstInstances;

};


class PAMBASE_IMPEXPORT DNSServiceBrowser
{
    public:
        DNSServiceBrowser(wxEvtHandler* pHandler);
        ~DNSServiceBrowser();
        bool Start(const std::set<wxString>& setServices);

        #ifdef __WXMSW__
        std::map<wxString, dnsService*>::const_iterator GetServiceBegin() const;
        std::map<wxString, dnsService*>::const_iterator GetServiceEnd() const;
        std::map<wxString, dnsService*>::const_iterator FindService(wxString sService) const;
        #endif // __WXMSW__
    private:
        wxServiceBrowser* m_pBrowser;
};

DECLARE_EXPORTED_EVENT_TYPE(PAMBASE_IMPEXPORT, wxEVT_BROWSE_FINISHED, -1)
DECLARE_EXPORTED_EVENT_TYPE(PAMBASE_IMPEXPORT, wxEVT_BROWSE_RESOLVED, -1)
