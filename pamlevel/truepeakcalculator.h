#pragma once

#include "pamleveldll.h"
#include <vector>


class Filter;
struct session;
struct timedbuffer;
class R128Thread;
struct SrcWrapper;

class PAMLEVEL_IMPEXPORT TruePeakCalculator
{
    public:
        TruePeakCalculator();
        ~TruePeakCalculator();

        void InputSession(const session& aSession);
        void CalculateLevel(const timedbuffer* pBuffer);
        double GetLevel(unsigned int nChannel);

    private:
        SrcWrapper* m_pSrc;
        std::vector<Filter*> m_vFilter;
        std::vector<float> m_vTruePeak;
        std::vector<float> m_vCurrentPeak;
        std::vector<float> m_vSamplePeak;

        unsigned int m_nChannels;
};

