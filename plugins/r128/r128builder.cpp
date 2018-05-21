#include "r128builder.h"
#include <wx/panel.h>
#include "settings.h"
#include "settingevent.h"
#include "pnlEbuMeter.h"
#include <wx/log.h>
#include "version.h"

using namespace std;

R128Builder::R128Builder() : MonitorPluginBuilder(),
m_pMeters(0)
{


    Connect(wxID_ANY, wxEVT_SETTING_CHANGED, (wxObjectEventFunction)&R128Builder::OnSettingChanged);



    m_nInputChannels = 1;
    m_nDisplayChannel = 0;
}

R128Builder::~R128Builder()
{
}

void R128Builder::SetAudioData(const timedbuffer* pBuffer)
{
    if(m_pMeters)
    {
        m_pMeters->SetAudioData(pBuffer);
    }
}

wxWindow* R128Builder::CreateMonitorPanel(wxWindow* pParent)
{
    m_pMeters = new pnlEbuMeter(pParent,this);
    //m_pRadar->SetTimespan(ReadSetting(wxT("Timeframe"),60));
    //m_pRadar->SetMode(m_nMode);
    return m_pMeters;
}

list<pairOptionPanel_t> R128Builder::CreateOptionPanels(wxWindow* pParent)
{
    list<pairOptionPanel_t> lstOptionPanels;


   // lstOptionPanels.push_back(make_pair(wxT("Routing"), m_ppnlRouting));
   // lstOptionPanels.push_back(make_pair(wxT("Time"), new pnlDisplay(pParent, this)));
   // lstOptionPanels.push_back(make_pair(wxT("Meter"), new pnlMeters(pParent, this)));
//    lstOptionPanels.push_back(make_pair(wxT("Options"), pOptions));
//
    return lstOptionPanels;
}




void R128Builder::LoadSettings()
{


}


void R128Builder::InputSession(const session& aSession)
{
    m_pMeters->SetSession(aSession);

}

void R128Builder::OutputChannels(const std::vector<char>& vChannels)
{
}


void R128Builder::OnSettingChanged(SettingEvent& event)
{

}


void R128Builder::ClearMeter()
{
    m_pMeters->ClearMeters();
}



