#pragma once
#include "FramedSource.hh"
#include "mp3encoder.h"


class MP3LiveSource : public FramedSource
{
    public:
        static MP3LiveSource* createNew(MP3Encoder& encoder, UsageEnvironment& env);
        static EventTriggerId s_eventTriggerId;


    protected:
        MP3LiveSource(MP3Encoder& encoder, UsageEnvironment& env);
        virtual ~MP3LiveSource();

    private:
        virtual void doGetNextFrame();
        virtual void doStopGettingFrames(); // optional

        void doReadFromQueue();
        void AddToTimedBuffer(float dSample);

        MP3Encoder& m_encoder;
        static unsigned int s_referenceCount; // used to count how many instances of this class currently exist

        int m_nRegistrationId;
};

