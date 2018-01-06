#include "levelcalculator.h"
#include "timedbuffer.h"
#include "session.h"
#include "ppm.h"
#include "loud.h"

using namespace std;

 LevelCalculator::LevelCalculator(float dMin) :
  m_nChannels(2),
  m_nMode(PPM),
  m_bMS(false),
  m_nMSMode(meter::M6),
  m_nSpeed(meter::NORMAL),
  m_pPpm(0),
  m_pLoud(0),
  m_pPpmMS(0),
  m_pLoudMS(0),
  m_dMin(dMin)
 {

 }

LevelCalculator::~LevelCalculator()
{
    DeletePpmLoud();
}


void LevelCalculator::DeletePpmLoud()
{
    if(m_pPpm)
    {
        delete m_pPpm;
    }
    m_pPpm = 0;
    if(m_pLoud)
    {
        delete m_pLoud;
    }
    m_pLoud = 0;

    if(m_pPpmMS)
    {
        delete m_pPpmMS;
    }
    m_pPpmMS = 0;
    if(m_pLoudMS)
    {
        delete m_pLoudMS;
    }
    m_pLoudMS = 0;
}
void LevelCalculator::InputSession(const session& aSession)
{
    if(aSession.itCurrentSubsession != aSession.lstSubsession.end())
    {
        m_nChannels = aSession.itCurrentSubsession->nChannels;
    }
    else
    {
        m_nChannels = 0;
    }
    SetMode(m_nMode);
}

void LevelCalculator::SetMode(unsigned int nMode)
{
    DeletePpmLoud();

    m_nMode = nMode;
    switch(nMode)
    {
        case PPM:
            CreatePpm();
            break;
        case LOUD:
            CreateLoud();
            break;
    }
}


void LevelCalculator::CreatePpm()
{
    if(m_nChannels != 0)
    {
        m_pPpm = new ppm(m_nChannels);
        m_pPpm->setMode(meter::AB);
        m_pPpm->setSpeed(m_nSpeed);

        m_pPpmMS = new ppm(m_nChannels);
        m_pPpmMS->setMode(m_nMSMode);
        m_pPpmMS->setSpeed(m_nSpeed);
    }
}

void LevelCalculator::CreateLoud()
{
    if(m_nChannels != 0)
    {
        m_pLoud = new loud(m_nChannels);
        m_pLoud->setIntegrationTime(8);
        m_pLoud->setMode(meter::AB);
        m_pLoud->setSpeed(m_nSpeed);

        m_pLoudMS = new loud(m_nChannels);
        m_pLoudMS->setIntegrationTime(8);
        m_pLoudMS->setMode(m_nMSMode);
        m_pLoudMS->setSpeed(m_nSpeed);
    }
}


void LevelCalculator::SetMSMode(long nMode)
{
    m_nMSMode = nMode;
    if(m_pPpmMS)
    {
        m_pPpmMS->setMode(nMode);
    }
    else if(m_pLoudMS)
    {
        m_pLoudMS->setMode(nMode);
    }
}

void LevelCalculator::SetSpeed(long nSpeed)
{
    m_nSpeed = nSpeed;
    if(m_pPpm)
    {
        m_pPpm->setSpeed(nSpeed);
    }
    if(m_pLoud)
    {
        m_pLoud->setSpeed(nSpeed);
    }

    if(m_pPpmMS)
    {
        m_pPpmMS->setSpeed(nSpeed);
    }
    if(m_pLoudMS)
    {
        m_pLoudMS->setSpeed(nSpeed);
    }
}

void LevelCalculator::CalculateLevel(const timedbuffer* pBuffer)
{
    switch(m_nMode)
    {
        case PPM:
             CalculatePpm(pBuffer);
             break;
        case LOUD:
            CalculateLoud(pBuffer);
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
    if(m_pPpm)
    {
        m_pPpm->calcIntermediate(m_nChannels, pBuffer->GetBufferSize()/m_nChannels, pBuffer->GetBuffer());
    }
    if(m_nChannels == 2)
    {
        m_pPpmMS->calcIntermediate(m_nChannels, pBuffer->GetBufferSize()/m_nChannels, pBuffer->GetBuffer());
    }
}

void LevelCalculator::CalculateLoud(const timedbuffer* pBuffer)
{
    if(m_pLoud)
    {
        m_pLoud->calcIntermediate(m_nChannels, pBuffer->GetBufferSize()/m_nChannels, pBuffer->GetBuffer());
    }

    if(m_nChannels == 2)
    {
        m_pLoudMS->calcIntermediate(m_nChannels, pBuffer->GetBufferSize()/m_nChannels, pBuffer->GetBuffer());
    }
}

void LevelCalculator::ResetLevels(float dLevel)
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
            float dSample(fabs(pBuffer->GetBuffer()[i+j]));
            m_dLevel[j] = max(m_dLevel[j],dSample);
        }
    }
    if(m_nChannels == 2)
    {
        for(unsigned int i=0; i < pBuffer->GetBufferSize(); i+=2)
        {
            float dSampleM(fabs(pBuffer->GetBuffer()[i]+pBuffer->GetBuffer()[i+1]));
            float dSampleS(fabs(pBuffer->GetBuffer()[i]-pBuffer->GetBuffer()[i+1]));
            if(m_nMSMode == meter::M6)
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
}

void LevelCalculator::CalculateEnergy(const timedbuffer* pBuffer)
{
    ResetLevels(0.0);

    for(unsigned int i=0; i < pBuffer->GetBufferSize(); i++)
    {
        m_dLevel[i%m_nChannels] += pow(pBuffer->GetBuffer()[i],2);
    }
    if(m_nChannels == 2)
    {
        for(unsigned int i=0; i < pBuffer->GetBufferSize(); i++)
        {
            if(m_nMSMode == meter::M6)
            {
                m_dMS[0] += pow((pBuffer->GetBuffer()[i]+pBuffer->GetBuffer()[i+1])/2,2);
                m_dMS[1] += pow((pBuffer->GetBuffer()[i]-pBuffer->GetBuffer()[i+1])/2,2);
            }
            else
            {
                m_dMS[0] += pow((pBuffer->GetBuffer()[i]+pBuffer->GetBuffer()[i+1]),2);
                m_dMS[1] += pow((pBuffer->GetBuffer()[i]-pBuffer->GetBuffer()[i+1]),2);
            }
        }

        m_dMS[0] = sqrt(m_dMS[0]/(pBuffer->GetBufferSize()/2));
        m_dMS[1] = sqrt(m_dMS[1]/(pBuffer->GetBufferSize()/2));
    }

    for(int i = 0; i < 8; i++)
    {
        m_dLevel[i] = sqrt(m_dLevel[i]/(pBuffer->GetBufferSize()/m_nChannels));
    }

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
            if(m_nMSMode == meter::M6)
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
}


double LevelCalculator::GetLevel(unsigned int nChannel)
{
    if(nChannel < 8 && m_nChannels > 0)
    {
        switch(m_nMode)
        {
        case PPM:
            return m_pPpm->getValue(nChannel);
        case LOUD:
            return m_pLoud->getValue(nChannel);
        default:
            return m_dLevel[nChannel];
        }
    }
    return m_dMin;
}


double LevelCalculator::GetMSLevel(bool bStereo)
{
    switch(m_nMode)
    {
        case PPM:
            return m_pPpmMS->getValue(bStereo);
        case LOUD:
            return m_pLoudMS->getValue(bStereo);
    }
    return m_dMS[bStereo];
}
