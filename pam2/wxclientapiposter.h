#pragma once
#ifdef __NMOS__
#include "clientapiposter.h"
#include <wx/event.h>
#include <memory>
#include "clientapi.h"
#include <list>

class wxClientApiPoster : public ClientApiPoster
{
    public:
        wxClientApiPoster(wxEvtHandler* pHandler);
        virtual ~wxClientApiPoster();


    protected:
        virtual void ModeChanged(bool bQueryApi);

        /** @brief called when one or more Self resources that match a subscribed query are added, updated or removed
        *   @param lstNodesAdded a list of Selfs that have been added
        *   @param lstNodesUpdated a list of Selfs that have been updated
        *   @param lstNodesRemoved a list of Selfs that have been removed
        **/
        virtual void NodeChanged(const std::list<std::shared_ptr<Self> >& lstNodesAdded, const std::list<std::shared_ptr<Self> >& lstNodesUpdated, const std::list<std::shared_ptr<Self> >& lstNodesRemoved);

        /** @brief called when one or more Device resources that match a subscribed query are added, updated or removed
        *   @param lstNodesAdded a list of Devices that have been added
        *   @param lstNodesUpdated a list of Devices that have been updated
        *   @param lstNodesRemoved a list of Devices that have been removed
        **/
        virtual void DeviceChanged(const std::list<std::shared_ptr<Device> >& lstDevicesAdded, const std::list<std::shared_ptr<Device> >& lstDevicesUpdated, const std::list<std::shared_ptr<Device> >& lstDevicesRemoved);

        /** @brief called when one or more Source resources that match a subscribed query are added, updated or removed
        *   @param lstNodesAdded a list of Sources that have been added
        *   @param lstNodesUpdated a list of Sources that have been updated
        *   @param lstNodesRemoved a list of Sources that have been removed
        **/
        virtual void SourceChanged(const std::list<std::shared_ptr<Source> >& lstSourcesAdded, const std::list<std::shared_ptr<Source> >& lstSourcesUpdated, const std::list<std::shared_ptr<Source> >& lstSourcesRemoved);

        /** @brief called when one or more Flow resources that match a subscribed query are added, updated or removed
        *   @param lstNodesAdded a list of Flows that have been added
        *   @param lstNodesUpdated a list of Flows that have been updated
        *   @param lstNodesRemoved a list of Flows that have been removed
        **/
        virtual void FlowChanged(const std::list<std::shared_ptr<Flow> >& lstFlowsAdded, const std::list<std::shared_ptr<Flow> >& lstFlowsUpdated, const std::list<std::shared_ptr<Flow> >& lstFlowsRemoved);

        /** @brief called when one or more Sender resources that match a subscribed query are added, updated or removed
        *   @param lstNodesAdded a list of Senders that have been added
        *   @param lstNodesUpdated a list of Senders that have been updated
        *   @param lstNodesRemoved a list of Senders that have been removed
        **/
        virtual void SenderChanged(const std::list<std::shared_ptr<Sender> >& lstSendersAdded, const std::list<std::shared_ptr<Sender> >& lstSendersUpdated, const std::list<std::shared_ptr<Sender> >& lstSendersRemoved);

        /** @brief called when one or more Receiver resources that match a subscribed query are added, updated or removed
        *   @param lstNodesAdded a list of Receivers that have been added
        *   @param lstNodesUpdated a list of Receivers that have been updated
        *   @param lstNodesRemoved a list of Receivers that have been removed
        **/
        virtual void ReceiverChanged(const std::list<std::shared_ptr<Receiver> >& lstReceiversAdded, const std::list<std::shared_ptr<Receiver> >& lstReceiversUpdated, const std::list<std::shared_ptr<Receiver> >& lstReceiversRemoved);

        /** @brief Called when when a query is subscibed (either with the query node or in p2p mode)
        *   @param sSubscriptionId the return id of the subscription. On failure this will be an empty string
        *   @param nResource the nmos resource type we are querying
        *   @param sQuery the query string if any
        **/
        virtual void QuerySubscription(const std::string& sSubscriptionId, int nResource, const std::string& sQuery);

        /** @brief Called when a query subscritption is removed
        *   @param sSubscriptionId the id of the remove subscription
        **/
        virtual void QuerySubscriptionRemoved(const std::string& sSubscriptionId);


        /** @brief Called when a ClientApi::Subscribe or ClientApi::Unsubscribe gets an answer from the target webserver
        *   @param nResult the http result code
        *   @param sResonse on success this will contain the uuId of the Sender, on failure this will contain a message with the reason for failure
        *   @param sResourceId contains the uuId of the Receiver
        **/
        virtual void RequestTargetResult(unsigned long nResult, const std::string& sResponse, const std::string& sResourceId);


        /** @brief Called when a ClientAPI::PatchSenderStaged gets an answer for the target webserver
        *   @param nResult the http result code
        *   @param sResonse on success this will contain the raw json, on failure this will contain a message with the reason for failure
        *   @param sResourceId contains the uuId of the Sender
        **/
        virtual void RequestPatchSenderResult(unsigned long nResult, const std::string& sResponse, const std::string& sResourceId);

