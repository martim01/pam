#include "pnlSettingsOutput.h"
#include "settings.h"
#include "settingevent.h"
#include "iomanager.h"
#include "dlgmask.h"
#include "soundcardmanager.h"
#include "portaudio.h"
#include <map>
#include "log.h"
//(*InternalHeaders(pnlSettingsOutput)
#include <wx/font.h>
#include <wx/intl.h>
#include <wx/string.h>
//*)

using namespace std;
//(*IdInit(pnlSettingsOutput)
const long pnlSettingsOutput::ID_M_PLBL4 = wxNewId();
const long pnlSettingsOutput::ID_M_PLST3 = wxNewId();
const long pnlSettingsOutput::ID_PANEL9 = wxNewId();
const long pnlSettingsOutput::ID_M_PLST4 = wxNewId();
const long pnlSettingsOutput::ID_M_PLBL5 = wxNewId();
const long pnlSettingsOutput::ID_M_PLST6 = wxNewId();
const long pnlSettingsOutput::ID_M_PLBL3 = wxNewId();
const long pnlSettingsOutput::ID_M_PSLIDER1 = wxNewId();
const long pnlSettingsOutput::ID_M_PLBL10 = wxNewId();
const long pnlSettingsOutput::ID_PANEL10 = wxNewId();
const long pnlSettingsOutput::ID_M_PLBL1 = wxNewId();
const long pnlSettingsOutput::ID_PANEL12 = wxNewId();
const long pnlSettingsOutput::ID_M_PLBL2 = wxNewId();
const long pnlSettingsOutput::ID_M_PLBL8 = wxNewId();
const long pnlSettingsOutput::ID_M_PBTN6 = wxNewId();
const long pnlSettingsOutput::ID_M_PLBL6 = wxNewId();
const long pnlSettingsOutput::ID_M_PEDT3 = wxNewId();
const long pnlSettingsOutput::ID_M_PLBL7 = wxNewId();
const long pnlSettingsOutput::ID_M_PBTN7 = wxNewId();
const long pnlSettingsOutput::ID_M_PEDT2 = wxNewId();
const long pnlSettingsOutput::ID_M_PBTN12 = wxNewId();
const long pnlSettingsOutput::ID_M_PBTN2 = wxNewId();
const long pnlSettingsOutput::ID_M_PKBD2 = wxNewId();
const long pnlSettingsOutput::ID_M_PLBL9 = wxNewId();
const long pnlSettingsOutput::ID_M_PBTN8 = wxNewId();
const long pnlSettingsOutput::ID_M_PBTN9 = wxNewId();
const long pnlSettingsOutput::ID_M_PBTN3 = wxNewId();
const long pnlSettingsOutput::ID_M_PBTN1 = wxNewId();
const long pnlSettingsOutput::ID_M_PLBL12 = wxNewId();
const long pnlSettingsOutput::ID_M_PBTN5 = wxNewId();
const long pnlSettingsOutput::ID_M_PLBL13 = wxNewId();
const long pnlSettingsOutput::ID_M_PBTN10 = wxNewId();
const long pnlSettingsOutput::ID_M_PBTN11 = wxNewId();
const long pnlSettingsOutput::ID_M_PLBL11 = wxNewId();
const long pnlSettingsOutput::ID_M_PBTN4 = wxNewId();
const long pnlSettingsOutput::ID_PANEL11 = wxNewId();
const long pnlSettingsOutput::ID_M_PSWP2 = wxNewId();
//*)

BEGIN_EVENT_TABLE(pnlSettingsOutput,wxPanel)
	//(*EventTable(pnlSettingsOutput)
	//*)
END_EVENT_TABLE()

