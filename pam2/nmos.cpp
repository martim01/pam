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
    m_pPoster(std::make_shared<wxEventPoster>(this)),
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

NmosManager::~NmosManager()
{
    StopClient();
    StopNode();
}



void NmosManager::Setup()
{
    pmlLog() << "NMOS\tSetup NMOS";

    char chHost[256];
    gethostname(chHost, 256);



    pml::nmos::NodeApi::Get().Init(m_pPoster, Settings::Get().Read("NMOS", "Port_Discovery", 8080),
                        Settings::Get().Read("NMOS", "Port_Connection", 8080),
                        Settings::Get().Read("NMOS", "HostLabel", wxEmptyString).ToStdString(),
                        Settings::Get().Read("NMOS", "HostDescription", "PAM").ToStdString());
    pml::nmos::NodeApi::Get().GetSelf().AddInternalClock("clk0");
    pml::nmos::NodeApi::Get().GetSelf().AddInterface("eth0");
    pml::nmos::NodeApi::Get().SetHeartbeatTime(5000);

    auto pDevice = std::make_shared<pml::nmos::Device>(chHost, "Live555", pml::nmos::Device::GENERIC,pml::nmos::NodeApi::Get().GetSelf().GetId());

    auto pSource = std::make_shared<pml::nmos::SourceAudio>(chHost, "Live555", pDevice->GetId());
    pSource->AddChannel("Left", "L");
    pSource->AddChannel("Right", "R");

    m_pFlow = std::make_shared<pml::nmos::FlowAudioRaw>(chHost, "Live555", pSource->GetId(), pDevice->GetId(), 48000, pml::nmos::FlowAudioRaw::L24);
    int nPackeTime = Settings::Get().Read("Server", "PacketTime", 1000);
    switch(nPackeTime)
    {
    case 125:
        m_pFlow->SetPacketTime(pml::nmos::FlowAudioRaw::US_125);
        break;
    case 250:
        m_pFlow->SetPacketTime(pml::nmos::FlowAudioRaw::US_250);
        break;
    case 333:
        m_pFlow->SetPacketTime(pml::nmos::FlowAudioRaw::US_333);
        break;
    case 4000:
        m_pFlow->SetPacketTime(pml::nmos::FlowAudioRaw::US_4000);
        break;
    default:
        m_pFlow->SetPacketTime(pml::nmos::FlowAudioRaw::US_1000);
        break;
    }

    m_pFlow->SetMediaClkOffset(Settings::Get().Read("AoIP_Settings", "Epoch", 0));    //@todo get this from Live555

    m_pSender = std::make_shared<pml::nmos::Sender>(chHost, "Live555 sender", m_pFlow->GetId(), pml::nmos::Sender::RTP_MCAST, pDevice->GetId(), "eth0");
    m_pSender->SetDestinationDetails(Settings::Get().Read("Server", "DestinationIp",wxEmptyString).ToStdString(),
                                     Settings::Get().Read("Server", "RTP_Port", 5004));

    m_pReceiver = std::make_shared<pml::nmos::Receiver>(chHost, "Live555 receiver", pml::nmos::Receiver::RTP_MCAST, pDevice->GetId(), pml::nmos::Receiver::AUDIO,
                                                pml::nmos::TransportParamsRTP::CORE | pml::nmos::TransportParamsRTP::MULTICAST);
    m_pReceiver->AddCap("audio/L24");
    m_pReceiver->AddCap("audio/L16");
    m_pReceiver->AddInterfaceBinding("eth0");

    if(pml::nmos::NodeApi::Get().AddDevice(pDevice) == false)
    {
        pmlLog(pml::LOG_ERROR) << "NMOS\tFailed to add Device";
    }
    if(pml::nmos::NodeApi::Get().AddSource(pSource) == false)
    {
        pmlLog(pml::LOG_ERROR) << "NMOS\tFailed to add Source";
    }
    if(pml::nmos::NodeApi::Get().AddFlow(m_pFlow) == false)
    {
        pmlLog(pml::LOG_ERROR) << "NMOS\tFailed to add Flow";
    }
    pml::nmos::NodeApi::Get().AddSender(m_pSender);
    pml::nmos::NodeApi::Get().AddReceiver(m_pReceiver);

    pml::nmos::NodeApi::Get().Commit();



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
                pml::nmos::NodeApi::Get().RemoveSender(m_pSender->GetId());
                pml::nmos::NodeApi::Get().AddReceiver(m_pReceiver);
                m_pReceiver->MasterEnable(true);
                m_pInputPanel->SetReceiverId(m_pReceiver->GetId());
                pml::nmos::NodeApi::Get().Commit();
                break;
            case NODE_SENDER:
                pml::nmos::NodeApi::Get().RemoveReceiver(m_pReceiver->GetId());
                pml::nmos::NodeApi::Get().AddSender(m_pSender);
                m_pSender->MasterEnable(true);
                m_pInputPanel->SetReceiverId("");
                pml::nmos::NodeApi::Get().Commit();
                break;
            case NODE_BOTH:
                pml::nmos::NodeApi::Get().AddSender(m_pSender);
                pml::nmos::NodeApi::Get().AddReceiver(m_pReceiver);
                m_pReceiver->MasterEnable(true);
                m_pSender->MasterEnable(true);
                m_pInputPanel->SetReceiverId(m_pReceiver->GetId());
                pml::nmos::NodeApi::Get().Commit();
                break;
        }

        if(m_nNodeMode == NODE_OFF)
        {
            pmlLog() << "NMOS\tStart NMOS Services " << nMode;
            pml::nmos::NodeApi::Get().StartServices();
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
            pmlLog() << "NMOS: NODE UPDATED " << event.GetValue((long)NODE_OFF);
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
            pml::nmos::NodeApi::Get().Commit();
        }
        if(m_pFlow && event.GetKey() == "PacketTime")
        {
            switch(event.GetValue(long(1000)))
            {
            case 125:
                m_pFlow->SetPacketTime(pml::nmos::FlowAudioRaw::US_125);
                break;
            case 250:
                m_pFlow->SetPacketTime(pml::nmos::FlowAudioRaw::US_250);
                break;
            case 333:
                m_pFlow->SetPacketTime(pml::nmos::FlowAudioRaw::US_333);
                break;
            case 4000:
                m_pFlow->SetPacketTime(pml::nmos::FlowAudioRaw::US_4000);
                break;
            default:
                m_pFlow->SetPacketTime(pml::nmos::FlowAudioRaw::US_1000);
                break;
            }
            pml::nmos::NodeApi::Get().Commit();
        }
        if(m_pSender && event.GetKey()=="Stream")
        {
            //m_pSender->MasterEnable(event.GetValue(false));
            pml::nmos::NodeApi::Get().Commit();
        }
    }
}



