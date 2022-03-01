#include "levelsbuilder.h"
#include "pnlLevels.h"

LevelsBuilder::LevelsBuilder() : TestPluginBuilder(),
m_ppnlLevels(0)
{
    RegisterRemoteApiEnum("Monitor", {{0, "Level"}, {1,"Range"}, {2, "Min-Max"}}, 0);
    RegisterRemoteApiRangeDouble("Range", {0.0,50.0}, 40.0);
    RegisterRemoteApiRangeDouble("LevelMin", {-90.0,0.0}, -10.0);
    RegisterRemoteApiRangeDouble("LevelMax", {-90.0,0.0}, -25.0);

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
    m_ppnlLevels = new pnlLevels(pParent, this);
    return m_ppnlLevels;
}

void LevelsBuilder::LoadSettings()
{

}




