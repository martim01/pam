#include "truepeakcalculator.h"
#include "timedbuffer.h"
#include "session.h"
#include "filt.h"
#include "srcwrapper.h"
#include <wx/log.h>

using namespace std;

TruePeakCalculator::TruePeakCalculator() : m_pSrc(0)
{
    m_pSrc = new SrcWrapper();
}


TruePeakCalculator::~TruePeakCalculator()
{
    if(m_pSrc->pState)
    {
        delete m_pSrc->pState;
    }
    for(size_t i = 0; i < m_vFilter.size(); i++)
    {
        delete m_vFilter[i];
        m_vFilter.clear();
    }
}

void TruePeakCalculator::InputSession(const session& aSession)
{
    if(m_pSrc->pState)
    {
        src_delete(m_pSrc->pState);
        m_pSrc->pState = 0;
    }

    for(size_t i = 0; i < m_vFilter.size(); i++)
    {
        delete m_vFilter[i];
        m_vFilter.clear();
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

        m_vFilter.resize(aSession.itCurrentSubsession->nChannels);
        for(size_t i = 0; i < m_vFilter.size(); i++)
        {
            m_vFilter[i] = new Filter(LPF, 48, (aSession.itCurrentSubsession->nSampleRate*4)/1000, (aSession.itCurrentSubsession->nSampleRate/2)/1000);
            if(m_vFilter[i]->get_error_flag() != 0)
            {
                delete m_vFilter[i];
                m_vFilter[i] = 0;
            }
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
    if(m_pSrc->pState && m_vFilter.empty() == false)
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
            for(int i = 0; i < m_vCurrentPeak.size();i++)
            {
                m_vCurrentPeak[i] = -80.0;
            }
            for(int i = 0; i < m_pSrc->data.output_frames_gen*m_nChannels; i+= m_nChannels)
            {
                for(int j = 0; j < m_nChannels; j++)
                {
                    float dSample;
                    if(m_vFilter[j])
                    {
                        dSample = m_vFilter[j]->do_sample(m_pSrc->data.data_out[i+j]);
                    }
                    else
                    {
                        dSample = m_pSrc->data.data_out[i+j];
                    }

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
        delete[] m_pSrc->data.data_out;
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
