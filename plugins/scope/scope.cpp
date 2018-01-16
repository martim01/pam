#include "scope.h"
#include "scopebuilder.h"
#include "monitorpluginfactory.h"

void CreateMonitorBuilder()
{
    MonitorPluginFactory::Get()->Register(new ScopeBuilder());
}


wxString GetPluginName()
{
    return wxT("Scope");
}
