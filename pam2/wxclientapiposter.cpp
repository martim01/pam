#ifdef __NMOS__
#include "wxclientapiposter.h"
#include <wx/log.h>


DEFINE_EVENT_TYPE(wxEVT_NMOS_CLIENT_NODE)
DEFINE_EVENT_TYPE(wxEVT_NMOS_CLIENT_SOURCE)
DEFINE_EVENT_TYPE(wxEVT_NMOS_CLIENT_FLOW)
DEFINE_EVENT_TYPE(wxEVT_NMOS_CLIENT_DEVICE)
DEFINE_EVENT_TYPE(wxEVT_NMOS_CLIENT_SENDER)
DEFINE_EVENT_TYPE(wxEVT_NMOS_CLIENT_RECEIVER)
DEFINE_EVENT_TYPE(wxEVT_NMOS_CLIENT_RESOURCES_REMOVED)
DEFINE_EVENT_TYPE(wxEVT_NMOS_CLIENTCURL_SUBSCRIBE)
DEFINE_EVENT_TYPE(wxEVT_NMOS_CLIENTCURL_PATCH_SENDER)
DEFINE_EVENT_TYPE(wxEVT_NMOS_CLIENTCURL_PATCH_RECEIVER)

IMPLEMENT_DYNAMIC_CLASS(wxNmosClientEvent, wxCommandEvent)
IMPLEMENT_DYNAMIC_CLASS(wxNmosClientCurlEvent, wxCommandEvent)

wxClientApiPoster::wxClientApiPoster(wxEvtHandler* pHandler) : ClientApiPoster(),
    m_pHandler(pHandler)
{

}

wxClientApiPoster::~wxClientApiPoster()
{

}

void wxClientApiPoster::NodeChanged(std::shared_ptr<Self> pNode, enumChange eChange)
{
    wxNmosClientEvent* pEvent(new wxNmosClientEvent(pNode, eChange));
    wxQueueEvent(m_pHandler, pEvent);
}

void wxClientApiPoster::DeviceChanged(std::shared_ptr<Device> pDevice, enumChange eChange)
{
    wxNmosClientEvent* pEvent(new wxNmosClientEvent(pDevice, eChange));
    wxQueueEvent(m_pHandler, pEvent);
}

void wxClientApiPoster::SourceChanged(std::shared_ptr<Source> pSource, enumChange eChange)
{
    wxNmosClientEvent* pEvent(new wxNmosClientEvent(pSource, eChange));
    wxQueueEvent(m_pHandler, pEvent);
}

void wxClientApiPoster::FlowChanged(std::shared_ptr<Flow> pFlow, enumChange eChange)
{
    wxNmosClientEvent* pEvent(new wxNmosClientEvent(pFlow, eChange));
    wxQueueEvent(m_pHandler, pEvent);
}

void wxClientApiPoster::SenderChanged(std::shared_ptr<Sender> pSender, enumChange eChange)
{
//    wxLogDebug(wxT("wxClientApiPoster::SenderChanged"));
    wxNmosClientEvent* pEvent(new wxNmosClientEvent(pSender, eChange));
    wxQueueEvent(m_pHandler, pEvent);
}

void wxClientApiPoster::ReceiverChanged(std::shared_ptr<Receiver> pReceiver, enumChange eChange)
{
    wxNmosClientEvent* pEvent(new wxNmosClientEvent(pReceiver, eChange));
    wxQueueEvent(m_pHandler, pEvent);
}

void wxClientApiPoster::NodesRemoved(const std::set<std::string>& setRemoved)
{
    wxNmosClientEvent* pEvent(new wxNmosClientEvent(setRemoved, ClientApi::NODES));
    wxQueueEvent(m_pHandler, pEvent);
}

void wxClientApiPoster::DevicesRemoved(const std::set<std::string>& setRemoved)
{
    wxNmosClientEvent* pEvent(new wxNmosClientEvent(setRemoved, ClientApi::DEVICES));
    wxQueueEvent(m_pHandler, pEvent);
}

