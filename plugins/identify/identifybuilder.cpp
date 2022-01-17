#include "identifybuilder.h"
#include "pnlIdentify.h"


IdentifyBuilder::IdentifyBuilder() : TestPluginBuilder(),
m_ppnlIdentify(0)
{

}

IdentifyBuilder::~IdentifyBuilder()
{
}

void IdentifyBuilder::InputSession(const session& aSession)
{
    m_ppnlIdentify->InputSession(aSession);
}

void IdentifyBuilder::OutputChannels(const std::vector<char>& vChannels)
{
    m_ppnlIdentify->OutputChannels(vChannels);
}

void IdentifyBuilder::SetAudioData(const timedbuffer* pBuffer)
{
    m_ppnlIdentify->SetAudioData(pBuffer);
}

wxString IdentifyBuilder::GetName() const
{
    return wxT("Identify");
}


wxWindow* IdentifyBuilder::CreateTestPanel(wxWindow* pParent)
{
    m_ppnlIdentify = new pnlIdentify(pParent, this);
    return m_ppnlIdentify;
}

void IdentifyBuilder::LoadSettings()
{

}




