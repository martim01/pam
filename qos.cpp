#include "qos.h"
#include "rtsputil.h"
#include "timedbuffer.h"
#include "rtpthread.h"
#include <wx/log.h>




void qosMeasurementRecord::periodicQOSMeasurement(struct timeval const& timeNow)
{
    unsigned secsDiff = timeNow.tv_sec - m_tvMeasurementEnd.tv_sec;
    int usecsDiff = timeNow.tv_usec - m_tvMeasurementEnd.tv_usec;
    double timeDiff = secsDiff + usecsDiff/1000000.0;
    m_tvMeasurementEnd = timeNow;

    RTPReceptionStatsDB::Iterator statsIter(m_pSource->receptionStatsDB());
    // Assume that there's only one SSRC source (usually the case):
    RTPReceptionStats* stats = statsIter.next(True);
    if (stats != NULL)
    {
        double kBytesTotalNow = stats->totNumKBytesReceived();
        double kBytesDeltaNow = kBytesTotalNow - m_dkBytesTotal;
        m_dkBytesTotal = kBytesTotalNow;

        double kbpsNow = timeDiff == 0.0 ? 0.0 : 8*kBytesDeltaNow/timeDiff;
        if (kbpsNow < 0.0) kbpsNow = 0.0; // in case of roundoff error
        if (kbpsNow < m_dkbits_per_second_min)
        {
            m_dkbits_per_second_min = kbpsNow;
        }
        if (kbpsNow > m_dkbits_per_second_max)
        {
            m_dkbits_per_second_max = kbpsNow;
        }

        unsigned totReceivedNow = stats->totNumPacketsReceived();
        unsigned totExpectedNow = stats->totNumPacketsExpected();
        unsigned deltaReceivedNow = totReceivedNow - m_nTotNumPacketsReceived;
        unsigned deltaExpectedNow = totExpectedNow - m_nTotNumPacketsExpected;
        m_nTotNumPacketsReceived = totReceivedNow;
        m_nTotNumPacketsExpected = totExpectedNow;

        double lossFractionNow = deltaExpectedNow == 0 ? 0.0 : 1.0 - deltaReceivedNow/(double)deltaExpectedNow;
        //if (lossFractionNow < 0.0) lossFractionNow = 0.0; //reordering can cause
        if (lossFractionNow < m_dPacket_loss_fraction_min)
        {
            m_dPacket_loss_fraction_min = lossFractionNow;
        }
        if (lossFractionNow > m_dPacket_loss_fraction_max)
        {
            m_dPacket_loss_fraction_max = lossFractionNow;
        }
    }

    printQOSData();

}


void qosMeasurementRecord::printQOSData()
{
    qosData* pData = new qosData();
    pData->nTotNumPacketsReceived = m_nTotNumPacketsReceived;
    pData->nTotNumPacketsLost = int(m_nTotNumPacketsExpected - m_nTotNumPacketsReceived);

    unsigned secsDiff = m_tvMeasurementEnd.tv_sec - m_tvMeasurementStart.tv_sec;
    int usecsDiff = m_tvMeasurementEnd.tv_usec - m_tvMeasurementStart.tv_usec;


    pData->tsTime = wxTimeSpan(0,0,secsDiff,usecsDiff/1000.0);
    double dTime = secsDiff + (secsDiff/1000000.0);

    pData->dkBytesTotal = m_dkBytesTotal;

    if (m_dkbits_per_second_max == 0)
    {
        pData->dkbits_per_second_min = 0;
        pData->dkbits_per_second_max = 0;
        pData->dkbits_per_second_Av = 0;
    }
    else
    {
        pData->dkbits_per_second_min = m_dkbits_per_second_min;
        pData->dkbits_per_second_Av = (dTime == 0.0 ? 0.0 : 8*m_dkBytesTotal/dTime);
        pData->dkbits_per_second_max = m_dkbits_per_second_max;
    }

    pData->dPacket_loss_fraction_min = 100*m_dPacket_loss_fraction_min;

    double packetLossFraction = m_nTotNumPacketsExpected == 0 ? 1.0 : 1.0 - m_nTotNumPacketsReceived/(double)m_nTotNumPacketsExpected;
    if (packetLossFraction < 0.0)
    {
        packetLossFraction = 0.0;
    }
    pData->dPacket_loss_fraction_av = 100*packetLossFraction;
    pData->dPacket_loss_fraction_max =  (packetLossFraction == 1.0 ? 100.0 : 100*m_dPacket_loss_fraction_max);

    m_pThread->QosUpdated(pData);

    if(m_pSource)
    {

        RTPReceptionStatsDB::Iterator statsIter(m_pSource->receptionStatsDB());
        // Assume that there's only one SSRC source (usually the case):
        RTPReceptionStats* stats = statsIter.next(True);
        if (stats != NULL)
        {
            pData->dInter_packet_gap_ms_min = stats->minInterPacketGapUS()/1000.0;
            struct timeval totalGaps = stats->totalInterPacketGaps();
            double totalGapsMS = totalGaps.tv_sec*1000.0 + totalGaps.tv_usec/1000.0;
            unsigned totNumPacketsReceived = stats->totNumPacketsReceived();

            pData->dInter_packet_gap_ms_av = (totNumPacketsReceived == 0 ? 0.0 : totalGapsMS/totNumPacketsReceived);
            pData->dInter_packet_gap_ms_max = stats->maxInterPacketGapUS()/1000.0;

            pData->nBaseExtSeqNum = stats->baseExtSeqNumReceived();
            pData->nLastResetExtSeqNum = stats->lastResetExtSeqNumReceived();
            pData->nHighestExtSeqNum = stats->highestExtSeqNumReceived();
            pData->nJitter = stats->jitter();
            pData->nLastSR_NTPmsw = stats->lastReceivedSR_NTPmsw();
            pData->nLastSR_NTPlsw = stats->lastReceivedSR_NTPlsw();
            pData->tvLastSR_Time = stats->lastReceivedSR_time();
            pData->nSSRC = stats->SSRC();
        }


    }
}
