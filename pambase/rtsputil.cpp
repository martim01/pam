#include "rtsputil.h"
#include "rtpthread.h"
#include "timedbuffer.h"
#include "qos.h"
#include <wx/log.h>
#include <iostream>
#include "smpte2110mediasession.h"
#include "SIPClient.hh"
#include "log.h"

UsageEnvironment& operator<<(UsageEnvironment& env, const RTSPClient& rtspClient)
{
    return env << "[URL:\"" << rtspClient.url() << "\"]: ";
}

//// A function that outputs a string that identifies each subsession (for debugging output).  Modify this if you wish:
UsageEnvironment& operator<<(UsageEnvironment& env, const MediaSubsession& subsession)
{
    return env << subsession.sessionId() << ":" <<  subsession.mediumName() << "/" << subsession.codecName();
}

std::stringstream& operator<<(std::stringstream& ss, const MediaSubsession& subsession)
{
    ss << subsession.sessionId() << ":" <<  subsession.mediumName() << "/" << subsession.codecName();
    return ss;
}


void continueAfterOPTIONS(RTSPClient* rtspClient, int resultCode, char* resultString)
{
    if(resultCode == 0 && rtspClient != nullptr)
    {
        UsageEnvironment& env = rtspClient->envir(); // alias

        std::string sOptions(resultString);
        pmlLog() << "RTP Client\tRTSP Server supports: "<< resultString;
        delete[] resultString;



        if(sOptions.find("PLAY") != std::string::npos && sOptions.find("SETUP") != std::string::npos)
        {
            rtspClient->sendDescribeCommand(continueAfterDESCRIBE);
        }
        else
        {
            pmlLog()  << "RTP Client\tPLAY or SETUP not supported, use SDP only.";
            rtspClient->sendDescribeCommand(saveAfterDESCRIBE);
        }
    }
    else
    {
        pmlLog(pml::LOG_ERROR) << "RTP Client\tFailed to get option command for client";
    }
}

void continueAfterDESCRIBE(RTSPClient* rtspClient, int resultCode, char* resultString)
{
    bool bSuccess(true);
    UsageEnvironment& env = rtspClient->envir(); // alias
    StreamClientState& scs = ((ourRTSPClient*)rtspClient)->scs; // alias


    if (resultCode != 0)
    {
        pmlLog(pml::LOG_ERROR) << "RTP Client\tFailed to get an SDP description: " << resultString;
        delete[] resultString;
        bSuccess = false;
    }
    else
    {
        char* const sdpDescription = resultString;

        // Create a media session object from this SDP description:
        scs.session = Smpte2110MediaSession::createNew(env, sdpDescription);
        delete[] sdpDescription; // because we don't need it anymore
        if (scs.session == NULL)
        {
            pmlLog(pml::LOG_ERROR)  << "RTP Client\tFailed to create a MediaSession object from the SDP description: " << env.getResultMsg();
            bSuccess = false;
        }
        else if (!scs.session->hasSubsessions())
        {
            pmlLog(pml::LOG_WARN)  << "RTP Client\tThis session has no media subsessions (i.e., no \"m=\" lines)";
            bSuccess = false;
        }
        else
        {
            scs.iter = new MediaSubsessionIterator(*scs.session);
            setupNextSubsession(rtspClient);
        }
    }
    if(!bSuccess)
    {
        // An unrecoverable error occurred with this stream.
        pmlLog(pml::LOG_TRACE) << "continueAfterDescribe\t" << "Shutdown stream";
        shutdownStream(rtspClient);
    }
}


void saveAfterDESCRIBE(RTSPClient* rtspClient, int resultCode, char* resultString)
{
    bool bSuccess(true);
    ourRTSPClient* pClient = dynamic_cast<ourRTSPClient*>(rtspClient);
    if(pClient)
    {
        pClient->GetHandler()->SaveSDP(resultCode, std::string(resultString));
    }
    else
    {
        pmlLog(pml::LOG_TRACE) << "saveAfterDescribe\t" << "Shutdown stream";
        shutdownStream(rtspClient);
        rtspClient = nullptr;
    }
    delete[] resultString;
}




