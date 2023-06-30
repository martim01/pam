#include "r128builder.h"
#include <wx/panel.h>
#include "settings.h"
#include "settingevent.h"
#include "pnlEbuMeter.h"
#include <wx/log.h>
#include "pnlControl.h"
#include "pnlScale.h"
#include "pnlDisplay.h"
#include "pnlgroups.h"
#include "ppmtypes.h"
#include "r128types.h"
#include "pnlMeters.h"
using namespace std;

R128Builder::R128Builder() : MonitorPluginBuilder(),
m_pMeters(0)
{

    //m_bWantsAudioAlways = true;
    RegisterForSettingsUpdates(this);
    Connect(wxID_ANY, wxEVT_SETTING_CHANGED, (wxObjectEventFunction)&R128Builder::OnSettingChanged);


    RegisterRemoteApiEnum("Calculate", {{0, "Pause"},{1,"Run"}}, 1);
    RegisterRemoteApiEnum("Scale", {{0, "+9"},{1,"+18"}}, 0);
    RegisterRemoteApiEnum("Zero", {{0,"-23"},{1,"Abs"}}, 0);
    RegisterRemoteApiEnum("Show_Short", {{0,"Hide"},{1,"Show"}},1);
    RegisterRemoteApiEnum("Show_Momentary", {{0,"Hide"},{1,"Show"}},1);
    RegisterRemoteApiEnum("Show_Live", {{0,"Hide"},{1,"Show"}},1);
    RegisterRemoteApiEnum("Show_True", {{0,"Hide"},{1,"Show"}},1);
    RegisterRemoteApiEnum("Show_Phase", {{0,"Hide"},{1,"Show"}},1);
    RegisterRemoteApiEnum("MeterMode", PPMTypeManager::Get().GetTypes(), "BBC");
    RegisterRemoteApiEnum("R128Mode", R128TypeManager::Get().GetTypes(), "R128");

    //RegisterRemoteApiCallback("Group", std::bind(&R128Builder::GetGroups, this));

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
        if(WebsocketsActive())
        {
            SendWebsocketMessage(m_pMeters->CreateWebsocketMessage());
        }
    }
    else if(WebsocketsActive())
    {
        Json::Value jsMessage;
        jsMessage["state"] = "paused";
        SendWebsocketMessage(jsMessage);
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

    m_ppnlGroups = new pnlGroups(pParent, this);
    m_ppnlMeters = new pnlMeters(pParent, this);

    lstOptionPanels.push_back(make_pair("Channels", m_ppnlGroups));
    lstOptionPanels.push_back(make_pair("Display", new pnlDisplay(pParent, this)));
    lstOptionPanels.push_back(make_pair("Meters", m_ppnlMeters));
    lstOptionPanels.push_back(make_pair("Scale", new pnlScale(pParent, this)));

    return lstOptionPanels;
}




void R128Builder::LoadSettings()
{
    m_pMeters->SetGroup(ReadSetting("Group", 0));
}


void R128Builder::InputSession(const session& aSession)
{
    m_pMeters->SetSession(aSession);
    if(aSession.GetCurrentSubsession() != aSession.lstSubsession.end())
    {
        m_ppnlGroups->SetChannels(aSession.GetCurrentSubsession()->vChannels);
        m_pMeters->SetGroup(ReadSetting("Group", 0));
    }
    else
    {
        m_ppnlGroups->SetChannels({});
    }
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
    else if(event.GetKey() == wxT("Zero"))
    {
        m_pMeters->ChangeR128(ReadSetting("R128Mode", "R128 +18"));
    }
    else if(event.GetKey() == "Group")
    {
        m_pMeters->SetGroup(event.GetValue(0l));
    }
    else if(event.GetKey() == "MeterMode")
    {
        m_pMeters->ChangeMeters(event.GetValue());
    }
    else if(event.GetKey() == "R128Mode")
    {
        m_pMeters->ChangeR128(event.GetValue());
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



