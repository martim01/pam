#ifdef __NMOS__
#include "wxclientapiposter.h"
#include "wxnmosclientevent.h"
#include <wx/log.h>
#include "log.h"

wxDEFINE_EVENT(wxEVT_NMOS_CLIENT_NODE,wxNmosClientNodeEvent);
wxDEFINE_EVENT(wxEVT_NMOS_CLIENT_SOURCE,wxNmosClientSourceEvent);
wxDEFINE_EVENT(wxEVT_NMOS_CLIENT_FLOW,wxNmosClientFlowEvent);
wxDEFINE_EVENT(wxEVT_NMOS_CLIENT_DEVICE,wxNmosClientDeviceEvent);
wxDEFINE_EVENT(wxEVT_NMOS_CLIENT_SENDER,wxNmosClientSenderEvent);
wxDEFINE_EVENT(wxEVT_NMOS_CLIENT_RECEIVER,wxNmosClientReceiverEvent);

wxDEFINE_EVENT(wxEVT_NMOS_CLIENTCURL_SUBSCRIBE, wxNmosClientCurlEvent);
wxDEFINE_EVENT(wxEVT_NMOS_CLIENTCURL_PATCH_SENDER, wxNmosClientCurlEvent);
wxDEFINE_EVENT(wxEVT_NMOS_CLIENTCURL_PATCH_RECEIVER, wxNmosClientCurlEvent);
wxDEFINE_EVENT(wxEVT_NMOS_CLIENTCURL_CONNECT, wxNmosClientCurlEvent);

wxDEFINE_EVENT(wxEVT_NMOS_CLIENTQUERY_FOUND, wxNmosClientQueryEvent);
wxDEFINE_EVENT(wxEVT_NMOS_CLIENTQUERY_REMOVED, wxNmosClientQueryEvent);
wxDEFINE_EVENT(wxEVT_NMOS_CLIENTQUERY_CHANGED, wxNmosClientQueryEvent);


IMPLEMENT_DYNAMIC_CLASS(wxNmosClientNodeEvent, wxCommandEvent)
IMPLEMENT_DYNAMIC_CLASS(wxNmosClientDeviceEvent, wxCommandEvent)
IMPLEMENT_DYNAMIC_CLASS(wxNmosClientSourceEvent, wxCommandEvent)
IMPLEMENT_DYNAMIC_CLASS(wxNmosClientFlowEvent, wxCommandEvent)
IMPLEMENT_DYNAMIC_CLASS(wxNmosClientSenderEvent, wxCommandEvent)
IMPLEMENT_DYNAMIC_CLASS(wxNmosClientReceiverEvent, wxCommandEvent)
IMPLEMENT_DYNAMIC_CLASS(wxNmosClientCurlEvent, wxCommandEvent)

IMPLEMENT_DYNAMIC_CLASS(wxNmosClientQueryEvent, wxCommandEvent)

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


void wxClientApiPoster::NodeChanged(const std::list<std::shared_ptr<pml::nmos::Self> >& lstNodesAdded, const std::list<std::shared_ptr<pml::nmos::Self> >& lstNodesUpdated, const std::list<std::shared_ptr<pml::nmos::Self> >& lstNodesRemoved)
{
    wxNmosClientNodeEvent* pEvent = new wxNmosClientNodeEvent(lstNodesAdded, lstNodesUpdated, lstNodesRemoved);
    wxQueueEvent(m_pHandler, pEvent);
}

void wxClientApiPoster::DeviceChanged(const std::list<std::shared_ptr<pml::nmos::Device> >& lstDevicesAdded, const std::list<std::shared_ptr<pml::nmos::Device> >& lstDevicesUpdated, const std::list<std::shared_ptr<pml::nmos::Device> >& lstDevicesRemoved)
{
    wxNmosClientDeviceEvent* pEvent = new wxNmosClientDeviceEvent(lstDevicesAdded, lstDevicesUpdated, lstDevicesRemoved);
    wxQueueEvent(m_pHandler, pEvent);
}

void wxClientApiPoster::SourceChanged(const std::list<std::shared_ptr<pml::nmos::Source> >& lstSourcesAdded, const std::list<std::shared_ptr<pml::nmos::Source> >& lstSourcesUpdated, const std::list<std::shared_ptr<pml::nmos::Source> >& lstSourcesRemoved)
{
    wxNmosClientSourceEvent* pEvent = new wxNmosClientSourceEvent(lstSourcesAdded, lstSourcesUpdated, lstSourcesRemoved);
    wxQueueEvent(m_pHandler, pEvent);
}

void wxClientApiPoster::FlowChanged(const std::list<std::shared_ptr<pml::nmos::Flow> >& lstFlowsAdded, const std::list<std::shared_ptr<pml::nmos::Flow> >& lstFlowsUpdated, const std::list<std::shared_ptr<pml::nmos::Flow> >& lstFlowsRemoved)
{
    wxNmosClientFlowEvent* pEvent = new wxNmosClientFlowEvent(lstFlowsAdded, lstFlowsUpdated, lstFlowsRemoved);
    wxQueueEvent(m_pHandler, pEvent);
}

