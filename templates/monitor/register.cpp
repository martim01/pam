#include "[CLASS_PREFIX]register.h"
#include "[CLASS_PREFIX]builder.h"
#include "monitorpluginfactory.h"
#include "version.h"

void CreateMonitorBuilder()
{
    MonitorPluginFactory::Get()->Register(new [CLASS_PREFIX]Builder());
}

wxString GetPluginName()
{
    return wxT("[CLASS_PREFIX]");
}

wxString GetPluginVersion()
{
    return wxString::FromAscii(AutoVersion::FULLVERSION_STRING);
}

wxString GetDetails()
{
    return wxT("[DESCRIPTION]");
}
