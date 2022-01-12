#include "MP3Streamerregister.h"
#include "MP3Streamerbuilder.h"
#include "monitorpluginfactory.h"
#include "mp3streamer_version.h"

void CreateMonitorBuilder()
{
    MonitorPluginFactory::Get()->Register(new MP3StreamerBuilder());
}

wxString GetPluginName()
{
    return wxT("MP3Streamer");
}

wxString GetPluginVersion()
{
    return pml::mp3streamer::VERSION_STRING;
}

wxString GetDetails()
{
    return wxT("Plugin that will stream incoming audio out as MP3");
}

