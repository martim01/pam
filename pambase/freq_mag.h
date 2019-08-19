#pragma once
#include "dlldefine.h"
#include <vector>
#include <list>

struct PAMBASE_IMPEXPORT freq_mag
{
    freq_mag(size_t nSize) : vAmplitude(nSize,0.0),
							dBinSize(0.0){}

	freq_mag(const freq_mag& fm) : vAmplitude(fm.vAmplitude), lstPeaks(fm.lstPeaks), dBinSize(fm.dBinSize){}
    freq_mag& operator=(const freq_mag& fm)
    {
        vAmplitude = fm.vAmplitude;
        lstPeaks = fm.lstPeaks;
        dBinSize = fm.dBinSize;
        return *this;
    }

    std::vector<float> vAmplitude;
    std::list<int> lstPeaks;
    double dBinSize;

    void WorkoutPeaks(double dLimit);
};
