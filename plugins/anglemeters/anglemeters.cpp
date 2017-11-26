#include "anglemeters.h"
#include "anglemetersbuilder.h"
#include "monitorpluginfactory.h"

void CreateBuilders()
{
    MonitorPluginFactory::Get()->Register(new AngleMetersBuilder());
}

wxString GetPluginName()
{
    return wxT("Angle Meters");
}
