#include "pnlAoipManual.h"
#include "settings.h"
#include "dlgmask.h"
#include "aoipsourcemanager.h"
#include "settingevent.h"
#include "dlgEditName.h"

//(*InternalHeaders(pnlAoipManual)
#include <wx/font.h>
#include <wx/intl.h>
#include <wx/settings.h>
#include <wx/string.h>
//*)

//(*IdInit(pnlAoipManual)
const long pnlAoipManual::ID_M_PKBD2 = wxNewId();
const long pnlAoipManual::ID_M_PBTN3 = wxNewId();
const long pnlAoipManual::ID_M_PBTN5 = wxNewId();
const long pnlAoipManual::ID_M_PLST1 = wxNewId();
const long pnlAoipManual::ID_M_PLBL8 = wxNewId();
const long pnlAoipManual::ID_M_PIP1 = wxNewId();
const long pnlAoipManual::ID_M_PLBL1 = wxNewId();
const long pnlAoipManual::ID_M_PEDT3 = wxNewId();
const long pnlAoipManual::ID_M_PLBL2 = wxNewId();
const long pnlAoipManual::ID_M_PBTN6 = wxNewId();
const long pnlAoipManual::ID_M_PBTN1 = wxNewId();
const long pnlAoipManual::ID_M_PLBL4 = wxNewId();
const long pnlAoipManual::ID_M_PBTN2 = wxNewId();
const long pnlAoipManual::ID_M_PBTN13 = wxNewId();
const long pnlAoipManual::ID_M_PLBL11 = wxNewId();
const long pnlAoipManual::ID_M_PBTN4 = wxNewId();
const long pnlAoipManual::ID_PANEL2 = wxNewId();
const long pnlAoipManual::ID_M_PLBL3 = wxNewId();
const long pnlAoipManual::ID_M_PEDT1 = wxNewId();
const long pnlAoipManual::ID_PANEL3 = wxNewId();
const long pnlAoipManual::ID_M_PSWP2 = wxNewId();
const long pnlAoipManual::ID_PANEL1 = wxNewId();
const long pnlAoipManual::ID_CHANNEL_MAPPING = wxNewId();
const long pnlAoipManual::ID_M_PSWP1 = wxNewId();
//*)

BEGIN_EVENT_TABLE(pnlAoipManual,wxPanel)
	//(*EventTable(pnlAoipManual)
	//*)
END_EVENT_TABLE()

