#include "anglemeters.h"
#include "anglemetersbuilder.h"
#include "monitorpluginfactory.h"
#include "anglemeters_version.h"


void CreateMonitorBuilder()
{
    MonitorPluginFactory::Get()->Register(new AngleMetersBuilder());
}

wxString GetPluginName()
{
    return wxT("Angle Meters");
}


wxString GetPluginVersion()
{
    return pml::anglemeters::VERSION_STRING;
}

wxString GetDetails()
{
    return wxT("Displays a 'moving coil' level meter of the incoming audio channels");
}