pnlSettingsOutput::pnlSettingsOutput(wxWindow* parent,wxWindowID id,const wxPoint& pos,const wxSize& size, long nStyle, const wxString sempty)
{
	//(*Initialize(pnlSettingsOutput)
	Create(parent, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL, _T("wxID_ANY"));
	m_pLbl4 = new wmLabel(this, ID_M_PLBL4, _("Audio Output Device"), wxPoint(0,0), wxSize(600,30), 0, _T("ID_M_PLBL4"));
	m_pLbl4->SetBorderState(uiRect::BORDER_NONE);
	m_pLbl4->GetUiRect().SetGradient(0);
	m_pLbl4->SetForegroundColour(wxColour(255,255,255));
	m_pLbl4->SetBackgroundColour(wxColour(0,64,0));
	wxFont m_pLbl4Font(10,wxFONTFAMILY_SWISS,wxFONTSTYLE_NORMAL,wxFONTWEIGHT_BOLD,false,_T("Arial"),wxFONTENCODING_DEFAULT);
	m_pLbl4->SetFont(m_pLbl4Font);
	m_plstDestination = new wmList(this, ID_M_PLST3, wxPoint(0,31), wxSize(600,34), wmList::STYLE_SELECT, 0, wxSize(100,30), 3, wxSize(-1,-1));
	m_plstDestination->SetButtonColour(wxColour(wxT("#400080")));
	m_plstDestination->SetSelectedButtonColour(wxColour(wxT("#FF8000")));
	m_pswpDestination = new wmSwitcherPanel(this, ID_M_PSWP2, wxPoint(0,65), wxSize(600,380), wmSwitcherPanel::STYLE_NOSWIPE|wmSwitcherPanel::STYLE_NOANIMATION, _T("ID_M_PSWP2"));
	m_pswpDestination->SetPageNameStyle(0);
	pnlDisabled = new wxPanel(m_pswpDestination, ID_PANEL9, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL, _T("ID_PANEL9"));
	pnlDisabled->SetBackgroundColour(wxColour(0,0,0));
	pnlSoundcard = new wxPanel(m_pswpDestination, ID_PANEL10, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL, _T("ID_PANEL10"));
	pnlSoundcard->SetBackgroundColour(wxColour(0,0,0));
	m_plstPlayback = new wmList(pnlSoundcard, ID_M_PLST4, wxPoint(0,0), wxSize(600,200), wmList::STYLE_SELECT, 0, wxSize(-1,60), 4, wxSize(5,1));
	m_plstPlayback->Disable();
	m_plstPlayback->SetBackgroundColour(wxColour(0,0,0));
	m_plstPlayback->SetSelectedButtonColour(wxColour(wxT("#008000")));
	m_pLbl5 = new wmLabel(pnlSoundcard, ID_M_PLBL5, _("Latency"), wxPoint(0,250), wxSize(80,30), 0, _T("ID_M_PLBL5"));
	m_pLbl5->SetBorderState(uiRect::BORDER_NONE);
	m_pLbl5->GetUiRect().SetGradient(0);
	m_pLbl5->SetForegroundColour(wxColour(255,255,255));
	m_pLbl5->SetBackgroundColour(wxColour(144,144,144));
	m_plstLatency = new wmList(pnlSoundcard, ID_M_PLST6, wxPoint(80,250), wxSize(520,30), wmList::STYLE_SELECT, 0, wxSize(-1,26), 10, wxSize(5,1));
	m_plstLatency->Disable();
	m_plstLatency->SetBackgroundColour(wxColour(144,144,144));
	m_plstLatency->SetButtonColour(wxColour(wxT("#008040")));
	m_plstLatency->SetSelectedButtonColour(wxColour(wxT("#FF8000")));
	m_pLbl3 = new wmLabel(pnlSoundcard, ID_M_PLBL3, _("Gain"), wxPoint(10,300), wxSize(70,30), 0, _T("ID_M_PLBL3"));
	m_pLbl3->SetBorderState(uiRect::BORDER_NONE);
	m_pLbl3->GetUiRect().SetGradient(0);
	m_pLbl3->SetForegroundColour(wxColour(255,255,255));
	m_pLbl3->SetBackgroundColour(wxColour(0,64,0));
	wxFont m_pLbl3Font(10,wxFONTFAMILY_SWISS,wxFONTSTYLE_NORMAL,wxFONTWEIGHT_BOLD,false,_T("Arial"),wxFONTENCODING_DEFAULT);
	m_pLbl3->SetFont(m_pLbl3Font);
	m_plsliderOutputGain = new wmSlider(pnlSoundcard, ID_M_PSLIDER1, _("Slider"), wxPoint(80,300), wxSize(400,30));
	m_plsliderOutputGain->Init(0,10000,5000);
	m_plblOutputGain = new wmLabel(pnlSoundcard, ID_M_PLBL10, wxEmptyString, wxPoint(480,300), wxSize(70,30), 0, _T("ID_M_PLBL10"));
	m_plblOutputGain->SetBorderState(uiRect::BORDER_NONE);
	m_plblOutputGain->GetUiRect().SetGradient(0);
	m_plblOutputGain->SetForegroundColour(wxColour(0,0,0));
	m_plblOutputGain->SetBackgroundColour(wxColour(255,255,255));
	wxFont m_plblOutputGainFont(10,wxFONTFAMILY_SWISS,wxFONTSTYLE_NORMAL,wxFONTWEIGHT_BOLD,false,_T("Arial"),wxFONTENCODING_DEFAULT);
	m_plblOutputGain->SetFont(m_plblOutputGainFont);
	pnlAoip = new wxPanel(m_pswpDestination, ID_PANEL11, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL, _T("ID_PANEL11"));
	pnlAoip->SetBackgroundColour(wxColour(0,0,0));
	m_pLbl1 = new wmLabel(pnlAoip, ID_M_PLBL1, _("Destination"), wxPoint(0,55), wxSize(100,40), 0, _T("ID_M_PLBL1"));
	m_pLbl1->SetBorderState(uiRect::BORDER_NONE);
	m_pLbl1->GetUiRect().SetGradient(0);
	m_pLbl1->SetForegroundColour(wxColour(255,255,255));
	m_pLbl1->SetBackgroundColour(wxColour(64,0,128));
	m_ppnlAddress = new wmipeditpnl(pnlAoip, ID_PANEL12, wxPoint(100,55), wxSize(200,40), wxTAB_TRAVERSAL, _T("ID_PANEL12"));
	m_ppnlAddress->SetForegroundColour(wxColour(0,0,0));
	m_ppnlAddress->SetBackgroundColour(wxColour(255,255,255));
	m_pLbl2 = new wmLabel(pnlAoip, ID_M_PLBL2, _("RTP Port"), wxPoint(300,55), wxSize(50,40), 0, _T("ID_M_PLBL2"));
	m_pLbl2->SetBorderState(uiRect::BORDER_NONE);
	m_pLbl2->GetUiRect().SetGradient(0);
	m_pLbl2->SetForegroundColour(wxColour(255,255,255));
	m_pLbl2->SetBackgroundColour(wxColour(64,0,128));
	m_pLbl9 = new wmLabel(pnlAoip, ID_M_PLBL8, _("RTSP"), wxPoint(0,10), wxSize(100,40), 0, _T("ID_M_PLBL8"));
	m_pLbl9->SetBorderState(uiRect::BORDER_NONE);
	m_pLbl9->GetUiRect().SetGradient(0);
	m_pLbl9->SetForegroundColour(wxColour(255,255,255));
	m_pLbl9->SetBackgroundColour(wxColour(64,0,128));
	m_pbtnRTSP = new wmButton(pnlAoip, ID_M_PBTN6, _("eth0"), wxPoint(100,10), wxSize(200,40), wmButton::STYLE_NORMAL, wxDefaultValidator, _T("ID_M_PBTN6"));
	m_pbtnRTSP->SetForegroundColour(wxColour(0,0,0));
	m_pbtnRTSP->SetBackgroundColour(wxColour(255,255,255));
	m_pbtnRTSP->SetColourDisabled(wxColour(wxT("#B0B0B0")));
	m_pLbl6 = new wmLabel(pnlAoip, ID_M_PLBL6, _("Port"), wxPoint(300,10), wxSize(50,40), 0, _T("ID_M_PLBL6"));
	m_pLbl6->SetBorderState(uiRect::BORDER_NONE);
	m_pLbl6->GetUiRect().SetGradient(0);
	m_pLbl6->SetForegroundColour(wxColour(255,255,255));
	m_pLbl6->SetBackgroundColour(wxColour(64,0,128));
	m_pedtRTSPPort = new wmEdit(pnlAoip, ID_M_PEDT3, wxEmptyString, wxPoint(350,10), wxSize(100,40), 0, wxDefaultValidator, _T("ID_M_PEDT3"));
	m_pedtRTSPPort->SetValidation(4);
	m_pedtRTSPPort->SetBorderStyle(1,1);
	m_pLbl7 = new wmLabel(pnlAoip, ID_M_PLBL7, _("Packet Time"), wxPoint(0,110), wxSize(100,40), 0, _T("ID_M_PLBL7"));
	m_pLbl7->SetBorderState(uiRect::BORDER_NONE);
	m_pLbl7->GetUiRect().SetGradient(0);
	m_pLbl7->SetForegroundColour(wxColour(255,255,255));
	m_pLbl7->SetBackgroundColour(wxColour(64,0,128));
	m_pbtnPacketTime = new wmButton(pnlAoip, ID_M_PBTN7, _("1 ms"), wxPoint(100,110), wxSize(70,40), wmButton::STYLE_NORMAL, wxDefaultValidator, _T("ID_M_PBTN7"));
	m_pbtnPacketTime->SetForegroundColour(wxColour(0,0,0));
	m_pbtnPacketTime->SetBackgroundColour(wxColour(255,255,255));
	m_pbtnPacketTime->SetColourDisabled(wxColour(wxT("#B0B0B0")));
	m_pedtRTPPort = new wmEdit(pnlAoip, ID_M_PEDT2, wxEmptyString, wxPoint(350,55), wxSize(100,40), 0, wxDefaultValidator, _T("ID_M_PEDT2"));
	m_pedtRTPPort->SetValidation(4);
	m_pedtRTPPort->SetBorderStyle(1,1);
	m_pbtnRTCP = new wmButton(pnlAoip, ID_M_PBTN12, _("RTCP"), wxPoint(455,55), wxSize(135,40), wmButton::STYLE_SELECT, wxDefaultValidator, _T("ID_M_PBTN12"));
	m_pbtnRTCP->SetBackgroundColour(wxColour(0,128,0));
	m_pbtnRTCP->SetToggle(true, wxT("Off"), wxT("On"), 50);
	m_pbtnStats = new wmButton(pnlAoip, ID_M_PBTN2, _("RTCP Stats"), wxPoint(358,255), wxSize(135,35), 0, wxDefaultValidator, _T("ID_M_PBTN2"));
	m_pbtnStats->SetBackgroundColour(wxColour(64,128,128));
	wxFont m_pbtnStatsFont(12,wxFONTFAMILY_SWISS,wxFONTSTYLE_NORMAL,wxFONTWEIGHT_NORMAL,false,_T("Tahoma"),wxFONTENCODING_DEFAULT);
	m_pbtnStats->SetFont(m_pbtnStatsFont);
	m_pkbd = new wmKeyboard(pnlAoip, ID_M_PKBD2, wxPoint(10,174), wxSize(240,200), 5, 0);
	m_pkbd->SetForegroundColour(wxColour(255,255,255));
	wxFont m_pkbdFont(10,wxFONTFAMILY_SWISS,wxFONTSTYLE_NORMAL,wxFONTWEIGHT_BOLD,false,_T("Arial"),wxFONTENCODING_DEFAULT);
	m_pkbd->SetFont(m_pkbdFont);
	m_pLbl10 = new wmLabel(pnlAoip, ID_M_PLBL9, _("Advertise"), wxPoint(260,295), wxSize(330,30), 0, _T("ID_M_PLBL9"));
	m_pLbl10->SetBorderState(uiRect::BORDER_NONE);
	m_pLbl10->GetUiRect().SetGradient(0);
	m_pLbl10->SetForegroundColour(wxColour(255,255,255));
	m_pLbl10->SetBackgroundColour(wxColour(61,120,218));
	wxFont m_pLbl10Font(10,wxFONTFAMILY_SWISS,wxFONTSTYLE_NORMAL,wxFONTWEIGHT_BOLD,false,wxEmptyString,wxFONTENCODING_DEFAULT);
	m_pLbl10->SetFont(m_pLbl10Font);
	m_pbtnDNS = new wmButton(pnlAoip, ID_M_PBTN8, _("mDNS-SD"), wxPoint(260,330), wxSize(160,40), wmButton::STYLE_SELECT, wxDefaultValidator, _T("ID_M_PBTN8"));
	m_pbtnDNS->SetBackgroundColour(wxColour(61,120,218));
	m_pbtnDNS->SetToggle(true, wxT("Off"), wxT("On"), 60);
	m_pbtnSAP = new wmButton(pnlAoip, ID_M_PBTN9, _("SAP"), wxPoint(430,330), wxSize(160,40), wmButton::STYLE_SELECT, wxDefaultValidator, _T("ID_M_PBTN9"));
	m_pbtnSAP->SetBackgroundColour(wxColour(61,120,218));
	m_pbtnSAP->SetToggle(true, wxT("Off"), wxT("On"), 60);
	m_pbtnStream = new wmButton(pnlAoip, ID_M_PBTN3, _("Stream"), wxPoint(260,175), wxSize(330,35), wmButton::STYLE_SELECT, wxDefaultValidator, _T("ID_M_PBTN3"));
	m_pbtnStream->SetBackgroundColour(wxColour(0,128,0));
	m_pbtnStream->SetToggle(true, wxT("On Demand"), wxT("Always On"), 40);
	m_pbtnActive = new wmButton(pnlAoip, ID_M_PBTN1, _("Server"), wxPoint(260,215), wxSize(330,35), wmButton::STYLE_SELECT, wxDefaultValidator, _T("ID_M_PBTN1"));
	m_pbtnActive->SetBackgroundColour(wxColour(0,128,0));
	m_pbtnActive->SetToggle(true, wxT("Inactive"), wxT("Active"), 40);
	m_pLbl11 = new wmLabel(pnlAoip, ID_M_PLBL12, _("Channels"), wxPoint(172,110), wxSize(60,40), 0, _T("ID_M_PLBL12"));
	m_pLbl11->SetBorderState(uiRect::BORDER_NONE);
	m_pLbl11->GetUiRect().SetGradient(0);
	m_pLbl11->SetForegroundColour(wxColour(255,255,255));
	m_pLbl11->SetBackgroundColour(wxColour(64,0,128));
	m_pbtnChannels = new wmButton(pnlAoip, ID_M_PBTN5, _("2"), wxPoint(232,110), wxSize(50,40), wmButton::STYLE_NORMAL, wxDefaultValidator, _T("ID_M_PBTN5"));
	m_pbtnChannels->SetForegroundColour(wxColour(0,0,0));
	m_pbtnChannels->SetBackgroundColour(wxColour(255,255,255));
	m_pbtnChannels->SetColourDisabled(wxColour(wxT("#B0B0B0")));
	m_pLbl12 = new wmLabel(pnlAoip, ID_M_PLBL13, _("Sample Rate"), wxPoint(284,110), wxSize(60,40), 0, _T("ID_M_PLBL13"));
	m_pLbl12->SetBorderState(uiRect::BORDER_NONE);
	m_pLbl12->GetUiRect().SetGradient(0);
	m_pLbl12->SetForegroundColour(wxColour(255,255,255));
	m_pLbl12->SetBackgroundColour(wxColour(64,0,128));
	m_pbtnSampleRate = new wmButton(pnlAoip, ID_M_PBTN10, _("48 kHz"), wxPoint(344,110), wxSize(70,40), wmButton::STYLE_NORMAL, wxDefaultValidator, _T("ID_M_PBTN10"));
    m_pbtnSampleRate->SetForegroundColour(wxColour(0,0,0));
	m_pbtnSampleRate->SetBackgroundColour(wxColour(255,255,255));
	m_pbtnSampleRate->SetColourDisabled(wxColour(wxT("#B0B0B0")));
	m_pbtnBits = new wmButton(pnlAoip, ID_M_PBTN11, _("Bits"), wxPoint(416,110), wxSize(174,40), wmButton::STYLE_SELECT, wxDefaultValidator, _T("ID_M_PBTN11"));

	m_pbtnBits->SetBackgroundColour(wxColour(0,128,0));
	m_pbtnBits->SetColourDisabled(wxColour(wxT("#606060")));
	m_pbtnBits->SetToggle(true, wxT("16"), wxT("24"), 50);
	m_pLbl8 = new wmLabel(pnlAoip, ID_M_PLBL11, _("RTP Payload"), wxPoint(455,10), wxSize(70,40), 0, _T("ID_M_PLBL11"));
	m_pLbl8->SetBorderState(uiRect::BORDER_NONE);
	m_pLbl8->GetUiRect().SetGradient(0);
	m_pLbl8->SetForegroundColour(wxColour(255,255,255));
	m_pLbl8->SetBackgroundColour(wxColour(64,0,128));
	m_pbtnRtpMap = new wmButton(pnlAoip, ID_M_PBTN4, _("96"), wxPoint(525,10), wxSize(65,40), wmButton::STYLE_NORMAL, wxDefaultValidator, _T("ID_M_PBTN4"));
	m_pbtnRtpMap->SetForegroundColour(wxColour(0,0,0));
	m_pbtnRtpMap->SetBackgroundColour(wxColour(255,255,255));
	m_pbtnRtpMap->SetColourDisabled(wxColour(wxT("#B0B0B0")));
	m_pswpDestination->AddPage(pnlDisabled, _("Disabled"), false);
	m_pswpDestination->AddPage(pnlSoundcard, _("Soundcard"), false);
	m_pswpDestination->AddPage(pnlAoip, _("AoIP"), false);

	Connect(ID_M_PLST3,wxEVT_LIST_SELECTED,(wxObjectEventFunction)&pnlSettingsOutput::OnlstDestinationSelected);
	Connect(ID_M_PLST4,wxEVT_LIST_SELECTED,(wxObjectEventFunction)&pnlSettingsOutput::OnlstPlaybackSelected);
	Connect(ID_M_PLST6,wxEVT_LIST_SELECTED,(wxObjectEventFunction)&pnlSettingsOutput::OnlstLatencySelected);
	Connect(ID_M_PSLIDER1,wxEVT_SLIDER_MOVE,(wxObjectEventFunction)&pnlSettingsOutput::OnlsliderOutputGainMove);
	Connect(ID_M_PBTN6,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&pnlSettingsOutput::OnbtnRTSPClick);
	Connect(ID_M_PEDT3,wxEVT_COMMAND_TEXT_UPDATED,(wxObjectEventFunction)&pnlSettingsOutput::OnedtRTSPPortText);
	Connect(ID_M_PBTN7,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&pnlSettingsOutput::OnbtnPacketTimeClick);
	Connect(ID_M_PEDT2,wxEVT_COMMAND_TEXT_UPDATED,(wxObjectEventFunction)&pnlSettingsOutput::OnedtRTPPortText);
	Connect(ID_M_PBTN12,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&pnlSettingsOutput::OnbtnRTCPClick);
	Connect(ID_M_PBTN2,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&pnlSettingsOutput::OnbtnStatsClick);
	Connect(ID_M_PBTN8,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&pnlSettingsOutput::OnbtnDNSClick);
	Connect(ID_M_PBTN9,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&pnlSettingsOutput::OnbtnSAPClick);
	Connect(ID_M_PBTN3,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&pnlSettingsOutput::OnbtnStreamClick);
	Connect(ID_M_PBTN1,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&pnlSettingsOutput::OnbtnActiveClick);
	Connect(ID_M_PBTN5,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&pnlSettingsOutput::OnbtnChannelsClick);
	Connect(ID_M_PBTN10,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&pnlSettingsOutput::OnbtnSampleRateClick);
	Connect(ID_M_PBTN11,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&pnlSettingsOutput::OnbtnBitsClick);
	Connect(ID_M_PBTN4,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&pnlSettingsOutput::OnbtnRtpMapClick);
	//*)

	SetSize(size);
	SetPosition(pos);

    m_pbtnBits->SetColourDisabled(wxColour(150,150,150));
    m_pbtnSampleRate->SetColourDisabled(wxColour(150,150,150));
    m_pbtnStream->SetColourDisabled(wxColour(150,150,150));
    m_pbtnRTCP->SetColourDisabled(wxColour(150,150,150));
    m_pbtnDNS->SetColourDisabled(wxColour(150,150,150));
    m_pbtnSAP->SetColourDisabled(wxColour(150,150,150));

	m_plstDestination->AddButton(wxT("Disabled"));
    m_plstDestination->AddButton(wxT("Soundcard"));
    m_plstDestination->AddButton(wxT("AoIP"));

    for(unsigned int i = 0; i < 10; i++)
    {
        m_plstLatency->AddButton(wxString::Format(wxT("%u ms"), i*40));
    }


    ShowSoundcardOutputs();
    UpdateDisplayedSettings();

    //m_pbtnBits->Enable(false);
    m_pbtnSampleRate->Enable(false);

    m_plstPlayback->Disable();
    //m_pbtnStream->ToggleSelection(false);

    Settings::Get().AddHandler(wxT("Server"), wxT("RTP_Port"), this);
    Settings::Get().AddHandler(wxT("Server"), wxT("RTSP_Port"), this);
    Settings::Get().AddHandler(wxT("Server"), wxT("DestinationIp"), this);
    Settings::Get().AddHandler(wxT("Server"), wxT("Latency"), this);
    Settings::Get().AddHandler(wxT("Server"), wxT("RTSP_Interface"), this);
    Settings::Get().AddHandler(wxT("Server"), wxT("Stream"), this);
    Settings::Get().AddHandler(wxT("Server"), wxT("SAP"), this);
    Settings::Get().AddHandler(wxT("Server"), wxT("DNS-SD"), this);
    Settings::Get().AddHandler(wxT("Server"), wxT("RTPMap"), this);
    Settings::Get().AddHandler(wxT("Server"), wxT("Channels"), this);
    Settings::Get().AddHandler(wxT("Server"), wxT("RTCP"), this);
    Settings::Get().AddHandler(wxT("Server"), wxT("State"), this);

    Connect(wxID_ANY, wxEVT_SETTING_CHANGED, (wxObjectEventFunction)&pnlSettingsOutput::OnSettingChanged);
}

