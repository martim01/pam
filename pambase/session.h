#pragma once
#include "dlldefine.h"
#include "timedbuffer.h"
#include <list>
#include <algorithm>


struct PAMBASE_IMPEXPORT frameBuffer
{
    frameBuffer() : nFrameSize(0), pBuffer(0), nBytesPerSample(0), nChannels(0){}
    frameBuffer(const pairTime_t& tv, unsigned long nFs, unsigned char* pB, unsigned char nBS, const pairTime_t& tvT) : timePresentation(tv), nFrameSize(nFs), pBuffer(pB), nBytesPerSample(nBS), nChannels(2), timeTransmission(tvT){}

    pairTime_t timePresentation;
    unsigned long nFrameSize;
    unsigned char* pBuffer;
    unsigned char nBytesPerSample;
    unsigned char nChannels;
    pairTime_t timeTransmission;
};


struct PAMBASE_IMPEXPORT refclk
{
    refclk() : sType(wxEmptyString), sId(wxEmptyString), sVersion(wxEmptyString),nDomain(0){}
    wxString sType;
    wxString sId;
    wxString sVersion;
    unsigned long nDomain;
};

struct PAMBASE_IMPEXPORT subsession
{

    subsession(const wxString& sI, const wxString& sourceaddress, const wxString& medium, const wxString& codec, const wxString& protocol, unsigned int port, unsigned int samplerate, unsigned int channels, const wxString& channelnames, unsigned int synctimestamp, const pairTime_t& epoch, const refclk& clk) :
    sId(sI), sSourceAddress(sourceaddress), sMedium(medium), sCodec(codec), sProtocol(protocol), nPort(port), nSampleRate(samplerate),nChannels(std::min((unsigned int)256, channels)),sChannelNames(channelnames),nSyncTimestamp(synctimestamp), tvEpoch(epoch),refClock(clk){}

    wxString sId;
    wxString sSourceAddress;
    wxString sMedium;
    wxString sCodec;
    wxString sProtocol;
    unsigned int nPort;
    unsigned int nSampleRate;
    unsigned int nChannels;
    wxString sChannelNames;
    unsigned int nSyncTimestamp;
    pairTime_t tvEpoch;
    refclk refClock;

};

struct PAMBASE_IMPEXPORT session
{
    session(const wxString& sRaw=wxEmptyString, const wxString& sn=wxEmptyString, const wxString& st=wxEmptyString, const wxString& sD=wxEmptyString, const wxString& sGr=wxEmptyString) : sRawSDP(sRaw), sName(sn), sType(st), sDescription(sD), sGroups(sGr), lstSubsession(std::list<subsession>())
    {

        itCurrentSubsession = lstSubsession.end();
    }

    std::list<subsession>::const_iterator GetCurrentSubsession() const
    {
        if(lstSubsession.empty())
        {
            return lstSubsession.end();
        }
        return itCurrentSubsession;
    }

    void SetCurrentSubsession()
    {
        itCurrentSubsession = lstSubsession.begin();
    }

    wxString sRawSDP;
    wxString sName;
    wxString sType;
    wxString sDescription;
    wxString sGroups;
    refclk refClock;

    std::list<subsession> lstSubsession;

    private:
        std::list<subsession>::const_iterator itCurrentSubsession;

};


struct PAMBASE_IMPEXPORT qosData
{
    wxTimeSpan tsTime;
    double dkbits_per_second_min;
    double dkbits_per_second_max;
    double dkbits_per_second_Av;
    double dkbits_per_second_Now;
    double dkBytesTotal;
    double dPacket_loss_fraction_min;
    double dPacket_loss_fraction_max;
    double dPacket_loss_fraction_av;
    unsigned int nTotNumPacketsReceived;

    double dInter_packet_gap_ms_Now;
    double dInter_packet_gap_ms_min;
    double dInter_packet_gap_ms_av;
    double dInter_packet_gap_ms_max;
    int nTotNumPacketsLost;

    unsigned int nBaseExtSeqNum;
    unsigned int nLastResetExtSeqNum;
    unsigned int nHighestExtSeqNum;
    double dJitter;
    unsigned int nLastSR_NTPmsw;
    unsigned int nLastSR_NTPlsw;
    timeval tvLastSR_Time;
    unsigned int nSSRC;

    double dTSDF;
    unsigned int nTimestampErrors;
    unsigned int nTimestampErrorsTotal;
};

