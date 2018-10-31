#include "bufferlogregister.h"
#include "bufferlogbuilder.h"
#include "testpluginfactory.h"
#include "version.h"

void CreateTestBuilder()
{
    TestPluginFactory::Get()->Register(new bufferlogBuilder());
}

wxString GetTestPluginName()
{
    return wxT("bufferlog");
}

wxString GetPluginVersion()
{
    return wxString::FromAscii(AutoVersion::FULLVERSION_STRING);
}

wxString GetDetails()
{
    return wxT("");
}

