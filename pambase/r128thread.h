#pragma once
#include <list>
#include <wx/thread.h>

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

    private:

        void CalculateLive();
        void CalculateRange();

        wxMutex m_mutex;
        std::list<double> m_lstLive;
        std::list<double> m_lstRange;

        double m_dLive;
        double m_dRange;
        double m_dLiveTotal;
        double m_dRangeTotal;
};