pnlAoipManual::pnlAoipManual(wxWindow* parent,wxWindowID id,const wxPoint& pos,const wxSize& size, long nStyle, const wxString& sId)
{
	//(*Initialize(pnlAoipManual)
	Create(parent, id, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL, _T("id"));
	SetBackgroundColour(wxColour(0,0,0));
	m_pswpMain = new wmSwitcherPanel(this, ID_M_PSWP1, wxDefaultPosition, wxSize(600,480), wmSwitcherPanel::STYLE_NOSWIPE|wmSwitcherPanel::STYLE_NOANIMATION, _T("ID_M_PSWP1"));
	m_pswpMain->SetPageNameStyle(0);
	m_ppnlSettings = new wxPanel(m_pswpMain, ID_PANEL1, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL, _T("ID_PANEL1"));
	m_ppnlSettings->SetBackgroundColour(wxColour(0,0,0));
	m_pkbd = new wmKeyboard(m_ppnlSettings, ID_M_PKBD2, wxPoint(10,160), wxSize(240,200), 5, 0);
	m_pkbd->SetForegroundColour(wxColour(255,255,255));
	wxFont m_pkbdFont(10,wxFONTFAMILY_SWISS,wxFONTSTYLE_NORMAL,wxFONTWEIGHT_BOLD,false,_T("Arial"),wxFONTENCODING_DEFAULT);
	m_pkbd->SetFont(m_pkbdFont);
	m_pbtnStream = new wmButton(m_ppnlSettings, ID_M_PBTN3, _("Receive"), wxPoint(260,161), wxSize(268,40), wmButton::STYLE_SELECT, wxDefaultValidator, _T("ID_M_PBTN3"));
	m_pbtnStream->SetBackgroundColour(wxColour(0,128,0));
	m_pbtnStream->SetToggleLook(true, wxT("Stop"), wxT("Start"), 40);
	m_pbtnSave = new wmButton(m_ppnlSettings, ID_M_PBTN5, _("Save"), wxPoint(340,315), wxSize(100,40), wmButton::STYLE_SELECT, wxDefaultValidator, _T("ID_M_PBTN5"));
	m_pbtnSave->SetBackgroundColour(wxColour(0,128,0));
	m_plstManualMode = new wmList(m_ppnlSettings, ID_M_PLST1, wxPoint(5,13), wxSize(94,132), 0, 0, wxSize(90,64), 1, wxSize(-1,-1));
	m_plstManualMode->SetBackgroundColour(wxSystemSettings::GetColour(wxSYS_COLOUR_SCROLLBAR));
	m_plstManualMode->SetPressedButtonColour(wxColour(wxT("#F07800")));
	m_pswpManualMode = new wmSwitcherPanel(m_ppnlSettings, ID_M_PSWP2, wxPoint(100,0), wxSize(470,150), wmSwitcherPanel::STYLE_NOSWIPE|wmSwitcherPanel::STYLE_NOANIMATION, _T("ID_M_PSWP2"));
	m_pswpManualMode->SetPageNameStyle(0);
	pnlIp = new wxPanel(m_pswpManualMode, ID_PANEL2, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL, _T("ID_PANEL2"));
	pnlIp->SetBackgroundColour(wxColour(0,0,0));
	m_pLbl9 = new wmLabel(pnlIp, ID_M_PLBL8, _("IP"), wxPoint(10,15), wxSize(100,40), 0, _T("ID_M_PLBL8"));
	m_pLbl9->SetBorderState(uiRect::BORDER_NONE);
	m_pLbl9->GetUiRect().SetGradient(0);
	m_pLbl9->SetForegroundColour(wxColour(255,255,255));
	m_pLbl9->SetBackgroundColour(wxColour(64,0,128));
	m_pipServer = new wmIpEditPnl(pnlIp, ID_M_PIP1, wxPoint(110,15), wxSize(200,40));
	m_pipServer->SetValue(wxEmptyString);
	m_pLbl1 = new wmLabel(pnlIp, ID_M_PLBL1, _("PORT"), wxPoint(310,15), wxSize(50,40), 0, _T("ID_M_PLBL1"));
	m_pLbl1->SetBorderState(uiRect::BORDER_NONE);
	m_pLbl1->GetUiRect().SetGradient(0);
	m_pLbl1->SetForegroundColour(wxColour(255,255,255));
	m_pLbl1->SetBackgroundColour(wxColour(64,0,128));
	m_pedtPort = new wmEdit(pnlIp, ID_M_PEDT3, wxEmptyString, wxPoint(360,15), wxSize(100,40), 0, wxDefaultValidator, _T("ID_M_PEDT3"));
	m_pedtPort->SetValidation(4);
	m_pedtPort->SetBorderStyle(1,1);
	m_pLbl2 = new wmLabel(pnlIp, ID_M_PLBL2, _("Sample Rate"), wxPoint(10,60), wxSize(100,40), 0, _T("ID_M_PLBL2"));
	m_pLbl2->SetBorderState(uiRect::BORDER_NONE);
	m_pLbl2->GetUiRect().SetGradient(0);
	m_pLbl2->SetForegroundColour(wxColour(255,255,255));
	m_pLbl2->SetBackgroundColour(wxColour(64,0,128));
	m_pbtnSampleRate = new wmButton(pnlIp, ID_M_PBTN6, _("48000"), wxPoint(110,60), wxSize(90,40), wmButton::STYLE_NORMAL, wxDefaultValidator, _T("ID_M_PBTN6"));
	m_pbtnSampleRate->SetForegroundColour(wxColour(0,0,0));
	m_pbtnSampleRate->SetBackgroundColour(wxColour(255,255,255));
	m_pbtnSampleRate->SetColourDisabled(wxColour(wxT("#B0B0B0")));
	m_pbtnBits = new wmButton(pnlIp, ID_M_PBTN1, _("Bits"), wxPoint(260,60), wxSize(200,40), wmButton::STYLE_SELECT, wxDefaultValidator, _T("ID_M_PBTN1"));
	m_pbtnBits->SetBackgroundColour(wxColour(0,128,0));
	m_pbtnBits->SetToggleLook(true, wxT("16"), wxT("24"), 40);
	m_pLbl4 = new wmLabel(pnlIp, ID_M_PLBL4, _("Channels"), wxPoint(10,105), wxSize(100,40), 0, _T("ID_M_PLBL4"));
	m_pLbl4->SetBorderState(uiRect::BORDER_NONE);
	m_pLbl4->GetUiRect().SetGradient(0);
	m_pLbl4->SetForegroundColour(wxColour(255,255,255));
	m_pLbl4->SetBackgroundColour(wxColour(64,0,128));
	m_pbtnChannels = new wmButton(pnlIp, ID_M_PBTN2, _("2"), wxPoint(110,105), wxSize(90,40), wmButton::STYLE_NORMAL, wxDefaultValidator, _T("ID_M_PBTN2"));
	m_pbtnChannels->SetForegroundColour(wxColour(0,0,0));
	m_pbtnChannels->SetBackgroundColour(wxColour(255,255,255));
	m_pbtnChannels->SetColourDisabled(wxColour(wxT("#B0B0B0")));
	m_pbtnMapping = new wmButton(pnlIp, ID_M_PBTN13, _("Mapping"), wxPoint(200,105), wxSize(60,40), wmButton::STYLE_NORMAL, wxDefaultValidator, _T("ID_M_PBTN13"));
	m_pbtnMapping->SetForegroundColour(wxColour(0,0,0));
	m_pbtnMapping->SetBackgroundColour(wxColour(163,211,211));
	m_pbtnMapping->SetColourDisabled(wxColour(wxT("#B0B0B0")));
	m_pLbl8 = new wmLabel(pnlIp, ID_M_PLBL11, _("RTP Payload"), wxPoint(310,105), wxSize(70,40), 0, _T("ID_M_PLBL11"));
	m_pLbl8->SetBorderState(uiRect::BORDER_NONE);
	m_pLbl8->GetUiRect().SetGradient(0);
	m_pLbl8->SetForegroundColour(wxColour(255,255,255));
	m_pLbl8->SetBackgroundColour(wxColour(64,0,128));
	m_pbtnRtpMap = new wmButton(pnlIp, ID_M_PBTN4, _("96"), wxPoint(380,105), wxSize(80,40), wmButton::STYLE_NORMAL, wxDefaultValidator, _T("ID_M_PBTN4"));
	m_pbtnRtpMap->SetForegroundColour(wxColour(0,0,0));
	m_pbtnRtpMap->SetBackgroundColour(wxColour(255,255,255));
	m_pbtnRtpMap->SetColourDisabled(wxColour(wxT("#B0B0B0")));
	pnlLivewire = new wxPanel(m_pswpManualMode, ID_PANEL3, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL, _T("ID_PANEL3"));
	pnlLivewire->SetBackgroundColour(wxColour(0,0,0));
	m_pLbl3 = new wmLabel(pnlLivewire, ID_M_PLBL3, _("STREAM"), wxPoint(10,15), wxSize(100,40), 0, _T("ID_M_PLBL3"));
	m_pLbl3->SetBorderState(uiRect::BORDER_NONE);
	m_pLbl3->GetUiRect().SetGradient(0);
	m_pLbl3->SetForegroundColour(wxColour(255,255,255));
	m_pLbl3->SetBackgroundColour(wxColour(64,0,128));
	m_pedtLivewire = new wmEdit(pnlLivewire, ID_M_PEDT1, wxEmptyString, wxPoint(110,15), wxSize(250,40), 0, wxDefaultValidator, _T("ID_M_PEDT1"));
	m_pedtLivewire->SetValidation(4);
	m_pedtLivewire->SetBorderStyle(1,1);
	m_pswpManualMode->AddPage(pnlIp, _("IP"), false);
	m_pswpManualMode->AddPage(pnlLivewire, _("Livewire"), false);
	m_ppnlMapping = new pnlChannelMapping(m_pswpMain,"ManualAoIp", ID_CHANNEL_MAPPING,wxDefaultPosition,wxDefaultSize);
	m_pswpMain->AddPage(m_ppnlSettings, _("Settings"), true);
	m_pswpMain->AddPage(m_ppnlMapping, _("Mapping"), false);

	Connect(ID_M_PBTN3,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&pnlAoipManual::OnbtnStreamClick);
	Connect(ID_M_PBTN5,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&pnlAoipManual::OnbtnSaveClick);
	Connect(ID_M_PBTN6,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&pnlAoipManual::OnbtnSampleRateClick);
	Connect(ID_M_PBTN2,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&pnlAoipManual::OnbtnChannelsClick);
	Connect(ID_M_PBTN13,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&pnlAoipManual::OnbtnMappingClick);
	Connect(ID_M_PBTN4,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&pnlAoipManual::OnbtnRtpMapClick);
	//*)
	SetSize(size);
	SetPosition(pos);

	Connect(m_pipServer->GetId(), wxEVT_IPEDIT_CHANGE, (wxObjectEventFunction)&pnlAoipManual::OnIpChanged);
	Connect(m_pedtPort->GetId(),wxEVT_COMMAND_TEXT_UPDATED,(wxObjectEventFunction)&pnlAoipManual::OnedtRTPPort);

	m_pipServer->ConnectToSetting("ManualAoip", "Source", "0.0.0.0");
	m_pedtPort->ConnectToSetting("ManualAoip", "Port", "5004", true);

    m_pbtnSampleRate->SetPopup({"44100", "48000", "96000"});
    m_pbtnSampleRate->ConnectToSetting("ManualAoIp", "SampleRate", "48000");

    m_pbtnBits->ConnectToSetting("ManualAoIP", "Bits", "24");
    m_pbtnBits->SetColourDisabled(wxColour(150,150,150));


    m_pbtnRtpMap->SetPopup({"96", "97", "98", "99", "100", "101", "102", "103", "104", "105", "106", "107", "108", "109", "110", "111", "112", "113", "114", "115", "116", "117", "118", "119", "120", "121", "122", "123", "124", "125", "126", "127"});
    m_pbtnRtpMap->ConnectToSetting("ManualAoIp", "RtpMap", "96");

	m_pbtnChannels->SetPopup({"1", "2", "3", "4", "5", "6", "7", "8"});
	m_pbtnChannels->ConnectToSetting("ManualAoIp", "Channels", "2");

	m_plstManualMode->AddButton("IP");
	m_plstManualMode->AddButton("Livewire+");
	m_plstManualMode->ConnectToSetting("ManualAoIp", "Mode", "IP");

	Settings::Get().AddHandler(this, "Input", "AoIP");
	Settings::Get().AddHandler(this, "ManualAoIp", "Mode");
	Bind(wxEVT_SETTING_CHANGED, &pnlAoipManual::OnSettingChanged, this);

	Bind(wxEVT_COMMAND_BUTTON_CLICKED, &pnlAoipManual::OnbtnBackClick, this, m_ppnlMapping->m_pbtnBack->GetId());

}

