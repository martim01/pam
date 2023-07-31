#pragma once
#include "ptpeventhander.h"
#include <map>

namespace ptpmonkey
{
    class PtpV2Clock;
}

class wxPtpEventHandler : public ptpmonkey::PtpEventHandler
{
    public:
        void AddHandler(wxEvtHandler* pHandler, unsigned char nDomain);
        void RemoveHandler(wxEvtHandler* pHandler, unsigned char nDomain);

        void ClockAdded(std::shared_ptr<ptpmonkey::PtpV2Clock> pClock) override;
        void ClockUpdated(std::shared_ptr<ptpmonkey::PtpV2Clock> pClock) override;
        void ClockRemoved(std::shared_ptr<ptpmonkey::PtpV2Clock> pClock) override;
        void ClockTimeCalculated(std::shared_ptr<ptpmonkey::PtpV2Clock> pClock) override;
        void ClockBecomeMaster(std::shared_ptr<ptpmonkey::PtpV2Clock> pClock) override;
        void ClockBecomeSlave(std::shared_ptr<ptpmonkey::PtpV2Clock> pClock) override;

        void AnnounceSent(std::shared_ptr<ptpmonkey::PtpV2Clock> pClock) override;
        void SyncSent(std::shared_ptr<ptpmonkey::PtpV2Clock> pClock) override;
        void FollowUpSent(std::shared_ptr<ptpmonkey::PtpV2Clock> pClock) override;
        void DelayRequestSent(std::shared_ptr<ptpmonkey::PtpV2Clock> pClock) override;
        void DelayResponseSent(std::shared_ptr<ptpmonkey::PtpV2Clock> pClock) override;

    protected:
        void NotifyHandlers(wxEventType type, const wxString& sClockId, unsigned char nDomain);
        std::multimap<unsigned char, wxEvtHandler*> m_mmHandlers;
};




