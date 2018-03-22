#include "Spectogrambuilder.h"
#include "Spectogrammeter.h"
#include <wx/panel.h>
#include "settings.h"

using namespace std;

SpectogramBuilder::SpectogramBuilder() : MonitorPluginBuilder(),
m_pMeter(0)
{

}

void SpectogramBuilder::SetAudioData(const timedbuffer* pBuffer)
{
    if(m_pMeter)
    {
        m_pMeter->SetData(pBuffer);
    }
}

wxWindow* SpectogramBuilder::CreateMonitorPanel(wxWindow* pParent)
{
    m_pMeter = new SpectogramMeter(pParent,this,wxNewId(),wxDefaultPosition, wxDefaultSize);
    return m_pMeter;
}

list<pairOptionPanel_t> SpectogramBuilder::CreateOptionPanels(wxWindow* pParent)
{
    list<pairOptionPanel_t> lstOptionPanels;

    return lstOptionPanels;
}





void SpectogramBuilder::InputSession(const session& aSession)
{
    if(aSession.itCurrentSubsession != aSession.lstSubsession.end())
    {
        m_pMeter->SetSampleRate(aSession.itCurrentSubsession->nSampleRate);
        m_pMeter->SetNumberOfChannels(aSession.itCurrentSubsession->nChannels);
    }
    else
    {
        m_pMeter->SetSampleRate(48000);
        m_pMeter->SetNumberOfChannels(0);
    }


    //@todo if more than 2 channels then let's change our routing buttons...

}

void SpectogramBuilder::OutputChannels(const std::vector<char>& vChannels)
{

}
