#include "delayline.h"
#include "offsetcalculator.h"
#include <algorithm>
#include "log.h"

DelayLine::DelayLine(unsigned int nWindowSamples, unsigned int nAccuracy) : m_nWindowSamples(nWindowSamples),
m_nAccuracy(nAccuracy),
m_bOneShot(false),
m_bCalculated(false)
{

}

DelayLine::~DelayLine()
{

}

void DelayLine::SetCalculationMode(bool bOneShot)
{
    m_bOneShot = bOneShot;
    m_bCalculated = false;
}

void DelayLine::CalculateChannelOffset()
{
    if(m_Buffer.first.size() >= m_nWindowSamples && m_Buffer.second.size() >= m_nWindowSamples)
    {
        // calculate delay
        auto vLeft = std::vector<float>(m_Buffer.first.begin(), std::next(m_Buffer.first.begin(),m_nWindowSamples));
        auto vRight = std::vector<float>(m_Buffer.second.begin(), std::next(m_Buffer.second.begin(), m_nWindowSamples));

        auto offset = CalculateOffset(vLeft, vRight);

        m_nOffset = offset;

        // remove samples from leading side so that buffer is aligned
        if(m_nOffset < -static_cast<int>(m_nAccuracy))
        {
            m_Buffer.second.erase(m_Buffer.second.begin(), std::next(m_Buffer.second.begin(), -m_nOffset));
        }
        else if(m_nOffset > static_cast<int>(m_nAccuracy))
        {
            m_Buffer.first.erase(m_Buffer.first.begin(), std::next(m_Buffer.first.begin(), m_nOffset));
        }
    }
}

int DelayLine::ProcessAudio(nonInterlacedVector& data)
{
    // add audio to our buffer
    std::copy(data.first.begin(), data.first.end(), std::back_inserter(m_Buffer.first));
    std::copy(data.second.begin(), data.second.end(), std::back_inserter(m_Buffer.second));

    if(!m_bOneShot || !m_bCalculated)
    {
        CalculateChannelOffset();
        m_bCalculated = true;
    }

    if(m_Buffer.first.size() >= data.first.size()+m_nWindowSamples && m_Buffer.second.size() >= data.second.size()+m_nWindowSamples)
    {
        data.first.assign(m_Buffer.first.begin(), std::next(m_Buffer.first.begin(), data.first.size()));
        data.second.assign(m_Buffer.second.begin(), std::next(m_Buffer.second.begin(), data.second.size()));

        m_Buffer.first.erase(m_Buffer.first.begin(), std::next(m_Buffer.first.begin(), data.first.size()));
        m_Buffer.second.erase(m_Buffer.second.begin(), std::next(m_Buffer.second.begin(), data.second.size()));
    }


    return m_nOffset;
}


void DelayLine::Reset()
{
    m_Buffer.first.clear();
    m_Buffer.second.clear();
}
