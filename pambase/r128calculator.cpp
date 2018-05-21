#include "r128calculator.h"
#include "session.h"
#include "timedbuffer.h"
#include <wx/log.h>
#include <math.h>

using namespace std;

R128Calculator::R128Calculator() :
    m_dMomentary(0),
    m_dShort(0),
    m_dLiveGate(0),
    m_dLiveAbs(0),
    m_dRange(0),
    m_nInputChannels(0),
    m_nChunkFrames(0)
{

}

R128Calculator::~R128Calculator()
{

}

void R128Calculator::InputSession(const session& aSession)
{
    if(aSession.itCurrentSubsession != aSession.lstSubsession.end())
    {
        m_nInputChannels = aSession.itCurrentSubsession->nChannels;
        m_nChunkFrames = aSession.itCurrentSubsession->nSampleRate/10;
        m_nChunkSize = m_nInputChannels*m_nChunkFrames;

    }
    else
    {
        m_nInputChannels = 0;
        m_nChunkSize = 0;
    }
    ResetMeter();
}

void R128Calculator::CalculateLevel(const timedbuffer* pBuffer)
{
    //copy the frames into chunks

    wxLogMessage(wxT("ChunkSize  is %d"), m_nChunkSize);
    if(m_nChunkSize != 0)
    {

        for(unsigned int i=0; i < pBuffer->GetBufferSize(); i+=m_nInputChannels)
        {
            for(unsigned int j = 0; j < m_nInputChannels; j++)
            {

                if(m_lstChunk.size() == m_nChunkSize)
                {
                    wxLogMessage(wxT("ChunkSize %d"), m_nChunkSize);
                    //got enough chunks now so work out the MS value
                   // WorkoutMS();
                    m_lstChunk.clear();
                    wxLogMessage(wxT("ChunkSize Cleared"));

                }


                m_lstChunk.push_back(ApplyFilter(pBuffer->GetBuffer()[i+j],j));
            }
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
        m_dMomentary = -0.691 + 10 * log10(dMomentaryValue);

        //store the no db value in our short list
        m_lstShort.push_back(dMomentaryValue);
        if(m_dMomentary > -70.0)
        {   //greater than silence so store the none db value in our live list
            m_lstLive.push_back(dMomentaryValue);
        }
        //move the sliding frame along 100ms
        m_lstMS.pop_front();

        CalculateShort();
        //CalculateLive();
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
        m_dShort = -0.691 + 10 * log10(dShortValue);

        //move the sliding frame along 100ms
        m_lstShort.pop_front();
    }
}

void R128Calculator::CalculateLive()
{
    //work out absolute non-gated value
    double dLiveValue(0.0);
    for(list<double>::iterator itLive = m_lstLive.begin(); itLive != m_lstLive.end(); ++itLive)
    {
        dLiveValue += (*itLive)/static_cast<double>(m_lstLive.size());
    }
    m_dLiveAbs = -0.691 + 10*log10(dLiveValue);

    //now
    double dLiveGate(0.0);
    double dCount(0);
    for(list<double>::iterator itLive = m_lstLive.begin(); itLive != m_lstLive.end(); ++itLive)
    {
        double dValue = -0.691 + 10*log10((*itLive));
        if(m_dLiveAbs-dValue < 10.0)
        {
            dLiveGate += (*itLive);
            dCount++;
        }
    }
    dLiveGate /= dCount;

    m_dLiveGate = -0.691 + 10*log10(dLiveValue);

}

void R128Calculator::WorkoutMS()
{
    double dMS = 0.0;
    for(list<double>::iterator itMS = m_lstChunk.begin(); itMS != m_lstChunk.end(); ++itMS)
    {
        dMS += (*itMS); //@todo we should do something clever here to do with weighting of channels
    }
    dMS /= static_cast<double>(m_lstChunk.size());

    m_lstMS.push_back(dMS);
}

double R128Calculator::GetMomentaryLevel()
{
    return m_dMomentary;
}

double R128Calculator::GetShortLevel()
{
    return m_dShort;
}

double R128Calculator::GetLiveLevel()
{
    return m_dLiveGate;
}

double R128Calculator::GetLURange()
{
    return m_dRange;
}

void R128Calculator::ResetMeter()
{
    m_lstMS.clear();
    m_lstChunk.clear();
    m_lstShort.clear();
    m_lstLive.clear();
    m_dMomentary = 0.0;
    m_dShort = 0.0;
    m_dLiveGate = 0.0;
    m_dLiveAbs = 0.0;
    m_dRange = 0.0;

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