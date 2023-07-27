#include "wxsink.h"
#include "rtpthread.h"
#include <wx/log.h>
#ifdef __WXMSW__
#include "Ws2tcpip.h"
#endif

#include "aes67source.h"
#include "smpte2110mediasession.h"

// Even though we're not going to be doing anything with the incoming data, we still need to receive it.
// Define the size of the buffer that we'll use:
#define DUMMY_SINK_RECEIVE_BUFFER_SIZE 100000

using namespace std;

bool operator<(const timeval& t1, const timeval& t2)
{
    return (t1.tv_sec < t2.tv_sec || (t1.tv_sec == t2.tv_sec && t1.tv_usec < t2.tv_usec));
}

wxSink* wxSink::createNew(UsageEnvironment& env, MediaSubsession& subsession, RtpThread* pHandler, char const* streamId)
{
    return new wxSink(env, subsession, pHandler, streamId);
}

wxSink::wxSink(UsageEnvironment& env, MediaSubsession& subsession,RtpThread* pHandler, char const* streamId)
    : MediaSink(env),
      m_pSubsession(dynamic_cast<Smpte2110MediaSubsession*>(&subsession)),
      m_pHandler(pHandler)
{
    fStreamId = strDup(streamId);
    fReceiveBuffer = new u_int8_t[DUMMY_SINK_RECEIVE_BUFFER_SIZE];
}

wxSink::~wxSink()
{
    delete[] fReceiveBuffer;
    delete[] fStreamId;
}

void wxSink::afterGettingFrame(void* clientData, unsigned frameSize, unsigned numTruncatedBytes, struct timeval presentationTime, unsigned durationInMicroseconds)
{
    auto sink = (wxSink*)clientData;

    sink->afterGettingFrame(frameSize, numTruncatedBytes, presentationTime, durationInMicroseconds);
}


void wxSink::afterGettingFrame(unsigned frameSize, unsigned numTruncatedBytes, const timeval& tvPresentation, unsigned durationInMicroseconds)
{
    // We've just received a frame of data.  (Optionally) print out information about it:
    if(strcmp(m_pSubsession->mediumName(), "audio") == 0)
    {
        Aes67Source* pSource = dynamic_cast<Aes67Source*>(m_pSubsession->rtpSource());


        if(m_nLastTimestamp > pSource->GetRTPTimestamp())   //this means we must have crossed an Epoch
        {
            pSource->WorkoutLastEpoch();
        }

        int nDifference = pSource->GetRTPTimestamp()-m_nLastTimestamp;
        m_nLastTimestamp = pSource->GetRTPTimestamp();

        //do we have an associated header ext??
        mExtension_t* mExt = nullptr;
        map<timeval, mExtension_t*>::iterator itExt = m_mExtension.find(tvPresentation);
        if(itExt != m_mExtension.end())
        {
            mExt = itExt->second;
            m_mExtension.erase(itExt); //remove the extension map
        }

        //fSubsession.rtpSource()->hasBeenSynchronizedUsingRTCP();

        unsigned int nBytesPerSample = 0;

        if(strcmp(m_pSubsession->codecName(),"L16") == 0)
        {
            nBytesPerSample = 2;
        }
        else if(strcmp(m_pSubsession->codecName(),"L24") == 0)
        {
            nBytesPerSample = 3;
        }
        if(nBytesPerSample != 0)
        {
            auto pFrame = std::make_shared<rtpFrame>();
            pFrame->sEndpoint = m_pSubsession->GetEndpoint();
            pFrame->sGroup = m_pSubsession->GetGroup();
            pFrame->nSSRC = pSource->lastReceivedSSRC();
            pFrame->timePresentation = tvPresentation;
            pFrame->timeTransmission = pSource->GetTransmissionTime();
            pFrame->nFrameSize = frameSize;
            pFrame->pBuffer = fReceiveBuffer;
            pFrame->nBytesPerSample = nBytesPerSample;
            pFrame->nTimestamp = pSource->GetRTPTimestamp();
            pFrame->nTimestampDifference = nDifference;
            pFrame->mExt = mExt;

            timeval tvSub;
            int nFramesPerSec = (m_pSubsession->rtpTimestampFrequency()*m_pSubsession->GetChannelGrouping().size()*pFrame->nBytesPerSample)/pFrame->nFrameSize;
            timersub(&pFrame->timePresentation, &pFrame->timeTransmission, &tvSub);
            double dTSDF = (static_cast<double>(tvSub.tv_sec)*1000000.0)+tvSub.tv_usec;

            if(m_dDelay0 == std::numeric_limits<double>::lowest() || m_nTSDFCount == nFramesPerSec)
            {
                m_dTSDF = m_dTSDFMax-m_dTSDFMin;
                m_dDelay0 = dTSDF;

                m_dTSDFMax = std::numeric_limits<double>::lowest();
                m_dTSDFMin = std::numeric_limits<double>::max();

                m_nTSDFCount = 1;

            }
            else
            {
                dTSDF -= m_dDelay0;
                m_dTSDFMax = max(m_dTSDFMax, dTSDF);
                m_dTSDFMin = min(m_dTSDFMin, dTSDF);
                m_nTSDFCount++;
            }
            pFrame->dTSDF = m_dTSDF;
            timersub(&(pFrame->timePresentation), &(pFrame->timeTransmission), &pFrame->timeLatency);
            m_pHandler->AddFrame(pFrame);
        }
    }

    continuePlaying();
}

