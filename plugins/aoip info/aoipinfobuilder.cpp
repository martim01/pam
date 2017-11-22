#include "aoipinfobuilder.h"
#include <wx/panel.h>
#include "settings.h"
#include "settingevent.h"
#include "pnlAoIPInfo.h"

using namespace std;

AoIPInfoBuilder::AoIPInfoBuilder() : MonitorPluginBuilder(),
m_pInfo(0)
{

//    RegisterForSettingsUpdates(wxT("Shading"), this);

    Connect(wxID_ANY, wxEVT_SETTING_CHANGED, (wxObjectEventFunction)&AoIPInfoBuilder::OnSettingChanged);

}

void AoIPInfoBuilder::SetAudioData(const timedbuffer* pBuffer)
{
    m_pInfo->SetAudioData(pBuffer);
}

wxWindow* AoIPInfoBuilder::CreateMonitorPanel(wxWindow* pParent)
{
    m_pInfo = new pnlAoIPInfo(pParent);
    return m_pInfo;
}

list<pairOptionPanel_t> AoIPInfoBuilder::CreateOptionPanels(wxWindow* pParent)
{
    list<pairOptionPanel_t> lstOptionPanels;

    return lstOptionPanels;
}




void AoIPInfoBuilder::LoadSettings()
{


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

}

void AoIPInfoBuilder::SetQoSData(qosData* pData)
{
    m_pInfo->QoSUpdated(pData);
}


bool AoIPInfoBuilder::CanBeMaximized() const
{
    return false;
}
