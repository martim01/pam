#pragma once
#ifdef __NMOS__
#include "clientapiposter.h"
#include <wx/event.h>
#include <memory>
#include "clientapi.h"
#include <list>

class wxClientApiPoster : public pml::nmos::ClientApiPoster
{
    public:
        wxClientApiPoster(wxEvtHandler* pHandler);
        ~wxClientApiPoster();


    protected:
        void ModeChanged(bool bQueryApi) override;

        /** @brief called when one or more Self resources that match a subscribed query are added, updated or removed
        *   @param lstNodesAdded a list of Selfs that have been added
        *   @param lstNodesUpdated a list of Selfs that have been updated
        *   @param lstNodesRemoved a list of Selfs that have been removed
        **/
        void NodeChanged(const std::list<std::shared_ptr<pml::nmos::Self> >& lstNodesAdded, const std::list<std::shared_ptr<pml::nmos::Self> >& lstNodesUpdated, const std::list<std::shared_ptr<pml::nmos::Self> >& lstNodesRemoved) override;

        /** @brief called when one or more Device resources that match a subscribed query are added, updated or removed
        *   @param lstNodesAdded a list of Devices that have been added
        *   @param lstNodesUpdated a list of Devices that have been updated
        *   @param lstNodesRemoved a list of Devices that have been removed
        **/
        void DeviceChanged(const std::list<std::shared_ptr<pml::nmos::Device> >& lstDevicesAdded, const std::list<std::shared_ptr<pml::nmos::Device> >& lstDevicesUpdated, const std::list<std::shared_ptr<pml::nmos::Device> >& lstDevicesRemoved) override;

        /** @brief called when one or more Source resources that match a subscribed query are added, updated or removed
        *   @param lstNodesAdded a list of Sources that have been added
        *   @param lstNodesUpdated a list of Sources that have been updated
        *   @param lstNodesRemoved a list of Sources that have been removed
        **/
        void SourceChanged(const std::list<std::shared_ptr<pml::nmos::Source> >& lstSourcesAdded, const std::list<std::shared_ptr<pml::nmos::Source> >& lstSourcesUpdated, const std::list<std::shared_ptr<pml::nmos::Source> >& lstSourcesRemoved) override;

        /** @brief called when one or more Flow resources that match a subscribed query are added, updated or removed
        *   @param lstNodesAdded a list of Flows that have been added
        *   @param lstNodesUpdated a list of Flows that have been updated
        *   @param lstNodesRemoved a list of Flows that have been removed
        **/
        void FlowChanged(const std::list<std::shared_ptr<pml::nmos::Flow> >& lstFlowsAdded, const std::list<std::shared_ptr<pml::nmos::Flow> >& lstFlowsUpdated, const std::list<std::shared_ptr<pml::nmos::Flow> >& lstFlowsRemoved) override;

        /** @brief called when one or more Sender resources that match a subscribed query are added, updated or removed
        *   @param lstNodesAdded a list of Senders that have been added
        *   @param lstNodesUpdated a list of Senders that have been updated
        *   @param lstNodesRemoved a list of Senders that have been removed
        **/
        void SenderChanged(const std::list<std::shared_ptr<pml::nmos::SenderBase> >& lstSendersAdded, const std::list<std::shared_ptr<pml::nmos::SenderBase> >& lstSendersUpdated, const std::list<std::shared_ptr<pml::nmos::SenderBase> >& lstSendersRemoved) override;

        /** @brief called when one or more Receiver resources that match a subscribed query are added, updated or removed
        *   @param lstNodesAdded a list of Receivers that have been added
        *   @param lstNodesUpdated a list of Receivers that have been updated
        *   @param lstNodesRemoved a list of Receivers that have been removed
        **/
        void ReceiverChanged(const std::list<std::shared_ptr<pml::nmos::ReceiverBase> >& lstReceiversAdded, const std::list<std::shared_ptr<pml::nmos::ReceiverBase> >& lstReceiversUpdated, const std::list<std::shared_ptr<pml::nmos::ReceiverBase> >& lstReceiversRemoved) override;

        /** @brief Called when when a query is subscibed (either with the query node or in p2p mode)
        *   @param sSubscriptionId the return id of the subscription. On failure this will be an empty string
        *   @param nResource the nmos resource type we are querying
        *   @param sQuery the query string if any
        **/
        void QuerySubscription(const std::string& sSubscriptionId, int nResource, const std::string& sQuery) override;

