#include "soundcardmanager.h"
#include "audio.h"
#include "timedbuffer.h"
#include "portaudio.h"
#include "wmlogevent.h"
#include <wx/log.h>

using namespace std;

SoundcardManager& SoundcardManager::Get()
{
    static SoundcardManager sm;
    return sm;
}

bool SoundcardManager::Init(wxEvtHandler* pHandler, int nInputDevice, int nOutputDevice, unsigned int nOutputSampleRate)
{

    if(m_pInput)
    {
        wxLogDebug(wxT("SoundcardManager: Input already open"));
        if(m_pInput->GetDevice() != nInputDevice || (m_pInput != m_pOutput && m_pInput->GetDevice() == nOutputDevice))
        {
            wxLogDebug(wxT("SoundcardManager: Input changed or output changed to input device"));
            if(m_pInput == m_pOutput)
            {
                m_pOutput = 0;
            }
            delete m_pInput;
            m_pInput = 0;
        }
    }

    if(m_pOutput)
    {
        wxLogDebug(wxT("SoundcardManager: Output already open"));
        if(m_pOutput->GetDevice() != nOutputDevice || (m_pOutput != m_pInput && m_pOutput->GetDevice() == nInputDevice))
        {
            wxLogDebug(wxT("SoundcardManager: Output changed or input changed to output device"));
            if(m_pInput == m_pOutput)
            {
                m_pInput = 0;
            }
            delete m_pOutput;
            m_pOutput = 0;
        }
    }

    wxLogDebug(wxT("InputDevice 0x%X OutputDevice 0x%X"), (int)m_pInput, (int)m_pOutput);

    wxLogDebug(wxString::Format(wxT("SoundcardManager: Input device =%d Output device =%d"), nInputDevice, nOutputDevice));

    if(m_pInput == NULL && m_pOutput == NULL)
    {
        Pa_Terminate();
        Pa_Initialize();
    }

    if(nInputDevice == nOutputDevice)
    {
        if(nInputDevice != -1)
        {
            if(!m_pInput && !m_pOutput)
            {
                wxLogDebug(wxT("SoundcardManager: Create duplex audio stream"));
                m_pInput = new Audio(pHandler, nInputDevice, Audio::DUPLEX);
                m_pOutput = m_pInput;
                m_pOutput->SetMixer(m_vOutputMixer, m_nTotalMixerChannels);

                if(m_pInput->Init(nOutputSampleRate) == false)
                {
                    delete m_pInput;
                    m_pInput = 0;
                    m_pOutput = 0;
                    return false;
                }
                return true;
            }
            return false;
        }
        return true;
    }

    bool bSuccess(true);
    if(!m_pInput && nInputDevice != -1)
    {
        m_pInput = new Audio(pHandler, nInputDevice, Audio::INPUT);
        if(m_pInput->Init(48000) == false)
        {
            delete m_pInput;
            m_pInput = 0;
            bSuccess = false;
        }

    }

    if(!m_pOutput && nOutputDevice != -1)
    {
        m_pOutput = new Audio(pHandler, nOutputDevice, Audio::OUTPUT);
        m_pOutput->SetMixer(m_vOutputMixer, m_nTotalMixerChannels);
        if(m_pOutput->Init(nOutputSampleRate) == false)
        {
            delete m_pOutput;
            m_pOutput = 0;
            bSuccess = false;
        }

    }

    return bSuccess;

}

int SoundcardManager::GetInputDevice() const
{
    if(m_pInput)
    {
        return m_pInput->GetDevice();
    }
    return -1;
}

int SoundcardManager::GetOutputDevice() const
{
    if(m_pOutput)
    {
        return m_pOutput->GetDevice();
    }
    return -1;
}

wxString SoundcardManager::GetInputDeviceName() const
{
    if(m_pInput)
    {
        return m_pInput->GetDeviceName();
    }
    return wxEmptyString;
}

wxString SoundcardManager::GetOutputDeviceName() const
{
    if(m_pOutput)
    {
        return m_pOutput->GetDeviceName();
    }
    return wxEmptyString;
}



unsigned int SoundcardManager::GetInputNumberOfChannels()
{
    if(m_pInput)
    {
        return m_pInput->GetInputNumberOfChannels();
    }
    return 0;
}

unsigned int SoundcardManager::GetOutputNumberOfChannels()
{
    if(m_pOutput)
    {
        return m_pInput->GetOutputNumberOfChannels();
    }
    return 0;
}

unsigned int SoundcardManager::GetInputSampleRate()
{
    if(m_pInput)
    {
        return m_pInput->GetSampleRate();
    }
    return 0;
}

unsigned int SoundcardManager::GetOutputSampleRate()
{
    if(m_pInput)
    {
        return m_pInput->GetSampleRate();
    }
    return 48000;
}


void SoundcardManager::SetOutputMixer(const std::vector<char>& vChannels, unsigned int nTotalChannels)
{
    if(m_pOutput)
    {
        m_pOutput->SetMixer(vChannels, nTotalChannels);
    }
    m_vOutputMixer = vChannels;
    m_nTotalMixerChannels = nTotalChannels;
}

bool SoundcardManager::IsInputStreamOpen()
{
    if(m_pInput)
    {
        return m_pInput->IsStreamOpen();
    }
    return false;
}

bool SoundcardManager::IsOutputStreamOpen()
{
    if(m_pOutput)
    {
        return m_pOutput->IsStreamOpen();
    }
    return false;
}

double SoundcardManager::GetInputLatency()
{
    if(m_pInput)
    {
        return m_pInput->GetInputLatency();
    }
}

double SoundcardManager::GetOutputLatency()
{
    if(m_pOutput)
    {
        return m_pOutput->GetOutputLatency();
    }
}

void SoundcardManager::AddOutputSamples(const timedbuffer* pTimedBuffer)
{
    if(m_pOutput)
    {
        m_pOutput->AddSamples(pTimedBuffer);
    }
}

vector<char> SoundcardManager::GetOutputChannels()
{
    if(m_pOutput)
    {
        return m_pOutput->GetOutputChannels();
    }
    return vector<char>(0);
}


SoundcardManager::SoundcardManager() : m_pInput(0), m_pOutput(0), m_nTotalMixerChannels(2)
{
    m_vOutputMixer.push_back(0);
    m_vOutputMixer.push_back(1);
}

SoundcardManager::~SoundcardManager()
{

}

void SoundcardManager::Initialize()
{
    Pa_Initialize();
}

void SoundcardManager::Terminate()
{
    delete m_pInput;
    if(m_pOutput != m_pInput)
    {
        delete m_pOutput;
    }
    m_pInput = 0;
    m_pOutput = 0;
    Pa_Terminate();
}
