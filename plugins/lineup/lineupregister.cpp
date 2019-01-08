#include "lineupregister.h"
#include "lineupbuilder.h"
#include "testpluginfactory.h"
#include "version.h"

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
    return wxString::FromAscii(AutoVersion::FULLVERSION_STRING);
}

wxString GetDetails()
{
    return wxT("Test page for line up of lines");
}

