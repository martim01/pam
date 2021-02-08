#pragma once
#include <wx/string.h>
#include "MediaSession.hh"
#include "timedbuffer.h"
#include "session.h"
#include <map>


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

        const wxString& GetGroupDup() const
        {
            return m_sGroups;
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

        wxString m_sGroups;
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

        std::map<unsigned long, wxString>::const_iterator GetExtHeaderBegin() const;
        std::map<unsigned long, wxString>::const_iterator GetExtHeaderEnd() const;
        std::map<unsigned long, wxString>::const_iterator GetExtHeader(unsigned long nId) const;

        int GetSampling();
        unsigned char GetDepth();
        bool AreSamplesInteger();
        unsigned int GetWidth();
        unsigned int GetHeight();
        std::pair<unsigned long, unsigned long> GetExactFrameRate();
        int GetColorimetry();
        int GetPackingMode();
        wxString GetSSN();
        bool IsInterlaced();
        bool IsSegmented();
        int GetTCS();
        int GetRange();
        bool UseMaxUDP();
        std::pair<unsigned long, unsigned long> GetAspectRatio();


        enum Sampling{YCbCr_444, YCbCr_422, YCbCr_420, CLYCbCr_444, CLYCbCr_422, CLYCbCr_420, ICtCp_444, ICtCp_422, ICtCp_420, RGB_, XYZ, KEY, UNKNOWN};
        enum Colorimetry{C_BT601, C_BT709, BC_T2020, C_BT2100, C_ST2065_1, C_ST2065_3, C_XYZ, C_UNSPECIFIED};
        enum TCS{SDR, PQ, HLG, LINEAR, BT2100LINPQ, BT2100LINHLG, ST2065_1, ST2428_1, DENSITY, UNSPECIFIED};
        enum Range{NARROW, FULLPROTECT, FULL};
        enum Packing{GPM, BPM};

        static const wxString STR_SAMPLING[13];
        static const wxString STR_COLORIMETRY[8];
        static const wxString STR_TCS[10];
        static const wxString STR_RANGE[3];
        static const wxString STR_PACKING[2];


    protected:
        void AnalyzeAttributes();

        friend class Smpte2110MediaSession;
        Smpte2110MediaSubsession(MediaSession& parent);
        virtual Boolean createSourceObjects(int useSpecialRTPoffset);



        void  parseSDPAttribute_Sync();
        void  parseSDPAttribute_Deviation();
        void  parseSDPAttribute_RefClk();
        void parseSDPAttribute_ClockDomain();
        void  parseSDPAttribute_PTime();
        void parseSDPAttribute_MaxPTime();
        void parseSDPAttribute_ExtMap();
        void parseSDPAttribute_Mid();

        unsigned long m_nSyncTime;
        unsigned int m_nFirstTimestamp;
        wxString m_sEndpoint;

        double m_dClockDeviation;


        refclk m_refclk;
        double m_dPackageMs;
        double m_dMaxPackageMs;

        std::map<unsigned long, wxString> m_mExtHeader;

        wxString m_sGroup;


        int m_nSampling;
        unsigned char m_nDepth;
        bool m_bFloating;
        unsigned long m_nWidth;
        unsigned long m_nHeight;
        std::pair<unsigned long, unsigned long> m_pairFrameRate;
        int m_nColorimetry;
        int m_nPackingMode;
        wxString m_sSSN;
        bool m_bInterlaced;
        bool m_bSegmented;
        int m_nTCS;
        int m_nRange;
        bool m_bMaxUdp;
        std::pair<unsigned long, unsigned long> m_pairAspectRatio;
};
