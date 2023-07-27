#include "qos.h"
#include "rtsputil.h"
#include "timedbuffer.h"
#include "rtpthread.h"
#include <wx/log.h>
#include <iostream>
#include "smpte2110mediasession.h"
#include "log.h"

qosMeasurementRecord::qosMeasurementRecord(Smpte2110MediaSubsession* pSubsession) : 
    m_pSubsession(pSubsession)
{
    
    gettimeofday(&m_tvMeasurementStart, nullptr);
    gettimeofday(&m_tvMeasurementEnd, nullptr);
    
    if(m_pSubsession->rtpSource())
    {
        RTPReceptionStatsDB::Iterator statsIter(m_pSubsession->rtpSource()->receptionStatsDB());
        // Assume that there's only one SSRC source (usually the case):
        RTPReceptionStats* stats = statsIter.next(True);
        if (stats != NULL)
        {
            m_dkBytesTotal = stats->totNumKBytesReceived();
            m_nTotNumPacketsReceived = stats->totNumPacketsReceived();
            m_nTotNumPacketsExpected = stats->totNumPacketsExpected();
        }
    }
}

void qosMeasurementRecord::periodicQOSMeasurement(struct timeval const& timeNow)
{
    pmlLog(pml::LOG_DEBUG) << "qosMeasurementRecord::periodicQOSMeasurement";
    if(!m_pSubsession || !m_pSubsession->rtpSource() || !m_pThread)
    {
        return;
    }

    unsigned secsDiff = timeNow.tv_sec - m_tvMeasurementEnd.tv_sec;
    int usecsDiff = timeNow.tv_usec - m_tvMeasurementEnd.tv_usec;
    double timeDiff = secsDiff + usecsDiff/1000000.0;
    m_tvMeasurementEnd = timeNow;

    RTPReceptionStatsDB::Iterator statsIter(m_pSubsession->rtpSource()->receptionStatsDB());
    // Assume that there's only one SSRC source (usually the case):
    RTPReceptionStats* stats = statsIter.next(True);
    if (stats != NULL)
    {
        double kBytesTotalNow = stats->totNumKBytesReceived();
        m_kBytesDeltaNow = kBytesTotalNow - m_dkBytesTotal;
        m_dkBytesTotal = kBytesTotalNow;

        m_dkbpsNow = timeDiff == 0.0 ? 0.0 : 8*m_kBytesDeltaNow/timeDiff;
        if (m_dkbpsNow < 0.0) m_dkbpsNow = 0.0; // in case of roundoff error
        if (m_dkbpsNow < m_dkbits_per_second_min)
        {
            m_dkbits_per_second_min = m_dkbpsNow;
        }
        if (m_dkbpsNow > m_dkbits_per_second_max)
        {
            m_dkbits_per_second_max = m_dkbpsNow;
        }

        unsigned totReceivedNow = stats->totNumPacketsReceived();
        unsigned totExpectedNow = stats->totNumPacketsExpected();
        unsigned deltaReceivedNow = totReceivedNow - m_nTotNumPacketsReceived;
        unsigned deltaExpectedNow = totExpectedNow - m_nTotNumPacketsExpected;
        m_nTotNumPacketsReceived = totReceivedNow;
        m_nTotNumPacketsExpected = totExpectedNow;

        double lossFractionNow = deltaExpectedNow == 0 ? 0.0 : 1.0 - deltaReceivedNow/(double)deltaExpectedNow;
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
    unsigned long nTime = secsDiff + (secsDiff/1000000.0);

    pData->dkBytesTotal = m_dkBytesTotal;

    if (m_dkbits_per_second_max == 0)
    {
        pData->dkbits_per_second_min = 0;
        pData->dkbits_per_second_max = 0;
        pData->dkbits_per_second_Av = 0;
        pData->dkbits_per_second_Now = 0;
    }
    else
    {
        pData->dkbits_per_second_min = m_dkbits_per_second_min;
        pData->dkbits_per_second_Av = (pData->tsTime.GetMilliseconds() == 0.0 ? 0.0 : 8000.0*m_dkBytesTotal/pData->tsTime.GetMilliseconds().ToDouble());
        pData->dkbits_per_second_max = m_dkbits_per_second_max;
        pData->dkbits_per_second_Now = m_dkbpsNow;
    }

    pData->dPacket_loss_fraction_min = 100*m_dPacket_loss_fraction_min;

    double packetLossFraction = m_nTotNumPacketsExpected == 0 ? 1.0 : 1.0 - m_nTotNumPacketsReceived/(double)m_nTotNumPacketsExpected;
    if (packetLossFraction < 0.0)
    {
        packetLossFraction = 0.0;
    }
    pData->dPacket_loss_fraction_av = 100*packetLossFraction;
    pData->dPacket_loss_fraction_max =  (packetLossFraction == 1.0 ? 100.0 : 100*m_dPacket_loss_fraction_max);

    if(m_pSubsession->rtpSource())
    {
        RTPReceptionStatsDB::Iterator statsIter(m_pSubsession->rtpSource()->receptionStatsDB());
        // Assume that there's only one SSRC source (usually the case):
        RTPReceptionStats* stats = statsIter.next(True);
        if (stats != NULL)
        {
            pData->dInter_packet_gap_ms_Now = stats->currentInterPacketGapUS()/1000.0;
            pData->dInter_packet_gap_ms_min = stats->minInterPacketGapUS()/1000.0;
            auto  totalGaps = stats->totalInterPacketGaps();
            auto  totalGapsMS = totalGaps.tv_sec*1000.0 + totalGaps.tv_usec/1000.0;
            auto totNumPacketsReceived = stats->totNumPacketsReceived();

            pData->dInter_packet_gap_ms_av = (totNumPacketsReceived == 0 ? 0.0 : totalGapsMS/totNumPacketsReceived);
            pData->dInter_packet_gap_ms_max = stats->maxInterPacketGapUS()/1000.0;

            pData->nBaseExtSeqNum = stats->baseExtSeqNumReceived();
            pData->nLastResetExtSeqNum = stats->lastResetExtSeqNumReceived();
            pData->nHighestExtSeqNum = stats->highestExtSeqNumReceived();
            pData->dJitter = static_cast<double>(stats->jitter())/static_cast<double>(m_pSubsession->rtpSource()->timestampFrequency());
            pData->dJitter *= 1000.0;   //into ms not seconds
            pData->nLastSR_NTPmsw = stats->lastReceivedSR_NTPmsw();
            pData->nLastSR_NTPlsw = stats->lastReceivedSR_NTPlsw();
            pData->tvLastSR_Time = stats->lastReceivedSR_time();
            pData->nSSRC = stats->SSRC();

            pData->tvSync.tv_sec = pData->nLastSR_NTPmsw - 0x83AA7E80; // 1/1/1900 -> 1/1/1970
            double microseconds = (pData->nLastSR_NTPlsw*15625.0)/0x04000000; // 10^6/2^32
            pData->tvSync.tv_usec = (unsigned)(microseconds+0.5);

        }
    }
    pData->sStream = m_pSubsession->GetGroup();
    m_pThread->QosUpdated(pData);
}


unsigned long qosMeasurementRecord::GetQosMeasurementIntervalMS() const
{
    if(m_pThread)
    {
        return m_pThread->GetQosMeasurementIntervalMS();
    }
    return 5000;
}