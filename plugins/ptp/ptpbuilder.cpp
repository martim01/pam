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

    RegisterForSettingsUpdates(this);
    Settings::Get().AddHandler(this, "Time", "PTP_Domain");

    Connect(wxID_ANY, wxEVT_SETTING_CHANGED, (wxObjectEventFunction)&ptpBuilder::OnSettingChanged);

    RegisterRemoteApiEnum("Window", {"Info", "Graph", "Histogram"}, "Info");
    RegisterRemoteApiEnum("Data", {"Offset", wxString("Delay")}, "Offset");
    RegisterRemoteApiEnum("reset", {{0,""},{1,"Clear All"}},0);
}

void ptpBuilder::SetAudioData(const timedbuffer* pBuffer)
{

}

wxWindow* ptpBuilder::CreateMonitorPanel(wxWindow* pParent)
{

	m_pMeter = new ptpPanel(pParent, this, Settings::Get().Read("Time", "PTP_Domain", 0));
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
    if(event.GetKey() == "Window")
    {
        m_pMeter->ChangeView(event.GetValue());
        Maximize(true);
    }
    else if(event.GetKey() == "Data")
    {
        m_pMeter->SetData(event.GetValue());
    }
    else if(event.GetKey() == "reset" && event.GetValue(0l) == 1)
    {
        m_pMeter->ResetStats();
    }
    else if(event.GetKey() == "PTP_Domain")
    {
        m_pMeter->SetDomain(event.GetValue(0l));
    }
}


