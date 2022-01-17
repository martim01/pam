#include "r128.h"
#include "r128builder.h"
#include "monitorpluginfactory.h"
#include "r128_version.h"

void CreateMonitorBuilder()
{
    MonitorPluginFactory::Get()->Register(new R128Builder());
}

wxString GetPluginName()
{
    return wxT("R128");
}


wxString GetPluginVersion()
{
    return pml::r128::VERSION_STRING;
}

wxString GetDetails()
{
    return wxT("Displays an EBU R128 loudness meter");
}
