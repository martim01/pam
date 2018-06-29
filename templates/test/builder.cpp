#include "[CLASS_PREFIX]builder.h"
#include <wx/panel.h>
#include "settings.h"
#include "settingevent.h"
#include "version.h"
@panel#include "[CLASS_PREFIX]panel.h"
@meter#include "[CLASS_PREFIX]meter.h"

using namespace std;

[CLASS_PREFIX]Builder::[CLASS_PREFIX]Builder() : TestPluginBuilder()
{

    //RegisterForSettingsUpdates(wxT("Mode"), this);
    
    Connect(wxID_ANY, wxEVT_SETTING_CHANGED, (wxObjectEventFunction)&[CLASS_PREFIX]Builder::OnSettingChanged);

}

void [CLASS_PREFIX]Builder::SetAudioData(const timedbuffer* pBuffer)
{
	m_pMeter->SetAudioData(pBuffer);
}

wxWindow* [CLASS_PREFIX]Builder::CreateTestPanel(wxWindow* pParent)
{
@meter	m_pMeter = new [CLASS_PREFIX]Meter(pParent,this);
@panel	m_pMeter = new [CLASS_PREFIX]Panel(pParent, this);
	return m_pMeter;

}


void [CLASS_PREFIX]Builder::LoadSettings()
{
	//@todo load any settings
}


void [CLASS_PREFIX]Builder::InputSession(const session& aSession)
{
	m_pMeter->InputSession(aSession);
}

void [CLASS_PREFIX]Builder::OutputChannels(const std::vector<char>& vChannels)
{
	m_pMeter->OutputChannels(vChannels);
}


void [CLASS_PREFIX]Builder::OnSettingChanged(SettingEvent& event)
{
    //@todo a setting registered for has changed. Handle it
}




