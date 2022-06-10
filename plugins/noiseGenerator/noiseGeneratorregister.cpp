#include "noiseGeneratorregister.h"
#include "noiseGeneratorbuilder.h"
#include "generatorpluginfactory.h"
#include "noiseGenerator_version.h"

void CreateGeneratorBuilder()
{
    GeneratorPluginFactory::Get()->Register(new noiseGeneratorBuilder());
}

wxString GetGeneratorPluginName()
{
    return wxT("Noise");
}

wxString GetPluginVersion()
{
    return pml::noiseGenerator::VERSION_STRING;
}

wxString GetDetails()
{
    return wxT("");
}

