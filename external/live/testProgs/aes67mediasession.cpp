#include "aes67mediasession.h"
#include "UsageEnvironment.hh"
#include "aes67source.h"
#include <string>
#include <cmath>

static int gettimeofday(struct timeval * tp, struct timezone * tzp)
{
    // Note: some broken versions only have 8 trailing zero's, the correct epoch has 9 trailing zero's
    // This magic number is the number of 100 nanosecond intervals since January 1, 1601 (UTC)
    // until 00:00:00 January 1, 1970
    static const uint64_t EPOCH = ((uint64_t) 116444736000000000ULL);

    SYSTEMTIME  system_time;
    FILETIME    file_time;
    uint64_t    time;

    GetSystemTime( &system_time );
    SystemTimeToFileTime( &system_time, &file_time );
    time =  ((uint64_t)file_time.dwLowDateTime )      ;
    time += ((uint64_t)file_time.dwHighDateTime) << 32;

    tp->tv_sec  = (long) ((time - EPOCH) / 10000000L);
    tp->tv_usec = (long) (system_time.wMilliseconds * 1000);
    return 0;
}


using namespace std;

Aes67MediaSession* Aes67MediaSession::createNew(UsageEnvironment& env,
				      char const* sdpDescription)
{
    Aes67MediaSession* newSession = new Aes67MediaSession(env);
    if (newSession != NULL)
    {
        if (!newSession->initializeWithSDP(sdpDescription))
        {
            delete newSession;
            return NULL;
        }
    }

  return newSession;
}


Aes67MediaSession::Aes67MediaSession(UsageEnvironment& env) : MediaSession(env)
{

}




MediaSubsession* Aes67MediaSession::createNewMediaSubsession()
{
    return new Aes67MediaSubsession(*this);
}


Aes67MediaSubsession::Aes67MediaSubsession(MediaSession& parent) : MediaSubsession(parent)
{

}

Boolean Aes67MediaSubsession::createSourceObjects(int useSpecialRTPoffset)
{
    if (strcmp(fCodecName, "L16") == 0 || strcmp(fCodecName, "L24") == 0) // 16 or 24-bit linear audio (RFC 3190)
    {
        parseSDPAttribute_Sync();

        char* mimeType = new char[strlen(mediumName()) + strlen(codecName()) + 2] ;
        sprintf(mimeType, "%s/%s", mediumName(), codecName());
        fReadSource = fRTPSource = Aes67Source::createNew(env(), fRTPSocket, fRTPPayloadFormat, fRTPTimestampFrequency, mimeType, 0 ,FALSE, m_nSyncTime);
        delete[] mimeType;

        return TRUE;
    }
    else
    {
        env().setResultMsg("RTP payload format unknown or not supported");
    }

  return False; // an error occurred
}


Boolean Aes67MediaSubsession::parseSDPAttribute_Sync()
{
    string sSdp(fSavedSDPLines);

    size_t nFront = sSdp.find("a=sync-time:");
    if(nFront != sSdp.size())
    {
        size_t nEnd = sSdp.find("\n", nFront);
        nEnd -= nFront;
        string sTime = sSdp.substr(nFront+12, (nEnd-12));

        m_nSyncTime = atoi(sTime.c_str());

        return True;
    }

    m_nSyncTime = 0;
    return False;
}


//void Aes67MediaSubsession::WorkoutLastEpoch()
//{
//    double dTime = static_cast<double>(m_nSyncTime)/static_cast<double>(fRTPTimestampFrequency); //this is number of seconds before Epoch that the stream started...
//    dTime *= 1000000.0;
//
//    //get the current time
//    timeval tvNow;
//    gettimeofday(&tvNow, 0);
//
//    double dNow = static_cast<double>(tvNow.tv_sec)*1000000.0;
//    dNow += tvNow.tv_usec;
//    dNow += dTime;
//
//    //divide answer by 2^32
//    double dSecPerCycle = (4294967296.0/static_cast<double>(fRTPTimestampFrequency));
//    unsigned int nCycles = static_cast<unsigned int>(dNow / dSecPerCycle);
//
//    //now workout when the time was
//    dTime = static_cast<double>(nCycles)*dSecPerCycle;
//
//    double dInt, dDec;
//    dDec = modf(dTime, &dInt);
//
//    m_tvLastEpoch.tv_sec = dInt;
//    m_tvLastEpoch.tv_usec = (dDec*1000000.0);
//
//
//}


//unsigned int Aes67MediaSubsession::GetExpectedTimestamp()
//{
//    timeval tv;
//    gettimeofday(&tv,0);
//
//    return GetExpectedTimestamp(tv);
//
//}

//unsigned int Aes67MediaSubsession::GetExpectedTimestamp(const timeval& tv)
//{
//    double dSec = tv.tv_sec;
//    double dUSec  = tv.tv_usec;
//
//    double dTime = dSec + (dUSec/1000000.0);
//
//    //work out timestamp cycles since then
//    double dCycles = static_cast<double>(fRTPTimestampFrequency)*dTime;
//    //add the epoch timestamp
//    double dTimestamp = static_cast<double>(m_nSyncTime)+dCycles;
//    int64_t nTimestamp = dTimestamp;
//
//    return static_cast<unsigned int>(nTimestamp % 4294967296);
//}
//
Boolean Aes67MediaSubsession::parseSDPAttribute_Domain()
{

}

Boolean Aes67MediaSubsession::parseSDPAttribute_Deviation()
{

}

/*
timeval Aes67MediaSubsession::GetTransmissionTime(unsigned int nRTPTimestamp)
{

    double dSeconds = static_cast<double>(nRTPTimestamp)/static_cast<double>(fRTPTimestampFrequency);

    timeval tvTransmision;
    tvTransmision.tv_sec = m_tvLastEpoch.tv_sec;
    tvTransmision.tv_usec = m_tvLastEpoch.tv_usec;

    double dInt, dDec;
    dDec = modf(dSeconds, &dInt);
    tvTransmision.tv_sec += dInt;
    tvTransmision.tv_usec += dDec*1000000.0;

    return tvTransmision;
/*

    timeval tvTransmission;
    gettimeofday(&tvTransmission,0);

    unsigned int nExpected = GetExpectedTimestamp(tvTransmission);

    double dSamples;
    dSamples = nExpected - nRTPTimestamp;

    //divide the samples by the frequency to give us the number of seconds difference
    dSamples /= static_cast<double>(rtpTimestampFrequency());
    dSamples *= 1000000.0;  //in usec

    int64_t nTime = tvTransmission.tv_sec;
    nTime *= 1000000;
    nTime += tvTransmission.tv_usec;

    nTime -= dSamples;

    tvTransmission.tv_sec = nTime / 1000000;
    tvTransmission.tv_usec = nTime % 1000000;

    return tvTransmission;
}
*/
