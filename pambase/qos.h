#pragma once
#include "liveMedia.hh"
#include <wx/datetime.h>
#include "session.h"

class RtpThread;



class qosMeasurementRecord
{
public:
    qosMeasurementRecord(struct timeval const& startTime, RTPSource* src,UsageEnvironment& env, RtpThread* pThread)
        : m_pSource(src), m_pNext(NULL),
          m_pEnv(&env),
          m_pThread(pThread),
		  m_tvMeasurementEnd(startTime),
		  m_tvMeasurementStart(startTime),
          m_dkbits_per_second_min(1e20), m_dkbits_per_second_max(0),
          m_dkBytesTotal(0.0),
		  m_kBytesDeltaNow(0.0),
		  m_dkbpsNow(0.0),
          m_dPacket_loss_fraction_min(1.0), m_dPacket_loss_fraction_max(0.0),
          m_nTotNumPacketsReceived(0), m_nTotNumPacketsExpected(0)
    {
        

        RTPReceptionStatsDB::Iterator statsIter(src->receptionStatsDB());
        // Assume that there's only one SSRC source (usually the case):
        RTPReceptionStats* stats = statsIter.next(True);
        if (stats != NULL)
        {
            m_dkBytesTotal = stats->totNumKBytesReceived();
            m_nTotNumPacketsReceived = stats->totNumPacketsReceived();
            m_nTotNumPacketsExpected = stats->totNumPacketsExpected();
        }
    }
    virtual ~qosMeasurementRecord()
    {
        delete m_pNext;
    }

    void periodicQOSMeasurement(struct timeval const& timeNow);

    void printQOSData();


public:
    RTPSource* m_pSource;
    qosMeasurementRecord* m_pNext;
    UsageEnvironment* m_pEnv;

    RtpThread* m_pThread;
    timeval m_tvMeasurementStart;
    timeval m_tvMeasurementEnd;
    double m_dkbits_per_second_min;
    double m_dkbits_per_second_max;
    double m_dkBytesTotal;
    double m_kBytesDeltaNow;
    double m_dkbpsNow;
    double m_dPacket_loss_fraction_min;
    double m_dPacket_loss_fraction_max;
    unsigned int m_nTotNumPacketsReceived;
    unsigned int m_nTotNumPacketsExpected;


};

//static qosMeasurementRecord* qosRecordHead = NULL;




