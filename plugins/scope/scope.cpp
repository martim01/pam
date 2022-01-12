#include "scope.h"
#include "scopebuilder.h"
#include "monitorpluginfactory.h"
#include "scope_version.h"


void CreateMonitorBuilder()
{
    MonitorPluginFactory::Get()->Register(new ScopeBuilder());
}


wxString GetPluginName()
{
    return wxT("Scope");
}


wxString GetPluginVersion()
{
    return pml::scope::VERSION_STRING;
}

wxString GetDetails()
{
    return wxT("Displays an graph of time vs amplitude for an incoming audio channel");
}
