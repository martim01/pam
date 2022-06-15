#include "fftdiffregister.h"
#include "fftdiffbuilder.h"
#include "testpluginfactory.h"
#include "version.h"

void CreateTestBuilder()
{
    TestPluginFactory::Get()->Register(new fftdiffBuilder());
}

wxString GetTestPluginName()
{
    return "C.A.R.";
}

wxString GetPluginVersion()
{
    return wxString::FromAscii(AutoVersion::FULLVERSION_STRING);
}

wxString GetDetails()
{
    return "Displays a circuit's audio response and latency";
}
