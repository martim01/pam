#include "LTCGeneratorregister.h"
#include "LTCGeneratorbuilder.h"
#include "generatorpluginfactory.h"
#include "version.h"

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
    return wxString::FromUTF8(AutoVersion::FULLVERSION_STRING);
}

wxString GetDetails()
{
    return wxT("");
}

