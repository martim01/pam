#include "peaklog.h"
#include "peaklogbuilder.h"
#include "testpluginfactory.h"
#include "version.h"

void CreateTestBuilder()
{
    TestPluginFactory::Get()->Register(new PeakLogBuilder());
}


wxString GetTestPluginName()
{
    return wxT("Peak Log");
}


wxString GetPluginVersion()
{
    return wxString::FromAscii(AutoVersion::FULLVERSION_STRING);
}

wxString GetDetails()
{
    return wxT("Displays a graph of peak level vs time.");
}
