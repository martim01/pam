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

    private:

        void CreateRaw();
        void ltc_frame_to_time_bbc(SMPTETimecode* stime, LTCFrame* frame);

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

        size_t m_nTotal;
        unsigned char m_nFPS;

        static const int APV = 1920;
};
