#include "ltcdecoder.h"
#include <wx/datetime.h>
#include "timedbuffer.h"
#include <bitset>
#include <wx/log.h>
#include <algorithm>

LtcDecoder::LtcDecoder()
{
    m_pDecoder = ltc_decoder_create(APV, 32);
    m_nTotal = 0;
    m_nFPS = 0;
}

LtcDecoder::~LtcDecoder()
{
    ltc_decoder_free(m_pDecoder);
}


bool LtcDecoder::DecodeLtc(const timedbuffer* pBuffer, unsigned int nTotalChannels, unsigned int nChannel)
{

    bool bFrame(false);
    if(nTotalChannels != 0)
    {
        size_t nSize = pBuffer->GetBufferSize()/nTotalChannels;
        float* pSamples = new float[nSize];
        int nSample = 0;
        for(size_t i = nChannel; i < pBuffer->GetBufferSize(); i+=nTotalChannels)
        {
            pSamples[nSample] = pBuffer->GetBuffer()[i];
            nSample++;
        }

        ltc_decoder_write_float(m_pDecoder, pSamples, nSize, m_nTotal);
        while (ltc_decoder_read(m_pDecoder, &m_Frame))
        {
            bFrame = true;

            SMPTETimecode stime;


            if(m_Frame.ltc.binary_group_flag_bit0 == 1)
            {
                ltc_frame_to_time(&stime, &m_Frame.ltc, 1);
                m_sMode = wxT("LTC 309M");
            }
            else if(m_Frame.ltc.binary_group_flag_bit0 == 0)
            {
                ltc_frame_to_time_bbc(&stime, &m_Frame.ltc);
                m_sMode = wxT("EBU/Leitch");
            }
            else
            {
                wxLogDebug(wxT("%u %u"),m_Frame.ltc.binary_group_flag_bit0, m_Frame.ltc.binary_group_flag_bit2);
            }
            wxLogDebug(m_sMode);

            m_sDate.Printf(wxT("%04d-%02d-%02d %s"),
                            ((stime.years < 67) ? 2000+stime.years : 1900+stime.years),
                            stime.months,
                            stime.days,
                            stime.timezone);
            m_sTime.Printf(wxT("%02d:%02d:%02d%c%02d"),
                            stime.hours,
                            stime.mins,
                            stime.secs,
                            (m_Frame.ltc.dfbit) ? '.' : ':',
                            stime.frame);
            m_sFrameStart.Printf(wxT("%8lld"),m_Frame.off_end - m_Frame.off_start);
            m_sFrameEnd.Printf(wxT("%8lld"),m_Frame.off_end);

            m_sAmpltitude.Printf(wxT("%.2f dBFS"), m_Frame.volume);

            m_nFPS = std::max(stime.frame, m_nFPS);
            m_sFPS.Printf(wxT("%u"), (m_nFPS+1));

            CreateRaw();

        }
        delete[] pSamples;
        m_nTotal += nSize;
    }
    return bFrame;
}


const wxString& LtcDecoder::GetDate() const
{
    return m_sDate;
}
const wxString& LtcDecoder::GetTime() const
{
    return m_sTime;
}

const wxString& LtcDecoder::GetFrameStart() const
{
    return m_sFrameStart;
}

const wxString& LtcDecoder::GetFrameEnd() const
{
    return m_sFrameEnd;
}

const wxString& LtcDecoder::GetAmplitude() const
{
    return m_sAmpltitude;
}

const wxString& LtcDecoder::GetRaw() const
{
    return m_sRaw;
}

const wxString& LtcDecoder::GetFPS() const
{
    return m_sFPS;
}


