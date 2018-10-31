#include "bufferlogbuilder.h"
#include <wx/panel.h>
#include "settings.h"
#include "settingevent.h"
#include "version.h"
#include "bufferlogpanel.h"

using namespace std;

bufferlogBuilder::bufferlogBuilder() : TestPluginBuilder()
{

    //RegisterForSettingsUpdates(wxT("Mode"), this);
    m_pMeter = 0;
    Connect(wxID_ANY, wxEVT_SETTING_CHANGED, (wxObjectEventFunction)&bufferlogBuilder::OnSettingChanged);

}

void bufferlogBuilder::SetAudioData(const timedbuffer* pBuffer)
{
    if(m_pMeter)
	{
	    m_pMeter->SetAudioData(pBuffer);
	}
}

wxWindow* bufferlogBuilder::CreateTestPanel(wxWindow* pParent)
{
	m_pMeter = new bufferlogPanel(pParent, this);
	return m_pMeter;

}


void bufferlogBuilder::LoadSettings()
{
	//@todo load any settings
}


void bufferlogBuilder::InputSession(const session& aSession)
{
	m_pMeter->InputSession(aSession);
}

void bufferlogBuilder::OutputChannels(const std::vector<char>& vChannels)
{
	m_pMeter->OutputChannels(vChannels);
}


void bufferlogBuilder::OnSettingChanged(SettingEvent& event)
{
    //@todo a setting registered for has changed. Handle it
}





