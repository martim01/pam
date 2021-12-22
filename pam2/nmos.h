#pragma once

#ifdef __NMOS__
#include <wx/event.h>
#include <map>
#include <memory>

class SettingEvent;
namespace pml
{
    namespace nmos
    {
        class SenderBase;
        class Sender;
        class Receiver;
        class FlowAudioRaw;
        class SourceAudio;
    };
};

class wxNmosNodeConnectionEvent;
class wxNmosNodeRegistrationEvent;
class wxNmosClientRequestEvent;
class wxNmosClientNodeEvent;
class wxNmosClientDeviceEvent;
class wxNmosClientSourceEvent;
class wxNmosClientFlowEvent;
class wxNmosClientSenderEvent;
class wxNmosClientReceiverEvent;
class pnlSettingsInputNmos;
class wxEventPoster;
class wxClientApiPoster;

class NmosManager : public wxEvtHandler
{
    public:
        NmosManager(pnlSettingsInputNmos* pPnl);
        ~NmosManager();

        void Setup();
        void StartNode(int nMode);
        void StopNode();
        void StartClient(int nMode);
        void StopClient();

        void AddHandlerToEventPoster(wxEvtHandler* pHandler);
        void AddHandlerToClientEventPoster(wxEvtHandler* pHandler);

        enum {NODE_OFF=0, NODE_RECEIVER, NODE_SENDER, NODE_BOTH};
        enum {CLIENT_OFF=0, CLIENT_IS04, CLIENT_IS05};

    protected:
        void OnSettingChanged(SettingEvent& event);

        void OnTarget(wxNmosNodeConnectionEvent& event);
        void OnPatchSender(wxNmosNodeConnectionEvent& event);
        void OnPatchReceiver(wxNmosNodeConnectionEvent& event);
        void OnSenderActivated(wxNmosNodeConnectionEvent& event);
        void OnReceiverActivated(wxNmosNodeConnectionEvent& event);

        void OnNmosSenderChanged(wxNmosClientSenderEvent& event);
        void OnNmosReceiverChanged(wxNmosClientReceiverEvent& event);
        void OnNmosFlowChanged(wxNmosClientFlowEvent& event);

        void OnNmosSubscribeRequest(wxNmosClientRequestEvent& event);

        void ActivateSender(const std::string& sId);
        void ActivateReceiver(const std::string& sId);

        std::shared_ptr<wxEventPoster> m_pNodePoster;
        std::shared_ptr<wxClientApiPoster> m_pClientPoster;
        pnlSettingsInputNmos* m_pInputPanel;

        std::shared_ptr<pml::nmos::FlowAudioRaw> m_pFlow;
        std::shared_ptr<pml::nmos::Sender> m_pSender;
        std::shared_ptr<pml::nmos::Receiver> m_pReceiver;
        std::shared_ptr<pml::nmos::SourceAudio> m_pSource;

        std::multimap<std::string, std::shared_ptr<const pml::nmos::Sender> > m_mmLonelySender;

        size_t m_nNodeMode;
        size_t m_nClientMode;

};
#endif
