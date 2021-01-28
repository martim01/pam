#include "ondemandaes67mediasubsession.h"
#include "liveaudiosource.h"
#include <sstream>
#include "wxsink.h"
#include "wxptp.h"
#include "log.h"
#include "PamUsageEnvironment.h"

static OnDemandAES67MediaSubsession* g_session;

AES67RTPSink::AES67RTPSink(UsageEnvironment& env, Groupsock* RTPgs) : AudioRTPSink(env, RTPgs, 96, 48000, "L24", 2)
{
}

AES67RTPSink::~AES67RTPSink()
{
}

AES67RTPSink* AES67RTPSink::createNew(UsageEnvironment& env, Groupsock* RTPgs)
{
    return new AES67RTPSink(env, RTPgs);
}


char const* AES67RTPSink::auxSDPLine()
{
    if(m_sAux.empty())
    {
        std::stringstream ss;

        #ifdef PTPMONKEY
            ss << "a=ts-refclk:ptp=IEEE1588-2008:" << wxPtp::Get().GetMasterClockId(0).ToStdString() << ":0\r\n";
        #else
            ss << "a=ts-refclk:" << "\r\n";
        #endif // PTPMONKEY

        ss << "a=mediaclk:direct=" << GetEpochTimestamp() << "\r\n";

        std::cout << "auxSDPLine: " << ss.str() << std::endl;

        m_sAux = ss.str();
    }
    return m_sAux.c_str();
}

unsigned int AES67RTPSink::GetEpochTimestamp()
{
    u_int32_t nTimestamp = presetNextTimestamp();

    //get the current time
    timeval tvNow;
    #ifdef PTPMONKEY
    tvNow = wxPtp::Get().GetPtpTime(0);
    #else
    gettimeofday(&tvNow, 0);
    #endif // PTPMONKEY

    u_int32_t nTimestampIncrement = (rtpTimestampFrequency()*tvNow.tv_sec);
    nTimestampIncrement += (u_int32_t)(rtpTimestampFrequency()*(tvNow.tv_usec/1000000.0) + 0.5); // note: rounding


    u_int32_t nBaseTimestamp = (nTimestamp - nTimestampIncrement);

    double dNow = static_cast<double>(tvNow.tv_sec);//*1000000.0;
    dNow += static_cast<double>(tvNow.tv_usec) / 1000000.0;

    int64_t nTimestampsToMinus = static_cast<int64_t>(dNow*static_cast<double>(rtpTimestampFrequency()));

    nTimestampsToMinus = nTimestampsToMinus % 42949672960;

    unsigned int nEpoch = nTimestamp-static_cast<unsigned int>(nTimestampsToMinus);

    return 0;

    //std::cout << "Timestamp: " << nTimestamp << " Base\t: " << nBaseTimestamp << "\tEpoch: " << nEpoch << std::endl;

    //return nEpoch;
}


//u_int32_t RTPSink::convertToRTPTimestamp(struct timeval tv)
//{

  // Begin by converting from "struct timeval" units to RTP timestamp units:






OnDemandAES67MediaSubsession* OnDemandAES67MediaSubsession::createNew(wxEvtHandler* pHandler, PamUsageEnvironment& env, unsigned char nNumChannels,LiveAudioSource::enumPacketTime ePacketTime, portNumBits initialPortNum)
{
    return new OnDemandAES67MediaSubsession(pHandler, env, nNumChannels, ePacketTime, initialPortNum);
}

OnDemandAES67MediaSubsession::OnDemandAES67MediaSubsession(wxEvtHandler* pHandler, PamUsageEnvironment& env, unsigned char nNumChannels, LiveAudioSource::enumPacketTime ePacketTime, portNumBits initialPortNum ) :
    OnDemandPamSubsession(pHandler, env, initialPortNum),
  m_nNumberOfChannels(nNumChannels),
  m_ePacketTime(ePacketTime),
  m_pSource(nullptr)
{

}
OnDemandAES67MediaSubsession::~OnDemandAES67MediaSubsession()
{

}


FramedSource* OnDemandAES67MediaSubsession::createNewStreamSource(unsigned clientSessionId, unsigned& estBitrate)
{
    estBitrate = 2250;
    m_pSource = LiveAudioSource::createNew(m_pHandler, m_mutex, envir(), m_nNumberOfChannels, m_ePacketTime);
    return m_pSource;
}


RTPSink* OnDemandAES67MediaSubsession::createNewRTPSink(Groupsock* rtpGroupsock, unsigned char rtpPayloadTypeIfDynamic, FramedSource* inputSource)
{
    m_pSink =  AES67RTPSink::createNew(envir(), rtpGroupsock);
    BeginQOSMeasurement();
    return m_pSink;
}

