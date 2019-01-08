#include "lineupbuilder.h"
#include <wx/panel.h>
#include "settings.h"
#include "settingevent.h"
#include "version.h"
#include "lineuppanel.h"

using namespace std;

lineupBuilder::lineupBuilder() : TestPluginBuilder()
{

    //RegisterForSettingsUpdates(wxT("Mode"), this);
    
    Connect(wxID_ANY, wxEVT_SETTING_CHANGED, (wxObjectEventFunction)&lineupBuilder::OnSettingChanged);

}

void lineupBuilder::SetAudioData(const timedbuffer* pBuffer)
{
	m_pMeter->SetAudioData(pBuffer);
}

wxWindow* lineupBuilder::CreateTestPanel(wxWindow* pParent)
{
	m_pMeter = new lineupPanel(pParent, this);
	return m_pMeter;

}


void lineupBuilder::LoadSettings()
{
	//@todo load any settings
}


void lineupBuilder::InputSession(const session& aSession)
{
	m_pMeter->InputSession(aSession);
}

void lineupBuilder::OutputChannels(const std::vector<char>& vChannels)
{
	m_pMeter->OutputChannels(vChannels);
}


void lineupBuilder::OnSettingChanged(SettingEvent& event)
{
    //@todo a setting registered for has changed. Handle it
}





