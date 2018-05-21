#include "r128thread.h"
#include <wx/utils.h>
#include <wx/log.h>


using namespace std;

R128Thread::R128Thread() : wxThread()
{

}

void R128Thread::AddToLive(double dValue)
{
    wxMutexLocker ml(m_mutex);
    m_lstLive.push_back(dValue);
    if(m_lstLive.size() > 72000)
    {
        m_lstLive.pop_front();
    }
}

void R128Thread::AddToRange(double dValue)
{
    wxMutexLocker ml(m_mutex);
    m_lstRange.push_back(dValue);
    if(m_lstRange.size() > 72000)
    {
        m_lstRange.pop_front();
    }
}


void* R128Thread::Entry()
{
    while(TestDestroy() == false)
    {
        CalculateLive();
        CalculateRange();

        ::wxMilliSleep(500);
    }


    return NULL;
}

void R128Thread::CalculateLive()
{
    wxMutexLocker ml(m_mutex);
    //work out absolute non-gated value
    double dLiveValue(0.0);
    for(list<double>::iterator itLive = m_lstLive.begin(); itLive != m_lstLive.end(); ++itLive)
    {
        dLiveValue += (*itLive)/static_cast<double>(m_lstLive.size());
    }
    double dLiveAbs = -0.691 + 10*log10(dLiveValue);

    //now
    double dLiveGate(0.0);
    double dCount(0);
    for(list<double>::iterator itLive = m_lstLive.begin(); itLive != m_lstLive.end(); ++itLive)
    {
        double dValue = -0.691 + 10*log10((*itLive));
        if(dLiveAbs-dValue < 10.0)
        {
            dLiveGate += (*itLive);
            dCount++;
        }

    }
    dLiveGate /= dCount;

    m_dLive = -0.691 + 10*log10(dLiveGate);

    wxLogDebug(wxT("Abs: %.3f  Gate: %.3f"), dLiveAbs, m_dLive);
}


void R128Thread::CalculateRange()
{
    wxMutexLocker ml(m_mutex);
    //work out absolute non-gated value
    double dRangeValue(0.0);
    for(list<double>::iterator itRange = m_lstRange.begin(); itRange != m_lstRange.end(); ++itRange)
    {
        dRangeValue += (*itRange)/static_cast<double>(m_lstRange.size());
    }
    double dRangeAbs = -0.691 + 10*log10(dRangeValue);

    //now
    list<double> lstRangeGate;
    double dRangeGate(0.0);
    double dCount(0);
    for(list<double>::iterator itRange = m_lstRange.begin(); itRange != m_lstRange.end(); ++itRange)
    {
        double dValue = -0.691 + 10*log10((*itRange));
        if(dRangeAbs-dValue < 20.0)
        {
            lstRangeGate.push_back(dValue);
        }
    }
    lstRangeGate.sort();
    int nLower = (10*lstRangeGate.size())/100;
    int nHigher = (95*lstRangeGate.size())/100;

    double dRange10, dRange95;
    list<double>::iterator itValue = lstRangeGate.begin();
    for(int i = 0; i <= nHigher; i++)
    {
        if(i == nLower)
        {
            dRange10 = (*itValue);
        }
        ++itValue;
    }
    dRange95 = (*itValue);
    m_dRange = dRange95-dRange10;

}


double R128Thread::GetLive()
{
    wxMutexLocker ml(m_mutex);
    return m_dLive;
}

double R128Thread::GetRange()
{
    wxMutexLocker ml(m_mutex);
    return m_dRange;
}


void R128Thread::Reset()
{
    wxMutexLocker ml(m_mutex);
    m_dRange = -80.0;
    m_dLive = -80.0;
    m_lstLive.clear();
    m_lstRange.clear();
}
