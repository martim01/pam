#include "fftdiffbuilder.h"
#include <wx/panel.h>
#include "settings.h"
#include "settingevent.h"
#include "version.h"
#include "fftdiffmeter.h"

#include "pnlwindow.h"
#include "pnloverlap.h"
#include "pnlbins.h"
#include "pnloptions.h"
#include "pnlmode.h"
#include "pnldelay.h"
#include "pnlrange.h"



using namespace std;

fftdiffBuilder::fftdiffBuilder() : MonitorPluginBuilder()
{

    RegisterForSettingsUpdates(this);

    Connect(wxID_ANY, wxEVT_SETTING_CHANGED, (wxObjectEventFunction)&fftdiffBuilder::OnSettingChanged);

    RegisterRemoteApiEnum("peaks", {{0,"Off"}, {1,"On"}}, 0);
    RegisterRemoteApiEnum("troughs", {{0,"Off"}, {1,"On"}}, 0);
    RegisterRemoteApiEnum("Bins", {{512,"46 Hz"},{1024,"23 Hz"},{1536,"16 Hz"},{2048,"12 Hz"}}, 1024);
    RegisterRemoteApiEnum("Window", {{0,"None"},{1,"Hann"},{2,"Hamming"},{3,"Blackman"},{4,"Kaiser"},{5,"KaiserBessel"}}, 4);
    RegisterRemoteApiEnum("Overlap", {{0,"0%"},{25,"25%"},{50,"50%"},{75,"75%"}}, 75);
//    RegisterRemoteApiRangeInt("Routing", {0,7},0);   //@todo this should be dynamic based on sessions
    RegisterRemoteApiEnum("Hold", {{0,"Off"}, {1,"On"}}, 0);
    RegisterRemoteApiEnum("Cursor", {{0,"Off"}, {1,"On"}}, 0);
    RegisterRemoteApiEnum("Colour", {{0,"Off"}, {1,"On"}}, 0);
    RegisterRemoteApiRangeInt("Range", {10,80}, 80);
}

void fftdiffBuilder::SetAudioData(const timedbuffer* pBuffer)
{
	m_pMeter->SetAudioData(pBuffer);
}

wxWindow* fftdiffBuilder::CreateMonitorPanel(wxWindow* pParent)
{
	    m_pMeter = new fftdiffMeter(pParent,this);
	return m_pMeter;

}

list<pairOptionPanel_t> fftdiffBuilder::CreateOptionPanels(wxWindow* pParent)
{
    list<pairOptionPanel_t> lstOptionPanels;

    //m_ppnlRouting = new pnlRoutiing(pParent,this);
    pnlWindow* pWindow = new pnlWindow(pParent,this);
    pnlOverlap* pOverlap = new pnlOverlap(pParent,this);
    pnlBins* pBins = new pnlBins(pParent,this);
    pnlOptions* pOptions = new pnlOptions(pParent,this);
    pnlMode* pMode = new pnlMode(pParent,this);

    //lstOptionPanels.push_back(make_pair(wxT("Routing"), m_ppnlRouting));
    lstOptionPanels.push_back(make_pair(wxT("Window"), pWindow));
    lstOptionPanels.push_back(make_pair(wxT("Overlap"), pOverlap));
    lstOptionPanels.push_back(make_pair(wxT("Bins"), pBins));
    lstOptionPanels.push_back(make_pair(wxT("Display"), pOptions));
    lstOptionPanels.push_back(make_pair(wxT("Mode"), pMode));

    lstOptionPanels.push_back(make_pair(wxT("Delay"), new pnlDelay(pParent, this)));
    lstOptionPanels.push_back(make_pair(wxT("Range"), new pnlRange(pParent, this)));

    return lstOptionPanels;
}




void fftdiffBuilder::LoadSettings()
{
	//@todo load any settings
}


void fftdiffBuilder::InputSession(const session& aSession)
{
	if(aSession.GetCurrentSubsession() != aSession.lstSubsession.end())
    {
        m_pMeter->SetSampleRate(aSession.GetCurrentSubsession()->nSampleRate);
        m_pMeter->SetChannels(aSession.GetCurrentSubsession()->vChannels);
        //m_ppnlRouting->SetChannels(aSession.GetCurrentSubsession()->vChannels);
    }
    else
    {
        m_pMeter->SetSampleRate(48000);
        m_pMeter->SetChannels({});
        //m_ppnlRouting->SetChannels({});
    }
}

void fftdiffBuilder::OutputChannels(const std::vector<char>& vChannels)
{
	//m_pMeter->OutputChannels(vChannels);
}


void fftdiffBuilder::OnSettingChanged(SettingEvent& event)
{
    if(event.GetKey() == "max")
    {
        m_pMeter->ShowMax(event.GetValue(false));
    }
    else if(event.GetKey() == "min")
    {
        m_pMeter->ShowMin(event.GetValue(false));
    }
    else if(event.GetKey() == "average")
    {
        m_pMeter->ShowAverage(event.GetValue(false));
    }
    else if(event.GetKey() == "Bins")
    {
        m_pMeter->SetNumberOfBins(event.GetValue(long(0)));
    }
    else if(event.GetKey() == "Window")
    {
        m_pMeter->SetWindowType(event.GetValue(long(0)));
    }
    else if(event.GetKey() == "Overlap")
    {
        m_pMeter->SetOverlap(event.GetValue(0.0));
    }
    else if(event.GetKey() == "Hold")
    {
        m_pMeter->SetHold(event.GetValue(long(0)));
    }
    else if(event.GetKey() == "Cursor")
    {
        m_pMeter->SetCursorMode(event.GetValue(long(0)));
    }
    else if(event.GetKey() == "Colour")
    {
        m_pMeter->SetColourMode(event.GetValue(long(0)));
    }
    else if(event.GetKey() == "Delay")
    {
        m_pMeter->SetDelayMode(event.GetValue(long(0)));
    }
    else if(event.GetKey() == "Range")
    {
        m_pMeter->SetVerticalRange(event.GetValue(long(0)));
    }
}


void fftdiffBuilder::ResetMax()
{
    m_pMeter->ResetMax();
}

void fftdiffBuilder::ResetMin()
{
    m_pMeter->ResetMin();
}

void fftdiffBuilder::ResetAverage()
{
    m_pMeter->ResetAverage();
}

void fftdiffBuilder::CalculateDelay()
{
    m_pMeter->SetDelayMode(1);
}
