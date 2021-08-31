#ifndef TIMEMANAGER_H
#define TIMEMANAGER_H

#include <wx/event.h>
#include <map>
#include "dlldefine.h"

class SettingEvent;

class PAMBASE_IMPEXPORT TimeManager : public wxEvtHandler
{
    public:
        static TimeManager& Get();

        enum enumSync {SYNC_OFF=0, SYNC_NTP, SYNC_LTC, SYNC_PTP};


        enumSync GetCurrentSync() const {return m_eCurrentSync;}


        std::map<wxString, bool> GetNtpServers();
        void SetNtpServers(const std::map<wxString, bool>& mServers);

        std::map<wxString, wxString> GetNtpTracking();
        bool IsPtpFrequencyLocked() const {return m_bPtpLock;}

        bool HasNTP();

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

        bool PtpSyncFrequency();
        void StopCurrentSync();
        bool ManageChrony(const wxString& sAction);

        std::pair<bool, long> SetGetFrequency(std::pair<bool, long> setFreq);

        enumSync m_eSyncTo;
        size_t m_nPtpDomain;

        enumSync m_eCurrentSync;

        unsigned long m_nSyncCount;

        bool m_bPtpLock;
        size_t m_nPtpSamples;
        size_t m_nMinSamplSize;

        long m_nFrequency;

};

#endif // TIMEMANAGER_H
