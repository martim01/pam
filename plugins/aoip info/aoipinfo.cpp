#include "aoipinfo.h"
#include "aoipinfobuilder.h"
#include "monitorpluginfactory.h"

void CreateBuilders()
{
    MonitorPluginFactory::Get()->Register(new AoIPInfoBuilder());
}


wxString GetPluginName()
{
    return wxT("AoIP Info");
}
