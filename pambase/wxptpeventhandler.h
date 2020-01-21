#pragma once
#include "ptpeventhander.h"

class wxPtpEventHandler : public PtpEventHandler
{
    public:
        void AddHandler(wxEvtHandler* pHandler);

        void ClockAdded(std::shared_ptr<PtpV2Clock> pClock) override;
        void ClockUpdated(std::shared_ptr<PtpV2Clock> pClock) override;
        void ClockRemoved(std::shared_ptr<PtpV2Clock> pClock) override;
        void ClockTimeCalculated(std::shared_ptr<PtpV2Clock> pClock) override;
        void ClockBecomeMaster(std::shared_ptr<PtpV2Clock> pClock) override;
        void ClockBecomeSlave(std::shared_ptr<PtpV2Clock> pClock) override;

    protected:
        void NotifyHandlers(wxEventType type, const wxString& sClockId);
        std::list<wxEvtHandler*> m_lstHandlers;
};




