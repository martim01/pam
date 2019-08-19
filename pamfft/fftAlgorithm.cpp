#include "fftAlgorithm.h"
#include "kaiserwindow.h"
#include <wx/log.h>
#include <wx/string.h>
#include <math.h>

using namespace std;

FFTAlgorithm::FFTAlgorithm() :
m_dBinSize(0.0),
m_dFundamentalBinFrequency(0.0),
m_dFundamentalAmplitude(0.0),
m_dTHD(0.0),
m_nPeaks(0)
{

}

vector<kiss_fft_cpx> FFTAlgorithm::DoFFT(list<float>& lstBuffer, unsigned long nSampleRate, unsigned long nChannels, int nRouting, int nWindow, unsigned int nBins, unsigned int nBufferToDelete)

{
    m_vfft_in.resize((nBins-1)*2);
    vector<kiss_fft_cpx> vfft_out;
    vfft_out.resize(nBins);

    m_dTHD = -1;
    m_dFundamentalBinFrequency=-1;
    m_dFundamentalAmplitude=-1;
    m_nPeaks = 0;

    switch(nRouting)
    {
        case ANALYSE_L_P_R:
            CopyLeftPlusRightIntoFFT(lstBuffer, nBufferToDelete, nWindow);
            break;
        case ANALYSE_L_M_R:
            CopyLeftMinusRightIntoFFT(lstBuffer, nBufferToDelete, nWindow);
            break;
        default:
           CopyChannelIntoFFT(lstBuffer, nChannels, nRouting, nBufferToDelete, nWindow);
    }

    //Now do the check
    kiss_fftr_cfg cfg;


    if ((cfg = kiss_fftr_alloc(m_vfft_in.size(), 0, NULL, NULL)) != NULL)
    {
        kiss_fftr(cfg, m_vfft_in.data(), vfft_out.data());
    }
    free(cfg);

    return vfft_out;
}


void FFTAlgorithm::CopyChannelIntoFFT(std::list<float>& lstBuffer, unsigned long nChannels, unsigned int nChannel, unsigned int nBufferToDelete, int nWindow)
{
    list<float>::iterator itSample = lstBuffer.begin();


    for(size_t nSample = 0; nSample < m_vfft_in.size(); nSample++)
    {
        //skip the channels before the one we want
        int i = 0;
        if(nSample < nBufferToDelete)
        {
            for(; i < nChannel; ++i)   //skip the other channels
            {
                itSample = lstBuffer.erase(itSample);
            }
        }
        else
        {
            for(; i < nChannel; ++i)   //skip the other channels
            {
                ++itSample;
            }
        }


        m_vfft_in[nSample] = ApplyWindow(nWindow, (*itSample), nSample);

        if(nSample < nBufferToDelete)
        {
            for(; i < nChannels; ++i)
            {
                itSample = lstBuffer.erase(itSample);
            }
        }
        else
        {
            //++itSample; //move on from our sample
            for(; i < nChannels; ++i)   //skip the other channels
            {
                ++itSample;
            }
        }
    }
}


void FFTAlgorithm::CopyLeftPlusRightIntoFFT(list<float>& lstBuffer, unsigned int nBufferToDelete, int nWindow)
{
    if(lstBuffer.empty() == false)
    {
        list<float>::iterator itSample = lstBuffer.begin();
        for(size_t i = 0; i < m_vfft_in.size(); i++)
        {
            list<float>::iterator itRight(itSample);
            ++itRight;

            m_vfft_in[i] = ApplyWindow(nWindow, ((*itSample)+(*itRight))/2, i);

            if(i < nBufferToDelete)
            {
                itSample = lstBuffer.erase(lstBuffer.erase(itSample));  //erase the left
            }
            else
            {
                ++(++itSample);
            }
        }
    }
}

void FFTAlgorithm::CopyLeftMinusRightIntoFFT(list<float>& lstBuffer, unsigned int nBufferToDelete, int nWindow)
{
    if(lstBuffer.empty() == false)
    {
        list<float>::iterator itSample = lstBuffer.begin();
        for(size_t i = 0; i < m_vfft_in.size(); i++)
        {
            list<float>::iterator itRight(itSample);
            ++itRight;

            m_vfft_in[i] = ApplyWindow(nWindow, ((*itSample)-(*itRight))/2, i);

            if(i < nBufferToDelete)
            {
                itSample = lstBuffer.erase(lstBuffer.erase(itSample));
            }
            else
            {
                ++(++itSample);
            }
        }
    }
}

