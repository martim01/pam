#pragma once
#include "ptpeventhander.h"
#include <set>

namespace pml::ptpmonkey
{
    class PtpV2Clock;
}

class wxPtpEventHandler : public pml::ptpmonkey::PtpEventHandler
{
    public:
        void AddHandler(wxEvtHandler* pHandler);
        void RemoveHandler(wxEvtHandler* pHandler);

        void ClockAdded(std::shared_ptr<pml::ptpmonkey::PtpV2Clock> pClock) override;
        void ClockUpdated(std::shared_ptr<pml::ptpmonkey::PtpV2Clock> pClock) override;
        void ClockRemoved(std::shared_ptr<pml::ptpmonkey::PtpV2Clock> pClock) override;
        void ClockTimeCalculated(std::shared_ptr<pml::ptpmonkey::PtpV2Clock> pClock) override;
        void ClockBecomeMaster(std::shared_ptr<pml::ptpmonkey::PtpV2Clock> pClock) override;
        void ClockBecomeSlave(std::shared_ptr<pml::ptpmonkey::PtpV2Clock> pClock) override;

        void AnnounceSent(std::shared_ptr<pml::ptpmonkey::PtpV2Clock> pClock) override;
        void SyncSent(std::shared_ptr<pml::ptpmonkey::PtpV2Clock> pClock) override;
        void FollowUpSent(std::shared_ptr<pml::ptpmonkey::PtpV2Clock> pClock) override;
        void DelayRequestSent(std::shared_ptr<pml::ptpmonkey::PtpV2Clock> pClock) override;
        void DelayResponseSent(std::shared_ptr<pml::ptpmonkey::PtpV2Clock> pClock) override;

        void ManagementMessageReceived(std::shared_ptr<pml::ptpmonkey::ptpV2Header> pHeader, std::shared_ptr<pml::ptpmonkey::ptpManagement> pMessage) override{};

    protected:
        void NotifyHandlers(wxEventType type, const wxString& sClockId);
        std::set<wxEvtHandler*> m_setHandlers;
};




