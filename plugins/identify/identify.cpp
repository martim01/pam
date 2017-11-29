#include "identify.h"
#include "identifybuilder.h"
#include "testpluginfactory.h"

void CreateBuilders()
{
    TestPluginFactory::Get()->Register(new IdentifyBuilder());
}


wxString GetTestPluginName()
{
    return wxT("Identify");
}
