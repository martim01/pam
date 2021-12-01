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
                void NodeChanged(const pml::nmos::resourcechanges<pml::nmos::Self>& changed) override;

                /** @brief called when one or more Device resources that match a subscribed query are added, updated or removed
                *   @param lstNodesAdded a list of Devices that have been added
                *   @param lstNodesUpdated a list of Devices that have been updated
                *   @param lstNodesRemoved a list of Devices that have been removed
                **/
                void DeviceChanged(const pml::nmos::resourcechanges<pml::nmos::Device>& changed) override;

                /** @brief called when one or more Source resources that match a subscribed query are added, updated or removed
                *   @param lstNodesAdded a list of Sources that have been added
                *   @param lstNodesUpdated a list of Sources that have been updated
                *   @param lstNodesRemoved a list of Sources that have been removed
                **/
                void SourceChanged(const pml::nmos::resourcechanges<pml::nmos::Source>& changed) override;

                /** @brief called when one or more Flow resources that match a subscribed query are added, updated or removed
                *   @param lstNodesAdded a list of Flows that have been added
                *   @param lstNodesUpdated a list of Flows that have been updated
                *   @param lstNodesRemoved a list of Flows that have been removed
                **/
                void FlowChanged(const pml::nmos::resourcechanges<pml::nmos::Flow>& changed) override;

                /** @brief called when one or more Sender resources that match a subscribed query are added, updated or removed
                *   @param lstNodesAdded a list of Senders that have been added
                *   @param lstNodesUpdated a list of Senders that have been updated
                *   @param lstNodesRemoved a list of Senders that have been removed
                **/
                void SenderChanged(const pml::nmos::resourcechanges<pml::nmos::Sender>& changed) override;

                /** @brief called when one or more Receiver resources that match a subscribed query are added, updated or removed
                *   @param lstNodesAdded a list of Receivers that have been added
                *   @param lstNodesUpdated a list of Receivers that have been updated
                *   @param lstNodesRemoved a list of Receivers that have been removed
                **/
                void ReceiverChanged(const pml::nmos::resourcechanges<pml::nmos::Receiver>& changed) override;

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

                void QueryServerChanged(const std::string& sUrl) override;
                void QueryServerFound(const std::string& sUrl, unsigned short nPriority) override;
                void QueryServerRemoved(const std::string& sUrl) override;

                /** @brief Called when a ClientApi::Subscribe or ClientApi::Unsubscribe gets an answer from the target webserver
                *   @param nResult the http result code
                *   @param sResonse on success this will contain the uuId of the Sender, on failure this will contain a message with the reason for failure
                *   @param sResourceId contains the uuId of the Receiver
                **/
                void RequestTargetResult(unsigned long nResult, const std::string& sResponse, const std::string& sResourceId) override;


                void RequestPatchSenderResult(const pml::nmos::curlResponse& resp, const std::experimental::optional<pml::nmos::connectionSender<pml::nmos::activationResponse>>& con, const std::string& sResourceId) override;
                void RequestPatchReceiverResult(const pml::nmos::curlResponse& resp, const std::experimental::optional<pml::nmos::connectionReceiver<pml::nmos::activationResponse>>& con, const std::string& sResourceId) override;
                void RequestGetSenderStagedResult(const pml::nmos::curlResponse& resp, const std::experimental::optional<pml::nmos::connectionSender<pml::nmos::activationResponse>>& con, const std::string& sResourceId) override;
                void RequestGetSenderActiveResult(const pml::nmos::curlResponse& resp, const std::experimental::optional<pml::nmos::connectionSender<pml::nmos::activationResponse>>& con, const std::string& sResourceId) override;
                void RequestGetSenderTransportFileResult(const pml::nmos::curlResponse& resp, const std::experimental::optional<std::string>& transportFile, const std::string& sResourceId) override;
                void RequestGetReceiverStagedResult(const pml::nmos::curlResponse& resp, const std::experimental::optional<pml::nmos::connectionReceiver<pml::nmos::activationResponse>>& con, const std::string& sResourceId) override;
                void RequestGetReceiverActiveResult(const pml::nmos::curlResponse& resp, const std::experimental::optional<pml::nmos::connectionReceiver<pml::nmos::activationResponse>>& con, const std::string& sResourceId) override;

                void RequestGetSenderConstraintsResult(const pml::nmos::curlResponse& resp, const std::vector<pml::nmos::Constraints>& vConstraints, const std::string& sResourceId) override;
                void RequestGetReceiverConstraintsResult(const pml::nmos::curlResponse& resp, const std::vector<pml::nmos::Constraints>& vConstraints, const std::string& sResourceId) override;

                void RequestConnectResult(const std::string& sSenderId, const std::string& sReceiverId, bool bSuccess, const std::string& sResponse) override;

    private:
        wxEvtHandler* m_pHandler;

};






class wxNmosClientRequestEvent : public wxCommandEvent
{

public:

    wxNmosClientRequestEvent(wxEventType type, const pml::nmos::curlResponse& resp, const std::experimental::optional<pml::nmos::connectionSender<pml::nmos::activationResponse>>& con, const wxString& sResourceId);
    wxNmosClientRequestEvent(wxEventType type, const pml::nmos::curlResponse& resp, const std::experimental::optional<pml::nmos::connectionReceiver<pml::nmos::activationResponse>>& con, const wxString& sResourceId);
    wxNmosClientRequestEvent(wxEventType type, const pml::nmos::curlResponse& resp, const std::vector<pml::nmos::Constraints>& vConstraints, const wxString& sResourceId);
    wxNmosClientRequestEvent(const pml::nmos::curlResponse& resp, const wxString& sTransportFile, const wxString& sResourceId);