void OnDemandAES67MediaSubsession::AddSamples(const timedbuffer* pTimedBuffer)
{
    if(m_pSource && m_nConnections > 0)
    {
        m_pSource->AddSamples(pTimedBuffer);
    }
}

void OnDemandAES67MediaSubsession::FlushQueue()
{
    if(m_pSource)
    {
        m_pSource->FlushQueue();
    }
}


//char const* OnDemandAES67MediaSubsession::sdpLines()
//{
//    if (fSDPLines == NULL)
//    {
//        pml::Log::Get() << "OnDemandAES67MediaSubsession::sdpLines" << std::endl;
//        GetSDP();
//        fSDPLines =new char[m_sSDP.size()+1];
//        strcpy(fSDPLines, m_sSDP.c_str());
//    }
//    pml::Log::Get() << "OnDemandAES67MediaSubsession::sdpLines: " << fSDPLines << std::endl;
//    return fSDPLines;
//}
//
//
//
//std::string OnDemandAES67MediaSubsession::GetSDP()
//{
//    if(m_sSDP.empty())
//    {
//
//        unsigned estBitrate;
//        FramedSource* inputSource = createNewStreamSource(0, estBitrate);
//        if (inputSource == NULL) return NULL; // file not found
//
//        struct in_addr dummyAddr;
//        dummyAddr.s_addr = 0;
//        Groupsock* dummyGroupsock = createGroupsock(dummyAddr, 0);
//        unsigned char rtpPayloadType = 96 + trackNumber()-1; // if dynamic
//        RTPSink* dummyRTPSink = createNewRTPSink(dummyGroupsock, rtpPayloadType, inputSource);
//        if (dummyRTPSink != NULL && dummyRTPSink->estimatedBitrate() > 0)
//        {
//            estBitrate = dummyRTPSink->estimatedBitrate();
//        }
//
//        AddressString groupAddressStr(dummyGroupsock->groupAddress());
//        unsigned short portNum = ntohs(dummyGroupsock->port().num());
//        unsigned char ttl = dummyGroupsock->ttl();
//        char const* mediaType = dummyRTPSink->sdpMediaType();
//        char* rtpmapLine = dummyRTPSink->rtpmapLine();
//
//        char const* rangeLine = rangeSDPLine();
//        char const* auxSDPLine = dummyRTPSink->auxSDPLine();
//
//        std::stringstream ss;
//        ss << "m=" << mediaType << " " << portNum << " RTP/AVP " << (int)rtpPayloadType << "\r\n";
//        ss << "c=IN IP4 " << groupAddressStr.val() << "/" << (int)ttl << "\r\n";
//        ss << "b=AS:" << estBitrate << "\r\n";
//        ss << rtpmapLine;
//        ss << rangeLine;
//        ss << auxSDPLine;
//        ss << "a=ptime:" << m_ePacketTime << "\r\n";
//        ss << "a=maxptime:" << m_ePacketTime << "\r\n";
//    #ifdef PTPMONKEY
//        ss << "a=ts-refclk:ptp=IEEE1588-2008:" << wxPtp::Get().GetMasterClockId(0) << ":0\r\n";
//    #else
//        ss << "a=ts-refclk:" << "\r\n";
//    #endif // PTPMONKEY
//
//        ss << "a=mediaclk:direct=" << GetEpochTimestamp(dummyRTPSink) << "\r\n";
//        ss << "a=control:" << trackId() << "\r\n";
//
//        delete[] (char*)rangeLine;
//        delete[] rtpmapLine;
//
//
//        Medium::close(dummyRTPSink);
//        delete dummyGroupsock;
//        closeStreamSource(inputSource);
//
//        m_sSDP =  ss.str();
//    }
//    return m_sSDP;
//}
//
//

void OnDemandAES67MediaSubsession::BeginQOSMeasurement()
{
    pml::Log::Get(pml::Log::LOG_DEBUG) << "RTP Server\tBegin QOS: " << this << std::endl;

    // Set up a measurement record for each active subsession:
    struct timeval startTime;
    gettimeofday(&startTime, NULL);
    m_nQOSMeasurementUSecs = startTime.tv_sec*1000000 + startTime.tv_usec;

    if(m_pSink)
    {
        g_session = this;
        ScheduleNextQOSMeasurement();
    }
}


void periodicQOSMeasurement(UsageEnvironment& env, void* clientData)
{
    g_session->DoQoS();
}

void OnDemandAES67MediaSubsession::ScheduleNextQOSMeasurement()
{

    m_nQOSMeasurementUSecs += m_nQOSIntervalUSecs*1000;
    struct timeval timeNow;
    gettimeofday(&timeNow, NULL);
    unsigned timeNowUSecs = timeNow.tv_sec*1000000 + timeNow.tv_usec;
    int usecsToDelay = m_nQOSMeasurementUSecs - timeNowUSecs;

    m_env.taskScheduler().scheduleDelayedTask(usecsToDelay, (TaskFunc*)periodicQOSMeasurement, reinterpret_cast<void*>(this));

}

