#pragma once
#ifdef __NMOS__
#include "clientapiposter.h"
#include <wx/event.h>
#include <memory>
#include "clientapi.h"

class wxClientApiPoster : public ClientApiPoster
{
    public:
        wxClientApiPoster(wxEvtHandler* pHandler);
        ~wxClientApiPoster();


    protected:
        void NodeChanged(std::shared_ptr<Self> pNode, enumChange eChange);
        void DeviceChanged(std::shared_ptr<Device> pDevice, enumChange eChange);
        void SourceChanged(std::shared_ptr<Source> pSource, enumChange eChange);
        void FlowChanged(std::shared_ptr<Flow> pFlow, enumChange eChange);
        void SenderChanged(std::shared_ptr<Sender> pSender, enumChange eChange);
        void ReceiverChanged(std::shared_ptr<Receiver> pReceiver, enumChange eChange);

        void NodesRemoved(const std::set<std::string>& setRemoved);
        void DevicesRemoved(const std::set<std::string>& setRemoved);
        void SourcesRemoved(const std::set<std::string>& setRemoved);
        void FlowsRemoved(const std::set<std::string>& setRemoved);
        void SendersRemoved(const std::set<std::string>& setRemoved);
        void ReceiversRemoved(const std::set<std::string>& setRemoved);

        void RequestTargetResult(unsigned long nResult, const std::string& sResponse, const std::string& sResourceId);
        void RequestPatchSenderResult(unsigned long nResult, const std::string& sResponse, const std::string& sResourceId);
        void RequestPatchReceiverResult(unsigned long nResult, const std::string& sResponse, const std::string& sResourceId);


    private:
        wxEvtHandler* m_pHandler;

};


class wxNmosClientEvent : public wxCommandEvent
{

public:

    wxNmosClientEvent(std::shared_ptr<Self> pNode, ClientApiPoster::enumChange eChange);
    wxNmosClientEvent(std::shared_ptr<Device> pDevice, ClientApiPoster::enumChange eChange);
    wxNmosClientEvent(std::shared_ptr<Source> pSource, ClientApiPoster::enumChange eChange);
    wxNmosClientEvent(std::shared_ptr<Flow> pFlow, ClientApiPoster::enumChange eChange);
    wxNmosClientEvent(std::shared_ptr<Sender> pSender, ClientApiPoster::enumChange eChange);
    wxNmosClientEvent(std::shared_ptr<Receiver> pReceiver, ClientApiPoster::enumChange eChange);
    wxNmosClientEvent(const std::set<std::string>& setRemoved, ClientApi::flagResource eResource);

    wxNmosClientEvent() : wxCommandEvent(){}

    /** Copy Constructor
    *   @param event a wxNIEvent
    **/
    wxNmosClientEvent(const wxNmosClientEvent& event);

    /** Destructor
    **/
    ~wxNmosClientEvent(){}

    /** Creates a copy of the wxNmosClientEvent
    *   @return <i>wxNmosClientEvent</i>
    **/
    virtual wxEvent *Clone() const { return new wxNmosClientEvent(*this); }

    std::shared_ptr<Self> GetNode() const;
    std::shared_ptr<Device> GetDevice() const;
    std::shared_ptr<Source> GetSource() const;
    std::shared_ptr<Flow> GetFlow() const;
    std::shared_ptr<Sender> GetSender() const;
    std::shared_ptr<Receiver> GetReceiver() const;
    std::set<std::string>::const_iterator GetRemovedBegin() const;
    std::set<std::string>::const_iterator GetRemovedEnd() const;



    DECLARE_DYNAMIC_CLASS(wxNmosClientEvent)

private:
        std::shared_ptr<Self> m_pNode;
        std::shared_ptr<Device> m_pDevice;
        std::shared_ptr<Source> m_pSource;
        std::shared_ptr<Flow> m_pFlow;
        std::shared_ptr<Sender> m_pSender;
        std::shared_ptr<Receiver> m_pReceiver;
        std::set<std::string> m_setRemoved;
};

typedef void (wxEvtHandler::*wxNmosClientEventFunction)(wxNmosClientEvent&);


BEGIN_DECLARE_EVENT_TYPES()
    DECLARE_EXPORTED_EVENT_TYPE(WXEXPORT, wxEVT_NMOS_CLIENT_NODE, -1)
    DECLARE_EXPORTED_EVENT_TYPE(WXEXPORT, wxEVT_NMOS_CLIENT_SOURCE, -1)
    DECLARE_EXPORTED_EVENT_TYPE(WXEXPORT, wxEVT_NMOS_CLIENT_FLOW, -1)
    DECLARE_EXPORTED_EVENT_TYPE(WXEXPORT, wxEVT_NMOS_CLIENT_DEVICE, -1)
    DECLARE_EXPORTED_EVENT_TYPE(WXEXPORT, wxEVT_NMOS_CLIENT_SENDER, -1)
    DECLARE_EXPORTED_EVENT_TYPE(WXEXPORT, wxEVT_NMOS_CLIENT_RECEIVER, -1)
    DECLARE_EXPORTED_EVENT_TYPE(WXEXPORT, wxEVT_NMOS_CLIENT_RESOURCES_REMOVED, -1)