void wxClientApiPoster::SenderChanged(const std::list<std::shared_ptr<pml::nmos::SenderBase> >& lstSendersAdded, const std::list<std::shared_ptr<pml::nmos::SenderBase> >& lstSendersUpdated, const std::list<std::shared_ptr<pml::nmos::SenderBase> >& lstSendersRemoved)
{
    pmlLog(pml::LOG_DEBUG) << "NMOS: wxClientApiPoster::SenderChanged";
    wxNmosClientSenderEvent* pEvent = new wxNmosClientSenderEvent(lstSendersAdded, lstSendersUpdated, lstSendersRemoved);
    wxQueueEvent(m_pHandler, pEvent);
}

void wxClientApiPoster::ReceiverChanged(const std::list<std::shared_ptr<pml::nmos::ReceiverBase> >& lstReceiversAdded, const std::list<std::shared_ptr<pml::nmos::ReceiverBase> >& lstReceiversUpdated, const std::list<std::shared_ptr<pml::nmos::ReceiverBase> >& lstReceiversRemoved)
{
    pmlLog(pml::LOG_DEBUG) << "NMOS: wxClientApiPoster::ReceiverChanged";
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
    pmlLog(pml::LOG_DEBUG) << "NMOS: wxClientApiPoster::RequestPatchSenderResult";
    wxNmosClientCurlEvent* pEvent = new wxNmosClientCurlEvent(wxEVT_NMOS_CLIENTCURL_PATCH_SENDER, nResult, wxString::FromUTF8(sResponse.c_str()), wxString::FromUTF8(sResourceId.c_str()));
    wxQueueEvent(m_pHandler, pEvent);
}

void wxClientApiPoster::RequestPatchReceiverResult(unsigned long nResult, const std::string& sResponse, const std::string& sResourceId)
{
    pmlLog(pml::LOG_DEBUG) << "NMOS: wxClientApiPoster::RequestPatchReceiverResult";
    wxNmosClientCurlEvent* pEvent = new wxNmosClientCurlEvent(wxEVT_NMOS_CLIENTCURL_PATCH_RECEIVER, nResult, wxString::FromUTF8(sResponse.c_str()), wxString::FromUTF8(sResourceId.c_str()));
    wxQueueEvent(m_pHandler, pEvent);
}

void wxClientApiPoster::RequestConnectResult(const std::string& sSenderId, const std::string& sReceiverId, bool bSuccess, const std::string& sResponse)
{
    pmlLog(pml::LOG_DEBUG) << "NMOS: wxClientApiPoster::RequestConnectResult";
    wxNmosClientCurlEvent* pEvent = new wxNmosClientCurlEvent(wxEVT_NMOS_CLIENTCURL_CONNECT, bSuccess, wxString::FromUTF8(sResponse.c_str()), wxString::FromUTF8(sReceiverId.c_str()));
    wxQueueEvent(m_pHandler, pEvent);
}


void wxClientApiPoster::RequestGetSenderStagedResult(unsigned long nResult, const std::string& sResponse, const std::string& sResourceId)
{
    pmlLog(pml::LOG_DEBUG) << "NMOS: wxClientApiPoster::RequestGetSenderStagedResult";
}

void wxClientApiPoster::RequestGetSenderActiveResult(unsigned long nResult, const std::string& sResponse, const std::string& sResourceId)
{
    pmlLog(pml::LOG_DEBUG) << "NMOS: wxClientApiPoster::RequestGetSenderActiveResult";
}

void wxClientApiPoster::RequestGetSenderTransportFileResult(unsigned long nResult, const std::string& sResponse, const std::string& sResourceId)
{
    pmlLog(pml::LOG_DEBUG) << "NMOS: wxClientApiPoster::RequestGetSenderTransportFileResult";
}

void wxClientApiPoster::RequestGetReceiverStagedResult(unsigned long nResult, const std::string& sResponse, const std::string& sResourceId)
{
    pmlLog(pml::LOG_DEBUG) << "NMOS: wxClientApiPoster::RequestGetReceiverStagedResult";
}

void wxClientApiPoster::RequestGetReceiverActiveResult(unsigned long nResult, const std::string& sResponse, const std::string& sResourceId)
{
    pmlLog(pml::LOG_DEBUG) << "NMOS: wxClientApiPoster::RequestGetReceiverActiveResult";
}


void wxClientApiPoster::QueryServerFound(const std::string& sUrl, unsigned short nPriority)
{
    wxQueueEvent(m_pHandler, new wxNmosClientQueryEvent(wxEVT_NMOS_CLIENTQUERY_FOUND, sUrl, nPriority));
}

void wxClientApiPoster::QueryServerRemoved(const std::string& sUrl)
{
    wxQueueEvent(m_pHandler, new wxNmosClientQueryEvent(wxEVT_NMOS_CLIENTQUERY_REMOVED, sUrl, 0));
}

void wxClientApiPoster::QueryServerChanged(const std::string& sUrl)
{
    wxQueueEvent(m_pHandler, new wxNmosClientQueryEvent(wxEVT_NMOS_CLIENTQUERY_CHANGED, sUrl, 0));
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





wxNmosClientQueryEvent::wxNmosClientQueryEvent(wxEventType type, const wxString& sUrl, unsigned short nPriority) : wxCommandEvent(type),
    m_sUrl(sUrl),
    m_nPriority(nPriority)
{

}

wxNmosClientQueryEvent::wxNmosClientQueryEvent(const wxNmosClientQueryEvent& event) : wxCommandEvent(event),
m_sUrl(event.GetUrl()),
m_nPriority(event.GetPriority())
{

}


#endif
