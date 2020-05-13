#include "ondemandaes67mediasubsession.h"
#include "liveaudiosource.h"
#include <sstream>
#include "wxsink.h"



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
    return AES67RTPSink::createNew(envir(), rtpGroupsock);
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

/*
char const* OnDemandAES67MediaSubsession::sdpLines()
{
    if (fSDPLines == NULL)
    {
        GetSDP();
        fSDPLines =new char[m_sSDP.size()+1];
        strcpy(fSDPLines, m_sSDP.c_str());
    }
    return fSDPLines;
}
*/

/*
std::string OnDemandAES67MediaSubsession::GetSDP()
{
    if(m_sSDP.empty())
    {

        unsigned estBitrate;
        FramedSource* inputSource = createNewStreamSource(0, estBitrate);
        if (inputSource == NULL) return NULL; // file not found

        struct in_addr dummyAddr;
        dummyAddr.s_addr = 0;
        Groupsock* dummyGroupsock = createGroupsock(dummyAddr, 0);
        unsigned char rtpPayloadType = 96 + trackNumber()-1; // if dynamic
        RTPSink* dummyRTPSink = createNewRTPSink(dummyGroupsock, rtpPayloadType, inputSource);
        if (dummyRTPSink != NULL && dummyRTPSink->estimatedBitrate() > 0)
        {
            estBitrate = dummyRTPSink->estimatedBitrate();
        }

        AddressString groupAddressStr(dummyGroupsock->groupAddress());
        unsigned short portNum = ntohs(dummyGroupsock->port().num());
        unsigned char ttl = dummyGroupsock->ttl();
        char const* mediaType = dummyRTPSink->sdpMediaType();
        char* rtpmapLine = dummyRTPSink->rtpmapLine();

        char const* rangeLine = rangeSDPLine();
        char const* auxSDPLine = dummyRTPSink->auxSDPLine();

        std::stringstream ss;
        ss << "m=" << mediaType << " " << portNum << " RTP/AVP " << (int)rtpPayloadType << "\r\n";
        ss << "c=IN IP4 " << groupAddressStr.val() << "/" << (int)ttl << "\r\n";
        ss << "b=AS:" << estBitrate << "\r\n";
        ss << rtpmapLine;
        ss << rangeLine;
        ss << auxSDPLine;
        ss << "a=ptime:" << m_ePacketTime << "\r\n";
        ss << "a=maxptime:" << m_ePacketTime << "\r\n";
    #ifdef PTPMONKEY
        ss << "a=ts-refclk:ptp=IEEE1588-2008:" << wxPtp::Get().GetMasterClockId(0) << ":0\r\n";
    #else
        ss << "a=ts-refclk:" << "\r\n";
    #endif // PTPMONKEY

        ss << "a=mediaclk:direct=" << GetEpochTimestamp(dummyRTPSink) << "\r\n";
        ss << "a=control:" << trackId() << "\r\n";

        delete[] (char*)rangeLine;
        delete[] rtpmapLine;


        Medium::close(dummyRTPSink);
        delete dummyGroupsock;
        closeStreamSource(inputSource);

        m_sSDP =  ss.str();
    }
    return m_sSDP;
}
*/

/*
int OnDemandAES67MediaSubsession::GetEpochTimestamp(RTPSink* pSink)
{
    int nTimestamp = pSink->presetNextTimestamp();

    //get the current time
    timeval tvNow;
    #ifdef PTPMONKEY
    tvNow = wxPtp::Get().GetPtpTime(0);
    #else
    gettimeofday(&tvNow, 0);
    #endif // PTPMONKEY


    double dNow = static_cast<double>(tvNow.tv_sec);//*1000000.0;
    dNow += static_cast<double>(tvNow.tv_usec) / 1000000.0;

    int64_t nTimestampsToMinus = static_cast<int64_t>(dNow*static_cast<double>(pSink->rtpTimestampFrequency()));

    nTimestampsToMinus = nTimestampsToMinus % 42949672960;

    return nTimestamp-static_cast<int>(nTimestampsToMinus);
}
*/