pnlSettingsOutput::~pnlSettingsOutput()
{
	//(*Destroy(pnlSettingsOutput)
	//*)
}


void pnlSettingsOutput::UpdateDisplayedSettings()
{
    m_plstDestination->SelectButton(Settings::Get().Read(wxT("Output"), wxT("Destination"), wxT("Disabled")), false);

    int nTime = Settings::Get().Read("Server", "PacketTime", 1000);
    if(nTime > 999)
    {
        m_pbtnPacketTime->SetLabel(wxString::Format("%d ms", nTime/1000));
    }
    else
    {
        m_pbtnPacketTime->SetLabel(wxString::Format(L"%d \u00B5s", nTime));
    }


    m_pswpDestination->ChangeSelection(Settings::Get().Read(wxT("Output"), wxT("Destination"), wxT("Disabled")));

    m_pedtRTPPort->SetValue(Settings::Get().Read(wxT("Server"), wxT("RTP_Port"), wxT("5004")));
    m_pedtRTSPPort->SetValue(Settings::Get().Read(wxT("Server"), wxT("RTSP_Port"), wxT("5555")));


    m_ppnlAddress->SetValue(Settings::Get().Read(wxT("Server"), wxT("DestinationIp"), IOManager::Get().GetRandomMulticastAddress()));
    m_plstLatency->SelectButton(Settings::Get().Read(wxT("Output"), wxT("Latency"), 0)/40, false);
    m_pbtnRTSP->SetLabel(Settings::Get().Read(wxT("Server"), wxT("RTSP_Interface"), wxEmptyString));

    bool bMulticast(Settings::Get().Read(wxT("Server"), wxT("Stream"), "OnDemand") == "AlwaysOn");
    m_pbtnStream->ToggleSelection(bMulticast);

    m_pbtnRtpMap->SetLabel(Settings::Get().Read("Server", "RTPMap", "96"));
    m_pbtnChannels->SetLabel(Settings::Get().Read("Server", "Channels", "2"));

    m_pbtnSAP->Show(bMulticast);
    m_pLbl1->Show(bMulticast);
    m_ppnlAddress->Show(bMulticast);

    m_pbtnDNS->ToggleSelection(Settings::Get().Read("Server", "DNS-SD", 0), true);
    m_pbtnSAP->ToggleSelection(Settings::Get().Read("Server", "SAP", 0), true);
    m_pbtnRTCP->ToggleSelection(Settings::Get().Read("Server", "RTCP", 0), true);

    m_pbtnStats->Show(m_pbtnRTCP->IsChecked());

    double dGain = ConvertRatioToGain(Settings::Get().Read("Output", "Ratio_00", 1.0));
    m_plblOutputGain->SetLabel(wxString::Format("%.2f dB", dGain));
    m_plsliderOutputGain->SetSliderPosition(dGain*500+5000, false);

    m_pbtnBits->ToggleSelection(Settings::Get().Read("Server", "Bits", 24) == 24);
    m_pbtnSampleRate->SetLabel(Settings::Get().Read("Server", "SampleRate", "48000"));

    m_pbtnActive->ToggleSelection(Settings::Get().Read("Server", "State", 0) == 1);
    EnableStreamSettings();
}


