#include "scopebuilder.h"
#include "scopemeter.h"
#include <wx/panel.h>
#include "settings.h"
#include "settingevent.h"
#include "pnlOptions.h"
#include "pnlRouting.h"
#include "pnlTimeframe.h"
#include "pnlTrigger.h"
#include "pnlVertical.h"

using namespace std;

ScopeBuilder::ScopeBuilder() : MonitorPluginBuilder(),
m_pScope(0)
{

    RegisterForSettingsUpdates(wxT("Trigger"), this);
    RegisterForSettingsUpdates(wxT("Autotrigger"), this);
    RegisterForSettingsUpdates(wxT("Mode"), this);
    RegisterForSettingsUpdates(wxT("Routing1"), this);
    RegisterForSettingsUpdates(wxT("Routing2"), this);
    RegisterForSettingsUpdates(wxT("Vertical"), this);
    RegisterForSettingsUpdates(wxT("Timeframe"), this);

    Connect(wxID_ANY, wxEVT_SETTING_CHANGED, (wxObjectEventFunction)&ScopeBuilder::OnSettingChanged);

}

void ScopeBuilder::SetAudioData(const timedbuffer* pBuffer)
{
    if(m_pScope)
    {
        m_pScope->SetData(pBuffer);
    }
}

wxWindow* ScopeBuilder::CreateMonitorPanel(wxWindow* pParent)
{
    m_pScope = new Scope(pParent, this);
    return m_pScope;
}

list<pairOptionPanel_t> ScopeBuilder::CreateOptionPanels(wxWindow* pParent)
{
    list<pairOptionPanel_t> lstOptionPanels;

    m_pRouting = new pnlRouting(pParent, this);
    pnlTimeframe* pTimeframe(new pnlTimeframe(pParent, this));
    pnlVertical* pVertical(new pnlVertical(pParent, this));
    m_pTrigger = new pnlTrigger(pParent, this);
    pnlOptions* pOptions(new pnlOptions(pParent, this));



    lstOptionPanels.push_back(make_pair(wxT("Routing"), m_pRouting));
    lstOptionPanels.push_back(make_pair(wxT("Timeframe"), pTimeframe));
    lstOptionPanels.push_back(make_pair(wxT("Vertical"),pVertical));
    lstOptionPanels.push_back(make_pair(wxT("Trigger"), m_pTrigger));
    lstOptionPanels.push_back(make_pair(wxT("Options"), pOptions));

    return lstOptionPanels;
}




void ScopeBuilder::LoadSettings()
{
    if(m_pScope)
    {
        m_pScope->SetTrigger(ReadSetting(wxT("Trigger"), 1.0));
        m_pScope->SetAutotrigger((ReadSetting(wxT("Autotrigger"), 0) == 1));
        m_pTrigger->EnableButtons((ReadSetting(wxT("Autotrigger"), 0) == 0));
        m_pScope->SetMode(ReadSetting(wxT("Mode"),0));
        m_pScope->SetRouting(ReadSetting(wxT("Routing1"), 0), 0);
        m_pScope->SetRouting(ReadSetting(wxT("Routing2"), 0), 1);
        m_pScope->SetTimeFrame(ReadSetting(wxT("Timeframe"), 1.0));
        m_pScope->SetVerticalZoom(ReadSetting(wxT("Vertical"),1.0));
    }

}


void ScopeBuilder::InputSession(const session& aSession)
{
    if(aSession.GetCurrentSubsession() != aSession.lstSubsession.end())
    {
        m_pScope->SetNumberOfChannels(min((unsigned int)256 ,aSession.GetCurrentSubsession()->nChannels));
        m_pRouting->SetNumberOfChannels(min((unsigned int)256 ,aSession.GetCurrentSubsession()->nChannels));
    }
    else

    {
        m_pScope->SetNumberOfChannels(0);
        m_pRouting->SetNumberOfChannels(0);
    }

}

void ScopeBuilder::OutputChannels(const std::vector<char>& vChannels)
{

}


void ScopeBuilder::OnSettingChanged(SettingEvent& event)
{
    if(event.GetKey() == wxT("Trigger"))
    {
        m_pScope->SetTrigger(ReadSetting(wxT("Trigger"), 1.0));
    }
    else if(event.GetKey() == wxT("Autotrigger"))
    {
        m_pScope->SetAutotrigger((ReadSetting(wxT("Autotrigger"), 0) == 1));
        m_pTrigger->EnableButtons((ReadSetting(wxT("Autotrigger"), 0) == 0));

    }
    else if(event.GetKey() == wxT("Mode"))
    {
        m_pScope->SetMode(ReadSetting(wxT("Mode"),0));

    }
    else if(event.GetKey() == wxT("Routing1"))
    {
        m_pScope->SetRouting(ReadSetting(wxT("Routing1"), 0), 0);
    }
    else if(event.GetKey() == wxT("Routing2"))
    {
        m_pScope->SetRouting(ReadSetting(wxT("Routing2"), 0), 1);
    }
    else if(event.GetKey() == wxT("Timeframe"))
    {
        m_pScope->SetTimeFrame(ReadSetting(wxT("Timeframe"), 1.0));
    }
    else if(event.GetKey() == wxT("Vertical"))
    {
        m_pScope->SetVerticalZoom(ReadSetting(wxT("Vertical"),1.0));
    }
}

void ScopeBuilder::ResetSlide()
{
    m_pScope->ResetSlide();
}

void ScopeBuilder::ClearMemory()
{
    m_pScope->ResetMemory();
}

void ScopeBuilder::SetMemory(bool bSet)
{
    m_pScope->SetMemory(bSet);
}


