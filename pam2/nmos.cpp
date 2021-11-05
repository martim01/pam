#ifdef __NMOS__
#include "aoipsourcemanager.h"
#include "nmos.h"
#include "libnmos.h"
#include "wxeventposter.h"
#include "wxclientapiposter.h"
#include "wxnmosclientevent.h"
#include "settings.h"
#include "settingevent.h"
#include "log.h"
#include "pnlSettingsInputNmos.h"
#include "iomanager.h"
#include <wx/log.h>

NmosManager::NmosManager(pnlSettingsInputNmos* pPnl) :
    m_pInputPanel(pPnl),
    m_pFlow(0),
    m_pSender(0),
    m_nNodeMode(NODE_OFF),
    m_nClientMode(CLIENT_OFF)
{

    Settings::Get().AddHandler("NMOS", "Node", this);
    Settings::Get().AddHandler("NMOS", "Client", this);

    Settings::Get().AddHandler("AoIP","Epoch", this);
    Settings::Get().AddHandler("Server", "DestinationIp", this);
    Settings::Get().AddHandler("Server", "RTP_Port", this);
    Settings::Get().AddHandler("Server", "PacketTime", this);
    Settings::Get().AddHandler("Server", "Stream", this);

    Bind(wxEVT_SETTING_CHANGED, &NmosManager::OnSettingChanged, this);
}



void NmosManager::Setup()
{
    pmlLog() << "NMOS\tSetup NMOS";

    char chHost[256];
    gethostname(chHost, 256);


    NodeApi::Get().Init(std::make_shared<wxEventPoster>(this), Settings::Get().Read("NMOS", "Port_Discovery", 8080),
                        Settings::Get().Read("NMOS", "Port_Connection", 8080),
                        Settings::Get().Read("NMOS", "HostLabel", wxEmptyString).ToStdString(),
                        Settings::Get().Read("NMOS", "HostDescription", "PAM").ToStdString());
    NodeApi::Get().GetSelf().AddInternalClock("clk0");
    NodeApi::Get().GetSelf().AddInterface("eth0");
    NodeApi::Get().SetHeartbeatTime(5000);

    auto pDevice = std::make_shared<Device>(chHost, "Live555", Device::GENERIC,NodeApi::Get().GetSelf().GetId());

    auto pSource = std::make_shared<SourceAudio>(chHost, "Live555", pDevice->GetId());
    pSource->AddChannel("Left", "L");
    pSource->AddChannel("Right", "R");

    m_pFlow = std::make_shared<FlowAudioRaw>(chHost, "Live555", pSource->GetId(), pDevice->GetId(), 48000, FlowAudioRaw::L24);
    int nPackeTime = Settings::Get().Read("Server", "PacketTime", 1000);
    switch(nPackeTime)
    {
    case 125:
        m_pFlow->SetPacketTime(FlowAudioRaw::US_125);
        break;
    case 250:
        m_pFlow->SetPacketTime(FlowAudioRaw::US_250);
        break;
    case 333:
        m_pFlow->SetPacketTime(FlowAudioRaw::US_333);
        break;
    case 4000:
        m_pFlow->SetPacketTime(FlowAudioRaw::US_4000);
        break;
    default:
        m_pFlow->SetPacketTime(FlowAudioRaw::US_1000);
        break;
    }

    m_pFlow->SetMediaClkOffset(Settings::Get().Read("AoIP_Settings", "Epoch", 0));    //@todo get this from Live555

    m_pSender = std::make_shared<Sender>(chHost, "Live555 sender", m_pFlow->GetId(), Sender::RTP_MCAST, pDevice->GetId(), "eth0");
    m_pSender->SetDestinationDetails(Settings::Get().Read("Server", "DestinationIp",wxEmptyString).ToStdString(),
                                     Settings::Get().Read("Server", "RTP_Port", 5004));

    m_pReceiver = std::make_shared<Receiver>(chHost, "Live555 receiver", Receiver::RTP_MCAST, pDevice->GetId(), Receiver::AUDIO,
                                                TransportParamsRTP::CORE | TransportParamsRTP::MULTICAST);
    m_pReceiver->AddCap("audio/L24");
    m_pReceiver->AddCap("audio/L16");
    m_pReceiver->AddInterfaceBinding("eth0");

    if(NodeApi::Get().AddDevice(pDevice) == false)
    {
        pmlLog(pml::LOG_ERROR) << "NMOS\tFailed to add Device";
    }
    if(NodeApi::Get().AddSource(pSource) == false)
    {
        pmlLog(pml::LOG_ERROR) << "NMOS\tFailed to add Source";
    }
    if(NodeApi::Get().AddFlow(m_pFlow) == false)
    {
        pmlLog(pml::LOG_ERROR) << "NMOS\tFailed to add Flow";
    }
    NodeApi::Get().AddSender(m_pSender);
    NodeApi::Get().AddReceiver(m_pReceiver);

    NodeApi::Get().Commit();



    Bind(wxEVT_NMOS_TARGET, &NmosManager::OnTarget, this);
    Bind(wxEVT_NMOS_PATCH_SENDER, &NmosManager::OnPatchSender, this);
    Bind(wxEVT_NMOS_PATCH_RECEIVER, &NmosManager::OnPatchReceiver, this);
    Bind(wxEVT_NMOS_ACTIVATE_RECEIVER, &NmosManager::OnReceiverActivated, this);
    Bind(wxEVT_NMOS_ACTIVATE_SENDER, &NmosManager::OnSenderActivated, this);
    Bind(wxEVT_NMOS_CLIENT_SENDER, &NmosManager::OnNmosSenderChanged, this);
    Bind(wxEVT_NMOS_CLIENT_RECEIVER, &NmosManager::OnNmosReceiverChanged, this);
    Bind(wxEVT_NMOS_CLIENT_FLOW, &NmosManager::OnNmosFlowChanged, this);
    Bind(wxEVT_NMOS_CLIENTCURL_SUBSCRIBE, &NmosManager::OnNmosSubscribeRequest, this);


    StartNode(Settings::Get().Read("NMOS", "Node",0));
    StartClient(Settings::Get().Read("NMOS", "Client", 0));
}


