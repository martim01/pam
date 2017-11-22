#include "scope.h"
#include "scopebuilder.h"
#include "monitorpluginfactory.h"

void CreateBuilders()
{
    MonitorPluginFactory::Get()->Register(new ScopeBuilder());
}


wxString GetPluginName()
{
    return wxT("Scope");
}
