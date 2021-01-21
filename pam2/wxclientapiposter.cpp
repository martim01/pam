#ifdef __NMOS__
#include "wxclientapiposter.h"
#include "wxnmosclientevent.h"
#include <wx/log.h>


DEFINE_EVENT_TYPE(wxEVT_NMOS_CLIENT_NODE)
DEFINE_EVENT_TYPE(wxEVT_NMOS_CLIENT_SOURCE)
DEFINE_EVENT_TYPE(wxEVT_NMOS_CLIENT_FLOW)
DEFINE_EVENT_TYPE(wxEVT_NMOS_CLIENT_DEVICE)
DEFINE_EVENT_TYPE(wxEVT_NMOS_CLIENT_SENDER)
DEFINE_EVENT_TYPE(wxEVT_NMOS_CLIENT_RECEIVER)

DEFINE_EVENT_TYPE(wxEVT_NMOS_CLIENTCURL_SUBSCRIBE)
DEFINE_EVENT_TYPE(wxEVT_NMOS_CLIENTCURL_PATCH_SENDER)
DEFINE_EVENT_TYPE(wxEVT_NMOS_CLIENTCURL_PATCH_RECEIVER)
DEFINE_EVENT_TYPE(wxEVT_NMOS_CLIENTCURL_CONNECT)

IMPLEMENT_DYNAMIC_CLASS(wxNmosClientNodeEvent, wxCommandEvent)
IMPLEMENT_DYNAMIC_CLASS(wxNmosClientDeviceEvent, wxCommandEvent)
IMPLEMENT_DYNAMIC_CLASS(wxNmosClientSourceEvent, wxCommandEvent)
IMPLEMENT_DYNAMIC_CLASS(wxNmosClientFlowEvent, wxCommandEvent)
IMPLEMENT_DYNAMIC_CLASS(wxNmosClientSenderEvent, wxCommandEvent)
IMPLEMENT_DYNAMIC_CLASS(wxNmosClientReceiverEvent, wxCommandEvent)
IMPLEMENT_DYNAMIC_CLASS(wxNmosClientCurlEvent, wxCommandEvent)

wxClientApiPoster::wxClientApiPoster(wxEvtHandler* pHandler) : ClientApiPoster(),
    m_pHandler(pHandler)
{

}

wxClientApiPoster::~wxClientApiPoster()
{

}

void wxClientApiPoster::ModeChanged(bool bQueryApi)
{
    // @todo wxClientApiPoster::ModeChanged
}

void wxClientApiPoster::QuerySubscription(const std::string& sSubscriptionId, int nResource, const std::string& sQuery)
{
    //@todo wxClientApiPoster::QuerySubscription
}

void wxClientApiPoster::QuerySubscriptionRemoved(const std::string& sSubscriptionId)
{
    //@todo wxClientApiPoster::QuerySubscriptionRemoved
}


void wxClientApiPoster::NodeChanged(const std::list<std::shared_ptr<Self> >& lstNodesAdded, const std::list<std::shared_ptr<Self> >& lstNodesUpdated, const std::list<std::shared_ptr<Self> >& lstNodesRemoved)
{
    wxNmosClientNodeEvent* pEvent = new wxNmosClientNodeEvent(lstNodesAdded, lstNodesUpdated, lstNodesRemoved);
    wxQueueEvent(m_pHandler, pEvent);
}

void wxClientApiPoster::DeviceChanged(const std::list<std::shared_ptr<Device> >& lstDevicesAdded, const std::list<std::shared_ptr<Device> >& lstDevicesUpdated, const std::list<std::shared_ptr<Device> >& lstDevicesRemoved)
{
    wxNmosClientDeviceEvent* pEvent = new wxNmosClientDeviceEvent(lstDevicesAdded, lstDevicesUpdated, lstDevicesRemoved);
    wxQueueEvent(m_pHandler, pEvent);
}

void wxClientApiPoster::SourceChanged(const std::list<std::shared_ptr<Source> >& lstSourcesAdded, const std::list<std::shared_ptr<Source> >& lstSourcesUpdated, const std::list<std::shared_ptr<Source> >& lstSourcesRemoved)
{
    wxNmosClientSourceEvent* pEvent = new wxNmosClientSourceEvent(lstSourcesAdded, lstSourcesUpdated, lstSourcesRemoved);
    wxQueueEvent(m_pHandler, pEvent);
}

