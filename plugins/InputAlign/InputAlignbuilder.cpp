#include "InputAlignbuilder.h"
#include <wx/panel.h>
#include "settings.h"
#include "settingevent.h"
#include "InputAlignpanel.h"

using namespace std;

InputAlignBuilder::InputAlignBuilder() : TestPluginBuilder()
{

    //RegisterForSettingsUpdates(wxT("Mode"), this);

    Connect(wxID_ANY, wxEVT_SETTING_CHANGED, (wxObjectEventFunction)&InputAlignBuilder::OnSettingChanged);

}

void InputAlignBuilder::SetAudioData(const timedbuffer* pBuffer)
{
	m_pMeter->SetAudioData(pBuffer);
}

wxWindow* InputAlignBuilder::CreateTestPanel(wxWindow* pParent)
{
	m_pMeter = new InputAlignPanel(pParent, this);
	return m_pMeter;

}


void InputAlignBuilder::LoadSettings()
{
	//@todo load any settings
}


void InputAlignBuilder::InputSession(const session& aSession)
{
	m_pMeter->InputSession(aSession);
}

void InputAlignBuilder::OutputChannels(const std::vector<char>& vChannels)
{
	m_pMeter->OutputChannels(vChannels);
}


void InputAlignBuilder::OnSettingChanged(SettingEvent& event)
{
    //@todo a setting registered for has changed. Handle it
}





