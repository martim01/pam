#include "aoipinfobuilder.h"
#include <wx/panel.h>
#include "settings.h"
#include "settingevent.h"
#include "pnlAoIPInfo.h"
#include "pnlGraph.h"
#include "pnlSettings.h"
#include "remoteapi.h"

using namespace std;

AoIPInfoBuilder::AoIPInfoBuilder() : MonitorPluginBuilder(),
m_pInfo(0)
{
    m_bWantsAudioAlways = true;
    m_bWantsQoSAlways = true;
    RegisterForSettingsUpdates(this);


    Connect(wxID_ANY, wxEVT_SETTING_CHANGED, (wxObjectEventFunction)&AoIPInfoBuilder::OnSettingChanged);

    RegisterRemoteApiEnum("QoS", {{50, "50 ms"},{100, "100 ms"},{250, "250 ms"},{500, "500 ms"},{1000, "1000 ms"},{5000, "5000 ms"}}, 1000);

}

void AoIPInfoBuilder::SetAudioData(const timedbuffer* pBuffer)
{
    m_pInfo->SetAudioData(pBuffer);
    if(WebsocketsActive())
    {
        SendWebsocketMessage(CreateWebsocketMessage(pBuffer));
    }
}

wxWindow* AoIPInfoBuilder::CreateMonitorPanel(wxWindow* pParent)
{
    m_pInfo = new pnlAoIPInfo(pParent, this);
    return m_pInfo;
}

list<pairOptionPanel_t> AoIPInfoBuilder::CreateOptionPanels(wxWindow* pParent)
{
    list<pairOptionPanel_t> lstOptionPanels;

    lstOptionPanels.push_back(make_pair(wxT("Settings"), new pnlSettings(pParent, this)));

    return lstOptionPanels;
}




void AoIPInfoBuilder::LoadSettings()
{
}

void AoIPInfoBuilder::InputSession(const session& aSession)
{
    m_pInfo->SessionStarted(aSession);
    if(WebsocketsActive())
    {
        SendWebsocketMessage(CreateWebsocketMessage(aSession));
    }
}

void AoIPInfoBuilder::OutputChannels(const std::vector<char>& vChannels)
{
    //m_pMeters->OutputChannels(vChannels);
}


void AoIPInfoBuilder::OnSettingChanged(SettingEvent& event)
{
}

void AoIPInfoBuilder::SetQoSData(qosData* pData)
{
    m_pInfo->QoSUpdated(pData);
    if(WebsocketsActive())
    {
        SendWebsocketMessage(CreateWebsocketMessage(pData));
    }
}


bool AoIPInfoBuilder::CanBeMaximized() const
{
    return false;
}


void AoIPInfoBuilder::ClearGraphs()
{
}

void AoIPInfoBuilder::RecalculateRange()
{
}

void AoIPInfoBuilder::ChangeGranularity(int nWhich)
{
    m_pInfo->ChangeGranularity(nWhich);
}

void AoIPInfoBuilder::ChangeResolution(int nWhich)
{
    m_pInfo->ChangeResolution(nWhich);
}

Json::Value AoIPInfoBuilder::CreateWebsocketMessage(qosData* pData)
{
    Json::Value jsMessage;
    if(pData)
    {
        jsMessage["qos"]["time"] = pData->tsTime.Format("%H:%M:%S:%l").ToStdString();
        jsMessage["qos"]["kbits"]["average"] = pData->dkbits_per_second_Av;
        jsMessage["qos"]["kbits"]["current"] = pData->dkbits_per_second_Now;
        jsMessage["qos"]["kbits"]["max"] = pData->dkbits_per_second_max;
        jsMessage["qos"]["kbits"]["min"] = pData->dkbits_per_second_min;

        jsMessage["qos"]["lost"]["total"] = pData->nTotNumPacketsLost;
        jsMessage["qos"]["lost"]["average"] = pData->dPacket_loss_fraction_av;
        jsMessage["qos"]["lost"]["max"] = pData->dPacket_loss_fraction_max;
        jsMessage["qos"]["lost"]["min"] = pData->dPacket_loss_fraction_min;

        jsMessage["qos"]["packets"]["total"] = pData->nTotNumPacketsReceived;

        jsMessage["qos"]["interpacket"]["min"] = pData->dInter_packet_gap_ms_min;
        jsMessage["qos"]["interpacket"]["max"] = pData->dInter_packet_gap_ms_max;
        jsMessage["qos"]["interpacket"]["average"] = pData->dInter_packet_gap_ms_av;
        jsMessage["qos"]["interpacket"]["current"] = pData->dInter_packet_gap_ms_Now;

        jsMessage["qos"]["jitter"] = pData->dJitter;
        jsMessage["qos"]["ts-df"] = pData->dTSDF;


        wxDateTime dtSR(time_t(pData->tvLastSR_Time.tv_sec));
        dtSR.SetMillisecond(pData->tvLastSR_Time.tv_usec/1000);

        if(dtSR.IsValid())
        {
            jsMessage["qos"]["SR"] = dtSR.Format("%H:%M:%S:%l").ToStdString();
        }
        else
        {
            jsMessage["qos"]["SR"] = "na";
        }

        wxDateTime dtNTP(time_t(pData->tvSync.tv_sec));
        dtNTP.SetMillisecond(pData->tvSync.tv_usec/1000);
        if(dtNTP.IsValid())
        {
            jsMessage["qos"]["NTP"] = dtNTP.Format("%H:%M:%S:%l").ToStdString();
        }
        else
        {
            jsMessage["qos"]["NTP"] = "na";
        }

        jsMessage["qos"]["timestamp"]["errors"] = pData->nTimestampErrors;

    }
    return jsMessage;
}

