#include "peaklogbuilder.h"
#include "pnlPeakLog.h"
#include "version.h"

PeakLogBuilder::PeakLogBuilder() : TestPluginBuilder(),
m_ppnlPeakLog(0)
{

}

PeakLogBuilder::~PeakLogBuilder()
{
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
    m_ppnlPeakLog = new pnlPeakLog(pParent);
    return m_ppnlPeakLog;
}

void PeakLogBuilder::LoadSettings()
{

}





