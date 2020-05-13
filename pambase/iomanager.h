#pragma once
#include <wx/event.h>
#include <set>
#include <map>
#include <wx/timer.h>
#include "session.h"
#include "dlldefine.h"
#include <memory>

class SettingEvent;
class AudioEvent;
class RtpThread;
class RtpServerThread;
class OnDemandStreamer;
class OnDemandAES67MediaSubsession;
class Generator;

namespace pml
{
    class SapServer;
}

namespace pml
{
    class Publisher;
}


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

        void DoSAP(bool bRun);
        void DoDNSSD(bool bRun);
        void OnTimerSilence(wxTimerEvent& event);

        void OnUnicastServerFinished(wxCommandEvent& event);

        void Stream();
        void StreamMulticast();
        void StreamUnicast();
        void StopStream();

        std::set<wxEvtHandler*> m_setHandlers;
        bool m_bSingleHandler;

        session m_SessionIn;
        session m_SessionOut;

        int m_nInputSource;
        int m_nOutputDestination;
        int m_nPlaybackSource;
        bool m_bPlaybackInput;
        bool  m_bMonitorOutput;
        bool m_bStreamMulticast;

        Generator* m_pGenerator;
        std::map<unsigned int, RtpThread*> m_mRtp;
        std::set<unsigned int> m_setRtpOrphan;
        unsigned int m_nCurrentRtp;

        RtpServerThread* m_pMulticastServer;
        OnDemandStreamer* m_pUnicastServer;
        OnDemandAES67MediaSubsession* m_pOnDemandSubsession;
        wxTimer m_timerSilence;

        bool m_bQueueToStream;
        bool m_bStreamActive;

        std::unique_ptr<pml::SapServer> m_pSapServer;
        std::unique_ptr<pml::Publisher> m_pPublisher;


};

DECLARE_EXPORTED_EVENT_TYPE(PAMBASE_IMPEXPORT, wxEVT_SESSION,-1)
DECLARE_EXPORTED_EVENT_TYPE(PAMBASE_IMPEXPORT, wxEVT_PLAYBACK_CHANNELS,-1)
DECLARE_EXPORTED_EVENT_TYPE(PAMBASE_IMPEXPORT, wxEVT_INPUT_FAILED,-1)
