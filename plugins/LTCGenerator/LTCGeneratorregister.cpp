#include "LTCGeneratorregister.h"
#include "LTCGeneratorbuilder.h"
#include "generatorpluginfactory.h"
#include "LTCGenerator_version.h"

void CreateGeneratorBuilder()
{
    GeneratorPluginFactory::Get()->Register(new LTCGeneratorBuilder());
}

wxString GetGeneratorPluginName()
{
    return wxT("LTC");
}

wxString GetPluginVersion()
{
    return pml::LTCGenerator::VERSION_STRING;
}

wxString GetDetails()
{
    return wxT("");
}

