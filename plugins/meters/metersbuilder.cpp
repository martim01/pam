#include "metersbuilder.h"
#include <wx/panel.h>
#include "settings.h"
#include "settingevent.h"
#include "pnlMeters.h"
#include "pnlMode.h"
#include "pnlOptions.h"
#include "pnlMeterSettings.h"
#include "pnlScale.h"
#include "levelcalculator.h"
#include "ppmtypes.h"


using namespace std;

MetersBuilder::MetersBuilder() : MonitorPluginBuilder(),
m_pMeters(0)
{

    RegisterForSettingsUpdates(this);

    Connect(wxID_ANY, wxEVT_SETTING_CHANGED, (wxObjectEventFunction)&MetersBuilder::OnSettingChanged);


    //Register settings with RemoteAPI
    RegisterRemoteApiEnum("Mode", PPMTypeManager::Get().GetTypes(), "BBC");
    RegisterRemoteApiEnum("Freeze", {{0,"Off"}, {1,"On"}},0);
    RegisterRemoteApiEnum("Peaks", {{0,"Off"}, {1,"Show"}, {2,"Hold"}},1);
    RegisterRemoteApiEnum("Speed", {{0,"Slow"}, {1,"Normal"}, {2,"Fast"}},1);
    RegisterRemoteApiEnum("M3M6", {{0,"M3"}, {1,"M6"}},0);
    RegisterRemoteApiEnum("Shading", {{0,"Off"}, {1,"On"}},0);

}

void MetersBuilder::SetAudioData(const timedbuffer* pBuffer)
{
    m_pMeters->SetAudioData(pBuffer);

    if(WebsocketsActive())
    {
        SendWebsocketMessage(m_pMeters->CreateWebsocketMessage());
    }
}


wxWindow* MetersBuilder::CreateMonitorPanel(wxWindow* pParent)
{
    m_pMeters = new pnlMeters(pParent, this);
    return m_pMeters;
}

list<pairOptionPanel_t> MetersBuilder::CreateOptionPanels(wxWindow* pParent)
{
    list<pairOptionPanel_t> lstOptionPanels;

    pnlMode* pMode = new pnlMode(pParent, this);
    pnlOptions* pOptions = new pnlOptions(pParent, this);

    lstOptionPanels.push_back(make_pair(wxT("Mode"), pMode));
    lstOptionPanels.push_back(make_pair(wxT("Meters"), new pnlMeterSettings(pParent, this)));
    lstOptionPanels.push_back(make_pair(wxT("Options"), pOptions));

    return lstOptionPanels;
}




void MetersBuilder::LoadSettings()
{


}


void MetersBuilder::InputSession(const session& aSession)
{
    m_pMeters->SetSession(aSession);
    m_pMeters->SetMode(ReadSetting(wxT("Mode"),wxT("BBC")));
}

void MetersBuilder::OutputChannels(const std::vector<char>& vChannels)
{
    m_pMeters->OutputChannels(vChannels);
}


void MetersBuilder::OnSettingChanged(SettingEvent& event)
{
    if(event.GetKey() == wxT("Mode"))
    {
        m_pMeters->SetMode(ReadSetting(wxT("Mode"),wxT("BBC")));
    }
    else if(event.GetKey() == wxT("Freeze"))
    {
        m_pMeters->Freeze((ReadSetting(wxT("Freeze"),0)==1));
    }
    else if(event.GetKey() == wxT("Peaks"))
    {
        m_pMeters->ShowPeaks(ReadSetting(wxT("Peaks"),0));
    }
    else if(event.GetKey() == wxT("Speed"))
    {
        m_pMeters->SetSpeed(ReadSetting(wxT("Speed"),1));
    }
    else if(event.GetKey() == wxT("M3M6"))
    {
        m_pMeters->SetM3M6(ReadSetting(wxT("M3M6"),2));
    }
    else if(event.GetKey() == wxT("Shading"))
    {
        m_pMeters->SetShading(ReadSetting(wxT("Shading"),0));
    }

}


void MetersBuilder::ClearMeter()
{
    m_pMeters->ClearMeters();
}