void NmosManager::StartNode(int nMode)
{
    if(nMode != m_nNodeMode)
    {
        switch(nMode)
        {
            case NODE_RECEIVER:
                //NodeApi::Get().RemoveSender(m_pSender->GetId());
                m_pSender->MasterEnable(false);
                //NodeApi::Get().AddReceiver(m_pReceiver);
                m_pReceiver->MasterEnable(true);
                m_pInputPanel->SetReceiverId(m_pReceiver->GetId());
                NodeApi::Get().Commit();
                break;
            case NODE_SENDER:
                //NodeApi::Get().RemoveReceiver(m_pReceiver->GetId());
                //NodeApi::Get().AddSender(m_pSender);
                m_pReceiver->MasterEnable(false);
                m_pSender->MasterEnable(true);
                m_pInputPanel->SetReceiverId("");
                NodeApi::Get().Commit();
                break;
            case NODE_BOTH:
                //NodeApi::Get().AddSender(m_pSender);
                //NodeApi::Get().AddReceiver(m_pReceiver);
                m_pReceiver->MasterEnable(true);
                m_pSender->MasterEnable(true);
                m_pInputPanel->SetReceiverId(m_pReceiver->GetId());
                NodeApi::Get().Commit();
                break;
        }

        if(m_nNodeMode == NODE_OFF)
        {
            pmlLog() << "NMOS\tStart NMOS Services";
            NodeApi::Get().StartServices();
        }
        else if(nMode == NODE_OFF)
        {
            StopNode();
        }
        m_nNodeMode = nMode;

    }
}




void NmosManager::OnSettingChanged(SettingEvent& event)
{

    if(event.GetSection() == "NMOS")
    {
        if(event.GetKey() == "Node")
        {
            StartNode(event.GetValue((long)NODE_OFF));
        }
        else if(event.GetKey() == "Client")
        {
            StartClient(event.GetValue((long)CLIENT_OFF));
        }
    }
    else if(event.GetSection() == "Server")
    {
        if(m_pSender && (event.GetKey() == "DestinationIp" || event.GetKey() == "RTP_Port"))
        {
            m_pSender->SetDestinationDetails(Settings::Get().Read("Server", "DestinationIp",wxEmptyString).ToStdString(),
                                             Settings::Get().Read("Server", "RTP_Port", 5004));
            NodeApi::Get().Commit();
        }
        if(m_pFlow && event.GetKey() == "PacketTime")
        {
            switch(event.GetValue(long(1000)))
            {
            case 125:
                m_pFlow->SetPacketTime(FlowAudioRaw::US_125);
                break;
            case 250:
                m_pFlow->SetPacketTime(FlowAudioRaw::US_250);
                break;
            case 333:
                m_pFlow->SetPacketTime(FlowAudioRaw::US_333);
                break;
            case 4000:
                m_pFlow->SetPacketTime(FlowAudioRaw::US_4000);
                break;
            default:
                m_pFlow->SetPacketTime(FlowAudioRaw::US_1000);
                break;
            }
            NodeApi::Get().Commit();
        }
        if(m_pSender && event.GetKey()=="Stream")
        {
            //m_pSender->MasterEnable(event.GetValue(false));
            NodeApi::Get().Commit();
        }
    }
}



