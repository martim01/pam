#include "waveformbuilder.h"
#include <wx/panel.h>
#include "settings.h"
#include "settingevent.h"
#include "waveformmeter.h"

using namespace std;

waveformBuilder::waveformBuilder() : MonitorPluginBuilder()
{

    //RegisterForSettingsUpdates(wxT("Mode"), this);

    Connect(wxID_ANY, wxEVT_SETTING_CHANGED, (wxObjectEventFunction)&waveformBuilder::OnSettingChanged);

}

void waveformBuilder::SetAudioData(const timedbuffer* pBuffer)
{
	m_pMeter->SetAudioData(pBuffer);
}

wxWindow* waveformBuilder::CreateMonitorPanel(wxWindow* pParent)
{
	m_pMeter = new waveformMeter(pParent,this);
	return m_pMeter;

}

list<pairOptionPanel_t> waveformBuilder::CreateOptionPanels(wxWindow* pParent)
{
    list<pairOptionPanel_t> lstOptionPanels;

    //@todo create and return all the option panels
    return lstOptionPanels;
}




void waveformBuilder::LoadSettings()
{
	//@todo load any settings
}


void waveformBuilder::InputSession(const session& aSession)
{
	m_pMeter->InputSession(aSession);
}

void waveformBuilder::OutputChannels(const std::vector<char>& vChannels)
{
	m_pMeter->OutputChannels(vChannels);
}


void waveformBuilder::OnSettingChanged(SettingEvent& event)
{
    //@todo a setting registered for has changed. Handle it
}