pnlAoipManual::~pnlAoipManual()
{
	//(*Destroy(pnlAoipManual)
	//*)
	Settings::Get().RemoveHandler(this);
}


void pnlAoipManual::OnbtnSampleRateClick(wxCommandEvent& event)
{
}

wxString pnlAoipManual::CreateSDP()
{
    wxString sSdp;
    if(Settings::Get().Read("ManualAoIp", "Mode", "IP") == "IP")
    {
        sSdp << "v=0\r\n"
         << "o=- " << wxDateTime::UNow().Format("%Y%m%d%H%M%S%l") << " 1 IN 169.254.1.1\r\n"
         << "s=Manual SDP created by PAM\r\n"
         << "t=0 0\r\n"
         << "a=recvonly\r\n"
         << "m=audio " << Settings::Get().Read("ManualAoip", "Port", "5004") << " RTP/AVP " << Settings::Get().Read("ManualAoip","RtpMap", "96") << "\r\n"
         << "c=IN IP4 " << Settings::Get().Read("ManualAoip","Source", "0.0.0.0") << "/255\r\n"
         << "a=rtpmap:" << Settings::Get().Read("ManualAoip","RtpMap", "96") <<" L" << Settings::Get().Read("ManualAoip","Bits", "24") << "/" << Settings::Get().Read("ManualAoIp", "SampleRate", "48000") << "/"
         << Settings::Get().Read("ManualAoIp", "Channels", "2") << "\r\n";

         if(Settings::Get().Read("ManualAoip", "MappingEnabled",0) == 1)
         {
            sSdp << "a=fmtp:" << Settings::Get().Read("ManualAoip","RtpMap", "96") << " channel-order=SMPTE2110.(" << Settings::Get().Read("ManualAoip","ChannelMapping", "St") << ")\r\n";
         }
         sSdp << "a=mediaclk:direct=0\r\n";
        //@todo grandmaster if we have ,
    }
    else
    {
        unsigned long nChannel = 0;
        m_pedtLivewire->GetValue().ToULong(&nChannel);
        unsigned long X = nChannel/256;
        unsigned long Y = nChannel-(X*256);

        sSdp << "v=0\r\n"
         << "o=- " << wxDateTime::UNow().Format("%Y%m%d%H%M%S%l") << " 1 IN 169.254.1.1\r\n"
         << "s=Manual SDP created by PAM\r\n"
         << "t=0 0\r\n"
         << "a=recvonly\r\n"
         << "m=audio 5004" << " RTP/AVP " << "96" << "\r\n"
         << "c=IN IP4 239.192." << X << "." << Y << "/255\r\n"
         << "a=rtpmap:96 L24/48000/2\r\n";

         sSdp << "a=mediaclk:direct=0\r\n";
         //@todo grandmaster if we have ,

    }
    return sSdp;
}

