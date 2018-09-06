#include "r128builder.h"
#include <wx/panel.h>
#include "settings.h"
#include "settingevent.h"
#include "pnlEbuMeter.h"
#include <wx/log.h>
#include "version.h"
#include "pnlControl.h"
#include "pnlScale.h"
#include "pnlDisplay.h"

using namespace std;

R128Builder::R128Builder() : MonitorPluginBuilder(),
m_pMeters(0)
{

    RegisterForSettingsUpdates(wxT("Calculate"), this);
    RegisterForSettingsUpdates(wxT("Scale"), this);
    RegisterForSettingsUpdates(wxT("Zero"), this);
    RegisterForSettingsUpdates(wxT("Show_Short"), this);
    RegisterForSettingsUpdates(wxT("Show_Momentary"), this);
    RegisterForSettingsUpdates(wxT("Show_Live"), this);
    RegisterForSettingsUpdates(wxT("Show_True"), this);
    RegisterForSettingsUpdates(wxT("Show_Phase"), this);
    Connect(wxID_ANY, wxEVT_SETTING_CHANGED, (wxObjectEventFunction)&R128Builder::OnSettingChanged);
    m_nInputChannels = 1;
    m_nDisplayChannel = 0;
    m_bRun = true;
}

R128Builder::~R128Builder()
{
}

void R128Builder::SetAudioData(const timedbuffer* pBuffer)
{
    if(m_pMeters && m_bRun)
    {
        m_pMeters->SetAudioData(pBuffer);
    }
}

wxWindow* R128Builder::CreateMonitorPanel(wxWindow* pParent)
{
    m_pMeters = new pnlEbuMeter(pParent,this);
    return m_pMeters;
}

list<pairOptionPanel_t> R128Builder::CreateOptionPanels(wxWindow* pParent)
{
    list<pairOptionPanel_t> lstOptionPanels;


    lstOptionPanels.push_back(make_pair(wxT("Display"), new pnlDisplay(pParent, this)));
    lstOptionPanels.push_back(make_pair(wxT("Scale"), new pnlScale(pParent, this)));

   // lstOptionPanels.push_back(make_pair(wxT("Time"), new pnlDisplay(pParent, this)));
   // lstOptionPanels.push_back(make_pair(wxT("Meter"), new pnlMeters(pParent, this)));
//    lstOptionPanels.push_back(make_pair(wxT("Options"), pOptions));
//
    return lstOptionPanels;
}




void R128Builder::LoadSettings()
{

}


void R128Builder::InputSession(const session& aSession)
{
    m_pMeters->SetSession(aSession);

}

void R128Builder::OutputChannels(const std::vector<char>& vChannels)
{
}


void R128Builder::OnSettingChanged(SettingEvent& event)
{
    if(event.GetKey() == wxT("Calculate"))
    {
        m_bRun = (ReadSetting(wxT("Calculate"),1) == 1);
    }
    else if(event.GetKey() == wxT("Scale"))
    {
        m_pMeters->ChangeScale();
    }
    else if(event.GetKey() == wxT("Zero"))
    {
        m_pMeters->ChangeScale();
    }
    else
    {
        m_pMeters->LoadSettings();
    }
}


void R128Builder::ClearMeter()
{
    m_pMeters->ClearMeters();
}



