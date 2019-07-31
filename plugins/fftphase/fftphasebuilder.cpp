#include "fftphasebuilder.h"
#include <wx/panel.h>
#include "settings.h"
#include "settingevent.h"
#include "version.h"
#include "fftphasemeter.h"
//#include "pnlroutiing.h"
#include "pnldisplay.h"
#include "pnlwindow.h"
#include "pnloverlap.h"
#include "pnlbins.h"
#include "pnloptions.h"


using namespace std;

fftphaseBuilder::fftphaseBuilder() : MonitorPluginBuilder()
{

    //RegisterForSettingsUpdates(wxT("Mode"), this);

    Connect(wxID_ANY, wxEVT_SETTING_CHANGED, (wxObjectEventFunction)&fftphaseBuilder::OnSettingChanged);

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

    //m_ppnlRouting = new pnlRoutiing(pParent,this);
    pnlDisplay* pDisplay = new pnlDisplay(pParent,this);
    pnlWindow* pWindow = new pnlWindow(pParent,this);
    pnlOverlap* pOverlap = new pnlOverlap(pParent,this);
    pnlBins* pBins = new pnlBins(pParent,this);

    //lstOptionPanels.push_back(make_pair(wxT("Routing"), m_ppnlRouting));
    lstOptionPanels.push_back(make_pair(wxT("Display"), pDisplay));
    lstOptionPanels.push_back(make_pair(wxT("Window"), pWindow));
    lstOptionPanels.push_back(make_pair(wxT("Overlap"), pOverlap));
    lstOptionPanels.push_back(make_pair(wxT("Bins"), pBins));


//    m_ppnlRouting->m_plstFFT_Routing->SelectButton(ReadSetting(wxT("Routing"),0), true);
    pDisplay->m_plstFFT_Display->SelectButton(ReadSetting(wxT("Display"), 0),true);
    pWindow->m_plstFFT_Window->SelectButton(ReadSetting(wxT("Window"), 0), true);
    pOverlap->m_plstFFT_Overlap->SelectButton(ReadSetting(wxT("Overlap"), 0), true);
    pBins->m_plstFFT_Bins->SelectButton(ReadSetting(wxT("Bins"),0), true);


    return lstOptionPanels;
}




void fftphaseBuilder::LoadSettings()
{
	//@todo load any settings
}


void fftphaseBuilder::InputSession(const session& aSession)
{
	m_pMeter->InputSession(aSession);
}

void fftphaseBuilder::OutputChannels(const std::vector<char>& vChannels)
{
	m_pMeter->OutputChannels(vChannels);
}


void fftphaseBuilder::OnSettingChanged(SettingEvent& event)
{
    //@todo a setting registered for has changed. Handle it
}



void fftphaseBuilder::OnBinsChanged(wxCommandEvent& event)
{
    m_pMeter->SetNumberOfBins(reinterpret_cast<size_t>(event.GetClientData()));
    WriteSetting(wxT("Bins"), event.GetInt());
}

void fftphaseBuilder::OnDisplayChanged(wxCommandEvent& event)
{
    m_pMeter->SetDisplayType(event.GetInt());
    WriteSetting(wxT("Display"), event.GetInt());
}

void fftphaseBuilder::OnWindowChanged(wxCommandEvent& event)
{
    m_pMeter->SetWindowType(event.GetInt());
    WriteSetting(wxT("Window"), event.GetInt());
}

void fftphaseBuilder::OnOverlapChanged(wxCommandEvent& event)
{
    m_pMeter->SetOverlap(reinterpret_cast<int>(event.GetClientData()));
    WriteSetting(wxT("Overlap"), event.GetInt());
}



void fftphaseBuilder::OnTypeChanged(wxCommandEvent& event)
{
  //  m_pMeter->SetMeter(event.GetInt());
    WriteSetting(wxT("Type"), event.GetInt());
}

void fftphaseBuilder::FallSelected(bool bSelected)
{
    m_pMeter->SetFall(bSelected);
    WriteSetting(wxT("Fall"), bSelected);

}