void NmosManager::OnTarget(wxNmosEvent& event)
{

    // @todo move this somewhere else...
    std::multimap<wxString, wxString> mmButtons(Settings::Get().GetInterfaces());
    auto itInterface = mmButtons.find("eth0");
    if(itInterface != mmButtons.end())
    {

        //Save the SDP file details
        int nInput = AoipSourceManager::SOURCE_NMOS_A;
        if(Settings::Get().Read("Input", "AoIP", 0) == AoipSourceManager::SOURCE_NMOS_A)
        {
            nInput = AoipSourceManager::SOURCE_NMOS_B;
        }
        AoipSourceManager::Get().SetSourceSDP(nInput, event.GetTransportFile());

        Settings::Get().Write("Input", "Type", "AoIP");
        Settings::Get().Write("Input", "AoIP", nInput); //write the new source number in

        //Now tell NMOS that we have taken the target
        NodeApi::Get().TargetTaken(itInterface->second.ToStdString(), event.GetPort(), true);
    }
}

void NmosManager::OnPatchSender(wxNmosEvent& event)
{
    auto pSender = NodeApi::Get().GetSender(event.GetString().ToStdString());
    if(pSender)
    {
        std::string sSourceIp(pSender->GetStaged().tpSender.sSourceIp);
        if(sSourceIp.empty() || sSourceIp == "auto")
        {
            sSourceIp = Settings::Get().Read("Server", "RTSP_Address", wxEmptyString).ToStdString();
            if(sSourceIp.empty())
            {
                 std::multimap<wxString, wxString> mmButtons(Settings::Get().GetInterfaces());
                 if(mmButtons.empty() == false)
                 {
                     sSourceIp = mmButtons.begin()->second.ToStdString();
                 }
            }
        }

        std::string sDestinationIp;
        pmlLog() << "Patch Sender: ReceiverId = '" << pSender->GetStaged().sReceiverId << "'";

        if(pSender->GetStaged().sReceiverId.empty())
        {   //no receiver so multicast
            if(pSender->GetStaged().tpSender.sDestinationIp.empty() == false && pSender->GetStaged().tpSender.sDestinationIp != "auto")
            {   //address has been chosen by nmos
                sDestinationIp = pSender->GetStaged().tpSender.sDestinationIp;

                pmlLog() << "Patch Sender: DestinationIp set by receiver = '" << sDestinationIp << "'";
            }
            else //if(Settings::Get().Read("Server", "DestinationIp", wxEmptyString).empty())
            {   //address not chosen by nmos and pam hasn't chosen one yet. create rando,
                sDestinationIp = IOManager::Get().GetRandomMulticastAddress().ToStdString();

                pmlLog() << "Patch Sender: DestinationIp set by us = '" << sDestinationIp << "'";
            }
        }
        else
        {   //receiverid so unicast. -- @todo if we are already multicasting what do we do here????
            if(pSender->GetStaged().tpSender.sDestinationIp.empty() == false && pSender->GetStaged().tpSender.sDestinationIp != "auto")
            {   //one would hope ip has been set. so set the live555 address to it
                sDestinationIp = pSender->GetStaged().tpSender.sDestinationIp;
                pmlLog() << "Patch Sender: DestinationIp set by receiver = '" << sDestinationIp << "'";
            }
            else
            {   // @todo no ip address set. Should we try to find what the receiver ip address is from the clientapi??
                pmlLog() << "Patch Sender: DestinationIp set by no-one = '" << sDestinationIp << "'";
            }
        }

        // @todo Set the sender stuff - maybe changes the Live555 settings
        NodeApi::Get().SenderPatchAllowed(event.GetPort(), true, pSender->GetId(), sSourceIp, sDestinationIp);
        if(event.GetSenderConnection().eActivate == connection::ACT_NOW)
        {
            ActivateSender(pSender);
        }
    }
    else
    {
        NodeApi::Get().SenderPatchAllowed(event.GetPort(), false, event.GetResourceId().ToStdString(), "", "");
    }

}

