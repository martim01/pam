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
    m_pNodePoster(std::make_shared<wxEventPoster>(this)),
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
    Settings::Get().AddHandler("Time", "Grandmaster", this);

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

    m_pClientPoster = std::make_shared<wxClientApiPoster>(this);

    pml::nmos::NodeApi::Get().Init(m_pNodePoster, Settings::Get().Read("NMOS", "Port_Discovery", 8080),
                        Settings::Get().Read("NMOS", "Port_Connection", 8080),
                        Settings::Get().Read("NMOS", "HostLabel", wxGetHostName()).ToStdString(),
                        Settings::Get().Read("NMOS", "HostDescription", "PAM").ToStdString());
    pml::nmos::NodeApi::Get().GetSelf().AddInternalClock("clk0");
    pml::nmos::NodeApi::Get().GetSelf().AddInterface("eth0");
    pml::nmos::NodeApi::Get().SetHeartbeatTime(5000);

    auto pDevice = std::make_shared<pml::nmos::Device>(wxString(wxGetHostName()+"/PAM").ToStdString(), "Live555", pml::nmos::Device::GENERIC,pml::nmos::NodeApi::Get().GetSelf().GetId());

    m_pSource = std::make_shared<pml::nmos::SourceAudio>(wxString(wxGetHostName()+"/PAM/Live555").ToStdString(), "Live555", pDevice->GetId());
    m_pSource->AddChannels({{channelSymbol("L"), channelLabel("Left")},{channelSymbol("R"), channelLabel("Right")}});

    m_pFlow = std::make_shared<pml::nmos::FlowAudioRaw>(wxString(wxGetHostName()+"/PAM/Live555/L24").ToStdString(), "Live555", m_pSource->GetId(), pDevice->GetId(), 48000, pml::nmos::FlowAudioRaw::L24);
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

    m_pFlow->SetMediaClkOffset(Settings::Get().Read("AoIP_Settings", "Epoch", 0));

    m_pSender = std::make_shared<pml::nmos::Sender>(wxString(wxGetHostName()+"/PAM/Live555/Sender").ToStdString(), "Live555 sender", m_pFlow->GetId(), pml::nmos::Sender::RTP_MCAST, pDevice->GetId(), "eth0");
    m_pSender->SetDestinationDetails(Settings::Get().Read("Server", "DestinationIp",wxEmptyString).ToStdString(),
                                     Settings::Get().Read("Server", "RTP_Port", 5004));

    m_pReceiver = std::make_shared<pml::nmos::Receiver>(wxString(wxGetHostName()+"/PAM/Live555/Receiver").ToStdString(), "Live555 receiver", pml::nmos::Receiver::RTP_MCAST, pDevice->GetId(), pml::nmos::Receiver::AUDIO,
                                                pml::nmos::TransportParamsRTP::CORE | pml::nmos::TransportParamsRTP::MULTICAST);
    m_pReceiver->AddCaps({"audio/L24","audio/L16"});
    m_pReceiver->AddInterfaceBinding("eth0");

    if(pml::nmos::NodeApi::Get().AddDevice(pDevice) == false)
    {
        pmlLog(pml::LOG_ERROR) << "NMOS\tFailed to add Device";
    }
    if(pml::nmos::NodeApi::Get().AddSource(m_pSource) == false)
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
    Bind(wxEVT_NMOS_CLIENT_SUBSCRIBE, &NmosManager::OnNmosSubscribeRequest, this);


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
    else if(event.GetSection() == "Time")
    {
        if(event.GetKey() == "Grandmaster")
        {
            pml::nmos::NodeApi::Get().GetSelf().RemoveClock("clk1");
            if(event.GetValue().empty() == false)
            {
                pml::nmos::NodeApi::Get().GetSelf().AddPTPClock("clk1", true, "IEEE1588-2008", event.GetValue().ToStdString(), true);
                m_pSource->SetClock("clk1");
            }
            else
            {
                m_pSource->SetClock("clk0");
            }
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
    if(event.GetSenderConnection())
    {
        std::string sSourceIp(event.GetSenderConnection()->GetTransportParams()[0].GetSourceIp());
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


        if(event.GetSenderConnection()->GetReceiverId() && (*(event.GetSenderConnection()->GetReceiverId())).empty() == false)
        {
            pmlLog() << "Patch Sender: ReceiverId = '" << *(event.GetSenderConnection()->GetReceiverId()) << "'";

            //receiverid so unicast. -- @todo if we are already multicasting what do we do here????
            if(event.GetSenderConnection()->GetTransportParams()[0].GetDestinationIp().empty() == false && event.GetSenderConnection()->GetTransportParams()[0].GetDestinationIp() != "auto")
            {   //one would hope ip has been set. so set the live555 address to it
                sDestinationIp = event.GetSenderConnection()->GetTransportParams()[0].GetDestinationIp();
                pmlLog() << "Patch Sender: DestinationIp set by receiver = '" << sDestinationIp << "'";
            }
            else
            {   // @todo no ip address set. Should we try to find what the receiver ip address is from the clientapi??
                pmlLog() << "Patch Sender: DestinationIp set by no-one = '" << sDestinationIp << "'";
            }
        }
        else
        {
             //no receiver so multicast
            if(event.GetSenderConnection()->GetTransportParams()[0].GetDestinationIp().empty() == false && event.GetSenderConnection()->GetTransportParams()[0].GetDestinationIp() != "auto")
            {   //address has been chosen by nmos
                sDestinationIp = event.GetSenderConnection()->GetTransportParams()[0].GetDestinationIp();

                pmlLog() << "Patch Sender: DestinationIp set by receiver = '" << sDestinationIp << "'";
            }
            else //if(Settings::Get().Read("Server", "DestinationIp", wxEmptyString).empty())
            {   //address not chosen by nmos and pam hasn't chosen one yet. create rando,
                sDestinationIp = IOManager::Get().GetRandomMulticastAddress().ToStdString();

                pmlLog() << "Patch Sender: DestinationIp set by us = '" << sDestinationIp << "'";
            }
        }

        // @todo Set the sender stuff - maybe changes the Live555 settings
        pml::nmos::NodeApi::Get().SenderPatchAllowed(event.GetPort(), true, event.GetResourceId(), sSourceIp, sDestinationIp);
        if(event.GetSenderConnection()->GetConstActivation().GetMode() == pml::nmos::activation::ACT_NOW)
        {
            ActivateSender(event.GetResourceId());
        }
    }
    else
    {
        pml::nmos::NodeApi::Get().SenderPatchAllowed(event.GetPort(), false, event.GetResourceId(), "", "");
    }
}

void NmosManager::OnPatchReceiver(wxNmosNodeConnectionEvent& event)
{
    if(event.GetReceiverConnection())
    {
        auto itInterface = Settings::Get().GetInterfaces().find("eth0");
        if(itInterface != Settings::Get().GetInterfaces().end())
        {
            pml::nmos::NodeApi::Get().ReceiverPatchAllowed(event.GetPort(), true, event.GetResourceId(), itInterface->second.ToStdString());

            if(event.GetReceiverConnection()->GetConstActivation().GetMode() == pml::nmos::activation::ACT_NOW)
            {
                ActivateReceiver(event.GetResourceId());
            }
        }
    }
    else
    {
        pml::nmos::NodeApi::Get().ReceiverPatchAllowed(event.GetPort(), false, event.GetResourceId(),"");
    }
    // @todo Set the receiver stuff - maybe changes the Live555 settings
}

void NmosManager::OnSenderActivated(wxNmosNodeConnectionEvent& event)
{
    pmlLog() << "OnSenderActivated " << event.GetResourceId();
    ActivateSender(event.GetResourceId());
}


void NmosManager::ActivateSender(const std::string& sId)
{
    pmlLog() << "NmosManager::ActivateSender";
    auto pSender = pml::nmos::NodeApi::Get().GetSender(sId);
    if(pSender)
    {
        if(pSender->GetDestinationIp().empty() == false)
        {   //address has been chosen by nmos
            Settings::Get().Write("Server", "DestinationIp", pSender->GetDestinationIp());
            pmlLog() << "ActivateSender: DestinationIp set '" << pSender->GetDestinationIp() << "'";
        }
        else
        {
            if(pSender->GetStaged().GetReceiverId())
            {   // @todo no ip address set. Should we try to find what the receiver ip address is from the clientapi??
                pmlLog() << "ActivateSender: DestinationIp not set but ReceiverId is set";
            }
            else
            {   //address not chosen by nmos and pam hasn't chosen one yet. create rando,
                Settings::Get().Write("Server", "DestinationIp", IOManager::Get().GetRandomMulticastAddress());
                pmlLog() << "ActivateSender: DestinationIp set by random";

            }
        }

        //if the source interface is set then change our source ip. else do the opposites
        std::string sSourceIp(pSender->GetStaged().GetTransportParams()[0].GetSourceIp());
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

        if(pSender->GetStaged().GetTransportParams()[0].GetDestinationPort() != 0)
        {
            Settings::Get().Write("Server", "RTP_Port", pSender->GetStaged().GetTransportParams()[0].GetDestinationPort());
        }

        if(pSender->IsStagedMasterEnabled() == true)
        {
            Settings::Get().Write("Output", "Destination", "AoIP");
        }
        Settings::Get().Write("Server", "Stream", "AlwaysOn");

        IOManager::Get().RestartStream();
    }
}

void NmosManager::OnReceiverActivated(wxNmosNodeConnectionEvent& event)
{
    // @todo move this somewhere else...
    ActivateReceiver(event.GetResourceId());
}

void NmosManager::ActivateReceiver(const std::string& sId)
{
    auto pReceiver = pml::nmos::NodeApi::Get().GetReceiver(sId);
    if(pReceiver && pReceiver->GetStaged().GetTransportFileType() && (*pReceiver->GetStaged().GetTransportFileType()) == "application/sdp" && pReceiver->GetStaged().GetTransportFileData())
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
            AoipSourceManager::Get().SetSourceSDP(nInput, *(pReceiver->GetStaged().GetTransportFileData()));

            //Chage the AOIP source to be NMOS IS-04
            wxString sSender;
            if(pReceiver->IsStagedMasterEnabled())
            {
                //Make sure the input type is AoIP
                Settings::Get().Write("Input", "Type", "AoIP");
                Settings::Get().Write("Input", "AoIP", nInput); //write the new source number in
                if(pReceiver->GetStaged().GetSenderId())
                {
                    sSender = wxString(*(pReceiver->GetStaged().GetSenderId()));
                }
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
            pml::nmos::ClientApi::Get().SetPoster(m_pClientPoster);
            pml::nmos::ClientApi::Get().AddQuerySubscription(pml::nmos::ClientApi::NODES, "",0);
            pml::nmos::ClientApi::Get().AddQuerySubscription(pml::nmos::ClientApi::DEVICES, "",0);
            pml::nmos::ClientApi::Get().AddQuerySubscription(pml::nmos::ClientApi::SOURCES, "",0);
            pml::nmos::ClientApi::Get().AddQuerySubscription(pml::nmos::ClientApi::FLOWS, "",0);
            pml::nmos::ClientApi::Get().AddQuerySubscription(pml::nmos::ClientApi::RECEIVERS, "",0);
            pml::nmos::ClientApi::Get().AddQuerySubscription(pml::nmos::ClientApi::SENDERS, "",0);
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
        for(auto pUpdated : event.GetChanges().lstUpdated)
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
        for(auto pSender : event.GetChanges().lstAdded)
        {
            if(pml::nmos::NodeApi::Get().GetSender(pSender->GetId()) == nullptr)    //make sure sender isn't us
            {  //not one of our senders
                auto pFlow = pml::nmos::ClientApi::Get().FindFlow(pSender->GetFlowId());
                if(pFlow)
                {
                    if(pFlow->GetFormat().find("urn:x-nmos:format:audio") != std::string::npos)
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
        for(auto pSender : event.GetChanges().lstUpdated)
        {
            if(pml::nmos::NodeApi::Get().GetSender(pSender->GetId()) == nullptr)
            {   //not one of our senders
                auto pFlow = pml::nmos::ClientApi::Get().FindFlow(pSender->GetFlowId());
                if(pFlow)
                {
                    if(pFlow->GetFormat().find("urn:x-nmos:format:audio") != std::string::npos)
                    {
                        m_pInputPanel->UpdateSender(pSender);
                    }
                }
            }
        }
        m_pInputPanel->RemoveSenders(event.GetChanges().lstRemoved);
    }
}

void NmosManager::OnNmosFlowChanged(wxNmosClientFlowEvent& event)
{
    if(m_pInputPanel)
    {
        for(auto pFlow : event.GetChanges().lstAdded)
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


void NmosManager::OnNmosSubscribeRequest(wxNmosClientRequestEvent& event)
{
    /*if(m_pInputPanel)
    {
        m_pInputPanel->SubscriptionRequest(event.GetResourceId(), event.GetResponse(), event.GetResult());
    }*/
}


void NmosManager::AddHandlerToEventPoster(wxEvtHandler* pHandler)
{
    m_pNodePoster->AddHandler(pHandler);
}

void NmosManager::AddHandlerToClientEventPoster(wxEvtHandler* pHandler)
{
    m_pClientPoster->AddHandler(pHandler);
}

#endif // __NMOS__