Boolean wxSink::continuePlaying()
{
    if (fSource != NULL)
    {
        Aes67Source* pSource = dynamic_cast<Aes67Source*>(fSource);
        if(pSource)
        {
            //pSource->setRtpExtHdrCallback(rtpExtensionCallback, this); @TODO ??
        }
            // Request the next frame of data from our input source.  "afterGettingFrame()" will get called later, when it arrives:
        fSource->getNextFrame(fReceiveBuffer, DUMMY_SINK_RECEIVE_BUFFER_SIZE,
                              afterGettingFrame, this,
                              onSourceClosure, this);
        return True;
    }
    else
    {
         return False; // sanity check (should not happen)
    }
}


void wxSink::rtpExtensionCallback(unsigned int nDefinedByProfile, unsigned char* pExtHdrData, unsigned nExtHdrDataLen, struct timeval& presentationTime, unsigned short nRtpSeqNo, unsigned nRtpTimestamp, bool bRtpMarkerBitSet)
{

    // the DefinedByProfile should be 0xBEDE
    if(nDefinedByProfile == 0xBEDE) //one byte extension
    {
        unsigned long nPlace(0);

        map<timeval, mExtension_t*>::iterator itExt = m_mExtension.insert(make_pair(presentationTime, new mExtension_t())).first;

        //now we have 4 bits are id, then 4 bit of length, then payload
        while(nPlace < nExtHdrDataLen)
        {
            unsigned char nId = (pExtHdrData[nPlace] >> 4);
            unsigned long nBytes = (pExtHdrData[nPlace] & 0xF);
            ++nPlace;

            //extract the data and store it ready for when the rest of the frame arrives
            unsigned char* pData = new unsigned char[nBytes];
            memcpy(pData, &pExtHdrData[nPlace], nBytes);
            itExt->second->insert(make_pair(nId, pData));


            nPlace += nBytes; //move along to the next id
        }
    }

}

void wxSink::rtpExtensionCallback(unsigned int definedByProfile, unsigned char* extHdrData, unsigned extHdrDataLen, struct timeval& presentationTime, unsigned short rtpSeqNo, unsigned rtpTimestamp, bool rtpMarkerBitSet, void* pPriv)
{
    wxSink* pSink = (wxSink*)pPriv;
    pSink->rtpExtensionCallback(definedByProfile, extHdrData, extHdrDataLen, presentationTime, rtpSeqNo, rtpTimestamp, rtpMarkerBitSet);
}