void wxClientApiPoster::SourcesRemoved(const std::set<std::string>& setRemoved)
{
    wxNmosClientEvent* pEvent(new wxNmosClientEvent(setRemoved, ClientApi::SOURCES));
    wxQueueEvent(m_pHandler, pEvent);
}

void wxClientApiPoster::FlowsRemoved(const std::set<std::string>& setRemoved)
{
    wxNmosClientEvent* pEvent(new wxNmosClientEvent(setRemoved, ClientApi::FLOWS));
    wxQueueEvent(m_pHandler, pEvent);
}

void wxClientApiPoster::SendersRemoved(const std::set<std::string>& setRemoved)
{
    wxNmosClientEvent* pEvent(new wxNmosClientEvent(setRemoved, ClientApi::SENDERS));
    wxQueueEvent(m_pHandler, pEvent);
}

void wxClientApiPoster::ReceiversRemoved(const std::set<std::string>& setRemoved)
{
    wxNmosClientEvent* pEvent(new wxNmosClientEvent(setRemoved, ClientApi::RECEIVERS));
    wxQueueEvent(m_pHandler, pEvent);
}


void wxClientApiPoster::RequestTargetResult(unsigned long nResult, const std::string& sResponse, const std::string& sResourceId)
{
    //@todo work out why this is causing it to crash
    //wxLogMessage(wxT("%s %s"), wxString::FromAscii(sResponse.c_str()).c_str(), wxString::FromAscii(sResourceId.c_str()).c_str());
    wxNmosClientCurlEvent* pEvent = new wxNmosClientCurlEvent(wxEVT_NMOS_CLIENTCURL_SUBSCRIBE, nResult, wxString::FromAscii(sResponse.c_str()), wxString::FromAscii(sResourceId.c_str()));
    wxQueueEvent(m_pHandler, pEvent);
}

void wxClientApiPoster::RequestPatchSenderResult(unsigned long nResult, const std::string& sResponse, const std::string& sResourceId)
{
    wxNmosClientCurlEvent* pEvent = new wxNmosClientCurlEvent(wxEVT_NMOS_CLIENTCURL_PATCH_SENDER, nResult, wxString::FromAscii(sResponse.c_str()), wxString::FromAscii(sResourceId.c_str()));
    wxQueueEvent(m_pHandler, pEvent);
}

void wxClientApiPoster::RequestPatchReceiverResult(unsigned long nResult, const std::string& sResponse, const std::string& sResourceId)
{
    wxNmosClientCurlEvent* pEvent = new wxNmosClientCurlEvent(wxEVT_NMOS_CLIENTCURL_PATCH_RECEIVER, nResult, wxString::FromAscii(sResponse.c_str()), wxString::FromAscii(sResourceId.c_str()));
    wxQueueEvent(m_pHandler, pEvent);
}


wxNmosClientEvent::wxNmosClientEvent(std::shared_ptr<Self> pNode, ClientApiPoster::enumChange eChange) : wxCommandEvent(wxEVT_NMOS_CLIENT_NODE),
    m_pNode(pNode),
    m_pDevice(0),
    m_pSource(0),
    m_pFlow(0),
    m_pSender(0),
    m_pReceiver(0)
{
    SetInt(eChange);
}

wxNmosClientEvent::wxNmosClientEvent(std::shared_ptr<Device> pDevice, ClientApiPoster::enumChange eChange) : wxCommandEvent(wxEVT_NMOS_CLIENT_DEVICE),
    m_pNode(0),
    m_pDevice(pDevice),
    m_pSource(0),
    m_pFlow(0),
    m_pSender(0),
    m_pReceiver(0)
{
    SetInt(eChange);
}

wxNmosClientEvent::wxNmosClientEvent(std::shared_ptr<Source> pSource, ClientApiPoster::enumChange eChange) : wxCommandEvent(wxEVT_NMOS_CLIENT_SOURCE),
    m_pNode(0),
    m_pDevice(0),
    m_pSource(pSource),
    m_pFlow(0),
    m_pSender(0),
    m_pReceiver(0)
{
    SetInt(eChange);
}