void pnlSettingsOutput::OnSettingChanged(SettingEvent& event)
{
    if(event.GetSection() == wxT("Server"))
    {
        if(event.GetKey() == wxT("RTP_Port"))
        {
            m_pedtRTPPort->SetValue(Settings::Get().Read(wxT("Server"), wxT("RTP_Port"), wxT("5004")));
        }
        else if(event.GetKey() == wxT("RTSP_Port"))
        {
            m_pedtRTSPPort->SetValue(Settings::Get().Read(wxT("Server"), wxT("RTSP_Port"), wxT("5555")));
        }
        else if(event.GetKey() == wxT("DestinationIp"))
        {
            m_ppnlAddress->SetValue(Settings::Get().Read(wxT("Server"), wxT("DestinationIp"), IOManager::Get().GetRandomMulticastAddress()));
        }
        else if(event.GetKey() == wxT("Latency"))
        {
            m_plstLatency->SelectButton(Settings::Get().Read(wxT("Output"), wxT("Latency"), 0)/40, false);
        }
        else if(event.GetKey() == wxT("RTSP_Interface"))
        {
            m_pbtnRTSP->SetLabel(Settings::Get().Read(wxT("Server"), wxT("RTSP_Interface"), wxEmptyString));
        }
        else if(event.GetKey() == wxT("Stream"))
        {
            bool bMulticast(Settings::Get().Read(wxT("Server"), wxT("Stream"), "OnDemand") == "AlwaysOn");
            m_pbtnStream->ToggleSelection(bMulticast);
            m_pbtnSAP->Show(bMulticast);
            m_pLbl1->Show(bMulticast);
            m_ppnlAddress->Show(bMulticast);
        }
        else if(event.GetKey() == wxT("DNS-SD"))
        {
            m_pbtnDNS->ToggleSelection(Settings::Get().Read("Server", "DNS-SD", 0));
        }
        else if(event.GetKey() == "SAP")
        {
            m_pbtnSAP->ToggleSelection(Settings::Get().Read("Server", "SAP", 0));
        }
        else if(event.GetKey() == "RTPMap")
        {
            m_pbtnRtpMap->SetLabel(event.GetValue());
        }
        else if(event.GetKey() == "Channels")
        {
            m_pbtnChannels->SetLabel(event.GetValue());
        }
        else if(event.GetKey() == "RTCP")
        {
            m_pbtnStats->Show(event.GetValue(false));
        }
        else if(event.GetKey() == "State")
        {
            m_pbtnActive->ToggleSelection(event.GetValue(false), false);
            EnableStreamSettings();
        }
    }
}

