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
class pnlSettingsInputNmos;


class NmosManager : public wxEvtHandler
{
    public:
        NmosManager(pnlSettingsInputNmos* pPnl);

        void Setup();
        void StartNode(int nMode);
        void StopNode();
        void StartClient(int nMode);
        void StopClient();


        enum {NODE_OFF=0, NODE_RECEIVER, NODE_SENDER, NODE_BOTH};
        enum {CLIENT_OFF=0, CLIENT_IS04, CLIENT_IS05};

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

        pnlSettingsInputNmos* m_pInputPanel;

        std::shared_ptr<FlowAudioRaw> m_pFlow;
        std::shared_ptr<Sender> m_pSender;
        std::shared_ptr<Receiver> m_pReceiver;
        std::multimap<std::string, std::shared_ptr<Sender> > m_mmLonelySender;

        size_t m_nNodeMode;
        size_t m_nClientMode;

};
#endif
