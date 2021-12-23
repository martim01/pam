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

wxDEFINE_EVENT(wxEVT_NMOS_CLIENT_GET_SENDER_TRANSPORTFILE, wxNmosClientRequestEvent);
wxDEFINE_EVENT(wxEVT_NMOS_CLIENT_GET_SENDER_CONSTRAINTS, wxNmosClientRequestEvent);
wxDEFINE_EVENT(wxEVT_NMOS_CLIENT_GET_RECEIVER_CONSTRAINTS, wxNmosClientRequestEvent);
wxDEFINE_EVENT(wxEVT_NMOS_CLIENT_GET_SENDER_STAGED, wxNmosClientRequestEvent);
wxDEFINE_EVENT(wxEVT_NMOS_CLIENT_GET_SENDER_ACTIVE, wxNmosClientRequestEvent);
wxDEFINE_EVENT(wxEVT_NMOS_CLIENT_GET_RECEIVER_STAGED, wxNmosClientRequestEvent);
wxDEFINE_EVENT(wxEVT_NMOS_CLIENT_GET_RECEIVER_ACTIVE, wxNmosClientRequestEvent);
wxDEFINE_EVENT(wxEVT_NMOS_CLIENT_PATCH_SENDER, wxNmosClientRequestEvent);
wxDEFINE_EVENT(wxEVT_NMOS_CLIENT_PATCH_RECEIVER, wxNmosClientRequestEvent);
wxDEFINE_EVENT(wxEVT_NMOS_CLIENT_SUBSCRIBE, wxNmosClientRequestEvent);
wxDEFINE_EVENT(wxEVT_NMOS_CLIENT_CONNECT, wxNmosClientRequestEvent);


wxDEFINE_EVENT(wxEVT_NMOS_CLIENTQUERY_FOUND, wxNmosClientQueryEvent);
wxDEFINE_EVENT(wxEVT_NMOS_CLIENTQUERY_REMOVED, wxNmosClientQueryEvent);
wxDEFINE_EVENT(wxEVT_NMOS_CLIENTQUERY_CHANGED, wxNmosClientQueryEvent);


IMPLEMENT_DYNAMIC_CLASS(wxNmosClientNodeEvent, wxCommandEvent)
IMPLEMENT_DYNAMIC_CLASS(wxNmosClientSourceEvent, wxCommandEvent)
IMPLEMENT_DYNAMIC_CLASS(wxNmosClientDeviceEvent, wxCommandEvent)
IMPLEMENT_DYNAMIC_CLASS(wxNmosClientFlowEvent, wxCommandEvent)
IMPLEMENT_DYNAMIC_CLASS(wxNmosClientSenderEvent, wxCommandEvent)
IMPLEMENT_DYNAMIC_CLASS(wxNmosClientReceiverEvent, wxCommandEvent)

IMPLEMENT_DYNAMIC_CLASS(wxNmosClientRequestEvent, wxCommandEvent)

IMPLEMENT_DYNAMIC_CLASS(wxNmosClientQueryEvent, wxCommandEvent)

wxClientApiPoster::wxClientApiPoster(wxEvtHandler* pHandler) : ClientApiPoster() ,
m_setHandlers({pHandler})
{

}

wxClientApiPoster::~wxClientApiPoster()
{

}

void wxClientApiPoster::AddHandler(wxEvtHandler* pHandler)
{
    m_setHandlers.insert(pHandler);
}

void wxClientApiPoster::RemoveHandler(wxEvtHandler* pHandler)
{
    m_setHandlers.erase(pHandler);
}

void wxClientApiPoster::ModeChanged(bool bQueryApi)
{
    //@todo
}

void wxClientApiPoster::NodeChanged(const pml::nmos::resourcechanges<pml::nmos::Self>& changed)
{
    for(auto pHandler : m_setHandlers)
    {
        wxQueueEvent(pHandler, new wxNmosClientNodeEvent(changed));
    }
}

void wxClientApiPoster::DeviceChanged(const pml::nmos::resourcechanges<pml::nmos::Device>& changed)
{
    for(auto pHandler : m_setHandlers)
    {
        wxQueueEvent(pHandler, new wxNmosClientDeviceEvent(changed));
    }
}

void wxClientApiPoster::SourceChanged(const pml::nmos::resourcechanges<pml::nmos::Source>& changed)
{
    for(auto pHandler : m_setHandlers)
    {
        wxQueueEvent(pHandler, new wxNmosClientSourceEvent(changed));
    }
}

