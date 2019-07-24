#include "batteryregister.h"
#include "batterybuilder.h"
#include "monitorpluginfactory.h"
#include "version.h"

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
    return wxString::FromAscii(AutoVersion::FULLVERSION_STRING);
}

wxString GetDetails()
{
    return wxT("Monitor PiJuice battery status");
}

