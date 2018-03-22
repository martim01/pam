#include "meters.h"
#include "metersbuilder.h"
#include "monitorpluginfactory.h"
#include "version.h"

void CreateMonitorBuilder()
{
    MonitorPluginFactory::Get()->Register(new MetersBuilder());
}

wxString GetPluginName()
{
    return wxT("Meters");
}

wxString GetPluginVersion()
{
    return wxString::FromAscii(AutoVersion::FULLVERSION_STRING);
}

wxString GetDetails()
{
    return wxT("Displays a linear level meters for all the incoming audio channels.");
}