void LtcDecoder::CreateRaw()
{
    std::string sRaw;
    std::string str;

    str = std::bitset<4>(m_Frame.ltc.frame_units).to_string();
    std::reverse(str.begin(), str.end());
    sRaw+=str;
    sRaw+= " ";
    str = std::bitset<4>(m_Frame.ltc.user1).to_string();
    std::reverse(str.begin(), str.end());
    sRaw+=str;
    sRaw+= " ";
    str = std::bitset<2>(m_Frame.ltc.frame_tens).to_string();
    std::reverse(str.begin(), str.end());
    sRaw+=str;
    sRaw+= " ";
    str = std::bitset<1>(m_Frame.ltc.dfbit).to_string();
    std::reverse(str.begin(), str.end());
    sRaw+=str;
    sRaw+= " ";
    str = std::bitset<1>(m_Frame.ltc.col_frame).to_string();
    std::reverse(str.begin(), str.end());
    sRaw+=str;
    sRaw+= " ";
    str = std::bitset<4>(m_Frame.ltc.user2).to_string();
    std::reverse(str.begin(), str.end());
    sRaw+=str;
    sRaw+= " ";
    str = std::bitset<4>(m_Frame.ltc.secs_units).to_string();
    std::reverse(str.begin(), str.end());
    sRaw+=str;
    sRaw+= " ";
    str = std::bitset<4>(m_Frame.ltc.user3).to_string();
    std::reverse(str.begin(), str.end());
    sRaw+=str;
    sRaw+= " ";
    str = std::bitset<3>(m_Frame.ltc.secs_tens).to_string();
    std::reverse(str.begin(), str.end());
    sRaw+=str;
    sRaw+= " ";
    str = std::bitset<1>(m_Frame.ltc.biphase_mark_phase_correction).to_string();
    std::reverse(str.begin(), str.end());
    sRaw+=str;
    sRaw+= " ";
    str = std::bitset<4>(m_Frame.ltc.user4).to_string();
    std::reverse(str.begin(), str.end());
    sRaw+=str;
    sRaw+= " ";
    str = std::bitset<4>(m_Frame.ltc.mins_units).to_string();
    std::reverse(str.begin(), str.end());
    sRaw+=str;
    sRaw+= " ";
    str = std::bitset<4>(m_Frame.ltc.user5).to_string();
    std::reverse(str.begin(), str.end());
    sRaw+=str;
    sRaw+= " ";
    str = std::bitset<3>(m_Frame.ltc.mins_tens).to_string();
    std::reverse(str.begin(), str.end());
    sRaw+=str;
    sRaw+= " ";
    str = std::bitset<1>(m_Frame.ltc.binary_group_flag_bit0).to_string();
    std::reverse(str.begin(), str.end());
    sRaw+=str;
    sRaw+= " ";
    str = std::bitset<4>(m_Frame.ltc.user6).to_string();
    std::reverse(str.begin(), str.end());
    sRaw+=str;
    sRaw+= " ";
    str = std::bitset<4>(m_Frame.ltc.hours_units).to_string();
    std::reverse(str.begin(), str.end());
    sRaw+=str;
    sRaw+= " ";
    str = std::bitset<4>(m_Frame.ltc.user7).to_string();
    std::reverse(str.begin(), str.end());
    sRaw+=str;
    sRaw+= " ";
    str = std::bitset<2>(m_Frame.ltc.hours_tens).to_string();
    std::reverse(str.begin(), str.end());
    sRaw+=str;
    sRaw+= " ";
    str = std::bitset<1>(m_Frame.ltc.binary_group_flag_bit1).to_string();
    std::reverse(str.begin(), str.end());
    sRaw+=str;
    sRaw+= " ";
    str = std::bitset<1>(m_Frame.ltc.binary_group_flag_bit2).to_string();
    std::reverse(str.begin(), str.end());
    sRaw+=str;
    sRaw+= " ";
    str = std::bitset<4>(m_Frame.ltc.user8).to_string();
    std::reverse(str.begin(), str.end());
    sRaw+=str;
    sRaw+= " ";
    str = std::bitset<16>(m_Frame.ltc.sync_word).to_string();
    std::reverse(str.begin(), str.end());
    sRaw+=str;


    m_sRaw = wxString::FromAscii(sRaw.c_str());
}


void LtcDecoder::ltc_frame_to_time_bbc(SMPTETimecode* stime, LTCFrame* frame)
{
    if(stime)
    {
       stime->years = frame->user6 + frame->user8*10;
       stime->months = frame->user3;
       if((frame->user4&0x4)!=0)
       {
            stime->months += 10;
       }
        stime->days = frame->user2 + (frame->user4&0x3)*10;

        stime->hours = frame->hours_units + frame->hours_tens*10;
        stime->mins  = frame->mins_units  + frame->mins_tens*10;
        stime->secs  = frame->secs_units  + frame->secs_tens*10;
        stime->frame = frame->frame_units + frame->frame_tens*10;

        sprintf(stime->timezone,"+0000");
    }
}
