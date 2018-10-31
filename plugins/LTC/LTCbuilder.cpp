#include "LTCbuilder.h"
#include <wx/panel.h>
#include "settings.h"
#include "settingevent.h"
#include "version.h"
#include "LTCpanel.h"

using namespace std;

LTCBuilder::LTCBuilder() : TestPluginBuilder()
{

    //RegisterForSettingsUpdates(wxT("Mode"), this);
    
    Connect(wxID_ANY, wxEVT_SETTING_CHANGED, (wxObjectEventFunction)&LTCBuilder::OnSettingChanged);

}

void LTCBuilder::SetAudioData(const timedbuffer* pBuffer)
{
	m_pMeter->SetAudioData(pBuffer);
}

wxWindow* LTCBuilder::CreateTestPanel(wxWindow* pParent)
{
	m_pMeter = new LTCPanel(pParent, this);
	return m_pMeter;

}


void LTCBuilder::LoadSettings()
{
	//@todo load any settings
}


void LTCBuilder::InputSession(const session& aSession)
{
	m_pMeter->InputSession(aSession);
}

void LTCBuilder::OutputChannels(const std::vector<char>& vChannels)
{
	m_pMeter->OutputChannels(vChannels);
}


void LTCBuilder::OnSettingChanged(SettingEvent& event)
{
    //@todo a setting registered for has changed. Handle it
}





