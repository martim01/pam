#include "InputAlignregister.h"
#include "InputAlignbuilder.h"
#include "testpluginfactory.h"
#include "InputAlign_version.h"

void CreateTestBuilder()
{
    TestPluginFactory::Get()->Register(new InputAlignBuilder());
}

wxString GetTestPluginName()
{
    return wxT("InputAlign");
}

wxString GetPluginVersion()
{
    return pml::InputAlign::VERSION_STRING;
}

wxString GetDetails()
{
    return wxT("Simple plugin that makes sure the input gain on the soundcard is correct");
}


