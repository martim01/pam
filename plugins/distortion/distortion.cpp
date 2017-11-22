#include "distortion.h"
#include "distortionbuilder.h"
#include "testpluginfactory.h"

void CreateBuilders()
{
    TestPluginFactory::Get()->Register(new DistortionBuilder());
}


wxString GetPluginName()
{
    return wxT("Distortion");
}
