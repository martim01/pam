#include "bobregister.h"
#include "bobbuilder.h"
#include "monitorpluginfactory.h"
#ifdef _CMAKE_
#include "bob_version.h"
#endif

void CreateMonitorBuilder()
{
    MonitorPluginFactory::Get()->Register(new bobBuilder());
}

wxString GetPluginName()
{
    return "bob";
}

wxString GetPluginVersion()
{
    #ifdef _CMAKE_
    return pml::bob::VERSION_STRING;
    #else
    return "Unknown";
    #endif
}

wxString GetDetails()
{
    return "";
}
