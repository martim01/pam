#pragma once
#include <wx/string.h>
#include "MediaSession.hh"
#include "timedbuffer.h"
#include "session.h"
#include <map>


class Aes67MediaSubsession;


class Aes67MediaSession : public MediaSession
{
    public:
        static Aes67MediaSession* createNew(UsageEnvironment& env, char const* sdpDescription);

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

        const wxString& GetGroupDup() const
        {
            return m_sGroups;
        }



    protected:


        Aes67MediaSession(UsageEnvironment& env);
        //~Aes67MediaSession();
        void initializeSMPTE_SDP(char const* sdpDescription);

        Boolean parseSDPAttribute_RefClk(char const* sdpLine);
        Boolean parseSDPAttribute_ClockDomain(char const* sdpLine);
        Boolean parseSDPAttribute_PTime(char const* sdpLine);
        Boolean parseSDPAttribute_MaxPTime(char const* sdpLine);
        Boolean parseSDPAttribute_Group(char const* sdpLine);

        MediaSubsession* createNewMediaSubsession();

        wxString m_sRawSDP;

        wxString m_sGroups;
        refclk m_refclk;
        double m_dPackageMs;
        double m_dMaxPackageMs;
};


class Aes67MediaSubsession : public MediaSubsession
{
    public:
        unsigned long GetSyncTime()
        {
            return m_nSyncTime;
        }

        const timeval& GetLastEpoch();

        wxString GetEndpoint();

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

        std::map<unsigned long, wxString>::const_iterator GetExtHeaderBegin() const;
        std::map<unsigned long, wxString>::const_iterator GetExtHeaderEnd() const;
        std::map<unsigned long, wxString>::const_iterator GetExtHeader(unsigned long nId) const;

    protected:
        friend class Aes67MediaSession;
        Aes67MediaSubsession(MediaSession& parent);
        virtual Boolean createSourceObjects(int useSpecialRTPoffset);



        void  parseSDPAttribute_Sync();
        void  parseSDPAttribute_Deviation();
        void  parseSDPAttribute_RefClk();
        void parseSDPAttribute_ClockDomain();
        void  parseSDPAttribute_PTime();
        void parseSDPAttribute_MaxPTime();
        void parseSDPAttribute_ExtMap();

        unsigned long m_nSyncTime;
        unsigned int m_nFirstTimestamp;
        wxString m_sEndpoint;

        double m_dClockDeviation;


        refclk m_refclk;
        double m_dPackageMs;
        double m_dMaxPackageMs;

        std::map<unsigned long, wxString> m_mExtHeader;
};