Json::Value AoIPInfoBuilder::CreateWebsocketMessage(const timedbuffer* pBuffer)
{
    Json::Value jsMessage;
    jsMessage["audio"]["timestamp"] = pBuffer->GetTimestamp();

    jsMessage["audio"]["time"]["transmitted"] = wxString::Format("%s:%03ld", wxDateTime(time_t(pBuffer->GetTransmissionTime().tv_sec)).Format("%Y-%m-%d %H:%M:%S").c_str(), pBuffer->GetTransmissionTime().tv_usec).ToStdString();
    jsMessage["audio"]["time"]["received"] = wxString::Format("%s:%03ld", wxDateTime(time_t(pBuffer->GetTimeVal().tv_sec)).Format("%Y-%m-%d %H:%M:%S").c_str(), pBuffer->GetTimeVal().tv_usec).ToStdString();
    jsMessage["audio"]["time"]["played"] = wxString::Format("%s:%03ld", wxDateTime(time_t(pBuffer->GetPlaybackTime().tv_sec)).Format("%Y-%m-%d %H:%M:%S").c_str(), pBuffer->GetPlaybackTime().tv_usec).ToStdString();

    jsMessage["audio"]["time"]["latency"] = m_pInfo->GetLatency();
    jsMessage["audio"]["time"]["slip"] = m_pInfo->GetSlip();

    jsMessage["audio"]["frame"]["size"] = pBuffer->GetDuration();
    jsMessage["audio"]["frame"]["duration"] = static_cast<double>(pBuffer->GetDuration())/static_cast<double>(m_pInfo->GetSampleRate()*m_pInfo->GetFrameSize())*1e6;

    return jsMessage;

}


Json::Value AoIPInfoBuilder::CreateWebsocketMessage(const session& aSession)
{
    Json::Value jsMessage;
    jsMessage["session"]["name"] = aSession.sName.ToStdString();
    jsMessage["session"]["type"] = aSession.sType.ToStdString();

    jsMessage["session"]["refclock"]["type"] = aSession.refClock.sType.ToStdString();
    jsMessage["session"]["refclock"]["version"] = aSession.refClock.sVersion.ToStdString();
    jsMessage["session"]["refclock"]["id"] = aSession.refClock.sId.ToStdString();
    jsMessage["session"]["refclock"]["domain"] = (int)aSession.refClock.nDomain;

    jsMessage["session"]["sdp"] = aSession.sRawSDP.ToStdString();

    jsMessage["session"]["subsession"] = Json::Value(Json::arrayValue);
    if(aSession.GetCurrentSubsession() != aSession.lstSubsession.end())
    {
        Json::Value jsSub;
        jsSub["id"] = aSession.GetCurrentSubsession()->sId.ToStdString();
        jsSub["source"] = aSession.GetCurrentSubsession()->sSourceAddress.ToStdString();
        jsSub["codec"] = aSession.GetCurrentSubsession()->sCodec.ToStdString();
        jsSub["samplerate"] = aSession.GetCurrentSubsession()->nSampleRate/1000.0;
        jsSub["channels"] = aSession.GetCurrentSubsession()->nChannels;

        jsSub["sync_timestamp"] = aSession.GetCurrentSubsession()->nSyncTimestamp;

        jsSub["refclock"]["type"] = aSession.GetCurrentSubsession()->refClock.sType.ToStdString();
        jsSub["refclock"]["version"] = aSession.GetCurrentSubsession()->refClock.sVersion.ToStdString();
        jsSub["refclock"]["id"] = aSession.GetCurrentSubsession()->refClock.sId.ToStdString();
        jsSub["refclock"]["domain"] = (int)aSession.GetCurrentSubsession()->refClock.nDomain;

        jsMessage["subsession"].append(jsSub);
    }
    return jsMessage;
}
