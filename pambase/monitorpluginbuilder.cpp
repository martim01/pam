#include "monitorpluginbuilder.h"
#include "wmswitcherpanel.h"
#include "settings.h"
#include <wx/log.h>
#include "monitorevent.h"

using namespace std;

DEFINE_EVENT_TYPE(wxEVT_MONITOR_MAX)

MonitorPluginBuilder::MonitorPluginBuilder() :
    m_pHandler(0)
{

}

void MonitorPluginBuilder::SetHandler(wxEvtHandler* pHandler)
{
    m_pHandler = pHandler;
}

void MonitorPluginBuilder::CreatePanels(wmSwitcherPanel* pswpMonitor, wmSwitcherPanel* pswpOptions)
{
    pswpMonitor->AddPage(CreateMonitorPanel(pswpMonitor), GetName(), false);
    list<pairOptionPanel_t> lstOptions(CreateOptionPanels(pswpOptions));

    for(list<pairOptionPanel_t>::iterator itOption = lstOptions.begin(); itOption != lstOptions.end(); ++itOption)
    {
        pswpOptions->AddPage((*itOption).second,wxString::Format(wxT("%s|%s"), GetName().c_str(), (*itOption).first.c_str()), false);
    }

    LoadSettings();
}


void MonitorPluginBuilder::WriteSetting(const wxString& sSetting, const wxString& sValue)
{
    Settings::Get().Write(GetName(), sSetting, sValue);
}

void MonitorPluginBuilder::WriteSetting(const wxString& sSetting, int nValue)
{
    Settings::Get().Write(GetName(), sSetting, nValue);
}

void MonitorPluginBuilder::WriteSetting(const wxString& sSetting, double dValue)
{
    Settings::Get().Write(GetName(), sSetting, dValue);
}

wxString MonitorPluginBuilder::ReadSetting(const wxString& sSetting, const wxString& sDefault)
{
    return Settings::Get().Read(GetName(), sSetting, sDefault);
}

int MonitorPluginBuilder::ReadSetting(const wxString& sSetting, int nDefault)
{
    return Settings::Get().Read(GetName(), sSetting, nDefault);
}

double MonitorPluginBuilder::ReadSetting(const wxString& sSetting, double dDefault)
{
    return Settings::Get().Read(GetName(), sSetting, dDefault);
}


void MonitorPluginBuilder::Maximize(bool bMax)
{
    if(m_pHandler)
    {
        wxCommandEvent event(wxEVT_MONITOR_MAX);
        event.SetInt(bMax);
        wxPostEvent(m_pHandler, event);
    }
}


void MonitorPluginBuilder::RegisterForSettingsUpdates(const wxString& sSetting, wxEvtHandler* pHandler)
{
    Settings::Get().AddHandler(GetName(), sSetting, pHandler);
}


void MonitorPluginBuilder::AskToMonitor(const vector<char>& vChannels)
{
    wxLogDebug(wxT("AskToMonitor"));
    if(m_pHandler)
    {
        MonitorEvent event(vChannels);
        wxPostEvent(m_pHandler, event);
    }
}


bool MonitorPluginBuilder::CanBeMaximized() const
{
    return true;
}
