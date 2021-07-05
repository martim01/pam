#include "pnlAoipManual.h"
#include "settings.h"

//(*InternalHeaders(pnlAoipManual)
#include <wx/font.h>
#include <wx/intl.h>
#include <wx/string.h>
//*)

//(*IdInit(pnlAoipManual)
const long pnlAoipManual::ID_M_PLBL8 = wxNewId();
const long pnlAoipManual::ID_M_PIP1 = wxNewId();
const long pnlAoipManual::ID_M_PLBL1 = wxNewId();
const long pnlAoipManual::ID_M_PEDT3 = wxNewId();
const long pnlAoipManual::ID_M_PLBL2 = wxNewId();
const long pnlAoipManual::ID_M_PBTN6 = wxNewId();
const long pnlAoipManual::ID_M_PBTN1 = wxNewId();
const long pnlAoipManual::ID_M_PLBL4 = wxNewId();
const long pnlAoipManual::ID_M_PLST5 = wxNewId();
const long pnlAoipManual::ID_M_PKBD2 = wxNewId();
const long pnlAoipManual::ID_M_PBTN3 = wxNewId();
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
	m_pLbl9 = new wmLabel(this, ID_M_PLBL8, _("IP"), wxPoint(10,15), wxSize(100,40), 0, _T("ID_M_PLBL8"));
	m_pLbl9->SetBorderState(uiRect::BORDER_NONE);
	m_pLbl9->GetUiRect().SetGradient(0);
	m_pLbl9->SetForegroundColour(wxColour(255,255,255));
	m_pLbl9->SetBackgroundColour(wxColour(64,0,128));
	m_pipServer = new wmIpEditPnl(this, ID_M_PIP1, wxPoint(110,15), wxSize(200,40));
	m_pipServer->SetValue(wxEmptyString);
	m_pLbl1 = new wmLabel(this, ID_M_PLBL1, _("PORT"), wxPoint(310,15), wxSize(50,40), 0, _T("ID_M_PLBL1"));
	m_pLbl1->SetBorderState(uiRect::BORDER_NONE);
	m_pLbl1->GetUiRect().SetGradient(0);
	m_pLbl1->SetForegroundColour(wxColour(255,255,255));
	m_pLbl1->SetBackgroundColour(wxColour(64,0,128));
	m_pedtPort = new wmEdit(this, ID_M_PEDT3, wxEmptyString, wxPoint(360,15), wxSize(100,40), 0, wxDefaultValidator, _T("ID_M_PEDT3"));
	m_pedtPort->SetValidation(4);
	m_pedtPort->SetBorderStyle(1,1);
	m_pLbl2 = new wmLabel(this, ID_M_PLBL2, _("Sample Rate"), wxPoint(10,60), wxSize(100,40), 0, _T("ID_M_PLBL2"));
	m_pLbl2->SetBorderState(uiRect::BORDER_NONE);
	m_pLbl2->GetUiRect().SetGradient(0);
	m_pLbl2->SetForegroundColour(wxColour(255,255,255));
	m_pLbl2->SetBackgroundColour(wxColour(64,0,128));
	m_pbtnSampleRate = new wmButton(this, ID_M_PBTN6, _("48000"), wxPoint(110,60), wxSize(92,40), wmButton::STYLE_NORMAL, wxDefaultValidator, _T("ID_M_PBTN6"));
	m_pbtnSampleRate->SetForegroundColour(wxColour(0,0,0));
	m_pbtnSampleRate->SetBackgroundColour(wxColour(255,255,255));
	m_pbtnBits = new wmButton(this, ID_M_PBTN1, _("Bits"), wxPoint(260,60), wxSize(200,40), wmButton::STYLE_SELECT, wxDefaultValidator, _T("ID_M_PBTN1"));
	m_pbtnBits->SetBackgroundColour(wxColour(0,128,0));
	m_pbtnBits->SetToggleLook(true, wxT("16"), wxT("24"), 40);
	m_pLbl4 = new wmLabel(this, ID_M_PLBL4, _("Channels"), wxPoint(10,105), wxSize(100,40), 0, _T("ID_M_PLBL4"));
	m_pLbl4->SetBorderState(uiRect::BORDER_NONE);
	m_pLbl4->GetUiRect().SetGradient(0);
	m_pLbl4->SetForegroundColour(wxColour(255,255,255));
	m_pLbl4->SetBackgroundColour(wxColour(64,0,128));
	m_plstChannels = new wmList(this, ID_M_PLST5, wxPoint(110,105), wxSize(480,40), wmList::STYLE_SELECT, 0, wxSize(-1,-1), 8, wxSize(5,0));
	m_plstChannels->SetBackgroundColour(wxColour(0,0,0));
	m_plstChannels->SetButtonColour(wxColour(wxT("#004040")));
	m_plstChannels->SetSelectedButtonColour(wxColour(wxT("#FF8000")));
	m_pkbd = new wmKeyboard(this, ID_M_PKBD2, wxPoint(10,160), wxSize(240,200), 5, 0);
	m_pkbd->SetForegroundColour(wxColour(255,255,255));
	wxFont m_pkbdFont(10,wxFONTFAMILY_SWISS,wxFONTSTYLE_NORMAL,wxFONTWEIGHT_BOLD,false,_T("Arial"),wxFONTENCODING_DEFAULT);
	m_pkbd->SetFont(m_pkbdFont);
	m_pbtnStream = new wmButton(this, ID_M_PBTN3, _("Receive"), wxPoint(300,320), wxSize(268,40), wmButton::STYLE_SELECT, wxDefaultValidator, _T("ID_M_PBTN3"));
	m_pbtnStream->SetBackgroundColour(wxColour(0,128,0));
	m_pbtnStream->SetToggleLook(true, wxT("Stop"), wxT("Start"), 40);

	Connect(ID_M_PBTN6,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&pnlAoipManual::OnbtnSampleRateClick);
	Connect(ID_M_PBTN3,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&pnlAoipManual::OnbtnStreamClick);
	//*)
	SetSize(size);
	SetPosition(pos);

	for(int i = 1; i < 9; i++)
    {
        m_plstChannels->AddButton(wxString::Format("%d", i));
    }

	m_pipServer->SetValue(Settings::Get().Read("ManualAoip", "Source", ""));
	m_pedtPort->SetValue(Settings::Get().Read("ManualAoip", "Port", "5004"));
	m_pbtnSampleRate->SetLabel(Settings::Get().Read("ManualAoip", "SampleRate", "48000"));
	m_pbtnBits->ToggleSelection(Settings::Get().Read("ManualAoip", "Bits", 24) == 24);
	m_plstChannels->SelectButton(Settings::Get().Read("ManualAoip", "Channels", "2"));

}

