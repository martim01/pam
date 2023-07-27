#pragma once
#include "dlldefine.h"
#include "timedbuffer.h"
#include <list>
#include <algorithm>
#include <vector>
#include <map>
#include <wx/colour.h>
#include <set>


extern const wxString CH_GROUP_ORDER[8];
extern const wxString CH_GROUP_TYPE[15];
extern const wxString CH_GROUP_CHANNEL[26];
extern const wxString CH_GROUP_CHANNEL_LONG[26];
extern const wxColour CH_GROUP_COLOUR[8];

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
    enum enumChannelGrouping { M, DM, ST, LtRt, FIVE1, SEVEN1, SGRP, U01, U02, U03, U04, U05, U06, U07, U08};

    enum enumChannel{ MONO=0, MONO_1, MONO_2, LEFT, RIGHT, LEFT_TOTAL, RIGHT_TOTAL, CENTER, LFE, LEFT_SIDE, RIGHT_SIDE, LEFT_REAR_SIDE, RIGHT_REAR_SIDE, SDI_1,
        SDI_2,SDI_3,SDI_4, UNDEFINED_1,UNDEFINED_2,UNDEFINED_3,UNDEFINED_4,UNDEFINED_5,UNDEFINED_6,UNDEFINED_7,UNDEFINED_8, UNSET};


    struct channelGrouping
    {
        channelGrouping(unsigned char n, enumChannelGrouping g, enumChannel t) : nId(n), grouping(g), type(t){}
        channelGrouping() : nId(0), grouping(enumChannelGrouping::U01), type(enumChannel::UNDEFINED_1){}

        unsigned char nId;
        enumChannelGrouping grouping;
        enumChannel type;
    };

    subsession()=default;
    subsession(const wxString& sI, const wxString& sourceaddress, const wxString& group, const wxString& medium, const wxString& codec, const wxString& protocol, unsigned int port, unsigned int samplerate, const std::vector<channelGrouping>& channels,unsigned int synctimestamp, const timeval& epoch, const refclk& clk) :
    sId(sI), sSourceAddress(sourceaddress), sGroup(group), sMedium(medium), sCodec(codec), sProtocol(protocol), nPort(port), nSampleRate(samplerate), nChannels(channels.size()),
    vChannels(channels), nSyncTimestamp(synctimestamp), tvEpoch(epoch),refClock(clk){}

    wxString sId;
    wxString sSourceAddress;
    wxString sGroup;
    wxString sMedium;
    wxString sCodec;
    wxString sProtocol;
    
    unsigned int nPort = 0;
    unsigned int nSampleRate = 0;
    unsigned int nChannels = 0;
    
    std::vector<channelGrouping> vChannels;

    unsigned int nSyncTimestamp = 0;
    timeval tvEpoch;
    refclk refClock;

};

struct PAMBASE_IMPEXPORT session
{
    session(const wxString& sRaw=wxEmptyString, const wxString& sn=wxEmptyString, const wxString& st=wxEmptyString, const wxString& sD=wxEmptyString, const std::set<wxString>& groups={}) : sRawSDP(sRaw), sName(sn), sType(st), sDescription(sD), setGroups(groups), lstSubsession(std::list<subsession>())
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
    std::set<wxString> setGroups;
    refclk refClock;

    std::list<subsession> lstSubsession;

    private:
        std::list<subsession>::const_iterator itCurrentSubsession;

};


struct PAMBASE_IMPEXPORT qosData
{
    qosData() : tsTime(0,0,0,0),
    tvLastSR_Time{0,0},
    tvSync{0,0}
    {}


    wxTimeSpan tsTime;
    double dkbits_per_second_min = -1.0;
    double dkbits_per_second_max = -1.0;
    double dkbits_per_second_Av = -1.0;
    double dkbits_per_second_Now = -1.0;
    double dkBytesTotal = -1.0;
    double dPacket_loss_fraction_min = -1.0;
    double dPacket_loss_fraction_max = -1.0;
    double dPacket_loss_fraction_av = -1.0;
    unsigned int nTotNumPacketsReceived = 0;

    double dInter_packet_gap_ms_Now = -1.0;
    double dInter_packet_gap_ms_min = -1.0;
    double dInter_packet_gap_ms_av = -1.0;
    double dInter_packet_gap_ms_max = -1.0;
    int nTotNumPacketsLost = -1;

    unsigned int nBaseExtSeqNum = 0;
    unsigned int nLastResetExtSeqNum = 0;
    unsigned int nHighestExtSeqNum = 0;
    double dJitter = -1.0;
    unsigned int nLastSR_NTPmsw = 0;
    unsigned int nLastSR_NTPlsw = 0;
    timeval tvLastSR_Time;
    timeval tvSync;
    unsigned int nSSRC = 0;

    double dTSDF = -1.0;
    unsigned int nTimestampErrors = 0;
    unsigned int nTimestampErrorsTotal = 0;

    wxString sStream;
    unsigned long nFramesUsed = 0;
};

extern const std::map<subsession::enumChannelGrouping, unsigned char> CH_GROUP_SIZE;

extern wxString GetChannelLabel(const subsession::channelGrouping& ch);
extern wxString GetChannelLabelLong(const subsession::channelGrouping& ch);
extern wxString GetChannelGroupName(const subsession::channelGrouping& ch);