void pnlSettingsOutput::OnlstDestinationSelected(wxCommandEvent& event)
{
    Settings::Get().Write(wxT("Output"), wxT("Destination"), event.GetString());
    m_plstPlayback->Enable(event.GetString()!=wxT("Disabled"));
    m_pswpDestination->ChangeSelection(event.GetString());
}

void pnlSettingsOutput::OnlstPlaybackSelected(wxCommandEvent& event)
{
    Settings::Get().Write(wxT("Output"), wxT("Device"), (int)event.GetClientData());
}

void pnlSettingsOutput::OnbtnStreamClick(wxCommandEvent& event)
{
    m_pLbl1->Show(event.IsChecked());
    m_ppnlAddress->Show(event.IsChecked());

    if(event.IsChecked() == false)
    {
        Settings::Get().Write(wxT("Server"), wxT("Stream"), "OnDemand");
    }
    else
    {
        //@todo create the new session
        Settings::Get().Write(wxT("Server"), wxT("DestinationIp"), m_ppnlAddress->GetValue());
        Settings::Get().Write(wxT("Server"), wxT("Stream"), "AlwaysOn");
    }
}

void pnlSettingsOutput::OnlstPacketSelected(wxCommandEvent& event)
{
    Settings::Get().Write(wxT("Server"), wxT("PacketTime"), reinterpret_cast<int>(event.GetClientData()));
}

