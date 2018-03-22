#include "polarscope.h"
#include "polarscopebuilder.h"
#include "monitorpluginfactory.h"
#include "version.h"

void CreateMonitorBuilder()
{
    MonitorPluginFactory::Get()->Register(new PolarScopeBuilder());
}

wxString GetPluginName()
{
    return wxT("Polar Scope");
}


wxString GetPluginVersion()
{
    return wxString::FromAscii(AutoVersion::FULLVERSION_STRING);
}

wxString GetDetails()
{
    return wxT("Displays a 'polar' lissajou of two incoming audio channels, the phase difference between the two channels and the relative balance.");
}
