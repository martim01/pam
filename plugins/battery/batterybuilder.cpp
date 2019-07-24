#include "batterybuilder.h"
#include <wx/panel.h>
#include "settings.h"
#include "settingevent.h"
#include "version.h"
#include "batterypanel.h"

using namespace std;

batteryBuilder::batteryBuilder() : MonitorPluginBuilder()
{

    //RegisterForSettingsUpdates(wxT("Mode"), this);
    
    Connect(wxID_ANY, wxEVT_SETTING_CHANGED, (wxObjectEventFunction)&batteryBuilder::OnSettingChanged);

}

void batteryBuilder::SetAudioData(const timedbuffer* pBuffer)
{
	m_pMeter->SetAudioData(pBuffer);
}

wxWindow* batteryBuilder::CreateMonitorPanel(wxWindow* pParent)
{
	m_pMeter = new batteryPanel(pParent, this);
	return m_pMeter;

}

list<pairOptionPanel_t> batteryBuilder::CreateOptionPanels(wxWindow* pParent)
{
    list<pairOptionPanel_t> lstOptionPanels;

    //@todo create and return all the option panels
    return lstOptionPanels;
}




void batteryBuilder::LoadSettings()
{
	//@todo load any settings
}


void batteryBuilder::InputSession(const session& aSession)
{
	m_pMeter->InputSession(aSession);
}

void batteryBuilder::OutputChannels(const std::vector<char>& vChannels)
{
	m_pMeter->OutputChannels(vChannels);
}


void batteryBuilder::OnSettingChanged(SettingEvent& event)
{
    //@todo a setting registered for has changed. Handle it
}





