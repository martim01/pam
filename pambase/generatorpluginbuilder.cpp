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
    Settings::Get().Write(wxString::Format(wxT("Generator %s"), GetName().c_str()), sSetting, sValue);
}

void GeneratorPluginBuilder::WriteSetting(const wxString& sSetting, int nValue)
{
    Settings::Get().Write(wxString::Format(wxT("Generator %s"), GetName().c_str()), sSetting, nValue);
}

void GeneratorPluginBuilder::WriteSetting(const wxString& sSetting, double dValue)
{
    Settings::Get().Write(wxString::Format(wxT("Generator %s"), GetName().c_str()), sSetting, dValue);
}

wxString GeneratorPluginBuilder::ReadSetting(const wxString& sSetting, const wxString& sDefault)
{
    return Settings::Get().Read(wxString::Format(wxT("Generator %s"), GetName().c_str()), sSetting, sDefault);
}

int GeneratorPluginBuilder::ReadSetting(const wxString& sSetting, int nDefault)
{
    return Settings::Get().Read(wxString::Format(wxT("Generator %s"), GetName().c_str()), sSetting, nDefault);
}

double GeneratorPluginBuilder::ReadSetting(const wxString& sSetting, double dDefault)
{
    return Settings::Get().Read(wxString::Format(wxT("Generator %s"), GetName().c_str()), sSetting, dDefault);
}



void GeneratorPluginBuilder::RegisterForSettingsUpdates(const wxString& sSetting, wxEvtHandler* pHandler)
{
    Settings::Get().AddHandler(wxString::Format(wxT("Generator %s"), GetName().c_str()), sSetting, pHandler);
}


bool GeneratorPluginBuilder::IsLogActive()
{
    return (Settings::Get().Read(wxT("Generators"), wxT("Log"), 0) == 1);
}

void GeneratorPluginBuilder::InitRemoteApi()
{
    pmlLog() << "Generator: " << GetName().ToStdString() << "\tInitRemoteApi";
    RemoteApi::Get().AddPluginWebsocketEndpoint(endpoint(GetName().ToStdString()));
    RemoteApi::Get().AddPluginEndpoint(pml::restgoose::GET, endpoint("/x-pam/plugins/generator/"+GetName().ToStdString()), std::bind(&GeneratorPluginBuilder::GetStatus, this, _1,_2,_3,_4));
}

pml::restgoose::response GeneratorPluginBuilder::GetStatus(const query& theQuery, const std::vector<pml::restgoose::partData>& vData, const endpoint& theEndpoint, const userName& theUser)
{
    pml::restgoose::response resp;
    auto pSection = Settings::Get().GetSection(wxString::Format(wxT("Generator %s"), GetName().c_str()));
    if(pSection)
    {
        for(auto pairData : pSection->GetData())
        {
            resp.jsonData[pairData.first.ToStdString()] = pairData.second.ToStdString();
        }
    }
    return resp;

}