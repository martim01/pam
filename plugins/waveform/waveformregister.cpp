#include "waveformregister.h"
#include "waveformbuilder.h"
#include "monitorpluginfactory.h"
#include "version.h"

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
    return wxString::FromAscii(AutoVersion::FULLVERSION_STRING);
}

wxString GetDetails()
{
    return wxT("Shows a rolling waveform of the incoming audio");
}

