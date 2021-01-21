#include "fft.h"
#include "fftbuilder.h"
#include "monitorpluginfactory.h"
#include "version.h"

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
    return wxString::FromUTF8(AutoVersion::FULLVERSION_STRING);
}

wxString GetDetails()
{
    return wxT("Displays a graph of amplitude vs frequency.\n");
}
