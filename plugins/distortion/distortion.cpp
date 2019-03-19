#include "distortion.h"
#include "distortionbuilder.h"
#include "testpluginfactory.h"
#include "version.h"

void CreateTestBuilder()
{
    TestPluginFactory::Get()->Register(new DistortionBuilder());
}


wxString GetTestPluginName()
{
    return wxT("Distortion");

}


wxString GetPluginVersion()
{
    return wxString::FromAscii(AutoVersion::FULLVERSION_STRING);
}

wxString GetDetails()
{
    return wxT("Displays details about the harmonic distortion of incoming audio channels");
}
