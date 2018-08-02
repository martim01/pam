#pragma once
#include <wx/event.h>
#include <set>
#include <map>
#include "session.h"
#include "dlldefine.h"

class SettingEvent;
class AudioEvent;
class RtpThread;

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


    private:
        IOManager();
        ~IOManager();

        void OnSettingEvent(SettingEvent& event);
        void OnAudioEvent(AudioEvent& event);
        void PassOnAudio(AudioEvent& event);

         void OnRTPSession(wxCommandEvent& event);
        void OnRTPSessionClosed(wxCommandEvent& event);
        void OnQoS(wxCommandEvent& event);

        void InputTypeChanged();
        void InputChanged(const wxString& sKey);
        void OutputChanged(const wxString& sKey);
        void GeneratorToneChanged();
        void GeneratorNoiseChanged();


        void InitGeneratorFile();
        void InitGeneratorSequence();
        void InitGeneratorTone();
        void InitGeneratorNoise();

        void InitAudioInputDevice();
        void OpenSoundcardDevice(unsigned long nOutputSampleRate);

        void OutputChannelsChanged();

        void ClearSession();
        void SessionChanged();

        void CheckPlayback(unsigned long nSampleRate, unsigned long nChannels);
        void CreateSessionFromOutput(const wxString& sSource);



        std::set<wxEvtHandler*> m_setHandlers;
        bool m_bSingleHandler;

        session m_SessionIn;
        session m_SessionOut;

        int m_nInputSource;
        int m_nPlaybackSource;
        bool m_bPlaybackInput;
        bool  m_bMonitorOutput;

        Generator* m_pGenerator;
        std::map<wxString, RtpThread*> m_mRtp;
        std::set<wxString> m_setRtpOrphan;
        wxString m_sCurrentRtp;


        //wxString m_sCurrentSequence;
};

DECLARE_EXPORTED_EVENT_TYPE(WXEXPORT, wxEVT_SESSION,-1)
DECLARE_EXPORTED_EVENT_TYPE(WXEXPORT, wxEVT_PLAYBACK_CHANNELS,-1)
