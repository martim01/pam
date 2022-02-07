#include "liveaudiosource.h"
#include "GroupsockHelper.hh"
#include "Groupsock.hh"
#include "timedbuffer.h"
#include <wx/defs.h>
#include <wx/log.h>
#include "audioevent.h"
#ifdef PTPMONKEY
#include "wxptp.h"
#ifdef __GNU__
#include <sys/time.h>
#endif // __GNU__
#endif // PTPMONKEY

#include "log.h"

const double LiveAudioSource::TWENTYFOURBIT = 8388608.0;
const double LiveAudioSource::SIXTEENBIT = 32768.0;

////////// LiveAudioSource //////////

LiveAudioSource* LiveAudioSource::createNew(wxEvtHandler* pHandler, wxMutex& mutex, UsageEnvironment& env,  unsigned char nNumChannels,enumPacketTime ePacketTime,
unsigned char nBitsPerSample, unsigned short nSampleRate)
{
    return new LiveAudioSource(pHandler, mutex, env, nNumChannels, ePacketTime, nBitsPerSample, nSampleRate);
}


LiveAudioSource::LiveAudioSource(wxEvtHandler* pHandler, wxMutex& mutex, UsageEnvironment& env,unsigned char nNumChannels,  enumPacketTime ePacketTime,
unsigned char nBitsPerSample, unsigned short nSampleRate)
  : AudioInputDevice(env, nBitsPerSample, nNumChannels, nSampleRate, 0)/* set the real parameters later */,
    m_pHandler(pHandler),
    m_mutex(mutex),
    m_nLastPlayTime(0),
    m_bLimitNumBytesToStream(False),
    m_nNumBytesToStream(0),
	m_nAudioFormat(0),
    m_nPacketTime(ePacketTime),
    m_nLastBufferSize(8192),
    m_nBufferWritten(0),
    m_pAudioBuffer(0)
    m_vChannelMapping{0,1,2,3,4,5,6,7}
{

    unsigned int nSamplesPerPacket = (fSamplingFrequency/1000)*m_nPacketTime;
    nSamplesPerPacket /= 1000;
    m_nPreferredFrameSize = (nSamplesPerPacket*fNumChannels*fBitsPerSample)/8;

    m_dPlayTimePerSample = 1e6/(double)fSamplingFrequency;

}

LiveAudioSource::~LiveAudioSource()
{
}

void LiveAudioSource::doGetNextFrame()
{
    fFrameSize = 0; // until it's set later
    doReadFromQueue();
}

void LiveAudioSource::doStopGettingFrames()
{
  envir().taskScheduler().unscheduleDelayedTask(nextTask());
}

void LiveAudioSource::doReadFromQueue()
{
    wxMutexLocker lock(m_mutex);

    if (m_nPreferredFrameSize < fMaxSize)
    {
        fMaxSize = m_nPreferredFrameSize;
    }
    unsigned int nBytesPerSample = (fNumChannels*fBitsPerSample)/8;

    unsigned int nBytesToRead = (fMaxSize - fMaxSize%nBytesPerSample);
    unsigned numBytesRead;


    // Set the 'presentation time' and 'duration' of this frame:
    if (fPresentationTime.tv_sec == 0 && fPresentationTime.tv_usec == 0)
    {
        // This is the first frame, so use the current time:
        gettimeofday(&fPresentationTime, 0);

        while(m_qBuffer.empty() == false)
        {
            m_qBuffer.pop();
        }
    }
    else
    {
        // Increment by the play time of the previous data:
        unsigned uSeconds = fPresentationTime.tv_usec + m_nLastPlayTime;
        fPresentationTime.tv_sec += uSeconds/1000000;
        fPresentationTime.tv_usec = uSeconds%1000000;
    }


    if(m_nBufferWritten == 0)
    {
        m_pAudioBuffer = new timedbuffer(m_nLastBufferSize, fNumChannels);
    }

    for(int i = 0; i < nBytesToRead; i+=(fBitsPerSample/8))
    {
        long nValue(0);
        float dValue(0.0);
        if(m_qBuffer.empty() == false)
        {
            dValue = m_qBuffer.front();

            m_qBuffer.pop();
        }
        if(fBitsPerSample == 24)
        {
            nValue = static_cast<long>(dValue*TWENTYFOURBIT);
            fTo[0] = (nValue >> 16) & 0xFF;
            fTo[1] = (nValue >> 8) & 0xFF;
            fTo[2] = nValue & 0xFF;
            fTo+=3;
        }
        else if(fBitsPerSample == 16)
        {
            nValue = static_cast<long>(dValue*SIXTEENBIT);
            fTo[0] = (nValue >> 8) & 0xFF;
            fTo[1] = nValue & 0xFF;
            fTo+=2;
        }
        AddToTimedBuffer(dValue);
    }

    fFrameSize += nBytesToRead;
    fMaxSize -= nBytesToRead;



    // Remember the play time of this data:
    fDurationInMicroseconds = m_nLastPlayTime = (unsigned)((m_dPlayTimePerSample*fFrameSize)/nBytesPerSample);


    // Inform the reader that he has data:
    // To avoid possible infinite recursion, we need to return to the event loop to do this:
    nextTask() = envir().taskScheduler().scheduleDelayedTask(0, (TaskFunc*)FramedSource::afterGetting, this);
}

Boolean LiveAudioSource::setInputPort(int /*portIndex*/)
{
    return True;
}

double LiveAudioSource::getAverageLevel() const
{
  return 0.0;//##### fix this later
}


void LiveAudioSource::AddSamples(const timedbuffer* pTimedBuffer)
{
    wxMutexLocker lg(m_mutex);
    m_nLastBufferSize = 0;
    for(unsigned int i =0; i < pTimedBuffer->GetBufferSize(); i+=pTimedBuffer->GetNumberOfChannels())
    {
        for(unsigned int j = 0; j < fNumChannels; j++)
        {
            if(j < m_vChannelMapping.size())
            {
                m_qBuffer.push(pTimedBuffer->GetBuffer()[i+(m_vChannelMapping[j]%pTimedBuffer->GetNumberOfChannels())]);
            }
            else
            {
                m_qBuffer.push(0.0);
            }
            ++m_nLastBufferSize;
        }
    }
}


void LiveAudioSource::FlushQueue()
{
    wxMutexLocker lg(m_mutex);
    while(m_qBuffer.empty() == false)
    {
        m_qBuffer.pop();
    }
}


void LiveAudioSource::AddToTimedBuffer(float dSample)
{
    if(m_nBufferWritten == m_pAudioBuffer->GetBufferSize())
    {
        m_pAudioBuffer->SetTransmissionTime(fPresentationTime);
        m_pAudioBuffer->SetBufferDepth(m_qBuffer.size()/fNumChannels);
        m_pAudioBuffer->SetDuration(m_pAudioBuffer->GetBufferSize()*4);
        AudioEvent event(m_pAudioBuffer, AudioEvent::OUTPUT, m_pAudioBuffer->GetBufferSize()/fNumChannels, fSamplingFrequency, 0,0);
        wxPostEvent(m_pHandler, event);
        m_nBufferWritten = 0;
        m_pAudioBuffer = new timedbuffer(m_nLastBufferSize, fNumChannels);
    }
    m_pAudioBuffer->GetWritableBuffer()[m_nBufferWritten] = dSample;
    ++m_nBufferWritten;
}

void LiveAudioSource::SetChannelMapping(const vector<char> vMapping)
{
    wxMutexLocker lg(m_mutex);
    m_vChannelMapping = vMapping;
}
