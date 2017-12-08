#pragma once
#include "dlldefine.h"

class ppm;
class loud;

struct session;
class timedbuffer;

class PAMBASE_IMPEXPORT LevelCalculator
{
    public:
        LevelCalculator(float dMin);
        ~LevelCalculator();

        void InputSession(const session& aSession);
        void SetMode(unsigned int nMode);
        void SetMSMode(long nMode);
        void SetSpeed(long nSpeed);
        void CalculateLevel(const timedbuffer* pBuffer);

        double GetLevel(unsigned int nChannel);
        double GetMSLevel(bool bStereo);

        enum {PPM, PEAK, ENERGY, LOUD, TOTAL, AVERAGE};


    private:

        void CreatePpm();
        void CreateLoud();

        void CalculatePpm(const timedbuffer* pBuffer);
        void CalculateLoud(const timedbuffer* pBuffer);
        void CalculatePeak(const timedbuffer* pBuffer);
        void CalculateEnergy(const timedbuffer* pBuffer);
        void CalculateTotal(const timedbuffer* pBuffer);
        void CalculateAverage(const timedbuffer* pBuffer);
        void ResetLevels(float dLevel);

        void DeletePpmLoud();

        unsigned int m_nChannels;
        unsigned int m_nMode;
        bool m_bMS;
        long m_nMSMode;
        long m_nSpeed;

        ppm* m_pPpm;
        loud* m_pLoud;

        ppm* m_pPpmMS;
        loud* m_pLoudMS;

        float m_dLevel[8];
        float m_dMS[2];
        float m_dMin;
};
