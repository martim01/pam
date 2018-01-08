#include "aoipinfo.h"
#include "aoipinfobuilder.h"
#include "monitorpluginfactory.h"

void CreateMonitorBuilder()
{
    MonitorPluginFactory::Get()->Register(new AoIPInfoBuilder());
}


wxString GetPluginName()
{
    return wxT("AoIP Info");
}
