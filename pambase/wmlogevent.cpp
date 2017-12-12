#include "wmlogevent.h"
#include <wx/log.h>


wmLog* wmLog::Get()
{
    static wmLog lg;
    return &lg;
}

void wmLog::SetTarget(wxEvtHandler* pHandler)
{
    m_pHandler = pHandler;
}

void wmLog::Log(const wxString& sDevice, const wxString& sMessage, bool bSend)
{
    if(m_pHandler)
    {
        wmLogEvent* plge = new wmLogEvent(sDevice, sMessage, false);
        if(bSend == true)
        {
            m_pHandler->QueueEvent(plge);
        }

    }
}

void wmLog::Log(const wxString& sMessage)
{
    Log(wxEmptyString, sMessage, true);
}


DEFINE_EVENT_TYPE(wxEVT_WMLOG)

IMPLEMENT_DYNAMIC_CLASS(wmLogEvent, wxCommandEvent)

wmLogEvent::wmLogEvent(const wmLogEvent& event) : wxCommandEvent(event)
{
    m_dtTimestamp = event.GetTimestamp();
    m_sDevice = event.GetDevice().c_str();
    m_sMessage = event.GetLogMessage().c_str();
    m_bDebug = event.IsDebug();
}

wmLogEvent::wmLogEvent(const wxString& sDevice, const wxString& sMessage, bool bDebug) : wxCommandEvent(wxEVT_WMLOG,0),
m_dtTimestamp(wxDateTime::Now()),
m_sDevice(sDevice.c_str()),
m_sMessage(sMessage.c_str()),
m_bDebug(bDebug)
{

}

const wxDateTime& wmLogEvent::GetTimestamp() const
{
    return m_dtTimestamp;
}

const wxString& wmLogEvent::GetDevice() const
{
    return m_sDevice;
}

const wxString& wmLogEvent::GetLogMessage() const
{
    return m_sMessage;
}

bool wmLogEvent::IsDebug() const
{
    return m_bDebug;
}
