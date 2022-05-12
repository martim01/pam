#pragma once
#include <wx/event.h>
#include <set>
#include <map>
#include <wx/timer.h>
#include "session.h"
#include "dlldefine.h"
#include <memory>
#include <vector>
#include <array>
#include "session.h"

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

    namespace dnssd
    {
        class Publisher;
    };
};


class PAMBASE_IMPEXPORT IOManager : public wxEvtHandler
{
    public:
        static IOManager& Get();

        void Start();
        void RegisterHandler(wxEvtHandler* pHandler);
        void DeregisterHandler(wxEvtHandler* pHandler);

        void RegisterForRTCPTransmission(wxEvtHandler* pHandler);
        void DeregisterForRTCPTransmission(wxEvtHandler* pHandler);
        void RegisterForRTSPTransmission(wxEvtHandler* pHandler);
        void DeregisterForRTSPTransmission(wxEvtHandler* pHandler);

        void Stop();
        const session& GetSession();
        const session& GetInputSession();
        const session& GetOutputSession();

        wxString GetRandomMulticastAddress();

        void RestartStream();

        wxString GetDnsSdService() const;

        int GetNumberOfSourceChannels() const;

    private:
        IOManager();
        ~IOManager();

        void OnSettingEvent(SettingEvent& event);
        void OnSettingEventMonitor(SettingEvent& event);
        void OnSettingEventInput(SettingEvent& event);
        void OnSettingEventQoS(SettingEvent& event);
        void OnSettingEventServer(SettingEvent& event);
        void OnSettingEventChannelMapping(SettingEvent& event);

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

        void InitAudioInputDevice(bool bStart =  false);
        void InitAudioOutputDevice();
        void OpenSoundcardDevice(unsigned long nOutputSampleRate);

        void OutputChannelsChanged();

        void ClearSession();
        void SessionChanged();

        void CheckPlayback(unsigned long nSampleRate, unsigned long nChannels);
        void CreateSessionFromOutput(const wxString& sSource);
        void UpdateOutputSession(bool bMapping=false);
        std::vector<subsession::channelGrouping> CreateChannels(unsigned long nChannels);
        std::vector<subsession::channelGrouping> CreateChannelsFrom2110(unsigned long nChannels);

        void DoSAP(bool bRun);
        void DoDNSSD(bool bRun);
        void OnTimerSilence(wxTimerEvent& event);
        void OnTimerReset(wxTimerEvent& event);

        void RTPServerFinished();

        void Stream();
        void StreamAlwaysOn();
        void StreamOnDemand();
        void StopStream();

        void DoGain(AudioEvent& event);
        void CheckIfGain();
        void InitAoIPInput();

        std::vector<char> CreateRouting();

                std::set<wxEvtHandler*> m_setHandlers;
        std::set<wxEvtHandler*> m_setRTCPHandlers;
        std::set<wxEvtHandler*> m_setRTSPHandlers;
        bool m_bSingleHandler;

        session m_SessionIn;
        session m_SessionOut;

        int m_nInputSource;
        int m_nOutputDestination;
        int m_nPlaybackSource;
        bool m_bPlaybackInput;
        bool  m_bMonitorOutput;
        bool m_bStreamAlwaysOn;

        std::unique_ptr<Generator> m_pGenerator;
        std::map<int, RtpThread*> m_mRtp;
        std::set<int> m_setRtpOrphan;
        int m_nCurrentRtp;

        RtpServerThread* m_pAlwaysOnServer;
        OnDemandStreamer* m_pOnDemandServer;
        OnDemandAES67MediaSubsession* m_pOnDemandSubsession;
        wxTimer m_timerSilence;
        wxTimer m_timerResetStream;

        bool m_bQueueToStream;
        bool m_bStreamActive;

        std::unique_ptr<pml::SapServer> m_pSapServer;
        std::unique_ptr<pml::dnssd::Publisher> m_pPublisher;

        std::vector<double> m_vRatio;
        bool m_bGain;
};

DECLARE_EXPORTED_EVENT_TYPE(PAMBASE_IMPEXPORT, wxEVT_SESSION,-1)
DECLARE_EXPORTED_EVENT_TYPE(PAMBASE_IMPEXPORT, wxEVT_PLAYBACK_CHANNELS,-1)
DECLARE_EXPORTED_EVENT_TYPE(PAMBASE_IMPEXPORT, wxEVT_INPUT_FAILED,-1)
