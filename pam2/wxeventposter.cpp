#ifdef __NMOS__
#include "wxeventposter.h"
#include "log.h"
#include "mdns.h"
#include <wx/log.h>


wxDEFINE_EVENT(wxEVT_NMOS_TARGET,wxNmosNodeConnectionEvent);
wxDEFINE_EVENT(wxEVT_NMOS_PATCH_SENDER,wxNmosNodeConnectionEvent);
wxDEFINE_EVENT(wxEVT_NMOS_PATCH_RECEIVER,wxNmosNodeConnectionEvent);
wxDEFINE_EVENT(wxEVT_NMOS_ACTIVATE_RECEIVER,wxNmosNodeConnectionEvent);
wxDEFINE_EVENT(wxEVT_NMOS_ACTIVATE_SENDER,wxNmosNodeConnectionEvent);


wxDEFINE_EVENT(wxEVT_NMOS_REGNODE_FOUND, wxNmosNodeRegistrationEvent);
wxDEFINE_EVENT(wxEVT_NMOS_REGNODE_REMOVED, wxNmosNodeRegistrationEvent);
wxDEFINE_EVENT(wxEVT_NMOS_REGNODE_CHANGED, wxNmosNodeRegistrationEvent);
wxDEFINE_EVENT(wxEVT_NMOS_REGNODE_CHOSEN, wxNmosNodeRegistrationEvent);
wxDEFINE_EVENT(wxEVT_NMOS_REGISTRATION_CHANGED, wxNmosNodeRegistrationEvent);

wxEventPoster::wxEventPoster(wxEvtHandler* pHandler)
{
    if(pHandler)
    {
        m_lstHandlers.push_back(pHandler);
    }
}


void wxEventPoster::Target(const std::string& sReceiverId, const std::string& sTransportFile, unsigned short nPort)
{
    for(auto pHandler : m_lstHandlers)
    {
        wxNmosNodeConnectionEvent* pEvent = new wxNmosNodeConnectionEvent(wxEVT_NMOS_TARGET);
        pEvent->SetResourceId(sReceiverId);
        pEvent->SetTransportFile(sTransportFile);
        pEvent->SetPort(nPort);
        wxQueueEvent(pHandler, pEvent);
    }
}

void wxEventPoster::PatchSender(const std::string& sSenderId, const pml::nmos::connectionSender& conPatch, unsigned short nPort)
{
    for(auto pHandler : m_lstHandlers)
    {
        wxNmosNodeConnectionEvent* pEvent = new wxNmosNodeConnectionEvent(wxEVT_NMOS_PATCH_SENDER);
        pEvent->SetResourceId(sSenderId);
        pEvent->SetSenderConnection(conPatch);
        pEvent->SetPort(nPort);
        wxQueueEvent(pHandler, pEvent);
    }
}

void wxEventPoster::PatchReceiver(const std::string& sReceiverId, const pml::nmos::connectionReceiver& conPatch, unsigned short nPort)
{
    for(auto pHandler : m_lstHandlers)
    {
        wxNmosNodeConnectionEvent* pEvent = new wxNmosNodeConnectionEvent(wxEVT_NMOS_PATCH_RECEIVER);
        pEvent->SetResourceId(sReceiverId);
        pEvent->SetReceiverConnection(conPatch);
        pEvent->SetPort(nPort);
        wxQueueEvent(pHandler, pEvent);
    }
}

void wxEventPoster::SenderActivated(const std::string& sSenderId)
{
    for(auto pHandler : m_lstHandlers)
    {
        wxNmosNodeConnectionEvent* pEvent = new wxNmosNodeConnectionEvent(wxEVT_NMOS_ACTIVATE_SENDER);
        pEvent->SetResourceId(sSenderId);
        wxQueueEvent(pHandler, pEvent);
    }
}

void wxEventPoster::ReceiverActivated(const std::string& sReceiverId)
{
    for(auto pHandler : m_lstHandlers)
    {
        wxNmosNodeConnectionEvent* pEvent = new wxNmosNodeConnectionEvent(wxEVT_NMOS_ACTIVATE_RECEIVER);
        pEvent->SetResourceId(sReceiverId);
        wxQueueEvent(pHandler, pEvent);
    }
}


void wxEventPoster::RegistrationNodeFound(const std::string& sUrl, unsigned short nPriority, const pml::nmos::ApiVersion& version)
{
    for(auto pHandler : m_lstHandlers)
    {
        wxNmosNodeRegistrationEvent* pEvent = new wxNmosNodeRegistrationEvent(wxEVT_NMOS_REGNODE_FOUND);
        pEvent->SetNodeUrl(sUrl);
        pEvent->SetNodePriority(nPriority);
        pEvent->SetNodeVersion(version);
        pEvent->SetNodeStatus(true);
        wxQueueEvent(pHandler, pEvent);
    }
}

