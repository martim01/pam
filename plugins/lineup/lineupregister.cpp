#include "lineupregister.h"
#include "lineupbuilder.h"
#include "testpluginfactory.h"
#include "lineup_version.h"

void CreateTestBuilder()
{
    TestPluginFactory::Get()->Register(new lineupBuilder());
}

wxString GetTestPluginName()
{
    return wxT("lineup");
}

wxString GetPluginVersion()
{
    return pml::lineup::VERSION_STRING;
}

wxString GetDetails()
{
    return wxT("Test page for line up of lines");
}

