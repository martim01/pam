#include "levels.h"
#include "levelsbuilder.h"
#include "testpluginfactory.h"

void CreateTestBuilder()
{
    TestPluginFactory::Get()->Register(new LevelsBuilder());
}


wxString GetTestPluginName()
{
    return wxT("Levels");
}
