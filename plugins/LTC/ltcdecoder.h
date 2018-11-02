#pragma once
#include "ltc.h"
#include <wx/string.h>

struct timedbuffer;

class LtcDecoder
{
    public:
        LtcDecoder();
        ~LtcDecoder();
        bool DecodeLtc(const timedbuffer* pBuffer, unsigned int nTotalChannels, unsigned int nChannel);

        const wxString& GetDate() const;
        const wxString& GetTime() const;
        const wxString& GetFrameStart() const;
        const wxString& GetFrameEnd() const;
        const wxString& GetAmplitude() const;
        const wxString& GetRaw() const;
        const wxString& GetFPS() const;
        const wxString& GetMode() const;
        const wxString& GetFormat() const;

        bool IsColourFlagSet() const;
        bool IsClockFlagSet() const;

        void SetDateMode(int nMode);

        enum {UNKNOWN, SMPTE, BBC, TVE, MTD};

    private:

        void CreateRaw();

        int WorkoutUserMode();
        void DecodeDateAndTime(int nUserMode);
        bool DecodeDateAndTime(SMPTETimecode& stime, int nDateMode);

        void ltc_frame_to_time_bbc(SMPTETimecode& stime);
        void ltc_frame_to_time_tve(SMPTETimecode& stime);
        void ltc_frame_to_time_mtd(SMPTETimecode& stime);
        void ltc_frame_to_time_only(SMPTETimecode& stime);

        LTCDecoder* m_pDecoder;
        LTCFrameExt m_Frame;
        wxString m_sDate;
        wxString m_sTime;
        wxString m_sFrameStart;
        wxString m_sFrameEnd;
        wxString m_sAmpltitude;
        wxString m_sFPS;
        wxString m_sRaw;
        wxString m_sMode;
        wxString m_sDateFormat;




        size_t m_nTotal;
        unsigned char m_nFPS;
        unsigned char m_nLastFrame;
        unsigned char m_nLastFPS;
        unsigned int m_nDateMode;

        static const int APV = 1920;

        static const wxString STR_MODE[4];
        static const wxString STR_DATE_MODE[5];
};
