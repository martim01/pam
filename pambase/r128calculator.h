#pragma once
#include <list>
#include <vector>
#include "dlldefine.h"

struct session;
struct timedbuffer;

class PAMBASE_IMPEXPORT R128Calculator
{
    public:
        R128Calculator();
        ~R128Calculator();

        void InputSession(const session& aSession);
        void CalculateLevel(const timedbuffer* pBuffer);
        double GetMomentaryLevel();
        double GetShortLevel();
        double GetLiveLevel();
        double GetLURange();

        void ResetMeter();

    protected:

        double ApplyFilter(double dSample, unsigned int nChannel);

        void CalculateMomentary();
        void CalculateShort();
        void CalculateLive();

        std::list<double> m_lstMS;


        std::list<double> m_lstShort;
        std::list<double> m_lstLive;

        double m_dMomentary;
        double m_dShort;
        double m_dLiveGate;
        double m_dLiveAbs;
        double m_dRange;

        unsigned int m_nInputChannels;
        unsigned int m_nChunkFrames;
        unsigned int  m_nChunkSize;

        unsigned int m_nFrames;
        double m_dTempMS;

        std::vector<std::pair<double,double> > m_vPreFilter;
        std::vector<std::pair<double,double> > m_vFilter;


        /* pre-filter coefficients */
        static const double PF_A1 = -1.69065929318241;
        static const double PF_A2 = 0.73248077421585;
        static const double PF_B0 = 1.53512485958679;
        static const double PF_B1 = -2.691696189403638;
        static const double PF_B2 = 1.19839281085285;

        /* filter coefficients */
        static const double F_A1 = -1.99004745483398;
        static const double F_A2 = 0.99007225036621;
        static const double F_B0 = 1.0;
        static const double F_B1 = -2.0;
        static const double F_B2 = 1.0;

};
