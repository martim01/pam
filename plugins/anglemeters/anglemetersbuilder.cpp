#include "anglemetersbuilder.h"
#include <wx/panel.h>
#include "settings.h"
#include "settingevent.h"
#include "pnlAngleMeters.h"
#include "pnlMode.h"
#include "pnlOptions.h"
#include "pnlMeterSettings.h"
#include "ppmtypes.h"

using namespace std;

AngleMetersBuilder::AngleMetersBuilder() : MonitorPluginBuilder(),
m_pAngleMeters(0)
{

    RegisterForSettingsUpdates(this);

    Connect(wxID_ANY, wxEVT_SETTING_CHANGED, (wxObjectEventFunction)&AngleMetersBuilder::OnSettingChanged);

    RegisterRemoteApiEnum("Mode", PPMTypeManager::Get().GetTypes());
    RegisterRemoteApiEnum("Freeze", {{0,"Off"}, {1,"On"}});
    RegisterRemoteApiEnum("Peaks", {{0,"Off"}, {1,"Show"}, {2,"Hold"}});
    RegisterRemoteApiEnum("Speed", {{0,"Slow"}, {1,"Normal"}, {2,"Fast"}});
    RegisterRemoteApiEnum("M3M6", {{0,"M3"}, {1,"M6"}});
    RegisterRemoteApiEnum("Stereo", {{0,"Mono"},{1,"Stereo"}});
    RegisterRemoteApiEnum("DisplayText_Current", {{0,"Off"}, {1,"On"}});
    RegisterRemoteApiEnum("DisplayText_Peak", {{0,"Off"}, {1,"On"}});
    RegisterRemoteApiEnum("Surround", {{0,"Off"}, {1,"On"}});


}

void AngleMetersBuilder::SetAudioData(const timedbuffer* pBuffer)
{
    m_pAngleMeters->SetAudioData(pBuffer);

}

wxWindow* AngleMetersBuilder::CreateMonitorPanel(wxWindow* pParent)
{
    m_pAngleMeters = new pnlAngleMeters(pParent, this);
    return m_pAngleMeters;
}

list<pairOptionPanel_t> AngleMetersBuilder::CreateOptionPanels(wxWindow* pParent)
{
    list<pairOptionPanel_t> lstOptionPanels;

    pnlMode* pMode = new pnlMode(pParent, this);
    pnlOptions* pOptions = new pnlOptions(pParent, this);

    lstOptionPanels.push_back(make_pair(wxT("Mode"), pMode));
    lstOptionPanels.push_back(make_pair(wxT("Meters"), new pnlMeterSettings(pParent, this)));
    lstOptionPanels.push_back(make_pair(wxT("Options"), pOptions));

    return lstOptionPanels;
}




void AngleMetersBuilder::LoadSettings()
{

    m_pAngleMeters->Freeze((ReadSetting(wxT("Freeze"),0)==1));
    m_pAngleMeters->ShowPeaks(ReadSetting(wxT("Peaks"),0));
    m_pAngleMeters->SetSpeed(ReadSetting(wxT("Speed"),1));
    m_pAngleMeters->SetM3M6(ReadSetting(wxT("M3M6"),2));
    m_pAngleMeters->UpdateMeterStereo();
    m_pAngleMeters->DisplayCurrentLevelAsText(ReadSetting(wxT("DisplayText_Current"),1));
    m_pAngleMeters->DisplayPeakLevelAsText(ReadSetting(wxT("DisplayText_Peak"),1));
    m_pAngleMeters->SetMode(ReadSetting(wxT("Mode"),wxT("BBC")));
}


void AngleMetersBuilder::InputSession(const session& aSession)
{
    m_pAngleMeters->SetSession(aSession);
   // m_pAngleMeters->SetMode(ReadSetting(wxT("Mode"),wxT("BBC")));
}

void AngleMetersBuilder::OutputChannels(const std::vector<char>& vChannels)
{
    m_pAngleMeters->OutputChannels(vChannels);
}


void AngleMetersBuilder::OnSettingChanged(SettingEvent& event)
{
    if(event.GetKey() == wxT("Mode"))
    {
        m_pAngleMeters->SetMode(ReadSetting(wxT("Mode"),wxT("BBC")));
    }
    else if(event.GetKey() == wxT("Freeze"))
    {
        m_pAngleMeters->Freeze((ReadSetting(wxT("Freeze"),0)==1));
    }
    else if(event.GetKey() == wxT("Peaks"))
    {
        m_pAngleMeters->ShowPeaks(ReadSetting(wxT("Peaks"),0));
    }
    else if(event.GetKey() == wxT("Speed"))
    {
        m_pAngleMeters->SetSpeed(ReadSetting(wxT("Speed"),1));
    }
    else if(event.GetKey() == wxT("M3M6"))
    {
        m_pAngleMeters->SetM3M6(ReadSetting(wxT("M3M6"),2));
    }
    else if(event.GetKey() == wxT("Stereo"))
    {
        m_pAngleMeters->UpdateMeterStereo();
    }
    else if(event.GetKey() == wxT("DisplayText_Current"))
    {
        m_pAngleMeters->DisplayCurrentLevelAsText(ReadSetting(wxT("DisplayText_Current"),1));
    }
    else if(event.GetKey() == wxT("DisplayText_Peak"))
    {
        m_pAngleMeters->DisplayPeakLevelAsText(ReadSetting(wxT("DisplayText_Peak"),1));
    }
    else if(event.GetKey() == wxT("Surround"))
    {
        m_pAngleMeters->ShowSurround(event.GetValue(true));
    }
}


void AngleMetersBuilder::ClearMeter()
{
    m_pAngleMeters->ClearMeters();
}



