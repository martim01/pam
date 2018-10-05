#pragma once
#include "dlldefine.h"


struct session;
class timedbuffer;



class PAMBASE_IMPEXPORT LevelCalculator
{
    public:
        LevelCalculator(double dMin);
        ~LevelCalculator();

        void InputSession(const session& aSession);
        void SetMode(unsigned int nMode);
        void SetMSMode(long nMode);
        void SetSpeed(long nSpeed);
        void CalculateLevel(const timedbuffer* pBuffer);

        void SetDynamicResponse(double dRiseTime, double dRisedB, double dFallTime, double dFalldB);



        double GetLevel(unsigned int nChannel);
        double GetMSLevel(bool bStereo);

        enum {PPM, PEAK, ENERGY, TOTAL, AVERAGE};
        enum speeds { SLOW, NORMAL, FAST };
        enum {M3, M6};

    private:


        void CalculatePpm(const timedbuffer* pBuffer);
        void CalculatePeak(const timedbuffer* pBuffer);
        void CalculateEnergy(const timedbuffer* pBuffer);
        void CalculateTotal(const timedbuffer* pBuffer);
        void CalculateAverage(const timedbuffer* pBuffer);
        void ResetLevels(double dLevel);


        void CalculateRiseFall(unsigned long nSamples);

        void CalculateRiseFall(double& dCurrent, double& dLast,double dFalldB, double dRisedB, bool bDebug=false);

        void ConvertToDb(double& dSample);
        void CalculateDynamicRepsonse();


        unsigned int m_nChannels;
        unsigned int m_nSampleRate;
        unsigned int m_nMode;
        bool m_bMS;
        long m_nMSMode;

        double m_dSpeed;

        double m_dLevel[8];
        double m_dMS[2];
        double m_dMin;

        double m_dLastLevel[8];
        double m_dLastMS[2];
        double m_dInterim[8];
        double m_dInterimMS[2];

        double m_dRisedB;
        double m_dFalldB;
        double m_dRiseMs;
        double m_dFallMs;
        double m_dRiseSample;
        double m_dFallSample;

        double m_dDropFactor;
        double m_dDropRatio;

        double m_dRiseFactor;
        double m_dRiseTime;
        double m_dRiseRatio;
};
