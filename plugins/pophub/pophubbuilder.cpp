#include "pophubbuilder.h"
#include <wx/panel.h>
#include "settings.h"
#include "settingevent.h"
#include "version.h"
#include "pophubpanel.h"
#include "pnlMode.h"
#include "pnlMeters.h"
#include "pnlOptions.h"
#include "pnlPhase.h"
#include "pnlView.h"

using namespace std;

pophubBuilder::pophubBuilder() : MonitorPluginBuilder()
{

    RegisterForSettingsUpdates(this);
    
    Bind(wxEVT_SETTING_CHANGED, &pophubBuilder::OnSettingChanged, this);

}

void pophubBuilder::SetAudioData(const timedbuffer* pBuffer)
{
	m_pMeter->SetAudioData(pBuffer);
}

wxWindow* pophubBuilder::CreateMonitorPanel(wxWindow* pParent)
{
    m_pMeter = new pophubPanel(pParent,this);
	return m_pMeter;

}

list<pairOptionPanel_t> pophubBuilder::CreateOptionPanels(wxWindow* pParent)
 {
    return { {"View", new pnlView(pParent, this)}, {"Meters", new pnlMeters(pParent, this)}, {"Mode", new pnlMode(pParent, this)},{"Phase", new pnlPhase(pParent, this)}, {"Options", new pnlOptions(pParent, this)}};
}




void pophubBuilder::LoadSettings()
{
	//@todo load any settings
}


void pophubBuilder::InputSession(const session& aSession)
{
	m_pMeter->InputSession(aSession);
}

void pophubBuilder::OutputChannels(const std::vector<char>& vChannels)
{
	m_pMeter->OutputChannels(vChannels);
}


void pophubBuilder::OnSettingChanged(SettingEvent& event)
{
    //@todo a setting registered for has changed. Handle it
}




