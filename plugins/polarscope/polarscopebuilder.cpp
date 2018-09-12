#include "polarscopebuilder.h"
#include <wx/panel.h>
#include "settings.h"
#include "settingevent.h"
#include "polarscopemeter.h"
#include "pnlOptionsMode.h"
#include <wx/log.h>
#include "version.h"


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
    wxLogDebug(wxT("PSB:: SetAuiodData"));
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



    lstOptionPanels.push_back(make_pair(wxT("Mode"), new pnlOptionsMode(pParent, this)));
//
    return lstOptionPanels;
}




void PolarScopeBuilder::LoadSettings()
{


}


void PolarScopeBuilder::InputSession(const session& aSession)
{
    if(aSession.GetCurrentSubsession() != aSession.lstSubsession.end())
    {
        m_nInputChannels = aSession.GetCurrentSubsession()->nChannels;
        m_pPolarScope->SetNumberOfInputChannels(m_nInputChannels);
    }
    else
    {
        m_pPolarScope->SetNumberOfInputChannels(0);
        m_nInputChannels = 0;
    }
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
}


void PolarScopeBuilder::ClearMeter()
{
    m_pPolarScope->ClearMeter();
}


