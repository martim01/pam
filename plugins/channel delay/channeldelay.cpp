#include "channeldelay.h"
#include "channeldelaybuilder.h"
#include "testpluginfactory.h"

void CreateTestBuilder()
{
    TestPluginFactory::Get()->Register(new ChannelDelayBuilder());
}


wxString GetTestPluginName()
{
    return wxT("Channel Delay");
}
