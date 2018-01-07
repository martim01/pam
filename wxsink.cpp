#include "wxsink.h"
#include "rtpthread.h"
#include <wx/log.h>
#ifdef __WXMSW__
#include "Ws2tcpip.h"
#endif

#include "aes67source.h"
#include "aes67mediasession.h"

// Even though we're not going to be doing anything with the incoming data, we still need to receive it.
// Define the size of the buffer that we'll use:
#define DUMMY_SINK_RECEIVE_BUFFER_SIZE 100000

using namespace std;

wxSink* wxSink::createNew(UsageEnvironment& env, MediaSubsession& subsession, RtpThread* pHandler, char const* streamId)
{
    return new wxSink(env, subsession, pHandler, streamId);
}

wxSink::wxSink(UsageEnvironment& env, MediaSubsession& subsession,RtpThread* pHandler, char const* streamId)
    : MediaSink(env),
      m_pSubsession(dynamic_cast<Aes67MediaSubsession*>(&subsession)),
      m_pHandler(pHandler)
{
    fStreamId = strDup(streamId);
    fReceiveBuffer = new u_int8_t[DUMMY_SINK_RECEIVE_BUFFER_SIZE];
    m_nLastTimestamp = 0;
}

wxSink::~wxSink()
{
    delete[] fReceiveBuffer;
    delete[] fStreamId;
}

void wxSink::afterGettingFrame(void* clientData, unsigned frameSize, unsigned numTruncatedBytes, struct timeval presentationTime, unsigned durationInMicroseconds)
{
    wxSink* sink = (wxSink*)clientData;
    pairTime_t tvPresentation(make_pair(presentationTime.tv_sec, presentationTime.tv_usec));
    sink->afterGettingFrame(frameSize, numTruncatedBytes, tvPresentation, durationInMicroseconds);
}


void wxSink::afterGettingFrame(unsigned frameSize, unsigned numTruncatedBytes, const pairTime_t& presentationTime, unsigned durationInMicroseconds)
{
    // We've just received a frame of data.  (Optionally) print out information about it:

    if(strcmp(m_pSubsession->mediumName(), "audio") == 0)
    {
        Aes67Source* pSource = dynamic_cast<Aes67Source*>(m_pSubsession->rtpSource());


        if(m_nLastTimestamp > pSource->GetRTPTimestamp())   //this means we must have crossed an Epoch
        {
            pSource->WorkoutLastEpoch();
        }
        m_nLastTimestamp = pSource->GetRTPTimestamp();

        if(strcmp(m_pSubsession->codecName(),"L16") == 0)
        {
            m_pHandler->AddFrame(m_pSubsession->GetEndpoint(),  pSource->lastReceivedSSRC(), presentationTime, frameSize, fReceiveBuffer, 2, pSource->GetTransmissionTime(), pSource->GetRTPTimestamp(),frameSize);
        }
        else if(strcmp(m_pSubsession->codecName(),"L24") == 0)
        {
            m_pHandler->AddFrame(m_pSubsession->GetEndpoint(), pSource->lastReceivedSSRC(), presentationTime, frameSize, fReceiveBuffer, 3, pSource->GetTransmissionTime(), pSource->GetRTPTimestamp(),frameSize);
        }


    }


    continuePlaying();
}

Boolean wxSink::continuePlaying()
{
    if (fSource != NULL)
    {
        Aes67Source* pSource = dynamic_cast<Aes67Source*>(fSource);

        pSource->setRtpExtHdrCallback(rtpExtensionCallback, this);

        // Request the next frame of data from our input source.  "afterGettingFrame()" will get called later, when it arrives:
        pSource->getNextFrame(fReceiveBuffer, DUMMY_SINK_RECEIVE_BUFFER_SIZE,
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
    wxLogDebug(wxT("Extension"));

    // the DefinedByProfile should be 0xBEDE
    if(nDefinedByProfile == 0xBEDE) //one byte extension
    {
        unsigned long nPlace(0);
        //now we have 4 bits are id, then 4 bit of length, then payload
        while(nPlace < nExtHdrDataLen)
        {
            extIdLengthBits_t* pBits = reinterpret_cast<extIdLengthBits_t*>(&pExtHdrData[nPlace]);
            unsigned long nId = pBits->id;
            unsigned long nBytes = (pBits->length)+1;

            ++nPlace;
            //extract the data and tell the thread so it can pass stuff up again

            nPlace += nBytes; //move along to the next id
        }
    }

}

void wxSink::rtpExtensionCallback(unsigned int definedByProfile, unsigned char* extHdrData, unsigned extHdrDataLen, struct timeval& presentationTime, unsigned short rtpSeqNo, unsigned rtpTimestamp, bool rtpMarkerBitSet, void* pPriv)
{
    wxSink* pSink = (wxSink*)pPriv;
    pSink->rtpExtensionCallback(definedByProfile, extHdrData, extHdrDataLen, presentationTime, rtpSeqNo, rtpTimestamp, rtpMarkerBitSet);
}
