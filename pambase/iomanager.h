#pragma once
#include <wx/event.h>
#include <set>
#include <map>
#include <wx/timer.h>
#include "session.h"
#include "dlldefine.h"

class SettingEvent;
class AudioEvent;
class RtpThread;
class RtpServerThread;

class Generator;

class PAMBASE_IMPEXPORT IOManager : public wxEvtHandler
{
    public:
        static IOManager& Get();

        void Start();
        void RegisterHandler(wxEvtHandler* pHandler);
        void DeregisterHandler(wxEvtHandler* pHandler);

        void Stop();
        const session& GetSession();

        wxString GetRandomMulticastAddress();

    private:
        IOManager();
        ~IOManager();

        void OnSettingEvent(SettingEvent& event);
        void OnAudioEvent(AudioEvent& event);
        void PassOnAudio(AudioEvent& event);

        void AddOutputSamples(size_t nSize);

        void OnPtpEvent(wxCommandEvent& event);

         void OnRTPSession(wxCommandEvent& event);
        void OnRTPSessionClosed(wxCommandEvent& event);
        void OnQoS(wxCommandEvent& event);

        void InputTypeChanged();
        void InputChanged(const wxString& sKey);

        void OutputDestinationChanged();
        void OutputChanged(const wxString& sKey);
        void GeneratorToneChanged();
        void GeneratorNoiseChanged(const wxString& sKey);


        void InitGeneratorFile();
        void InitGeneratorSequence();
        void InitGeneratorTone();
        void InitGeneratorNoise();
        void InitGeneratorPlugin(const wxString& sPlugin);

        void InitAudioInputDevice();
        void InitAudioOutputDevice();
        void OpenSoundcardDevice(unsigned long nOutputSampleRate);

        void OutputChannelsChanged();

        void ClearSession();
        void SessionChanged();

        void CheckPlayback(unsigned long nSampleRate, unsigned long nChannels);
        void CreateSessionFromOutput(const wxString& sSource);


        void OnTimerSilence(wxTimerEvent& event);

        std::set<wxEvtHandler*> m_setHandlers;
        bool m_bSingleHandler;

        session m_SessionIn;
        session m_SessionOut;

        int m_nInputSource;
        int m_nOutputDestination;
        int m_nPlaybackSource;
        bool m_bPlaybackInput;
        bool  m_bMonitorOutput;
        bool m_bStream;

        Generator* m_pGenerator;
        std::map<unsigned int, RtpThread*> m_mRtp;
        std::set<unsigned int> m_setRtpOrphan;
        unsigned int m_nCurrentRtp;

        RtpServerThread* m_pRtpServer;
        wxTimer m_timerSilence;
        //wxString m_sCurrentSequence;
};

DECLARE_EXPORTED_EVENT_TYPE(PAMBASE_IMPEXPORT, wxEVT_SESSION,-1)
DECLARE_EXPORTED_EVENT_TYPE(PAMBASE_IMPEXPORT, wxEVT_PLAYBACK_CHANNELS,-1)
DECLARE_EXPORTED_EVENT_TYPE(PAMBASE_IMPEXPORT, wxEVT_INPUT_FAILED,-1)
