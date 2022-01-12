#include "ptpregister.h"
#include "ptpbuilder.h"
#include "monitorpluginfactory.h"
#include "ptp_version.h"

void CreateMonitorBuilder()
{
    MonitorPluginFactory::Get()->Register(new ptpBuilder());
}

wxString GetPluginName()
{
    return wxT("PTP Monkey");
}

wxString GetPluginVersion()
{
    return pml::ptp::VERSION_STRING;
}

wxString GetDetails()
{
    return wxT("");
}

