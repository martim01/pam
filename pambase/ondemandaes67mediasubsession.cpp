#include "ondemandaes67mediasubsession.h"
#include "liveaudiosource.h"
#include <sstream>
#include "wxsink.h"
#include "wxptp.h"
#include "log.h"
#include "PamUsageEnvironment.h"
#include "RTCPTransmissionEvent.h"
#include "settings.h"
static OnDemandAES67MediaSubsession* g_session;

AES67RTPSink::AES67RTPSink(UsageEnvironment& env, Groupsock* RTPgs, unsigned int nFrameSize, unsigned char nRtpPayload,
unsigned char nChannels, const std::string& sFormat, unsigned short nSampleRate) :
AudioRTPSink(env, RTPgs, nRtpPayload, nSampleRate, sFormat.c_str(), nChannels)
{
    setPacketSizes(nFrameSize, nFrameSize);
}

AES67RTPSink::~AES67RTPSink()
{

    pmlLog(pml::LOG_DEBUG) << "~AES67RTPSink" ;
    //@todo tell higher up that this sink has finished so we can remove the RTCP stats (or at least mark as done)
}

AES67RTPSink* AES67RTPSink::createNew(UsageEnvironment& env, Groupsock* RTPgs, unsigned int nFrameSize, unsigned char nRtpPayload,
unsigned char nChannels, const std::string& sFormat, unsigned short nSampleRate)
{
    return new AES67RTPSink(env, RTPgs, nFrameSize, nRtpPayload, nChannels, sFormat, nSampleRate);
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


        m_sAux = ss.str();
    }
    return m_sAux.c_str();
}

unsigned int AES67RTPSink::GetEpochTimestamp()
{
    u_int32_t nTimestamp = presetNextTimestamp();

    //get the current time
    timeval tvNow;
    gettimeofday(&tvNow, 0);

    u_int32_t nTimestampIncrement = (rtpTimestampFrequency()*tvNow.tv_sec);
    nTimestampIncrement += (u_int32_t)(rtpTimestampFrequency()*(tvNow.tv_usec/1000000.0) + 0.5); // note: rounding


    u_int32_t nBaseTimestamp = (nTimestamp - nTimestampIncrement);

    double dNow = static_cast<double>(tvNow.tv_sec);//*1000000.0;
    dNow += static_cast<double>(tvNow.tv_usec) / 1000000.0;

    int64_t nTimestampsToMinus = static_cast<int64_t>(dNow*static_cast<double>(rtpTimestampFrequency()));

    nTimestampsToMinus = nTimestampsToMinus % 42949672960;

    unsigned int nEpoch = nTimestamp-static_cast<unsigned int>(nTimestampsToMinus);

    return 0;


    //return nEpoch;
}


//u_int32_t RTPSink::convertToRTPTimestamp(struct timeval tv)
//{

  // Begin by converting from "struct timeval" units to RTP timestamp units:






OnDemandAES67MediaSubsession* OnDemandAES67MediaSubsession::createNew(wxEvtHandler* pHandler, PamUsageEnvironment& env, unsigned char nNumChannels,unsigned char nRtpPayload,
LiveAudioSource::enumPacketTime ePacketTime, unsigned char nBitsPerSample, unsigned short nSampleRate, portNumBits initialPortNum)
{
    return new OnDemandAES67MediaSubsession(pHandler, env, nNumChannels, nRtpPayload, ePacketTime, nBitsPerSample, nSampleRate, initialPortNum);
}

OnDemandAES67MediaSubsession::OnDemandAES67MediaSubsession(wxEvtHandler* pHandler, PamUsageEnvironment& env, unsigned char nNumChannels,
unsigned char nRtpPayload,  unsigned char nBitsPerSample, unsigned short nSampleRate, LiveAudioSource::enumPacketTime ePacketTime, portNumBits initialPortNum ) :
    OnDemandPamSubsession(pHandler, env, initialPortNum),
  m_nNumberOfChannels(nNumChannels),
  m_nRtpPayload(nRtpPayload),
  m_ePacketTime(ePacketTime),
  m_nBitsPerSample(nBitsPerSample),
  m_nSampleRate(nSampleRate),
  m_pSource(nullptr),
  m_pRTCP(nullptr),
  m_bQos(false)
{

}
OnDemandAES67MediaSubsession::~OnDemandAES67MediaSubsession()
{

}


