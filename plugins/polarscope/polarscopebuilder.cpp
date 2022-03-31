#include "polarscopebuilder.h"
#include <wx/panel.h>
#include "settings.h"
#include "settingevent.h"
#include "polarscopemeter.h"
#include "pnlOptionsMode.h"
#include <wx/log.h>
#include "pnlRouting.h"

using namespace std;

PolarScopeBuilder::PolarScopeBuilder() : MonitorPluginBuilder(),
m_pPolarScope(0)
{

    RegisterForSettingsUpdates(this);

    Connect(wxID_ANY, wxEVT_SETTING_CHANGED, (wxObjectEventFunction)&PolarScopeBuilder::OnSettingChanged);

    RegisterRemoteApiEnum("Mode", {{0,"Points"}, {1,"Hull"}, {2,"Levels"}},0);
    RegisterRemoteApiRangeInt("Axis_X", {0,7},0);
    RegisterRemoteApiRangeInt("Axis_Y", {0,7},1);

    m_nInputChannels = 1;
    m_nDisplayChannel = 0;
}

PolarScopeBuilder::~PolarScopeBuilder()
{
}

void PolarScopeBuilder::SetAudioData(const timedbuffer* pBuffer)
{
    m_pPolarScope->SetAudioData(pBuffer);
    if(WebsocketsActive())
    {
        SendWebsocketMessage(m_pPolarScope->CreateWebsocketMessage());
    }
}

wxWindow* PolarScopeBuilder::CreateMonitorPanel(wxWindow* pParent)
{
    m_pPolarScope = new PolarScope(pParent);
    return m_pPolarScope;
}

list<pairOptionPanel_t> PolarScopeBuilder::CreateOptionPanels(wxWindow* pParent)
{
    list<pairOptionPanel_t> lstOptionPanels;


    m_ppnlRouting = new pnlRouting(pParent, this);
    lstOptionPanels.push_back(make_pair("Routing", m_ppnlRouting));
    lstOptionPanels.push_back(make_pair("Mode", new pnlOptionsMode(pParent, this)));
//
    return lstOptionPanels;
}




void PolarScopeBuilder::LoadSettings()
{


}


void PolarScopeBuilder::InputSession(const session& aSession)
{
    if(aSession.GetCurrentSubsession() != aSession.lstSubsession.end())
    {
        m_nInputChannels = aSession.GetCurrentSubsession()->nChannels;
        m_ppnlRouting->SetChannels(aSession.GetCurrentSubsession()->vChannels);
        m_pPolarScope->SetNumberOfInputChannels(m_nInputChannels);
    }
    else
    {
        m_pPolarScope->SetNumberOfInputChannels(0);
        m_nInputChannels = 0;
        m_ppnlRouting->SetChannels({});
    }
    ClearMeter();
}

void PolarScopeBuilder::OutputChannels(const std::vector<char>& vChannels)
{
}


void PolarScopeBuilder::OnSettingChanged(SettingEvent& event)
{
    if(event.GetKey() == wxT("Mode"))
    {
        m_pPolarScope->SetMode(event.GetValue(0l));
    }
    else if(event.GetKey() == "Axis_X")
    {
        m_pPolarScope->SetAxisX(event.GetValue(0l));
    }
    else if(event.GetKey() == "Axis_Y")
    {
        m_pPolarScope->SetAxisY(event.GetValue(1l));
    }
}


void PolarScopeBuilder::ClearMeter()
{
    m_pPolarScope->ClearMeter();
}


