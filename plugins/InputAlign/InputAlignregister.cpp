<<<<<<< HEAD
#include "InputAlignregister.h"
#include "InputAlignbuilder.h"
#include "testpluginfactory.h"
#include "version.h"

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
    return wxString::FromAscii(AutoVersion::FULLVERSION_STRING);
}

wxString GetDetails()
{
    return wxT("Simple plugin that makes sure the input gain on the soundcard is correct");
}

=======
#include "InputAlignregister.h"
#include "InputAlignbuilder.h"
#include "testpluginfactory.h"
#include "version.h"

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
    return wxString::FromAscii(AutoVersion::FULLVERSION_STRING);
}

wxString GetDetails()
{
    return wxT("Simple plugin that makes sure the input gain on the soundcard is correct");
}

>>>>>>> 48cbadaff16dba28b31960ccd27596c44fa587cf