void pnlAoipManual::OnbtnStreamClick(wxCommandEvent& event)
{
    if(event.IsChecked())
    {
        wxString sSdp = CreateSDP();
        int nInput = AoipSourceManager::SOURCE_MANUAL_A;
        if(Settings::Get().Read("Input", "AoIP", 0) == AoipSourceManager::SOURCE_MANUAL_A)
        {
            nInput = AoipSourceManager::SOURCE_MANUAL_B;
        }
        AoipSourceManager::Get().SetSourceSDP(nInput, sSdp);
        Settings::Get().Write("Input", "AoIP", nInput); //write the new source number i
    }
    else
    {
        Settings::Get().Write("Input", "AoIP", 0); //write the new source number in
    }
}

void pnlAoipManual::OnSettingChanged(SettingEvent& event)
{
    if(event.GetSection() == "Input" && event.GetKey() == "AoIP")
    {
        m_pbtnBits->Enable(event.GetValue(0l) == 0);
        m_pbtnChannels->Enable(event.GetValue(0l) == 0);
        m_pbtnRtpMap->Enable(event.GetValue(0l) == 0);
        m_pipServer->Enable(event.GetValue(0l) == 0);
        m_pkbd->Enable(event.GetValue(0l) == 0);
        m_pedtPort->Enable(event.GetValue(0l) == 0);
        m_pbtnSampleRate->Enable(event.GetValue(0l) == 0);
    }
    else if(event.GetSection() == "ManualAoIp" && event.GetKey() == "Mode")
    {
        m_pswpManualMode->ChangeSelection(event.GetValue());
    }
}

void pnlAoipManual::OnbtnBitsClick(wxCommandEvent& event)
{
}

void pnlAoipManual::OnbtnRtpMapClick(wxCommandEvent& event)
{
}

void pnlAoipManual::OnbtnChannelsClick(wxCommandEvent& event)
{
}

void pnlAoipManual::OnIpChanged(wxCommandEvent& event)
{
}

void pnlAoipManual::OnedtRTPPort(wxCommandEvent& event)
{
}

void pnlAoipManual::OnbtnSaveClick(wxCommandEvent& event)
{
    dlgEditName dlg(this, "");
    if(dlg.ShowModal() == wxID_OK)
    {
        AoipSourceManager::Get().AddSource(dlg.m_pedtName->GetValue(), "", CreateSDP());
    }
}

void pnlAoipManual::OnbtnMappingClick(wxCommandEvent& event)
{
    m_pswpMain->ChangeSelection("Mapping");
}

void pnlAoipManual::OnbtnBackClick(wxCommandEvent& event)
{
    m_pswpMain->ChangeSelection("Settings");
}
