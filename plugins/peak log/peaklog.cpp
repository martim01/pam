#include "peaklog.h"
#include "peaklogbuilder.h"
#include "testpluginfactory.h"

void CreateTestBuilder()
{
    TestPluginFactory::Get()->Register(new PeakLogBuilder());
}


wxString GetTestPluginName()
{
    return wxT("Peak Log");
}
