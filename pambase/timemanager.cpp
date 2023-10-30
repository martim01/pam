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
    m_eSyncTo(SYNC_OFF),
    m_nPtpDomain(0),
    m_bUseTai((Settings::Get().Read("Time", "Tai", 0 ) != 0)),
    m_nSyncCount(0),
    m_bPtpLock(false),
    m_nMinSamplSize(10),
    m_nPtpSamples(0)
{
    wxPtp::Get().AddHandler(this);
    Bind(wxEVT_CLOCK_TIME, &TimeManager::OnPtpClockSync, this);
    Bind(wxEVT_CLOCK_REMOVED, &TimeManager::OnPtpClockRemoved, this);

    Settings::Get().AddHandler(this, "Time");

    Settings::Get().Write("Time", "Grandmaster", "");   //only just started so currently have no Grandmaster
    m_eSyncTo = Settings::Get().Read("Time", "Sync", SYNC_OFF);


    Bind(wxEVT_SETTING_CHANGED, &TimeManager::OnSettingChanged, this);


    DoSync();
}

TimeManager::~TimeManager()
{
    //dtor
    Settings::Get().RemoveHandler(this);
}

void TimeManager::OnSettingChanged(SettingEvent& event)
{
    if(event.GetKey() == "Sync")
    {
        if(m_eSyncTo != event.GetValue(long(SYNC_OFF)))
        {
            m_eSyncTo = event.GetValue(long(SYNC_OFF));
            StopCurrentSync();
            DoSync();
        }
    }
    else if(event.GetKey() == "PTP_Domain")
    {
        m_nPtpDomain = event.GetValue((long)0);
    }
    else if(event.GetKey() == "Tai")
    {
        m_bUseTai = event.GetValue(false);
    }
    else if(event.GetKey() == "LTC_Format")
    {
        //@todo
    }
}

void TimeManager::OnPtpClockSync(wxCommandEvent& event)
{
    if(m_eSyncTo == SYNC_PTP)
    {
        TrySyncToPtp();
    }

}


void TimeManager::DoSync()
{
    bool bSuccess(true);
    switch(m_eSyncTo)
    {
        case SYNC_NTP:
            bSuccess = TrySyncToNtp();
            break;
        case SYNC_PTP:
            break;
        case SYNC_LTC:
            bSuccess = TrySyncToLtc();
            break;
    }

    if(!bSuccess)
    {
        pmlLog(pml::LOG_ERROR) << "Could not sync time";
    }
}

bool TimeManager::PtpSyncFrequency()
{
    if(m_eSyncTo == SYNC_PTP)
    {
        m_nPtpSamples++;

        auto pLocal = wxPtp::Get().GetLocalClock();
        if(pLocal)
        {
            StopCurrentSync();

            auto pMaster = wxPtp::Get().GetSyncMasterClock();
            auto offset = pLocal->GetOffset(pml::ptpmonkey::PtpV2Clock::CURRENT);//DoubleToTime(dEstimate);

            //store the PTP details for use by NMOS etc
            Settings::Get().Write("Time", "Grandmaster", wxString(pMaster->GetClockId()));


            if(!m_bUseTai)
            {
                auto utc = std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::seconds(pMaster->GetUtcOffset()));
                offset += utc;
            }

            if(abs(std::chrono::duration_cast<std::chrono::milliseconds>(offset).count()) > 500)
            {
                if(m_nPtpSamples > 1)   //first stat is often 37s out
                {
                    auto now = std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::system_clock::now().time_since_epoch());
                    auto hardSetM = now-offset;
                    auto split = Split(hardSetM);



                    timespec tv;
                    tv.tv_sec = split.first.count();
                    tv.tv_nsec = split.second.count();

                    if(clock_settime(CLOCK_REALTIME, &tv) != 0)
                    {
                        pmlLog(pml::LOG_ERROR) << "Failed to hard crash " <<strerror(errno);
                    }
                    else
                    {
                        pmlLog() << "PTP: Hard crashed to " << TimeToIsoString(hardSetM);
                        clock_gettime(CLOCK_REALTIME, &tv);
                        pmlLog() << "PTP: Current time now: " << ctime(&tv.tv_sec);
                    }
                    wxPtp::Get().ResetLocalClockStats();
                    m_nPtpSamples = 0;
                    m_bPtpLock = false;
                }
                return true;
            }
            else
            {
                if(m_nPtpSamples > m_nMinSamplSize)
                {
                    auto slope = pLocal->GetOffsetSlope()*1e6;   //slope in ppm
                    auto origin = pLocal->GetOffsetIntersection();

                    auto setFreq = SetGetFrequency(std::make_pair(false,0));
                    if(!setFreq.first)
                    {
                        return false;
                    }

                    double dOffsetFreq = slope*65535.0;
                    m_nFrequency =setFreq.second-dOffsetFreq; //this is the frequency to keep the clock in sync


                    auto nOffsetPPM = std::chrono::duration_cast<std::chrono::microseconds>(offset).count();
                    auto nNewFreq = m_nFrequency - nOffsetPPM;
                    wxPtp::Get().ResetLocalClockStats();
                    m_nPtpSamples = 0;

                    std::thread th([this, nNewFreq]{
                                   std::this_thread::sleep_for(std::chrono::milliseconds(1000));
                                   SetGetFrequency(std::make_pair(true, nNewFreq));
                                   });

                    th.detach();
                    return true;
                }
                else
                {
                    return true;
                }
            }
        }
    }
    return false;
}

