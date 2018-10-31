#include "ltcdecoder.h"
#include <wx/datetime.h>
#include "timedbuffer.h"

LtcDecoder::LtcDecoder()
{
    m_pDecoder = ltc_decoder_create(APV, 32);
    m_nTotal = 0;
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
            ltc_frame_to_time(&stime, &m_Frame.ltc, 1);
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
