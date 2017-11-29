#include "levels.h"
#include "levelsbuilder.h"
#include "testpluginfactory.h"

void CreateBuilders()
{
    TestPluginFactory::Get()->Register(new LevelsBuilder());
}


wxString GetTestPluginName()
{
    return wxT("Levels");
}
