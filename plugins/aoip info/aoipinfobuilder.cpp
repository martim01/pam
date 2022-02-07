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
    RegisterForSettingsUpdates(wxT("Graph"), this);
    RegisterForSettingsUpdates(wxT("Type"), this);

    Connect(wxID_ANY, wxEVT_SETTING_CHANGED, (wxObjectEventFunction)&AoIPInfoBuilder::OnSettingChanged);

    RegisterRemoteApiEnum("QoS", {{50, "50 ms"},{100, "100 ms"},{250, "250 ms"},{500, "500 ms"},{1000, "1000 ms"},{5000, "5000 ms"}});
    RegisterRemoteApiEnum("Graph", {"kBit/s", "Packet Gap", "Packet Loss", "Jitter", "Timestamp", "Timestamp Errors", "TS-DF", "Slip"});
    RegisterRemoteApiEnum("Type", {"Line Graph/s", "Bar Chart", "Histogram"});



}

void AoIPInfoBuilder::SetAudioData(const timedbuffer* pBuffer)
{
    m_pInfo->SetAudioData(pBuffer);
}

wxWindow* AoIPInfoBuilder::CreateMonitorPanel(wxWindow* pParent)
{
    m_pInfo = new pnlAoIPInfo(pParent, this);
    return m_pInfo;
}

list<pairOptionPanel_t> AoIPInfoBuilder::CreateOptionPanels(wxWindow* pParent)
{
    list<pairOptionPanel_t> lstOptionPanels;

    lstOptionPanels.push_back(make_pair(wxT("Graph"), new pnlGraph(pParent, this)));
    lstOptionPanels.push_back(make_pair(wxT("Settings"), new pnlSettings(pParent, this)));

    return lstOptionPanels;
}




void AoIPInfoBuilder::LoadSettings()
{
    if(m_pInfo)
    {
        m_pInfo->ShowGraph(ReadSetting(wxT("Graph"), wxT("kBit/s")));
        m_pInfo->SetGraphType(ReadSetting("Type", "Graph"));
    }

}

void AoIPInfoBuilder::InputSession(const session& aSession)
{
    m_pInfo->SessionStarted(aSession);
}

void AoIPInfoBuilder::OutputChannels(const std::vector<char>& vChannels)
{
    //m_pMeters->OutputChannels(vChannels);
}


void AoIPInfoBuilder::OnSettingChanged(SettingEvent& event)
{
    if(event.GetKey() == wxT("Graph"))
    {
        m_pInfo->ShowGraph(event.GetValue());
    }
    else if(event.GetKey() == "Type")
    {
        m_pInfo->SetGraphType(event.GetValue());
    }
}

void AoIPInfoBuilder::SetQoSData(qosData* pData)
{
    m_pInfo->QoSUpdated(pData);
}


bool AoIPInfoBuilder::CanBeMaximized() const
{
    return false;
}


void AoIPInfoBuilder::ClearGraphs()
{
    m_pInfo->ClearGraphs();
}

void AoIPInfoBuilder::RecalculateRange()
{
    m_pInfo->RecalculateRange();
}

void AoIPInfoBuilder::ChangeGranularity(int nWhich)
{
    m_pInfo->ChangeGranularity(nWhich);
}

void AoIPInfoBuilder::ChangeResolution(int nWhich)
{
    m_pInfo->ChangeResolution(nWhich);
}
