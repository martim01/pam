#include "spectogrambuilder.h"
#include "spectogrammeter.h"
#include <wx/panel.h>
#include "settings.h"
#include "pnlroutiing.h"
#include "pnlwindow.h"
#include "pnloverlap.h"
#include "pnlbins.h"
#include "pnlColours.h"
#include "pnlOptions.h"
#include "settings.h"
#include "settingevent.h"

using namespace std;

SpectogramBuilder::SpectogramBuilder() : MonitorPluginBuilder(),
m_pMeter(0)
{
    RegisterForSettingsUpdates(wxT("Routing"), this);
    RegisterForSettingsUpdates(wxT("Window"), this);
    RegisterForSettingsUpdates(wxT("Overlap"), this);
    RegisterForSettingsUpdates(wxT("Bins"), this);
    RegisterForSettingsUpdates(wxT("HeatMap"), this);
    RegisterForSettingsUpdates(wxT("linear"), this);

    Connect(wxID_ANY, wxEVT_SETTING_CHANGED, (wxObjectEventFunction)&SpectogramBuilder::OnSettingChanged);
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


    m_ppnlRouting = new pnlRoutiing(pParent,this);
    pnlWindow* pWindow = new pnlWindow(pParent,this);
    pnlOverlap* pOverlap = new pnlOverlap(pParent,this);
    pnlBins* pBins = new pnlBins(pParent,this);

    lstOptionPanels.push_back(make_pair(wxT("Routing"), m_ppnlRouting));
    lstOptionPanels.push_back(make_pair(wxT("Window"), pWindow));
    lstOptionPanels.push_back(make_pair(wxT("Overlap"), pOverlap));
    lstOptionPanels.push_back(make_pair(wxT("Bins"), pBins));
    lstOptionPanels.push_back(make_pair(wxT("HeatMap"), new pnlColours(pParent, this)));
    lstOptionPanels.push_back(make_pair(wxT("Options"), new pnlOptions(pParent, this)));



    return lstOptionPanels;
}





void SpectogramBuilder::InputSession(const session& aSession)
{
    if(aSession.GetCurrentSubsession() != aSession.lstSubsession.end())
    {
        m_pMeter->SetSampleRate(aSession.GetCurrentSubsession()->nSampleRate);
        m_pMeter->SetNumberOfChannels(min((unsigned int)256 ,aSession.GetCurrentSubsession()->nChannels));
        m_ppnlRouting->SetNumberOfChannels(min((unsigned int)256 ,aSession.GetCurrentSubsession()->nChannels));
    }
    else
    {
        m_pMeter->SetSampleRate(48000);
        m_pMeter->SetNumberOfChannels(0);
        m_ppnlRouting->SetNumberOfChannels(0);
    }


    //@todo if more than 2 channels then let's change our routing buttons...

}

void SpectogramBuilder::OutputChannels(const std::vector<char>& vChannels)
{

}

void SpectogramBuilder::OnSettingChanged(SettingEvent& event)
{
    if(event.GetKey() == wxT("Routing"))
    {
        m_pMeter->SetAnalyseMode(ReadSetting(wxT("Routing"), 0));
    }
    else if(event.GetKey() == wxT("Window"))
    {
        m_pMeter->SetWindowType(ReadSetting(wxT("Window"), 3));
    }
    else if(event.GetKey() == wxT("Overlap"))
    {
        m_pMeter->SetOverlap(ReadSetting(wxT("Overlap"), 50));
    }
    else if(event.GetKey() == wxT("Bins"))
    {
        m_pMeter->SetNumberOfBins(ReadSetting(wxT("Bins"), 1024));
    }
    else if(event.GetKey() == wxT("HeatMap"))
    {
        m_pMeter->SetHeatMap(ReadSetting(wxT("HeatMap"), SpectogramMeter::MAP_FIVE));
    }
    else if(event.GetKey() == wxT("linear"))
    {
        m_pMeter->SetLinear(event.GetValue(false));
    }
}


void SpectogramBuilder::LoadSettings()
{
    if(m_pMeter)
    {
        m_pMeter->SetAnalyseMode(ReadSetting(wxT("Routing"), 0));
        m_pMeter->SetWindowType(ReadSetting(wxT("Window"), 3));
        m_pMeter->SetOverlap(ReadSetting(wxT("Overlap"), 50));
        m_pMeter->SetNumberOfBins(ReadSetting(wxT("Bins"), 1024));
        m_pMeter->SetHeatMap(ReadSetting(wxT("HeatMap"), SpectogramMeter::MAP_FIVE));
    }
}
