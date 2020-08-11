#ifdef __NMOS__


#include "nmos.h"
#include "libnmos.h"
#include "wxeventposter.h"
#include "wxclientapiposter.h"
#include "wxnmosclientevent.h"
#include "settings.h"
#include "settingevent.h"
#include "log.h"


NmosManager::NmosManager(pnlSettingsNmos* pPnl) :
    m_pSettingsPanel(pPnl)
    m_pFlow(0),
    m_pSender(0)
{

    Settings::Get().AddHandler(wxT("NMOS"), wxT("Activate"), this);
    Settings::Get().AddHandler(wxT("NMOS"), wxT("Client"), this);

    Settings::Get().AddHandler(wxT("AoIP"),wxT("Epoch"), this);
    Settings::Get().AddHandler(wxT("Server"), wxT("DestinationIp"), this);
    Settings::Get().AddHandler(wxT("Server"), wxT("RTP_Port"), this);
    Settings::Get().AddHandler(wxT("Server"), wxT("PacketTime"), this);
    Settings::Get().AddHandler(wxT("Server"), wxT("Stream"), this);
}



void NmosManager::Setup()
{


    char chHost[256];
    gethostname(chHost, 256);


    NodeApi::Get().Init(std::make_shared<wxEventPoster>(this), Settings::Get().Read(wxT("NMOS"), wxT("Port_Discovery"), 8080),
                        Settings::Get().Read(wxT("NMOS"), wxT("Port_Connection"), 8080),
                        string(Settings::Get().Read(wxT("NMOS"), wxT("HostLabel"), wxEmptyString).mb_str()),
                        string(Settings::Get().Read(wxT("NMOS"), wxT("HostDescription"), wxT("PAM")).mb_str()));
    NodeApi::Get().GetSelf().AddInternalClock("clk0");
    NodeApi::Get().GetSelf().AddInterface("eth0");

    shared_ptr<Device> pDevice = make_shared<Device>(chHost, "Live555", Device::GENERIC,NodeApi::Get().GetSelf().GetId());

    wxLogDebug(wxT("Make Source"));
    shared_ptr<SourceAudio> pSource = make_shared<SourceAudio>(chHost, "Live555", pDevice->GetId());
    pSource->AddChannel("Left", "L");
    pSource->AddChannel("Right", "R");

    wxLogDebug(wxT("Make Flow"));
    m_pFlow = make_shared<FlowAudioRaw>(chHost, "Live555", pSource->GetId(), pDevice->GetId(), 48000, FlowAudioRaw::L24);
    int nPackeTime = Settings::Get().Read(wxT("Server"), wxT("PacketTime"), 1000);
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


    m_pFlow->SetMediaClkOffset(Settings::Get().Read(wxT("AoIP_Settings"), wxT("Epoch"), 0));    //@todo get this from Live555

    m_pSender = make_shared<Sender>(chHost, "Live555 sender", m_pFlow->GetId(), Sender::RTP_MCAST, pDevice->GetId(), "eth0");
    m_pSender->SetDestinationDetails(std::string(Settings::Get().Read(wxT("Server"), wxT("DestinationIp"),wxEmptyString).mbc_str()), Settings::Get().Read(wxT("Server"), wxT("RTP_Port"), 5004));

    shared_ptr<Receiver> pReceiver = make_shared<Receiver>(chHost, "Live555 receiver", Receiver::RTP_MCAST, pDevice->GetId(), Receiver::AUDIO, TransportParamsRTP::CORE | TransportParamsRTP::MULTICAST);
    pReceiver->AddCap("audio/L24");
    pReceiver->AddCap("audio/L16");
    pReceiver->AddInterfaceBinding("eth0");

    if(NodeApi::Get().AddDevice(pDevice) == false)
    {
        pml::Log::Get(pml::Log::LOG_ERROR) << "NMOS\tFailed to add Device" << std::endl;
    }
    if(NodeApi::Get().AddSource(pSource) == false)
    {
        pml::Log::Get(pml::Log::LOG_ERROR) << "NMOS\tFailed to add Source" << std::endl;
    }
    if(NodeApi::Get().AddFlow(m_pFlow) == false)
    {
        pml::Log::Get(pml::Log::LOG_ERROR) << "NMOS\tFailed to add Flow" << std::endl;
    }
    if(NodeApi::Get().AddReceiver(pReceiver) == false)
    {
        pml::Log::Get(pml::Log::LOG_ERROR) << "NMOS\tFailed to add Receiver" << std::endl;
    }
    else
    {
        m_pSettingsPanel->SetReceiverId(wxString::FromUTF8(pReceiver->GetId().c_str()));
    }
    if(NodeApi::Get().AddSender(m_pSender) == false)
    {
        pml::Log::Get(pml::Log::LOG_ERROR) << "NMOS\tFailed to add Sender" << std::endl;
    }
    NodeApi::Get().Commit();



    Connect(wxID_ANY, wxEVT_NMOS_TARGET, (wxObjectEventFunction)&NmosManager::OnTarget);
    Connect(wxID_ANY, wxEVT_NMOS_PATCH_SENDER, (wxObjectEventFunction)&NmosManager::OnPatchSender);
    Connect(wxID_ANY, wxEVT_NMOS_PATCH_RECEIVER, (wxObjectEventFunction)&NmosManager::OnPatchReceiver);
    Connect(wxID_ANY, wxEVT_NMOS_ACTIVATE_RECEIVER, (wxObjectEventFunction)&NmosManager::OnReceiverActivated);
    Connect(wxID_ANY, wxEVT_NMOS_ACTIVATE_SENDER, (wxObjectEventFunction)&NmosManager::OnSenderActivated);
    Connect(wxID_ANY, wxEVT_NMOS_CLIENT_SENDER, (wxObjectEventFunction)&NmosManager::OnNmosSenderChanged);
    Connect(wxID_ANY, wxEVT_NMOS_CLIENT_RECEIVER, (wxObjectEventFunction)&NmosManager::OnNmosReceiverChanged);
    Connect(wxID_ANY, wxEVT_NMOS_CLIENT_FLOW, (wxObjectEventFunction)&NmosManager::OnNmosFlowChanged);
    Connect(wxID_ANY, wxEVT_NMOS_CLIENTCURL_SUBSCRIBE, (wxObjectEventFunction)&NmosManager::OnNmosSubscribeRequest);


    if(Settings::Get().Read(wxT("NMOS"), wxT("Activate"),false) == true)
    {
        StartNode();
    }
    if(Settings::Get().Read(wxT("NMOS"), wxT("Client"),false) == true)
    {
        StartClient();
    }
}


