#include "audio.h"
#include <wx/log.h>
#include "portaudio.h"
#include <wx/msgdlg.h>
#include "soundfile.h"
#include <wx/datetime.h>
#include "timedbuffer.h"
#include "log.h"
#include "settings.h"
#include "audioevent.h"
#include "pa_linux_alsa.h"
#include "settingevent.h"

using namespace std;


Audio::Audio(wxEvtHandler* pHandler, unsigned int nDevice, int nType) :
 m_pManager(pHandler),
 m_pStream(0),
 m_nDevice(nDevice),
 m_nType(nType),
 m_dGain(1),
 m_dPeakdB(-50),
 m_bRecord(false),
 m_pInfo(Pa_GetDeviceInfo(nDevice)),
 m_nSampleRate(48000),
 m_nBufferSize(2048),
 m_nChannelsIn(2),
 m_nChannelsOut(0),
 m_bDone(false),
 m_nBuffer(0),
 m_bFirst(false),
 m_bOpen(false),
 m_bClose(false),
 m_dLatency(0.2),
 m_nTimeStamp(0),
 m_nTotalChannels(2),
 m_bPlaying(false)
 {
    m_vOutputRatio.resize(8);
    for(unsigned int i = 0; i < 8; i++)
    {
        m_vOutputRatio[i] = Settings::Get().Read("Output", wxString::Format("Ratio_%02d", i), 1.0);
        Settings::Get().AddHandler(this, "Output", wxString::Format("Ratio_%02d", i));
    }
    Connect(wxID_ANY, wxEVT_SETTING_CHANGED, (wxObjectEventFunction)&Audio::OnSettingChanged);
 }

bool Audio::Init(unsigned int nSampleRate)
{
    m_nSampleRate = nSampleRate;
    return (OpenStream(paCallback));
}

bool Audio::OpenStream(PaStreamCallback *streamCallback)
{
    pmlLog(pml::LOG_INFO, "pam::soundcard") << "Attempt to open device " << m_nDevice;


    PaStreamParameters inputParameters;
    PaStreamParameters outputParameters;

    const PaDeviceInfo* pInfo = Pa_GetDeviceInfo(m_nDevice);
    if(pInfo)
    {
        if(pInfo->maxInputChannels < 2)
        {
            m_nChannelsIn = pInfo->maxInputChannels;
            pmlLog(pml::LOG_INFO, "pam::soundcard") << "Input channels changed to " << m_nChannelsIn;
        }
        else
        {
            m_nChannelsIn = 2;
        }

        if(pInfo->maxOutputChannels < 2)
        {
            m_nChannelsOut = pInfo->maxInputChannels;
            pmlLog(pml::LOG_INFO, "pam::soundcard") << "Output channels changed to " << m_nChannelsOut;
        }
        else
        {
            m_nChannelsOut = 2;
        }
    }
    else
    {
        m_nChannelsIn = 2;
        m_nChannelsOut = 2;
    }


    inputParameters.channelCount = m_nChannelsIn;
    inputParameters.device = m_nDevice;
    inputParameters.hostApiSpecificStreamInfo = NULL;
    inputParameters.sampleFormat = paFloat32;
    inputParameters.suggestedLatency = 0;
    inputParameters.hostApiSpecificStreamInfo = NULL;

    double dLatency(.04);
    dLatency = Settings::Get().Read(wxT("Output"), wxT("Latency"), 200.0)/1000.0;

    outputParameters.channelCount = m_nChannelsOut;
    outputParameters.device = m_nDevice;
    outputParameters.hostApiSpecificStreamInfo = NULL;
    outputParameters.sampleFormat = paFloat32;
    outputParameters.suggestedLatency = dLatency;
    outputParameters.hostApiSpecificStreamInfo = NULL;

    PaError err;

    switch(m_nType)
    {
        case INPUT:
            pmlLog(pml::LOG_INFO, "pam::soundcard") << "Attempt to open " << m_nChannelsIn << " channel INPUT stream on device " << m_nDevice;
            err = Pa_OpenStream(&m_pStream, &inputParameters, 0, m_nSampleRate, 1024, paNoFlag, streamCallback, reinterpret_cast<void*>(this) );
            break;
        case OUTPUT:
            pmlLog(pml::LOG_INFO, "pam::soundcard") << "Attempt to open " << m_nChannelsOut << " channel OUTPUT stream on device " <<  m_nDevice;
            err = Pa_OpenStream(&m_pStream, 0, &outputParameters, m_nSampleRate, 0, paNoFlag, streamCallback, reinterpret_cast<void*>(this) );
            break;
        case DUPLEX:
            pmlLog(pml::LOG_INFO, "pam::soundcard") << "Attempt to open " << m_nChannelsIn << " in and " << m_nChannelsOut << "out DUPLEX stream on device " << m_nDevice;
            err = Pa_OpenStream(&m_pStream, &inputParameters, &outputParameters, m_nSampleRate, 2048, paNoFlag, streamCallback, reinterpret_cast<void*>(this) );
            break;
    }

    if(err == paNoError)
    {
        err = Pa_StartStream(m_pStream);
        if(err == paNoError)
        {
            #ifdef __WXGTK__
            PaAlsa_EnableRealtimeScheduling(m_pStream,1);
            #endif
            pmlLog(pml::LOG_INFO, "pam::soundcard") << "Device " << m_nDevice << " opened: Mode " << m_nType;
            const PaStreamInfo* pStreamInfo = Pa_GetStreamInfo(m_pStream);
            if(pStreamInfo)
            {
                pmlLog(pml::LOG_INFO, "pam::soundcard") << "StreamInfo: Input Latency " << pStreamInfo->inputLatency << " Output Latency " << pStreamInfo->outputLatency << " Sample Rate " << pStreamInfo->sampleRate;
            }
            return true;
        }
    }
    m_pStream = 0;
    pmlLog(pml::LOG_ERROR, "pam::soundcard") << "Failed to open device " << m_nDevice << " " << Pa_GetErrorText(err)
                                       << " with sample rate=" << m_nSampleRate << " input channels=" << m_nChannelsIn
                                       << " and output channels=" << m_nChannelsOut;




    return false;
}


