#pragma once
#ifdef __NMOS__
#include "eventposter.h"
#include "connection.h"
#include <wx/event.h>
#include "nmosapiversion.h"
#include <list>

class wxEventPoster : public pml::nmos::EventPoster
{
    public:
        wxEventPoster(wxEvtHandler* pHandler);
        virtual ~wxEventPoster(){}

        void AddHandler(wxEvtHandler* pHandler) { m_lstHandlers.push_back(pHandler);}

    protected:

        void Target(const std::string& sReceiverId, const std::string& sTransportFile, unsigned short nPort);
        void PatchSender(const std::string& sSenderId, const pml::nmos::connectionSender<pml::nmos::activationResponse>& conPatch, unsigned short nPort);
        void PatchReceiver(const std::string& sReceiverId, const pml::nmos::connectionReceiver<pml::nmos::activationResponse>& conPatch, unsigned short nPort);
        void SenderActivated(const std::string& sSenderId);
        void ReceiverActivated(const std::string& sReceiverId);


        void RegistrationNodeFound(const std::string& sUrl, unsigned short nPriority, const pml::nmos::ApiVersion& version);
        void RegistrationNodeRemoved(const std::string& sUrl);
        void RegistrationNodeChanged(const std::string& sUrl, unsigned short nPriority, bool bGood, const pml::nmos::ApiVersion& version);
        void RegistrationChanged(const std::string& sUrl, pml::nmos::EventPoster::enumRegState eState);



    private:
        std::list<wxEvtHandler*> m_lstHandlers;

};


class wxNmosNodeConnectionEvent : public wxCommandEvent
{

public:
    /** Constructor
    *   @param commandType should be wxEVT_NI
    *   @param id the ID of the button list control
    **/
    wxNmosNodeConnectionEvent(wxEventType type);

    wxNmosNodeConnectionEvent() : wxCommandEvent(){}

    /** Copy Constructor
    *   @param event a wxNIEvent
    **/

    wxNmosNodeConnectionEvent(const wxNmosNodeConnectionEvent& event);

    /** Destructor
    **/
    ~wxNmosNodeConnectionEvent(){}

    /** Creates a copy of the wxNmosNodeConnectionEvent
    *   @return <i>wxNmosNodeConnectionEvent</i>
    **/
    virtual wxEvent *Clone() const { return new wxNmosNodeConnectionEvent(*this); }

    void SetTransportFile(const std::string& sTransportFile);
    const std::string& GetTransportFile() const;

    void SetSenderConnection(const pml::nmos::connectionSender<pml::nmos::activationResponse>& con);
    const std::experimental::optional<pml::nmos::connectionSender<pml::nmos::activationResponse>>& GetSenderConnection() const;

    void SetReceiverConnection(const pml::nmos::connectionReceiver<pml::nmos::activationResponse>& con);
    const std::experimental::optional<pml::nmos::connectionReceiver<pml::nmos::activationResponse>>& GetReceiverConnection() const;

    unsigned short GetPort() const { return m_nPort;}
    void SetPort(unsigned short nPort) { m_nPort = nPort;}


    const std::string& GetResourceId() const { return m_sResourceId;}
    void SetResourceId(const std::string& sId) { m_sResourceId = sId;}

    DECLARE_DYNAMIC_CLASS(wxNmosNodeConnectionEvent)



private:

    std::string m_sTransportFile;
    std::experimental::optional<pml::nmos::connectionSender<pml::nmos::activationResponse>> m_conSender;
    std::experimental::optional<pml::nmos::connectionReceiver<pml::nmos::activationResponse>> m_conReceiver;
    std::string m_sResourceId;
    unsigned short m_nPort;

};

wxDECLARE_EXPORTED_EVENT(WXEXPORT, wxEVT_NMOS_TARGET, wxNmosNodeConnectionEvent);
wxDECLARE_EXPORTED_EVENT(WXEXPORT, wxEVT_NMOS_PATCH_SENDER, wxNmosNodeConnectionEvent);
wxDECLARE_EXPORTED_EVENT(WXEXPORT, wxEVT_NMOS_PATCH_RECEIVER, wxNmosNodeConnectionEvent);
wxDECLARE_EXPORTED_EVENT(WXEXPORT, wxEVT_NMOS_ACTIVATE_RECEIVER, wxNmosNodeConnectionEvent);
wxDECLARE_EXPORTED_EVENT(WXEXPORT, wxEVT_NMOS_ACTIVATE_SENDER, wxNmosNodeConnectionEvent);


class wxNmosNodeRegistrationEvent : public wxCommandEvent
{

public:

    wxNmosNodeRegistrationEvent() : wxCommandEvent(){}

    wxNmosNodeRegistrationEvent(wxEventType type);
    wxNmosNodeRegistrationEvent(const wxNmosNodeRegistrationEvent& event);

    /** Destructor
    **/
    ~wxNmosNodeRegistrationEvent(){}


    virtual wxEvent *Clone() const { return new wxNmosNodeRegistrationEvent(*this); }

    void SetNodeUrl(const wxString& sUrl) { m_sUrl = sUrl;}
    const wxString& GetNodeUrl() const  { return m_sUrl; }

    void SetNodePriority(int nPriority) { m_nPriority = nPriority;}
    int GetNodePriority() const { return m_nPriority;   }

    void SetNodeVersion(const pml::nmos::ApiVersion& version)   { m_version =version;   }
    const pml::nmos::ApiVersion& GetNodeVersion() const { return m_version;}

    void SetRegistered(pml::nmos::EventPoster::enumRegState eState)    { m_eState = eState;}
    pml::nmos::EventPoster::enumRegState GetRegistered() const  { return m_eState;}


    void SetNodeStatus(bool bGood) { m_bGood = bGood;}
    bool GetNodeStatus() const { return m_bGood;}

    DECLARE_DYNAMIC_CLASS(wxNmosNodeRegistrationEvent)



private:
    wxString m_sUrl;
    int m_nPriority;
    pml::nmos::ApiVersion m_version;
    bool m_bGood;
    pml::nmos::EventPoster::enumRegState m_eState;

};


wxDECLARE_EXPORTED_EVENT(WXEXPORT, wxEVT_NMOS_REGNODE_FOUND, wxNmosNodeRegistrationEvent);
wxDECLARE_EXPORTED_EVENT(WXEXPORT, wxEVT_NMOS_REGNODE_REMOVED, wxNmosNodeRegistrationEvent);
wxDECLARE_EXPORTED_EVENT(WXEXPORT, wxEVT_NMOS_REGNODE_CHANGED, wxNmosNodeRegistrationEvent);
wxDECLARE_EXPORTED_EVENT(WXEXPORT, wxEVT_NMOS_REGISTRATION_CHANGED, wxNmosNodeRegistrationEvent);





#endif
