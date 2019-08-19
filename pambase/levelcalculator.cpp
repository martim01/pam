#include <wx/log.h>
#include "levelcalculator.h"
#include "timedbuffer.h"
#include "session.h"
#include <cmath>


using namespace std;


 LevelCalculator::LevelCalculator(double dMin) :
  m_nChannels(2),
  m_nSampleRate(48000),
  m_nMode(PPM),
  m_bMS(false),
  m_nMSMode(M6),
  m_dSpeed(1.0),
  m_dMin(-138.0),
  m_dRiseFactor(0.0),
  m_dRiseTime(0.0)
   {
     for(int i = 0; i < 8; i++)
    {
        m_dLastLevel[i] = -80.0;
		m_dLevel[i] = -80.0;
		m_dInterim[i] = -80.0;
    }
    m_dLastMS[0] = -80.0;
    m_dLastMS[1] = -80.0;
	m_dMS[0] = -80.0;
	m_dMS[1] = -80.0;
	m_dInterimMS[0] = -80.0;
	m_dInterimMS[1] = -80.0;

    SetDynamicResponse(1,78, 1200, 20);

 }

LevelCalculator::~LevelCalculator()
{
}

void LevelCalculator::InputSession(const session& aSession)
{
    if(aSession.GetCurrentSubsession() != aSession.lstSubsession.end())
    {
        m_nChannels = min((unsigned int)8 ,aSession.GetCurrentSubsession()->nChannels);
        m_nSampleRate = aSession.GetCurrentSubsession()->nSampleRate;
        double dBits;
        if(aSession.GetCurrentSubsession()->sCodec == wxT("L16"))
        {
            dBits = 16;
        }
        else if(aSession.GetCurrentSubsession()->sCodec == wxT("L24"))
        {
            dBits = 24;
        }
        else
        {
            dBits = 32;
        }
        m_dMin = 20 * log10(1/(pow(2.0,dBits)));

    }
    else
    {
        m_nChannels = 0;
    }
    SetMode(m_nMode);
}

void LevelCalculator::SetMode(unsigned int nMode)
{
    m_nMode = nMode;
}




void LevelCalculator::SetMSMode(long nMode)
{
    m_nMSMode = nMode;
}

void LevelCalculator::SetSpeed(long nSpeed)
{
    switch(nSpeed)
    {
        case SLOW:
            m_dSpeed = 0.5;
            break;
        case NORMAL:
            m_dSpeed = 1.0;
            break;
        case FAST:
            m_dSpeed = 2.0;
            break;
    }
    CalculateDynamicRepsonse();
}


void LevelCalculator::SetDynamicResponse(double dRiseTime, double dRisedB, double dFallTime, double dFalldB)
{
    m_dRiseMs = dRiseTime;
    m_dRisedB = dRisedB;
    m_dFallMs = dFallTime;
    m_dFalldB = dFalldB;

    m_dDropFactor = pow(10.0, -dFalldB/20.0);

    CalculateDynamicRepsonse();
}

void LevelCalculator::CalculateDynamicRepsonse()
{
    double dInterval = (1.0/static_cast<double>(m_nSampleRate))*1000.0;
    double dFallInterval = dInterval/m_dFallMs;
    double dRiseInterval(0.0);
    if(m_dRiseMs != 0.0)
    {
        dRiseInterval=dInterval/m_dRiseMs;
    }

    m_dFallSample = m_dFalldB*dFallInterval*m_dSpeed;
    m_dRiseSample = 0.0;
    if(dRiseInterval > 0.0)
    {
        m_dRiseSample = m_dRisedB*dRiseInterval*m_dSpeed;
    }

    m_dDropRatio = 1 - (m_dSpeed - m_dSpeed*pow(m_dDropFactor, dFallInterval));

    m_dRiseRatio = 1.0 - pow(0.4, 1.0/(static_cast<double>(m_nSampleRate)*0.0025));

    m_dRiseRatio = m_dRiseRatio*(10.0/m_dRiseMs)*m_dSpeed;

}

void LevelCalculator::CalculateLevel(const timedbuffer* pBuffer)
{
    switch(m_nMode)
    {
        case PPM:
             CalculatePpm(pBuffer);
             break;
        case PEAK:
            CalculatePeak(pBuffer);
            break;
        case ENERGY:
            CalculateEnergy(pBuffer);
            break;
        case TOTAL:
            CalculateTotal(pBuffer);
            break;
        case AVERAGE:
            CalculateAverage(pBuffer);
            break;
    }


}


