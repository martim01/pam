#include "radar.h"
#include "radarbuilder.h"
#include "monitorpluginfactory.h"

void CreateBuilders()
{
    MonitorPluginFactory::Get()->Register(new RadarBuilder());
}

wxString GetPluginName()
{
    return wxT("Radar");
}
