#include "r128calculator.h"
#include "session.h"
#include "timedbuffer.h"
#include <wx/log.h>
#include <math.h>
#include <wx/stopwatch.h>
#include <algorithm>
#include "r128thread.h"

using namespace std;

R128Calculator::R128Calculator() :
    m_dMomentary(-80),
    m_dShort(-80),
	m_dMomentaryMax(-80.0),
	m_dShortMax(-80.0),
    m_nInputChannels(0),
    m_nChunkFrames(0),
    m_nFrames(0),
    m_dTempMS(0.0),
    m_pThread(new R128Thread())
{
    m_pThread->Create();
    m_pThread->SetPriority(WXTHREAD_MIN_PRIORITY);
    m_pThread->Run();
}

R128Calculator::~R128Calculator()
{
    m_pThread->Stop();
    m_pThread->Wait();
    delete m_pThread;
}

void R128Calculator::InputSession(const session& aSession)
{
    if(aSession.GetCurrentSubsession() != aSession.lstSubsession.end())
    {
        m_subsession = (*aSession.GetCurrentSubsession());
        m_nInputChannels = min((unsigned int)8 ,m_subsession.nChannels);
        m_nChunkFrames = m_subsession.nSampleRate/10;

    }
    else
    {
        m_nInputChannels = 0;
    }
    ResetMeter();
}

void R128Calculator::CalculateLevel(const timedbuffer* pBuffer)
{
    //copy the frames into chunks
    if(m_nInputChannels != 0 && !m_vChannels.empty())
    {
        for(unsigned int i=0; i < pBuffer->GetBufferSize(); i+=m_nInputChannels)
        {
            for(unsigned int n = 0; n < m_vChannels.size(); n++)
            {
                if(m_nFrames == m_nChunkFrames)
                {
                    m_nFrames = 0;
                    m_lstMS.push_back(m_dTempMS/static_cast<double>(m_nChunkFrames));
                    m_dTempMS = 0.0;
                }

                m_dTempMS += (ApplyFilter(pBuffer->GetBuffer()[i+m_vChannels[n].first],m_vChannels[n].first))*m_vChannels[n].second;
            }
            ++m_nFrames;
        }
        CalculateMomentary();
    }
}

void R128Calculator::CalculateMomentary()
{
    while(m_lstMS.size() > 4)    //at least 400 ms
    {
        list<double>::iterator itChunk = m_lstMS.begin();
        double dMomentaryValue = 0.0;
        for(int i = 0; i < 4; i++)
        {
            dMomentaryValue += (*itChunk);
            ++itChunk;
        }
        dMomentaryValue /=4.0;
        if(dMomentaryValue > 0.0)
        {
            m_dMomentary = -0.691 + 10 * log10(dMomentaryValue);
        }
        else
        {
            dMomentaryValue = -80.0;
        }
        m_dMomentaryMax = max(m_dMomentary, m_dMomentaryMax);


        //store the no db value in our short list
        m_lstShort.push_back(dMomentaryValue);
        if(m_dMomentary > -70.0)
        {   //greater than silence so store the none db value in our live list
            m_pThread->AddToLive(dMomentaryValue);
        }
        //move the sliding frame along 100ms
        m_lstMS.pop_front();

        CalculateShort();
        CalculateLive();
        CalculateRange();
    }
}

void R128Calculator::CalculateShort()
{
    if(m_lstShort.size() > 29)  //got at leas 3 seconds
    {
        list<double>::iterator itShort = m_lstShort.begin();
        double dShortValue = 0.0;
        for(int i = 0; i < 30; i++)
        {
            dShortValue += (*itShort);
            ++itShort;
        }
        dShortValue/=30.0;

        if(dShortValue > 0.0)
        {
            m_dShort = -0.691 + 10 * log10(dShortValue);
        }
        else
        {
            m_dShort = -80.0;
        }
        m_dShortMax = max(m_dShort, m_dShortMax);

        if(m_dShort > -70)
        {
            m_pThread->AddToRange(dShortValue);
        }
        //move the sliding frame along 100ms
        m_lstShort.pop_front();
    }
}

void R128Calculator::CalculateLive()
{


}

void R128Calculator::CalculateRange()
{

}


double R128Calculator::GetMomentaryMax() const
{
    return m_dMomentaryMax;
}

double R128Calculator::GetShortMax() const
{
    return m_dShortMax;
}

double R128Calculator::GetMomentaryLevel() const
{
    return m_dMomentary;
}

double R128Calculator::GetShortLevel() const
{
    return m_dShort;
}

double R128Calculator::GetLiveLevel() const
{
    return m_pThread->GetLive();
}

double R128Calculator::GetLURange() const
{
    return m_pThread->GetRange();
}

void R128Calculator::ResetMeter()
{
    m_lstMS.clear();
    m_lstShort.clear();
    m_pThread->Reset();

    m_dMomentary = -80.0;
    m_dShort = -80.0;
    m_dMomentaryMax = -80.0;
    m_dShortMax = -80.0;
    m_dTempMS = 0.0;
    m_nFrames = 0;

    m_vPreFilter.clear();
    m_vFilter.clear();

    m_vPreFilter = vector<pair<double,double> >(m_nInputChannels, make_pair(0.0,0.0));
    m_vFilter = vector<pair<double,double> >(m_nInputChannels, make_pair(0.0,0.0));
}

double R128Calculator::ApplyFilter(double dSample, unsigned int nChannel)
{

     double dPw_n = dSample - (m_vPreFilter[nChannel].first * PF_A1 + m_vPreFilter[nChannel].second * PF_A2);
     double dH_n = dPw_n * PF_B0 + m_vPreFilter[nChannel].first * PF_B1 + m_vPreFilter[nChannel].second * PF_B2;
     // shuffle the samples along the delay ..
     m_vPreFilter[nChannel].second = m_vPreFilter[nChannel].first;
     m_vPreFilter[nChannel].first = dPw_n;

     /* end of HRTF weighting curve application */

     /* Apply RLB weighting curve */

     double dW_n = 0.1 + dH_n - (m_vFilter[nChannel].first * F_A1 + m_vFilter[nChannel].second * F_A2);
     double dOutput =  dW_n * F_B0 + m_vFilter[nChannel].first * F_B1 + m_vFilter[nChannel].second * F_B2;

    // shuffle the samples along the delay ..
     m_vFilter[nChannel].second = m_vFilter[nChannel].first;
     m_vFilter[nChannel].first = dW_n;

     return (dOutput * dOutput); /* and square it */

}


size_t R128Calculator::GetIntegrationTime()
{
    if(m_pThread)
    {
        return m_pThread->GetIntegrationTime();
    }
    return 0;
}

void R128Calculator::SetChannelGroup(unsigned char nGroup)
{
    m_vChannels.clear();
    for(size_t i = 0; i < m_subsession.vChannels.size(); i++)
    {
        //add all the channels in the group except the LFE one
        if(m_subsession.vChannels[i].nId == nGroup && m_subsession.vChannels[i].type != subsession::enumChannel::LFE)
        {
            if(m_subsession.vChannels[i].type == subsession::enumChannel::LEFT_SIDE || m_subsession.vChannels[i].type == subsession::enumChannel::RIGHT_SIDE ||
               m_subsession.vChannels[i].type == subsession::enumChannel::LEFT_REAR_SIDE || m_subsession.vChannels[i].type == subsession::enumChannel::RIGHT_REAR_SIDE)
            {
                m_vChannels.push_back(std::make_pair(i, 1.41));
            }
            else
            {
                m_vChannels.push_back(std::make_pair(i, 1.0));
            }
        }
    }
}
