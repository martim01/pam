#include "peakcount.h"
#include "peakcountbuilder.h"
#include "testpluginfactory.h"
#include "peakcount_version.h"

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
    return pml::peakcount::VERSION_STRING;
}

wxString GetDetails()
{
    return wxT("Counts the number of samples above a set dBFS value.");
}