float FFTAlgorithm::ApplyWindow(int nWindow, float dIn, size_t nSample)
{
    switch(nWindow)
    {
        case WINDOW_RECTANGULAR:
            return dIn;
        case WINDOW_HANNING:
            return HanningWindow(dIn, nSample);
        case WINDOW_KAISER:
            return KaiserWindow(dIn, nSample);
        case WINDOW_BLACKMAN:
            return BlackmanWindow(dIn, nSample);
        case WINDOW_KAISERBESSEL:
            return KaiserBesselWindow(dIn, nSample);
    }
    return dIn;
}

float FFTAlgorithm::HanningWindow(float dIn, size_t nSample)
{
    return 0.5*(1-cos((2*M_PI*static_cast<float>(nSample))/static_cast<float>(m_vfft_in.size()-1)))*dIn;
}

float FFTAlgorithm::HammingWindow(float dIn, size_t nSample)
{
    return (0.54 - (0.46*(1-cos((2*M_PI*static_cast<float>(nSample))/static_cast<float>(m_vfft_in.size()-1)))))*dIn;
}

float FFTAlgorithm::BlackmanWindow(float dIn, size_t nSample)
{
    return (0.42 - (0.5*cos((2*M_PI*static_cast<float>(nSample))/static_cast<float>(m_vfft_in.size()-1)))+(0.08*cos((4*M_PI*static_cast<float>(nSample))/static_cast<float>(m_vfft_in.size()-1))))*dIn;
}

float FFTAlgorithm::KaiserBesselWindow(float dIn, size_t nSample)
{
    return (0.402-0.498 * cos(2*M_PI*static_cast<float>(nSample)/static_cast<float>(m_vfft_in.size()-1))
                +0.098 * cos(4*M_PI*static_cast<float>(nSample)/static_cast<float>(m_vfft_in.size()-1))
                +0.001 * cos(6*M_PI*static_cast<float>(nSample)/static_cast<float>(m_vfft_in.size()-1)))*dIn;
}


float FFTAlgorithm::KaiserWindow(float dIn, size_t nSample)
{
    double dShape = KaiserWindow::computeShape(60.0);

    return KaiserWindow::buildWindow(dIn, nSample, dShape, m_vfft_in.size()-1);

}

float FFTAlgorithm::Bessel(float x)
{
    float dSum = 0;
    float dXtoIPower;

    for(int i = 0; i < 10; i++)
    {
        dXtoIPower = pow(x/2.0, static_cast<float>(i));

        int nFactorial(1);
        for(int j = 1; j <= i; j++)
        {
            nFactorial *= j;
        }
        dSum += pow(dXtoIPower/static_cast<float>(nFactorial), 2.0);
    }
    return 1.0+dSum;
}


double FFTAlgorithm::GetMaxBinFrequency(list<float>& lstBuffer, unsigned long nSampleRate,  unsigned long nChannels, int nRouting, int nWindow, unsigned int nBins, unsigned int nBufferToDelete)
{

    vector<kiss_fft_cpx> vfft_out(DoFFT(lstBuffer, nSampleRate, nChannels, nRouting, nWindow, nBins, nBufferToDelete));

    double dBinSize = static_cast<double>(nSampleRate)/static_cast<double>((vfft_out.size()-1)*2);

    m_dFundamentalAmplitude = -80;
    double dMaxBin(0);
    for(size_t i = 0; i < vfft_out.size(); i++)
    {
        float dAmplitude(sqrt( (vfft_out[i].r*vfft_out[i].r) + (vfft_out[i].i*vfft_out[i].i)));
        if(dAmplitude<0)
        {
            dAmplitude=-dAmplitude;
        }
        dAmplitude /= static_cast<float>(vfft_out.size());
        double dLog = 20*log10(dAmplitude);

        if(m_dFundamentalAmplitude < dLog)
        {
            m_dFundamentalAmplitude = dLog;
            dMaxBin = i;
        }
    }

    m_dTHD = -1;

    m_dFundamentalBinFrequency = dMaxBin*dBinSize;
    return m_dFundamentalBinFrequency;
}


