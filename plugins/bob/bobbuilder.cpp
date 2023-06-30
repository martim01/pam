#include "bobbuilder.h"
#include <wx/panel.h>
#include "settings.h"
#include "settingevent.h"
#include "version.h"
#include "bobpanel.h"


using namespace std;

bobBuilder::bobBuilder() : MonitorPluginBuilder()
{

    RegisterForSettingsUpdates(this);
    
    Bind(wxEVT_SETTING_CHANGED, &bobBuilder::OnSettingChanged, this);

}

void bobBuilder::SetAudioData(const timedbuffer* pBuffer)
{
	m_pMeter->SetAudioData(pBuffer);
}

wxWindow* bobBuilder::CreateMonitorPanel(wxWindow* pParent)
{
    m_pMeter = new bobPanel(pParent,this);
	return m_pMeter;

}

list<pairOptionPanel_t> bobBuilder::CreateOptionPanels(wxWindow* pParent)
 {
    return {};
}




void bobBuilder::LoadSettings()
{
	//@todo load any settings
}


void bobBuilder::InputSession(const session& aSession)
{
	m_pMeter->InputSession(aSession);
}

void bobBuilder::OutputChannels(const std::vector<char>& vChannels)
{
	m_pMeter->OutputChannels(vChannels);
}


void bobBuilder::OnSettingChanged(SettingEvent& event)
{
    //@todo a setting registered for has changed. Handle it
}