void NmosManager::StartNode()
{
    pml::Log::Get() << "NMOS\tStart NMOS Services" << std::endl;

    NodeApi::Get().StartServices();
}




void NmosManager::OnSettingChanged(wxSettingEvent& event)
{
    if(event.GetSection() == wxT("NMOS"))
    {
        if(event.GetKey() == wxT("Activate"))
        {
            if(event.GetValue(false))
            {
                StartNmos();
            }
            else
            {
                StopNmos();
            }
        }
        else if(event.GetKey() == wxT("Client"))
        {
            if(event.GetValue(false))
            {
                StartNmosClient();
            }
            else
            {
                StopNmosClient();
            }
        }
    }
    else if(event.GetSection() == wxT("AoIP_Settings") && event.GetKey() == wxT("Epoch"))
    {
        if(m_pFlow && m_pSender)
        {
            m_pFlow->SetMediaClkOffset(event.GetValue((long(0))));
            m_pSender->CreateSDP();
        }
    }
    else if(event.GetSection() == wxT("Server"))
    {
        if(m_pSender && (event.GetKey() == wxT("DestinationIp") || event.GetKey() == wxT("RTP_Port")))
        {
            m_pSender->SetDestinationDetails(std::string(Settings::Get().Read(wxT("Server"), wxT("DestinationIp"),wxEmptyString).mbc_str()),
                                             Settings::Get().Read(wxT("Server"), wxT("RTP_Port"), 5004));
            NodeApi::Get().Commit();
        }
        if(m_pFlow && event.GetKey() == wxT("PacketTime"))
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
        if(m_pSender && event.GetKey()==wxT("Stream"))
        {
            m_pSender->MasterEnable(event.GetValue(false));
            NodeApi::Get().Commit();
        }
    }
}



