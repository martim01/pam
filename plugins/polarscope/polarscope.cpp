#include "polarscope.h"
#include "polarscopebuilder.h"
#include "monitorpluginfactory.h"

void CreateMonitorBuilder()
{
    MonitorPluginFactory::Get()->Register(new PolarScopeBuilder());
}

wxString GetPluginName()
{
    return wxT("Polar Scope");
}
