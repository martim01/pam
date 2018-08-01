#pragma once
#include <wx/event.h>
#include <queue>
#include "portaudio.h"
#include <list>
#include <wx/datetime.h>
#include "dlldefine.h"
#include "freq_mag.h"
#include "timedbuffer.h"

class SoundFile;



/** Class the reads in stereo audio from the sound card and compares the A and B leg to see if they sound the same. This class uses portaudio to read in the audio and pHash to do the comparison
**/
class PAMBASE_IMPEXPORT Audio  : public wxEvtHandler
{
public:
    /** Constructor
    *   @param pHandler the wxEvtHandler to send the results of the comparison to
    *   @param nDevice the id of the sound card input
    **/
    Audio(wxEvtHandler* pHandler, unsigned int nDevice, int nType);

    enum {INPUT, OUTPUT, DUPLEX};
    ///< Destructor
    ~Audio();

    /** Initializes portaudio
    **/
    bool Init(unsigned int nSampleRate);

    unsigned int GetDevice() const
    {
        return m_nDevice;
    }

    wxString GetDeviceName() const;

    unsigned int GetBitDepth() const;

    void SetPeakCutoff(double ddB)
    {
        m_dPeakdB = ddB;
    }
    /** Function called by portaudio when it has received a buffer worth of audio. Checks whether enough audio has been collected and if so does the comparison
    *   @param pBuffer pointer to the buffer containing the audio samples
    *   @param nFrameCount the number of audio frames (2 x number of samples)
    **/
    void InputCallback(const float* pBuffer, size_t nFrameCount, int nFlags);
    void OutputCallback(float* pBuffer, size_t nFrameCount, double dLatency, int nFlags);

    void RecordAudio(bool bRecord);

    void SetGain(int nGain);


    void SetBufferSize(size_t nFrames);
//    freq_mag GetResult();
 //   void ClearResult();

    double GetLastPeak(int nType);

    unsigned int GetInputNumberOfChannels();
    unsigned int GetOutputNumberOfChannels();
    unsigned int GetSampleRate();


    void SetMixer(const std::vector<char>& vChannels, unsigned int nTotalChannels);
    bool IsStreamOpen();
    void FlushBuffer();
    double GetInputLatency();
    double GetOutputLatency();

    void AddSamples(const timedbuffer* pTimedBuffer);
    const std::vector<char>& GetOutputChannels();



private:

    /** Works out the offset between the A and B legs and starts a HashThread to do the actual comparison
    **/
    void Frequency();

    /** Attempts to open a portaudio stream on the given device to read in audio
    *   @param streamCallback function pointer to the callback function
    *   @return <i>bool</i> true on success
    **/
    bool OpenStream(PaStreamCallback *streamCallback);

    void CloseFile();


    wxEvtHandler* m_pManager;           ///< the wxEvtHandler that receives event messages from this class

    PaStream* m_pStream;                ///< pointer to the PaStream tha reads in the audio

    int m_nDevice;
    int m_nType;
    double m_dGain;
    double m_dPeakdB;
    bool m_bRecord;

    wxDateTime m_dtFile;
    const PaDeviceInfo* m_pInfo;

    unsigned int m_nSampleRate;
    unsigned int m_nBufferSize;

    unsigned int m_nChannelsIn;
    unsigned int m_nChannelsOut;


    wxMutex m_mutex;
    pairTime_t ConvertDoubleToPairTime(double dTime);
    double ConvertPairTimeToDouble(const pairTime_t& tv);

    struct timedSample
    {
        timedSample(){}
        timedSample(double dT, double dP, unsigned int nT, float dS) : dTransmission(dT), dPresentation(dP), nTimestamp(nT), dSample(dS){}
        double dTransmission;
        double dPresentation;
        unsigned int nTimestamp;
        float dSample;
    };

    std::queue<timedSample> m_qBuffer;
    std::vector<char> m_vMixer;

    bool m_bDone;

    size_t m_nBuffer;
    bool m_bFirst;

    bool m_bOpen;
    bool m_bClose;
    double m_dLatency;

    unsigned int m_nTotalChannels;

    bool m_bPlaying;
};

/** PaStreamCallback function - simply calls wmComparitor::Callback using userData to get the wmComparitor object
**/
int paCallback( const void *input, void *output, unsigned long frameCount, const PaStreamCallbackTimeInfo* timeInfo, PaStreamCallbackFlags statusFlags, void *userData );
//int paCallbackB( const void *input, void *output, unsigned long frameCount, const PaStreamCallbackTimeInfo* timeInfo, PaStreamCallbackFlags statusFlags, void *userData );



