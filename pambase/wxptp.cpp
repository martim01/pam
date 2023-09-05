#include "wxptp.h"
#include "ptpmonkey.h"
#include "timedbuffer.h"
#include <wx/log.h>
#include "log.h"
#include "ptpeventhander.h"
#include "timeutils.h"
#include <iostream>
#include <iomanip>
#include "ptpclock.h"
#include "wxptpeventhandler.h"
#include "ptpeventloghandler.h"

wxDEFINE_EVENT(wxEVT_CLOCK_ADDED,wxCommandEvent);
wxDEFINE_EVENT(wxEVT_CLOCK_UPDATED,wxCommandEvent);
wxDEFINE_EVENT(xEVT_CLOCK_REMOVED,wxCommandEvent);
wxDEFINE_EVENT(wxEVT_CLOCK_TIME,wxCommandEvent);
wxDEFINE_EVENT(wxEVT_CLOCK_MASTER,wxCommandEvent);
wxDEFINE_EVENT(wxEVT_CLOCK_SLAVE,wxCommandEvent);
wxDEFINE_EVENT(wxEVT_CLOCK_REMOVED,wxCommandEvent);
wxDEFINE_EVENT(wxEVT_CLOCK_MSG_ANNOUNCE,wxCommandEvent);
wxDEFINE_EVENT(wxEVT_CLOCK_MSG_SYNC,wxCommandEvent);
wxDEFINE_EVENT(wxEVT_CLOCK_MSG_FOLLOWUP,wxCommandEvent);
wxDEFINE_EVENT(wxEVT_CLOCK_MSG_DELAY_REQUEST,wxCommandEvent);
wxDEFINE_EVENT(wxEVT_CLOCK_MSG_DELAY_RESPONSE,wxCommandEvent);


using namespace ptpmonkey;

void wxPtpEventHandler::AddHandler(wxEvtHandler* pHandler)
{
    m_setHandlers.insert(pHandler);
}

void wxPtpEventHandler::RemoveHandler(wxEvtHandler* pHandler)
{
    m_setHandlers.erase(pHandler);
}

void wxPtpEventHandler::NotifyHandlers(wxEventType type, const wxString& sClockId)
{
    for(auto pHandler : m_setHandlers)
    {
        wxCommandEvent* pEvent = new wxCommandEvent(type);
        pEvent->SetString(sClockId);
        wxQueueEvent(pHandler, pEvent);
    }
}

void wxPtpEventHandler::ClockAdded(std::shared_ptr<PtpV2Clock> pClock)
{
    NotifyHandlers(wxEVT_CLOCK_ADDED, wxString::FromUTF8(pClock->GetId().c_str()));
}

void wxPtpEventHandler::ClockUpdated(std::shared_ptr<PtpV2Clock> pClock)
{

    NotifyHandlers(wxEVT_CLOCK_UPDATED, wxString::FromUTF8(pClock->GetId().c_str()));
}

void wxPtpEventHandler::ClockBecomeMaster(std::shared_ptr<PtpV2Clock> pClock)
{
    NotifyHandlers(wxEVT_CLOCK_MASTER, wxString::FromUTF8(pClock->GetId().c_str()));
}

void wxPtpEventHandler::ClockBecomeSlave(std::shared_ptr<PtpV2Clock> pClock)
{
    NotifyHandlers(wxEVT_CLOCK_SLAVE, wxString::FromUTF8(pClock->GetId().c_str()));
}

void wxPtpEventHandler::ClockRemoved(std::shared_ptr<PtpV2Clock> pClock)
{
    NotifyHandlers(wxEVT_CLOCK_REMOVED, wxString::FromUTF8(pClock->GetId().c_str()));
}

void wxPtpEventHandler::ClockTimeCalculated(std::shared_ptr<PtpV2Clock> pClock)
{
    NotifyHandlers(wxEVT_CLOCK_TIME, wxString::FromUTF8(pClock->GetId().c_str()));
}

void wxPtpEventHandler::AnnounceSent(std::shared_ptr<PtpV2Clock> pClock)
{
    NotifyHandlers(wxEVT_CLOCK_MSG_ANNOUNCE, wxString::FromUTF8(pClock->GetId().c_str()));
}

void wxPtpEventHandler::SyncSent(std::shared_ptr<PtpV2Clock> pClock)
{
    NotifyHandlers(wxEVT_CLOCK_MSG_SYNC, wxString::FromUTF8(pClock->GetId().c_str()));
}

void wxPtpEventHandler::FollowUpSent(std::shared_ptr<PtpV2Clock> pClock)
{
    NotifyHandlers(wxEVT_CLOCK_MSG_FOLLOWUP, wxString::FromUTF8(pClock->GetId().c_str()));
}

void wxPtpEventHandler::DelayRequestSent(std::shared_ptr<PtpV2Clock> pClock)
{
    pmlLog() << "DelayRequestSent";
    NotifyHandlers(wxEVT_CLOCK_MSG_DELAY_REQUEST, wxString::FromUTF8(pClock->GetId().c_str()));
}

