#include "wxzxposter.h"
#include <memory>

wxDEFINE_EVENT(wxEVT_ZC_RESOLVED, wxCommandEvent);
wxDEFINE_EVENT(wxEVT_ZC_ALLFORNOW, wxCommandEvent);
wxDEFINE_EVENT(wxEVT_ZC_FINISHED, wxCommandEvent);
wxDEFINE_EVENT(wxEVT_ZC_ERROR, wxCommandEvent);
wxDEFINE_EVENT(wxEVT_ZC_REMOVED, wxCommandEvent);


wxZCPoster::wxZCPoster(wxEvtHandler* pHandler) : m_pHandler(pHandler)
{

}

void wxZCPoster::InstanceResolved(std::shared_ptr<pml::dnsInstance> pInstance)
{
    if(m_pHandler)
    {
        int nId = m_mInstances.insert(std::make_pair(wxNewId(), pInstance)).first->first;
        wxCommandEvent* pEvent = new wxCommandEvent(wxEVT_ZC_RESOLVED);
        pEvent->SetInt(nId);
        wxQueueEvent(m_pHandler, pEvent);
    }
}

void wxZCPoster::AllForNow(const std::string& sService)
{
    if(m_pHandler)
    {
        wxCommandEvent* pEvent = new wxCommandEvent(wxEVT_ZC_ALLFORNOW);
        pEvent->SetString(wxString(sService));
        wxQueueEvent(m_pHandler, pEvent);
    }
}

void wxZCPoster::Finished()
{
    if(m_pHandler)
    {
        wxCommandEvent* pEvent = new wxCommandEvent(wxEVT_ZC_FINISHED);
        wxQueueEvent(m_pHandler, pEvent);
    }
}

void wxZCPoster::RegistrationNodeError()
{
    if(m_pHandler)
    {
        wxCommandEvent* pEvent = new wxCommandEvent(wxEVT_ZC_ERROR);
        wxQueueEvent(m_pHandler, pEvent);
    }
}

void wxZCPoster::InstanceRemoved(std::shared_ptr<pml::dnsInstance> pInstance)
{
    if(m_pHandler)
    {
        wxCommandEvent* pEvent = new wxCommandEvent(wxEVT_ZC_REMOVED);
        int nId = -1;
        for(auto pairInstance : m_mInstances)
        {
            if(pairInstance.second == pInstance)
            {
                nId = pairInstance.first;
                break;
            }
        }
        pEvent->SetInt(nId);
        wxQueueEvent(m_pHandler, pEvent);
    }
}

std::shared_ptr<pml::dnsInstance> wxZCPoster::GetInstance(int nId)
{
    auto itInstance = m_mInstances.find(nId);
    if(itInstance != m_mInstances.end())
    {
        return itInstance->second;
    }
    else
    {
        return nullptr;
    }
}
