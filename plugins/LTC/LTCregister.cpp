#include "LTCregister.h"
#include "LTCbuilder.h"
#include "testpluginfactory.h"
#include "LTC_version.h"

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
    return pml::LTC::VERSION_STRING;
}

wxString GetDetails()
{
    return wxT("");
}

