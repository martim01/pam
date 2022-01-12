#include "aoipinfo.h"
#include "aoipinfobuilder.h"
#include "monitorpluginfactory.h"
#include "aoipinfo_version.h"

void CreateMonitorBuilder()
{
    MonitorPluginFactory::Get()->Register(new AoIPInfoBuilder());
}


wxString GetPluginName()
{
    return wxT("Session Info");

}


wxString GetPluginVersion()
{
    return pml::aoipinfo::VERSION_STRING;
}

wxString GetDetails()
{
    return wxT("Displays details about RTP sessions and QoS.");
}