void OnDemandAES67MediaSubsession::DoQoS()
{
    struct timeval timeNow;
    gettimeofday(&timeNow, NULL);
    if(m_pSink)
    {
        RTPTransmissionStatsDB& db = m_pSink->transmissionStatsDB();
        // Assume that there's only one SSRC source (usually the case):
        RTPTransmissionStatsDB::Iterator statsIter(db);
        RTPTransmissionStats* pStats = statsIter.next();
        if (pStats != NULL)
        {
            m_qos.nSR_RRTime = pStats->diffSR_RRTime();
            m_qos.nFirstPacketNumber = pStats->firstPacketNumReported();

            u_int32_t nOctHigh,  nOctLow,  nPacketHigh, nPacketLow;
            pStats->getTotalOctetCount(nOctHigh, nOctLow);
            pStats->getTotalPacketCount(nPacketHigh, nPacketLow);

            m_qos.nOctets = nOctHigh;
            m_qos.nOctets = m_qos.nOctets << 32;
            m_qos.nOctets += nOctLow;

            m_qos.nPackets = nPacketHigh;
            m_qos.nPackets = m_qos.nPackets << 32;
            m_qos.nPackets += nPacketLow;


            m_qos.nJitter = pStats->jitter();

            char addr[256];
            if(pStats->lastFromAddress().sin_family == AF_INET)
            {
                inet_ntop(AF_INET, &pStats->lastFromAddress().sin_addr, &addr[0], 256);
            }
            else if(pStats->lastFromAddress().sin_family == AF_INET6)
            {
                inet_ntop(AF_INET6, &pStats->lastFromAddress().sin_addr, &addr[0], 256);
            }
            m_qos.sLastFromAddress = std::string(addr);


            m_qos.nLastPacketNumber = pStats->lastPacketNumReceived();
            m_qos.nLastSRTime = pStats->lastSRTime();
            m_qos.tvLastTimeReceived = pStats->lastTimeReceived();
            m_qos.nPacketLossRatio = pStats->packetLossRatio();
            m_qos.nPacketsLostBetweenRR = pStats->packetsLostBetweenRR();
            m_qos.nPacketsReceivedSinceLastRR = pStats->packetsReceivedSinceLastRR();
            m_qos.nRoundTripDelay = (static_cast<double>(pStats->roundTripDelay())/65536.0)*1000000;
            m_qos.nSSRC = pStats->SSRC();
            m_qos.tvCreated = pStats->timeCreated();
            m_qos.nTotNumPacketsLost = pStats->totNumPacketsLost();


            if(m_qos.tvLastTimeReceived.tv_sec != m_qos.tvLastButOneTimeReceived.tv_sec ||
              m_qos.tvLastTimeReceived.tv_usec != m_qos.tvLastButOneTimeReceived.tv_usec)
            {
                pml::Log::Get(pml::Log::LOG_DEBUG) << "Address: " << m_qos.sLastFromAddress << "\tSSRC " << m_qos.nSSRC << std::endl;
                pml::Log::Get(pml::Log::LOG_DEBUG) << "Created: " << m_qos.tvCreated.tv_sec << "\tReceived: " << m_qos.tvLastTimeReceived.tv_sec << std::endl;

                pml::Log::Get(pml::Log::LOG_DEBUG) << "Octest: " << m_qos.nOctets << "\tPackets " << m_qos.nPackets << std::endl;
                pml::Log::Get(pml::Log::LOG_DEBUG) << "SR_RR: " << m_qos.nSR_RRTime << "\t" << "First#: " << m_qos.nFirstPacketNumber << "\tJitter: "<< m_qos.nJitter << std::endl;
                pml::Log::Get(pml::Log::LOG_DEBUG) << "Last#: " << m_qos.nLastPacketNumber << "\tLast Time: " << m_qos.nLastSRTime << std::endl;
                pml::Log::Get(pml::Log::LOG_DEBUG) << "LossRatio: " << (int)m_qos.nPacketLossRatio <<"\tLossBetweenRR: " << m_qos.nPacketsLostBetweenRR << "\tRecSinceRR: " << m_qos.nPacketsReceivedSinceLastRR << std::endl;
                pml::Log::Get(pml::Log::LOG_DEBUG) << "RoundTrip: " << m_qos.nRoundTripDelay << "us\tSSRC: " <<  m_qos.nSSRC << "\tTotalLost: " << m_qos.nTotNumPacketsLost << std::endl;

                m_qos.tvLastButOneTimeReceived = m_qos.tvLastTimeReceived;
            }

        }
        // Do this again later:
        ScheduleNextQOSMeasurement();
    }
}