void LevelCalculator::CalculatePpm(const timedbuffer* pBuffer)
{
    ResetLevels(-80.0);

    for(unsigned int i=0; i < pBuffer->GetBufferSize(); i+=m_nChannels)
    {
        for(unsigned int j = 0; j < m_nChannels; j++)
        {
            m_dInterim[j] = fabs(pBuffer->GetBuffer()[i+j]);
            if(m_dInterim[j] > m_dLastLevel[j])
            {
                m_dInterim[j] = m_dLastLevel[j] + m_dRiseRatio*(m_dInterim[j]-m_dLastLevel[j]);
            }
            else if(m_dInterim[j] < m_dLastLevel[j])
            {
                m_dInterim[j] = max(m_dInterim[j], m_dLastLevel[j]*m_dDropRatio);
            }
            m_dLastLevel[j] = m_dInterim[j];

            m_dLevel[j] = max(m_dLevel[j], m_dInterim[j]);
        }

    }

    for(unsigned int j = 0; j < m_nChannels; j++)
    {
        ConvertToDb(m_dLevel[j]);
    }

    if(m_nChannels == 2)
    {
        for(unsigned int i=0; i < pBuffer->GetBufferSize(); i+=2)
        {
            m_dInterimMS[0] = (fabs(pBuffer->GetBuffer()[i]+pBuffer->GetBuffer()[i+1]));
            m_dInterimMS[1] = (fabs(pBuffer->GetBuffer()[i]-pBuffer->GetBuffer()[i+1]));
            if(m_nMSMode == M6)
            {
                m_dInterimMS[0]*=0.5;
                m_dInterimMS[1]*=0.5;
            }
            else
            {
                m_dInterimMS[0]*=0.707;
                m_dInterimMS[1]*=0.707;
            }


            for(int j = 0; j < 2; j++)
            {
                if(m_dInterimMS[j] > m_dLastMS[j])
                {
                    m_dInterimMS[j] = m_dLastMS[j] + m_dRiseRatio*(m_dInterimMS[j]-m_dLastMS[j]);

                }
                else if(m_dInterimMS[j] < m_dLastMS[j])
                {
                    m_dInterimMS[j] = max(m_dInterimMS[j], m_dLastMS[j]*m_dDropRatio);
                }
                m_dLastMS[j] = m_dInterimMS[j];

                m_dMS[j] = max(m_dMS[j], m_dInterimMS[j]);
            }
        }
        ConvertToDb(m_dMS[0]);
        ConvertToDb(m_dMS[1]);
    }
}


void LevelCalculator::ResetLevels(double dLevel)
{
    for(int i = 0; i < 8; i++)
    {
        m_dLevel[i] = dLevel;
    }
    m_dMS[0] = dLevel;
    m_dMS[1] = dLevel;


}

void LevelCalculator::CalculatePeak(const timedbuffer* pBuffer)
{
    //find largest value
    ResetLevels(0.0);

    for(unsigned int i=0; i < pBuffer->GetBufferSize(); i+=m_nChannels)
    {
        for(unsigned int j = 0; j < m_nChannels; j++)
        {
            double dSample(fabs(pBuffer->GetBuffer()[i+j]));
            m_dLevel[j] = max(m_dLevel[j],dSample);
        }
    }

    if(m_nChannels == 2)
    {
        for(unsigned int i=0; i < pBuffer->GetBufferSize(); i+=2)
        {
            double dSampleM(fabs(pBuffer->GetBuffer()[i]+pBuffer->GetBuffer()[i+1]));
            double dSampleS(fabs(pBuffer->GetBuffer()[i]-pBuffer->GetBuffer()[i+1]));
            if(m_nMSMode == M6)
            {
                dSampleM*=0.5;
                dSampleS*=0.5;
            }
            else
            {
                dSampleM*=0.707;
                dSampleS*=0.707;
            }
            m_dMS[0] = max(m_dMS[0],dSampleM);
            m_dMS[1] = max(m_dMS[1],dSampleS);
        }
    }

    CalculateRiseFall(pBuffer->GetBufferSize()/m_nChannels);
}

