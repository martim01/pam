#include "recordbuilder.h"
#include "pnlRecord.h"

RecordBuilder::RecordBuilder() : TestPluginBuilder(),
m_ppnlRecord(0)
{

}

RecordBuilder::~RecordBuilder()
{
}

void RecordBuilder::InputSession(const session& aSession)
{
    m_ppnlRecord->InputSession(aSession);
}

void RecordBuilder::OutputChannels(const std::vector<char>& vChannels)
{

}

void RecordBuilder::SetAudioData(const timedbuffer* pBuffer)
{
    m_ppnlRecord->SetAudioData(pBuffer);
}

wxString RecordBuilder::GetName() const
{
    return wxT("Record");
}


wxWindow* RecordBuilder::CreateTestPanel(wxWindow* pParent)
{
    m_ppnlRecord = new pnlRecord(pParent);
    return m_ppnlRecord;
}

void RecordBuilder::LoadSettings()
{

}




