#ifndef PML_PAMBASE_LEVELCALCULATOR_H
#define PML_PAMBASE_LEVELCALCULATOR_H

#include <array>

#include "dlldefine.h"


struct session;
class timedbuffer;



class PAMBASE_IMPEXPORT LevelCalculator
{
    public:
        explicit LevelCalculator(double dMin);
        ~LevelCalculator();

        void InputSession(const session& aSession);
        void SetMode(unsigned int nMode);
        void SetMSMode(long nMode);
        void SetSpeed(long nSpeed);
        void CalculateLevel(const timedbuffer* pBuffer);

        void SetDynamicResponse(double dRiseTime, double dRisedB, double dFallTime, double dFalldB);



        double GetLevel(unsigned int nChannel);
        double GetMSLevel(bool bSide, unsigned int nPair=0);

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

        void CalculateMS();


        unsigned int m_nChannels;
        unsigned int m_nSampleRate;
        unsigned int m_nMode;
        bool m_bMS;
        long m_nMSMode;

        double m_dSpeed;

        std::array<double,8> m_dLevel;
        std::array<double,8> m_dMS;
        double m_dMin;

        std::array<double,8> m_dLastLevel;
        std::array<double,8> m_dLastMS;
        std::array<double,8> m_dInterim;
        std::array<double,8> m_dInterimMS;

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

#endif