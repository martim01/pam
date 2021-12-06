#pragma once
#include "zcposter.h"
#include <wx/event.h>
#include <map>

class wxZCPoster : public pml::dnssd::ZCPoster
{
    public:
        wxZCPoster(wxEvtHandler* pHandler);
        std::shared_ptr<pml::dnssd::dnsInstance> GetInstance(int nId);

    protected:
        void InstanceResolved(std::shared_ptr<pml::dnssd::dnsInstance> pInstance) override;

        /** @brief Called by AvahiBrowser when no more instances of the given service are likely to be found
        *   @param sService the name of the service
        *   @note this is a non-blocking event
        **/
        void AllForNow(const std::string& sService) override;

        /** @brief Called by AvahiBrowser when it has definitely finished browsing
        *   @note this is a non-blocking event
        **/
        void Finished() override;

        /** @brief Called by AvahiBrowser when an error occurs
        *   @note this is a non-blocking event
        **/
        void RegistrationNodeError() override;

        /** @brief Called by AvahiBrowser when a dns service instance has been removed
        *   @param pInstance a pointer to the removed instance.
        *   @param sService the name of the service.
        *   @note this is a non-blocking event
        **/
        void InstanceRemoved(std::shared_ptr<pml::dnssd::dnsInstance> pInstance) override;

        wxEvtHandler* m_pHandler;

        std::map<int, std::shared_ptr<pml::dnssd::dnsInstance> >m_mInstances;
};

wxDECLARE_EXPORTED_EVENT(WXEXPORT, wxEVT_ZC_RESOLVED, wxCommandEvent);
wxDECLARE_EXPORTED_EVENT(WXEXPORT, wxEVT_ZC_ALLFORNOW, wxCommandEvent);
wxDECLARE_EXPORTED_EVENT(WXEXPORT, wxEVT_ZC_FINISHED, wxCommandEvent);
wxDECLARE_EXPORTED_EVENT(WXEXPORT, wxEVT_ZC_ERROR, wxCommandEvent);
wxDECLARE_EXPORTED_EVENT(WXEXPORT, wxEVT_ZC_REMOVED, wxCommandEvent);
