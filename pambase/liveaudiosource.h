#pragma once
#include <queue>
#include <wx/thread.h>
#include "AudioInputDevice.hh"
#include <wx/event.h>

class timedbuffer;

class LiveAudioSource: public AudioInputDevice
{
    public:

        enum enumPacketTime{US125 = 125, US250 = 250, US333 = 333, MS1 = 1000, MS4 = 4000};

        static LiveAudioSource* createNew(wxEvtHandler* pHandler, wxMutex& mutex, UsageEnvironment& env, unsigned char nNumChannels, enumPacketTime ePacketTime,
unsigned char nBitsPerSample, unsigned short nSampleRate);



        protected:
            friend class RtpServerThread;
            friend class OnDemandAES67MediaSubsession;

            void AddSamples(const timedbuffer* pTimedBuffer);
            void FlushQueue();
            unsigned int GetPreferredFrameSize()
            {
                return m_nPreferredFrameSize;
            }

            LiveAudioSource(wxEvtHandler* pHandler, wxMutex& mutex, UsageEnvironment& env, unsigned char nNumChannels, enumPacketTime ePacketTime,
unsigned char nBitsPerSample, unsigned short nSampleRate);
            virtual ~LiveAudioSource();
            unsigned int m_nPreferredFrameSize;

        private:
            // redefined virtual functions:
            void doGetNextFrame() override;
            void doStopGettingFrames()  override;
            Boolean setInputPort(int portIndex) override;
            double getAverageLevel() const override;

            void doReadFromQueue();

            void AddToTimedBuffer(float dSample);

            wxEvtHandler* m_pHandler;
            wxMutex& m_mutex;
            double m_dPlayTimePerSample; // useconds
            Boolean m_bFidIsSeekable;
            unsigned int m_nLastPlayTime; // useconds
            Boolean m_bHaveStartedReading;
            Boolean m_bLimitNumBytesToStream;
            unsigned int m_nNumBytesToStream; // used iff "fLimitNumBytesToStream" is True
            unsigned char m_nAudioFormat;


            unsigned int m_nPacketTime;

            std::queue<float> m_qBuffer;

            unsigned long m_nLastBufferSize;
            unsigned long m_nBufferWritten;
            timedbuffer* m_pAudioBuffer;

            static const double TWENTYFOURBIT;
            static const double SIXTEENBIT;
};