void wxEventPoster::RegistrationNodeRemoved(const std::string& sUrl)
{
    for(auto pHandler : m_lstHandlers)
    {
        wxNmosNodeRegistrationEvent* pEvent = new wxNmosNodeRegistrationEvent(wxEVT_NMOS_REGNODE_REMOVED);
        pEvent->SetNodeUrl(sUrl);
        wxQueueEvent(pHandler, pEvent);
    }
}

void wxEventPoster::wxEventPoster::RegistrationNodeChanged(const std::string& sUrl, unsigned short nPriority, bool bGood, const pml::nmos::ApiVersion& version)
{
    for(auto pHandler : m_lstHandlers)
    {
        wxNmosNodeRegistrationEvent* pEvent = new wxNmosNodeRegistrationEvent(wxEVT_NMOS_REGNODE_CHANGED);
        pEvent->SetNodeUrl(sUrl);
        pEvent->SetNodePriority(nPriority);
        pEvent->SetNodeVersion(version);
        pEvent->SetNodeStatus(bGood);
        wxQueueEvent(pHandler, pEvent);
    }
}

void wxEventPoster::RegistrationNodeChosen(const std::string& sUrl, unsigned short nPriority, const pml::nmos::ApiVersion& version)
{
    for(auto pHandler : m_lstHandlers)
    {
        wxNmosNodeRegistrationEvent* pEvent = new wxNmosNodeRegistrationEvent(wxEVT_NMOS_REGNODE_CHOSEN);
        pEvent->SetNodeUrl(sUrl);
        pEvent->SetNodePriority(nPriority);
        pEvent->SetNodeVersion(version);
        pEvent->SetNodeStatus(true);
        wxQueueEvent(pHandler, pEvent);
    }
}

void wxEventPoster::RegistrationChanged(const std::string& sUrl, pml::nmos::EventPoster::enumRegState eState)
{
    for(auto pHandler : m_lstHandlers)
    {
        wxNmosNodeRegistrationEvent* pEvent = new wxNmosNodeRegistrationEvent(wxEVT_NMOS_REGISTRATION_CHANGED);
        pEvent->SetRegistered(eState);
        wxQueueEvent(pHandler, pEvent);
    }
}


IMPLEMENT_DYNAMIC_CLASS(wxNmosNodeConnectionEvent, wxCommandEvent)

wxNmosNodeConnectionEvent::wxNmosNodeConnectionEvent(wxEventType type) : wxCommandEvent(type)
{

}

wxNmosNodeConnectionEvent::wxNmosNodeConnectionEvent(const wxNmosNodeConnectionEvent& event) : wxCommandEvent(event),
m_sTransportFile(event.GetTransportFile()),
m_conSender(event.GetSenderConnection()),
m_conReceiver(event.GetReceiverConnection()),
m_sResourceId(event.GetResourceId()),
m_nPort(event.GetPort())
{

}

void wxNmosNodeConnectionEvent::SetTransportFile(const std::string& sTransportFile)
{
    m_sTransportFile = sTransportFile;
}

const wxString& wxNmosNodeConnectionEvent::GetTransportFile() const
{
    return m_sTransportFile;
}

void wxNmosNodeConnectionEvent::SetSenderConnection(const pml::nmos::connectionSender& con)
{
    m_conSender = con;
}

const pml::nmos::connectionSender& wxNmosNodeConnectionEvent::GetSenderConnection() const
{
    return m_conSender;
}

void wxNmosNodeConnectionEvent::SetReceiverConnection(const pml::nmos::connectionReceiver& con)
{
    m_conReceiver = con;
}

const pml::nmos::connectionReceiver& wxNmosNodeConnectionEvent::GetReceiverConnection() const
{
    return m_conReceiver;
}



IMPLEMENT_DYNAMIC_CLASS(wxNmosNodeRegistrationEvent, wxCommandEvent)


wxNmosNodeRegistrationEvent::wxNmosNodeRegistrationEvent(wxEventType type) : wxCommandEvent(type),
m_nPriority(-1),
m_bGood(false),
m_eState(pml::nmos::EventPoster::enumRegState::NODE_PEER)
{

}

wxNmosNodeRegistrationEvent::wxNmosNodeRegistrationEvent(const wxNmosNodeRegistrationEvent& event) : wxCommandEvent(event),
  m_sUrl(event.GetNodeUrl()),
  m_nPriority(event.GetNodePriority()),
  m_version(event.GetNodeVersion()),
  m_bGood(event.GetNodeStatus()),
  m_eState(event.GetRegistered())
{

}




#endif
