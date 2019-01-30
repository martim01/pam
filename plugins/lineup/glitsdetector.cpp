#include "glitsdetector.h"
#include <wx/log.h>

using namespace std;

GlitsDetector::GlitsDetector() : m_bLocked(false), m_nSamples(0), m_nSize(0), m_nSampleRate(48000), m_eType(GD_UNKNOWN)
{
    m_bLast[0] = false;
    m_bLast[1] = false;
}

GlitsDetector::~GlitsDetector()
{

}

void GlitsDetector::SetAudioData(const timedbuffer* pBuffer)
{
    if(!m_bLocked)
    {
        for(size_t i = 0; i < pBuffer->GetBufferSize(); i+= m_nChannels)
        {
            double dLeft = pBuffer->GetBuffer()[i];
            if(dLeft == 0)
            {
                dLeft = -80;
            }
            else
            {

            }
        }
    }



    if(m_nSamples != nSamples)
    {
        m_bLocked = false;
        m_eType = GD_UNKNOWN;
        m_nSamples = nSamples;
        m_nSize = (m_nSampleRate*4)/m_nSamples;
    }
    bool bLeft = (dLeft > -30.0);
    bool bRight = (dRight > -30.0);

    if(!m_bLocked)
    {
        if(m_bLast[0] == true && bLeft == false)
        {   //wait for first break in left
            m_bLocked = true;
            m_nCount = 0;
            m_vTransitions[0].clear();
            m_vTransitions[1].clear();
            m_vTransitions[0].reserve(2000);
            m_vTransitions[1].reserve(2000);
            wxLogDebug(wxT("LOCKED"));
        }
    }
    if(m_bLocked)
    {
        if(m_bLast[0] != bLeft)
        {
            m_vTransitions[0].push_back(make_pair(m_nCount, bLeft));
        }
        if(m_bLast[1] != bRight)
        {
            m_vTransitions[1].push_back(make_pair(m_nCount, bRight));
        }
        m_nCount++;

        if(m_nCount == m_nSize)
        {
            WorkoutSignal();

            m_bLocked = false;
        }
    }

    m_bLast[0] = bLeft;
    m_bLast[1] = bRight;
}


bool GlitsDetector::IsSignal(float fLevel)
{
    if(abs(fLevel) < )
}


GlitsDetector::enumType GlitsDetector::GetType()
{
    return m_eType;
}

void GlitsDetector::WorkoutSignal()
{
    if(IsGlits(m_vTransitions[0], m_vTransitions[1]))
    {
        m_eType = GD_GLITS_LR;
    }
    else if(IsGlits(m_vTransitions[1], m_vTransitions[0]))
    {
        m_eType = GD_GLITS_RL;
    }
    else
    {
        m_eType = GD_UNKNOWN;
    }
    wxLogDebug(wxT("SIGNAL = %d"), m_eType);
}

bool GlitsDetector::IsGlits(const std::vector<pairTransition_t>& vLeft, const std::vector<pairTransition_t>& vRight)
{
    if(vRight.size() == 4 && vLeft.size() == 2)
    {   //could be glits
        if(vLeft[0].second == false && vLeft[1].second == true && vRight[0].second == false && vRight[1].second == true && vRight[2].second == false && vRight[3].second == true && vRight[0].first != 0)
        {
            if(!CheckLength(vLeft[0].first, vLeft[1].first, 0.25, 1.0))
            {
                return false;
            }
            if(!CheckLength(vLeft[1].first, m_nSize, 3.75, 1.0))
            {
                return false;
            }
            if(!CheckLength(0, vRight[0].first, 0.5, 1.0))
            {
                return false;
            }
            if(!CheckLength(vRight[0].first, vRight[1].first, 0.25, 1.0))
            {
                return false;
            }
            if(!CheckLength(vRight[1].first, vRight[2].first, 0.25, 1.0))
            {
                return false;
            }
            if(!CheckLength(vRight[2].first, vRight[3].first, 0.25, 1.0))
            {
                return false;
            }
            if(!CheckLength(vRight[3].first, m_nSize, 2.75, 1))
            {
                return false;
            }
            return true;
        }
    }
    return false;
}

bool GlitsDetector::CheckLength(unsigned long nStart, unsigned long nEnd, double dTarget, double dBand)
{
    double dLength = nEnd - nStart;
    dTarget = dTarget*(static_cast<double>(m_nSize)/4.0);

    wxLogDebug(wxT("LENGTH: %.2f TARGET: %.2f"), dLength, dTarget);
    return (dLength >= dTarget-dBand && dLength <= dTarget+dBand);

}
