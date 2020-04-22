#pragma once
#include <queue>
#include <wx/thread.h>
#include "AudioInputDevice.hh"
#include <wx/event.h>
#include "lame/lame.h"

class timedbuffer;

class MP3LiveSource: public AudioInputDevice
{
    public:

        static MP3LiveSource* createNew(wxEvtHandler* pHandler, wxMutex& mutex, UsageEnvironment& env);



        protected:
            friend class RtpServerThread;

            void AddSamples(const timedbuffer* pTimedBuffer);
            void FlushQueue();
            unsigned int GetPreferredFrameSize()
            {
                return m_nPreferredFrameSize;
            }

            MP3LiveSource(wxEvtHandler* pHandler, wxMutex& mutex, UsageEnvironment& env;
            virtual ~MP3LiveSource();

            bool InitCodec();
        private:
            // redefined virtual functions:
            virtual void doGetNextFrame();
            virtual void doStopGettingFrames();
            virtual Boolean setInputPort(int portIndex);
            virtual double getAverageLevel() const;

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

            lame_t m_pLame;
            unsigned int m_nFrameSize;
            unsigned int m_nSamplesPerFrame;
};


