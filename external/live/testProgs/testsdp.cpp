#include "liveMedia.hh"
#include "BasicUsageEnvironment.hh"

UsageEnvironment& operator<<(UsageEnvironment& env, const MediaSubsession& subsession)
{
    return env << subsession.mediumName() << "/" << subsession.codecName();
}

#define DUMMY_SINK_RECEIVE_BUFFER_SIZE 100000

class DummySink: public MediaSink
{
    public:
        static DummySink* createNew(UsageEnvironment& env,
                      MediaSubsession& subsession, // identifies the kind of data that's being received
                      char const* streamId = NULL) // identifies the stream itself (optional)
        {
              return new DummySink(env, subsession, streamId);
        }

    private:
        DummySink(UsageEnvironment& env, MediaSubsession& subsession, char const* streamId)
            : MediaSink(env), fSubsession(subsession)
        {
            fStreamId = strDup(streamId);
            fReceiveBuffer = new u_int8_t[DUMMY_SINK_RECEIVE_BUFFER_SIZE];
        }

        virtual ~DummySink()
        {
            delete[] fReceiveBuffer;
            delete[] fStreamId;
        }

        static void afterGettingFrame(void* clientData, unsigned frameSize,
                    unsigned numTruncatedBytes,
                    struct timeval presentationTime,
                    unsigned durationInMicroseconds)
        {
            DummySink* sink = (DummySink*)clientData;
            sink->afterGettingFrame(frameSize, numTruncatedBytes, presentationTime, durationInMicroseconds);
        }
        void afterGettingFrame(unsigned frameSize, unsigned numTruncatedBytes,
                 struct timeval presentationTime, unsigned durationInMicroseconds)
        {
//            if (fStreamId != NULL)
//                envir() << "Stream \"" << fStreamId << "\"; ";
            envir() << fSubsession.mediumName() << "/" << fSubsession.codecName() << ":\tReceived " << frameSize << " bytes";
//            if (numTruncatedBytes > 0)
//                envir() << " (with " << numTruncatedBytes << " bytes truncated)";
//            char uSecsStr[6+1]; // used to output the 'microseconds' part of the presentation time
//            sprintf(uSecsStr, "%06u", (unsigned)presentationTime.tv_usec);
//            envir() << ".\tPresentation time: " << (int)presentationTime.tv_sec << "." << uSecsStr;
//            if (fSubsession.rtpSource() != NULL && !fSubsession.rtpSource()->hasBeenSynchronizedUsingRTCP())
//            {
//                envir() << "!"; // mark the debugging output to indicate that this presentation time is not RTCP-synchronized
//            }
//            envir() << "\tNPT: " << fSubsession.getNormalPlayTime(presentationTime);
//            envir() << "\n";

            // Then continue, to request the next frame of data:
            continuePlaying();
        }

    private:
        virtual Boolean continuePlaying()
        {
            if (fSource == NULL) return False; // sanity check (should not happen)
            fSource->getNextFrame(fReceiveBuffer, DUMMY_SINK_RECEIVE_BUFFER_SIZE, afterGettingFrame, this, onSourceClosure, this);
            return True;
        }

    private:
        u_int8_t* fReceiveBuffer;
        MediaSubsession& fSubsession;
        char* fStreamId;
};

int main(int argc, char** argv)
{
    TaskScheduler* scheduler = BasicTaskScheduler::createNew();
    UsageEnvironment* env = BasicUsageEnvironment::createNew(*scheduler);


    MediaSession* session = MediaSession::createNew(*env, "v=0 \no=- 5 1 IN IP4 10.10.11.200 \ns=Channel 13005 \nc=IN IP4 239.192.50.205 \nt=0 0 \nm=audio 5004 RTP/AVP 96 \na=clock-domain:PTPv2 0 \na=type:multicast \na=rtpmap:96 L24/48000/2 \na=sync-time:2708976114 \n");
    if (session == NULL)
    {
        *env << "Failed to create a MediaSession object from the SDP description: " << env->getResultMsg() << "\n";
        return 1;
    }

    MediaSubsessionIterator iter(*session);
    MediaSubsession* subsession = NULL;
    while ((subsession = iter.next()) != NULL)
    {
        if (!subsession->initiate (0))
        {
            *env << "Failed to initiate the \"" << *subsession << "\" subsession: " << env->getResultMsg() << "\n";
        }
        else
        {
            subsession->sink = DummySink::createNew(*env, *subsession);
            if (subsession->sink == NULL)
            {
                *env << "Failed to create a data sink for the \"" << *subsession << "\" subsession: " << env->getResultMsg() << "\n";
            }
            else
            {
                subsession->sink->startPlaying(*subsession->rtpSource(), NULL, NULL);
            }
        }
    }

    char eventLoopWatchVariable = 0;
    env->taskScheduler().doEventLoop(&eventLoopWatchVariable);

    return 0;
}