void setupNextSubsession(RTSPClient* rtspClient)
{
    UsageEnvironment& env = rtspClient->envir(); // alias
    StreamClientState& scs = ((ourRTSPClient*)rtspClient)->scs; // alias

    auto pSession = dynamic_cast<Smpte2110MediaSession*>(scs.session);

    scs.subsession = scs.iter->next();
    if (scs.subsession != NULL)
    {
        
        auto pSubsession = dynamic_cast<Smpte2110MediaSubsession*>(scs.subsession);

        if((pSession->GetGroups().find(pSubsession->GetGroup()) != pSession->GetGroups().end())) // duplicate streams
        {
            if (!pSubsession->initiate())
            {
                pmlLog(pml::LOG_ERROR)  << "RTP Client\tFailed to initiate the \"" << *pSubsession << "\" subsession: " << env.getResultMsg();;
                setupNextSubsession(rtspClient); // give up on this subsession; go to the next one
            }
            else
            {
                pmlLog(pml::LOG_DEBUG) << "RTP Client\tInitiated the \"" <<pSubsession->sessionId()<<  "\" subsession (";
                if (pSubsession->rtcpIsMuxed())
                {
                    pmlLog(pml::LOG_DEBUG) << "client port " << pSubsession->clientPortNum();
                }
                else
                {
                    pmlLog(pml::LOG_DEBUG) << "client ports " << pSubsession->clientPortNum() << "-" << pSubsession->clientPortNum()+1;
                }
                pmlLog(pml::LOG_DEBUG) << "  SubsessionId: " << pSubsession->sessionId();


                // Continue setting up this subsession, by sending a RTSP "SETUP" command:
                rtspClient->sendSetupCommand(*pSubsession, continueAfterSETUP, False, False);
            }
            return;
        }
        else
        {
            setupNextSubsession(rtspClient); // give up on this subsession; go to the next one
        }
    }

    if(scs.sFirstSubSessionId.empty() == false)
    {   //managed to setup at least 1 subsession
        auto pClient = dynamic_cast<ourRTSPClient*>(rtspClient);
        if(pClient)
        {
            pClient->GetHandler()->PassSessionDetails(pSession);

            // We've finished setting up all of the subsessions.  Now, send a RTSP "PLAY" command to start the streaming:
            scs.duration = scs.session->playEndTime() - scs.session->playStartTime();
            //Start the first subsession
            pClient->PlaySubsession(scs.sFirstSubSessionId, continueAfterPLAY);
        }
    }
    else
    {
        pmlLog(pml::LOG_ERROR) << "RTP Client\tFailed to create any subsessions";

    }
}

