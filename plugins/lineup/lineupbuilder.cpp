#include "lineupbuilder.h"
#include <wx/panel.h>
#include "settings.h"
#include "settingevent.h"
#include "lineuppanel.h"

using namespace std;

lineupBuilder::lineupBuilder() : TestPluginBuilder()
{

    RegisterForSettingsUpdates(this, "reset");

    Connect(wxID_ANY, wxEVT_SETTING_CHANGED, (wxObjectEventFunction)&lineupBuilder::OnSettingChanged);

    RegisterRemoteApiEnum("reset", {{0,""}, {1,"Reset"}},0);
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
    if(event.GetKey() == "reset")
    {
        if(event.GetValue(0l) == 1)
        {
            m_pMeter->Reset();
        }
    }
}





