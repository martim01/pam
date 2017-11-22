#pragma once

#include "BasicUsageEnvironment0.hh"

class PamTaskScheduler: public BasicTaskScheduler0 {
public:
  static PamTaskScheduler* createNew(unsigned maxSchedulerGranularity = 10000/*microseconds*/);
    // "maxSchedulerGranularity" (default value: 10 ms) specifies the maximum time that we wait (in "select()") before
    // returning to the event loop to handle non-socket or non-timer-based events, such as 'triggered events'.
    // You can change this is you wish (but only if you know what you're doing!), or set it to 0, to specify no such maximum time.
    // (You should set it to 0 only if you know that you will not be using 'event triggers'.)
  virtual ~PamTaskScheduler();

  virtual void doEventLoop(char volatile* watchVariable);

protected:
  PamTaskScheduler(unsigned maxSchedulerGranularity);
      // called only by "createNew()"

  static void schedulerTickTask(void* clientData);
  void schedulerTickTask();

protected:
  // Redefined virtual functions:
  virtual void SingleStep(unsigned maxDelayTime=0);

  virtual void setBackgroundHandling(int socketNum, int conditionSet, BackgroundHandlerProc* handlerProc, void* clientData);
  virtual void moveSocketHandling(int oldSocketNum, int newSocketNum);

protected:
  unsigned fMaxSchedulerGranularity;

  // To implement background operations:
  int fMaxNumSockets;
  fd_set fReadSet;
  fd_set fWriteSet;
  fd_set fExceptionSet;

private:
#if defined(__WIN32__) || defined(_WIN32)
  // Hack to work around a bug in Windows' "select()" implementation:
  int fDummySocketNum;
#endif
};