void NmosManager::OnPatchReceiver(wxNmosEvent& event)
{
    auto pReceiver = NodeApi::Get().GetReceiver(event.GetString().ToStdString());
    if(pReceiver)
    {
        std::multimap<wxString, wxString> mmButtons(Settings::Get().GetInterfaces());
        auto itInterface = mmButtons.find("eth0");
        if(itInterface != mmButtons.end())
        {
            //Chage the AOIP source to be NMOS IS-04
            NodeApi::Get().ReceiverPatchAllowed(event.GetPort(), true, pReceiver->GetId(), itInterface->second.ToStdString());

            if(event.GetReceiverConnection().eActivate == connection::ACT_NOW)
            {
                ActivateReceiver(pReceiver);
            }
        }
    }
    else
    {
        NodeApi::Get().SenderPatchAllowed(event.GetPort(), false, event.GetResourceId().ToStdString(),"","");
    }
    // @todo Set the receiver stuff - maybe changes the Live555 settings

}

void NmosManager::OnSenderActivated(wxNmosEvent& event)
{
    pmlLog() << "OnSenderActivated " << event.GetString().ToStdString();
    auto pSender = NodeApi::Get().GetSender(event.GetString().ToStdString());
    if(pSender)
    {
        ActivateSender(pSender);
    }
}


void NmosManager::ActivateSender(std::shared_ptr<Sender> pSender)
{
    pmlLog() << "NmosManager::ActivateSender";
    if(pSender->GetDestinationIp().empty() == false)
    {   //address has been chosen by nmos
        Settings::Get().Write("Server", "DestinationIp", pSender->GetDestinationIp());
        pmlLog() << "ActivateSender: DestinationIp set '" << pSender->GetDestinationIp() << "'";
    }
    else
    {
        if(pSender->GetStaged().sReceiverId.empty())
        {   //address not chosen by nmos and pam hasn't chosen one yet. create rando,
            Settings::Get().Write("Server", "DestinationIp", IOManager::Get().GetRandomMulticastAddress());
            pmlLog() << "ActivateSender: DestinationIp set by random";
        }
        else
        {   // @todo no ip address set. Should we try to find what the receiver ip address is from the clientapi??
            pmlLog() << "ActivateSender: DestinationIp not set";
        }
    }

    //if the source interface is set then change our source ip. else do the opposites
    std::string sSourceIp(pSender->GetStaged().tpSender.sSourceIp);
    if(sSourceIp.empty() == false && sSourceIp != "auto")
    {   //sourceip set by nmos
        Settings::Get().Write("Server", "RTSP_Address", sSourceIp);
    }
    else
    {
        sSourceIp = Settings::Get().Read("Server", "RTSP_Address", wxEmptyString).ToStdString();
        if(sSourceIp.empty())
        {
             std::multimap<wxString, wxString> mmButtons(Settings::Get().GetInterfaces());
             if(mmButtons.empty() == false)
             {
                 Settings::Get().Write("Server", "RTSP_Address", mmButtons.begin()->second);
             }
        }
    }

    if(pSender->GetStaged().tpSender.nDestinationPort != 0)
    {
        Settings::Get().Write("Server", "RTP_Port", pSender->GetStaged().tpSender.nDestinationPort);
    }

    if(pSender->GetStaged().bMasterEnable == true)
    {
        Settings::Get().Write("Output", "Destination", "AoIP");
    }
    Settings::Get().Write("Server", "Stream", "Multicast");

    IOManager::Get().RestartStream();
}

void NmosManager::OnReceiverActivated(wxNmosEvent& event)
{
    // @todo move this somewhere else...
    auto pReceiver = NodeApi::Get().GetReceiver(event.GetString().ToStdString());
    if(pReceiver)
    {
        ActivateReceiver(pReceiver);

    }
}

void NmosManager::ActivateReceiver(std::shared_ptr<Receiver> pReceiver)
{
    if(pReceiver)
    {
        std::multimap<wxString, wxString> mmButtons(Settings::Get().GetInterfaces());
        auto itInterface = mmButtons.find("eth0");
        if(itInterface != mmButtons.end())
        {
            //Save the SDP file details
            int nInput = AoipSourceManager::SOURCE_NMOS_A;
            if(Settings::Get().Read("Input", "AoIP", 0) == AoipSourceManager::SOURCE_NMOS_A)
            {
                nInput = AoipSourceManager::SOURCE_NMOS_B;
            }
            AoipSourceManager::Get().SetSourceSDP(nInput, pReceiver->GetStaged().sTransportFileData);

            //Chage the AOIP source to be NMOS IS-04
            wxString sSender;
            if(pReceiver->GetStaged().bMasterEnable)
            {
                //Make sure the input type is AoIP
                Settings::Get().Write("Input", "Type", "AoIP");
                Settings::Get().Write("Input", "AoIP", nInput); //write the new source number in
                sSender = wxString(pReceiver->GetStaged().sSenderId);
            }
            else
            {
                Settings::Get().Write("Input", "AoIP", 0); //write the new source number in
            }

            if(m_pInputPanel)
            {
                AoipSourceManager::Get().EditSource(nInput, (nInput == AoipSourceManager::SOURCE_NMOS_A ? "NMOS_A" : "NMOS_B"), sSender);
                m_pInputPanel->SetSender(sSender);
            }
        }
    }
}