void NmosManager::OnTarget(wxNmosEvent& event)
{
    // @todo move this somewhere else...
    multimap<wxString, wxString> mmButtons(Settings::Get().GetInterfaces());
    multimap<wxString, wxString>::iterator itInterface = mmButtons.find(wxT("eth0"));
    if(itInterface != mmButtons.end())
    {

        wxString sSdp = event.GetTransportFile();
        sSdp.Replace(wxT("\n"), wxT("`"));
        sSdp.Replace(wxT("\r"), wxT(""));

        wxString sInput(wxT("NMOS_IS-04_A"));
        //Save the SDP file details
        if(Settings::Get().Read(wxT("Input"), wxT("AoIP"), wxEmptyString) == sInput)
        {
            sInput = wxT("NMOS_IS-04_B");
        }
        Settings::Get().Write(wxT("AoIP"), sInput, wxString::Format(wxT("NMOS:[%s]"),sSdp.c_str()));
        //Make sure the input type is AoIP
        Settings::Get().Write(wxT("Input"), wxT("Type"), wxT("AoIP"));
        //Chage the AOIP source to be NMOS IS-04
        Settings::Get().Write(wxT("Input"), wxT("AoIP"), sInput);

        //Now tell NMOS that we have taken the target
        NodeApi::Get().TargetTaken(string(itInterface->second.mb_str()), event.GetPort(), true);
    }
}

