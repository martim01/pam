#pragma once

#include "mediasession.hh"

class PamMediaSession: public MediaSession {
public:
  static PamMediaSession* createNew(UsageEnvironment& env,
				 char const* sdpDescription);


protected:
  PamMediaSession(UsageEnvironment& env);
      // called only by createNew();
  virtual ~PamMediaSession();

  virtual PamMediaSubsession* createNewMediaSubsession();


protected:
  friend class MediaSubsessionIterator;

};


class PamMediaSubsession {

protected:
  friend class MediaSession;
  friend class MediaSubsessionIterator;
  MediaSubsession(MediaSession& parent);
  virtual ~MediaSubsession();

  UsageEnvironment& env() { return fParent.envir(); }
  void setNext(MediaSubsession* next) { fNext = next; }

  void setAttribute(char const* name, char const* value = NULL, Boolean valueIsHexadecimal = False);

  Boolean parseSDPLine_c(char const* sdpLine);
  Boolean parseSDPLine_b(char const* sdpLine);
  Boolean parseSDPAttribute_rtpmap(char const* sdpLine);
  Boolean parseSDPAttribute_rtcpmux(char const* sdpLine);
  Boolean parseSDPAttribute_control(char const* sdpLine);
  Boolean parseSDPAttribute_range(char const* sdpLine);
  Boolean parseSDPAttribute_fmtp(char const* sdpLine);
  Boolean parseSDPAttribute_source_filter(char const* sdpLine);
  Boolean parseSDPAttribute_x_dimensions(char const* sdpLine);
  Boolean parseSDPAttribute_framerate(char const* sdpLine);

  virtual Boolean createSourceObjects(int useSpecialRTPoffset);
    // create "fRTPSource" and "fReadSource" member objects, after we've been initialized via SDP

protected:
  // Linkage fields:
  MediaSession& fParent;
  MediaSubsession* fNext;

  // Fields set from a SDP description:
  char* fConnectionEndpointName; // may also be set by RTSP SETUP response
  unsigned short fClientPortNum; // in host byte order
      // This field is also set by initiate()
  unsigned char fRTPPayloadFormat;
  char* fSavedSDPLines;
  char* fMediumName;
  char* fCodecName;
  char* fProtocolName;
  unsigned fRTPTimestampFrequency;
  Boolean fMultiplexRTCPWithRTP;
  char* fControlPath; // holds optional a=control: string
  struct in_addr fSourceFilterAddr; // used for SSM
  unsigned fBandwidth; // in kilobits-per-second, from b= line

  double fPlayStartTime;
  double fPlayEndTime;
  char* fAbsStartTime;
  char* fAbsEndTime;
  unsigned short fVideoWidth, fVideoHeight;
     // screen dimensions (set by an optional a=x-dimensions: <w>,<h> line)
  unsigned fVideoFPS;
     // frame rate (set by an optional "a=framerate: <fps>" or "a=x-framerate: <fps>" line)
  unsigned fNumChannels;
     // optionally set by "a=rtpmap:" lines for audio sessions.  Default: 1
  float fScale; // set from a RTSP "Scale:" header
  float fSpeed;
  double fNPT_PTS_Offset; // set by "getNormalPlayTime()"; add this to a PTS to get NPT
  HashTable* fAttributeTable; // for "a=fmtp:" attributes.  (Later an array by payload type #####)

  // Fields set or used by initiate():
  Groupsock* fRTPSocket; Groupsock* fRTCPSocket; // works even for unicast
  RTPSource* fRTPSource; RTCPInstance* fRTCPInstance;
  FramedSource* fReadSource;
  Boolean fReceiveRawMP3ADUs, fReceiveRawJPEGFrames;

  // Other fields:
  char* fSessionId; // used by RTSP
};

