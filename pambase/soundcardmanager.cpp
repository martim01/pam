#include "soundcardmanager.h"
#include "audio.h"
#include "timedbuffer.h"
#include "portaudio.h"
#include "wmlogevent.h"

using namespace std;

SoundcardManager& SoundcardManager::Get()
{
    static SoundcardManager sm;
    return sm;
}

bool SoundcardManager::Init(wxEvtHandler* pHandler, int nInputDevice, int nOutputDevice, unsigned int nOutputSampleRate)
{
    if(m_pInput && m_pOutput && m_pInput == m_pOutput)
    {
        if(nInputDevice != m_pInput->GetDevice() || nOutputDevice != m_pOutput->GetDevice())
        {
            delete m_pInput;
            m_pInput = 0;
            m_pOutput = 0;
        }
    }
    else
    {
        if(m_pInput && nInputDevice != m_pInput->GetDevice())
        {
            if(!m_pOutput || nInputDevice != m_pOutput->GetDevice())
            {
                delete m_pInput;
                m_pInput = 0;
            }
            else if(nInputDevice == m_pOutput->GetDevice())
            {
                delete m_pInput;
                delete m_pOutput;
                m_pInput = 0;
                m_pOutput = 0;
            }
        }
        if(m_pOutput && nOutputDevice != m_pOutput->GetDevice())
        {
            if(!m_pInput || nOutputDevice != m_pInput->GetDevice())
            {
                delete m_pOutput;
                m_pOutput = 0;
            }
            else if(nOutputDevice == m_pInput->GetDevice())
            {
                delete m_pInput;
                delete m_pOutput;
                m_pInput = 0;
                m_pOutput = 0;
            }
        }

    }
    wmLog::Get()->Log(wxString::Format(wxT("SoundcardManager: Input device =%d Output device =%d"), nInputDevice, nOutputDevice));

    if(nInputDevice == nOutputDevice)
    {
        if(nInputDevice != -1)
        {
            if(!m_pInput && !m_pOutput)
            {
                m_pInput = new Audio(pHandler, nInputDevice, Audio::DUPLEX);
                m_pOutput = m_pInput;
                m_pOutput->SetMixer(m_vOutputMixer, m_nTotalMixerChannels);

                return m_pInput->Init(nOutputSampleRate);
            }
            return false;
        }
        return true;
    }

    bool bSuccess(true);
    if(!m_pInput && nInputDevice != -1)
    {
        m_pInput = new Audio(pHandler, nInputDevice, Audio::INPUT);
        bSuccess = m_pInput->Init(48000);
    }

    if(!m_pOutput && nOutputDevice != -1)
    {
        m_pOutput = new Audio(pHandler, nOutputDevice, Audio::OUTPUT);
        m_pOutput->SetMixer(m_vOutputMixer, m_nTotalMixerChannels);
        bSuccess &= m_pOutput->Init(nOutputSampleRate);
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

const vector<char>& SoundcardManager::GetOutputChannels()
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
    delete m_pOutput;
    Pa_Terminate();
}
