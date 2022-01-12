#include "channeldelay.h"
#include "channeldelaybuilder.h"
#include "testpluginfactory.h"
#include "channeldelay_version.h"

void CreateTestBuilder()
{
    TestPluginFactory::Get()->Register(new ChannelDelayBuilder());
}


wxString GetTestPluginName()
{
    return wxT("Channel Delay");
}


wxString GetPluginVersion()
{
    return pml::channeldelay::VERSION_STRING;

}

wxString GetDetails()
{
    return wxT("Calculates and displays the delay between two channels of incoming audio.\nThe channels should contain the same audio, offset by time.");
}
