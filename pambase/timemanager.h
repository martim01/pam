#ifndef TIMEMANAGER_H
#define TIMEMANAGER_H

#include <wx/event.h>
#include <map>

class SettingEvent;

class TimeManager : public wxEvtHandler
{
    public:
        static TimeManager& Get();

        enum enumSync {SYNC_OFF=0, SYNC_NTP, SYNC_LTC, SYNC_PTP};


        enumSync GetCurrentSync() const {return m_eCurrentSync;}


        std::map<wxString, bool> GetNtpServers();
        void SetNtpServers(const std::map<wxString, bool>& mServers);

        std::map<wxString, wxString> GetNtpTracking();

    protected:
        TimeManager();
        virtual ~TimeManager();

    private:

        void EnableSyncToPtp(bool bSync);
        void EnableSyncToNtp(bool bSync);
        void EnableSyncToLtc(bool bSync);

        void OnSettingChanged(SettingEvent& event);
        void OnPtpClockSync(wxCommandEvent& event);
        void OnPtpClockRemoved(wxCommandEvent& event);

        void DoSync();
        bool TrySyncToPtp();
        bool TrySyncToNtp();
        bool TrySyncToLtc();

        void StopCurrentSync();
        bool ManageChrony(const wxString& sAction);

        bool m_bNTP;
        bool m_bPTP;
        bool m_bLTC;

        size_t m_nPtpDomain;

        enumSync m_eCurrentSync;

        unsigned long m_nSyncCount;

};

#endif // TIMEMANAGER_H
