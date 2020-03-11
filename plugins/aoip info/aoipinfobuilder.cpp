#include "aoipinfobuilder.h"
#include <wx/panel.h>
#include "settings.h"
#include "settingevent.h"
#include "pnlAoIPInfo.h"
#include "pnlGraph.h"
#include "pnlSettings.h"
#include "version.h"


using namespace std;

AoIPInfoBuilder::AoIPInfoBuilder() : MonitorPluginBuilder(),
m_pInfo(0)
{

    RegisterForSettingsUpdates(wxT("Graph"), this);

    Connect(wxID_ANY, wxEVT_SETTING_CHANGED, (wxObjectEventFunction)&AoIPInfoBuilder::OnSettingChanged);

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
        m_pInfo->ShowGraph(ReadSetting(event.GetKey(), wxT("kBit/s")));
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


