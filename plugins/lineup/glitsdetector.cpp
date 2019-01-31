#include "glitsdetector.h"
#include <wx/log.h>
#include "timedbuffer.h"
#include "session.h"
#include <cmath>
using namespace std;

GlitsDetector::GlitsDetector() : m_bLocked(false), m_nSize(0), m_nSampleRate(48000), m_eType(GD_UNKNOWN)
{
    m_nChannels = 2;
    m_nSampleCount = 0;
    m_nSineCount = 0;
    m_vBuffer[0].resize(4000);
    m_vBuffer[1].resize(4000);
    m_dLastLeft = 0.0;
    m_dLastRight = 0.0;
    m_bLocked = false;
}

GlitsDetector::~GlitsDetector()
{

}

void GlitsDetector::SetAudioData(const timedbuffer* pBuffer)
{
    float dLeft(0), dRight(0);

    for(size_t i = 0; i < pBuffer->GetBufferSize(); i+=m_nChannels)
    {
        if(!m_bLocked)
        {
            if(m_dLastLeft < 0.0 && pBuffer->GetBuffer()[i] > 0.0)
            {
                wxLogDebug(wxT("LOCKED"));
                m_bLocked = true;
                m_nSampleCount = 0;
                m_nSineCount = 0;
            }
        }
        if(m_bLocked)
        {
            //count 48 samples - if max
            dLeft = max(dLeft, abs(pBuffer->GetBuffer()[i]));
            dRight = max(dRight, abs(pBuffer->GetBuffer()[i+1]));
            m_nSampleCount++;
            if(m_nSampleCount == 48)
            {
                m_vBuffer[0][m_nSineCount] = (dLeft > 0.003);
                m_vBuffer[1][m_nSineCount] = (dRight > 0.003);

                m_nSineCount++;
                if(m_nSineCount == m_vBuffer[0].size())
                {
                    WorkoutSignal();
                    m_nSineCount = 0;
                }
                m_nSampleCount = 0;
                dLeft = 0.0;
                dRight = 0.0;
            }
        }
        m_dLastLeft = pBuffer->GetBuffer()[i];
        m_dLastRight = pBuffer->GetBuffer()[i+1];
    }
}



GlitsDetector::enumType GlitsDetector::GetType()
{
    return m_eType;
}

void GlitsDetector::WorkoutSignal()
{
    size_t nSignalTimeL(0), nSilenceTimeL(0), nSilenceCountL(0), nSignalCountL(0);
    vector<pairTransition_t> vTransitionL(CreateTransition(m_vBuffer[0]));
    CountTransitions(vTransitionL, nSilenceCountL, nSilenceTimeL, nSignalCountL, nSignalTimeL);

    size_t nSignalTimeR(0), nSilenceTimeR(0), nSilenceCountR(0), nSignalCountR(0);
    vector<pairTransition_t> vTransitionR(CreateTransition(m_vBuffer[1]));
    CountTransitions(vTransitionR, nSilenceCountR, nSilenceTimeR, nSignalCountR, nSignalTimeR);

    //check if glits
    if(nSilenceCountL == 0 && nSilenceCountR == 0)
    {
        m_eType = GD_MONO;
    }
    else if(nSilenceCountR == 0 && nSilenceCountL < 3 && nSilenceTimeL > 200 && nSilenceTimeL < 550)
    {
        m_eType = GD_EBU_LR;
    }
    else if(nSilenceCountR == 0 && nSilenceCountL < 3 && nSilenceTimeR > 200 && nSilenceTimeR < 550)
    {
        m_eType = GD_EBU_RL;
    }
    else if(nSilenceTimeL > 200 && nSilenceTimeL < 300 && nSilenceCountL <= 2 && nSignalTimeL > 3700 && nSignalTimeL < 3800 && nSignalCountL <= 2 &&
       nSilenceTimeR > 450 && nSilenceTimeR < 550 && nSilenceCountR <= 3 && nSignalTimeR > 3450 && nSignalTimeR < 3550 && nSignalCountR <= 4)
    {
        m_eType = GD_GLITS_LR;
    }
    else if(nSilenceTimeR > 200 && nSilenceTimeR < 300 && nSilenceCountR <= 2 && nSignalTimeR > 3700 && nSignalTimeR < 3800 && nSignalCountR <= 2 &&
       nSilenceTimeL > 450 && nSilenceTimeL < 550 && nSilenceCountL <= 3 && nSignalTimeL > 3450 && nSignalTimeL < 3550 && nSignalCountL <= 4)
    {
        m_eType = GD_GLITS_RL;
    }
    else
    {
        m_eType = GD_UNKNOWN;
    }
    wxLogDebug(wxT("SIGNAL = %d"), m_eType);
}

std::vector<pairTransition_t> GlitsDetector::CreateTransition(const std::vector<bool>& vLevel)
{
    vector<pairTransition_t> vTransition;
    bool bLast = vLevel[0];
    size_t nCount(0);
    for(size_t i = 1; i < vLevel.size(); i++)
    {
        if(vLevel[i] == bLast)
        {
            nCount++;
        }
        else
        {   //transition
            if(nCount != 0)
            {
                if(vTransition.empty() || vTransition.back().second != bLast)
                {
                    vTransition.push_back(make_pair(nCount, bLast));
                }
                else
                {
                    vTransition.back().first+=nCount;
                }
            }
            nCount = 0;
        }
        bLast = vLevel[i];
    }

    if(nCount != 0)
    {
        if(vTransition.empty() || vTransition.back().second != bLast)
        {
            vTransition.push_back(make_pair(nCount, bLast));
        }
        else
        {
            vTransition.back().first+=nCount;
        }
    }
    return vTransition;

}

void GlitsDetector::CountTransitions(const std::vector<pairTransition_t>& vTransition, size_t& nSilenceCount, size_t& nSilenceTime, size_t& nSignalCount, size_t& nSignalTime)
{
    for(size_t i = 0; i < vTransition.size(); i++)
    {
        if(vTransition[i].second == false)
        {
            nSilenceCount++;
            nSilenceTime += vTransition[i].first;
            wxLogDebug(wxT("%d = Silence: %d"), i, vTransition[i].first);
        }
        else
        {
            nSignalCount++;
            nSignalTime += vTransition[i].first;
            wxLogDebug(wxT("%d = Signal: %d"), i, vTransition[i].first);
        }
    }
}

bool GlitsDetector::IsGlits(const std::vector<bool>& vLeft, const std::vector<bool>& vRight)
{



    return true;
}

bool GlitsDetector::CheckLength(unsigned long nStart, unsigned long nEnd, double dTarget, double dBand)
{
    double dLength = nEnd - nStart;
    dTarget = dTarget*m_nSampleRate;
    dBand *= m_nSampleRate;

    wxLogDebug(wxT("LENGTH: %.2f TARGET: %.2f"), dLength, dTarget);
    return (dLength >= dTarget-dBand && dLength <= dTarget+dBand);

}


void GlitsDetector::InputSession(const session& aSession)
{
    if(aSession.GetCurrentSubsession() != aSession.lstSubsession.end())
    {
        m_nSampleRate = aSession.GetCurrentSubsession()->nSampleRate;
        m_nChannels = std::min((unsigned int)256 ,aSession.GetCurrentSubsession()->nChannels);
    }
    else
    {
        m_nSampleRate = 48000;
        m_nChannels = 2;
    }
}


void GlitsDetector::Unlock()
{
    m_bLocked = false;
    m_dLastLeft = 0.0;
    m_dLastRight = 0.0;
    m_nSampleCount = 0;
    m_nSineCount = 0;
}
