#include "lissajou.h"
#include "lissajoubuilder.h"
#include "monitorpluginfactory.h"
#include "lissajou_version.h"

void CreateMonitorBuilder()
{
    MonitorPluginFactory::Get()->Register(new LissajouBuilder());
}

wxString GetPluginName()
{
    return wxT("Lissajou");
}


wxString GetPluginVersion()
{
    return pml::lissajou::VERSION_STRING;
}

wxString GetDetails()
{

    return wxT("Displays a lissajou image created from two input channels.\nAlso display linear level meters of these two channels");
}