void pnlSettingsOutput::OnedtRTSPPortText(wxCommandEvent& event)
{
    Settings::Get().Write(wxT("Server"), wxT("RTSP_Port"), m_pedtRTSPPort->GetValue());
}

void pnlSettingsOutput::OnedtRTPPortText(wxCommandEvent& event)
{
    Settings::Get().Write(wxT("Server"), wxT("RTP_Port"), m_pedtRTPPort->GetValue());
}

void pnlSettingsOutput::OnbtnRTSPClick(wxCommandEvent& event)
{
    wxArrayString asButtons;

    for(multimap<wxString, wxString>::const_iterator itInterface = Settings::Get().GetInterfaces().begin(); itInterface != Settings::Get().GetInterfaces().end(); ++itInterface)
    {
        asButtons.Add(itInterface->first);
    }


    dlgMask aDlg(this, asButtons, m_pbtnRTSP->GetLabel(), wxNewId(), ClientToScreen(m_pbtnRTSP->GetPosition()), m_pbtnRTSP->GetSize());
    if(aDlg.ShowModal()== wxID_OK)
    {
        m_pbtnRTSP->SetLabel(aDlg.m_sSelected);
        Settings::Get().Write(wxT("Server"), wxT("RTSP_Interface"), Settings::Get().GetInterfaces().lower_bound(aDlg.m_sSelected)->first);
        Settings::Get().Write(wxT("Server"), wxT("RTSP_Address"), Settings::Get().GetInterfaces().lower_bound(aDlg.m_sSelected)->second);

    }
}

