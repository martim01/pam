#include "fft.h"
#include "fftbuilder.h"
#include "monitorpluginfactory.h"

void CreateBuilders()
{
    MonitorPluginFactory::Get()->Register(new FFTBuilder());
}

wxString GetPluginName()
{
    return wxT("FFT");
}