void continueAfterSETUP(RTSPClient* rtspClient, int resultCode, char* resultString)
{
    do
    {
        UsageEnvironment& env = rtspClient->envir(); // alias
        StreamClientState& scs = ((ourRTSPClient*)rtspClient)->scs; // alias

        if (resultCode != 0)
        {
            pmlLog(pml::LOG_ERROR) << "RTP Client\tFailed to set up the \"" << *scs.subsession << "\" subsession: " << resultString;
            break;
        }

        pmlLog(pml::LOG_DEBUG) << "RTP Client\tSet up the \"" << *scs.subsession << "\" subsession (";
        if (scs.subsession->rtcpIsMuxed())
        {
            pmlLog(pml::LOG_DEBUG) << "client port " << scs.subsession->clientPortNum();
        }
        else
        {
            pmlLog(pml::LOG_DEBUG) << "client ports " << scs.subsession->clientPortNum() << "-" << scs.subsession->clientPortNum()+1;
        }
        pmlLog(pml::LOG_DEBUG) << ")";

        // Having successfully setup the subsession, create a data sink for it, and call "startPlaying()" on it.
        // (This will prepare the data sink to receive data; the actual flow of data from the client won't start happening until later,
        // after we've sent a RTSP "PLAY" command.)

        ourRTSPClient* pClient = dynamic_cast<ourRTSPClient*>(rtspClient);
        scs.subsession->sink = wxSink::createNew(env, *scs.subsession, pClient->GetHandler(), rtspClient->url());
        // perhaps use your own custom "MediaSink" subclass instead
        if (scs.subsession->sink == NULL)
        {
            pmlLog(pml::LOG_ERROR) << "RTP Client\tFailed to create a data sink for the \"" << *scs.subsession
                << "\" subsession: " << env.getResultMsg();
            break;
        }

        pmlLog(pml::LOG_DEBUG) << "RTP Client\tCreated a data sink for the \"" << *scs.subsession << "\" subsession";
        scs.subsession->miscPtr = rtspClient; // a hack to let subsession handler functions get the "RTSPClient" from the subsession
        scs.subsession->sink->startPlaying(*(scs.subsession->readSource()), subsessionAfterPlaying, scs.subsession);

        // Begin periodic QOS measurements:
        beginQOSMeasurement(env, ((ourRTSPClient*)rtspClient)->scs.session, ((ourRTSPClient*)rtspClient)->GetHandler());

        // Also set a handler to be called if a RTCP "BYE" arrives for this subsession:
        if (scs.subsession->rtcpInstance() != NULL)
        {
            scs.subsession->rtcpInstance()->setByeHandler(subsessionByeHandler, scs.subsession);
        }

        if(scs.sFirstSubSessionId.empty())
        {
            scs.sFirstSubSessionId = scs.subsession->sessionId();
        }

    }while (0);
    delete[] resultString;

    // Set up the next subsession, if any:

    // @todo decide how we are going to let user choose different subsessions
    setupNextSubsession(rtspClient);
}


void continueAfterPLAY(RTSPClient* rtspClient, int resultCode, char* resultString)
{

    Boolean success = False;

    UsageEnvironment& env = rtspClient->envir(); // alias
    StreamClientState& scs = ((ourRTSPClient*)rtspClient)->scs; // alias

    if (resultCode != 0)
    {
        pmlLog(pml::LOG_ERROR) << "RTP Client\tFailed to start playing session: " << resultString;
    }
    else
    {
        pmlLog(pml::LOG_DEBUG) << "RTP Client\tContinue after play: " << resultString;

        // Set a timer to be handled at the end of the stream's expected duration (if the stream does not already signal its end
        // using a RTCP "BYE").  This is optional.  If, instead, you want to keep the stream active - e.g., so you can later
        // 'seek' back within it and do another RTSP "PLAY" - then you can omit this code.
        // (Alternatively, if you don't want to receive the entire stream, you could set this timer for some shorter value.)
        if (scs.duration > 0)
        {
            unsigned const delaySlop = 2; // number of seconds extra to delay, after the stream's expected duration.  (This is optional.)
            scs.duration += delaySlop;
            unsigned uSecsToDelay = (unsigned)(scs.duration*1000000);
            scs.streamTimerTask = env.taskScheduler().scheduleDelayedTask(uSecsToDelay, (TaskFunc*)streamTimerHandler, rtspClient);
        }


        success = True;
    }
    delete[] resultString;


    if (!success)
    {
        // An unrecoverable error occurred with this stream.
        pmlLog(pml::LOG_TRACE) <<  "continueAfterPlay\t" << "Shutdown stream";
        shutdownStream(rtspClient);
        rtspClient = nullptr;
    }
    else
    {

    }
}


// Implementation of the other event handlers:

void subsessionAfterPlaying(void* clientData)
{
    MediaSubsession* subsession = (MediaSubsession*)clientData;
    RTSPClient* rtspClient = (RTSPClient*)(subsession->miscPtr);

    // Begin by closing this subsession's stream:
    Medium::close(subsession->sink);
    subsession->sink = NULL;

    // Next, check whether *all* subsessions' streams have now been closed:
    MediaSession& session = subsession->parentSession();
    MediaSubsessionIterator iter(session);
    while ((subsession = iter.next()) != NULL)
    {
        if (subsession->sink != NULL) return; // this subsession is still active
    }

    // All subsessions' streams have now been closed, so shutdown the client:
    pmlLog(pml::LOG_TRACE) <<  "subsessionAfterPlay\t" << "Shutdown stream";
    shutdownStream(rtspClient);
    rtspClient = nullptr;
}

