#include "distortionbuilder.h"
#include "pnlDistortion.h"

DistortionBuilder::DistortionBuilder() : TestPluginBuilder(),
m_ppnlDistortion(0)
{
    RegisterRemoteApiEnum("reset", {{0, ""}, {1, "Reset"}},0);
}

DistortionBuilder::~DistortionBuilder()
{
}

void DistortionBuilder::InputSession(const session& aSession)
{
    m_ppnlDistortion->InputSession(aSession);
}

void DistortionBuilder::OutputChannels(const std::vector<char>& vChannels)
{

}

void DistortionBuilder::SetAudioData(const timedbuffer* pBuffer)
{
    m_ppnlDistortion->SetAudioData(pBuffer);
}

wxString DistortionBuilder::GetName() const
{
    return wxT("Distortion");
}


wxWindow* DistortionBuilder::CreateTestPanel(wxWindow* pParent)
{
    m_ppnlDistortion = new pnlDistortion(pParent, this);
    return m_ppnlDistortion;
}

void DistortionBuilder::LoadSettings()
{

}




