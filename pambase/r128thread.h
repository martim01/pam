#pragma once
#include <list>
#include <wx/thread.h>
#include <atomic>

class R128Thread : public wxThread
{
    public:
        R128Thread();
        void* Entry();

        void AddToLive(double dValue);
        void AddToRange(double dValue);

        double GetLive();
        double GetRange();

        void Reset();

        void Stop();
        size_t GetIntegrationTime();
    private:

        void CalculateLive();
        void CalculateRange();

        wxMutex m_mutex;
        std::list<double> m_lstLive;
        std::list<double> m_lstRange;

        std::atomic<bool> m_bLoop;
        double m_dLive;
        double m_dRange;
        double m_dLiveTotal;
        double m_dRangeTotal;
};