void pnlSettingsOutput::OnbtnSAPClick(wxCommandEvent& event)
{
    Settings::Get().Write("Server", "SAP", event.IsChecked());
}

void pnlSettingsOutput::OnbtnDNSClick(wxCommandEvent& event)
{
    Settings::Get().Write("Server", "DNS-SD", event.IsChecked());
}

void pnlSettingsOutput::ShowSoundcardOutputs()
{
    m_plstPlayback->Clear();

    int nDevices =  SoundcardManager::Get().GetNumberOfDevices();
    for(int i = 0; i < nDevices; i++)
    {
        const PaDeviceInfo* pInfo = SoundcardManager::Get().GetDeviceInfo(i);
        if(pInfo && pInfo->maxOutputChannels > 0)
        {
            short nEnabled = wmList::wmENABLED;
            #ifdef __WXGTK__
            if(Settings::Get().Read(wxT("Input"), wxT("Type"), wxT("Soundcard")) == wxT("Soundcard") && Settings::Get().Read(wxT("Input"), wxT("Device"),0) == i)
            {
                nEnabled = wmList::wmDISABLED;
            }
            #endif // __WXGTK__
            m_plstPlayback->AddButton(wxString::Format(wxT("[%d]%s [%d]"), i, wxString::FromUTF8(pInfo->name).c_str(), pInfo->maxOutputChannels), wxNullBitmap, (void*)i, nEnabled);
        }
    }

    unsigned int nOutput(Settings::Get().Read(wxT("Output"), wxT("Device"), (int)Pa_GetDefaultOutputDevice()));
    int nDevice = m_plstPlayback->FindButton(reinterpret_cast<void*>(nOutput));
    if(nDevice == wmList::NOT_FOUND)
    {
        nDevice = 0;
    }
    m_plstPlayback->SelectButton(nDevice);
}


void pnlSettingsOutput::OnlstLatencySelected(wxCommandEvent& event)
{
    Settings::Get().Write(wxT("Output"), wxT("Latency"), event.GetInt()*40);
}

void pnlSettingsOutput::OnlsliderOutputGainMove(wxCommandEvent& event)
{
    double dGain = (m_plsliderOutputGain->GetPosition()-5000)/500.0;

    m_plblOutputGain->SetLabel(wxString::Format("%.2f dB", dGain));

    double dRatio = ConvertGainToRatio(dGain);
    for(unsigned int i = 0; i < 8; i++)
    {
        Settings::Get().Write("Output", wxString::Format("Ratio_%02d", i), dRatio);
    }
}


double pnlSettingsOutput::ConvertGainToRatio(double dGain)
{
    return pow(10.0, dGain/20.0);
}

double pnlSettingsOutput::ConvertRatioToGain(double dRatio)
{
    return 20*log10(dRatio);
}

