#include "meters.h"
#include "metersbuilder.h"
#include "monitorpluginfactory.h"

void CreateBuilders()
{
    MonitorPluginFactory::Get()->Register(new MetersBuilder());
}

wxString GetPluginName()
{
    return wxT("Meters");
}