void wxClientApiPoster::FlowChanged(const pml::nmos::resourcechanges<pml::nmos::Flow>& changed)
{
    for(auto pHandler : m_setHandlers)
    {
        wxQueueEvent(pHandler, new wxNmosClientFlowEvent(changed));
    }
}

void wxClientApiPoster::SenderChanged(const pml::nmos::resourcechanges<pml::nmos::Sender>& changed)
{
    for(auto pHandler : m_setHandlers)
    {
        wxQueueEvent(pHandler, new wxNmosClientSenderEvent(changed));
    }
}

void wxClientApiPoster::ReceiverChanged(const pml::nmos::resourcechanges<pml::nmos::Receiver>& changed)
{
    for(auto pHandler : m_setHandlers)
    {
        wxQueueEvent(pHandler, new wxNmosClientReceiverEvent(changed));
    }
}


void wxClientApiPoster::RequestTargetResult(unsigned long nResult, const std::string& sResponse, const std::string& sResourceId)
{
    //@todo
}

void wxClientApiPoster::RequestPatchSenderResult(const pml::nmos::curlResponse& resp, const std::experimental::optional<pml::nmos::connectionSender<pml::nmos::activationResponse>>& con, const std::string& sResourceId)
{
    for(auto pHandler : m_setHandlers)
    {
        wxQueueEvent(pHandler, new wxNmosClientRequestEvent(wxEVT_NMOS_CLIENT_PATCH_SENDER, resp, con, sResourceId));
    }
}

void wxClientApiPoster::RequestPatchReceiverResult(const pml::nmos::curlResponse& resp, const std::experimental::optional<pml::nmos::connectionReceiver<pml::nmos::activationResponse>>& con, const std::string& sResourceId)
{
    for(auto pHandler : m_setHandlers)
    {
        wxQueueEvent(pHandler, new wxNmosClientRequestEvent(wxEVT_NMOS_CLIENT_PATCH_RECEIVER, resp, con, sResourceId));
    }
}

void wxClientApiPoster::RequestGetSenderStagedResult(const pml::nmos::curlResponse& resp, const std::experimental::optional<pml::nmos::connectionSender<pml::nmos::activationResponse>>& con, const std::string& sResourceId)
{
    for(auto pHandler : m_setHandlers)
    {
        wxQueueEvent(pHandler, new wxNmosClientRequestEvent(wxEVT_NMOS_CLIENT_GET_SENDER_STAGED, resp, con, sResourceId));
    }
}

void wxClientApiPoster::RequestGetSenderActiveResult(const pml::nmos::curlResponse& resp, const std::experimental::optional<pml::nmos::connectionSender<pml::nmos::activationResponse>>& con, const std::string& sResourceId)
{
    for(auto pHandler : m_setHandlers)
    {
        wxQueueEvent(pHandler, new wxNmosClientRequestEvent(wxEVT_NMOS_CLIENT_GET_SENDER_ACTIVE, resp, con, sResourceId));
    }
}

void wxClientApiPoster::RequestGetSenderTransportFileResult(const pml::nmos::curlResponse& resp, const std::experimental::optional<std::string>& transportFile, const std::string& sResourceId)
{
    for(auto pHandler : m_setHandlers)
    {
        wxQueueEvent(pHandler, new wxNmosClientRequestEvent(resp, (transportFile ? *transportFile : wxString()), sResourceId));
    }
}

void wxClientApiPoster::RequestGetReceiverStagedResult(const pml::nmos::curlResponse& resp, const std::experimental::optional<pml::nmos::connectionReceiver<pml::nmos::activationResponse>>& con, const std::string& sResourceId)
{
    for(auto pHandler : m_setHandlers)
    {
        wxQueueEvent(pHandler, new wxNmosClientRequestEvent(wxEVT_NMOS_CLIENT_GET_RECEIVER_STAGED, resp, con, sResourceId));
    }
}

void wxClientApiPoster::RequestGetReceiverActiveResult(const pml::nmos::curlResponse& resp, const std::experimental::optional<pml::nmos::connectionReceiver<pml::nmos::activationResponse>>& con, const std::string& sResourceId)
{
    for(auto pHandler : m_setHandlers)
    {
        wxQueueEvent(pHandler, new wxNmosClientRequestEvent(wxEVT_NMOS_CLIENT_GET_RECEIVER_ACTIVE, resp, con, sResourceId));
    }
}

