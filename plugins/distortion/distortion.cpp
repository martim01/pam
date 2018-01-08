#include "distortion.h"
#include "distortionbuilder.h"
#include "testpluginfactory.h"

void CreateTestBuilder()
{
    TestPluginFactory::Get()->Register(new DistortionBuilder());
}


wxString GetTestPluginName()
{
    return wxT("Distortion");
}
