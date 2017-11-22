#include "channeldelay.h"
#include "channeldelaybuilder.h"
#include "testpluginfactory.h"

void CreateBuilders()
{
    TestPluginFactory::Get()->Register(new ChannelDelayBuilder());
}


wxString GetPluginName()
{
    return wxT("Channel Delay");
}