pnlAoipManual::~pnlAoipManual()
{
	//(*Destroy(pnlAoipManual)
	//*)
}


void pnlAoipManual::OnbtnSampleRateClick(wxCommandEvent& event)
{
    wxArrayString asButtons;
	asButtons.Add(wxT("44100"));
    asButtons.Add(wxT("48000"));
    asButtons.Add(wxT("96000"));

    dlgMask aDlg(this, asButtons, m_pbtnSampleRate->GetLabel(), wxNewId(), ClientToScreen(m_pbtnSampleRate->GetPosition()), m_pbtnSampleRate->GetSize());
    if(aDlg.ShowModal()== wxID_OK)
    {
        m_pbtnSampleRate->SetLabel(aDlg.m_sSelected);
        m_pipServer->SetFocus();
    }
}

void pnlAoipManual::OnbtnStreamClick(wxCommandEvent& event)
{
    m_pbtnBits->Enable(event.IsChecked() == false);
    m_plstChannels->Enable(event.IsChecked() == false);
    m_pipServer->Enable(event.IsChecked() == false);
    m_pkbd->Enable(event.IsChecked() == false);
    m_pedtPort->Enable(event.IsChecked() == false);

    if(event.IsChecked())
    {
        wxString sSdp;
        sSdp << "v=0\r\n"
             << "o=- " << sess-id << " " << sess-v << " IN 169.254.1.1\r\n"
             << "s=Manual SDP created by PAM\r\n"
             << "t=0 0\r\n"
             << "a=recvonly\r\n"
             << "m=audio " << m_pedtPort->GetValue() << " RTP/AVP 98\r\n"
             << "c=IN IP4 " << m_pipServer->GetValue() << "\r\n"
             << "a=rtpmap:98 L" << (m_pbtnBits->IsChecked() ? "24" : "16") << "/" << m_pbtnSampleRate->GetLabel() << "/" << m_sChannels << "\r\n"
             << "a=mediaclk:direct=0\r\n";
         //@todo grandmaster if we have one

        Settings::Get().Write("Input", "AoIPManual", sSdp);
    }
    else
    {
        Settings::Get().Write("Input", "AoIPManual", "");
    }
}