        /** @brief Called when a query subscritption is removed
        *   @param sSubscriptionId the id of the remove subscription
        **/
        void QuerySubscriptionRemoved(const std::string& sSubscriptionId) override;


        /** @brief Called when a ClientApi::Subscribe or ClientApi::Unsubscribe gets an answer from the target webserver
        *   @param nResult the http result code
        *   @param sResonse on success this will contain the uuId of the Sender, on failure this will contain a message with the reason for failure
        *   @param sResourceId contains the uuId of the Receiver
        **/
        void RequestTargetResult(unsigned long nResult, const std::string& sResponse, const std::string& sResourceId) override;


        /** @brief Called when a ClientAPI::PatchSenderStaged gets an answer for the target webserver
        *   @param nResult the http result code
        *   @param sResonse on success this will contain the raw json, on failure this will contain a message with the reason for failure
        *   @param sResourceId contains the uuId of the Sender
        **/
        void RequestPatchSenderResult(unsigned long nResult, const std::string& sResponse, const std::string& sResourceId) override;

        /** @brief Called when a ClientAPI::PatchReceiverStaged gets an answer for the target webserver
        *   @param nResult the http result code
        *   @param sResonse on success this will contain the raw json, on failure this will contain a message with the reason for failure
        *   @param sResourceId contains the uuId of the Sender
        **/
        void RequestPatchReceiverResult(unsigned long nResult, const std::string& sResponse, const std::string& sResourceId) override;

        /** @brief Called when a ClientAPI::Connect or ClientApi::Disconnect finishes
        *   @param sSenderId the uuid of the Sender that has been asked to be connected to
        *   @param sReceiverId the uuid of the Receiver that is been connected
        *   @param bSuccess true if the connection has been made, else false
        *   @param sResponse on failure this will contain a message describing the fault. On success it will contain the staged state of the receiver
        **/
        void RequestConnectResult(const std::string& sSenderId, const std::string& sReceiverId, bool bSuccess, const std::string& sResponse) override;

        /** @brief Called when a ClientAPI::RequestSenderStaged gets an answer for the target webserver
        *   @param nResult the http result code
        *   @param sResonse on success this will contain the raw json, on failure this will contain a message with the reason for failure
        *   @param sResourceId contains the uuId of the Sender
        **/
        void RequestGetSenderStagedResult(unsigned long nResult, const std::string& sResponse, const std::string& sResourceId) override;

        /** @brief Called when a ClientAPI::RequestSenderActive gets an answer for the target webserver
        *   @param nResult the http result code
        *   @param sResonse on success this will contain the raw json, on failure this will contain a message with the reason for failure
        *   @param sResourceId contains the uuId of the Sender
        **/
        void RequestGetSenderActiveResult(unsigned long nResult, const std::string& sResponse, const std::string& sResourceId) override;

        /** @brief Called when a ClientAPI::RequestSenderTransportFile gets an answer for the target webserver
        *   @param nResult the http result code
        *   @param sResonse on success this will contain the raw sdp, on failure this will contain a message with the reason for failure
        *   @param sResourceId contains the uuId of the Sender
        **/
        void RequestGetSenderTransportFileResult(unsigned long nResult, const std::string& sResponse, const std::string& sResourceId) override;

        /** @brief Called when a ClientAPI::RequestReceiverStaged gets an answer for the target webserver
        *   @param nResult the http result code
        *   @param sResonse on success this will contain the raw json, on failure this will contain a message with the reason for failure
        *   @param sResourceId contains the uuId of the Receiver
        **/
        void RequestGetReceiverStagedResult(unsigned long nResult, const std::string& sResponse, const std::string& sResourceId) override;

        /** @brief Called when a ClientAPI::RequestReceiverActive gets an answer for the target webserver
        *   @param nResult the http result code
        *   @param sResonse on success this will contain the raw json, on failure this will contain a message with the reason for failure
        *   @param sResourceId contains the uuId of the Receiver
        **/
        void RequestGetReceiverActiveResult(unsigned long nResult, const std::string& sResponse, const std::string& sResourceId) override;
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
    wxEvent *Clone() const { return new wxNmosClientCurlEvent(*this); }

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


