#include "truepeakcalculator.h"
#include "timedbuffer.h"
#include "session.h"
#include "filt.h"
#include "srcwrapper.h"

using namespace std;

TruePeakCalculator::TruePeakCalculator() : m_pSrc(0), m_pFilter(0)
{
    m_pSrc = new SrcWrapper();
}


TruePeakCalculator::~TruePeakCalculator()
{
    if(m_pSrc->pState)
    {
        delete m_pSrc->pState;
    }
    if(m_pFilter)
    {
        delete m_pFilter;
    }
}

void TruePeakCalculator::InputSession(const session& aSession)
{
    if(m_pSrc->pState)
    {
        src_delete(m_pSrc->pState);
        m_pSrc->pState = 0;
    }

    if(m_pFilter)
    {
        delete m_pFilter;
        m_pFilter = 0;
    }
    m_vTruePeak.clear();
    m_vCurrentPeak.clear();

    if(aSession.itCurrentSubsession != aSession.lstSubsession.end())
    {
        m_nChannels = aSession.itCurrentSubsession->nChannels;
        int nError;
        m_pSrc->pState = src_new (SRC_SINC_FASTEST, aSession.itCurrentSubsession->nChannels, &nError);
        if(m_pSrc->pState)
        {
            src_set_ratio(m_pSrc->pState, 4.0);
        }

        m_pFilter = new Filter(LPF, 48, aSession.itCurrentSubsession->nSampleRate*4, aSession.itCurrentSubsession->nSampleRate/2);
        if(m_pFilter->get_error_flag() != 0)
        {
            delete m_pFilter;
            m_pFilter = 0;
        }
        m_vCurrentPeak.resize(m_nChannels);
        m_vTruePeak.resize(m_nChannels);
    }
    else
    {
        m_nChannels = 0;
    }

}

void TruePeakCalculator::CalculateLevel(const timedbuffer* pBuffer)
{
    if(m_pSrc->pState && m_pFilter)
    {
        m_pSrc->data.data_out = new float[pBuffer->GetBufferSize()*4];
        m_pSrc->data.data_in = pBuffer->GetBuffer();

        m_pSrc->data.input_frames	= pBuffer->GetBufferSize() / m_nChannels ;
		m_pSrc->data.output_frames	= m_pSrc->data.input_frames*4;
        m_pSrc->data.src_ratio = 4.0;
        m_pSrc->data.end_of_input = 0;

        if(src_process(m_pSrc->pState, &m_pSrc->data) != 0)
        { //error

        }
        else
        {//Now filter
            for(int i = 0; i < m_pSrc->data.output_frames_gen*m_nChannels; i+= m_nChannels)
            {
                for(int j = 0; j < m_nChannels; j++)
                {
                    float dSample = m_pFilter->do_sample(m_pSrc->data.data_out[i+j]);
                    if(dSample < 0)
                    {
                        dSample = -dSample;
                    }
                    m_vCurrentPeak[j] = max(dSample, m_vCurrentPeak[j]);
                }
            }
            for(int j = 0; j < m_nChannels; j++)
            {
                m_vTruePeak[j] = 20 * log10(m_vCurrentPeak[j]);
            }
        }
    }
}

double TruePeakCalculator::GetLevel(unsigned int nChannel)
{
    if(nChannel < m_vTruePeak.size())
    {
        return m_vTruePeak[nChannel];
    }
    return 0.0;
}