void NmosManager::OnTarget(wxNmosNodeConnectionEvent& event)
{

    auto itInterface = Settings::Get().GetInterfaces().find("eth0");
    if(itInterface != Settings::Get().GetInterfaces().end())
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
        pml::nmos::NodeApi::Get().TargetTaken(itInterface->second.ToStdString(), event.GetPort(), true);
    }
}

void NmosManager::OnPatchSender(wxNmosNodeConnectionEvent& event)
{
    auto pSender = pml::nmos::NodeApi::Get().GetSender(event.GetResourceId().ToStdString());
    if(pSender)
    {
        std::string sSourceIp(pSender->GetStaged().tpSender.sSourceIp);
        if(sSourceIp.empty() || sSourceIp == "auto")
        {
            sSourceIp = Settings::Get().Read("Server", "RTSP_Address", wxEmptyString).ToStdString();
            if(sSourceIp.empty())
            {
                 if(Settings::Get().GetInterfaces().empty() == false)
                 {
                     sSourceIp = Settings::Get().GetInterfaces().begin()->second.ToStdString();
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
        pml::nmos::NodeApi::Get().SenderPatchAllowed(event.GetPort(), true, pSender->GetId(), sSourceIp, sDestinationIp);
        if(event.GetSenderConnection().eActivate == pml::nmos::connection::ACT_NOW)
        {
            ActivateSender(pSender);
        }
    }
    else
    {
        pml::nmos::NodeApi::Get().SenderPatchAllowed(event.GetPort(), false, event.GetResourceId().ToStdString(), "", "");
    }

}

void NmosManager::OnPatchReceiver(wxNmosNodeConnectionEvent& event)
{
    auto pReceiver = pml::nmos::NodeApi::Get().GetReceiver(event.GetResourceId().ToStdString());
    if(pReceiver)
    {
        auto itInterface = Settings::Get().GetInterfaces().find("eth0");
        if(itInterface != Settings::Get().GetInterfaces().end())
        {
            //Chage the AOIP source to be NMOS IS-04
            pml::nmos::NodeApi::Get().ReceiverPatchAllowed(event.GetPort(), true, pReceiver->GetId(), itInterface->second.ToStdString());

            if(event.GetReceiverConnection().eActivate == pml::nmos::connection::ACT_NOW)
            {
                ActivateReceiver(pReceiver);
            }
        }
    }
    else
    {
        pml::nmos::NodeApi::Get().SenderPatchAllowed(event.GetPort(), false, event.GetResourceId().ToStdString(),"","");
    }
    // @todo Set the receiver stuff - maybe changes the Live555 settings

}

void NmosManager::OnSenderActivated(wxNmosNodeConnectionEvent& event)
{
    pmlLog() << "OnSenderActivated " << event.GetResourceId().ToStdString();
    auto pSender = pml::nmos::NodeApi::Get().GetSender(event.GetResourceId().ToStdString());
    if(pSender)
    {
        ActivateSender(pSender);
    }
}


void NmosManager::ActivateSender(std::shared_ptr<pml::nmos::Sender> pSender)
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
             if(Settings::Get().GetInterfaces().empty() == false)
             {
                 Settings::Get().Write("Server", "RTSP_Address", Settings::Get().GetInterfaces().begin()->second);
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

void NmosManager::OnReceiverActivated(wxNmosNodeConnectionEvent& event)
{
    // @todo move this somewhere else...
    auto pReceiver = pml::nmos::NodeApi::Get().GetReceiver(event.GetResourceId().ToStdString());
    if(pReceiver)
    {
        ActivateReceiver(pReceiver);

    }
}

void NmosManager::ActivateReceiver(std::shared_ptr<pml::nmos::Receiver> pReceiver)
{
    if(pReceiver)
    {

        auto itInterface = Settings::Get().GetInterfaces().find("eth0");
        if(itInterface != Settings::Get().GetInterfaces().end())
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

            AoipSourceManager::Get().EditSource(nInput, (nInput == AoipSourceManager::SOURCE_NMOS_A ? "NMOS_A" : "NMOS_B"), sSender);
            if(m_pInputPanel)
            {
                m_pInputPanel->SetSender(sSender);
            }
        }
    }
}

void NmosManager::StopNode()
{
    pmlLog() << "NMOS\tStop NMOS Services";
    pml::nmos::NodeApi::Get().StopServices();
}


void NmosManager::StartClient(int nMode)
{
    if(m_nClientMode != nMode)
    {
        if(nMode != CLIENT_OFF && m_nClientMode == CLIENT_OFF)
        {
            pml::nmos::ClientApi::Get().SetPoster(std::make_shared<wxClientApiPoster>(this));
            pml::nmos::ClientApi::Get().AddQuerySubscription(pml::nmos::ClientApi::ALL, "",0);
            pml::nmos::ClientApi::Get().Start();
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
    pml::nmos::ClientApi::Get().Stop();
}



void NmosManager::OnNmosReceiverChanged(wxNmosClientReceiverEvent& event)
{
    if(m_pInputPanel)
    {
        for(auto pUpdated : event.GetUpdated())
        {
            if(pUpdated->GetId() == m_pInputPanel->GetReceiverId())
            {
                //is the receiver one of ours??
                if(pUpdated->GetSender().empty() == false)
                {
                    m_pInputPanel->SetSender(pUpdated->GetSender());
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
            if(pml::nmos::NodeApi::Get().GetSender(pSender->GetId()) == nullptr)
            {  //not one of our senders
                auto itFlow = pml::nmos::ClientApi::Get().FindFlow(pSender->GetFlowId());
                if(itFlow != pml::nmos::ClientApi::Get().GetFlows().end())
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
            if(pml::nmos::NodeApi::Get().GetSender(pSender->GetId()) == nullptr)
            {   //not one of our senders
                auto itFlow = pml::nmos::ClientApi::Get().FindFlow(pSender->GetFlowId());
                if(itFlow != pml::nmos::ClientApi::Get().GetFlows().end())
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
        for(auto pFlow : event.GetAdded())
        {
            if(pFlow->GetFormat().find("urn:x-nmos:format:audio") != std::string::npos)
            {
                for(auto itSender = m_mmLonelySender.lower_bound(pFlow->GetId()); itSender != m_mmLonelySender.upper_bound(pFlow->GetId()); ++itSender)
                {
                    m_pInputPanel->AddSender(itSender->second);
                }
                //remove the senders which are no longer lonely
                m_mmLonelySender.erase(pFlow->GetId());
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


void NmosManager::AddHandlerToEventPoster(wxEvtHandler* pHandler)
{

    m_pPoster->AddHandler(pHandler);
}

#endif // __NMOS__
