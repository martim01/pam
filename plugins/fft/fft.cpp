#include "fft.h"
#include "fftbuilder.h"
#include "monitorpluginfactory.h"
#include "fft_version.h"

void CreateMonitorBuilder()
{
    MonitorPluginFactory::Get()->Register(new FFTBuilder());
}

wxString GetPluginName()
{
    return wxT("FFT");
}


wxString GetPluginVersion()
{
    return pml::fft::VERSION_STRING;
}

wxString GetDetails()
{
    return wxT("Displays a graph of amplitude vs frequency.\n");
}
