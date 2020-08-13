#include "pcstats.h"
#include <wx/textfile.h>
#include <wx/tokenzr.h>

using namespace std;

pcStats& pcStats::Get()
{
    static pcStats pcs;
    return pcs;
}


pcStats::pcStats()
{

}


void pcStats::CalculateCpuStats()
{
    #ifdef __WXGNU__
    wxTextFile tfStats(wxT("/proc/stat"));
    if(tfStats.Open())
    {
        for(size_t i = 0; i < tfStats.GetLineCount(); i++)
        {
            if(tfStats.GetLine(i).Left(3) == wxT("cpu"))
            {
                ExtractTicks(tfStats.GetLine(i));
            }
        }
    }
    #endif // __WXGNU__
}

void pcStats::ExtractTicks(wxString sLine)
{
    wxArrayString asLine(wxStringTokenize(sLine, wxT(" ")));
    if(asLine.GetCount() >= 5)
    {
        unsigned long nTicks[4];
        unsigned long nDifference[4];
        map<wxString, cpu>::iterator itCpu = m_mCpu.insert(make_pair(asLine[0], cpu())).first;
        for(int i = 0; i < 4; i++)
        {
            asLine[i+1].ToULong(&nTicks[i]);
            nDifference[i] = nTicks[i]-itCpu->second.nTicks[i];
            itCpu->second.nTicks[i] = nTicks[i];
        }

        itCpu->second.dUsage = static_cast<double>(nDifference[USER]+nDifference[SYSTEM]+nDifference[NICE])/static_cast<double>(nDifference[USER]+nDifference[SYSTEM]+nDifference[NICE]+nDifference[IDLE]);

    }
}

double pcStats::GetTotalCpuUsage()
{
    return GetCpuUsage(wxT("cpu"));
}

double pcStats::GetCpuUsage(wxString sCpu)
{
    map<wxString, cpu>::iterator itCpu = m_mCpu.find(sCpu);
    if(itCpu != m_mCpu.end())
    {
        return itCpu->second.dUsage;
    }
    return 0.0;

}

double pcStats::GetCpuUsage(unsigned long nCpu)
{
    return GetCpuUsage(wxString::Format(wxT("cpu%lu"), nCpu));
}


void pcStats::CalculateNetworkStats()
{
    #ifdef __WXGNU__
    wxTextFile tfStats(wxT("/proc/net/dev"));
    if(tfStats.Open())
    {
        for(size_t i = 2; i < tfStats.GetLineCount(); i++)
        {
            ExtractNetStats(tfStats.GetLine(i));
        }
    }
    #endif // __WXGNU__
}

void pcStats::ExtractNetStats(wxString sLine)
{
    wxArrayString asLine(wxStringTokenize(sLine, wxT(" ")));
    if(asLine.GetCount() >= 5)
    {
        unsigned long nStats[4];
        unsigned long nDifference[4];

        asLine[0] = asLine[0].BeforeFirst(wxT(':'));
        asLine[0].Trim();

        map<wxString, netdev>::iterator itDevice = m_mDevice.insert(make_pair(asLine[0], netdev())).first;
        for(int i = 0; i < 4; i++)
        {
            asLine[i+1].ToULong(&nStats[i]);
            nDifference[i] = nStats[i]-itDevice->second.nStats[i];
            itDevice->second.nStats[i] = nStats[i];
        }
    }
}

unsigned long pcStats::GetNetBandwidth(wxString sDevice)
{
    return 0;
}

unsigned long pcStats::GetNetErrors(wxString sDevice)
{
    return 0;
}

unsigned long pcStats::GetNetDrop(wxString sDevice)
{
    return 0;
}
