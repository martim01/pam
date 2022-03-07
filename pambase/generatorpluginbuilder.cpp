#include "generatorpluginbuilder.h"
#include "wmswitcherpanel.h"
#include "settings.h"
#include <wx/log.h>
#include "remoteapi.h"
#include "log.h"

using namespace std;
using namespace std::placeholders;


GeneratorPluginBuilder::GeneratorPluginBuilder() :
    m_pHandler(0),
    m_dSampleRate(48000.0),
	m_pswpGenerators(0)
{

}

void GeneratorPluginBuilder::SetHandler(wxEvtHandler* pHandler)
{
    m_pHandler = pHandler;
}

void GeneratorPluginBuilder::CreatePanels(wmSwitcherPanel* pswpGenerators)
{
    pswpGenerators->AddPage(CreateGeneratorPanel(pswpGenerators), GetName(), false);

    LoadSettings();
}



void GeneratorPluginBuilder::WriteSetting(const wxString& sSetting, const wxString& sValue)
{
    Settings::Get().Write(GetSection(), sSetting, sValue);
}

void GeneratorPluginBuilder::WriteSetting(const wxString& sSetting, int nValue)
{
    Settings::Get().Write(GetSection(), sSetting, nValue);
}

void GeneratorPluginBuilder::WriteSetting(const wxString& sSetting, double dValue)
{
    Settings::Get().Write(GetSection(), sSetting, dValue);
}

wxString GeneratorPluginBuilder::ReadSetting(const wxString& sSetting, const wxString& sDefault)
{
    return Settings::Get().Read(GetSection(), sSetting, sDefault);
}

int GeneratorPluginBuilder::ReadSetting(const wxString& sSetting, int nDefault)
{
    return Settings::Get().Read(GetSection(), sSetting, nDefault);
}

double GeneratorPluginBuilder::ReadSetting(const wxString& sSetting, double dDefault)
{
    return Settings::Get().Read(GetSection(), sSetting, dDefault);
}



void GeneratorPluginBuilder::RegisterForSettingsUpdates(wxEvtHandler* pHandler, const wxString& sSetting)
{
    Settings::Get().AddHandler(this, GetSection(), sSetting);
}


bool GeneratorPluginBuilder::IsLogActive()
{
    return (Settings::Get().Read(wxT("Generators"), wxT("Log"), 0) == 1);
}

void GeneratorPluginBuilder::InitRemoteApi()
{
    pmlLog() << "Generator: " << GetName().ToStdString() << "\tInitRemoteApi";
    RemoteApi::Get().AddPluginEndpoint(pml::restgoose::GET, endpoint("/x-pam/plugins/generator/"+GetName().ToStdString()), std::bind(&GeneratorPluginBuilder::GetStatus, this, _1,_2,_3,_4));
    RemoteApi::Get().AddPluginEndpoint(pml::restgoose::PATCH, endpoint("/x-pam/plugins/generator/"+GetName().ToStdString()), std::bind(&GeneratorPluginBuilder::PatchSetting, this, _1,_2,_3,_4));
}

pml::restgoose::response GeneratorPluginBuilder::GetStatus(const query& theQuery, const std::vector<pml::restgoose::partData>& vData, const endpoint& theEndpoint, const userName& theUser)
{
    wxString sKey = RemoteApi::Get().ConvertQueryToMap(theQuery)["key"];

    pml::restgoose::response resp(200);
    resp.jsonData = RemoteApi::Get().GetSectionJson(GetSection(), sKey);
    return resp;

}

pml::restgoose::response GeneratorPluginBuilder::PatchSetting(const query& theQuery, const std::vector<pml::restgoose::partData>& vData, const endpoint& theEndpoint, const userName& theUser)
{
    return RemoteApi::Get().DoPatchSettings(vData, GetSection());
}

