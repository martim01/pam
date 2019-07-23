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
#include "version.h"

using namespace std;

FFTBuilder::FFTBuilder() : MonitorPluginBuilder(),
m_pMeter(0)
{

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


    m_ppnlRouting->m_plstFFT_Routing->SelectButton(ReadSetting(wxT("Routing"),0), true);
    pDisplay->m_plstFFT_Display->SelectButton(ReadSetting(wxT("Display"), 0),true);
    pWindow->m_plstFFT_Window->SelectButton(ReadSetting(wxT("Window"), 0), true);
    pType->m_plstFFT_Type->SelectButton(ReadSetting(wxT("Type"), 0), true);
    pOverlap->m_plstFFT_Overlap->SelectButton(ReadSetting(wxT("Overlap"), 0), true);
    pBins->m_plstFFT_Bins->SelectButton(ReadSetting(wxT("Bins"),0), true);
    pOptions->m_pbtnFFT_Hold->ToggleSelection(ReadSetting(wxT("Hold"), 0), true);


    return lstOptionPanels;
}



void FFTBuilder::OnBinsChanged(wxCommandEvent& event)
{
    m_pMeter->SetNumberOfBins(reinterpret_cast<size_t>(event.GetClientData()));
    WriteSetting(wxT("Bins"), event.GetInt());
}

void FFTBuilder::OnDisplayChanged(wxCommandEvent& event)
{
    m_pMeter->SetDisplayType(event.GetInt());
    WriteSetting(wxT("Display"), event.GetInt());
}

void FFTBuilder::OnWindowChanged(wxCommandEvent& event)
{
    m_pMeter->SetWindowType(event.GetInt());
    WriteSetting(wxT("Window"), event.GetInt());
}

void FFTBuilder::OnOverlapChanged(wxCommandEvent& event)
{
    m_pMeter->SetOverlap(reinterpret_cast<int>(event.GetClientData()));
    WriteSetting(wxT("Overlap"), event.GetInt());
}

void FFTBuilder::OnRoutingChanged(wxCommandEvent& event)
{
    m_pMeter->SetAnalyseMode((int)event.GetClientData());
    WriteSetting(wxT("Routing"), event.GetInt());
}


void FFTBuilder::OnTypeChanged(wxCommandEvent& event)
{
    m_pMeter->SetMeter(event.GetInt());
    WriteSetting(wxT("Type"), event.GetInt());
}

void FFTBuilder::OnHoldClicked(wxCommandEvent& event)
{
    m_pMeter->SetHold(event.IsChecked());
    WriteSetting(wxT("Hold"), event.IsChecked());
}

void FFTBuilder::OnCursorMode(bool bOn)
{
    m_pMeter->SetCursorMode(bOn);
    WriteSetting(wxT("Cursor"), bOn);
}

void FFTBuilder::ColourSelected(bool bSelected)
{
    WriteSetting(wxT("Colour"), bSelected);
    m_pMeter->SetColourMode(bSelected);
}


void FFTBuilder::LoadSettings()
{


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


    //@todo if more than 2 channels then let's change our routing buttons...

}

void FFTBuilder::OutputChannels(const std::vector<char>& vChannels)
{

}
