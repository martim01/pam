#pragma once
#include "liveMedia.hh"
#include <wx/datetime.h>
#include "session.h"

class RtpThread;
class Smpte2110MediaSubsession;

class qosMeasurementRecord
{
public:
    explicit qosMeasurementRecord(Smpte2110MediaSubsession* pSubsession);
    virtual ~qosMeasurementRecord(){}
    
    void periodicQOSMeasurement(struct timeval const& timeNow);

    void printQOSData();

    void SetHandler(RtpThread* pThread) { m_pThread = pThread;}

    unsigned long GetQosMeasurementIntervalMS() const;

public:
    

    timeval m_tvMeasurementStart;
    timeval m_tvMeasurementEnd;
    double m_dkbits_per_second_min = 1e20;
    double m_dkbits_per_second_max = 0.0;
    double m_dkBytesTotal = 0.0;
    double m_kBytesDeltaNow = 0.0;
    double m_dkbpsNow = 0.0;
    double m_dPacket_loss_fraction_min = 1.0;
    double m_dPacket_loss_fraction_max = 0.0;
    unsigned int m_nTotNumPacketsReceived = 0;
    unsigned int m_nTotNumPacketsExpected = 0;
    
private:
    Smpte2110MediaSubsession* m_pSubsession;
    //qosMeasurementRecord* m_pNext = nullptr;
    RtpThread* m_pThread = nullptr;

};

//static qosMeasurementRecord* qosRecordHead = NULL;




