#include "fftdiffbuilder.h"
#include <wx/panel.h>
#include "settings.h"
#include "settingevent.h"
#include "version.h"
#include "pnlMain.h"

#include "pnlwindow.h"
#include "pnloverlap.h"
#include "pnlbins.h"
#include "pnloptions.h"
#include "pnlmode.h"
#include "pnldelay.h"
#include "pnlrange.h"



using namespace std;

fftdiffBuilder::fftdiffBuilder() : TestPluginBuilder()
{

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

    RegisterRemoteApiEnum("DelayMode", {{0,"Off"}, {1,"One-Shot"},{2,"Auto"}}, 0);
    RegisterRemoteApiEnum("DelayWindow",{{50, "50ms"},{100, "100ms"}, {250,"250ms"}, {500, "500ms"}, {1000, "1s"}, {2000, "2s"}, {5000, "5s"}, {10000, "10s"}}, 50);
    RegisterRemoteApiEnum("DelayAccuracy",{{1, "1"},{12, "12"}, {24,"24"}, {48, "48"}, {96, "96"}}, 24);
}

void fftdiffBuilder::SetAudioData(const timedbuffer* pBuffer)
{
    m_ppnlMain->SetAudioData(pBuffer);
}

wxWindow* fftdiffBuilder::CreateTestPanel(wxWindow* pParent)
{
    m_ppnlMain = new pnlMain(pParent,this);
	return m_ppnlMain;

}


void fftdiffBuilder::LoadSettings()
{
	//@todo load any settings
}


void fftdiffBuilder::InputSession(const session& aSession)
{
	if(aSession.GetCurrentSubsession() != aSession.lstSubsession.end())
    {
        m_ppnlMain->SetSampleRate(aSession.GetCurrentSubsession()->nSampleRate);
        m_ppnlMain->SetChannels(aSession.GetCurrentSubsession()->vChannels);
        //m_ppnlRouting->SetChannels(aSession.GetCurrentSubsession()->vChannels);
    }
    else
    {
        m_ppnlMain->SetSampleRate(48000);
        m_ppnlMain->SetChannels({});
        //m_ppnlRouting->SetChannels({});
    }
}

void fftdiffBuilder::OutputChannels(const std::vector<char>& vChannels)
{
	//m_pMeter->OutputChannels(vChannels);
}





void fftdiffBuilder::ResetMax()
{
    m_ppnlMain->ResetMax();
}

void fftdiffBuilder::ResetMin()
{
    m_ppnlMain->ResetMin();
}

void fftdiffBuilder::ResetAverage()
{
    m_ppnlMain->ResetAverage();
}

void fftdiffBuilder::CalculateDelay()
{
    m_ppnlMain->CalculateDelay();
}
