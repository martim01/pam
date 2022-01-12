#include "batteryregister.h"
#include "batterybuilder.h"
#include "monitorpluginfactory.h"
#include "battery_version.h"

void CreateMonitorBuilder()
{
    MonitorPluginFactory::Get()->Register(new batteryBuilder());
}

wxString GetPluginName()
{
    return wxT("battery");
}

wxString GetPluginVersion()
{
    return pml::battery::VERSION_STRING;
}

wxString GetDetails()
{
    return wxT("Monitor PiJuice battery status");
}