double FFTAlgorithm::GetTHDistortion(list<float>& lstBuffer, unsigned long nSampleRate,  unsigned long nChannels, int nRouting, int nWindow, unsigned int nBins, unsigned int nBufferToDelete)
{
    vector<kiss_fft_cpx> vfft_out(DoFFT(lstBuffer, nSampleRate,nChannels, nRouting, nWindow, nBins, nBufferToDelete));

    if(vfft_out.size() > 1)
    {
        map<int, double> mPeaks;

        double dBinSize = static_cast<double>(nSampleRate)/static_cast<double>((vfft_out.size()-1)*2);

        double dLastBin(0);
        int nPeaks(0);
        bool bDown(false);
        double dMaxPeak(0);
        size_t nMaxBin;

        vector<double> vAmp;
        vAmp.resize(vfft_out.size());

        for(size_t i = 0; i < vfft_out.size(); i++)
        {
            float dAmplitude(sqrt( (vfft_out[i].r*vfft_out[i].r) + (vfft_out[i].i*vfft_out[i].i)));
            if(dAmplitude<0)
            {
                dAmplitude=-dAmplitude;
            }
            dAmplitude /= static_cast<float>(vfft_out.size());
            vAmp[i] = dAmplitude;

            if(dAmplitude < dLastBin)
            {
                if(bDown == false)  //we were going up, now we are going down so the last bin must be a peak
                {
                    if(dAmplitude > 0.001)
                    {
                        mPeaks.insert(make_pair(i-1, dLastBin));
                        if(dLastBin > dMaxPeak)
                        {
                            nMaxBin = i-1;
                            dMaxPeak = dLastBin;
                        }
                    }
                }
                bDown = true;
            }
            else
            {
                bDown = false;
            }

            dLastBin = dAmplitude;
        }


        if(dMaxPeak > 0.0)
        {
            double dQ = (vAmp[nMaxBin+1]-vAmp[nMaxBin-1])/(2*(2*vAmp[nMaxBin]-vAmp[nMaxBin-1]-vAmp[nMaxBin+1]));
            m_dFundamentalBinFrequency = (static_cast<double>(nMaxBin)+dQ)*dBinSize;
            m_dFundamentalAmplitude = 20*log10(dMaxPeak);

            if(mPeaks.size() > 0)
            {
                float dPower1(pow(dMaxPeak,2));
                float dPower(0);
                for(map<int,double>::iterator itPeak = mPeaks.begin(); itPeak != mPeaks.end(); ++itPeak)
                {
                    if(itPeak->first != nMaxBin)
                    {
                        float dPow = pow(itPeak->second,2);
                        dPower +=dPow;
                    }
                }
                m_dTHD = 100.0 * sqrt(dPower/dPower1);

            }
            else
            {
                m_dFundamentalAmplitude=-1000;
                m_dFundamentalBinFrequency=0;
                m_nPeaks = 0;
                return 0;
            }

            m_nPeaks = mPeaks.size();
        }
        else
        {
            m_dTHD = 0.0;
        }
    }
    else
    {
        m_dTHD = 0.0;
    }
    return m_dTHD;
}

map<int, double>  FFTAlgorithm::GetPeaks(list<float>& lstBuffer, unsigned long nSampleRate, unsigned long nChannels,  int nRouting, int nWindow, unsigned int nBins, unsigned int nBufferToDelete)
{
    vector<kiss_fft_cpx> vfft_out(DoFFT(lstBuffer, nSampleRate, nChannels, nRouting, nWindow, nBins, nBufferToDelete));

    map<int, double> mPeaks;

    double dBinSize = static_cast<double>(nSampleRate)/static_cast<double>((vfft_out.size()-1)*2);

    double dLastBin(-800);
    int nPeaks(0);
    bool bDown(false);

    for(size_t i = 0; i < vfft_out.size(); i++)
    {
        float dAmplitude(sqrt( (vfft_out[i].r*vfft_out[i].r) + (vfft_out[i].i*vfft_out[i].i)));
        if(dAmplitude<0)
        {
            dAmplitude=-dAmplitude;
        }
        dAmplitude /= static_cast<float>(vfft_out.size());
        double dLog = 20*log10(dAmplitude);


        if(dLog < dLastBin)
        {
            if(bDown == false)  //we were going up, now we are going down so the last bin must be a peak
            {
                if(dLog > -100)
                {
                    mPeaks.insert(make_pair(i-1, dLastBin));
                }
            }
            bDown = true;
        }
        else
        {
            bDown = false;
        }

        dLastBin = dLog;
    }
    m_nPeaks = mPeaks.size();

    return mPeaks;
}


double FFTAlgorithm::GetFundamentalBinFrequency()
{
    return m_dFundamentalBinFrequency;
}
double FFTAlgorithm::GetFundamentalAmplitude()
{
    return m_dFundamentalAmplitude;
}


size_t FFTAlgorithm::GetNumberOfPeaks()
{
    return m_nPeaks;
}
