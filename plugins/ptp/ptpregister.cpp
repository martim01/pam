#include "ptpregister.h"
#include "ptpbuilder.h"
#include "monitorpluginfactory.h"
#include "version.h"

void CreateMonitorBuilder()
{
    MonitorPluginFactory::Get()->Register(new ptpBuilder());
}

wxString GetPluginName()
{
    return wxT("ptp");
}

wxString GetPluginVersion()
{
    return wxString::FromAscii(AutoVersion::FULLVERSION_STRING);
}

wxString GetDetails()
{
    return wxT("");
}

