#include "lissajou.h"
#include "lissajoubuilder.h"
#include "monitorpluginfactory.h"

void CreateMonitorBuilder()
{
    MonitorPluginFactory::Get()->Register(new LissajouBuilder());
}

wxString GetPluginName()
{
    return wxT("Lissajou");
}