void LevelCalculator::CalculateEnergy(const timedbuffer* pBuffer)
{
    ResetLevels(0.0);

    for(unsigned int i=0; i < pBuffer->GetBufferSize(); i++)
    {
        m_dLevel[i%m_nChannels] += pow(pBuffer->GetBuffer()[i],2);
    }
    for(int i = 0; i < 8; i++)
    {
        m_dLevel[i] = sqrt(m_dLevel[i]/(pBuffer->GetBufferSize()/m_nChannels));
    }

    if(m_nChannels == 2)
    {
        if(m_nMSMode == M6)
        {
            m_dMS[0] = (m_dLevel[0]+m_dLevel[1])/2;
            m_dMS[1] = (m_dLevel[0]-m_dLevel[1])/2;
        }
        else
        {
            m_dMS[0] = (m_dLevel[0]+m_dLevel[1]);
            m_dMS[1] = (m_dLevel[0]-m_dLevel[1]);
        }
    }
    CalculateRiseFall(pBuffer->GetBufferSize()/m_nChannels);
}

void LevelCalculator::CalculateTotal(const timedbuffer* pBuffer)
{
    ResetLevels(0.0);

    for(unsigned int i=0; i < pBuffer->GetBufferSize(); i++)
    {
        m_dLevel[i%m_nChannels] += fabs(pBuffer->GetBuffer()[i]);
    }
    if(m_nChannels == 2)
    {
        for(unsigned int i=0; i < pBuffer->GetBufferSize(); i++)
        {
            if(m_nMSMode == M6)
            {
                m_dMS[0] += fabs((pBuffer->GetBuffer()[i]+pBuffer->GetBuffer()[i+1])/2);
                m_dMS[1] += fabs((pBuffer->GetBuffer()[i]-pBuffer->GetBuffer()[i+1])/2);
            }
            else
            {
                m_dMS[0] += fabs((pBuffer->GetBuffer()[i]+pBuffer->GetBuffer()[i+1]));
                m_dMS[1] += fabs((pBuffer->GetBuffer()[i]-pBuffer->GetBuffer()[i+1]));
            }
        }
    }
    CalculateRiseFall(pBuffer->GetBufferSize()/m_nChannels);
}

void LevelCalculator::CalculateAverage(const timedbuffer* pBuffer)
{
    CalculateTotal(pBuffer);
    for(unsigned int i = 0; i < 8; i++)
    {
        m_dLevel[i] /= (pBuffer->GetBufferSize()/m_nChannels);
    }
    m_dMS[0] /= (pBuffer->GetBufferSize()/m_nChannels);
    m_dMS[1] /= (pBuffer->GetBufferSize()/m_nChannels);

    CalculateRiseFall(pBuffer->GetBufferSize()/m_nChannels);
}


double LevelCalculator::GetLevel(unsigned int nChannel)
{
    if(nChannel < 8 && m_nChannels > 0)
    {
        return m_dLevel[nChannel];
    }
    return m_dMin;
}


double LevelCalculator::GetMSLevel(bool bStereo)
{
    return m_dMS[bStereo];
}


void LevelCalculator::CalculateRiseFall(unsigned long nSamples)
{
    double dFalldB(m_dFallSample*static_cast<double>(nSamples));
    double dRisedB(m_dRiseSample*static_cast<double>(nSamples));


    for(int i = 0; i < 8; i++)
    {
        CalculateRiseFall(m_dLevel[i], m_dLastLevel[i],dFalldB,dRisedB,(i==0));
    }

    CalculateRiseFall(m_dMS[0], m_dLastMS[0],dFalldB,dRisedB);
    CalculateRiseFall(m_dMS[1], m_dLastMS[1],dFalldB,dRisedB);

}

void LevelCalculator::CalculateRiseFall(double& dCurrent, double& dLast, double dFalldB, double dRisedB, bool bDebug)
{
    ConvertToDb(dCurrent);
    if(dLast > dCurrent)
    {
        dCurrent = max(dCurrent, dLast-dFalldB);
    }
    else if(dLast < dCurrent)
    {
        if(dRisedB > 0.0)
        {
            dCurrent = min(dCurrent, dLast+dRisedB);
        }
    }

    dLast = dCurrent;
}


void LevelCalculator::ConvertToDb(double& dSample)
{
    if(dSample > 0.0)
    {
        dSample =  max(m_dMin, 20*log10(dSample));
    }
    else
    {
        dSample =  m_dMin;
    }
}
