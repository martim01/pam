#include "identify.h"
#include "identifybuilder.h"
#include "testpluginfactory.h"

void CreateTestBuilder()
{
    TestPluginFactory::Get()->Register(new IdentifyBuilder());
}


wxString GetTestPluginName()
{
    return wxT("Identify");
}
