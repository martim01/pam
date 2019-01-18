#include "wxeventposter.h"
#include "log.h"
#include "mdns.h"


DEFINE_EVENT_TYPE(wxEVT_NMOS_MDNS_ALLFORNOW)
DEFINE_EVENT_TYPE(wxEVT_NMOS_MDNS_FINISHED)
DEFINE_EVENT_TYPE(wxEVT_NMOS_MDNS_RESOLVED)
DEFINE_EVENT_TYPE(wxEVT_NMOS_MDNS_REMOVED)
DEFINE_EVENT_TYPE(wxEVT_NMOS_CURL_DONE)
DEFINE_EVENT_TYPE(wxEVT_NMOS_REG_ERROR)

DEFINE_EVENT_TYPE(wxEVT_NMOS_TARGET)
DEFINE_EVENT_TYPE(wxEVT_NMOS_PATCH_SENDER)
DEFINE_EVENT_TYPE(wxEVT_NMOS_PATCH_RECEIVER)
DEFINE_EVENT_TYPE(wxEVT_NMOS_ACTIVATE_RECEIVER)
DEFINE_EVENT_TYPE(wxEVT_NMOS_ACTIVATE_SENDER)

wxEventPoster::wxEventPoster(wxEvtHandler* pHandler) :
m_pHandler(pHandler)
{

}

void wxEventPoster::InstanceResolved(std::shared_ptr<dnsInstance> pInstance)
{
    if(m_pHandler)
    {
        wxNmosEvent* pEvent = new wxNmosEvent(wxEVT_NMOS_MDNS_RESOLVED);
        pEvent->SetDnsInstance(pInstance);
        wxQueueEvent(m_pHandler, pEvent);
    }
}

void wxEventPoster::InstanceRemoved(const std::string& sInstance)
{
    if(m_pHandler)
    {
        wxNmosEvent* pEvent = new wxNmosEvent(wxEVT_NMOS_MDNS_REMOVED);
        pEvent->SetString(wxString::FromAscii(sInstance.c_str()));
        wxQueueEvent(m_pHandler, pEvent);
    }
}

void wxEventPoster::AllForNow(const std::string& sService)
{
    if(m_pHandler)
    {
        wxNmosEvent* pEvent = new wxNmosEvent(wxEVT_NMOS_MDNS_ALLFORNOW);
        pEvent->SetString(wxString::FromAscii(sService.c_str()));
        wxQueueEvent(m_pHandler, pEvent);
    }
}

void wxEventPoster::Finished()
{
    if(m_pHandler)
    {
        wxNmosEvent* pEvent = new wxNmosEvent(wxEVT_NMOS_MDNS_FINISHED);
        wxQueueEvent(m_pHandler, pEvent);
    }
}

void wxEventPoster::CurlDone(unsigned long nResult, const std::string& sResult, long nUserType)
{
    if(m_pHandler)
    {
        wxNmosEvent* pEvent(new wxNmosEvent(wxEVT_NMOS_CURL_DONE));
        pEvent->SetInt(nResult);
        pEvent->SetString(wxString::FromAscii(sResult.c_str()));
        pEvent->SetExtraLong(nUserType);
        wxQueueEvent(m_pHandler, pEvent);
    }
}

void wxEventPoster::RegistrationNodeError()
{
    if(m_pHandler)
    {
        wxNmosEvent* pEvent = new wxNmosEvent(wxEVT_NMOS_REG_ERROR);
        wxQueueEvent(m_pHandler, pEvent);
    }
}


void wxEventPoster::Target(const std::string& sReceiverId, std::shared_ptr<Sender> pSender, unsigned short nPort)
{
    if(m_pHandler)
    {
        wxNmosEvent* pEvent = new wxNmosEvent(wxEVT_NMOS_TARGET);
        pEvent->SetString(wxString::FromAscii(sReceiverId.c_str()));
        pEvent->SetSender(pSender);
        pEvent->SetInt(nPort);
        wxQueueEvent(m_pHandler, pEvent);
    }
}

