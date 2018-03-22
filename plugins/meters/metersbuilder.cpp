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
#include "version.h"

using namespace std;

MetersBuilder::MetersBuilder() : MonitorPluginBuilder(),
m_pMeters(0)
{

    RegisterForSettingsUpdates(wxT("Mode"), this);
    RegisterForSettingsUpdates(wxT("Freeze"), this);
    RegisterForSettingsUpdates(wxT("Peaks"), this);
    RegisterForSettingsUpdates(wxT("Speed"), this);
    RegisterForSettingsUpdates(wxT("M3M6"), this);
    RegisterForSettingsUpdates(wxT("Shading"), this);
    RegisterForSettingsUpdates(wxT("Scale"), this);

    Connect(wxID_ANY, wxEVT_SETTING_CHANGED, (wxObjectEventFunction)&MetersBuilder::OnSettingChanged);

}

void MetersBuilder::SetAudioData(const timedbuffer* pBuffer)
{
    m_pMeters->SetAudioData(pBuffer);
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
    lstOptionPanels.push_back(make_pair(wxT("Scale"), new pnlScale(pParent, this)));

    return lstOptionPanels;
}




void MetersBuilder::LoadSettings()
{


}


void MetersBuilder::InputSession(const session& aSession)
{
    m_pMeters->SetSession(aSession);
    m_pMeters->SetMode(ReadSetting(wxT("Mode"),0));
}

void MetersBuilder::OutputChannels(const std::vector<char>& vChannels)
{
    m_pMeters->OutputChannels(vChannels);
}


void MetersBuilder::OnSettingChanged(SettingEvent& event)
{
    if(event.GetKey() == wxT("Mode"))
    {
        m_pMeters->SetMode(ReadSetting(wxT("Mode"),0));
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
    else if(event.GetKey() == wxT("Scale"))
    {
        if(ReadSetting(wxT("Mode"),0) == LevelCalculator::ENERGY || ReadSetting(wxT("Mode"),0) == LevelCalculator::PEAK)
        {
            m_pMeters->SetScale(ReadSetting(wxT("Scale"), wxT("dBFS")));
        }
    }

}


void MetersBuilder::ClearMeter()
{
    m_pMeters->ClearMeters();
}



