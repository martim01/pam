#pragma once
#ifdef __NMOS__
#include "eventposter.h"
#include "connection.h"
#include <wx/event.h>

class wxEventPoster : public EventPoster
{
    public:
        wxEventPoster(wxEvtHandler* pHandler);
        virtual ~wxEventPoster(){}

    protected:
        void CurlDone(unsigned long nResult, const std::string& sResponse, long nType, const std::string& sResourceId);
        void InstanceResolved(std::shared_ptr<dnsInstance> pInstance);
        void AllForNow(const std::string& sService);
        void Finished();
        void RegistrationNodeError();
        void InstanceRemoved(std::shared_ptr<dnsInstance> pInstance);

        void Target(const std::string& sReceiverId, const std::string& sTransportFile, unsigned short nPort);
        void PatchSender(const std::string& sSenderId, const connectionSender& conPatch, unsigned short nPort);
        void PatchReceiver(const std::string& sReceiverId, const connectionReceiver& conPatch, unsigned short nPort);
        void SenderActivated(const std::string& sSenderId);
        void ReceiverActivated(const std::string& sReceiverId);

    private:
        wxEvtHandler* m_pHandler;
};


class wxNmosEvent : public wxCommandEvent
{

public:
    /** Constructor
    *   @param commandType should be wxEVT_NI
    *   @param id the ID of the button list control
    **/
    wxNmosEvent(wxEventType type);

    wxNmosEvent() : wxCommandEvent(){}

    /** Copy Constructor
    *   @param event a wxNIEvent
    **/
    wxNmosEvent(const wxNmosEvent& event);

    /** Destructor
    **/
    ~wxNmosEvent(){}

    /** Creates a copy of the wxNmosEvent
    *   @return <i>wxNmosEvent</i>
    **/
    virtual wxEvent *Clone() const { return new wxNmosEvent(*this); }

    void SetDnsInstance(std::shared_ptr<dnsInstance> pInstance);
    const std::shared_ptr<dnsInstance> GetDnsInstance() const;

    void SetTransportFile(const std::string& sTransportFile);
    const wxString& GetTransportFile() const;

    void SetSenderConnection(const connectionSender& con);
    const connectionSender& GetSenderConnection() const;

    void SetReceiverConnection(const connectionReceiver& con);
    const connectionReceiver& GetReceiverConnection() const;

    wxString GetResourceId() const;
    wxString GetCurlResponse() const;
    wxString GetService() const;
    int GetPort() const;
    int GetCurlResult() const;
    int GetCurlType() const;



    DECLARE_DYNAMIC_CLASS(wxNmosEvent)



private:

    std::shared_ptr<dnsInstance> m_pDnsInstance;
    wxString m_sTransportFile;
    connectionSender m_conSender;
    connectionReceiver m_conReceiver;

};

wxDECLARE_EXPORTED_EVENT(WXEXPORT, wxEVT_NMOS_MDNS_RESOLVED, wxNmosEvent);
wxDECLARE_EXPORTED_EVENT(WXEXPORT, wxEVT_NMOS_MDNS_ALLFORNOW, wxNmosEvent);
wxDECLARE_EXPORTED_EVENT(WXEXPORT, wxEVT_NMOS_MDNS_FINISHED, wxNmosEvent);
wxDECLARE_EXPORTED_EVENT(WXEXPORT, wxEVT_NMOS_CURL_DONE, wxNmosEvent);
wxDECLARE_EXPORTED_EVENT(WXEXPORT, wxEVT_NMOS_REG_ERROR, wxNmosEvent);
wxDECLARE_EXPORTED_EVENT(WXEXPORT, wxEVT_NMOS_TARGET, wxNmosEvent);
wxDECLARE_EXPORTED_EVENT(WXEXPORT, wxEVT_NMOS_PATCH_SENDER, wxNmosEvent);
wxDECLARE_EXPORTED_EVENT(WXEXPORT, wxEVT_NMOS_PATCH_RECEIVER, wxNmosEvent);
wxDECLARE_EXPORTED_EVENT(WXEXPORT, wxEVT_NMOS_ACTIVATE_RECEIVER, wxNmosEvent);
wxDECLARE_EXPORTED_EVENT(WXEXPORT, wxEVT_NMOS_ACTIVATE_SENDER, wxNmosEvent);







#endif
