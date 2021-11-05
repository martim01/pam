#pragma once

#ifdef __NMOS__
#include <wx/event.h>
#include <map>
#include <memory>

class SettingEvent;
class Sender;
class Receiver;
class FlowAudioRaw;
class wxNmosEvent;
class wxNmosClientCurlEvent;
class wxNmosClientNodeEvent;
class wxNmosClientDeviceEvent;
class wxNmosClientSourceEvent;
class wxNmosClientFlowEvent;
class wxNmosClientSenderEvent;
class wxNmosClientReceiverEvent;
class pnlSettingsNmos;


class NmosManager : public wxEvtHandler
{
    public:
        NmosManager(pnlSettingsNmos* pPnl);

        void Setup();
        void StartNode();
        void StopNode();
        void StartClient();
        void StopClient();

    protected:
        void OnSettingChanged(SettingEvent& event);

        void OnTarget(wxNmosEvent& event);
        void OnPatchSender(wxNmosEvent& event);
        void OnPatchReceiver(wxNmosEvent& event);
        void OnSenderActivated(wxNmosEvent& event);
        void OnReceiverActivated(wxNmosEvent& event);

        void OnNmosSenderChanged(wxNmosClientSenderEvent& event);
        void OnNmosReceiverChanged(wxNmosClientReceiverEvent& event);
        void OnNmosFlowChanged(wxNmosClientFlowEvent& event);

        void OnNmosSubscribeRequest(wxNmosClientCurlEvent& event);

        void ActivateSender(std::shared_ptr<Sender> pSender);
        void ActivateReceiver(std::shared_ptr<Receiver> pReceiver);

        pnlSettingsNmos* m_pSettingsPanel;

        std::shared_ptr<FlowAudioRaw> m_pFlow;
        std::shared_ptr<Sender> m_pSender;
        std::multimap<std::string, std::shared_ptr<Sender> > m_mmLonelySender;
};
#endif
