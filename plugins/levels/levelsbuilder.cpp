#include "levelsbuilder.h"
#include "pnlLevels.h"
#include "version.h"

LevelsBuilder::LevelsBuilder() : TestPluginBuilder(),
m_ppnlLevels(0)
{

}

LevelsBuilder::~LevelsBuilder()
{
}

void LevelsBuilder::InputSession(const session& aSession)
{
    m_ppnlLevels->InputSession(aSession);
}

void LevelsBuilder::OutputChannels(const std::vector<char>& vChannels)
{

}

void LevelsBuilder::SetAudioData(const timedbuffer* pBuffer)
{
    m_ppnlLevels->SetAudioData(pBuffer);
}

wxString LevelsBuilder::GetName() const
{
    return wxT("Levels");
}


wxWindow* LevelsBuilder::CreateTestPanel(wxWindow* pParent)
{
    m_ppnlLevels = new pnlLevels(pParent);
    return m_ppnlLevels;
}

void LevelsBuilder::LoadSettings()
{

}




