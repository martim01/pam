#include "record.h"
#include "recordbuilder.h"
#include "testpluginfactory.h"
#include "version.h"

void CreateTestBuilder()
{
    TestPluginFactory::Get()->Register(new RecordBuilder());
}


wxString GetTestPluginName()
{
    return wxT("Record");
}


wxString GetPluginVersion()
{
    return wxString::FromAscii(AutoVersion::FULLVERSION_STRING);
}

wxString GetDetails()
{
    return wxT("Records the incoming audio to a file");
}