void wxPtpEventHandler::DelayResponseSent(std::shared_ptr<PtpV2Clock> pClock)
{
    NotifyHandlers(wxEVT_CLOCK_MSG_DELAY_RESPONSE, wxString::FromUTF8(pClock->GetId().c_str()));
}





wxPtp& wxPtp::Get()
{
    static wxPtp ptp;
    return ptp;
}

wxPtp::wxPtp() : m_pNotifier(std::make_shared<wxPtpEventHandler>())
{
}

void wxPtp::Run(const wxString& sInterface, unsigned char nDomain, ptpmonkey::Mode mode)
{
    if(m_pMonkey == nullptr || sInterface.ToStdString() != m_interface.Get() || mode != m_pMonkey->GetMode())
    {
        m_interface = IpInterface(sInterface.ToStdString());

        m_pMonkey = std::make_unique<PtpMonkey>(m_interface, nDomain, 2, mode, ptpmonkey::Rate::EVERY_1_SEC);
        m_pMonkey->AddEventHandler(m_pNotifier);
        m_pMonkey->Run();
    }
    else if(nDomain != m_pMonkey->GetDomain())
    {
        m_pMonkey->SetDomain(nDomain);
    }
}

void wxPtp::ChangeDomain(unsigned char nDomain)
{
    if(m_pMonkey && m_pMonkey->GetDomain() != nDomain)
    {
        m_pMonkey->SetDomain(nDomain);
    }
}

void wxPtp::AddHandler(wxEvtHandler* pHandler)
{
    m_pNotifier->AddHandler(pHandler);
}

void wxPtp::RemoveHandler(wxEvtHandler* pHandler)
{
    m_pNotifier->RemoveHandler(pHandler);
}

void wxPtp::Stop()
{
    if(m_pMonkey)
    {
        m_pMonkey->Stop();
    }
}

std::shared_ptr<const PtpV2Clock> wxPtp::GetPtpClock(const wxString& sClockId)
{
    if(m_pMonkey)
    {
        return m_pMonkey->GetClock(std::string(sClockId.mb_str()));
    }
    return nullptr;
}

bool wxPtp::IsSyncedToMaster() const
{
    return (m_pMonkey && m_pMonkey->IsSyncedToMaster());
}

void wxPtp::ResyncToMaster()
{
    if(m_pMonkey)
    {
        m_pMonkey->ResyncToMaster();
    }
}

wxString wxPtp::GetMasterClockId()
{
    if(m_pMonkey)
    {
        return wxString::FromUTF8(m_pMonkey->GetMasterClockId().c_str());
    }
    return wxEmptyString;
}

std::shared_ptr<const PtpV2Clock> wxPtp::GetSyncMasterClock()
{
    if(m_pMonkey)
    {
        return m_pMonkey->GetSyncMasterClock();
    }
    return nullptr;
}

std::shared_ptr<const PtpV2Clock> wxPtp::GetLocalClock()
{
    if(m_pMonkey)
    {
        return m_pMonkey->GetLocalClock();
    }
    return nullptr;
}

void wxPtp::ResetLocalClockStats()
{
    if(m_pMonkey)
    {
        m_pMonkey->ResetLocalClockStats();
    }
}

timeval wxPtp::GetPtpTime()
{
    if(m_pMonkey)
    {
        timeval tv;
        std::pair<std::chrono::seconds, std::chrono::nanoseconds> ptpTime = Split(m_pMonkey->GetPtpTime());
        tv.tv_sec = ptpTime.first.count();
        tv.tv_usec =ptpTime.second.count()/1000;
        return tv;
    }
    return {0,0};
    
}

timespec wxPtp::GetPtpTimeSpec()
{
    if(m_pMonkey)
    {
        timespec ts{0,0};
        std::pair<std::chrono::seconds, std::chrono::nanoseconds> ptpTime = Split(m_pMonkey->GetPtpTime());
        ts.tv_sec = ptpTime.first.count();
        ts.tv_nsec = ptpTime.second.count();
        return ts;
    }
    return {0,0};
}

const std::map<std::string, std::shared_ptr<PtpV2Clock> >& wxPtp::GetClocks() const
{
    if(m_pMonkey)
    {
        return m_pMonkey->GetClocks();
    }
    return m_mEmpty;
}

timeval wxPtp::GetPtpOffset()
{
    if(m_pMonkey)
    {
        timeval tv;
        std::pair<std::chrono::seconds, std::chrono::nanoseconds> ptpTime = Split(m_pMonkey->GetPtpOffset());
        tv.tv_sec = ptpTime.first.count();
        tv.tv_usec = ptpTime.second.count()/1000;
        return tv;
    }
    return {0,0};
}

timeval wxPtp::GetLastPtpOffset()
{
    if(m_pMonkey)
    {
        timeval tv;
        std::pair<std::chrono::seconds, std::chrono::nanoseconds> ptpTime = Split(m_pMonkey->GetPtpOffset());
        tv.tv_sec = ptpTime.first.count();
        tv.tv_usec = ptpTime.second.count()/1000;
        return tv;
    }
    return {0,0};
}

