#include "LTCregister.h"
#include "LTCbuilder.h"
#include "testpluginfactory.h"
#include "version.h"

void CreateTestBuilder()
{
    TestPluginFactory::Get()->Register(new LTCBuilder());
}

wxString GetTestPluginName()
{
    return wxT("LTC");
}

wxString GetPluginVersion()
{
    return wxString::FromUTF8(AutoVersion::FULLVERSION_STRING);
}

wxString GetDetails()
{
    return wxT("");
}

