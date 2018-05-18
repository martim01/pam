#include "r128builder.h"
#include <wx/panel.h>
#include "settings.h"
#include "settingevent.h"
#include "r128meter.h"
#include "pnlrouting.h"
#include "pnlDisplay.h"
#include "pnlMeters.h"
#include <wx/log.h>
#include "version.h"

using namespace std;

R128Builder::R128Builder() : MonitorPluginBuilder(),
m_pR128(0)
{

    RegisterForSettingsUpdates(wxT("Routing"), this);
    RegisterForSettingsUpdates(wxT("Timeframe"), this);
    RegisterForSettingsUpdates(wxT("RefreshRate"), this);
    RegisterForSettingsUpdates(wxT("MeterMode"), this);

    Connect(wxID_ANY, wxEVT_SETTING_CHANGED, (wxObjectEventFunction)&R128Builder::OnSettingChanged);

    m_pCalculator = new LevelCalculator(0);
    m_nMode = ReadSetting(wxT("MeterMode"), 0);
    m_pCalculator->SetMode(m_nMode);


    m_nInputChannels = 1;
    m_nDisplayChannel = 0;
}

R128Builder::~R128Builder()
{
    delete m_pCalculator;
}

void R128Builder::SetAudioData(const timedbuffer* pBuffer)
{
    if(m_nInputChannels != 0)
    {

    }
}

wxWindow* R128Builder::CreateMonitorPanel(wxWindow* pParent)
{
    m_pR128 = new R128Meter(pParent);
    //m_pRadar->SetTimespan(ReadSetting(wxT("Timeframe"),60));
    //m_pRadar->SetMode(m_nMode);
    return m_pR128;
}

list<pairOptionPanel_t> R128Builder::CreateOptionPanels(wxWindow* pParent)
{
    list<pairOptionPanel_t> lstOptionPanels;

    m_ppnlRouting = new pnlRouting(pParent, this);

    lstOptionPanels.push_back(make_pair(wxT("Routing"), m_ppnlRouting));
    lstOptionPanels.push_back(make_pair(wxT("Time"), new pnlDisplay(pParent, this)));
    lstOptionPanels.push_back(make_pair(wxT("Meter"), new pnlMeters(pParent, this)));
//    lstOptionPanels.push_back(make_pair(wxT("Options"), pOptions));
//
    return lstOptionPanels;
}




void R128Builder::LoadSettings()
{


}


void R128Builder::InputSession(const session& aSession)
{
    m_pCalculator->InputSession(aSession);
    if(aSession.itCurrentSubsession != aSession.lstSubsession.end())
    {
        m_pR128->SetSampleRate(aSession.itCurrentSubsession->nSampleRate);
        m_nInputChannels = aSession.itCurrentSubsession->nChannels;
        m_ppnlRouting->SetNumberOfChannels(aSession.itCurrentSubsession->nChannels);
    }
    else
    {
        m_pR128->SetSampleRate(48000);
        m_nInputChannels = 0;
        m_ppnlRouting->SetNumberOfChannels(0);
    }
    ClearMeter();
}

void R128Builder::OutputChannels(const std::vector<char>& vChannels)
{
}


void R128Builder::OnSettingChanged(SettingEvent& event)
{

}


void R128Builder::ClearMeter()
{
    m_pRadar->ClearMeter();
}



