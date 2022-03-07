#include "peakcountbuilder.h"
#include "pnlpeakcount.h"
#include "settingevent.h"

PeakCountBuilder::PeakCountBuilder() : TestPluginBuilder(),
m_ppnlPeakCount(0)
{
    RegisterForSettingsUpdates(this, "limit");
    Bind(wxEVT_SETTING_CHANGED, &PeakCountBuilder::OnSettingChanged, this);

    RegisterRemoteApiRangeDouble("limit", {-90.0,0.0}, -8.0);

}

PeakCountBuilder::~PeakCountBuilder()
{
}

void PeakCountBuilder::OnSettingChanged(SettingEvent& event)
{
    if(event.GetKey() == "limit")
    {
        m_ppnlPeakCount->SetLimit(event.GetValue(-8.0));
    }
}

void PeakCountBuilder::InputSession(const session& aSession)
{
    m_ppnlPeakCount->InputSession(aSession);
}

void PeakCountBuilder::OutputChannels(const std::vector<char>& vChannels)
{

}

void PeakCountBuilder::SetAudioData(const timedbuffer* pBuffer)
{
    m_ppnlPeakCount->SetAudioData(pBuffer);

}

wxString PeakCountBuilder::GetName() const
{
    return wxT("Peak Count");
}


wxWindow* PeakCountBuilder::CreateTestPanel(wxWindow* pParent)
{
    m_ppnlPeakCount = new pnlPeakCount(pParent, this);
    return m_ppnlPeakCount;
}

void PeakCountBuilder::LoadSettings()
{

}