        /** @brief Called when a ClientAPI::PatchReceiverStaged gets an answer for the target webserver
        *   @param nResult the http result code
        *   @param sResonse on success this will contain the raw json, on failure this will contain a message with the reason for failure
        *   @param sResourceId contains the uuId of the Sender
        **/
        virtual void RequestPatchReceiverResult(unsigned long nResult, const std::string& sResponse, const std::string& sResourceId);

        /** @brief Called when a ClientAPI::Connect or ClientApi::Disconnect finishes
        *   @param sSenderId the uuid of the Sender that has been asked to be connected to
        *   @param sReceiverId the uuid of the Receiver that is been connected
        *   @param bSuccess true if the connection has been made, else false
        *   @param sResponse on failure this will contain a message describing the fault. On success it will contain the staged state of the receiver
        **/
        virtual void RequestConnectResult(const std::string& sSenderId, const std::string& sReceiverId, bool bSuccess, const std::string& sResponse);

        /** @brief Called when a ClientAPI::RequestSenderStaged gets an answer for the target webserver
        *   @param nResult the http result code
        *   @param sResonse on success this will contain the raw json, on failure this will contain a message with the reason for failure
        *   @param sResourceId contains the uuId of the Sender
        **/
        virtual void RequestGetSenderStagedResult(unsigned long nResult, const std::string& sResponse, const std::string& sResourceId);

        /** @brief Called when a ClientAPI::RequestSenderActive gets an answer for the target webserver
        *   @param nResult the http result code
        *   @param sResonse on success this will contain the raw json, on failure this will contain a message with the reason for failure
        *   @param sResourceId contains the uuId of the Sender
        **/
        virtual void RequestGetSenderActiveResult(unsigned long nResult, const std::string& sResponse, const std::string& sResourceId);

        /** @brief Called when a ClientAPI::RequestSenderTransportFile gets an answer for the target webserver
        *   @param nResult the http result code
        *   @param sResonse on success this will contain the raw sdp, on failure this will contain a message with the reason for failure
        *   @param sResourceId contains the uuId of the Sender
        **/
        virtual void RequestGetSenderTransportFileResult(unsigned long nResult, const std::string& sResponse, const std::string& sResourceId);

        /** @brief Called when a ClientAPI::RequestReceiverStaged gets an answer for the target webserver
        *   @param nResult the http result code
        *   @param sResonse on success this will contain the raw json, on failure this will contain a message with the reason for failure
        *   @param sResourceId contains the uuId of the Receiver
        **/
        virtual void RequestGetReceiverStagedResult(unsigned long nResult, const std::string& sResponse, const std::string& sResourceId);

        /** @brief Called when a ClientAPI::RequestReceiverActive gets an answer for the target webserver
        *   @param nResult the http result code
        *   @param sResonse on success this will contain the raw json, on failure this will contain a message with the reason for failure
        *   @param sResourceId contains the uuId of the Receiver
        **/
        virtual void RequestGetReceiverActiveResult(unsigned long nResult, const std::string& sResponse, const std::string& sResourceId);
    private:
        wxEvtHandler* m_pHandler;

};






class wxNmosClientCurlEvent : public wxCommandEvent
{

public:

    wxNmosClientCurlEvent(wxEventType type, unsigned long nResult, const wxString& sResponse, const wxString& sResourceId);

    wxNmosClientCurlEvent() : wxCommandEvent(){}

    /** Copy Constructor
    *   @param event a wxNIEvent
    **/
    wxNmosClientCurlEvent(const wxNmosClientCurlEvent& event);

    /** Destructor
    **/
    ~wxNmosClientCurlEvent(){}

    /** Creates a copy of the wxNmosClientEvent
    *   @return <i>wxNmosClientEvent</i>
    **/
    virtual wxEvent *Clone() const { return new wxNmosClientCurlEvent(*this); }

    unsigned long GetResult() const;
    const wxString& GetResponse() const;
    const wxString& GetResourceId() const;


    DECLARE_DYNAMIC_CLASS(wxNmosClientCurlEvent)

    private:
        wxString m_sResponse;
        wxString m_sResourceId;

};



wxDECLARE_EXPORTED_EVENT(WXEXPORT, wxEVT_NMOS_CLIENTCURL_SUBSCRIBE, wxNmosClientCurlEvent);
wxDECLARE_EXPORTED_EVENT(WXEXPORT, wxEVT_NMOS_CLIENTCURL_PATCH_SENDER, wxNmosClientCurlEvent);
wxDECLARE_EXPORTED_EVENT(WXEXPORT, wxEVT_NMOS_CLIENTCURL_PATCH_RECEIVER, wxNmosClientCurlEvent);
wxDECLARE_EXPORTED_EVENT(WXEXPORT, wxEVT_NMOS_CLIENTCURL_CONNECT, wxNmosClientCurlEvent);


#endif


