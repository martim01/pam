#include "[CLASS_PREFIX]register.h"
#include "[CLASS_PREFIX]builder.h"
#include "testpluginfactory.h"
#include "version.h"

void CreateTestBuilder()
{
    TestPluginFactory::Get()->Register(new [CLASS_PREFIX]Builder());
}

wxString GetTestPluginName()
{
    return wxT("[CLASS_PREFIX]");
}

wxString GetPluginVersion()
{
    return wxString::FromAscii(AutoVersion::FULLVERSION_STRING);
}

wxString GetDetails()
{
    return wxT("[DESCRIPTION]");
}
