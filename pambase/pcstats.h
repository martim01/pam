#pragma once
#include "dlldefine.h"
#include <map>

class PAMBASE_IMPEXPORT pcStats
{
    public:
        static pcStats& Get();

        void CalculateCpuStats();
        void CalculateNetworkStats();


        double GetTotalCpuUsage();
        double GetCpuUsage(unsigned long nCpu);

        unsigned long GetNetBandwidth(wxString sDevice);
        unsigned long GetNetErrors(wxString sDevice);
        unsigned long GetNetDrop(wxString sDevice);


    private:
        pcStats();
        ~pcStats(){}

        void ExtractTicks(wxString sLine);
        void ExtractNetStats(wxString sLine);

        double GetCpuUsage(wxString sCpu);



        struct cpu
        {
            cpu(): dUsage(0)
            {
                for(int i = 0; i < 4; i++)
                    nTicks[i] = 0;
            }
            double dUsage;
            unsigned long nTicks[4];
        };

        std::map<wxString, cpu> m_mCpu;

        struct netdev
        {
            netdev() : nStats{0,0,0,0}{}
            unsigned long nStats[4];
        };

        std::map<wxString, netdev> m_mDevice;

        enum {USER,SYSTEM,NICE,IDLE};
        enum {BYTES,PACKETS,ERRORS,DROP};
};