void wxClientApiPoster::FlowChanged(const std::list<std::shared_ptr<Flow> >& lstFlowsAdded, const std::list<std::shared_ptr<Flow> >& lstFlowsUpdated, const std::list<std::shared_ptr<Flow> >& lstFlowsRemoved)
{
    wxNmosClientFlowEvent* pEvent = new wxNmosClientFlowEvent(lstFlowsAdded, lstFlowsUpdated, lstFlowsRemoved);
    wxQueueEvent(m_pHandler, pEvent);
}

void wxClientApiPoster::SenderChanged(const std::list<std::shared_ptr<Sender> >& lstSendersAdded, const std::list<std::shared_ptr<Sender> >& lstSendersUpdated, const std::list<std::shared_ptr<Sender> >& lstSendersRemoved)
{
    wxNmosClientSenderEvent* pEvent = new wxNmosClientSenderEvent(lstSendersAdded, lstSendersUpdated, lstSendersRemoved);
    wxQueueEvent(m_pHandler, pEvent);
}

void wxClientApiPoster::ReceiverChanged(const std::list<std::shared_ptr<Receiver> >& lstReceiversAdded, const std::list<std::shared_ptr<Receiver> >& lstReceiversUpdated, const std::list<std::shared_ptr<Receiver> >& lstReceiversRemoved)
{
    wxNmosClientReceiverEvent* pEvent = new wxNmosClientReceiverEvent(lstReceiversAdded, lstReceiversUpdated, lstReceiversRemoved);
    wxQueueEvent(m_pHandler, pEvent);
}

void wxClientApiPoster::RequestTargetResult(unsigned long nResult, const std::string& sResponse, const std::string& sResourceId)
{
    //@todo work out why this is causing it to crash
    //wxLogMessage(wxT("%s %s"), wxString::FromUTF8(sResponse.c_str()).c_str(), wxString::FromUTF8(sResourceId.c_str()).c_str());
    wxNmosClientCurlEvent* pEvent = new wxNmosClientCurlEvent(wxEVT_NMOS_CLIENTCURL_SUBSCRIBE, nResult, wxString::FromUTF8(sResponse.c_str()), wxString::FromUTF8(sResourceId.c_str()));
    wxQueueEvent(m_pHandler, pEvent);
}

void wxClientApiPoster::RequestPatchSenderResult(unsigned long nResult, const std::string& sResponse, const std::string& sResourceId)
{
    wxNmosClientCurlEvent* pEvent = new wxNmosClientCurlEvent(wxEVT_NMOS_CLIENTCURL_PATCH_SENDER, nResult, wxString::FromUTF8(sResponse.c_str()), wxString::FromUTF8(sResourceId.c_str()));
    wxQueueEvent(m_pHandler, pEvent);
}

void wxClientApiPoster::RequestPatchReceiverResult(unsigned long nResult, const std::string& sResponse, const std::string& sResourceId)
{
    wxNmosClientCurlEvent* pEvent = new wxNmosClientCurlEvent(wxEVT_NMOS_CLIENTCURL_PATCH_RECEIVER, nResult, wxString::FromUTF8(sResponse.c_str()), wxString::FromUTF8(sResourceId.c_str()));
    wxQueueEvent(m_pHandler, pEvent);
}

void wxClientApiPoster::RequestConnectResult(const std::string& sSenderId, const std::string& sReceiverId, bool bSuccess, const std::string& sResponse)
{
    wxNmosClientCurlEvent* pEvent = new wxNmosClientCurlEvent(wxEVT_NMOS_CLIENTCURL_CONNECT, bSuccess, wxString::FromUTF8(sResponse.c_str()), wxString::FromUTF8(sReceiverId.c_str()));
    wxQueueEvent(m_pHandler, pEvent);
}


void wxClientApiPoster::RequestGetSenderStagedResult(unsigned long nResult, const std::string& sResponse, const std::string& sResourceId)
{

}

void wxClientApiPoster::RequestGetSenderActiveResult(unsigned long nResult, const std::string& sResponse, const std::string& sResourceId)
{

}

void wxClientApiPoster::RequestGetSenderTransportFileResult(unsigned long nResult, const std::string& sResponse, const std::string& sResourceId)
{

}

void wxClientApiPoster::RequestGetReceiverStagedResult(unsigned long nResult, const std::string& sResponse, const std::string& sResourceId)
{

}

void wxClientApiPoster::RequestGetReceiverActiveResult(unsigned long nResult, const std::string& sResponse, const std::string& sResourceId)
{

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


unsigned long wxNmosClientCurlEvent::GetResult() const
{
    return GetInt();
}

const wxString& wxNmosClientCurlEvent::GetResponse() const
{
    return m_sResponse;
}

const wxString& wxNmosClientCurlEvent::GetResourceId() const
{
    return m_sResourceId;
}





#endif
