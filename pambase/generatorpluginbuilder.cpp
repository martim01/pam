#include "generatorpluginbuilder.h"
#include "wmswitcherpanel.h"
#include "settings.h"
#include <wx/log.h>

using namespace std;


GeneratorPluginBuilder::GeneratorPluginBuilder() :
    m_pHandler(0),
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
