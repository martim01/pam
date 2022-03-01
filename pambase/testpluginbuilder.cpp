#include "testpluginbuilder.h"
#include "wmswitcherpanel.h"
#include "settings.h"
#include <wx/log.h>
#include "remoteapi.h"

using namespace std::placeholders;

using namespace std;


TestPluginBuilder::TestPluginBuilder() :
    m_pHandler(0),
	m_pswpTests(0)
{

}

void TestPluginBuilder::SetHandler(wxEvtHandler* pHandler)
{
    m_pHandler = pHandler;
}

void TestPluginBuilder::CreatePanels(wmSwitcherPanel* pswpTests)
{
    pswpTests->AddPage(CreateTestPanel(pswpTests), GetName(), false);

    LoadSettings();
}


void TestPluginBuilder::WriteSetting(const wxString& sSetting, const wxString& sValue)
{
    Settings::Get().Write(GetSection(), sSetting, sValue);
}

void TestPluginBuilder::WriteSetting(const wxString& sSetting, int nValue)
{
    Settings::Get().Write(GetSection(), sSetting, nValue);
}

void TestPluginBuilder::WriteSetting(const wxString& sSetting, double dValue)
{
    Settings::Get().Write(GetSection(), sSetting, dValue);
}

wxString TestPluginBuilder::ReadSetting(const wxString& sSetting, const wxString& sDefault)
{
    return Settings::Get().Read(GetSection(), sSetting, sDefault);
}

int TestPluginBuilder::ReadSetting(const wxString& sSetting, int nDefault)
{
    return Settings::Get().Read(GetSection(), sSetting, nDefault);
}

double TestPluginBuilder::ReadSetting(const wxString& sSetting, double dDefault)
{
    return Settings::Get().Read(GetSection(), sSetting, dDefault);
}



void TestPluginBuilder::RegisterForSettingsUpdates(wxEvtHandler* pHandler,const wxString& sSetting)
{
    Settings::Get().AddHandler(pHandler, GetSection(), sSetting);
}

void TestPluginBuilder::SendWebsocketMessage(const Json::Value& jsMessage)
{
    if(RemoteApi::Get().WebsocketsActive())
    {
        Json::Value jsWebsocket;
        jsWebsocket["plugin"]["type"] = "test";
        jsWebsocket["plugin"]["name"] = GetName().ToStdString();
        jsWebsocket["data"] = jsMessage;
        RemoteApi::Get().SendPluginWebsocketMessage(jsWebsocket);
    }
}

bool TestPluginBuilder::WebsocketsActive()
{
    return RemoteApi::Get().WebsocketsActive();
}

void TestPluginBuilder::RegisterRemoteApiEnum(const wxString& sKey, const std::set<wxString>& setEnum, const wxString& sDefault)
{
    RemoteApi::Get().RegisterRemoteApiEnum(GetSection(), sKey, setEnum, sDefault);
}

void TestPluginBuilder::RegisterRemoteApiEnum(const wxString& sKey, const std::map<int, wxString>& mEnum, int nDefault)
{
    RemoteApi::Get().RegisterRemoteApiEnum(GetSection(), sKey, mEnum, nDefault);
}

void TestPluginBuilder::RegisterRemoteApiRangeDouble(const wxString& sKey, const std::pair<double, double>& dRange, double dDefault)
{
    RemoteApi::Get().RegisterRemoteApiRangeDouble(GetSection(), sKey, dRange, dDefault);
}

void TestPluginBuilder::RegisterRemoteApiRangeInt(const wxString& sKey, const std::pair<int, int>& nRange, int nDefault)
{
    RemoteApi::Get().RegisterRemoteApiRangeInt(GetSection(), sKey, nRange, nDefault);
}
void TestPluginBuilder::RegisterRemoteApiCallback(const wxString& sKey, std::function<std::set<wxString>()> func, const wxString& sDefault)
{
    RemoteApi::Get().RegisterRemoteApiCallback(GetSection(), sKey, func, sDefault);
}

void TestPluginBuilder::RegisterRemoteApiCallback(const wxString& sKey, std::function<std::map<int, wxString>()> func, int nDefault)
{
    RemoteApi::Get().RegisterRemoteApiCallback(GetSection(), sKey, func, nDefault);
}

void TestPluginBuilder::RegisterRemoteApi(const wxString& sKey, const wxString& sDefault)
{
    RemoteApi::Get().RegisterRemoteApi(GetSection(), sKey, sDefault);
}

void TestPluginBuilder::RegisterRemoteApiCSV(const wxString& sKey, const std::set<wxString>& setEnum, const wxString& sDefault)
{
    RemoteApi::Get().RegisterRemoteApiCSV(GetSection(), sKey, setEnum, sDefault);
}



bool TestPluginBuilder::IsLogActive()
{
    return (Settings::Get().Read(wxT("Tests"), wxT("Log"), 0) == 1);
}

void TestPluginBuilder::InitRemoteApi()
{
    RemoteApi::Get().AddPluginEndpoint(pml::restgoose::GET, endpoint("/x-pam/plugins/test/"+GetName().ToStdString()), std::bind(&TestPluginBuilder::GetStatus, this, _1,_2,_3,_4));
    RemoteApi::Get().AddPluginEndpoint(pml::restgoose::PATCH, endpoint("/x-pam/plugins/test/"+GetName().ToStdString()), std::bind(&TestPluginBuilder::PatchSetting, this, _1,_2,_3,_4));
}

pml::restgoose::response TestPluginBuilder::GetStatus(const query& theQuery, const std::vector<pml::restgoose::partData>& vData, const endpoint& theEndpoint, const userName& theUser)
{
    wxString sKey = RemoteApi::Get().ConvertQueryToMap(theQuery)["key"];

    pml::restgoose::response resp(200);
    resp.jsonData = RemoteApi::Get().GetSectionJson(GetSection(), sKey);
    return resp;

}

pml::restgoose::response TestPluginBuilder::PatchSetting(const query& theQuery, const std::vector<pml::restgoose::partData>& vData, const endpoint& theEndpoint, const userName& theUser)
{
    return RemoteApi::Get().DoPatchSettings(vData, GetSection());
}

