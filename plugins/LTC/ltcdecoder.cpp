#include "ltcdecoder.h"
#include <wx/datetime.h>
#include "timedbuffer.h"
#include <bitset>
#include <wx/log.h>
#include <algorithm>

const wxString LtcDecoder::STR_MODE[4] = {wxT("Not specified"), wxT("8-bit"), wxT("Date"), wxT("Page/Line")};
const wxString LtcDecoder::STR_DATE_MODE[5] = {wxT("Unknown"), wxT("SMPTE"), wxT("BBC"), wxT("TVE"), wxT("MTD")};


LtcDecoder::LtcDecoder() :
    m_pDecoder(ltc_decoder_create(APV, 32)),
    m_nTotal(0),
    m_nFPS(0),
    m_nLastFrame(0),
    m_nDateMode(UNKNOWN)
{
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

            int nMode = WorkoutUserMode();
            DecodeDateAndTime(nMode);

            m_sFrameStart.Printf(wxT("%8lld"),m_Frame.off_end - m_Frame.off_start);
            m_sFrameEnd.Printf(wxT("%8lld"),m_Frame.off_end);

            m_sAmpltitude.Printf(wxT("%.2f dBFS"), m_Frame.volume);


            if(m_Frame.ltc.dfbit == 0)
            {
                m_sFPS.Printf(wxT("%u"), (m_nFPS+1));
            }
            else
            {
                double dFPS = m_nFPS+1;
                dFPS -= (1.0/dFPS);
                m_sFPS.Printf(wxT("%.2f"), dFPS);
            }

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

const wxString& LtcDecoder::GetFormat() const
{
    return m_sDateFormat;
}

void LtcDecoder::SetDateMode(int nMode)
{
    m_nDateMode = nMode%5;
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


    m_sRaw = wxString::FromUTF8(sRaw.c_str());
}

int LtcDecoder::WorkoutUserMode()
{
    int nbit0 = m_Frame.ltc.binary_group_flag_bit0;
    int nbit1 = m_Frame.ltc.binary_group_flag_bit1;
    int nbit2 = m_Frame.ltc.binary_group_flag_bit2;
    if(m_nFPS == 24)
    {
        nbit0 = m_Frame.ltc.biphase_mark_phase_correction;
        nbit2 = m_Frame.ltc.binary_group_flag_bit0;
    }

    int nMode = nbit0+(nbit2*2);


    m_sMode = STR_MODE[nMode%4];

    return nMode;
}

void LtcDecoder::DecodeDateAndTime(int nUserMode)
{
    SMPTETimecode stime;

    ltc_frame_to_time_only(stime);

    if(nUserMode == 0 || nUserMode == 2)
    {
        int nDateMode =  m_nDateMode;
        if(nDateMode == UNKNOWN)
        {
            while(nDateMode != MTD)
            {
                nDateMode++;
                if(DecodeDateAndTime(stime, nDateMode))
                    break;
            }
        }
        else
        {
            DecodeDateAndTime(stime, nDateMode);
        }
        m_sDate.Printf(wxT("%04d-%02d-%02d %s"), ((stime.years < 67) ? 2000+stime.years : 1900+stime.years),stime.months, stime.days, stime.timezone);

        m_sDateFormat = STR_DATE_MODE[nDateMode];
    }
    else
    {
        m_sDate = wxT("0000-00-00 +0");
    }

    m_sTime.Printf(wxT("%02d:%02d:%02d%c%02d"),stime.hours,stime.mins,stime.secs,(m_Frame.ltc.dfbit) ? '.' : ':',stime.frame);


}


bool LtcDecoder::DecodeDateAndTime(SMPTETimecode& stime, int nDateMode)
{
    switch(nDateMode)
    {
        case SMPTE:
            ltc_frame_to_time(&stime, &m_Frame.ltc, 1);
            break;
        case BBC:
            ltc_frame_to_time_bbc(stime);
            break;
        case TVE:
            ltc_frame_to_time_tve(stime);
            break;
        case MTD:
            ltc_frame_to_time_mtd(stime);
            break;

    }
    if(stime.days < 1 || stime.days > 31 || stime.months < 1 || stime.months > 12)
    {
        return false;
    }
    return true;
}

void LtcDecoder::ltc_frame_to_time_only(SMPTETimecode& stime)
{
    stime.hours = m_Frame.ltc.hours_units + m_Frame.ltc.hours_tens*10;
    stime.mins  = m_Frame.ltc.mins_units  + m_Frame.ltc.mins_tens*10;
    stime.secs  = m_Frame.ltc.secs_units  + m_Frame.ltc.secs_tens*10;
    stime.frame = m_Frame.ltc.frame_units + m_Frame.ltc.frame_tens*10;

    if(stime.frame == 0 && m_nLastFrame != 0)
    {
        m_nFPS = m_nLastFrame;
    }
    m_nLastFrame = stime.frame;
}


void LtcDecoder::ltc_frame_to_time_bbc(SMPTETimecode& stime)
{
   stime.years = m_Frame.ltc.user6 + m_Frame.ltc.user8*10;
   stime.months = m_Frame.ltc.user3;
   if((m_Frame.ltc.user4&0x4)!=0)
   {
        stime.months += 10;
   }
    stime.days = m_Frame.ltc.user2 + (m_Frame.ltc.user4&0x3)*10;

    sprintf(stime.timezone,"+0");
}

void LtcDecoder::ltc_frame_to_time_tve(SMPTETimecode& stime)
{
    stime.years  = m_Frame.ltc.user6 + m_Frame.ltc.user7*10;
    stime.months = m_Frame.ltc.user4 + m_Frame.ltc.user5*10;
    stime.days   = m_Frame.ltc.user2 + m_Frame.ltc.user3*10;

    sprintf(stime.timezone,"+0");
}


void LtcDecoder::ltc_frame_to_time_mtd(SMPTETimecode& stime)
{
    stime.years  = m_Frame.ltc.user2 + m_Frame.ltc.user1*10;
    stime.months = m_Frame.ltc.user4 + m_Frame.ltc.user3*10;
    stime.days   = m_Frame.ltc.user6 + m_Frame.ltc.user5*10;

    switch((m_Frame.ltc.user7 & 0x3))
    {
        case 0:
        case 3:
            sprintf(stime.timezone,"+0");
            break;
        case 1:
            sprintf(stime.timezone,"+1");
            break;
        case 2:
            sprintf(stime.timezone,"+2");
            break;
    }
}

const wxString& LtcDecoder::GetMode() const
{
    return m_sMode;
}

bool LtcDecoder::IsColourFlagSet() const
{
    return (m_Frame.ltc.col_frame!=0);
}

bool LtcDecoder::IsClockFlagSet() const
{
    return (m_Frame.ltc.binary_group_flag_bit1!=0);
}