wxNmosClientEvent::wxNmosClientEvent(std::shared_ptr<Flow> pFlow, ClientApiPoster::enumChange eChange) : wxCommandEvent(wxEVT_NMOS_CLIENT_FLOW),
    m_pNode(0),
    m_pDevice(0),
    m_pSource(0),
    m_pFlow(pFlow),
    m_pSender(0),
    m_pReceiver(0)
{
    SetInt(eChange);
}

wxNmosClientEvent::wxNmosClientEvent(std::shared_ptr<Sender> pSender, ClientApiPoster::enumChange eChange) : wxCommandEvent(wxEVT_NMOS_CLIENT_SENDER),
    m_pNode(0),
    m_pDevice(0),
    m_pSource(0),
    m_pFlow(0),
    m_pSender(pSender),
    m_pReceiver(0)
{
    SetInt(eChange);
}

wxNmosClientEvent::wxNmosClientEvent(std::shared_ptr<Receiver> pReceiver, ClientApiPoster::enumChange eChange) : wxCommandEvent(wxEVT_NMOS_CLIENT_RECEIVER),
    m_pNode(0),
    m_pDevice(0),
    m_pSource(0),
    m_pFlow(0),
    m_pSender(0),
    m_pReceiver(pReceiver)
{
    SetInt(eChange);
}

wxNmosClientEvent::wxNmosClientEvent(const std::set<std::string>& setRemoved, ClientApi::flagResource eResource) : wxCommandEvent(wxEVT_NMOS_CLIENT_RESOURCES_REMOVED),
    m_pNode(0),
    m_pDevice(0),
    m_pSource(0),
    m_pFlow(0),
    m_pSender(0),
    m_pReceiver(0),
    m_setRemoved(setRemoved)
{
    SetInt(eResource);
}

wxNmosClientEvent::wxNmosClientEvent(const wxNmosClientEvent& event) : wxCommandEvent(event),
    m_pNode(event.GetNode()),
    m_pDevice(event.GetDevice()),
    m_pSource(event.GetSource()),
    m_pFlow(event.GetFlow()),
    m_pSender(event.GetSender()),
    m_pReceiver(event.GetReceiver()),
    m_setRemoved(event.GetRemovedBegin(), event.GetRemovedEnd())
{
    SetInt(event.GetInt());
}


std::shared_ptr<Self> wxNmosClientEvent::GetNode() const
{
    return m_pNode;
}

std::shared_ptr<Device> wxNmosClientEvent::GetDevice() const
{
    return m_pDevice;
}

std::shared_ptr<Source> wxNmosClientEvent::GetSource() const
{
    return m_pSource;
}

std::shared_ptr<Flow> wxNmosClientEvent::GetFlow() const
{
    return m_pFlow;
}

std::shared_ptr<Sender> wxNmosClientEvent::GetSender() const
{
    return m_pSender;
}

std::shared_ptr<Receiver> wxNmosClientEvent::GetReceiver() const
{
    return m_pReceiver;
}

std::set<std::string>::const_iterator wxNmosClientEvent::GetRemovedBegin() const
{
    return m_setRemoved.begin();
}

std::set<std::string>::const_iterator wxNmosClientEvent::GetRemovedEnd() const
{
    return m_setRemoved.end();
}


wxNmosClientCurlEvent::wxNmosClientCurlEvent(wxEventType type, unsigned long nResult, const wxString& sResponse, const wxString& sResourceId) : wxCommandEvent(type),
    m_sResponse(sResponse.c_str()),
    m_sResourceId(sResourceId.c_str())
{
    SetInt(nResult);

}

wxNmosClientCurlEvent::wxNmosClientCurlEvent(const wxNmosClientCurlEvent& event) : wxCommandEvent(event),
m_sResponse(event.GetResponse().c_str()),
m_sResourceId(event.GetResourceId().c_str())
{

}


unsigned long wxNmosClientCurlEvent::GetResult()
{
    return GetInt();
}

const wxString& wxNmosClientCurlEvent::GetResponse()
{
    return m_sResponse;
}

const wxString& wxNmosClientCurlEvent::GetResourceId()
{
    return m_sResourceId;
}



#endif
