#include "peaklogbuilder.h"
#include "pnlPeakLog.h"
#include "settingevent.h"

PeakLogBuilder::PeakLogBuilder() : TestPluginBuilder(),
m_ppnlPeakLog(0)
{
    RegisterForSettingsUpdates("Plot", this);

    RegisterRemoteApiCSV("Plot", {"0","1","2","3","4","5","6","7"});

    Bind(wxEVT_SETTING_CHANGED, &PeakLogBuilder::OnSettingChanged, this);
}

PeakLogBuilder::~PeakLogBuilder()
{
}

void PeakLogBuilder::OnSettingChanged(SettingEvent& event)
{
    if(event.GetKey() == "Plot")
    {
        m_ppnlPeakLog->PlotChanged(event.GetValue());
    }
}
void PeakLogBuilder::InputSession(const session& aSession)
{

    m_ppnlPeakLog->InputSession(aSession);
}

void PeakLogBuilder::OutputChannels(const std::vector<char>& vChannels)
{

}

void PeakLogBuilder::SetAudioData(const timedbuffer* pBuffer)
{
    m_ppnlPeakLog->SetAudioData(pBuffer);
}

wxString PeakLogBuilder::GetName() const
{
    return wxT("Peak Log");
}


wxWindow* PeakLogBuilder::CreateTestPanel(wxWindow* pParent)
{
    m_ppnlPeakLog = new pnlPeakLog(pParent, this);
    return m_ppnlPeakLog;
}

void PeakLogBuilder::LoadSettings()
{

}