void wxClientApiPoster::RequestGetSenderConstraintsResult(const pml::nmos::curlResponse& resp, const std::vector<pml::nmos::Constraints>& vConstraints, const std::string& sResourceId)
{
    for(auto pHandler : m_setHandlers)
    {
        wxQueueEvent(pHandler, new wxNmosClientRequestEvent(wxEVT_NMOS_CLIENT_GET_SENDER_CONSTRAINTS, resp, vConstraints, sResourceId));
    }
}

void wxClientApiPoster::RequestGetReceiverConstraintsResult(const pml::nmos::curlResponse& resp, const std::vector<pml::nmos::Constraints>& vConstraints, const std::string& sResourceId)
{
    for(auto pHandler : m_setHandlers)
    {
        wxQueueEvent(pHandler, new wxNmosClientRequestEvent(wxEVT_NMOS_CLIENT_GET_RECEIVER_CONSTRAINTS, resp, vConstraints, sResourceId));
    }
}


void wxClientApiPoster::RequestConnectResult(const std::string& sSenderId, const std::string& sReceiverId, bool bSuccess, const std::string& sResponse)
{
    //@todo
}

void wxClientApiPoster::QuerySubscription(const std::string& sSubscriptionId, int nResource, const std::string& sQuery)
{
    //@todo
}

void wxClientApiPoster::QuerySubscriptionRemoved(const std::string& sSubscriptionId)
{
    //@todo
}


void wxClientApiPoster::QueryServerFound(const std::string& sUrl, unsigned short nPriority)
{
    for(auto pHandler : m_setHandlers)
    {
        wxQueueEvent(pHandler, new wxNmosClientQueryEvent(wxEVT_NMOS_CLIENTQUERY_FOUND, sUrl, nPriority));
    }
}

void wxClientApiPoster::QueryServerRemoved(const std::string& sUrl)
{
    for(auto pHandler : m_setHandlers)
    {
        wxQueueEvent(pHandler, new wxNmosClientQueryEvent(wxEVT_NMOS_CLIENTQUERY_REMOVED, sUrl, 0));
    }
}

void wxClientApiPoster::QueryServerChanged(const std::string& sUrl)
{
    for(auto pHandler : m_setHandlers)
    {
        wxQueueEvent(pHandler, new wxNmosClientQueryEvent(wxEVT_NMOS_CLIENTQUERY_CHANGED, sUrl, 0));
    }
}



wxNmosClientRequestEvent::wxNmosClientRequestEvent(wxEventType type, const pml::nmos::curlResponse& resp, const std::experimental::optional<pml::nmos::connectionSender<pml::nmos::activationResponse>>& con, const wxString& sResourceId) :
    wxCommandEvent(type),
    m_resp(resp),
    m_senderResponse(con),
    m_sResourceId(sResourceId)
{

}

wxNmosClientRequestEvent::wxNmosClientRequestEvent(wxEventType type, const pml::nmos::curlResponse& resp, const std::experimental::optional<pml::nmos::connectionReceiver<pml::nmos::activationResponse>>& con, const wxString& sResourceId) :
    wxCommandEvent(type),
    m_resp(resp),
    m_receiverResponse(con),
    m_sResourceId(sResourceId)
{

}

wxNmosClientRequestEvent::wxNmosClientRequestEvent(wxEventType type, const pml::nmos::curlResponse& resp, const std::vector<pml::nmos::Constraints>& vConstraints, const wxString& sResourceId) :
    wxCommandEvent(type),
    m_resp(resp),
    m_vConstraints(vConstraints),
    m_sResourceId(sResourceId)
{

}

wxNmosClientRequestEvent::wxNmosClientRequestEvent(const pml::nmos::curlResponse& resp, const wxString& sTransportFile, const wxString& sResourceId) :
    wxCommandEvent(wxEVT_NMOS_CLIENT_GET_SENDER_TRANSPORTFILE),
    m_resp(resp),
    m_sTransportFile(sTransportFile),
    m_sResourceId(sResourceId)
{

}

wxNmosClientRequestEvent::wxNmosClientRequestEvent(const wxNmosClientRequestEvent& event) :
    wxCommandEvent(event),
    m_resp(event.GetRawResponse()),
    m_senderResponse(event.GetSenderResponse()),
    m_receiverResponse(event.GetReceiverResponse()),
    m_vConstraints(event.GetConstraints()),
    m_sTransportFile(event.GetTransportFile())
{

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
