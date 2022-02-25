#include "fftbuilder.h"
#include "fftmeter.h"
#include <wx/panel.h>
#include "wmswitcherpanel.h"
#include "pnlroutiing.h"
#include "pnldisplay.h"
#include "pnlwindow.h"
#include "pnltype.h"
#include "pnloverlap.h"
#include "pnlbins.h"
#include "pnloptions.h"
#include "settings.h"
#include "settingevent.h"

using namespace std;

FFTBuilder::FFTBuilder() : MonitorPluginBuilder(),
m_pMeter(0)
{
    RegisterForSettingsUpdates(this);

    Connect(wxID_ANY, wxEVT_SETTING_CHANGED, (wxObjectEventFunction)&FFTBuilder::OnSettingChanged);

    //Register with remote api
    RegisterRemoteApiEnum("peaks", {{0,"Off"}, {1,"On"}});
    RegisterRemoteApiEnum("Bins", {{512,"46 Hz"},{1024,"23 Hz"},{1536,"16 Hz"},{2048,"12 Hz"}});
    RegisterRemoteApiEnum("Display", {{0,"Graph"},{1,"Lines"},{2,"EQ"}});
    RegisterRemoteApiEnum("Window", {{0,"None"},{1,"Hann"},{2,"Hamming"},{3,"Blackman"},{4,"Kaiser"},{5,"KaiserBessel"}});
    RegisterRemoteApiEnum("Overlap", {{0,"0%%"},{25,"25%%"},{50,"50%%"},{75,"75%%"}});
    RegisterRemoteApiRangeInt("Routing", {0,7});   //@todo this should be dynamic based on sessions
    RegisterRemoteApiEnum("Type", {{0,"FFT"},{1,"1/3 Octave"},{2,"Peaks"}});
    RegisterRemoteApiEnum("Hold", {{0,"Off"}, {1,"On"}});
    RegisterRemoteApiEnum("Cursor", {{0,"Off"}, {1,"On"}});
    RegisterRemoteApiEnum("Colour", {{0,"Off"}, {1,"On"}});
}

void FFTBuilder::SetAudioData(const timedbuffer* pBuffer)
{
    if(m_pMeter)
    {
        m_pMeter->SetData(pBuffer);
    }


}

wxWindow* FFTBuilder::CreateMonitorPanel(wxWindow* pParent)
{
    m_pMeter = new FftMeter(pParent,this,wxNewId(),wxDefaultPosition, wxDefaultSize);

    m_pMeter->ShowPeak((ReadSetting("peaks", 0)==1));
    return m_pMeter;
}

list<pairOptionPanel_t> FFTBuilder::CreateOptionPanels(wxWindow* pParent)
{
    list<pairOptionPanel_t> lstOptionPanels;

    m_ppnlRouting = new pnlRoutiing(pParent,this);
    pnlDisplay* pDisplay = new pnlDisplay(pParent,this);
    pnlWindow* pWindow = new pnlWindow(pParent,this);
    pnlType* pType = new pnlType(pParent,this);
    pnlOverlap* pOverlap = new pnlOverlap(pParent,this);
    pnlBins* pBins = new pnlBins(pParent,this);
    pnlOptions* pOptions = new pnlOptions(pParent,this);

    lstOptionPanels.push_back(make_pair(wxT("Routing"), m_ppnlRouting));
    lstOptionPanels.push_back(make_pair(wxT("Display"), pDisplay));
    lstOptionPanels.push_back(make_pair(wxT("Window"), pWindow));
    lstOptionPanels.push_back(make_pair(wxT("Type"), pType));
    lstOptionPanels.push_back(make_pair(wxT("Overlap"), pOverlap));
    lstOptionPanels.push_back(make_pair(wxT("Bins"), pBins));
    lstOptionPanels.push_back(make_pair(wxT("Options"), pOptions));


    return lstOptionPanels;
}



void FFTBuilder::LoadSettings()
{
    m_pMeter->ShowPeak(ReadSetting("peaks", (int)0));
    m_pMeter->SetNumberOfBins(ReadSetting("Bins", (int)1024));
    m_pMeter->SetDisplayType(ReadSetting("Display", (int)0));
    m_pMeter->SetWindowType(ReadSetting("Window", (int)0));
    m_pMeter->SetOverlap(ReadSetting("Overlap", (int)50));
    m_pMeter->SetAnalyseMode(ReadSetting("Routing", (int)0));
    m_pMeter->SetMeter(ReadSetting("Type", (int)0));
    m_pMeter->SetHold(ReadSetting("Hold", (int)0));
    m_pMeter->SetCursorMode(ReadSetting("Cursor", (int)0));
    m_pMeter->SetColourMode(ReadSetting("Colour", (int)0));
}

void FFTBuilder::InputSession(const session& aSession)
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

}

void FFTBuilder::OutputChannels(const std::vector<char>& vChannels)
{

}


void FFTBuilder::ResetPeaks()
{
    m_pMeter->ResetPeaks();
}

void FFTBuilder::OnSettingChanged(SettingEvent& event)
{
    if(event.GetKey() == "peaks")
    {
        m_pMeter->ShowPeak(event.GetValue(false));
    }
    else if(event.GetKey() == "Bins")
    {
        m_pMeter->SetNumberOfBins(event.GetValue(long(0)));
    }
    else if(event.GetKey() == "Display")
    {
        m_pMeter->SetDisplayType(event.GetValue(long(0)));
    }
    else if(event.GetKey() == "Window")
    {
        m_pMeter->SetWindowType(event.GetValue(long(0)));
    }
    else if(event.GetKey() == "Overlap")
    {
        m_pMeter->SetOverlap(event.GetValue(0.0));
    }
    else if(event.GetKey() == "Routing")
    {
        m_pMeter->SetAnalyseMode(event.GetValue(long(0)));
    }
    else if(event.GetKey() == "Type")
    {
        m_pMeter->SetMeter(event.GetValue(long(0)));
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
}

