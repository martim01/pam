#include "fftdiffbuilder.h"
#include <wx/panel.h>
#include "settings.h"
#include "settingevent.h"
#include "version.h"
@panel#include "fftdiffpanel.h"
@meter#include "fftdiffmeter.h"

using namespace std;

fftdiffBuilder::fftdiffBuilder() : MonitorPluginBuilder()
{

    //RegisterForSettingsUpdates(wxT("Mode"), this);
    
    Connect(wxID_ANY, wxEVT_SETTING_CHANGED, (wxObjectEventFunction)&fftdiffBuilder::OnSettingChanged);

}

void fftdiffBuilder::SetAudioData(const timedbuffer* pBuffer)
{
	m_pMeter->SetAudioData(pBuffer);
}

wxWindow* fftdiffBuilder::CreateMonitorPanel(wxWindow* pParent)
{
    m_pMeter = new fftdiff(pParent,this);
	return m_pMeter;

}

list<pairOptionPanel_t> fftdiffBuilder::CreateOptionPanels(wxWindow* pParent)
{
    list<pairOptionPanel_t> lstOptionPanels;

    //@todo create and return all the option panels
    return lstOptionPanels;
}




void fftdiffBuilder::LoadSettings()
{
	//@todo load any settings
}


void fftdiffBuilder::InputSession(const session& aSession)
{
	m_pMeter->InputSession(aSession);
}

void fftdiffBuilder::OutputChannels(const std::vector<char>& vChannels)
{
	m_pMeter->OutputChannels(vChannels);
}


void fftdiffBuilder::OnSettingChanged(SettingEvent& event)
{
    //@todo a setting registered for has changed. Handle it
}




