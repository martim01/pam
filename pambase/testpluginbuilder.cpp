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
    Settings::Get().Write(wxString::Format(wxT("Test %s"), GetName().c_str()), sSetting, sValue);
}

void TestPluginBuilder::WriteSetting(const wxString& sSetting, int nValue)
{
    Settings::Get().Write(wxString::Format(wxT("Test %s"), GetName().c_str()), sSetting, nValue);
}

void TestPluginBuilder::WriteSetting(const wxString& sSetting, double dValue)
{
    Settings::Get().Write(wxString::Format(wxT("Test %s"), GetName().c_str()), sSetting, dValue);
}

wxString TestPluginBuilder::ReadSetting(const wxString& sSetting, const wxString& sDefault)
{
    return Settings::Get().Read(wxString::Format(wxT("Test %s"), GetName().c_str()), sSetting, sDefault);
}

int TestPluginBuilder::ReadSetting(const wxString& sSetting, int nDefault)
{
    return Settings::Get().Read(wxString::Format(wxT("Test %s"), GetName().c_str()), sSetting, nDefault);
}

double TestPluginBuilder::ReadSetting(const wxString& sSetting, double dDefault)
{
    return Settings::Get().Read(wxString::Format(wxT("Test %s"), GetName().c_str()), sSetting, dDefault);
}



void TestPluginBuilder::RegisterForSettingsUpdates(const wxString& sSetting, wxEvtHandler* pHandler)
{
    Settings::Get().AddHandler(wxString::Format(wxT("Test %s"), GetName().c_str()), sSetting, pHandler);
}


bool TestPluginBuilder::IsLogActive()
{
    return (Settings::Get().Read(wxT("Tests"), wxT("Log"), 0) == 1);
}

void TestPluginBuilder::InitRemoteApi()
{
    RemoteApi::Get().AddPluginWebsocketEndpoint(endpoint(GetName().ToStdString()));
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