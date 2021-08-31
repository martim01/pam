#include "mp3livesource.h"
#include <GroupsockHelper.hh> // for "gettimeofday()"
#ifdef __WXWINDOWS__
#include "lame.h"
#endif // __WXWINDOWS__
#ifdef __WXGNU__
#include "lame/lame.h"
#endif // __WXGNU__
#include <iostream>
#include <chrono>

EventTriggerId MP3LiveSource::s_eventTriggerId = 0;
unsigned int MP3LiveSource::s_referenceCount = 0;

MP3LiveSource* MP3LiveSource::createNew(MP3Encoder& encoder, UsageEnvironment& env)
{
    MP3LiveSource* pSource = new MP3LiveSource(encoder, env);


    return pSource;
}



MP3LiveSource::MP3LiveSource(MP3Encoder& encoder,UsageEnvironment& env) : FramedSource(env), m_encoder(encoder), m_nRegistrationId(-1)
{
    env << "Start Unicast Streaming MP3\n";
    fPresentationTime.tv_sec = 0;
    fPresentationTime.tv_usec = 0;

    if (s_referenceCount == 0)
    {
        // Any global initialization of the device would be done here:
        //%%% TO BE WRITTEN %%%
    }
    ++s_referenceCount;

}


MP3LiveSource::~MP3LiveSource()
{
    m_encoder.Unregister(m_nRegistrationId);

    --s_referenceCount;
    if (s_referenceCount == 0)
    {
        envir().taskScheduler().deleteEventTrigger(s_eventTriggerId);
        s_eventTriggerId = 0;
    }
}

void MP3LiveSource::doGetNextFrame()
{
    doReadFromQueue();
}


void MP3LiveSource::doStopGettingFrames()
{
  envir().taskScheduler().unscheduleDelayedTask(nextTask());
}

void MP3LiveSource::doReadFromQueue()
{
    if(m_nRegistrationId == -1)
    {
        m_nRegistrationId = m_encoder.RegisterForFrames();
    }

    if(m_nRegistrationId != -1)
    {
        mp3frame frame(m_encoder.GetNextFrame(m_nRegistrationId));

        fFrameSize = frame.vData.size();

        if(frame.vData.size() > fMaxSize || frame.vData.size() == 0)
        {
            fDurationInMicroseconds = 0;
        }
        else
        {
            memcpy(fTo, frame.vData.data(), frame.vData.size());
            fDurationInMicroseconds = m_encoder.GetFrameDuration();
        }

        if (fPresentationTime.tv_sec == 0 && fPresentationTime.tv_usec == 0)
        {
            // This is the first frame, so use the current time:
            gettimeofday(&fPresentationTime, 0);

        }
        else
        {
            unsigned int uSeconds = fPresentationTime.tv_usec + fDurationInMicroseconds;
            fPresentationTime.tv_sec += uSeconds/1000000;   //any overflow from useds
            fPresentationTime.tv_usec = uSeconds%1000000;
        }
    }
    else
    {
        fDurationInMicroseconds = 0;
        envir() << "No frames to output!\n";
    }
    nextTask() = envir().taskScheduler().scheduleDelayedTask(0, (TaskFunc*)FramedSource::afterGetting, this);
}



void MP3LiveSource::AddToTimedBuffer(float dSample)
{
//    if(m_nBufferWritten == m_pAudioBuffer->GetBufferSize())
//    {
//        m_pAudioBuffer->SetTransmissionTime(std::make_pair(fPresentationTime.tv_sec, fPresentationTime.tv_usec));
//        m_pAudioBuffer->SetBufferDepth(m_qBuffer.size()/fNumChannels);
//        m_pAudioBuffer->SetDuration(m_pAudioBuffer->GetBufferSize()*4);
//        AudioEvent event(m_pAudioBuffer, AudioEvent::OUTPUT, m_pAudioBuffer->GetBufferSize()/fNumChannels, fSamplingFrequency, 0,0);
//        wxPostEvent(m_pHandler, event);
//        m_nBufferWritten = 0;
//        m_pAudioBuffer = new timedbuffer(m_nLastBufferSize);
//    }
//    m_pAudioBuffer->GetWritableBuffer()[m_nBufferWritten] = dSample;
//    ++m_nBufferWritten;
}



