#pragma once
#include <list>
#include <vector>


using nonInterlacedList = std::pair<std::list<float>, std::list<float>>;
using nonInterlacedVector = std::pair<std::vector<float>, std::vector<float>>;

class DelayLine
{
    public:
        DelayLine(unsigned int nWindowSamples = 9600, unsigned int nAccuracy=24);
        ~DelayLine();

        void SetWindowSize(unsigned int nSamples)   { m_nWindowSamples = nSamples;  }
        void SetAccuracy(unsigned int nSamples)     { m_nAccuracy = nSamples;      }

        void SetCalculationMode(bool bOneShot);

        int ProcessAudio(nonInterlacedVector& data);
        void CalculateChannelOffset();

    private:
        unsigned int m_nWindowSamples;
        unsigned int m_nAccuracy;
        nonInterlacedList m_Buffer;
        int m_nOffset;

        bool m_bOneShot;
        bool m_bCalculated;

};
