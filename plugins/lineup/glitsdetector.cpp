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
    if(IsGlits(m_vBuffer[0], m_vBuffer[1]))
    {
        m_eType = GD_GLITS_LR;
    }
    else if(IsGlits(m_vBuffer[1], m_vBuffer[0]))
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
    size_t nSignalTime(0), nSilenceTime(0), nSilenceCount(0), nSignalCount(0);
    vector<pairTransition_t> vTransitionL(CreateTransition(vLeft));
    CountTransitions(vTransitionL, nSilenceCount, nSilenceTime, nSignalCount, nSignalTime);

    wxLogDebug(wxT("Total %d Silence: %d"), nSilenceCount, nSilenceTime);
    wxLogDebug(wxT("Total %d Signal: %d"), nSignalCount, nSignalTime);

    if(nSilenceTime < 200 || nSilenceTime > 300 || nSilenceCount > 2)
    {
        return false;
    }

    if(nSignalTime < 3700 || nSignalTime > 3800 || nSignalCount > 2)
    {
        return false;
    }

    //check the right leg
    nSignalTime=nSilenceTime=nSilenceCount=nSignalCount = 0;
    vector<pairTransition_t> vTransitionR(CreateTransition(vRight));
    CountTransitions(vTransitionR, nSilenceCount, nSilenceTime, nSignalCount, nSignalTime);

    wxLogDebug(wxT("Total %d Silence: %d"), nSilenceCount, nSilenceTime);
    wxLogDebug(wxT("Total %d Signal: %d"), nSignalCount, nSignalTime);

    if(nSilenceTime < 450 || nSilenceTime > 550 || nSilenceCount > 3)
    {
        return false;
    }

    if(nSignalTime < 3450 || nSignalTime > 3550 || nSignalCount > 4)
    {
        return false;
    }

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
