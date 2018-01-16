#include "fft.h"
#include "fftbuilder.h"
#include "monitorpluginfactory.h"

void CreateMonitorBuilder()
{
    MonitorPluginFactory::Get()->Register(new FFTBuilder());
}

wxString GetPluginName()
{
    return wxT("FFT");
}
