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
    Settings::Get().Write(GetName(), sSetting, sValue);
}

void TestPluginBuilder::WriteSetting(const wxString& sSetting, int nValue)
{
    Settings::Get().Write(GetName(), sSetting, nValue);
}

void TestPluginBuilder::WriteSetting(const wxString& sSetting, double dValue)
{
    Settings::Get().Write(GetName(), sSetting, dValue);
}

wxString TestPluginBuilder::ReadSetting(const wxString& sSetting, const wxString& sDefault)
{
    return Settings::Get().Read(GetName(), sSetting, sDefault);
}

int TestPluginBuilder::ReadSetting(const wxString& sSetting, int nDefault)
{
    return Settings::Get().Read(GetName(), sSetting, nDefault);
}

double TestPluginBuilder::ReadSetting(const wxString& sSetting, double dDefault)
{
    return Settings::Get().Read(GetName(), sSetting, dDefault);
}



void TestPluginBuilder::RegisterForSettingsUpdates(const wxString& sSetting, wxEvtHandler* pHandler)
{
    Settings::Get().AddHandler(pHandler, GetName(), sSetting);
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

void TestPluginBuilder::RegisterRemoteApiEnum(const wxString& sKey, const std::set<wxString>& setEnum)
{
    RemoteApi::Get().RegisterRemoteApiEnum(GetName(), sKey, setEnum);
}

void TestPluginBuilder::RegisterRemoteApiEnum(const wxString& sKey, const std::map<int, wxString>& mEnum)
{
    RemoteApi::Get().RegisterRemoteApiEnum(GetName(), sKey, mEnum);
}

void TestPluginBuilder::RegisterRemoteApiRangeDouble(const wxString& sKey, const std::pair<double, double>& dRange)
{
    RemoteApi::Get().RegisterRemoteApiRangeDouble(GetName(), sKey, dRange);
}

void TestPluginBuilder::RegisterRemoteApiRangeInt(const wxString& sKey, const std::pair<int, int>& nRange)
{
    RemoteApi::Get().RegisterRemoteApiRangeInt(GetName(), sKey, nRange);
}
void TestPluginBuilder::RegisterRemoteApiCallback(const wxString& sKey, std::function<std::set<wxString>()> func)
{
    RemoteApi::Get().RegisterRemoteApiCallback(GetName(), sKey, func);
}

void TestPluginBuilder::RegisterRemoteApiCallback(const wxString& sKey, std::function<std::map<int, wxString>()> func)
{
    RemoteApi::Get().RegisterRemoteApiCallback(GetName(), sKey, func);
}

void TestPluginBuilder::RegisterRemoteApi(const wxString& sKey)
{
    RemoteApi::Get().RegisterRemoteApi(GetName(), sKey);
}

void TestPluginBuilder::RegisterRemoteApiCSV(const wxString& sKey, const std::set<wxString>& setEnum)
{
    RemoteApi::Get().RegisterRemoteApiCSV(GetName(), sKey, setEnum);
}



bool TestPluginBuilder::IsLogActive()
{
    return (Settings::Get().Read(wxT("Tests"), wxT("Log"), 0) == 1);
}

void TestPluginBuilder::InitRemoteApi()
{
    RemoteApi::Get().AddPluginEndpoint(pml::restgoose::GET, endpoint("/x-pam/plugins/test/"+GetName().ToStdString()), std::bind(&TestPluginBuilder::GetStatus, this, _1,_2,_3,_4));
}

pml::restgoose::response TestPluginBuilder::GetStatus(const query& theQuery, const std::vector<pml::restgoose::partData>& vData, const endpoint& theEndpoint, const userName& theUser)
{
    pml::restgoose::response resp;
    auto pSection = Settings::Get().GetSection(GetName());
    if(pSection)
    {
        for(auto pairData : pSection->GetData())
        {
            resp.jsonData[pairData.first.ToStdString()] = pairData.second.ToStdString();
        }
    }
    return resp;

}
