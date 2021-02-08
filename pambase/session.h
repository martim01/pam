#pragma once
#include "dlldefine.h"
#include "timedbuffer.h"
#include <list>
#include <algorithm>


struct PAMBASE_IMPEXPORT frameBuffer
{
    frameBuffer() : nFrameSize(0), pBuffer(0), nBytesPerSample(0), nChannels(0){}
    frameBuffer(const timeval& tv, unsigned long nFs, unsigned char* pB, unsigned char nBS, const timeval& tvT) : timePresentation(tv), nFrameSize(nFs), pBuffer(pB), nBytesPerSample(nBS), nChannels(2), timeTransmission(tvT){}

    timeval timePresentation;
    unsigned long nFrameSize;
    unsigned char* pBuffer;
    unsigned char nBytesPerSample;
    unsigned char nChannels;
    timeval timeTransmission;
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

    subsession(const wxString& sI, const wxString& sourceaddress, const wxString& medium, const wxString& codec, const wxString& protocol, unsigned int port, unsigned int samplerate, unsigned int channels, const wxString& channelnames, unsigned int synctimestamp, const timeval& epoch, const refclk& clk) :
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
    timeval tvEpoch;
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
    qosData() : tsTime(0,0,0,0),
    dkbits_per_second_min(-1.0),
    dkbits_per_second_max(-1.0),
    dkbits_per_second_Av(-1.0),
    dkbits_per_second_Now(-1.0),
    dkBytesTotal(-1.0),
    dPacket_loss_fraction_min(-1.0),
    dPacket_loss_fraction_max(-1.0),
    dPacket_loss_fraction_av(-1.0),
    nTotNumPacketsReceived(0),
    dInter_packet_gap_ms_Now(-1.0),
    dInter_packet_gap_ms_min(-1.0),
    dInter_packet_gap_ms_av(-1.0),
    dInter_packet_gap_ms_max(-1.0),
    nTotNumPacketsLost(-1),
    nBaseExtSeqNum(0),
    nLastResetExtSeqNum(0),
    nHighestExtSeqNum(0),
    dJitter(-1.0),
    nLastSR_NTPmsw(0),
    nLastSR_NTPlsw(0),
    tvLastSR_Time{0,0},
    tvSync{0,0},
    nSSRC(0),
    dTSDF(-1.0),
    nTimestampErrors(0),
    nTimestampErrorsTotal(0){}


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
    timeval tvSync;
    unsigned int nSSRC;

    double dTSDF;
    unsigned int nTimestampErrors;
    unsigned int nTimestampErrorsTotal;
};

