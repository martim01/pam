#include "fftphasebuilder.h"
#include <wx/panel.h>
#include "settings.h"
#include "settingevent.h"
#include "fftphasemeter.h"
#include "pnlRouting.h"
#include "pnldisplay.h"
#include "pnlwindow.h"
#include "pnloverlap.h"
#include "pnlbins.h"


using namespace std;

fftphaseBuilder::fftphaseBuilder() : MonitorPluginBuilder()
{

    RegisterForSettingsUpdates(this);

    Connect(wxID_ANY, wxEVT_SETTING_CHANGED, (wxObjectEventFunction)&fftphaseBuilder::OnSettingChanged);

    RegisterRemoteApiEnum("Bins", {{512,"46 Hz"},{1024,"23 Hz"},{1536,"16 Hz"},{2048,"12 Hz"}});
    RegisterRemoteApiEnum("Display", {{0,"Graph"},{1,"Stars"},{2,"Phase"}});
    RegisterRemoteApiEnum("Window", {{0,"None"},{1,"Hann"},{2,"Hamming"},{3,"Blackman"},{4,"Kaiser"},{5,"KaiserBessel"}});
    RegisterRemoteApiEnum("Overlap", {{0,"0%"},{25,"25%"},{50,"50%"},{75,"75%"}});
    RegisterRemoteApiEnum("Fall", {{0,"Off"}, {1,"On"}});

    RegisterRemoteApiRangeInt("ChannelX", {0,7});   //@todo this should be dynamic based on sessions
    RegisterRemoteApiRangeInt("ChannelY", {0,7});   //@todo this should be dynamic based on sessions
}

void fftphaseBuilder::SetAudioData(const timedbuffer* pBuffer)
{
	m_pMeter->SetAudioData(pBuffer);
}

wxWindow* fftphaseBuilder::CreateMonitorPanel(wxWindow* pParent)
{
	m_pMeter = new fftphaseMeter(pParent,this);
	return m_pMeter;

}

list<pairOptionPanel_t> fftphaseBuilder::CreateOptionPanels(wxWindow* pParent)
{
    list<pairOptionPanel_t> lstOptionPanels;

    m_ppnlRouting = new pnlRouting(pParent,this);
    pnlDisplay* pDisplay = new pnlDisplay(pParent,this);
    pnlWindow* pWindow = new pnlWindow(pParent,this);
    pnlOverlap* pOverlap = new pnlOverlap(pParent,this);
    pnlBins* pBins = new pnlBins(pParent,this);


    lstOptionPanels.push_back(make_pair(wxT("Routing"), m_ppnlRouting));
    lstOptionPanels.push_back(make_pair(wxT("Display"), pDisplay));
    lstOptionPanels.push_back(make_pair(wxT("Window"), pWindow));
    lstOptionPanels.push_back(make_pair(wxT("Overlap"), pOverlap));
    lstOptionPanels.push_back(make_pair(wxT("Bins"), pBins));

    return lstOptionPanels;
}




void fftphaseBuilder::LoadSettings()
{
	//@todo load any settings
}


void fftphaseBuilder::InputSession(const session& aSession)
{
	m_pMeter->InputSession(aSession);
    if(m_ppnlRouting)
    {
        if(aSession.GetCurrentSubsession() != aSession.lstSubsession.end())
        {
            m_ppnlRouting->SetNumberOfChannels(min((unsigned int)8 ,aSession.GetCurrentSubsession()->nChannels));
        }
        else
        {
            m_ppnlRouting->SetNumberOfChannels(0);
        }
    }
}

void fftphaseBuilder::OutputChannels(const std::vector<char>& vChannels)
{
	m_pMeter->OutputChannels(vChannels);
}


void fftphaseBuilder::OnSettingChanged(SettingEvent& event)
{
    if(event.GetKey() == "Bins")
    {
        m_pMeter->SetNumberOfBins(event.GetValue(1024l));
    }
    else if(event.GetKey() == "Display")
    {
        m_pMeter->SetDisplayType(event.GetValue(1l));
    }
    else if(event.GetKey() == "Window")
    {
        m_pMeter->SetWindowType(event.GetValue(3l));
    }
    else if(event.GetKey() == "Overlap")
    {
        m_pMeter->SetOverlap(event.GetValue(50l));
    }
    else if(event.GetKey() == "Fall")
    {
        m_pMeter->SetFall(event.GetValue(true));
    }
    else if(event.GetKey() == "ChannelX")
    {
        m_pMeter->SetChannelX(event.GetValue(0l));
    }
    else if(event.GetKey() == "ChannelY")
    {
        m_pMeter->SetChannelY(event.GetValue(1l));
    }
}

