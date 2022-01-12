#include "identify.h"
#include "identifybuilder.h"
#include "testpluginfactory.h"
#include "identify_version.h"

void CreateTestBuilder()
{
    TestPluginFactory::Get()->Register(new IdentifyBuilder());
}


wxString GetTestPluginName()
{
    return wxT("Identify");
}


wxString GetPluginVersion()
{
    return pml::identify::VERSION_STRING;
}

wxString GetDetails()
{
    return wxT("Uses Gracenote to attempt to identify the current audio.");
}
