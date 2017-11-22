#include "channeldelaybuilder.h"
#include "pnlChannelDelay.h"

ChannelDelayBuilder::ChannelDelayBuilder() : TestPluginBuilder(),
m_ppnlDelay(0)
{

}

ChannelDelayBuilder::~ChannelDelayBuilder()
{
}

void ChannelDelayBuilder::InputSession(const session& aSession)
{
    m_ppnlDelay->InputSession(aSession);
}

void ChannelDelayBuilder::OutputChannels(const std::vector<char>& vChannels)
{

}

void ChannelDelayBuilder::SetAudioData(const timedbuffer* pBuffer)
{
    m_ppnlDelay->SetData(pBuffer);
}

wxString ChannelDelayBuilder::GetName() const
{
    return wxT("Channel Delay");
}


wxWindow* ChannelDelayBuilder::CreateTestPanel(wxWindow* pParent)
{
    m_ppnlDelay = new pnlChannelDelay(pParent, this);
    return m_ppnlDelay;
}

void ChannelDelayBuilder::LoadSettings()
{

}



