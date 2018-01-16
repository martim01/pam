#include "meters.h"
#include "metersbuilder.h"
#include "monitorpluginfactory.h"

void CreateMonitorBuilder()
{
    MonitorPluginFactory::Get()->Register(new MetersBuilder());
}

wxString GetPluginName()
{
    return wxT("Meters");
}