std::pair<bool, long> TimeManager::SetGetFrequency(std::pair<bool, long> setFreq)
{
    timex buf;
    memset(&buf, 0,sizeof(buf));
    if(setFreq.first)
    {
        buf.freq = setFreq.second;
        buf.modes = ADJ_FREQUENCY;
        pmlLog() << "Set frequency to: " << setFreq.second;
    }

    if(adjtimex(&buf) == -1)
    {
        pmlLog(pml::LOG_ERROR) << "Failed to read/write frequency " <<strerror(errno);
        return std::make_pair(false,0);
    }
    return std::make_pair(true, buf.freq);
}

bool TimeManager::TrySyncToPtp()
{
    m_nPtpSamples++;

    auto pLocal = wxPtp::Get().GetLocalClock();
    if(pLocal)
    {


        StopCurrentSync();

        auto pMaster = wxPtp::Get().GetSyncMasterClock();

        auto offset = pLocal->GetOffset(pml::ptpmonkey::PtpV2Clock::CURRENT);//DoubleToTime(dEstimate);

        auto slope = pLocal->GetOffsetSlope()*1e6;   //slope in ppm

        auto utc = std::chrono::nanoseconds(0);
        if(m_bUseTai)
        {
            utc = std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::seconds(pMaster->GetUtcOffset()));
            offset += utc;
        }

        if(abs(std::chrono::duration_cast<std::chrono::milliseconds>(offset).count()) > 500)
        {
            if(m_nPtpSamples > 1)   //first stat is often 37s out
            {
                auto now = std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::system_clock::now().time_since_epoch());
                auto hardSetM = now-offset;
                auto split = Split(hardSetM);

                timespec tv;
                tv.tv_sec = split.first.count();
                tv.tv_nsec = split.second.count();

                if(clock_settime(CLOCK_REALTIME, &tv) != 0)
                {
                    pmlLog(pml::LOG_ERROR) << "Failed to hard crash " <<strerror(errno);
                }
                else
                {
                    pmlLog() << "PTP: Hard crashed to " << TimeToIsoString(hardSetM);
                    clock_gettime(CLOCK_REALTIME, &tv);
                    pmlLog() << "PTP: Current time now: " << ctime(&tv.tv_sec);
                }
                wxPtp::Get().ResetLocalClockStats();
                m_nPtpSamples = 0;
                m_bPtpLock = false;
            }
            return true;
        }

        if(!m_bPtpLock && m_nPtpSamples < m_nMinSamplSize) //
        {   //waiting for more info
            return true;
        }

        if(false && !m_bPtpLock && abs(slope) > 0.2)
        {
            pmlLog() << "PTP: Clock frequency adjust: " << slope;

            timex buf;
            memset(&buf, 0,sizeof(buf));
            if(adjtimex(&buf) == -1)
            {
                pmlLog(pml::LOG_ERROR) << "Failed to read frequency " <<strerror(errno);
                return false;
            }

            double dOffsetFreq = slope*65535.0;
            buf.freq -= dOffsetFreq;
            buf.modes = ADJ_FREQUENCY;

            if(adjtimex(&buf) == -1)
            {
                pmlLog(pml::LOG_ERROR) << "Failed to set frequency " <<strerror(errno);
                return false;
            }
            wxPtp::Get().ResetLocalClockStats();
            m_nPtpSamples = 0;
            m_bPtpLock = false;
            return true;
        }
        else
        {
            m_bPtpLock = true;

            auto mean = pLocal->GetOffset(pml::ptpmonkey::PtpV2Clock::MEAN);
            auto sd = pLocal->GetOffset(pml::ptpmonkey::PtpV2Clock::SD);
            auto maxBand = mean+sd+utc;
            auto minBand = mean-sd+utc;

            auto dUTCOffset = static_cast<double>(wxPtp::Get().GetSyncMasterClock()->GetUtcOffset());
            
            auto m = pLocal->GetOffsetSlope();
            auto c = pLocal->GetOffsetIntersection();
            auto now = std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::system_clock::now().time_since_epoch());
            auto dEstimate = (c + m * TimeToDouble(now-pLocal->GetFirstOffsetTime()));
            dEstimate = (dEstimate+dUTCOffset)*1e9;

            //offset = std::chrono::nanoseconds(static_cast<long>(dEstimate));
            offset = std::max(minBand, std::min(maxBand, offset));

            auto split = Split(-offset);
            timeval tv;
            tv.tv_sec = split.first.count();
            tv.tv_usec = split.second.count()/1000;
            while(tv.tv_usec < 0)
            {
                tv.tv_sec -= 1;
                tv.tv_usec += 1000000;
            }

            timeval tvOld;
            adjtime(nullptr, &tvOld);

            if(adjtime(&tv, nullptr) != 0)
            {
                pmlLog(pml::LOG_ERROR) << "Could not set time: " <<strerror(errno);
            }
            m_nPtpSamples = 0;
            return true;
        }

    }
    else
    {
        pmlLog(pml::LOG_WARN) << "Not synced to PTP master";
    }
    return false;
}

bool TimeManager::TrySyncToNtp()
{
    if(m_eCurrentSync == SYNC_NTP)
    {
        return true;
    }
    StopCurrentSync();

    return ManageChrony("start");
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

bool TimeManager::HasNTP()
{
    if(wxFileExists("/usr/sbin/chronyd"))
    {
        return true;
    }
    return false;
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
    if(wxPtp::Get().GetSyncMasterClock() == nullptr && m_eCurrentSync == SYNC_PTP)
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