void subsessionByeHandler(void* clientData)
{
    MediaSubsession* subsession = (MediaSubsession*)clientData;
    RTSPClient* rtspClient = (RTSPClient*)subsession->miscPtr;
    UsageEnvironment& env = rtspClient->envir(); // alias

    pmlLog(pml::LOG_DEBUG) << "RTP Client\tReceived RTCP \"BYE\" on \"" << *subsession << "\" subsession";

    // Now act as if the subsession had closed:
    subsessionAfterPlaying(subsession);
}

void streamTimerHandler(void* clientData)
{
    ourRTSPClient* rtspClient = (ourRTSPClient*)clientData;
    StreamClientState& scs = rtspClient->scs; // alias

    scs.streamTimerTask = NULL;

    // Shut down the stream:
    pmlLog(pml::LOG_TRACE) <<  "streamTimerHandler\t" << "Shutdown stream";
    shutdownStream(rtspClient);
    rtspClient = nullptr;
}

void shutdownStream(RTSPClient* rtspClient, int exitCode)
{
    if(rtspClient)
    {
        UsageEnvironment& env = rtspClient->envir(); // alias
        StreamClientState& scs = ((ourRTSPClient*)rtspClient)->scs; // alias

        pmlLog(pml::LOG_DEBUG) << "RTP Client\tshutdownStream Entry";

        // First, check whether any subsessions have still to be closed:
        if (scs.session != NULL)
        {
            Boolean someSubsessionsWereActive = False;
            MediaSubsessionIterator iter(*scs.session);
            MediaSubsession* subsession;

            while ((subsession = iter.next()) != NULL)
            {
                if (subsession->sink != NULL)
                {
                    Medium::close(subsession->sink);
                    subsession->sink = NULL;

                    if (subsession->rtcpInstance() != NULL)
                    {
                        subsession->rtcpInstance()->setByeHandler(NULL, NULL); // in case the server sends a RTCP "BYE" while handling "TEARDOWN"
                    }

                    someSubsessionsWereActive = True;
                }
            }

            if (someSubsessionsWereActive)
            {
                pmlLog(pml::LOG_INFO) << "RTP Client\tsome sessions active";
                // Send a RTSP "TEARDOWN" command, to tell the server to shutdown the stream.
                // Don't bother handling the response to the "TEARDOWN".
                rtspClient->sendTeardownCommand(*scs.session, NULL);
            }
        }

        pmlLog(pml::LOG_INFO) << "RTP Client\tClosing the stream.";



        if(exitCode != 0)   //0 means thread called close
        {
            // Note that this will also cause this stream's "StreamClientState" structure to get reclaimed.
            ourRTSPClient* pClient = reinterpret_cast<ourRTSPClient*>(rtspClient);
            if(pClient && pClient->GetHandler())
            {
                pClient->GetHandler()->StreamShutdown();
                pClient->GetHandler()->SetToClose();

            }
        }
        Medium::close(rtspClient);

        pmlLog(pml::LOG_INFO) << "RTP Client\tClosed the stream.";
    }
}



void beginQOSMeasurement(UsageEnvironment& env, MediaSession* session, RtpThread* pThread)
{
    pmlLog(pml::LOG_INFO) << "RTP Client\tBegin QOS";

    // Set up a measurement record for each active subsession:
    struct timeval startTime;
    gettimeofday(&startTime, NULL);
    nextQOSMeasurementUSecs = startTime.tv_sec*1000000 + startTime.tv_usec;

    MediaSubsessionIterator iter(*session);
    MediaSubsession* subsession;
    while ((subsession = iter.next()) != NULL)
    {
        auto pSubsession = dynamic_cast<Smpte2110MediaSubsession*>(subsession);
        if(pSubsession)
        {
            pSubsession->SetHandler(pThread);
            pSubsession->ScheduleNextQOSMeasurement();
        }
    }
}