Audio::~Audio()
{
    if(m_pStream)
    {

        PaError err = Pa_AbortStream(m_pStream);
        err = Pa_CloseStream(m_pStream);
        if(err != paNoError)
        {
            pmlLog(pml::LOG_ERROR, "pam::soundcard") << "Failed to stop PortAudio stream: " << Pa_GetErrorText(err);
        }
    }
    Settings::Get().RemoveHandler(this);
}

void Audio::InputCallback(const float* pBuffer, size_t nFrameCount, int nFlags)
{

    timedbuffer* pData = new timedbuffer(nFrameCount*2, m_nChannelsIn);
    pData->SetBuffer(pBuffer);
    pData->SetDuration(pData->GetBufferSize()*3);

    AudioEvent event(pData, AudioEvent::SOUNDCARD, nFrameCount, m_nSampleRate, nFlags&paInputOverflow, nFlags&paInputOverflow);
    wxPostEvent(m_pManager, event);

}

void Audio::OutputCallback(float* pBuffer, size_t nFrameCount, double dPlayoutLatency, int nFlags)
{

    wxMutexLocker ml(m_mutex);


    int nSamples = 0;

    timedSample tsBuffer;
    if(m_qBuffer.empty() == false)
    {
        tsBuffer = m_qBuffer.front();
    }

    size_t nSize(m_qBuffer.size());

    for(size_t i = nSamples; i < nFrameCount*m_nChannelsOut; i++)
    {
        if(m_qBuffer.empty() == false)
        {
            pBuffer[i] = m_qBuffer.front().dSample;
            m_qBuffer.pop();
        }
        else
        {
            pBuffer[i] = 0.0;
        }
    }

    if(m_bPlaying == false || nSize != 0)
    {
        m_bPlaying = true;

        timedbuffer* pTimedBuffer = new timedbuffer(nFrameCount*m_nChannelsOut, ConvertDoubleToPairTime(tsBuffer.dPresentation), tsBuffer.nTimestamp, m_nChannelsOut);
        pTimedBuffer->SetTransmissionTime(ConvertDoubleToPairTime(tsBuffer.dTransmission));
        pTimedBuffer->SetBuffer(pBuffer);
        pTimedBuffer->SetPlaybackOffset(dPlayoutLatency*1000000.0);//*1000.0);
        pTimedBuffer->SetBufferDepth(m_qBuffer.size()/m_nChannelsOut);
        pTimedBuffer->SetDuration(pTimedBuffer->GetBufferSize()*4);

        AudioEvent event(pTimedBuffer, AudioEvent::OUTPUT, nFrameCount, m_nSampleRate, nFlags&paOutputUnderflow, nFlags&paOutputOverflow);
        wxPostEvent(m_pManager, event);
    }
}


void Audio::SetGain(int nGain)
{
    m_dGain = pow(2.0, static_cast<double>(nGain));
}



int paCallback( const void *input, void *output, unsigned long frameCount, const PaStreamCallbackTimeInfo* timeInfo, PaStreamCallbackFlags statusFlags, void *userData )
{

    if(userData)
    {
        Audio* pComp = reinterpret_cast<Audio*>(userData);
        if(input)
        {

            pComp->InputCallback(reinterpret_cast<const float*>(input), frameCount, statusFlags);
        }
        if(output)
        {
            pComp->OutputCallback(reinterpret_cast<float*>(output), frameCount,timeInfo->outputBufferDacTime-timeInfo->currentTime, statusFlags);
        }
    }

    return 0;
}


