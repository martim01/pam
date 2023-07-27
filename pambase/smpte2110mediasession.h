#pragma once
#include <wx/string.h>
#include "MediaSession.hh"
#include "timedbuffer.h"
#include "session.h"
#include <map>
#include <vector>
#include "qos.h"


class Smpte2110MediaSubsession;


class Smpte2110MediaSession : public MediaSession
{
    public:
        static Smpte2110MediaSession* createNew(UsageEnvironment& env, char const* sdpDescription);


        const wxString& GetRawSDP() const
        {
            return m_sRawSDP;
        }

        const refclk& GetRefClock() const
        {
            return m_refclk;
        }

        double GetPackageTime() const
        {
            return m_dPackageMs;
        }

        double GetMaxPackageTime() const
        {
            return m_dMaxPackageMs;
        }

        const std::set<wxString>& GetGroups() const
        {
            return m_setGroups;
        }

        


    protected:


        Smpte2110MediaSession(UsageEnvironment& env);
        void initializeSMPTE_SDP(char const* sdpDescription);

        Boolean parseSDPAttribute_RefClk(char const* sdpLine);
        Boolean parseSDPAttribute_ClockDomain(char const* sdpLine);
        Boolean parseSDPAttribute_PTime(char const* sdpLine);
        Boolean parseSDPAttribute_MaxPTime(char const* sdpLine);
        Boolean parseSDPAttribute_Group(char const* sdpLine);

        MediaSubsession* createNewMediaSubsession();

        wxString m_sRawSDP;

        std::set<wxString> m_setGroups;
        refclk m_refclk;
        double m_dPackageMs;
        double m_dMaxPackageMs;

};


class Smpte2110MediaSubsession : public MediaSubsession
{
    public:
        unsigned long GetSyncTime()
        {
            return m_nSyncTime;
        }

        timeval GetLastEpoch();

        wxString GetEndpoint();

        refclk GetRefClock() const
        {
            return m_refclk;
        }
        double GetPackageTime() const
        {
            return m_dPackageMs;
        }

        double GetMaxPackageTime() const
        {
            return m_dMaxPackageMs;
        }

        const wxString& GetGroup() const { return m_sGroup; }

        std::map<unsigned long, wxString>::const_iterator GetExtHeaderBegin() const;
        std::map<unsigned long, wxString>::const_iterator GetExtHeaderEnd() const;
        std::map<unsigned long, wxString>::const_iterator GetExtHeader(unsigned long nId) const;

        void SetHandler(RtpThread* pThread) { m_qos.SetHandler(pThread);}
    
        const std::vector<subsession::channelGrouping>& GetChannelGrouping() const { return m_channels;}

        const qosMeasurementRecord& GetQoS() const { return m_qos;}

        void ScheduleNextQOSMeasurement();
        void PeriodicQOSMeasurement();

        UsageEnvironment& GetEnv() { return env(); }

    protected:
        friend class Smpte2110MediaSession;
        explicit Smpte2110MediaSubsession(MediaSession& parent);
        virtual Boolean createSourceObjects(int useSpecialRTPoffset);

        bool SetChannelGrouping(size_t& nChannel, const subsession::channelGrouping& grouping);

        void  parseSDPAttribute_Sync();
        void  parseSDPAttribute_Deviation();
        void  parseSDPAttribute_RefClk();
        void parseSDPAttribute_ClockDomain();
        void  parseSDPAttribute_PTime();
        void parseSDPAttribute_MaxPTime();
        void parseSDPAttribute_ExtMap();
        void parseSDPAttribute_Mid();
        void parseSDPAttribute_Channels();

        

        unsigned long m_nSyncTime = 0;
        unsigned int m_nFirstTimestamp = 0;
        wxString m_sEndpoint;

        double m_dClockDeviation = 0.0;


        refclk m_refclk;
        double m_dPackageMs = 0.0;
        double m_dMaxPackageMs = 0.0;

        std::map<unsigned long, wxString> m_mExtHeader;

        wxString m_sGroup;
    
        std::vector<subsession::channelGrouping> m_channels;
    
        qosMeasurementRecord m_qos;
        TaskToken m_qosMeasurementTimerTask = nullptr;

};
