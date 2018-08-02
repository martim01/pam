#pragma once
#include <wx/event.h>
#include <vector>
#include "dlldefine.h"

class Audio;
class timedbuffer;
class PaDeviceInfo;

class PAMBASE_IMPEXPORT SoundcardManager
{
    public:
        static SoundcardManager& Get();
        void Initialize();
        bool Init(wxEvtHandler* pHandler, int nInputDevice, int nOutputDevice, unsigned int nOutputSampleRate);

        int GetInputDevice() const;
        int GetOutputDevice() const;
        wxString GetInputDeviceName() const;
        wxString GetOutputDeviceName() const;


        unsigned int GetInputNumberOfChannels();
        unsigned int GetOutputNumberOfChannels();
        unsigned int GetInputSampleRate();
        unsigned int GetOutputSampleRate();

        void Terminate();
        void SetOutputMixer(const std::vector<char>& vChannels, unsigned int nTotalChannels);

        bool IsInputStreamOpen();
        bool IsOutputStreamOpen();

        double GetInputLatency();
        double GetOutputLatency();

        void AddOutputSamples(const timedbuffer* pTimedBuffer);

        std::vector<char> GetOutputChannels();

        size_t GetNumberOfDevices();
        const PaDeviceInfo* GetDeviceInfo(int nDevice);

        void FlushOutputQueue();

    private:

        SoundcardManager();
        ~SoundcardManager();

        void EnumerateDevices();

        Audio* m_pInput;
        Audio* m_pOutput;

        std::vector<char> m_vOutputMixer;
        unsigned int m_nTotalMixerChannels;

        std::vector<const PaDeviceInfo*> m_vDevices;


};

