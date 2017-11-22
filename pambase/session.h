#pragma once
#include "dlldefine.h"
#include "timedbuffer.h"


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


struct PAMBASE_IMPEXPORT session
{
    session(const wxString& sn, const wxString sE, const wxString& st, const wxString& sm, const wxString& sc, const wxString& sp, unsigned int np, unsigned int nSr, unsigned int nCh, unsigned int nSync, const pairTime_t& tvE) :
        sName(sn), sEndpoint(sE), sType(st), sMedium(sm), sCodec(sc), sProtocol(sp), nPort(np), nSampleRate(nSr), nChannels(nCh), nSyncTimestamp(nSync), tvEpoch(tvE){}
    wxString sName;
    wxString sEndpoint;
    wxString sType;
    wxString sMedium;
    wxString sCodec;
    wxString sProtocol;
    unsigned int nPort;
    unsigned int nSampleRate;
    unsigned int nChannels;

    unsigned int nSyncTimestamp;
    pairTime_t tvEpoch;
};


struct PAMBASE_IMPEXPORT qosData
{
    wxTimeSpan tsTime;
    double dkbits_per_second_min;
    double dkbits_per_second_max;
    double dkbits_per_second_Av;
    double dkBytesTotal;
    double dPacket_loss_fraction_min;
    double dPacket_loss_fraction_max;
    double dPacket_loss_fraction_av;
    unsigned int nTotNumPacketsReceived;

    double dInter_packet_gap_ms_min;
    double dInter_packet_gap_ms_av;
    double dInter_packet_gap_ms_max;
    int nTotNumPacketsLost;

    unsigned int nBaseExtSeqNum;
    unsigned int nLastResetExtSeqNum;
    unsigned int nHighestExtSeqNum;
    unsigned int nJitter;
    unsigned int nLastSR_NTPmsw;
    unsigned int nLastSR_NTPlsw;
    timeval tvLastSR_Time;
    unsigned int nSSRC;
};

