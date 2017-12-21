#include "polarscopebuilder.h"
#include <wx/panel.h>
#include "settings.h"
#include "settingevent.h"
#include "polarscopemeter.h"
#include "pnlOptionsMode.h"
#include <wx/log.h>

using namespace std;

PolarScopeBuilder::PolarScopeBuilder() : MonitorPluginBuilder(),
m_pPolarScope(0)
{

    RegisterForSettingsUpdates(wxT("Mode"), this);

    Connect(wxID_ANY, wxEVT_SETTING_CHANGED, (wxObjectEventFunction)&PolarScopeBuilder::OnSettingChanged);



    m_nInputChannels = 1;
    m_nDisplayChannel = 0;
}

PolarScopeBuilder::~PolarScopeBuilder()
{
}

void PolarScopeBuilder::SetAudioData(const timedbuffer* pBuffer)
{
    m_pPolarScope->SetAudioData(pBuffer);
}

wxWindow* PolarScopeBuilder::CreateMonitorPanel(wxWindow* pParent)
{
    m_pPolarScope = new PolarScope(pParent);
    return m_pPolarScope;
}

list<pairOptionPanel_t> PolarScopeBuilder::CreateOptionPanels(wxWindow* pParent)
{
    list<pairOptionPanel_t> lstOptionPanels;


//    m_ppnlRouting = new pnlRouting(pParent, this);

    lstOptionPanels.push_back(make_pair(wxT("Mode"), new pnlOptionsMode(pParent, this)));
//    lstOptionPanels.push_back(make_pair(wxT("Time"), new pnlDisplay(pParent, this)));
//    lstOptionPanels.push_back(make_pair(wxT("Meter"), new pnlMeters(pParent, this)));
//    lstOptionPanels.push_back(make_pair(wxT("Options"), pOptions));
//
    return lstOptionPanels;
}




void PolarScopeBuilder::LoadSettings()
{


}


void PolarScopeBuilder::InputSession(const session& aSession)
{
    m_pPolarScope->SetNumberOfInputChannels(aSession.nChannels);
    m_nInputChannels = aSession.nChannels;
//    m_ppnlRouting->SetNumberOfChannels(aSession.nChannels);
    ClearMeter();
}

void PolarScopeBuilder::OutputChannels(const std::vector<char>& vChannels)
{
}


void PolarScopeBuilder::OnSettingChanged(SettingEvent& event)
{
    if(event.GetKey() == wxT("Mode"))
    {
        m_pPolarScope->SetMode(ReadSetting(wxT("Mode"),0));
    }
//    if(event.GetKey() == wxT("Routing"))
//    {
//        m_nDisplayChannel = ReadSetting(wxT("Routing"),0);
//        m_pPolarScope->SetChannel(m_nDisplayChannel);
//        ClearMeter();
//    }
//    else if(event.GetKey() == wxT("Timeframe"))
//    {
//        m_pPolarScope->SetTimespan(ReadSetting(wxT("Timeframe"),60));
//    }
//    else if(event.GetKey() == wxT("RefreshRate"))
//    {
//        m_pPolarScope->SetRefreshRate(ReadSetting(wxT("RefreshRate"),250));
//    }
//    else if(event.GetKey() == wxT("MeterMode"))
//    {
//        m_nMode = ReadSetting(wxT("MeterMode"), 0);
//        m_pCalculator->SetMode(m_nMode);
//        m_pPolarScope->SetMode(m_nMode);
//
//    }
}


void PolarScopeBuilder::ClearMeter()
{
    m_pPolarScope->ClearMeter();
}
