#include "waveformregister.h"
#include "waveformbuilder.h"
#include "monitorpluginfactory.h"
#include "waveform_version.h"

void CreateMonitorBuilder()
{
    MonitorPluginFactory::Get()->Register(new waveformBuilder());
}

wxString GetPluginName()
{
    return wxT("waveform");
}

wxString GetPluginVersion()
{
    return pml::waveform::VERSION_STRING;
}

wxString GetDetails()
{
    return wxT("Shows a rolling waveform of the incoming audio");
}

