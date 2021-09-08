/**********
This library is free software; you can redistribute it and/or modify it under
the terms of the GNU Lesser General Public License as published by the
Free Software Foundation; either version 3 of the License, or (at your
option) any later version. (See <http://www.gnu.org/copyleft/lesser.html>.)

This library is distributed in the hope that it will be useful, but WITHOUT
ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public License for
more details.

You should have received a copy of the GNU Lesser General Public License
along with this library; if not, write to the Free Software Foundation, Inc.,
51 Franklin Street, Fifth Floor, Boston, MA 02110-1301  USA
**********/
// Copyright (c) 1996-2020, Live Networks, Inc.  All rights reserved
// A test program that streams a MP3 file via RTP/RTCP
// main program

#include "liveMedia.hh"
#include "GroupsockHelper.hh"
#include <iostream>
#include "mp3encoder.h"
#include "BasicUsageEnvironment.hh"

// To stream using 'ADUs' rather than raw MP3 frames, uncomment the following:
//#define STREAM_USING_ADUS 1
// To also reorder ADUs before streaming, uncomment the following:
//#define INTERLEAVE_ADUS 1
// (For more information about ADUs and interleaving,
//  see <http://www.live555.com/rtp-mp3/>)

// To stream using "source-specific multicast" (SSM), uncomment the following:
//#define USE_SSM 1
#ifdef USE_SSM
Boolean const isSSM = True;
#else
Boolean const isSSM = False;
#endif

#include "mp3livesource.h"

// To set up an internal RTSP server, uncomment the following:
//#define IMPLEMENT_RTSP_SERVER 1
// (Note that this RTSP server works for multicast only)

RTSPServer* rtspServer;

UsageEnvironment* env;

// A structure to hold the state of the current session.
// It is used in the "afterPlaying()" function to clean up the session.
struct sessionState_t
{
    FramedSource* source;
    RTPSink* sink;
    RTCPInstance* rtcpInstance;
    Groupsock* rtpGroupsock;
    Groupsock* rtcpGroupsock;
} sessionState;


void play(MP3Encoder& encoder); // forward

void stream(MP3Encoder& encoder)
{

    // Begin by setting up our usage environment:
    TaskScheduler* scheduler = BasicTaskScheduler::createNew();
    env = BasicUsageEnvironment::createNew(*scheduler);

    *env << "Start Stream\n";
    // Create 'groupsocks' for RTP and RTCP:
    char const* destinationAddressStr = "239.255.42.42";

    const unsigned short rtpPortNum = 6666;
    const unsigned short rtcpPortNum = rtpPortNum+1;
    const unsigned char ttl = 1; // low, in case routers don't admin scope

    struct sockaddr_storage destinationAddress;
    ((sockaddr_in&)destinationAddress).sin_addr.s_addr = inet_addr(destinationAddressStr);
    const Port rtpPort(rtpPortNum);
    const Port rtcpPort(rtcpPortNum);

    *env << "Create GroupSocks\n";
    sessionState.rtpGroupsock = new Groupsock(*env, destinationAddress, rtpPort, ttl);
    sessionState.rtcpGroupsock = new Groupsock(*env, destinationAddress, rtcpPort, ttl);
    sessionState.sink = MPEG1or2AudioRTPSink::createNew(*env, sessionState.rtpGroupsock);

    // Create (and start) a 'RTCP instance' for this RTP sink:
    /*
    const unsigned estimatedSessionBandwidth = 160; // in kbps; for RTCP b/w share
    const unsigned maxCNAMElen = 100;
    unsigned char CNAME[maxCNAMElen+1];
    gethostname((char*)CNAME, maxCNAMElen);
    CNAME[maxCNAMElen] = '\0'; // just in case
    sessionState.rtcpInstance = RTCPInstance::createNew(*env, sessionState.rtcpGroupsock,estimatedSessionBandwidth, CNAME, sessionState.sink, NULL /* we're a server *///, isSSM);
    // Note: This starts RTCP running automatically


    *env << "Create RTSP\n";
    rtspServer = RTSPServer::createNew(*env);
    // Note that this (attempts to) start a server on the default RTSP server
    // port: 554.  To use a different port number, add it as an extra
    // (optional) parameter to the "RTSPServer::createNew()" call above.
    if (rtspServer == NULL)
    {
        *env << "Failed to create RTSP server: " << env->getResultMsg() << "\n";
        return;
    }
    *env << "Create ServerMediaSession\n";
    ServerMediaSession* sms  = ServerMediaSession::createNew(*env, "testStream", "test", "Session streamed by \"testMP3Streamer\"", isSSM);
    sms->addSubsession(PassiveServerMediaSubsession::createNew(*sessionState.sink, sessionState.rtcpInstance));
    rtspServer->addServerMediaSession(sms);

    char* url = rtspServer->rtspURL(sms);
    *env << "Play this stream using the URL \"" << url << "\"\n";
    delete[] url;
//    getchar();

    play(encoder);

    env->taskScheduler().doEventLoop(); // does not return

}

void afterPlaying(void* clientData); // forward

void play(MP3Encoder& encoder)
{
    // Open the file as a 'MP3 file source':
    sessionState.source = MP3LiveSource::createNew(encoder, *env);
    if (sessionState.source == NULL)
    {
        *env << "Unable to set LAME up\n";
        exit(1);
    }

    // Finally, start the streaming:
    *env << "Beginning streaming...\n";
    sessionState.sink->startPlaying(*sessionState.source, afterPlaying, NULL);
}


void afterPlaying(void* /*clientData*/)
{
    *env << "...done streaming\n";

    sessionState.sink->stopPlaying();

    // End this loop by closing the current source:
    Medium::close(sessionState.source);

}


