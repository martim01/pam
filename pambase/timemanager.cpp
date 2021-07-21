#include "timemanager.h"
#include "wxptp.h"
#include "log.h"
#include <wx/textfile.h>
#include "settingevent.h"
#include "settings.h"
#include "ptpclock.h"
#include <sys/timex.h>
#include <sys/time.h>


TimeManager& TimeManager::Get()
{
    static TimeManager manager;
    return manager;
}

TimeManager::TimeManager() :
    m_bNTP(true),
    m_bPTP(false),
    m_bLTC(false),
    m_nPtpDomain(0),
    m_nSyncCount(0)
{
    wxPtp::Get().AddHandler(this);
    Bind(wxEVT_CLOCK_TIME, &TimeManager::OnPtpClockSync, this);
    Bind(wxEVT_CLOCK_REMOVED, &TimeManager::OnPtpClockRemoved, this);

    Settings::Get().AddHandler("Time", "NTP", this);
    Settings::Get().AddHandler("Time", "PTP", this);
    Settings::Get().AddHandler("Time", "PTP", this);
    Settings::Get().AddHandler("Time", "PTP_Domain", this);
    Settings::Get().AddHandler("Time", "LTC_Format", this);

    Bind(wxEVT_SETTING_CHANGED, &TimeManager::OnSettingChanged, this);

    EnableSyncToNtp(Settings::Get().Read("Time", "NTP", 1) == 1);
    EnableSyncToLtc(Settings::Get().Read("Time", "LTC", 0) == 1);
    EnableSyncToPtp(Settings::Get().Read("Time", "PTP", 0) == 1);

    //m_nPtpDomain = Settings::Get().Read("Time", "LTC_Format", 0);
}

TimeManager::~TimeManager()
{
    //dtor
}

void TimeManager::OnSettingChanged(SettingEvent& event)
{
    if(event.GetKey() == "NTP")
    {
        EnableSyncToNtp(event.GetValue(true));
    }
    else if(event.GetKey() == "PTP")
    {
        EnableSyncToPtp(event.GetValue(false));
    }
    else if(event.GetKey() == "LTC")
    {
        EnableSyncToLtc(event.GetValue(false));
    }
    else if(event.GetKey() == "PTP_Domain")
    {
        m_nPtpDomain = event.GetValue((long)0);
    }
    else if(event.GetKey() == "LTC_Format")
    {
        //@todo
    }
}

void TimeManager::OnPtpClockSync(wxCommandEvent& event)
{
    DoSync();

}


void TimeManager::EnableSyncToPtp(bool bSync)
{
    if(bSync == m_bPTP)
    {
        return;
    }

    m_bPTP = bSync;
    DoSync();
}

void TimeManager::EnableSyncToNtp(bool bSync)
{
    if(bSync == m_bNTP)
    {
        return;
    }

    m_bNTP = bSync;
    DoSync();
}

void TimeManager::EnableSyncToLtc(bool bSync)
{
    if(bSync == m_bLTC)
    {
        return;
    }

    m_bLTC = bSync;
    DoSync();
}

void TimeManager::DoSync()
{
    if(!TrySyncToPtp())
    {
        if(!TrySyncToLtc())
        {
            if(!TrySyncToNtp())
            {
                pmlLog(pml::LOG_ERROR) << "Could not sync time";
            }
        }
    }

}
//CAP_SYS_TIME
bool TimeManager::TrySyncToPtp()
{

    if(m_bPTP)
    {


        auto pLocal = wxPtp::Get().GetLocalClock(m_nPtpDomain);
        if(pLocal && pLocal->IsSynced())
        {

            pmlLog() << "Attempt to sync to PTP domain" << m_nPtpDomain;

            StopCurrentSync();

            auto pMaster = wxPtp::Get().GetSyncMasterClock(m_nPtpDomain);

            double m = pLocal->GetOffsetSlope();
            double c = pLocal->GetOffsetIntersection();
            auto now = std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::system_clock::now().time_since_epoch());
            double dEstimate = (static_cast<double>(pMaster->GetUtcOffset())+c + m * TimeToDouble(now-pLocal->GetFirstOffsetTime()));

            auto offset = pLocal->GetOffset(ptpmonkey::PtpV2Clock::CURRENT);//DoubleToTime(dEstimate);
            auto mean = pLocal->GetOffset(ptpmonkey::PtpV2Clock::MEAN);
            auto sd = pLocal->GetOffset(ptpmonkey::PtpV2Clock::SD);

            auto maxBand = mean+sd;
            auto minBand = mean-sd;

            if(offset <= maxBand && offset >= minBand)
            {
                auto utc = std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::seconds(pMaster->GetUtcOffset()));
                pmlLog() << "Offset: " << TimeToString(offset) << " utc " << TimeToString(utc);
                offset += utc;
                pmlLog() << "Offset: " << TimeToString(offset);

                auto split = Split(-offset);

                timeval tv;
                tv.tv_sec = split.first.count();
                tv.tv_usec = split.second.count()/1000;

                adjtime(&tv, nullptr);
                pmlLog() << "Setting system clock offset from PTP to " << tv.tv_sec << ":" << tv.tv_usec << "us  slope=" << pLocal->GetOffsetSlope();

                return true;

                timex tx;
                memset(&tx, 0, sizeof(tx));

                tx.modes |= ADJ_SETOFFSET | ADJ_NANO;
                tx.time.tv_sec = split.first.count();
                tx.time.tv_usec = split.second.count();

                while(tx.time.tv_usec < 0)
                {
                    tx.time.tv_sec -=1;
                    tx.time.tv_usec += 1000000000;
                }

                pmlLog() << "Setting system clock offset from PTP to " << tx.time.tv_sec << ":" << tx.time.tv_usec << "ns  slope=" << pLocal->GetOffsetSlope();

                if(adjtimex(&tx) < 0)
                {
                    pmlLog(pml::LOG_ERROR) << "Failed to adjust clock offset " << strerror(errno);
                }
            }
            return true;
        }
        else
        {
            pmlLog(pml::LOG_WARN) << "Not synced to PTP master";
        }
    }
    return false;
}

