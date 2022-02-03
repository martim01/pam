#include "lissajoubuilder.h"
#include <wx/panel.h>
#include "settings.h"
#include "settingevent.h"
#include "pnlLissajou.h"
#include "pnlDisplay.h"
#include "pnlMeters.h"
#include "pnlRouting.h"
#include <wx/log.h>

using namespace std;

LissajouBuilder::LissajouBuilder() : MonitorPluginBuilder(),
m_pLissajou(0),
m_ppnlRouting(0)
{

    RegisterForSettingsUpdates(wxT("Scale"), this);
    RegisterForSettingsUpdates(wxT("Rotate"), this);
    RegisterForSettingsUpdates(wxT("Levels"), this);
    RegisterForSettingsUpdates(wxT("Axis_X"), this);
    RegisterForSettingsUpdates(wxT("Axis_Y"), this);
    RegisterForSettingsUpdates(wxT("MeterMode"), this);
    RegisterForSettingsUpdates(wxT("Monitor"), this);
    RegisterForSettingsUpdates(wxT("Display"), this);

    Connect(wxID_ANY, wxEVT_SETTING_CHANGED, (wxObjectEventFunction)&LissajouBuilder::OnSettingChanged);

}

void LissajouBuilder::SetAudioData(const timedbuffer* pBuffer)
{
    m_pLissajou->SetAudioData(pBuffer);
}

wxWindow* LissajouBuilder::CreateMonitorPanel(wxWindow* pParent)
{
    wxASSERT(pParent);
    m_pLissajou = new pnlLissajou(pParent, this);
    return m_pLissajou;
}

list<pairOptionPanel_t> LissajouBuilder::CreateOptionPanels(wxWindow* pParent)
{
    list<pairOptionPanel_t> lstOptionPanels;

    m_ppnlRouting = new pnlRouting(pParent, this);
    lstOptionPanels.push_back(make_pair(wxT("Routing"), m_ppnlRouting));
    lstOptionPanels.push_back(make_pair(wxT("Display"), new pnlDisplay(pParent, this)));
    lstOptionPanels.push_back(make_pair(wxT("Meters"), new pnlMeters(pParent, this)));


    return lstOptionPanels;
}




void LissajouBuilder::LoadSettings()
{
    if(m_pLissajou)
    {
        m_pLissajou->SetScale(ReadSetting(wxT("Scale"),1));
        m_pLissajou->Rotate(ReadSetting(wxT("Rotate"),1));
        m_pLissajou->ShowLevels(ReadSetting(wxT("Levels"),1));
        m_pLissajou->SetAxis(ReadSetting(wxT("Axis_X"),0), ReadSetting(wxT("Axis_Y"),1));
        m_pLissajou->SetDisplayType(ReadSetting(wxT("Display"), 0));
        if(ReadSetting(wxT("Monitor"),0) == 1)  //force the output
        {
            vector<char> vChannels;
            vChannels.push_back(ReadSetting(wxT("Axis_X"),0));
            vChannels.push_back(ReadSetting(wxT("Axis_Y"),1));
            AskToMonitor(vChannels);
        }
        m_pLissajou->SetMeterMode(ReadSetting(wxT("MeterMode"),wxT("BBC")));
        if(ReadSetting(wxT("Monitor"),0) == 0)  //follow
        {
            m_pLissajou->Follow();
        }
    }

}


void LissajouBuilder::InputSession(const session& aSession)
{
    //check if we need to change our routing....
    if(m_ppnlRouting)
    {
        if(aSession.GetCurrentSubsession() != aSession.lstSubsession.end())
        {
            m_ppnlRouting->SetNumberOfChannels(min((unsigned int)256 ,aSession.GetCurrentSubsession()->nChannels));
        }
        else
        {
            m_ppnlRouting->SetNumberOfChannels(0);
        }
    }
    if(m_pLissajou)
    {
        m_pLissajou->SetSession(aSession);
    }
}

void LissajouBuilder::OutputChannels(const std::vector<char>& vChannels)
{
    if(ReadSetting(wxT("Monitor"),0) == 0)  //follow the output
    {
        if(vChannels.size() > 1)
        {
            WriteSetting(wxT("Axis_Y"), vChannels[1]);
        }
        if(vChannels.size() > 0)
        {
            WriteSetting(wxT("Axis_X"), vChannels[0]);
        }
        if(vChannels.size() >1 && m_ppnlRouting)
        {
            m_ppnlRouting->SetAxis(vChannels[0], vChannels[1]);
        }
    }
    m_pLissajou->OutputChannels(vChannels);
}


void LissajouBuilder::OnSettingChanged(SettingEvent& event)
{
    if(event.GetKey() == wxT("Scale"))
    {
        m_pLissajou->SetScale(ReadSetting(wxT("Scale"),1));
    }
    else if(event.GetKey() == wxT("Rotate"))
    {
        m_pLissajou->Rotate(ReadSetting(wxT("Rotate"),1));
    }
    else if(event.GetKey() == wxT("Levels"))
    {
        m_pLissajou->ShowLevels(ReadSetting(wxT("Levels"),1));
    }
    else if(event.GetKey() == wxT("Axis_X") ||event.GetKey() == wxT("Axis_Y"))
    {
        m_pLissajou->SetAxis(ReadSetting(wxT("Axis_X"),0), ReadSetting(wxT("Axis_Y"),1));

        if(ReadSetting(wxT("Monitor"),0) == 1)  //force the output
        {
            vector<char> vChannels;
            vChannels.push_back(ReadSetting(wxT("Axis_X"),0));
            vChannels.push_back(ReadSetting(wxT("Axis_Y"),1));
            AskToMonitor(vChannels);
        }
    }
    else if(event.GetKey() == wxT("MeterMode"))
    {
        m_pLissajou->SetMeterMode(ReadSetting(wxT("MeterMode"),wxT("BBC")));
    }
    else if(event.GetKey() == wxT("Monitor"))
    {
        if(ReadSetting(wxT("Monitor"),0) == 0)  //follow
        {
            m_pLissajou->Follow();
        }
    }
    else if(event.GetKey() == wxT("Display"))
    {
        m_pLissajou->SetDisplayType(ReadSetting(wxT("Display"), 0));
    }
}

