#pragma once

#include "liveMedia.hh"
#include "BasicUsageEnvironment.hh"
#include "streamclientstate.h"
#include "ourRTSPClient.h"
#include "wxsink.h"

class RtpThread;

class qosMeasurementRecord;

static unsigned nextQOSMeasurementUSecs;




// RTSP 'response handlers':
//void continueAfterDESCRIBE(RTSPClient* rtspClient, int resultCode, char* resultString);
//void continueAfterSETUP(RTSPClient* rtspClient, int resultCode, char* resultString);
void continueAfterPLAY(RTSPClient* rtspClient, int resultCode, char* resultString);

void saveAfterDESCRIBE(RTSPClient* rtspClient, int resultCode, char* resultString);

// Other event handler functions:
void subsessionAfterPlaying(void* clientData); // called when a stream's subsession (e.g., audio or video substream) ends
void subsessionByeHandler(void* clientData); // called when a RTCP "BYE" is received for a subsession
void streamTimerHandler(void* clientData);
// called at the end of a stream's expected duration (if the stream has not already signaled its end using a RTCP "BYE")



// Used to iterate through each stream's 'subsessions', setting up each one:
//void setupNextSubsession(RTSPClient* rtspClient);

// Used to shut down and close a stream (including its "RTSPClient" object):
void shutdownStream(RTSPClient* rtspClient, int exitCode = 1);

void beginQOSMeasurement(UsageEnvironment& env, MediaSession* session, RtpThread* pThread);
static void periodicQOSMeasurement(UsageEnvironment& env, void* clientData); // forward
static void scheduleNextQOSMeasurement();

