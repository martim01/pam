#include "timemanager.h"
#include "wxptp.h"
#include "log.h"
#include <wx/textfile.h>
#include "settingevent.h"
#include "settings.h"



TimeManager& TimeManager::Get()
{
    static TimeManager manager;
    return manager;
}

TimeManager::TimeManager() :
    m_bNTP(true),
    m_bPTP(false),
    m_bLTC(false),
    m_nPtpDomain(0)
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

    m_nPtpDomain = Settings::Get().Read("Time", "LTC_Format", 0);
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

bool TimeManager::TrySyncToPtp()
{
    if(m_bPTP)
    {
        if(m_eCurrentSync == SYNC_PTP)
        {
            return true;
        }

        if(wxPtp::Get().IsSyncedToMaster(m_nPtpDomain))
        {
            StopCurrentSync();


            timespec ts = wxPtp::Get().GetPtpTimeSpec(m_nPtpDomain);
            if(ts.tv_sec != 0)
            {
                pmlLog() << "Setting system clock from PTP";
                #ifdef __WXGNU__
                if(clock_settime(CLOCK_REALTIME, &ts) == 0)
                {
                    m_eCurrentSync = SYNC_PTP;
                    return true;
                }
                pmlLog(pml::LOG_ERROR) << strerror(errno);
                #endif // __WXGNU__
            }
            else
            {
                pmlLog(pml::LOG_ERROR) << "Could not get time from PTP master";
            }
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