void pnlSettingsOutput::OnbtnRestartStreamHeld(wxCommandEvent& event)
{
}

void pnlSettingsOutput::OnbtnStatsClick(wxCommandEvent& event)
{
    Settings::Get().Write("Splash", "Screen", "RTCP");
}

void pnlSettingsOutput::OnbtnRtpMapClick(wxCommandEvent& event)
{
    wxArrayString asButtons;
    for(int i = 96; i < 128; i++)
    {
        asButtons.Add(wxString::Format("%d", i));
    }

    dlgMask aDlg(this, asButtons, m_pbtnRtpMap->GetLabel(), wxNewId(), ClientToScreen(m_pbtnRtpMap->GetPosition()), m_pbtnRtpMap->GetSize());
    if(aDlg.ShowModal()== wxID_OK)
    {
        m_pbtnRtpMap->SetLabel(aDlg.m_sSelected);
pmlLog(pml::LOG_TRACE) << "RTPMAP: " << aDlg.m_sSelected.ToStdString();
        Settings::Get().Write("Server", "RTPMap", aDlg.m_sSelected);
    }
}

void pnlSettingsOutput::OnbtnChannelsClick(wxCommandEvent& event)
{
    wxArrayString asButtons;
    for(int i = 1; i < 9; i++)
    {
        asButtons.Add(wxString::Format("%d", i));
    }

    dlgMask aDlg(this, asButtons, m_pbtnChannels->GetLabel(), wxNewId(), ClientToScreen(m_pbtnChannels->GetPosition()), m_pbtnChannels->GetSize());
    if(aDlg.ShowModal()== wxID_OK)
    {
        m_pbtnChannels->SetLabel(aDlg.m_sSelected);
        Settings::Get().Write("Server", "Channels", aDlg.m_sSelected);
    }
}

void pnlSettingsOutput::OnbtnPacketTimeClick(wxCommandEvent& event)
{
    wxArrayString asButtons;
    asButtons.Add(L"125 \u00B5s");
    asButtons.Add(L"250 \u00B5s");
    asButtons.Add(L"333 \u00B5s");
    asButtons.Add("1 ms");
    asButtons.Add("4 ms");


    dlgMask aDlg(this, asButtons, m_pbtnPacketTime->GetLabel(), wxNewId(), ClientToScreen(m_pbtnPacketTime->GetPosition()), m_pbtnPacketTime->GetSize());
    if(aDlg.ShowModal()== wxID_OK)
    {
        m_pbtnPacketTime->SetLabel(aDlg.m_sSelected);
        unsigned long nTime;
        aDlg.m_sSelected.Before(' ').ToULong(&nTime);
        if(nTime == 1 || nTime == 4)
        {
            nTime*=1000;
        }
        Settings::Get().Write("Server", "PacketTime", (int)nTime);
    }
}

void pnlSettingsOutput::OnbtnSampleRateClick(wxCommandEvent& event)
{
    wxArrayString asButtons;
    asButtons.Add("44100");
    asButtons.Add("48000");
    asButtons.Add("96000");

    dlgMask aDlg(this, asButtons, m_pbtnSampleRate->GetLabel(), wxNewId(), ClientToScreen(m_pbtnSampleRate->GetPosition()), m_pbtnSampleRate->GetSize());
    if(aDlg.ShowModal()== wxID_OK)
    {
        m_pbtnSampleRate->SetLabel(aDlg.m_sSelected);
        unsigned long nSampleRate;
        aDlg.m_sSelected.ToULong(&nSampleRate);
        Settings::Get().Write("Server", "SampleRate", (int)nSampleRate);
    }
}

void pnlSettingsOutput::OnbtnBitsClick(wxCommandEvent& event)
{
    Settings::Get().Write("Server", "Bits", event.IsChecked() ? 24 : 16);
}

void pnlSettingsOutput::OnbtnRTCPClick(wxCommandEvent& event)
{
    Settings::Get().Write("Server", "RTCP", event.IsChecked() ? 1 : 0);
}

void pnlSettingsOutput::OnbtnActiveClick(wxCommandEvent& event)
{
    Settings::Get().Write("Server", "State", event.IsChecked() ? "1" : "0");
    EnableStreamSettings();
}

void pnlSettingsOutput::EnableStreamSettings()
{
    m_ppnlAddress->Enable(!m_pbtnActive->IsChecked());
    m_pbtnRTSP->Enable(!m_pbtnActive->IsChecked());
    m_pedtRTSPPort->Enable(!m_pbtnActive->IsChecked());
    m_pbtnPacketTime->Enable(!m_pbtnActive->IsChecked());
    m_pedtRTPPort->Enable(!m_pbtnActive->IsChecked());
    m_pbtnRTCP->Enable(!m_pbtnActive->IsChecked());
    m_pbtnStats->Enable(m_pbtnActive->IsChecked());
    m_pbtnDNS->Enable(!m_pbtnActive->IsChecked());
    m_pbtnSAP->Enable(!m_pbtnActive->IsChecked());
    m_pbtnChannels->Enable(!m_pbtnActive->IsChecked());
    m_pbtnRtpMap->Enable(!m_pbtnActive->IsChecked());
    m_pbtnStream->Enable(!m_pbtnActive->IsChecked());

    //m_pbtnSampleRate->Enable(!m_pbtnActive->IsChecked());
    m_pbtnBits->Enable(!m_pbtnActive->IsChecked());
}
