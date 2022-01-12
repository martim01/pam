#include "levels.h"
#include "levelsbuilder.h"
#include "testpluginfactory.h"
#include "levels_version.h"

void CreateTestBuilder()
{
    TestPluginFactory::Get()->Register(new LevelsBuilder());
}


wxString GetTestPluginName()
{
    return wxT("Levels");
}


wxString GetPluginVersion()
{
    return pml::levels::VERSION_STRING;
}

wxString GetDetails()
{
    return wxT("Displays a the current peak level and overall range of the incoming audio.\nUseful for checking for glitches in solid tone");
}
