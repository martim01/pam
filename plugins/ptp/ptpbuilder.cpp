#include "ptpbuilder.h"
#include <wx/panel.h>
#include "settings.h"
#include "settingevent.h"
#include "ptppanel.h"
#include "pnlOptions.h"
#include <wx/log.h>


using namespace std;

ptpBuilder::ptpBuilder() : MonitorPluginBuilder()
{

    RegisterForSettingsUpdates("Window", this);

    Connect(wxID_ANY, wxEVT_SETTING_CHANGED, (wxObjectEventFunction)&ptpBuilder::OnSettingChanged);

}

void ptpBuilder::SetAudioData(const timedbuffer* pBuffer)
{
//	m_pMeter->SetAudioData(pBuffer);
}

wxWindow* ptpBuilder::CreateMonitorPanel(wxWindow* pParent)
{

	m_pMeter = new ptpPanel(pParent, this);
	return m_pMeter;

}

list<pairOptionPanel_t> ptpBuilder::CreateOptionPanels(wxWindow* pParent)
{
    list<pairOptionPanel_t> lstOptionPanels;
    lstOptionPanels.push_back(std::make_pair("Options", new pnlOptions(pParent, this)));
    //@todo create and return all the option panels
    return lstOptionPanels;
}




void ptpBuilder::LoadSettings()
{
	//@todo load any settings
}


void ptpBuilder::InputSession(const session& aSession)
{
	//m_pMeter->InputSession(aSession);
}

void ptpBuilder::OutputChannels(const std::vector<char>& vChannels)
{
	//m_pMeter->OutputChannels(vChannels);
}


void ptpBuilder::OnSettingChanged(SettingEvent& event)
{
    m_pMeter->ChangeView(event.GetValue());
   Maximize(true);
}