void wxEventPoster::PatchSender(const std::string& sSenderId, const connectionSender& conPatch, unsigned short nPort)
{
    if(m_pHandler)
    {
        wxNmosEvent* pEvent = new wxNmosEvent(wxEVT_NMOS_PATCH_SENDER);
        pEvent->SetString(wxString::FromAscii(sSenderId.c_str()));
        pEvent->SetSenderConnection(conPatch);
        pEvent->SetInt(nPort);
        wxQueueEvent(m_pHandler, pEvent);
    }
}

void wxEventPoster::PatchReceiver(const std::string& sReceiverId, const connectionReceiver& conPatch, unsigned short nPort)
{
    if(m_pHandler)
    {
        wxNmosEvent* pEvent = new wxNmosEvent(wxEVT_NMOS_PATCH_RECEIVER);
        pEvent->SetString(wxString::FromAscii(sReceiverId.c_str()));
        pEvent->SetReceiverConnection(conPatch);
        pEvent->SetInt(nPort);
        wxQueueEvent(m_pHandler, pEvent);
    }
}

void wxEventPoster::ActivateSender(const std::string& sSenderId)
{
    if(m_pHandler)
    {
        wxNmosEvent* pEvent = new wxNmosEvent(wxEVT_NMOS_ACTIVATE_SENDER);
        pEvent->SetString(wxString::FromAscii(sSenderId.c_str()));
        wxQueueEvent(m_pHandler, pEvent);
    }
}

void wxEventPoster::ActivateReceiver(const std::string& sReceiverId)
{
    if(m_pHandler)
    {
        wxNmosEvent* pEvent = new wxNmosEvent(wxEVT_NMOS_ACTIVATE_RECEIVER);
        pEvent->SetString(wxString::FromAscii(sReceiverId.c_str()));
        wxQueueEvent(m_pHandler, pEvent);
    }
}


IMPLEMENT_DYNAMIC_CLASS(wxNmosEvent, wxCommandEvent)

wxNmosEvent::wxNmosEvent(wxEventType type) : wxCommandEvent(type),
m_pSender(0)
{

}

wxNmosEvent::wxNmosEvent(const wxNmosEvent& event) : wxCommandEvent(event),
m_pSender(event.GetSender())
{
    m_conSender = event.GetSenderConnection();
    m_conReceiver = event.GetReceiverConnection();
}

void wxNmosEvent::SetSender(std::shared_ptr<Sender> pSender)
{
    m_pSender = pSender;
}

const std::shared_ptr<Sender> wxNmosEvent::GetSender() const
{
    return m_pSender;
}

void wxNmosEvent::SetSenderConnection(const connectionSender& con)
{
    m_conSender = con;
}

const connectionSender& wxNmosEvent::GetSenderConnection() const
{
    return m_conSender;
}

void wxNmosEvent::SetReceiverConnection(const connectionReceiver& con)
{
    m_conReceiver = con;
}

const connectionReceiver& wxNmosEvent::GetReceiverConnection() const
{
    return m_conReceiver;
}


wxString wxNmosEvent::GetId() const
{
    return GetString();
}
wxString wxNmosEvent::GetCurlResponse() const
{
    return GetString();
}

wxString wxNmosEvent::GetService() const
{
    return GetString();
}

int wxNmosEvent::GetPort() const
{
    return GetInt();
}

int wxNmosEvent::GetCurlResult() const
{
    return GetInt();
}

int wxNmosEvent::GetCurlType() const
{
    return GetExtraLong();
}


void wxNmosEvent::SetDnsInstance(std::shared_ptr<dnsInstance> pInstance)
{
    m_pDnsInstance = pInstance;
}

const std::shared_ptr<dnsInstance> wxNmosEvent::GetDnsInstance() const
{
    return m_pDnsInstance;
}
