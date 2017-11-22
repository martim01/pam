#include "lissajou.h"
#include "lissajoubuilder.h"
#include "monitorpluginfactory.h"

void CreateBuilders()
{
    MonitorPluginFactory::Get()->Register(new LissajouBuilder());
}

wxString GetPluginName()
{
    return wxT("Lissajou");
}
