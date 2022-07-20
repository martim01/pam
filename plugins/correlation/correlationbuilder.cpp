#include "correlationbuilder.h"
#include <wx/panel.h>
#include "settings.h"
#include "settingevent.h"
#include "version.h"
#include "correlationpanel.h"

using namespace std;

correlationBuilder::correlationBuilder() : MonitorPluginBuilder()
{

    RegisterForSettingsUpdates(this);

    Bind(wxEVT_SETTING_CHANGED, &correlationBuilder::OnSettingChanged, this);

}

void correlationBuilder::SetAudioData(const timedbuffer* pBuffer)
{
	m_pMeter->SetAudioData(pBuffer);
}

wxWindow* correlationBuilder::CreateMonitorPanel(wxWindow* pParent)
{
    m_pMeter = new correlationPanel(pParent,this);
	return m_pMeter;

}

list<pairOptionPanel_t> correlationBuilder::CreateOptionPanels(wxWindow* pParent)
{
    list<pairOptionPanel_t> lstOptionPanels;

    //@todo create and return all the option panels
    return lstOptionPanels;
}




void correlationBuilder::LoadSettings()
{
	//@todo load any settings
}


void correlationBuilder::InputSession(const session& aSession)
{
	m_pMeter->InputSession(aSession);
}

void correlationBuilder::OutputChannels(const std::vector<char>& vChannels)
{
	m_pMeter->OutputChannels(vChannels);
}


void correlationBuilder::OnSettingChanged(SettingEvent& event)
{
    //@todo a setting registered for has changed. Handle it
}