void NmosManager::OnPatchSender(wxNmosEvent& event)
{
    shared_ptr<Sender> pSender = NodeApi::Get().GetSender(string(event.GetString().mb_str()));
    if(pSender)
    {
        string sSourceIp(pSender->GetStaged().tpSender.sSourceIp);
        if(sSourceIp.empty() || sSourceIp == "auto")
        {
            sSourceIp = string(Settings::Get().Read(wxT("Server"), wxT("RTSP_Address"), wxEmptyString).mb_str());
            if(sSourceIp.empty())
            {
                 multimap<wxString, wxString> mmButtons(Settings::Get().GetInterfaces());
                 if(mmButtons.empty() == false)
                 {
                     sSourceIp = string(mmButtons.begin()->second.mb_str());
                 }
            }
        }

        string sDestinationIp;

        if(pSender->GetStaged().sReceiverId.empty())
        {   //no receiver so multicast
            if(pSender->GetStaged().tpSender.sDestinationIp.empty() == false && pSender->GetStaged().tpSender.sDestinationIp != "auto")
            {   //address has been chosen by nmos
                sDestinationIp = pSender->GetStaged().tpSender.sDestinationIp;
            }
            else if(Settings::Get().Read(wxT("Server"), wxT("DestinationIp"), wxEmptyString).empty())
            {   //address not chosen by nmos and pam hasn't chosen one yet. create rando,
                sDestinationIp = string(IOManager::Get().GetRandomMulticastAddress().c_str());
            }
        }
        else
        {   //receiverid so unicast. -- @todo if we are already multicasting what do we do here????
            if(pSender->GetStaged().tpSender.sDestinationIp.empty() == false && pSender->GetStaged().tpSender.sDestinationIp != "auto")
            {   //one would hope ip has been set. so set the live555 address to it
                sDestinationIp = pSender->GetStaged().tpSender.sDestinationIp;
            }
            else
            {   // @todo no ip address set. Should we try to find what the receiver ip address is from the clientapi??

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
        NodeApi::Get().SenderPatchAllowed(event.GetPort(), false, string(event.GetResourceId().mb_str()), "", "");
    }

}

void NmosManager::OnPatchReceiver(wxNmosEvent& event)
{
    shared_ptr<Receiver> pReceiver = NodeApi::Get().GetReceiver(string(event.GetString().mb_str()));
    if(pReceiver)
    {
        multimap<wxString, wxString> mmButtons(Settings::Get().GetInterfaces());
        multimap<wxString, wxString>::iterator itInterface = mmButtons.find(wxT("eth0"));
        if(itInterface != mmButtons.end())
        {
            wxString sSdp = wxString::FromAscii(pReceiver->GetStaged().sTransportFileData.c_str());
            sSdp.Replace(wxT("\n"), wxT("`"));
            sSdp.Replace(wxT("\r"), wxT(""));

            wxString sInput(wxT("NMOS_IS-04_A"));
            //Save the SDP file details
            if(Settings::Get().Read(wxT("Input"), wxT("AoIP"), wxEmptyString) == sInput)
            {
                sInput = wxT("NMOS_IS-04_B");
            }

            //Chage the AOIP source to be NMOS IS-04
            wxString sSender;
            if(pReceiver->GetStaged().bMasterEnable)
            {
                //Make sure the input type is AoIP
                sSender = wxString::FromAscii(pReceiver->GetStaged().sSenderId.c_str());
            }
            NodeApi::Get().ReceiverPatchAllowed(event.GetPort(), true, pReceiver->GetId(), string(itInterface->second.mb_str()));

            if(event.GetReceiverConnection().eActivate == connection::ACT_NOW)
            {
                ActivateReceiver(pReceiver);
            }
        }
    }
    else
    {
        NodeApi::Get().SenderPatchAllowed(event.GetPort(), false, string(event.GetResourceId().mb_str()),"","");
    }
    // @todo Set the receiver stuff - maybe changes the Live555 settings

}

void NmosManager::OnSenderActivated(wxNmosEvent& event)
{
    shared_ptr<Sender> pSender = NodeApi::Get().GetSender(string(event.GetString().mb_str()));
    if(pSender)
    {
        ActivateSender(pSender);
    }
}


void NmosManager::ActivateSender(shared_ptr<Sender> pSender)
{
    if(pSender->GetStaged().sReceiverId.empty())
    {   //no receiver so multicast
        if(pSender->GetStaged().tpSender.sDestinationIp.empty() == false && pSender->GetStaged().tpSender.sDestinationIp != "auto")
        {   //address has been chosen by nmos
            Settings::Get().Write(wxT("Server"), wxT("DestinationIp"), wxString::FromAscii(pSender->GetStaged().tpSender.sDestinationIp.c_str()));
        }
        else if(Settings::Get().Read(wxT("Server"), wxT("DestinationIp"), wxEmptyString).empty())
        {   //address not chosen by nmos and pam hasn't chosen one yet. create rando,
            Settings::Get().Write(wxT("Server"), wxT("DestinationIp"), IOManager::Get().GetRandomMulticastAddress());
        }
    }
    else
    {   //receiverid so unicast. -- @todo if we are already multicasting what do we do here????
        if(pSender->GetStaged().tpSender.sDestinationIp.empty() == false && pSender->GetStaged().tpSender.sDestinationIp != "auto")
        {   //one would hope ip has been set. so set the live555 address to it
            Settings::Get().Write(wxT("Server"), wxT("DestinationIp"), wxString::FromAscii(pSender->GetStaged().tpSender.sDestinationIp.c_str()));
        }
        else
        {   // @todo no ip address set. Should we try to find what the receiver ip address is from the clientapi??

        }
    }

    //if the source interface is set then change our source ip. else do the opposites
    string sSourceIp(pSender->GetStaged().tpSender.sSourceIp);
    if(sSourceIp.empty() == false && sSourceIp != "auto")
    {   //sourceip set by nmos
        Settings::Get().Write(wxT("Server"), wxT("RTSP_Address"), wxString::FromAscii(sSourceIp.c_str()));
    }
    else
    {
        sSourceIp = string(Settings::Get().Read(wxT("Server"), wxT("RTSP_Address"), wxEmptyString).mb_str());
        if(sSourceIp.empty())
        {
             multimap<wxString, wxString> mmButtons(Settings::Get().GetInterfaces());
             if(mmButtons.empty() == false)
             {
                 Settings::Get().Write(wxT("Server"), wxT("RTSP_Address"), mmButtons.begin()->second);
             }
        }
    }

    if(pSender->GetStaged().tpSender.nDestinationPort != 0)
    {
        Settings::Get().Write(wxT("Server"), wxT("RTP_Port"), pSender->GetStaged().tpSender.nDestinationPort);
    }

    if(pSender->GetStaged().bMasterEnable == true)
    {
        Settings::Get().Write(wxT("Output"), wxT("Destination"), wxT("AoIP"));
    }
    Settings::Get().Write(wxT("Server"), wxT("Stream"), pSender->GetStaged().bMasterEnable);
}

void NmosManager::OnReceiverActivated(wxNmosEvent& event)
{
    // @todo move this somewhere else...
    shared_ptr<Receiver> pReceiver = NodeApi::Get().GetReceiver(string(event.GetString().mb_str()));
    if(pReceiver)
    {
        ActivateReceiver(pReceiver);

    }
}

void NmosManager::ActivateReceiver(shared_ptr<Receiver> pReceiver)
{
    if(pReceiver)
    {
        multimap<wxString, wxString> mmButtons(Settings::Get().GetInterfaces());
        multimap<wxString, wxString>::iterator itInterface = mmButtons.find(wxT("eth0"));
        if(itInterface != mmButtons.end())
        {
            wxString sSdp = wxString::FromAscii(pReceiver->GetStaged().sTransportFileData.c_str());
            sSdp.Replace(wxT("\n"), wxT("`"));
            sSdp.Replace(wxT("\r"), wxT(""));

            wxString sInput(wxT("NMOS_IS-04_A"));
            //Save the SDP file details
            if(Settings::Get().Read(wxT("Input"), wxT("AoIP"), wxEmptyString) == sInput)
            {
                sInput = wxT("NMOS_IS-04_B");
            }
            Settings::Get().Write(wxT("AoIP"), sInput, wxString::Format(wxT("NMOS:[%s]"),sSdp.c_str()));


            //Chage the AOIP source to be NMOS IS-04
            wxString sSender;
            if(pReceiver->GetStaged().bMasterEnable)
            {
                //Make sure the input type is AoIP
                Settings::Get().Write(wxT("Input"), wxT("Type"), wxT("AoIP"));
                sSender = wxString::FromAscii(pReceiver->GetStaged().sSenderId.c_str());
                Settings::Get().Write(wxT("Input"), wxT("AoIP"), sInput);
            }
            else
            {
                Settings::Get().Write(wxT("Input"), wxT("Type"), wxT("Disabled"));
            }
            if(m_pSettingsPanel)
            {
                m_pSettingsPanel->SetSender(sSender);
            }
        }
    }
}

void NmosManager::StopNmos()
{
    pml::Log::Get() << "NMOS\tStop NMOS Services" << std::endl;
    NodeApi::Get().StopServices();
    Settings::Get().RemoveKey(wxT("AoIP"), wxT("NMOS_IS-04"));
}


void NmosManager::StartNmosClient()
{
    ClientApi::Get().SetPoster(make_shared<wxClientApiPoster>(this));
    ClientApi::Get().AddQuerySubscription(ClientApi::ALL, "",0);
    ClientApi::Get().Start();
}

void NmosManager::StopNmosClient()
{
    ClientApi::Get().Stop();
}



void pam2Dialog::OnNmosReceiverChanged(wxNmosClientReceiverEvent& event)
{
    if(m_pSettingsPanel)
    {
        for(list<shared_ptr<Receiver> >::const_iterator itUpdated = event.GetUpdated().begin(); itUpdated != event.GetUpdated().end(); ++itUpdated)
        {
            if((*itUpdated)->GetId() == m_pSettingsPanel->GetReceiverId())
            {
                //is the receiver one of ours??
                if((*itUpdated)->GetSender().empty() == false)
                {
                    m_pSettingsPanel->SetSender(wxString::FromAscii((*itUpdated)->GetSender().c_str()));
                }
                else
                {
                    m_pSettingsPanel->SetSender(wxEmptyString);
                }
                break;
            }
        }
    }
}

void NmosManager::OnNmosSenderChanged(wxNmosClientSenderEvent& event)
{
    if(m_pSettingsPanel)
    {
        for(list<shared_ptr<Sender> >::const_iterator itAdded = event.GetAdded().begin(); itAdded != event.GetAdded().end(); ++itAdded)
        {
            if(NodeApi::Get().GetSender((*itAdded)->GetId()) == 0)
            {  //not one of our senders
                map<string, shared_ptr<Flow> >::const_iterator itFlow = ClientApi::Get().FindFlow((*itAdded)->GetFlowId());
                if(itFlow != ClientApi::Get().GetFlowEnd())
                {
                    if(itFlow->second->GetFormat().find("urn:x-nmos:format:audio") != string::npos)
                    {
                        m_pSettingsPanel->AddSender((*itAdded));
                    }
                }
            }
            else
            {
                //Store the sender for checking once we get a flow...
                m_mmLonelySender.insert(make_pair((*itAdded)->GetFlowId(), (*itAdded)));
            }
        }
        for(list<shared_ptr<Sender> >::const_iterator itUpdated = event.GetUpdated().begin(); itUpdated != event.GetUpdated().end(); ++itUpdated)
        {
            if(NodeApi::Get().GetSender((*itUpdated)->GetId()) == 0)
            {   //not one of our senders
                map<string, shared_ptr<Flow> >::const_iterator itFlow = ClientApi::Get().FindFlow((*itUpdated)->GetFlowId());
                if(itFlow != ClientApi::Get().GetFlowEnd())
                {
                    if(itFlow->second->GetFormat().find("urn:x-nmos:format:audio") != string::npos)
                    {
                        m_pSettingsPanel->UpdateSender((*itUpdated));
                    }
                }
            }
        }
        // @todo remove senders
    }
}

void NmosManager::OnNmosFlowChanged(wxNmosClientFlowEvent& event)
{
    if(m_pSettingsPanel)
    {
        for(list<shared_ptr<Flow> >::const_iterator itAdded = event.GetAdded().begin(); itAdded != event.GetAdded().end(); ++itAdded)
        {
            if((*itAdded)->GetFormat().find("urn:x-nmos:format:audio") != string::npos)
            {
                for(multimap<string, shared_ptr<Sender> >::const_iterator itSender = m_mmLonelySender.lower_bound((*itAdded)->GetId()); itSender != m_mmLonelySender.upper_bound((*itAdded)->GetId()); ++itSender)
                {
                    m_pSettingsPanel->AddSender(itSender->second);
                }
                //remove the senders which are no longer lonely
                m_mmLonelySender.erase((*itAdded)->GetId());
            }
        }

    }
}


void NmosManager::OnNmosSubscribeRequest(wxNmosClientCurlEvent& event)
{
    if(m_pSettingsPanel)
    {
        m_pSettingsPanel->SubscriptionRequest(event.GetResourceId(), event.GetResponse(), event.GetResult());
    }
}
#endif // __NMOS__
