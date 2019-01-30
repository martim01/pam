#pragma once
#include <vector>

typedef std::pair<unsigned long, bool > pairTransition_t;

class GlitsDetector
{
    public:
        enum enumType{GD_UNKNOWN, GD_GLITS_LR, GD_GLITS_RL, GD_EBU_LR, GD_EBU_RL};

        GlitsDetector();
        ~GlitsDetector();
        void SetAudioData(const timedbuffer* pBuffer);
		void InputSession(const session& aSession);

        enumType GetType();

    private:

        void WorkoutSignal();

        bool IsGlits(const std::vector<pairTransition_t>& vLeft, const std::vector<pairTransition_t>& vRight);
        bool CheckLength(unsigned long nStart, unsigned long nEnd, double dTarget, double dBand);

        bool IsSignal(float fLevel);

        std::vector<pairTransition_t> m_vTransitions[2];

        bool m_bLast[2];
        bool m_bLocked;
        unsigned long m_nCount;
        unsigned long m_nSamples;
        unsigned long m_nSize;
        unsigned long m_nSampleRate;
        unsigned long m_nChannels;
        enumType m_eType;
};
