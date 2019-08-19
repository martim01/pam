#include "monitorpluginbuilder.h"
#include "wmswitcherpanel.h"
#include "settings.h"
#include <wx/log.h>
#include "monitorevent.h"

using namespace std;

DEFINE_EVENT_TYPE(wxEVT_MONITOR_MAX)

MonitorPluginBuilder::MonitorPluginBuilder() :
    m_pHandler(0),
    m_bWantsAudioAlways(false),
	m_pswpMonitor(0),
	m_pswpOptions(0)
{

}

void MonitorPluginBuilder::SetHandler(wxEvtHandler* pHandler)
{
    m_pHandler = pHandler;
}

void MonitorPluginBuilder::CreatePanels(wmSwitcherPanel* pswpMonitor, wmSwitcherPanel* pswpOptions)
{
    m_pswpMonitor = pswpMonitor;
    m_pswpOptions = pswpOptions;

    pswpMonitor->AddPage(CreateMonitorPanel(pswpMonitor), GetName(), false);
    m_lstOptions = CreateOptionPanels(pswpOptions);

    for(list<pairOptionPanel_t>::iterator itOption = m_lstOptions.begin(); itOption != m_lstOptions.end(); ++itOption)
    {
        pswpOptions->AddPage((*itOption).second,wxString::Format(wxT("%s|%s"), GetName().c_str(), (*itOption).first.c_str()), false);
    }

    LoadSettings();
}

void MonitorPluginBuilder::DeletePanels()
{
    for(set<wxEvtHandler*>::iterator itHandler = m_setHandlers.begin(); itHandler != m_setHandlers.end(); ++itHandler)
    {
        Settings::Get().RemoveHandler((*itHandler));
    }

    m_setHandlers.clear();

    m_pswpMonitor->DeletePage(GetName());
    for(list<pairOptionPanel_t>::iterator itOption = m_lstOptions.begin(); itOption != m_lstOptions.end(); ++itOption)
    {
        m_pswpOptions->DeletePage(wxString::Format(wxT("%s|%s"), GetName().c_str(), (*itOption).first.c_str()));
    }

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
    cout << "Maximize" << endl;
    if(m_pHandler)
    {
        cout << "Maximize: Handler" << endl;
        wxCommandEvent event(wxEVT_MONITOR_MAX);
        event.SetInt(bMax);
        wxPostEvent(m_pHandler, event);
    }
}


void MonitorPluginBuilder::RegisterForSettingsUpdates(const wxString& sSetting, wxEvtHandler* pHandler)
{
    Settings::Get().AddHandler(GetName(), sSetting, pHandler);
    m_setHandlers.insert(pHandler);
}


void MonitorPluginBuilder::AskToMonitor(const vector<char>& vChannels)
{
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


MonitorPluginBuilder::~MonitorPluginBuilder()
{
}
