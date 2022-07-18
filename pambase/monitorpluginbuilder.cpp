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
    RemoteApi::Get().AddPluginEndpoint(pml::restgoose::PATCH, endpoint("/x-pam/plugins/monitor/"+GetName().ToStdString()), std::bind(&MonitorPluginBuilder::PatchSetting, this, _1,_2,_3,_4));
}

pml::restgoose::response MonitorPluginBuilder::GetStatus(const query& theQuery, const std::vector<pml::restgoose::partData>& vData, const endpoint& theEndpoint, const userName& theUser)
{
    auto itParam = theQuery.find(queryKey("key"));
    wxString sKey;
    if(itParam != theQuery.end())
    {
        sKey = itParam->second.Get();
    }

    pml::restgoose::response resp(200);
    resp.jsonData = RemoteApi::Get().GetSectionJson(GetSection(), sKey);
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

void MonitorPluginBuilder::RegisterRemoteApiEnum(const wxString& sKey, const std::set<wxString>& setEnum, const wxString& sDefault)
{
    RemoteApi::Get().RegisterRemoteApiEnum(GetSection(), sKey, setEnum, sDefault);
}

void MonitorPluginBuilder::RegisterRemoteApiEnum(const wxString& sKey, const std::map<int, wxString>& mEnum, int nDefault)
{
    RemoteApi::Get().RegisterRemoteApiEnum(GetSection(), sKey, mEnum, nDefault);
}

void MonitorPluginBuilder::RegisterRemoteApiRangeDouble(const wxString& sKey, const std::pair<double, double>& dRange, double dDefault)
{
    RemoteApi::Get().RegisterRemoteApiRangeDouble(GetSection(), sKey, dRange, dDefault);
}

void MonitorPluginBuilder::RegisterRemoteApiRangeInt(const wxString& sKey, const std::pair<int, int>& nRange, int nDefault)
{
    RemoteApi::Get().RegisterRemoteApiRangeInt(GetSection(), sKey, nRange, nDefault);
}
void MonitorPluginBuilder::RegisterRemoteApiCallback(const wxString& sKey, std::function<std::set<wxString>()> func, const wxString& sDefault)
{
    RemoteApi::Get().RegisterRemoteApiCallback(GetSection(), sKey, func, sDefault);
}

void MonitorPluginBuilder::RegisterRemoteApiCallback(const wxString& sKey, std::function<std::map<int, wxString>()> func, int nDefault)
{
    RemoteApi::Get().RegisterRemoteApiCallback(GetSection(), sKey, func, nDefault);
}

void MonitorPluginBuilder::RegisterRemoteApi(const wxString& sKey, const wxString& sDefault)
{
    RemoteApi::Get().RegisterRemoteApi(GetSection(), sKey, sDefault);
}

void MonitorPluginBuilder::RegisterRemoteApiCSV(const wxString& sKey, const std::set<wxString>& setEnum, const wxString& sDefault)
{
    RemoteApi::Get().RegisterRemoteApiCSV(GetSection(), sKey, setEnum, sDefault);
}

pml::restgoose::response MonitorPluginBuilder::PatchSetting(const query& theQuery, const std::vector<pml::restgoose::partData>& vData, const endpoint& theEndpoint, const userName& theUser)
{
    return RemoteApi::Get().DoPatchSettings(vData, GetSection());
}


