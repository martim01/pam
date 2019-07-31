#include "fftphaseregister.h"
#include "fftphasebuilder.h"
#include "monitorpluginfactory.h"
#include "version.h"

void CreateMonitorBuilder()
{
    MonitorPluginFactory::Get()->Register(new fftphaseBuilder());
}

wxString GetPluginName()
{
    return wxT("fftphase");
}

wxString GetPluginVersion()
{
    return wxString::FromAscii(AutoVersion::FULLVERSION_STRING);
}

wxString GetDetails()
{
    return wxT("Stereo fft meter showing phase at different frequencies");
}

