#include "peakcount.h"
#include "peakcountbuilder.h"
#include "testpluginfactory.h"
#include "version.h"

void CreateTestBuilder()
{
    TestPluginFactory::Get()->Register(new PeakCountBuilder());
}


wxString GetTestPluginName()
{
    return wxT("Peak Count");
}


wxString GetPluginVersion()
{
    return wxString::FromUTF8(AutoVersion::FULLVERSION_STRING);
}

wxString GetDetails()
{
    return wxT("Counts the number of samples above a set dBFS value.");
}