bool TimeManager::TrySyncToNtp()
{
    if(m_bNTP)
    {
        if(m_eCurrentSync == SYNC_NTP)
        {
            return true;
        }
        StopCurrentSync();

        return ManageChrony("start");
    }
    else if(m_eCurrentSync == SYNC_NTP)
    {
        StopCurrentSync();
    }
    return false;
}

bool TimeManager::TrySyncToLtc()
{
    return false;
    // @todo LTC time sync
}

void TimeManager::StopCurrentSync()
{
    switch(m_eCurrentSync)
    {
        case SYNC_PTP:
            //nothing to do
            break;
        case SYNC_LTC:
            // @todo stop LTC time sync
            break;
        case SYNC_NTP:
            ManageChrony("stop");
    }
}


std::map<wxString, bool> TimeManager::GetNtpServers()
{
    std::map<wxString, bool> mServers;
    wxTextFile tf;
    if(tf.Open("/etc/chrony/chrony.conf"))
    {
        for(size_t i = 0; i < tf.GetLineCount(); i++)
        {
            if(tf[i].Left(4).CmpNoCase("pool") == 0)
            {
                mServers.insert(std::make_pair(tf[i].Mid(5).Before(' '), true));
            }
            else if(tf[i].Left(6).CmpNoCase("server") == 0)
            {
                mServers.insert(std::make_pair(tf[i].Mid(7).Before(' '), true));
            }
        }
    }
    return mServers;
}


void TimeManager::SetNtpServers(const std::map<wxString, bool>& mServers)
{
    pmlLog() << "Updating NTP servers";

    wxTextFile tf;
    if(tf.Open("/etc/chrony/chrony.conf"))
    {
        for(size_t i = 0; i < tf.GetLineCount();)
        {
            if(tf[i].Left(4).CmpNoCase("pool") == 0 || tf[i].Left(6).CmpNoCase("server") == 0 || tf[i].Left(5).CmpNoCase("# pam") == 0)
            {
                tf.RemoveLine(i);
            }
            else
            {
                ++i;
            }
        }
    }
    tf.AddLine("# pam added servers");
    for(auto pairServer : mServers)
    {
        if(pairServer.second)
        {
            tf.AddLine("pool "+pairServer.first+" iburst");
        }
        else
        {
            tf.AddLine("server "+pairServer.first+" iburst");
        }
    }
    if(tf.Write())
    {
        if(m_eCurrentSync == SYNC_NTP)
        {
            ManageChrony("restart");
        }
    }
    else
    {
        pmlLog(pml::LOG_ERROR) << "Could not write to chrony.conf";
    }
}


std::map<wxString, wxString> TimeManager::GetNtpTracking()
{
    std::map<wxString, wxString> mStats;
    return mStats;
}

void TimeManager::OnPtpClockRemoved(wxCommandEvent& event)
{
    if(wxPtp::Get().GetSyncMasterClock(m_nPtpDomain) == nullptr && m_eCurrentSync == SYNC_PTP)
    {
        pmlLog() << "PTP Sync Master disappeared.";
        DoSync();
    }
}

bool TimeManager::ManageChrony(const wxString& sAction)
{
    long nResult = wxExecute(wxString::Format("sudo systemctl %s chronyd.service", sAction.c_str()), wxEXEC_SYNC);
    if(nResult == 0)
    {
        m_eCurrentSync = SYNC_PTP;
        pmlLog() << "Started chronyd for NTP";
        return true;
    }
    pmlLog(pml::LOG_ERROR) << "Could not change status of chronyd.service";
    return false;
}