void NmosManager::StopNode()
{
    pmlLog() << "NMOS\tStop NMOS Services";
    NodeApi::Get().StopServices();
}


void NmosManager::StartClient(int nMode)
{
    if(m_nClientMode != nMode)
    {
        if(nMode != CLIENT_OFF && m_nClientMode == CLIENT_OFF)
        {
            ClientApi::Get().SetPoster(std::make_shared<wxClientApiPoster>(this));
            ClientApi::Get().AddQuerySubscription(ClientApi::ALL, "",0);
            ClientApi::Get().Start();
        }
        else
        {
            StopClient();
        }
        m_nClientMode = nMode;
    }
}

void NmosManager::StopClient()
{
    ClientApi::Get().Stop();
}



void NmosManager::OnNmosReceiverChanged(wxNmosClientReceiverEvent& event)
{
    if(m_pInputPanel)
    {
        for(auto itUpdated = event.GetUpdated().begin(); itUpdated != event.GetUpdated().end(); ++itUpdated)
        {
            if((*itUpdated)->GetId() == m_pInputPanel->GetReceiverId())
            {
                //is the receiver one of ours??
                if((*itUpdated)->GetSender().empty() == false)
                {
                    m_pInputPanel->SetSender(wxString::FromUTF8((*itUpdated)->GetSender().c_str()));
                }
                else
                {
                    m_pInputPanel->SetSender(wxEmptyString);
                }
                break;
            }
        }
    }
}

void NmosManager::OnNmosSenderChanged(wxNmosClientSenderEvent& event)
{
    if(m_pInputPanel)
    {
        for(auto pSender : event.GetAdded())
        {
            if(NodeApi::Get().GetSender(pSender->GetId()) == 0)
            {  //not one of our senders
                auto itFlow = ClientApi::Get().FindFlow(pSender->GetFlowId());
                if(itFlow != ClientApi::Get().GetFlowEnd())
                {
                    if(itFlow->second->GetFormat().find("urn:x-nmos:format:audio") != std::string::npos)
                    {
                        m_pInputPanel->AddSender(pSender);
                    }
                }
            }
            else
            {
                //Store the sender for checking once we get a flow...
                m_mmLonelySender.insert(make_pair(pSender->GetFlowId(), pSender));
            }
        }
        for(auto pSender : event.GetUpdated())
        {
            if(NodeApi::Get().GetSender(pSender->GetId()) == 0)
            {   //not one of our senders
                auto itFlow = ClientApi::Get().FindFlow(pSender->GetFlowId());
                if(itFlow != ClientApi::Get().GetFlowEnd())
                {
                    if(itFlow->second->GetFormat().find("urn:x-nmos:format:audio") != std::string::npos)
                    {
                        m_pInputPanel->UpdateSender(pSender);
                    }
                }
            }
        }
        m_pInputPanel->RemoveSenders(event.GetRemoved());
    }
}

void NmosManager::OnNmosFlowChanged(wxNmosClientFlowEvent& event)
{
    if(m_pInputPanel)
    {
        for(auto itAdded = event.GetAdded().begin(); itAdded != event.GetAdded().end(); ++itAdded)
        {
            if((*itAdded)->GetFormat().find("urn:x-nmos:format:audio") != std::string::npos)
            {
                for(auto itSender = m_mmLonelySender.lower_bound((*itAdded)->GetId()); itSender != m_mmLonelySender.upper_bound((*itAdded)->GetId()); ++itSender)
                {
                    m_pInputPanel->AddSender(itSender->second);
                }
                //remove the senders which are no longer lonely
                m_mmLonelySender.erase((*itAdded)->GetId());
            }
        }

    }
}


void NmosManager::OnNmosSubscribeRequest(wxNmosClientCurlEvent& event)
{
    if(m_pInputPanel)
    {
        m_pInputPanel->SubscriptionRequest(event.GetResourceId(), event.GetResponse(), event.GetResult());
    }
}
#endif // __NMOS__
