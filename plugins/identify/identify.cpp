#include "identify.h"
#include "identifybuilder.h"
#include "testpluginfactory.h"
#include "version.h"

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
    return wxString::FromUTF8(AutoVersion::FULLVERSION_STRING);
}

wxString GetDetails()
{
    return wxT("Uses Gracenote to attempt to identify the current audio.");
}
