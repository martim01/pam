#pragma once


#include <vector>
#include <list>

class IIR
{
    public:
        IIR(const std::vector<double>& aCoeff, const std::vector<double>& bCoeff);
        IIR(const double aCoeff[], const double bCoeff[], int nSizeA, int nSizeB);

        double Filter(double dSample);

    private:
        std::vector<double> m_vCoeffA;
        std::vector<double> m_vCoeffB;


        std::list<double> m_lstX;
        std::list<double> m_lstY;

};



class KFilter
{
    public:
        KFilter();
        ~KFilter();

        double Filter(double dSample);

    private:
        static const double PF_A[2];
        static const double PF_B[3];

        static const double F_A[2];
        static const double F_B[3];

        IIR* m_pPreFilter;
        IIR* m_pFilter;
};

