#include "levels.h"
#include "levelsbuilder.h"
#include "testpluginfactory.h"

void CreateBuilders()
{
    TestPluginFactory::Get()->Register(new LevelsBuilder());
}


wxString GetPluginName()
{
    return wxT("Levels");
}
