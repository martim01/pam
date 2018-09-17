#include "niir.h"
#include <wx/log.h>

using namespace std;



IIR::IIR(const vector<double>& aCoeff, const vector<double>& bCoeff) :
    m_vCoeffA(aCoeff),
    m_vCoeffB(bCoeff)
{


}

IIR::IIR(const double aCoeff[], const double bCoeff[], int nSizeA, int nSizeB)
{
    m_vCoeffA.resize(nSizeA);
    m_vCoeffB.resize(nSizeB);

    for(int i = 0; i < nSizeA; i++)
    {
        m_vCoeffA[i] = aCoeff[i];
        m_lstY.push_back(0.0);
    }

    for(int i = 0; i < nSizeB; i++)
    {
        m_vCoeffB[i] = bCoeff[i];
        m_lstX.push_back(0.0);
    }
}


double IIR::Filter(double dSample)
{
    double dY = 0.0;
    m_lstX.push_front(dSample);

    size_t n = 0;
    for(list<double>::iterator itSample = m_lstX.begin(); itSample != m_lstX.end(); ++itSample)
    {
        dY += (*itSample)*m_vCoeffB[n];
        n++;
    }

    n = 0;
    for(list<double>::iterator itOutput = m_lstY.begin(); itOutput != m_lstY.end(); ++itOutput)
    {
        dY -= (*itOutput)*m_vCoeffA[n];
        n++;
    }

    m_lstX.pop_back();
    m_lstY.pop_back();

    m_lstY.push_front(dY);

   // wxLogDebug(wxT("Filter:: %d %d %f"), m_lstX.size(), m_lstY.size(), dY);
    return dY;

}



const double KFilter::PF_A[2] = {-1.69065929318241, 0.73248077421585};
const double KFilter::PF_B[3] = {1.53512485958679, -2.691696189403638, 1.19839281085285};
const double KFilter::F_A[2] = {-1.99004745483398, 0.99007225036621};
const double KFilter::F_B[3] = {1.0, -2.0, 1.0};


KFilter::KFilter()
{
    m_pPreFilter = new IIR(PF_A, PF_B, 2,3);
    m_pFilter = new IIR(F_A, F_B, 2,3);
}

KFilter::~KFilter()
{
    delete m_pPreFilter;
    delete m_pFilter;
}

double KFilter::Filter(double dSample)
{
    return m_pFilter->Filter(m_pPreFilter->Filter(dSample));
}
