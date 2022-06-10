#include "fftdiffregister.h"
#include "fftdiffbuilder.h"
#include "monitorpluginfactory.h"
#include "version.h"

void CreateMonitorBuilder()
{
    MonitorPluginFactory::Get()->Register(new fftdiffBuilder());
}

wxString GetPluginName()
{
    return "fftdiff";
}

wxString GetPluginVersion()
{
    return wxString::FromAscii(AutoVersion::FULLVERSION_STRING);
}

wxString GetDetails()
{
    return "";
}
