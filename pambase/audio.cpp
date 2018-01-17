#include "audio.h"
#include <wx/log.h>
#include "portaudio.h"
#include <wx/msgdlg.h>
#include "soundfile.h"
#include <wx/datetime.h>
//#include "kaiserwindow.h"
#include "timedbuffer.h"
#include "wmlogevent.h"

DEFINE_EVENT_TYPE(wxEVT_FFT)
DEFINE_EVENT_TYPE(wxEVT_DATA)

using namespace std;



Audio::Audio(wxEvtHandler* pHandler, unsigned int nDevice) :
 m_pManager(pHandler),
 m_pStream(0),
 m_nDevice(nDevice),
 m_dGain(1),
 m_dPeakdB(-50),
 m_pFile(0),
 m_pInfo(Pa_GetDeviceInfo(nDevice))
 {

    m_nSampleRate = 48000;
    m_nBufferSize = 2048;

    wmLog::Get()->Log(wxT("Init PortAudio"));
    PaError err = Pa_Initialize();
    if(err != paNoError)
    {
        wmLog::Get()->Log(wxString::Format(wxT("Failed to init PortAudio: %s"), wxString::FromAscii(Pa_GetErrorText(err)).c_str()));
    }


 }

 bool Audio::Init()
 {

    return (OpenStream(paCallback));
}
bool Audio::OpenStream(PaStreamCallback *streamCallback)
{
    wmLog::Get()->Log(wxString::Format(wxT("Attempt to open device %d"), m_nDevice));

    PaStreamParameters inputParameters;

    const PaDeviceInfo* pInfo = Pa_GetDeviceInfo(m_nDevice);
    if(pInfo && pInfo->maxInputChannels < 2)
    {
        m_nChannels = pInfo->maxInputChannels;
    }
    else
    {
        m_nChannels = 2;
    }



    inputParameters.channelCount = m_nChannels;
    inputParameters.device = m_nDevice;
    inputParameters.hostApiSpecificStreamInfo = NULL;
    inputParameters.sampleFormat = paFloat32;
    inputParameters.suggestedLatency = 0;
    inputParameters.hostApiSpecificStreamInfo = NULL;
    PaError err = Pa_OpenStream(&m_pStream, &inputParameters, 0, m_nSampleRate, m_nBufferSize, paNoFlag, streamCallback, reinterpret_cast<void*>(this) );

    if(err == paNoError)
    {
        err = Pa_StartStream(m_pStream);
        if(err == paNoError)
        {
            wmLog::Get()->Log(wxString::Format(wxT("Device %d opened"), m_nDevice));
            return true;
        }
    }
    m_pStream = 0;
    wmLog::Get()->Log(wxString::Format(wxT("Failed to open device %d %s %d %d,"), m_nDevice, wxString::FromAscii(Pa_GetErrorText(err)).c_str(), m_nSampleRate, m_nChannels));
    return false;
}


Audio::~Audio()
{
    if(m_pStream)
    {
        wmLog::Get()->Log(wxT("Stop PortAudio input stream"));
        PaError err = Pa_StopStream(m_pStream);
        if(err != paNoError)
        {
            wmLog::Get()->Log(wxString::Format(wxT("Failed to stop PortAudio input stream: %s"), wxString::FromAscii(Pa_GetErrorText(err)).c_str()));
        }
    }
    wmLog::Get()->Log(wxT("Terminate PortAudio"));
    PaError err = Pa_Terminate();
    if(err != paNoError)
    {
        wmLog::Get()->Log(wxString::Format(wxT("Failed to terminate PortAudio: %s"), wxString::FromAscii(Pa_GetErrorText(err)).c_str()));
    }

}

void Audio::Callback(const float* pBuffer, size_t nFrameCount)
{

    timedbuffer* pData = new timedbuffer(nFrameCount*2);
    pData->SetBuffer(pBuffer);


    wxCommandEvent event(wxEVT_DATA);
    event.SetId(0);
    event.SetClientData(reinterpret_cast<void*>(pData));
    event.SetInt(nFrameCount);
    event.SetExtraLong(m_nSampleRate);
    wxPostEvent(m_pManager, event);

}


void Audio::SetGain(int nGain)
{
    m_dGain = pow(2.0, static_cast<double>(nGain));
}



int paCallback( const void *input, void *output, unsigned long frameCount, const PaStreamCallbackTimeInfo* timeInfo, PaStreamCallbackFlags statusFlags, void *userData )
{
    if(userData && input)
    {
        Audio* pComp = reinterpret_cast<Audio*>(userData);
        pComp->Callback(reinterpret_cast<const float*>(input), frameCount);
    }
    return 0;
}


void Audio::SetBufferSize(size_t nFrames)
{
    m_nBufferSize = nFrames;
    Pa_CloseStream(m_pStream);
    OpenStream(paCallback);
}


unsigned int Audio::GetNumberOfChannels()
{
    return m_nChannels;
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
        return wxString::FromAscii(pInfo->name);
    }
    return wxEmptyString;
}

unsigned int Audio::GetBitDepth() const
{
    return 32;
}
