#include "radar.h"
#include "radarbuilder.h"
#include "monitorpluginfactory.h"
#include "radar_version.h"

void CreateMonitorBuilder()
{
    MonitorPluginFactory::Get()->Register(new RadarBuilder());
}

wxString GetPluginName()
{
    return wxT("Radar");
}


wxString GetPluginVersion()
{
    return pml::radar::VERSION_STRING;
}

wxString GetDetails()
{
    return wxT("Displays a 'peak level radar' of an audio channel.");
}
