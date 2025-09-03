#include "pophubregister.h"
#include "pophubbuilder.h"
#include "monitorpluginfactory.h"
#ifdef _CMAKE_
#include "pophub_version.h"
#endif

void CreateMonitorBuilder()
{
    MonitorPluginFactory::Get()->Register(new pophubBuilder());
}

wxString GetPluginName()
{
    return "pophub";
}

wxString GetPluginVersion()
{
    #ifdef _CMAKE_
    return pml::pophub::VERSION_STRING;
    #else
    return "Unknown";
    #endif
}

wxString GetDetails()
{
    return "Special plugin for Radio 1 etc";
}
