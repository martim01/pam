#pragma once
#include <vector>

typedef std::pair<unsigned long, bool > pairTransition_t;

class timedbuffer;
class session;

class GlitsDetector
{
    public:
        enum enumType{GD_UNKNOWN, GD_GLITS_LR, GD_GLITS_RL, GD_EBU_LR, GD_EBU_RL, GD_MONO};

        GlitsDetector();
        ~GlitsDetector();
        void SetAudioData(const timedbuffer* pBuffer);
		void InputSession(const session& aSession);

		void Unlock();
        enumType GetType();

    private:

        void WorkoutSignal();

        bool IsGlits(const std::vector<bool>& vLeft, const std::vector<bool>& vRight);
        bool CheckLength(unsigned long nStart, unsigned long nEnd, double dTarget, double dBand);

        bool IsSignal(float fLevel);

        std::vector<pairTransition_t> CreateTransition(const std::vector<bool>& vLevel);
        void CountTransitions(const std::vector<pairTransition_t>& vTransition, std::size_t& nSilenceCount, std::size_t& nSilenceTime, std::size_t& nSignalCount, std::size_t& nSignalTime);
        std::vector<bool> m_vBuffer[2];

        float m_dLastLeft;
        float m_dLastRight;
        bool m_bLocked;
        unsigned long m_nSampleCount;
        unsigned long m_nSineCount;
        unsigned long m_nSize;
        unsigned long m_nSampleRate;
        unsigned long m_nChannels;
        enumType m_eType;
};