END_DECLARE_EVENT_TYPES()

#define wxNmosClientEventHandler(func) \
    (wxObjectEventFunction)(wxEventFunction)wxStaticCastEvent(wxNmosClientEventFunction, &func)

#define EVT_NMOS_CLIENT_NODE(id,fn) DECLARE_EVENT_TABLE_ENTRY(EVT_NMOS_CLIENT_NODE,id,-1,(wxObjectEventFunction) (wxEventFunction) (wxCommandEventFunction) (wxNmosClientEventFunction) &fn, (wxObject*) NULL),
#define EVT_NMOS_CLIENT_SOURCE(id,fn) DECLARE_EVENT_TABLE_ENTRY(wxEVT_NMOS_CLIENT_SOURCE,id,-1,(wxObjectEventFunction) (wxEventFunction) (wxCommandEventFunction) (wxNmosClientEventFunction) &fn, (wxObject*) NULL),
#define EVT_NMOS_CLIENT_FLOW(id,fn) DECLARE_EVENT_TABLE_ENTRY(wxEVT_NMOS_CLIENT_FLOW,id,-1,(wxObjectEventFunction) (wxEventFunction) (wxCommandEventFunction) (wxNmosClientEventFunction) &fn, (wxObject*) NULL),
#define EVT_NMOS_CLIENT_DEVICE(id,fn) DECLARE_EVENT_TABLE_ENTRY(wxEVT_NMOS_CLIENT_DEVICE,id,-1,(wxObjectEventFunction) (wxEventFunction) (wxCommandEventFunction) (wxNmosClientEventFunction) &fn, (wxObject*) NULL),
#define EVT_NMOS_CLIENT_SENDER(id,fn) DECLARE_EVENT_TABLE_ENTRY(wxEVT_NMOS_CLIENT_SENDER,id,-1,(wxObjectEventFunction) (wxEventFunction) (wxCommandEventFunction) (wxNmosClientEventFunction) &fn, (wxObject*) NULL),
#define EVT_NMOS_CLIENT_RECEIVER(id,fn) DECLARE_EVENT_TABLE_ENTRY(wxEVT_NMOS_CLIENT_RECEIVER,id,-1,(wxObjectEventFunction) (wxEventFunction) (wxCommandEventFunction) (wxNmosClientEventFunction) &fn, (wxObject*) NULL),
#define EVT_NMOS_CLIENT_RESOURCES_REMOVED(id,fn) DECLARE_EVENT_TABLE_ENTRY(wxEVT_NMOS_CLIENT_RESOURCES_REMOVED,id,-1,(wxObjectEventFunction) (wxEventFunction) (wxCommandEventFunction) (wxNmosClientEventFunction) &fn, (wxObject*) NULL),


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

    unsigned long GetResult();
    const wxString& GetResponse();
    const wxString& GetResourceId();


    DECLARE_DYNAMIC_CLASS(wxNmosClientCurlEvent)

    private:
        wxString m_sResourceId;
};

typedef void (wxEvtHandler::*wxNmosClientCurlEventFunction)(wxNmosClientCurlEvent&);


BEGIN_DECLARE_EVENT_TYPES()
    DECLARE_EXPORTED_EVENT_TYPE(WXEXPORT, wxEVT_NMOS_CLIENTCURL_SUBSCRIBE, -1)
    DECLARE_EXPORTED_EVENT_TYPE(WXEXPORT, wxEVT_NMOS_CLIENTCURL_PATCH_SENDER, -1)
    DECLARE_EXPORTED_EVENT_TYPE(WXEXPORT, wxEVT_NMOS_CLIENTCURL_PATCH_RECEIVER, -1)
END_DECLARE_EVENT_TYPES()

#define wxNmosClientCurlEventHandler(func) \
    (wxObjectEventFunction)(wxEventFunction)wxStaticCastEvent(wxNmosClientCurlEventFunction, &func)

#define EVT_NMOS_CLIENTCURL_SUBSCRIBE(id,fn) DECLARE_EVENT_TABLE_ENTRY(wxEVT_NMOS_CLIENTCURL_SUBSCRIBE,id,-1,(wxObjectEventFunction) (wxEventFunction) (wxCommandEventFunction) (wxNmosClientCurlEventFunction) &fn, (wxObject*) NULL),
#define EVT_NMOS_CLIENTCURL_PATCH_SENDER(id,fn) DECLARE_EVENT_TABLE_ENTRY(wxEVT_NMOS_CLIENTCURL_PATCH_SENDER,id,-1,(wxObjectEventFunction) (wxEventFunction) (wxCommandEventFunction) (wxNmosClientCurlEventFunction) &fn, (wxObject*) NULL),
#define EVT_NMOS_CLIENTCURL_PATCH_RECEIVER(id,fn) DECLARE_EVENT_TABLE_ENTRY(wxEVT_NMOS_CLIENTCURL_PATCH_RECEIVER,id,-1,(wxObjectEventFunction) (wxEventFunction) (wxCommandEventFunction) (wxNmosClientCurlEventFunction) &fn, (wxObject*) NULL),


#endif


