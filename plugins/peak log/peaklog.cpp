#include "peaklog.h"
#include "peaklogbuilder.h"
#include "testpluginfactory.h"

void CreateBuilders()
{
    TestPluginFactory::Get()->Register(new PeakLogBuilder());
}


wxString GetTestPluginName()
{
    return wxT("Peak Log");
}
