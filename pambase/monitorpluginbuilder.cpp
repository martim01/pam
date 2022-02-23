#include "monitorpluginbuilder.h"
#include "wmswitcherpanel.h"
#include "settings.h"
#include <wx/log.h>
#include "monitorevent.h"
#include "remoteapi.h"

using namespace std;
using namespace std::placeholders;

DEFINE_EVENT_TYPE(wxEVT_MONITOR_MAX)

MonitorPluginBuilder::MonitorPluginBuilder() :
    m_pHandler(nullptr),
    m_bWantsAudioAlways(false),
    m_bWantsQoSAlways(false),
	m_pswpMonitor(nullptr),
	m_pswpOptions(nullptr),
	m_pWindow(nullptr)
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

    m_pWindow = CreateMonitorPanel(pswpMonitor);
    pswpMonitor->AddPage(m_pWindow, GetName(), false);
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
    Settings::Get().Write(GetSection(), sSetting, sValue);
}

void MonitorPluginBuilder::WriteSetting(const wxString& sSetting, int nValue)
{
    Settings::Get().Write(GetSection(), sSetting, nValue);
}

void MonitorPluginBuilder::WriteSetting(const wxString& sSetting, double dValue)
{
    Settings::Get().Write(GetSection(), sSetting, dValue);
}

wxString MonitorPluginBuilder::ReadSetting(const wxString& sSetting, const wxString& sDefault)
{
    return Settings::Get().Read(GetSection(), sSetting, sDefault);
}

int MonitorPluginBuilder::ReadSetting(const wxString& sSetting, int nDefault)
{
    return Settings::Get().Read(GetSection(), sSetting, nDefault);
}

double MonitorPluginBuilder::ReadSetting(const wxString& sSetting, double dDefault)
{
    return Settings::Get().Read(GetSection(), sSetting, dDefault);
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


void MonitorPluginBuilder::RegisterForSettingsUpdates(wxEvtHandler* pHandler, const wxString& sSetting)
{
    Settings::Get().AddHandler(pHandler, GetSection(), sSetting);
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

void MonitorPluginBuilder::InitRemoteApi()
{
    RemoteApi::Get().AddPluginEndpoint(pml::restgoose::GET, endpoint("/x-pam/plugins/monitor/"+GetName().ToStdString()), std::bind(&MonitorPluginBuilder::GetStatus, this, _1,_2,_3,_4));
}

pml::restgoose::response MonitorPluginBuilder::GetStatus(const query& theQuery, const std::vector<pml::restgoose::partData>& vData, const endpoint& theEndpoint, const userName& theUser)
{
    pml::restgoose::response resp;
    auto pSection = Settings::Get().GetSection(GetSection());
    if(pSection)
    {
        for(auto pairData : pSection->GetData())
        {
            resp.jsonData[pairData.first.ToStdString()] = pairData.second.ToStdString();
        }
    }
    return resp;

}

void MonitorPluginBuilder::SendWebsocketMessage(const Json::Value& jsMessage)
{
    if(RemoteApi::Get().WebsocketsActive())
    {
        Json::Value jsWebsocket;
        jsWebsocket["plugin"]["type"] = "monitor";
        jsWebsocket["plugin"]["name"] = GetName().ToStdString();
        jsWebsocket["data"] = jsMessage;
        RemoteApi::Get().SendPluginWebsocketMessage(jsWebsocket);
    }
}

bool MonitorPluginBuilder::WebsocketsActive()
{
    return RemoteApi::Get().WebsocketsActive();
}

void MonitorPluginBuilder::RegisterRemoteApiEnum(const wxString& sKey, const std::set<wxString>& setEnum)
{
    RemoteApi::Get().RegisterRemoteApiEnum(GetSection(), sKey, setEnum);
}

void MonitorPluginBuilder::RegisterRemoteApiEnum(const wxString& sKey, const std::map<int, wxString>& mEnum)
{
    RemoteApi::Get().RegisterRemoteApiEnum(GetSection(), sKey, mEnum);
}

void MonitorPluginBuilder::RegisterRemoteApiRangeDouble(const wxString& sKey, const std::pair<double, double>& dRange)
{
    RemoteApi::Get().RegisterRemoteApiRangeDouble(GetSection(), sKey, dRange);
}

void MonitorPluginBuilder::RegisterRemoteApiRangeInt(const wxString& sKey, const std::pair<int, int>& nRange)
{
    RemoteApi::Get().RegisterRemoteApiRangeInt(GetSection(), sKey, nRange);
}
void MonitorPluginBuilder::RegisterRemoteApiCallback(const wxString& sKey, std::function<std::set<wxString>()> func)
{
    RemoteApi::Get().RegisterRemoteApiCallback(GetSection(), sKey, func);
}

void MonitorPluginBuilder::RegisterRemoteApiCallback(const wxString& sKey, std::function<std::map<int, wxString>()> func)
{
    RemoteApi::Get().RegisterRemoteApiCallback(GetSection(), sKey, func);
}

void MonitorPluginBuilder::RegisterRemoteApi(const wxString& sKey)
{
    RemoteApi::Get().RegisterRemoteApi(GetSection(), sKey);
}

void MonitorPluginBuilder::RegisterRemoteApiCSV(const wxString& sKey, const std::set<wxString>& setEnum)
{
    RemoteApi::Get().RegisterRemoteApiCSV(GetSection(), sKey, setEnum);
}