FramedSource* OnDemandAES67MediaSubsession::createNewStreamSource(unsigned clientSessionId, unsigned& estBitrate)
{
    estBitrate = 2250;
    m_pSource = LiveAudioSource::createNew(m_pAudioHandler, m_mutex, envir(), m_nNumberOfChannels, m_ePacketTime, m_nBitsPerSample, m_nSampleRate);
    return m_pSource;
}


RTPSink* OnDemandAES67MediaSubsession::createNewRTPSink(Groupsock* rtpGroupsock, unsigned char rtpPayloadTypeIfDynamic, FramedSource* inputSource)
{
    std::string sFormat = "L"+std::to_string(m_nBitsPerSample);

    m_pSink =  AES67RTPSink::createNew(envir(), rtpGroupsock, m_pSource->GetPreferredFrameSize()+12, m_nRtpPayload, m_nNumberOfChannels, sFormat, m_nSampleRate);

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
//        pmlLog() << "OnDemandAES67MediaSubsession::sdpLines" ;
//        GetSDP();
//        fSDPLines =new char[m_sSDP.size()+1];
//        strcpy(fSDPLines, m_sSDP.c_str());
//    }
//    pmlLog() << "OnDemandAES67MediaSubsession::sdpLines: " << fSDPLines ;
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

void QOSMeasurement(UsageEnvironment& env, void* clientData)
{
    g_session->DoQoS();
}

void ByeHandler(UsageEnvironment& env, void* clientData)
{
    pmlLog() << "Received BYE message" ;
}


void OnDemandAES67MediaSubsession::BeginQOSMeasurement()
{
    pmlLog(pml::LOG_DEBUG) << "RTP Server\tBegin QOS: " << this ;

    if(m_pRTCP && m_pSink && !m_bQos)
    {
        g_session = this;
        m_pRTCP->setRRHandler((TaskFunc*)QOSMeasurement, reinterpret_cast<void*>(this));
        m_pRTCP->setByeHandler((TaskFunc*)ByeHandler, reinterpret_cast<void*>(this));
        m_bQos = true;
    }
}



RTCPInstance* OnDemandAES67MediaSubsession::createRTCP(Groupsock* RTCPgs, unsigned totSessionBW, unsigned char const* cname, RTPSink* sink)
{
    m_pRTCP = OnDemandServerMediaSubsession::createRTCP(RTCPgs,totSessionBW, cname, sink);
    BeginQOSMeasurement();

    return m_pRTCP;
}

//void OnDemandAES67MediaSubsession::ScheduleNextQOSMeasurement()
//{
//
//    m_nQOSMeasurementUSecs += m_nQOSIntervalUSecs*1000;
//    struct timeval timeNow;
//    gettimeofday(&timeNow, NULL);
//    unsigned timeNowUSecs = timeNow.tv_sec*1000000 + timeNow.tv_usec;
//    int usecsToDelay = m_nQOSMeasurementUSecs - timeNowUSecs;
//
//    m_env.taskScheduler().scheduleDelayedTask(usecsToDelay, (TaskFunc*)periodicQOSMeasurement, reinterpret_cast<void*>(this));
//
//}

void OnDemandAES67MediaSubsession::DoQoS()
{
    struct timeval timeNow;
    gettimeofday(&timeNow, NULL);
    if(m_pSink)
    {

        //void getTotalBitrate(unsigned& outNumBytes, double& outElapsedTime)


        RTPTransmissionStatsDB& db = m_pSink->transmissionStatsDB();
        // Assume that there's only one SSRC source (usually the case):
        RTPTransmissionStatsDB::Iterator statsIter(db);
        RTPTransmissionStats* pStats = statsIter.next();
        while (pStats != NULL)
        {
            for(auto pHandler : m_setRTCPHandlers)
            {
                RTCPTransmissionEvent* pEvent = new RTCPTransmissionEvent(pStats, m_pSink->rtpTimestampFrequency());
                wxQueueEvent(pHandler, pEvent);
            }
            pStats = statsIter.next();
        }
    }
}

void OnDemandAES67MediaSubsession::SetChannelMapping(const vector<char> vMapping)
{
    m_pSource->SetChannelMapping(vMapping);
}
