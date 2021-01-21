#include "r128.h"
#include "r128builder.h"
#include "monitorpluginfactory.h"
#include "version.h"

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
    return wxString::FromUTF8(AutoVersion::FULLVERSION_STRING);
}

wxString GetDetails()
{
    return wxT("Displays an EBU R128 loudness meter");
}
