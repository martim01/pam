#include "mp3livesource.h"
#include "GroupsockHelper.hh"
#include "timedbuffer.h"
#include <wx/defs.h>
#include <wx/log.h>
#include "audioevent.h"
#ifdef PTPMONKEY
#include "wxptp.h"
#ifdef __GNU__
#include <sys/time.h>
#endif // __GNU__
#endif // PTPMONKEY



MP3LiveSource* MP3LiveSource::createNew(wxEvtHandler* pHandler, wxMutex& mutex, UsageEnvironment& env)
{
    MP3LiveSource* pSource = new MP3LiveSource(pHandler, mutex, env);
    if(pSource->InitCodec())
    {
        return pSource;
    }
    else
    {
        delete pSource;
        return nullptr;
    }
}


MP3LiveSource::MP3LiveSource(wxEvtHandler* pHandler, wxMutex& mutex, UsageEnvironment& env)
  : AudioInputDevice(env, 24, nNumChannels, 48000, 0)/* set the real parameters later */,
    m_pHandler(pHandler),
    m_mutex(mutex),
    m_nLastPlayTime(0),
    m_bLimitNumBytesToStream(False),
    m_nNumBytesToStream(0),
	m_nAudioFormat(0),
    m_nLastBufferSize(8192),
    m_nBufferWritten(0),
    m_pAudioBuffer(0)
{

    m_dPlayTimePerSample = 1e6/(double)fSamplingFrequency;
}

bool MP3LiveSource::InitCodec()
{
    m_pLame = lame_init();
    lame_set_in_samplerate(m_pLame, 48000);
    lame_set_VBR(m_pLame, vbr_default);
    lame_set_VBR_q(m_pLame, 0);
    if(lame_init_params(m_pLame) < 0)
    {
        return false;
    }
    return true;
}

MP3LiveSource::~MP3LiveSource()
{
}

void MP3LiveSource::doGetNextFrame()
{
    fFrameSize = 0; // until it's set later
    doReadFromQueue();
}

void MP3LiveSource::doStopGettingFrames()
{
  envir().taskScheduler().unscheduleDelayedTask(nextTask());
}

void MP3LiveSource::doReadFromQueue()
{
    wxMutexLocker lock(m_mutex);

    if (m_nPreferredFrameSize < fMaxSize)
    {
        fMaxSize = m_nPreferredFrameSize;
    }
    unsigned int nBytesPerSample = (fNumChannels*fBitsPerSample)/8;


    unsigned numBytesRead;


    // Set the 'presentation time' and 'duration' of this frame:
    if (fPresentationTime.tv_sec == 0 && fPresentationTime.tv_usec == 0)
    {
        // This is the first frame, so use the current time:
        gettimeofday(&fPresentationTime, 0);

        #ifdef PTPMONKEY
        timeval tv = wxPtp::Get().GetPtpOffset(0);
        #ifdef __WXGNU__
        timersub(&fPresentationTime, &tv, &fPresentationTime);
        #else
        double dOffset = tv.tv_sec + (static_cast<double>(tv.tv_usec))/1000000.0;
        double dPresentation = fPresentationTime.tv_sec + (static_cast<double>(fPresentationTime.tv_usec))/1000000.0;
        dPresentation -= dOffset;

        fPresentationTime.tv_sec = static_cast<int>(dPresentation);
        fPresentationTime.tv_usec = (dPresentation-fPresentationTime.tv_sec)*1e6;
        #endif
        #endif // PTPMONKEY

        while(m_qBuffer.empty() == false)
        {
            m_qBuffer.pop();
        }
    }
    else
    {
        // Increment by the play time of the previous data:
        unsigned uSeconds = fPresentationTime.tv_usec + m_nLastPlayTime;
        fPresentationTime.tv_sec += uSeconds/1000000;
        fPresentationTime.tv_usec = uSeconds%1000000;
    }


    if(m_nBufferWritten == 0)
    {
        m_pAudioBuffer = new timedbuffer(m_nLastBufferSize);
    }

    lame_encode_buffer_interleaved_ieee_float(m_pLame, m_qBuffer.)
    for(int i = 0; i < m_nFrameSize; i+=3)
    {
        long nValue(0);
        float dValue(0.0);
        if(m_qBuffer.empty() == false)
        {
            dValue = m_qBuffer.front();

            m_qBuffer.pop();
        }



        nValue = static_cast<long>(dValue*TWENTYFOURBIT);

        fTo[0] = (nValue >> 16) & 0xFF;
        fTo[1] = (nValue >> 8) & 0xFF;
        fTo[2] = nValue & 0xFF;
        fTo+=3;
        AddToTimedBuffer(dValue);
    }

    fFrameSize += nBytesToRead;
    fMaxSize -= nBytesToRead;


    // Remember the play time of this data:
    fDurationInMicroseconds = m_nLastPlayTime = (unsigned)((m_dPlayTimePerSample*fFrameSize)/nBytesPerSample);



    // Inform the reader that he has data:
    // To avoid possible infinite recursion, we need to return to the event loop to do this:
    nextTask() = envir().taskScheduler().scheduleDelayedTask(0, (TaskFunc*)FramedSource::afterGetting, this);
}

Boolean MP3LiveSource::setInputPort(int /*portIndex*/)
{
    return True;
}

double MP3LiveSource::getAverageLevel() const
{
  return 0.0;//##### fix this later
}


void MP3LiveSource::AddSamples(const timedbuffer* pTimedBuffer)
{
    for(unsigned int i =0; i < pTimedBuffer->GetBufferSize(); i++)
    {
        m_qBuffer.push(pTimedBuffer->GetBuffer()[i]);
    }
    m_nLastBufferSize = pTimedBuffer->GetBufferSize();
}


void MP3LiveSource::FlushQueue()
{
    while(m_qBuffer.empty() == false)
    {
        m_qBuffer.pop();
    }
}


void MP3LiveSource::AddToTimedBuffer(float dSample)
{
    if(m_nBufferWritten == m_pAudioBuffer->GetBufferSize())
    {
        m_pAudioBuffer->SetTransmissionTime(std::make_pair(fPresentationTime.tv_sec, fPresentationTime.tv_usec));
        m_pAudioBuffer->SetBufferDepth(m_qBuffer.size()/fNumChannels);
        m_pAudioBuffer->SetDuration(m_pAudioBuffer->GetBufferSize()*4);
        AudioEvent event(m_pAudioBuffer, AudioEvent::OUTPUT, m_pAudioBuffer->GetBufferSize()/fNumChannels, fSamplingFrequency, 0,0);
        wxPostEvent(m_pHandler, event);
        m_nBufferWritten = 0;
        m_pAudioBuffer = new timedbuffer(m_nLastBufferSize);
    }
    m_pAudioBuffer->GetWritableBuffer()[m_nBufferWritten] = dSample;
    ++m_nBufferWritten;
}
