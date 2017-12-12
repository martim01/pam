#include "polarscope.h"
#include "polarscopebuilder.h"
#include "monitorpluginfactory.h"

void CreateBuilders()
{
    MonitorPluginFactory::Get()->Register(new PolarScopeBuilder());
}

wxString GetPluginName()
{
    return wxT("Polar Scope");
}
