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
    if (fSource == NULL) return False; // sanity check (should not happen)

    // Request the next frame of data from our input source.  "afterGettingFrame()" will get called later, when it arrives:
    fSource->getNextFrame(fReceiveBuffer, DUMMY_SINK_RECEIVE_BUFFER_SIZE,
                          afterGettingFrame, this,
                          onSourceClosure, this);
    return True;
}