void Audio::SetBufferSize(size_t nFrames)
{
    m_nBufferSize = nFrames;
    Pa_CloseStream(m_pStream);
    OpenStream(paCallback);
}


unsigned int Audio::GetInputNumberOfChannels()
{
    return m_nChannelsIn;
}

unsigned int Audio::GetOutputNumberOfChannels()
{
    return m_nChannelsIn;
}

unsigned int Audio::GetSampleRate()
{
    if(m_pStream)
    {
        return Pa_GetStreamInfo(m_pStream)->sampleRate;
    }
    return 0;
}

wxString Audio::GetDeviceName() const
{
    const PaDeviceInfo* pInfo = Pa_GetDeviceInfo(m_nDevice);
    if(pInfo)
    {
        return wxString::FromUTF8(pInfo->name);
    }
    return wxEmptyString;
}

unsigned int Audio::GetBitDepth() const
{
    return 32;
}


double Audio::GetOutputLatency()
{
    if(m_pStream)
    {
        const PaStreamInfo* pInfo = Pa_GetStreamInfo(m_pStream);
        if(pInfo)
        {
            return pInfo->outputLatency;
        }
    }

    return -1;
}

double Audio::GetInputLatency()
{
    if(m_pStream)
    {
        const PaStreamInfo* pInfo = Pa_GetStreamInfo(m_pStream);
        if(pInfo)
        {
            return pInfo->inputLatency;
        }
    }
    return -1;
}

timeval Audio::ConvertDoubleToPairTime(double dTime)
{
    double dInt, dDec;
    dDec = modf(dTime, &dInt);
    return {static_cast<time_t>(dInt), static_cast<unsigned int>(dDec*1000000.0)};
}


double Audio::ConvertPairTimeToDouble(const timeval& tv)
{
    return static_cast<double>(tv.tv_usec)/1000000.0 + static_cast<double>(tv.tv_sec);
}

void Audio::AddSamples(const timedbuffer* pTimedBuffer)
{
    wxMutexLocker ml(m_mutex);
    if(m_bPlaying)
    {
        if(m_nTotalChannels  > 0 && m_vMixer.size() > 0)
        {
            double dModifier(0.0);
            double dTransmission(ConvertPairTimeToDouble(pTimedBuffer->GetTransmissionTime()));
            double dPresentation(ConvertPairTimeToDouble(pTimedBuffer->GetTimeVal()));

            for(unsigned int i =0; i < pTimedBuffer->GetBufferSize(); i+=m_nTotalChannels)
            {
                int nSample = i/m_nTotalChannels;
                dModifier = static_cast<double>(nSample)/static_cast<double>(m_nSampleRate);

                for(size_t j = 0; j < m_vMixer.size(); j++)
                {
                    double dSample = 0.0;
                    if(m_vMixer[j] >= 0 && (int)m_vMixer[j] != 255)
                    {
                        dSample = pTimedBuffer->GetBuffer()[i+m_vMixer[j]];
                    }
                    if(j < m_vOutputRatio.size())
                    {
                        dSample *= m_vOutputRatio[j];
                    }
                    m_qBuffer.push(timedSample(dTransmission+dModifier, dPresentation+dModifier, pTimedBuffer->GetTimestamp()+nSample, dSample));
                }
            }
            //m_sLog << wxString::Format(wxT("Added: Size now %d\n"), m_qBuffer.size());
        }
    }

}

bool Audio::IsStreamOpen()
{
    return (m_pStream && Pa_IsStreamActive(m_pStream));
}



void Audio::FlushBuffer()
{
    wxMutexLocker ml(m_mutex);
    while(m_qBuffer.empty() == false)
    {
        m_qBuffer.pop();
    }
}

void Audio::SetMixer(const vector<char>& vChannels, unsigned int nTotalChannels)
{
    m_vMixer = vChannels;
    m_nTotalChannels = nTotalChannels;

}

const std::vector<char>& Audio::GetOutputChannels()
{
    return m_vMixer;
}

void Audio::OnSettingChanged(const SettingEvent& event)
{
    if(event.GetSection() == "Output" && event.GetKey().BeforeFirst('_') == "Ratio")
    {
        unsigned long nChannel;
        if(event.GetKey().AfterFirst('_').ToULong(&nChannel) && nChannel < m_vOutputRatio.size())
        {
            m_vOutputRatio[nChannel] = event.GetValue(1.0);
        }
    }
}