    wxNmosClientRequestEvent() : wxCommandEvent(){}

    /** Copy Constructor
    *   @param event a wxNIEvent
    **/
    wxNmosClientRequestEvent(const wxNmosClientRequestEvent& event);

    /** Destructor
    **/
    ~wxNmosClientRequestEvent(){}

    /** Creates a copy of the wxNmosClientEvent
    *   @return <i>wxNmosClientEvent</i>
    **/
    wxEvent *Clone() const { return new wxNmosClientRequestEvent(*this); }

    const pml::nmos::curlResponse& GetRawResponse() const
    {   return m_resp;  }
    unsigned short GetResponseCode() const
    {   return m_resp.nCode;    }

    std::experimental::optional<pml::nmos::connectionSender<pml::nmos::activationResponse>> GetSenderResponse() const
    {   return m_senderResponse; }

    std::experimental::optional<pml::nmos::connectionReceiver<pml::nmos::activationResponse>> GetReceiverResponse() const
    {   return m_receiverResponse;}

    const std::vector<pml::nmos::Constraints>& GetConstraints() const { return m_vConstraints;}
    const wxString& GetTransportFile() const { return m_sTransportFile;}
    const wxString& GetResourceId() const { return m_sResourceId;}

    DECLARE_DYNAMIC_CLASS(wxNmosClientRequestEvent)

    private:
        pml::nmos::curlResponse m_resp;
        std::experimental::optional<pml::nmos::connectionSender<pml::nmos::activationResponse>> m_senderResponse;;
        std::experimental::optional<pml::nmos::connectionReceiver<pml::nmos::activationResponse>> m_receiverResponse;
        std::vector<pml::nmos::Constraints> m_vConstraints;
        wxString m_sTransportFile;
        wxString m_sResourceId;
};




wxDECLARE_EXPORTED_EVENT(WXEXPORT, wxEVT_NMOS_CLIENT_GET_SENDER_TRANSPORTFILE, wxNmosClientRequestEvent);

wxDECLARE_EXPORTED_EVENT(WXEXPORT, wxEVT_NMOS_CLIENT_GET_SENDER_CONSTRAINTS, wxNmosClientRequestEvent);
wxDECLARE_EXPORTED_EVENT(WXEXPORT, wxEVT_NMOS_CLIENT_GET_RECEIVER_CONSTRAINTS, wxNmosClientRequestEvent);

wxDECLARE_EXPORTED_EVENT(WXEXPORT, wxEVT_NMOS_CLIENT_GET_SENDER_STAGED, wxNmosClientRequestEvent);
wxDECLARE_EXPORTED_EVENT(WXEXPORT, wxEVT_NMOS_CLIENT_GET_SENDER_ACTIVE, wxNmosClientRequestEvent);
wxDECLARE_EXPORTED_EVENT(WXEXPORT, wxEVT_NMOS_CLIENT_GET_RECEIVER_STAGED, wxNmosClientRequestEvent);
wxDECLARE_EXPORTED_EVENT(WXEXPORT, wxEVT_NMOS_CLIENT_GET_RECEIVER_ACTIVE, wxNmosClientRequestEvent);
wxDECLARE_EXPORTED_EVENT(WXEXPORT, wxEVT_NMOS_CLIENT_PATCH_SENDER, wxNmosClientRequestEvent);
wxDECLARE_EXPORTED_EVENT(WXEXPORT, wxEVT_NMOS_CLIENT_PATCH_RECEIVER, wxNmosClientRequestEvent);

wxDECLARE_EXPORTED_EVENT(WXEXPORT, wxEVT_NMOS_CLIENT_SUBSCRIBE, wxNmosClientRequestEvent);
wxDECLARE_EXPORTED_EVENT(WXEXPORT, wxEVT_NMOS_CLIENT_CONNECT, wxNmosClientRequestEvent);



class wxNmosClientQueryEvent : public wxCommandEvent
{

public:

    wxNmosClientQueryEvent(wxEventType type, const wxString& sUrl, unsigned short nPriorityoverride);

    wxNmosClientQueryEvent() : wxCommandEvent(){}

    /** Copy Constructor
    *   @param event a wxNIEvent
    **/
    wxNmosClientQueryEvent(const wxNmosClientQueryEvent& event);

    /** Destructor
    **/
    ~wxNmosClientQueryEvent(){}

    /** Creates a copy of the wxNmosClientEvent
    *   @return <i>wxNmosClientEvent</i>
    **/
    wxEvent *Clone() const { return new wxNmosClientQueryEvent(*this); }

    const wxString& GetUrl() const { return m_sUrl;}
    unsigned short GetPriority() const { return m_nPriority; }


    DECLARE_DYNAMIC_CLASS(wxNmosClientQueryEvent)

    private:
        wxString m_sUrl;
        unsigned short m_nPriority;

};


wxDECLARE_EXPORTED_EVENT(WXEXPORT, wxEVT_NMOS_CLIENTQUERY_FOUND, wxNmosClientQueryEvent);
wxDECLARE_EXPORTED_EVENT(WXEXPORT, wxEVT_NMOS_CLIENTQUERY_REMOVED, wxNmosClientQueryEvent);
wxDECLARE_EXPORTED_EVENT(WXEXPORT, wxEVT_NMOS_CLIENTQUERY_CHANGED, wxNmosClientQueryEvent);


#endif


