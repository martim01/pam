#include "MP3Streamerbuilder.h"
#include <wx/panel.h>
#include "settings.h"
#include "settingevent.h"
#include "MP3Streamerpanel.h"

using namespace std;

MP3StreamerBuilder::MP3StreamerBuilder() : MonitorPluginBuilder()
{
    m_bWantsAudioAlways = true;

    //RegisterForSettingsUpdates(wxT("Mode"), this);

    Connect(wxID_ANY, wxEVT_SETTING_CHANGED, (wxObjectEventFunction)&MP3StreamerBuilder::OnSettingChanged);

}

void MP3StreamerBuilder::SetAudioData(const timedbuffer* pBuffer)
{
	m_pMeter->SetAudioData(pBuffer);
}

wxWindow* MP3StreamerBuilder::CreateMonitorPanel(wxWindow* pParent)
{
	m_pMeter = new MP3StreamerPanel(pParent, this);
	return m_pMeter;

}

list<pairOptionPanel_t> MP3StreamerBuilder::CreateOptionPanels(wxWindow* pParent)
{
    list<pairOptionPanel_t> lstOptionPanels;

    //@todo create and return all the option panels
    return lstOptionPanels;
}




void MP3StreamerBuilder::LoadSettings()
{
	//@todo load any settings
}


void MP3StreamerBuilder::InputSession(const session& aSession)
{
	m_pMeter->InputSession(aSession);
}

void MP3StreamerBuilder::OutputChannels(const std::vector<char>& vChannels)
{
	m_pMeter->OutputChannels(vChannels);
}


void MP3StreamerBuilder::OnSettingChanged(SettingEvent& event)
{
    //@todo a setting registered for has changed. Handle it
}





