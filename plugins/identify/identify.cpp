#include "identify.h"
#include "identifybuilder.h"
#include "testpluginfactory.h"

void CreateBuilders()
{
    TestPluginFactory::Get()->Register(new IdentifyBuilder());
}


wxString GetPluginName()
{
    return wxT("Identify");
}
