#pragma once
#include "kiss_fftr.h"
#include <vector>
#include <list>
#include <map>


#ifndef M_PI
#define M_E		2.7182818284590452354
#define M_LOG2E		1.4426950408889634074
#define M_LOG10E	0.43429448190325182765
#define M_LN2		0.69314718055994530942
#define M_LN10		2.30258509299404568402
#define M_PI		3.14159265358979323846
#define M_PI_2		1.57079632679489661923
#define M_PI_4		0.78539816339744830962
#define M_1_PI		0.31830988618379067154
#define M_2_PI		0.63661977236758134308
#define M_2_SQRTPI	1.12837916709551257390
#define M_SQRT2		1.41421356237309504880
#define M_SQRT1_2	0.70710678118654752440
#endif

class FFTAlgorithm
{
    public:

        FFTAlgorithm();

        std::vector<kiss_fft_cpx> DoFFT(std::list<float>& lstBuffer, unsigned long nSampleRate, unsigned long nChannels, int nRouting, int nWindow, unsigned int nBins, unsigned int nBufferToDelete);

        double GetMaxBinFrequency(std::list<float>& lstBuffer, unsigned long nSampleRate,  unsigned long nChannels, int nRouting, int nWindow, unsigned int nBins, unsigned int nBufferToDelete);
        double GetTHDistortion(std::list<float>& lstBuffer, unsigned long nSampleRate,  unsigned long nChannels, int nRouting, int nWindow, unsigned int nBins, unsigned int nBufferToDelete);

        std::map<int, double> GetPeaks(std::list<float>& lstBuffer, unsigned long nSampleRate, unsigned long nChannels, int nRouting, int nWindow, unsigned int nBins, unsigned int nBufferToDelete);

        double GetFundamentalBinFrequency();
        double GetFundamentalAmplitude();
        size_t GetNumberOfPeaks();

        enum {ANALYSE_L=0,ANALYSE_R=1, ANALYSE_L_P_R=8, ANALYSE_L_M_R=9};
        enum {WINDOW_RECTANGULAR, WINDOW_HANNING, WINDOW_HAMMING, WINDOW_BLACKMAN, WINDOW_KAISER, WINDOW_KAISERBESSEL};

    private:

        void CopyChannelIntoFFT(std::list<float>& lstBuffer, unsigned long nChannels, unsigned int nChannel, unsigned int nBufferToDelete, int nWindow);
        void CopyLeftPlusRightIntoFFT(std::list<float>& lstBuffer, unsigned int nBufferToDelete, int nWindow);
        void CopyLeftMinusRightIntoFFT(std::list<float>& lstBuffer, unsigned int nBufferToDelete, int nWindow);
        float HanningWindow(float dIn, size_t nSample);
        float KaiserWindow(float dIn, size_t nSample);
        float KaiserBesselWindow(float dIn, size_t nSample);
        float HammingWindow(float dIn, size_t nSample);
        float BlackmanWindow(float dIn, size_t nSample);
        float ApplyWindow(int nWindow, float dIn, size_t nSample);
        float Bessel(float x);

        std::vector<kiss_fft_scalar> m_vfft_in;


        double m_dBinSize;

        double m_dFundamentalBinFrequency;
        double m_dFundamentalAmplitude;
        double m_dTHD;
        size_t m_nPeaks;
};
