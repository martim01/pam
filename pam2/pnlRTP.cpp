#include "pnlRTP.h"

//(*InternalHeaders(pnlRTP)
#include <wx/font.h>
#include <wx/intl.h>
#include <wx/string.h>
//*)

#include "settings.h"
#include <wx/log.h>
#include <wx/tokenzr.h>
#include "dnssdbrowser.h"
#include "sapwatchthread.h"
#include "settings.h"
#include "wmlogevent.h"
#include "PamUsageEnvironment.h"
#include "PamTaskScheduler.h"
#include "rtpthread.h"
#include "wmlistadv.h"
#include "logelement.h"
#include <wx/dcclient.h>
#include "inimanager.h"
#include "inisection.h"
#include "settingevent.h"
#include <wx/dir.h>
#include "wxdirtraverseusb.h"
#include "aoipsourcemanager.h"

using namespace std;


//(*IdInit(pnlRTP)
const long pnlRTP::ID_M_PLST1 = wxNewId();
const long pnlRTP::ID_M_PBTN3 = wxNewId();
const long pnlRTP::ID_M_PBTN4 = wxNewId();
const long pnlRTP::ID_M_PBTN5 = wxNewId();
const long pnlRTP::ID_M_PBTN7 = wxNewId();
const long pnlRTP::ID_M_PBTN11 = wxNewId();
const long pnlRTP::ID_M_PBTN6 = wxNewId();
const long pnlRTP::ID_M_PLBL4 = wxNewId();
const long pnlRTP::ID_M_PLBL5 = wxNewId();
const long pnlRTP::ID_PANEL1 = wxNewId();
const long pnlRTP::ID_M_PLBL1 = wxNewId();
const long pnlRTP::ID_M_PEDT1 = wxNewId();
const long pnlRTP::ID_M_PLBL2 = wxNewId();
const long pnlRTP::ID_M_PEDT2 = wxNewId();
const long pnlRTP::ID_M_PBTN1 = wxNewId();
const long pnlRTP::ID_M_PBTN2 = wxNewId();
const long pnlRTP::ID_M_PKBD1 = wxNewId();
const long pnlRTP::ID_M_PLBL6 = wxNewId();
const long pnlRTP::ID_PANEL2 = wxNewId();
const long pnlRTP::ID_M_PLBL7 = wxNewId();
const long pnlRTP::ID_M_PBTN8 = wxNewId();
const long pnlRTP::ID_M_PBTN9 = wxNewId();
const long pnlRTP::ID_M_PLBL9 = wxNewId();
const long pnlRTP::ID_M_PLBL8 = wxNewId();
const long pnlRTP::ID_M_PLST2 = wxNewId();
const long pnlRTP::ID_M_PBTN10 = wxNewId();
const long pnlRTP::ID_HTMLWINDOW1 = wxNewId();
const long pnlRTP::ID_PANEL3 = wxNewId();
const long pnlRTP::ID_PANEL5 = wxNewId();
const long pnlRTP::ID_M_PLBL3 = wxNewId();
const long pnlRTP::ID_M_PLST3 = wxNewId();
const long pnlRTP::ID_M_PBTN12 = wxNewId();
const long pnlRTP::ID_M_PBTN13 = wxNewId();
const long pnlRTP::ID_M_PLBL10 = wxNewId();
const long pnlRTP::ID_PANEL4 = wxNewId();
const long pnlRTP::ID_M_PSWP1 = wxNewId();
//*)



BEGIN_EVENT_TABLE(pnlRTP,wxPanel)
	//(*EventTable(pnlRTP)
	//*)
END_EVENT_TABLE()

pnlRTP::pnlRTP(wxWindow* parent,wxWindowID id,const wxPoint& pos,const wxSize& size, long nStyle, const wxString& sId) : m_pThread(0)
{
    //(*Initialize(pnlRTP)
    Create(parent, id, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL, _T("id"));
    SetBackgroundColour(wxColour(0,0,0));
    wxFont thisFont(10,wxFONTFAMILY_SWISS,wxFONTSTYLE_NORMAL,wxFONTWEIGHT_NORMAL,false,_T("Arial"),wxFONTENCODING_DEFAULT);
    SetFont(thisFont);
    m_pSwp1 = new wmSwitcherPanel(this, ID_M_PSWP1, wxPoint(0,0), wxSize(800,440), wmSwitcherPanel::STYLE_NOSWIPE|wmSwitcherPanel::STYLE_NOANIMATION, _T("ID_M_PSWP1"));
    m_pSwp1->SetPageNameStyle(0);
    Panel1 = new wxPanel(m_pSwp1, ID_PANEL1, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL, _T("ID_PANEL1"));
    Panel1->SetBackgroundColour(wxColour(0,0,0));
    m_plstSources = new wmList(Panel1, ID_M_PLST1, wxPoint(0,36), wxSize(800,349), wmList::STYLE_SELECT|wmList::STYLE_SELECT_ROW, 1, wxSize(-1,30), 2, wxSize(-1,-1));
    m_plstSources->SetBackgroundColour(wxColour(255,255,255));
    m_plstSources->SetGradient(128);
    m_plstSources->SetBorderStyle(3);
    m_plstSources->SetButtonColour(wxColour(wxT("#FFFFFF")));
    m_plstSources->SetPressedButtonColour(wxColour(wxT("#FFFFFF")));
    m_plstSources->SetSelectedButtonColour(wxColour(wxT("#8080FF")));
    m_plstSources->SetTextButtonColour(wxColour(wxT("#000000")));
    m_plstSources->SetTextPressedButtonColour(wxColour(wxT("#000000")));
    m_plstSources->SetTextSelectedButtonColour(wxColour(wxT("#000000")));
    m_pbtnAdd = new wmButton(Panel1, ID_M_PBTN3, _("Add\nSource"), wxPoint(10,390), wxSize(100,45), 0, wxDefaultValidator, _T("ID_M_PBTN3"));
    m_pbtnUpdate = new wmButton(Panel1, ID_M_PBTN4, _("Update\nSelected"), wxPoint(115,390), wxSize(100,45), 0, wxDefaultValidator, _T("ID_M_PBTN4"));
    m_pbtnUpdate->Disable();
    m_pbtnUpdate->SetColourDisabled(wxColour(wxT("#909090")));
    m_pbtnDelete = new wmButton(Panel1, ID_M_PBTN5, _("Delete\nSelected"), wxPoint(224,390), wxSize(100,45), 0, wxDefaultValidator, _T("ID_M_PBTN5"));
    m_pbtnDelete->Disable();
    m_pbtnDelete->SetColourDisabled(wxColour(wxT("#909090")));
    m_pbtnDeleteAll = new wmButton(Panel1, ID_M_PBTN7, _("Hold To\nDelete All"), wxPoint(344,390), wxSize(100,45), wmButton::STYLE_HOLD, wxDefaultValidator, _T("ID_M_PBTN7"));
    m_pbtnDeleteAll->SetColourDisabled(wxColour(wxT("#909090")));
    m_pbtnImport = new wmButton(Panel1, ID_M_PBTN11, _("Import From USB"), wxPoint(570,390), wxSize(100,45), 0, wxDefaultValidator, _T("ID_M_PBTN11"));
    m_pbtnImport->SetBackgroundColour(wxColour(146,50,252));
    m_pbtnImport->SetColourSelected(wxColour(wxT("#F07800")));
    m_pbtnDiscover = new wmButton(Panel1, ID_M_PBTN6, _("Discovery"), wxPoint(690,390), wxSize(100,45), 0, wxDefaultValidator, _T("ID_M_PBTN6"));
    m_pbtnDiscover->SetBackgroundColour(wxColour(0,128,0));
    m_pbtnDiscover->SetColourSelected(wxColour(wxT("#F07800")));
    m_pLbl4 = new wmLabel(Panel1, ID_M_PLBL4, _("Name"), wxPoint(0,5), wxSize(400,30), 0, _T("ID_M_PLBL4"));
    m_pLbl4->SetBorderState(uiRect::BORDER_UP);
    m_pLbl4->GetUiRect().SetGradient(0);
    m_pLbl4->SetForegroundColour(wxColour(255,255,255));
    m_pLbl4->SetBackgroundColour(wxColour(64,200,128));
    m_pLbl5 = new wmLabel(Panel1, ID_M_PLBL5, _("URL"), wxPoint(400,5), wxSize(400,30), 0, _T("ID_M_PLBL5"));
    m_pLbl5->SetBorderState(uiRect::BORDER_UP);
    m_pLbl5->GetUiRect().SetGradient(0);
    m_pLbl5->SetForegroundColour(wxColour(255,255,255));
    m_pLbl5->SetBackgroundColour(wxColour(64,200,128));
    Panel2 = new wxPanel(m_pSwp1, ID_PANEL2, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL, _T("ID_PANEL2"));
    Panel2->SetBackgroundColour(wxColour(0,0,0));
    m_pLbl1 = new wmLabel(Panel2, ID_M_PLBL1, _("Name"), wxPoint(10,40), wxSize(60,30), 0, _T("ID_M_PLBL1"));
    m_pLbl1->SetBorderState(uiRect::BORDER_NONE);
    m_pLbl1->GetUiRect().SetGradient(0);
    m_pLbl1->SetForegroundColour(wxColour(255,255,255));
    m_pLbl1->SetBackgroundColour(wxColour(0,0,255));
    m_pedtName = new wmEdit(Panel2, ID_M_PEDT1, _("Text"), wxPoint(71,40), wxSize(611,30), 0, wxDefaultValidator, _T("ID_M_PEDT1"));
    m_pedtName->SetValidation(0);
    m_pedtName->SetForegroundColour(wxColour(0,0,0));
    m_pedtName->SetBackgroundColour(wxColour(255,255,255));
    m_pedtName->SetFocusedBackground(wxColour(wxT("#FFFF9D")));
    m_pedtName->SetFocusedForeground(wxColour(wxT("#000000")));
    m_pedtName->SetBorderStyle(1,1);
    m_pLbl2 = new wmLabel(Panel2, ID_M_PLBL2, _("URL"), wxPoint(10,80), wxSize(60,30), 0, _T("ID_M_PLBL2"));
    m_pLbl2->SetBorderState(uiRect::BORDER_NONE);
    m_pLbl2->GetUiRect().SetGradient(0);
    m_pLbl2->SetForegroundColour(wxColour(255,255,255));
    m_pLbl2->SetBackgroundColour(wxColour(0,0,255));
    m_pedtUrl = new wmEdit(Panel2, ID_M_PEDT2, _("Text"), wxPoint(71,80), wxSize(611,30), 0, wxDefaultValidator, _T("ID_M_PEDT2"));
    m_pedtUrl->SetValidation(0);
    m_pedtUrl->SetForegroundColour(wxColour(0,0,0));
    m_pedtUrl->SetBackgroundColour(wxColour(255,255,255));
    m_pedtUrl->SetFocusedBackground(wxColour(wxT("#FFFF9D")));
    m_pedtUrl->SetFocusedForeground(wxColour(wxT("#000000")));
    m_pedtUrl->SetBorderStyle(1,1);
    m_pbtnConfirm = new wmButton(Panel2, ID_M_PBTN1, _("Add"), wxPoint(290,120), wxSize(100,45), 0, wxDefaultValidator, _T("ID_M_PBTN1"));
    m_pbtnConfirm->SetBackgroundColour(wxColour(0,128,0));
    m_pbtnConfirm->SetColourSelected(wxColour(wxT("#006F00")));
    m_pbtnCancel = new wmButton(Panel2, ID_M_PBTN2, _("Cancel"), wxPoint(410,120), wxSize(100,45), 0, wxDefaultValidator, _T("ID_M_PBTN2"));
    m_pbtnCancel->SetBackgroundColour(wxColour(128,0,0));
    m_pbtnCancel->SetColourSelected(wxColour(wxT("#6F0000")));
    m_pkeyboard = new wmKeyboard(Panel2, ID_M_PKBD1, wxPoint(100,180), wxSize(600,250), 0, 0);
    m_pkeyboard->SetForegroundColour(wxColour(255,255,255));
    wxFont m_pkeyboardFont(14,wxFONTFAMILY_SWISS,wxFONTSTYLE_NORMAL,wxFONTWEIGHT_NORMAL,false,_T("Arial"),wxFONTENCODING_DEFAULT);
    m_pkeyboard->SetFont(m_pkeyboardFont);
    m_pLbl6 = new wmLabel(Panel2, ID_M_PLBL6, _("Add/Edit AoIP Source"), wxPoint(0,5), wxSize(800,30), 0, _T("ID_M_PLBL6"));
    m_pLbl6->SetBorderState(uiRect::BORDER_NONE);
    m_pLbl6->GetUiRect().SetGradient(0);
    m_pLbl6->SetForegroundColour(wxColour(255,255,255));
    m_pLbl6->SetBackgroundColour(wxColour(0,64,0));
    wxFont m_pLbl6Font(10,wxFONTFAMILY_SWISS,wxFONTSTYLE_NORMAL,wxFONTWEIGHT_BOLD,false,_T("Arial"),wxFONTENCODING_DEFAULT);
    m_pLbl6->SetFont(m_pLbl6Font);
    pnlDiscovery = new wxPanel(m_pSwp1, ID_PANEL3, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL, _T("ID_PANEL3"));
    pnlDiscovery->SetBackgroundColour(wxColour(0,0,0));
    m_plblDiscovering = new wmLabel(pnlDiscovery, ID_M_PLBL7, wxEmptyString, wxPoint(210,395), wxSize(250,45), 0, _T("ID_M_PLBL7"));
    m_plblDiscovering->SetBorderState(uiRect::BORDER_NONE);
    m_plblDiscovering->GetUiRect().SetGradient(0);
    m_plblDiscovering->SetForegroundColour(wxColour(255,255,255));
    m_plblDiscovering->SetBackgroundColour(wxColour(0,0,0));
    m_pbtnSAP = new wmButton(pnlDiscovery, ID_M_PBTN8, _("SAP"), wxPoint(5,230), wxSize(130,45), wmButton::STYLE_SELECT, wxDefaultValidator, _T("ID_M_PBTN8"));
    m_pbtnSAP->SetForegroundColour(wxColour(255,255,255));
    m_pbtnSAP->SetBackgroundColour(wxColour(0,0,64));
    m_pbtnSAP->SetToggleLook(true, wxT("No"), wxT("Yes"), 50);
    m_pbtnStartDiscovery = new wmButton(pnlDiscovery, ID_M_PBTN9, _("Discovery"), wxPoint(5,390), wxSize(200,45), wmButton::STYLE_SELECT, wxDefaultValidator, _T("ID_M_PBTN9"));
    m_pbtnStartDiscovery->SetForegroundColour(wxColour(255,255,255));
    m_pbtnStartDiscovery->SetBackgroundColour(wxColour(0,0,160));
    m_pbtnStartDiscovery->SetToggleLook(true, wxT("Stop"), wxT("Start"), 50);
    m_pLbl8 = new wmLabel(pnlDiscovery, ID_M_PLBL9, _("Options"), wxPoint(5,3), wxSize(130,30), 0, _T("ID_M_PLBL9"));
    m_pLbl8->SetBorderState(uiRect::BORDER_NONE);
    m_pLbl8->GetUiRect().SetGradient(0);
    m_pLbl8->SetForegroundColour(wxColour(255,255,255));
    m_pLbl8->SetBackgroundColour(wxColour(0,128,64));
    m_pLbl7 = new wmLabel(pnlDiscovery, ID_M_PLBL8, _("DNS-SD"), wxPoint(5,35), wxSize(130,30), 0, _T("ID_M_PLBL8"));
    m_pLbl7->SetBorderState(uiRect::BORDER_NONE);
    m_pLbl7->GetUiRect().SetGradient(0);
    m_pLbl7->SetForegroundColour(wxColour(255,255,255));
    m_pLbl7->SetBackgroundColour(wxColour(0,0,64));
    m_plstServices = new wmList(pnlDiscovery, ID_M_PLST2, wxPoint(5,65), wxSize(130,160), wmList::STYLE_SELECT|wmList::STYLE_SELECT_MULTI, 0, wxSize(-1,-1), 1, wxSize(1,5));
    m_plstServices->SetBackgroundColour(wxColour(0,0,0));
    m_pbtnManual = new wmButton(pnlDiscovery, ID_M_PBTN10, _("Back"), wxPoint(690,390), wxSize(100,45), 0, wxDefaultValidator, _T("ID_M_PBTN10"));
    m_pbtnManual->SetBackgroundColour(wxColour(0,128,0));
    m_pbtnManual->SetColourSelected(wxColour(wxT("#F07800")));

    Panel3 = new wxPanel(m_pSwp1, ID_PANEL4, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL, _T("ID_PANEL4"));
    m_pnlUSB = new pnlUSB(Panel3, ID_PANEL5, wxPoint(0,0), wxSize(600,315), wxTAB_TRAVERSAL, _T("ID_PANEL5"));
    m_pnlUSB->SetBackgroundColour(wxColour(0,0,0));
    m_pLbl3 = new wmLabel(Panel3, ID_M_PLBL3, _("Import Files Found"), wxPoint(600,0), wxSize(200,30), 0, _T("ID_M_PLBL3"));
    m_pLbl3->SetBorderState(uiRect::BORDER_UP);
    m_pLbl3->GetUiRect().SetGradient(0);
    m_pLbl3->SetForegroundColour(wxColour(255,255,255));
    m_pLbl3->SetBackgroundColour(wxColour(64,200,128));
    m_plstFiles = new wmList(Panel3, ID_M_PLST3, wxPoint(600,30), wxSize(200,350), wmList::STYLE_SELECT, 1, wxSize(-1,-1), 1, wxSize(-1,-1));
    m_pbtnImportImport = new wmButton(Panel3, ID_M_PBTN12, _("Import"), wxPoint(690,390), wxSize(100,45), wmButton::STYLE_SELECT, wxDefaultValidator, _T("ID_M_PBTN12"));
    m_pbtnImportImport->SetBackgroundColour(wxColour(0,128,0));
    m_pbtnImportImport->SetColourSelected(wxColour(wxT("#F07800")));
    m_pbtnImportBack = new wmButton(Panel3, ID_M_PBTN13, _("Back"), wxPoint(570,390), wxSize(100,45), 0, wxDefaultValidator, _T("ID_M_PBTN13"));
    m_pbtnImportBack->SetBackgroundColour(wxColour(146,50,252));
    m_pbtnImportBack->SetColourSelected(wxColour(wxT("#F07800")));
    m_plblImportProgress = new wmLabel(Panel3, ID_M_PLBL10, wxEmptyString, wxPoint(144,368), wxSize(316,72), 0, _T("ID_M_PLBL10"));
    m_plblImportProgress->SetBorderState(uiRect::BORDER_NONE);
    m_plblImportProgress->GetUiRect().SetGradient(0);
    m_plblImportProgress->SetForegroundColour(wxColour(255,255,255));
    m_plblImportProgress->SetBackgroundColour(wxColour(0,0,0));
    m_pSwp1->AddPage(Panel1, _("Manual"), false);
    m_pSwp1->AddPage(Panel2, _("Edit"), false);
    m_pSwp1->AddPage(pnlDiscovery, _("Discovery"), false);
    m_pSwp1->AddPage(Panel3, _("Import"), false);

    Connect(ID_M_PLST1,wxEVT_LIST_SELECTED,(wxObjectEventFunction)&pnlRTP::OnlstSourcesSelected);
    Connect(ID_M_PBTN3,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&pnlRTP::OnbtnAddClick);
    Connect(ID_M_PBTN4,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&pnlRTP::OnbtnUpdateClick);
    Connect(ID_M_PBTN5,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&pnlRTP::OnbtnDeleteClick);
    Connect(ID_M_PBTN7,wxEVT_BUTTON_HELD,(wxObjectEventFunction)&pnlRTP::OnbtnDeleteAllHeld);
    Connect(ID_M_PBTN11,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&pnlRTP::OnbtnImportClick);
    Connect(ID_M_PBTN6,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&pnlRTP::OnbtnDiscoverClick);
    Connect(ID_M_PEDT1,wxEVT_COMMAND_TEXT_ENTER,(wxObjectEventFunction)&pnlRTP::OnedtNameTextEnter);
    Connect(ID_M_PEDT2,wxEVT_COMMAND_TEXT_ENTER,(wxObjectEventFunction)&pnlRTP::OnedtUrlTextEnter);
    Connect(ID_M_PBTN1,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&pnlRTP::OnbtnConfirmClick);
    Connect(ID_M_PBTN2,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&pnlRTP::OnbtnCancelClick);
    Connect(ID_M_PBTN8,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&pnlRTP::OnbtnSAPClick);
    Connect(ID_M_PBTN9,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&pnlRTP::OnbtnStartDiscoveryClick);
    Connect(ID_M_PLST2,wxEVT_LIST_SELECTED,(wxObjectEventFunction)&pnlRTP::OnlstServicesSelected);
    Connect(ID_M_PBTN10,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&pnlRTP::OnbtnManualClick);
    Connect(ID_M_PLST3,wxEVT_LIST_SELECTED,(wxObjectEventFunction)&pnlRTP::OnlstImportFilesSelected);
    Connect(ID_M_PBTN12,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&pnlRTP::OnbtnImportImportClick);
    Connect(ID_M_PBTN13,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&pnlRTP::OnbtnManualClick);
    //*)

    m_pList = new wmListAdv(pnlDiscovery, wxNewId(), wxPoint(140,0), wxSize(650,385),0, wmListAdv::SCROLL_VERTICAL, wxSize(-1,30), 1, wxSize(0,1));
    m_pList->SetFont(wxFont(8,wxFONTFAMILY_SWISS, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD, false, "Arial", wxFONTENCODING_DEFAULT));
    m_pList->SetBackgroundColour(wxColour(100,100,100));

	Connect(wxID_ANY, wxEVT_SDP, (wxObjectEventFunction)&pnlRTP::OnSDPReceived);
	Connect(wxID_ANY, wxEVT_RTP_SESSION_CLOSED, (wxObjectEventFunction)&pnlRTP::OnRTPClosed);
	m_plstSources->SetGradient(0);
	m_plstSources->SetTextAlign(wxALIGN_CENTER_VERTICAL | wxALIGN_LEFT);

	m_pbtnImportImport->SetColourDisabled(wxColour(180,180,180));
	m_pkeyboard->SetBlankString(wxT(":"));
    m_pkeyboard->SetReturnString(wxT("-->|"));
	SetSize(size);
	SetPosition(pos);
    m_pnlUSB->SetSection(wxT("ImportAoIP"));
	m_plstServices->AddButton(wxT("RTSP"));
	m_plstServices->AddButton(wxT("SIP"));
	//m_plstServices->AddButton(wxT("NMOS"));

	Panel3->SetBackgroundColour(*wxBLACK);

	m_pBrowser = 0;
	m_pSapWatch = 0;
	Connect(wxID_ANY, wxEVT_BROWSE_RESOLVED, (wxObjectEventFunction)&pnlRTP::OnDiscovery);
	Connect(wxID_ANY, wxEVT_BROWSE_FINISHED, (wxObjectEventFunction)&pnlRTP::OnDiscoveryFinished);
	Connect(wxID_ANY, wxEVT_SAP, (wxObjectEventFunction)&pnlRTP::OnSap);

	AoipSourceManager::Get();
    m_nSelectedSource = 0;

	Settings::Get().AddHandler(wxT("ImportAoIP"), wxT("USB"), this);
	Connect(wxID_ANY, wxEVT_SETTING_CHANGED, (wxObjectEventFunction)&pnlRTP::OnSettingEvent);
	if(Settings::Get().Read(wxT("Discovery"), wxT("RTSP"),1) == 1)
    {
        m_plstServices->SelectButton(wxT("RTSP"), false);
    }
    if(Settings::Get().Read(wxT("Discovery"), wxT("SIP"),0) == 1)
    {
        m_plstServices->SelectButton(wxT("SIP"), false);
    }
    if(Settings::Get().Read(wxT("Discovery"), wxT("NMOS"),0) == 1)
    {
        m_plstServices->SelectButton(wxT("NMOS"), false);
    }
    m_pbtnSAP->ToggleSelection((Settings::Get().Read(wxT("Discovery"), wxT("SAP"),0) == 1), false);


	ListSources();
}

pnlRTP::~pnlRTP()
{
	//(*Destroy(pnlRTP)
	//*)
	if(m_pBrowser)
    {
        delete m_pBrowser;
    }
    if(m_pSapWatch)
    {
        m_pSapWatch->Delete();
    }
}


void pnlRTP::OnbtnAddClick(wxCommandEvent& event)
{

    FillInEdit();
    m_pbtnConfirm->SetLabel(wxT("Add"));
}
void pnlRTP::FillInEdit()
{
    AoIPSource source = AoipSourceManager::Get().FindSource(m_nSelectedSource);

    m_pedtName->SetValue(source.sName);
    m_pedtUrl->SetValue(source.sDetails);

    m_pedtName->SetFocus();
    m_pSwp1->ChangeSelection(1);
}

void pnlRTP::OnedtNameTextEnter(wxCommandEvent& event)
{
    m_pedtUrl->SetFocus();
}

void pnlRTP::OnedtUrlTextEnter(wxCommandEvent& event)
{
    m_pedtName->SetFocus();
}

void pnlRTP::OnbtnConfirmClick(wxCommandEvent& event)
{
    if(m_nSelectedSource != 0)
    {
        AoipSourceManager::Get().EditSource(m_nSelectedSource,m_pedtName->GetValue(), m_pedtUrl->GetValue());
    }
    else
    {
        AoipSourceManager::Get().AddSource(m_pedtName->GetValue(), m_pedtUrl->GetValue());
    }

    ListSources();
    m_pSwp1->ChangeSelection(0);
    m_nSelectedSource = 0;
    m_pbtnDelete->Disable();
    m_pbtnUpdate->Disable();

}

void pnlRTP::OnbtnUpdateClick(wxCommandEvent& event)
{
    FillInEdit();
    m_pbtnConfirm->SetLabel(wxT("Update"));
}

void pnlRTP::OnbtnDeleteClick(wxCommandEvent& event)
{
    if(Settings::Get().Read("Input", "AoIP", 0) == m_nSelectedSource)
    {
        Settings::Get().Write("Input", "AoIP",0);
    }
    AoipSourceManager::Get().DeleteSource(m_nSelectedSource);
    m_nSelectedSource = 0;

    ListSources();
    m_pbtnDelete->Disable();
    m_pbtnUpdate->Disable();
}


void pnlRTP::ListSources()
{
    m_plstSources->Freeze();
    m_plstSources->Clear();

    for(auto itSource = AoipSourceManager::Get().GetSourceBegin(); itSource != AoipSourceManager::Get().GetSourceEnd(); ++itSource)
    {
        m_plstSources->AddButton(itSource->second.sName, wxNullBitmap, (void*)itSource->first);
        m_plstSources->AddButton(itSource->second.sDetails, wxNullBitmap, (void*)itSource->first);
    }
        m_plstSources->Thaw();
}

void pnlRTP::OnlstSourcesSelected(wxCommandEvent& event)
{
    m_nSelectedSource = (unsigned int)event.GetClientData();

    m_pbtnDelete->Enable();
    m_pbtnUpdate->Enable();
}


void pnlRTP::OnbtnCancelClick(wxCommandEvent& event)
{
    m_pSwp1->ChangeSelection(wxT("Manual"));
}

void pnlRTP::OnbtnDiscoverClick(wxCommandEvent& event)
{
    m_pSwp1->ChangeSelection(wxT("Discovery"));
    m_pnlUSB->StopCheck();
}


void pnlRTP::OnDiscovery(wxCommandEvent& event)
{
    dnsInstance* pInstance = reinterpret_cast<dnsInstance*>(event.GetClientData());

    wxString sIdentifier;
    if(pInstance->sService == "_rtsp._tcp")
    {
        sIdentifier = ("{"+pInstance->sService.AfterFirst('_').BeforeFirst('.')+"} "+pInstance->sName.BeforeFirst('('));
    }
    else if(pInstance->sService == "_sipuri._udp")
    {
        sIdentifier = ("{"+pInstance->sService.AfterFirst('_').BeforeFirst('.')+"} "+pInstance->sName.AfterFirst(' ').BeforeFirst('('));
    }


    if(m_setDiscover.insert(make_pair(sIdentifier, pInstance->sHostIP)).second)
    {
        wxString sAddress;
        if(pInstance->sService == "_rtsp._tcp")
        {
            if(pInstance->nPort == 554)
            {
                sAddress = (wxString::Format(wxT("rtsp://%s/by-name/%s"), pInstance->sHostIP.c_str(), pInstance->sName.c_str()));
            }
            else
            {
                sAddress = (wxString::Format(wxT("rtsp://%s:%d/by-name/%s"), pInstance->sHostIP.c_str(), pInstance->nPort, pInstance->sName.c_str()));
            }
            AoipSourceManager::Get().AddSource(wxString::Format(wxT("%s(%s)"), sIdentifier.c_str(), pInstance->sHostIP.c_str()), sAddress);
        }
        else if(pInstance->sService == "_sipuri._udp")
        {
            sAddress = pInstance->sName.BeforeFirst(' ');
            AoipSourceManager::Get().AddSource(wxString::Format(wxT("%s(%s)"), sIdentifier.c_str(), pInstance->sHostIP.c_str()), sAddress);
        }
        wxClientDC dc(this);
        dc.SetFont(m_pList->GetFont());
        LogElement* pElement(new LogElement(dc, GetClientSize().x, wxString::Format(wxT("[%s] %s = %s:%lu"), pInstance->sService.c_str(), pInstance->sName.c_str(), pInstance->sHostIP.c_str(), pInstance->nPort), wmLog::LOG_TEST_OK));
        m_pList->AddElement(pElement);
        pElement->Filter(255);
        m_pList->Refresh();

        m_nDiscovered++;
        m_plblDiscovering->SetLabel(wxString::Format(wxT("Discovering...\n%zu Found"), m_nDiscovered));
        m_plblDiscovering->Update();
    }
    wxLogDebug(wxT("Discover Done"));
}

void pnlRTP::OnSap(wxCommandEvent& event)
{
    DecodeSap(event.GetString());
}

void pnlRTP::DecodeSap(const wxString& sData)
{
    wxString sIpAddress = sData.BeforeFirst(wxT('\n'));
    wxString sSDP = sData.AfterFirst(wxT('\n'));
    wxString sName;
    wmLog::Get()->Log(wxT("OnSAP"));

    //is it an L24 or L16 session
    bool bCanDecode(false);
    wxArrayString asLines(wxStringTokenize(sSDP, wxT("\n")));
    for(size_t i = 0; i < asLines.size(); i++)
    {
        if(asLines[i].find(wxT("a=rtpmap:")) != wxNOT_FOUND)
        {
            unsigned long nCodec;
            if(asLines[i].AfterFirst(wxT(':')).BeforeFirst(wxT(' ')).ToULong(&nCodec) && nCodec > 95 && nCodec < 127)
            {
                if(asLines[i].find(wxT("L24")) != wxNOT_FOUND || asLines[i].find(wxT("L16")) != wxNOT_FOUND)
                {
                    bCanDecode = true;
                    break;
                }
            }
        }
    }
    if(bCanDecode)
    {
        //find the source name:
        int nStart = sSDP.Find(wxT("s="));
        if(nStart != wxNOT_FOUND)
        {
            sName = sSDP.Mid(nStart+2).BeforeFirst(wxT('\n'));
        }
        nStart = sSDP.Find(wxT("o="));
        if(nStart != wxNOT_FOUND)
        {
            wxArrayString asSplit(wxStringTokenize(sSDP.Mid(nStart+2).BeforeFirst(wxT('\n'))));
            if(asSplit.size() >= 6)
            {
                sIpAddress = asSplit[5];
            }
        }

        if(m_setDiscover.insert(make_pair(sName, sIpAddress)).second)
        {
            m_nDiscovered++;
            m_plblDiscovering->SetLabel(wxString::Format(wxT("Discovering...\n%uz Found"), m_nDiscovered));
            wmLog::Get()->Log(wxString::Format(wxT("SAP response from %s\n%s"), sIpAddress.c_str(), sSDP.c_str()));

            AoipSourceManager::Get().AddSource(sName, wxString::Format(wxT("sap:%s"), sIpAddress.c_str()), sSDP);

            wxClientDC dc(this);
            dc.SetFont(m_pList->GetFont());
            LogElement* pElement(new LogElement(dc, GetClientSize().x, wxString::Format(wxT("[SAP] %s = s"), sName.c_str(), sIpAddress.c_str()), wmLog::LOG_TEST_OK));
            m_pList->AddElement(pElement);
            pElement->Filter(255);
            m_pList->Refresh();

        }
    }
}

void pnlRTP::OnDiscoveryFinished(wxCommandEvent& event)
{

    m_plblDiscovering->SetLabel(wxT("DNS_SD Discovery finished..."));
}

void pnlRTP::OnbtnDeleteAllHeld(wxCommandEvent& event)
{
    Settings::Get().Write("Input", "AoIP",0);

    AoipSourceManager::Get().DeleteAllSources();
    m_nSelectedSource = 0;
    ListSources();
    m_pbtnDelete->Disable();
    m_pbtnUpdate->Disable();
}


void pnlRTP::GetSDP(const wxString& sUrl)
{
    m_queueUrl.push(sUrl);
//    GetSDP();
}

void pnlRTP::GetSDP()
{
//    if(m_pThread == 0 && m_queueUrl.empty() == false)
//    {
//        m_pThread = new RtpThread(this, Settings::Get().Read(wxT("AoIP"), wxT("Interface"), wxEmptyString), wxEmptyString, m_queueUrl.front(), 4096, true);
//        m_pThread->Create();
//        m_pThread->Run();
//    }
}

void pnlRTP::OnSDPReceived(wxCommandEvent& event)
{
    wxString sSDP(event.GetString());
    sSDP.Replace(wxT("\r"), wxEmptyString);
    sSDP.Replace(wxT("\n"), wxT("`"));

    Settings::Get().Write(wxT("SDP"), m_queueUrl.front(), sSDP);
    //wmLog::Get()->Log(wxString::Format(wxT("'%s' = '%s'"), m_queueUrl.front().c_str(), event.GetString().c_str()));

    m_queueUrl.pop();
}

void pnlRTP::OnRTPClosed(wxCommandEvent& event)
{
    //wmLog::Get()->Log(wxT("---------------------CLOSED-------------------"));
    m_pThread = NULL;
    GetSDP();
}

void pnlRTP::OnlstServicesSelected(wxCommandEvent& event)
{

}

void pnlRTP::OnbtnSAPClick(wxCommandEvent& event)
{
    Settings::Get().Write(wxT("Discovery"), wxT("SAP"),event.IsChecked());
}

void pnlRTP::OnbtnStartDiscoveryClick(wxCommandEvent& event)
{
    m_plstServices->Enable(!event.IsChecked());
    m_pbtnSAP->Enable(!event.IsChecked());
    m_pbtnManual->Enable(!event.IsChecked());

    if(event.IsChecked())
    {
        set<wxString> setServices;
        if(m_plstServices->IsSelected(0))   //RTSP
        {
            setServices.insert(wxT("_rtsp._tcp"));
        }
        if(m_plstServices->IsSelected(1))   //SIP
        {
            setServices.insert(wxT("_sipuri._udp"));
        }
        if(m_plstServices->IsSelected(2))   //NMOS
        {
            setServices.insert(wxT("_nmos-query._tcp"));
            setServices.insert(wxT("_nmos-node._tcp"));
        }

        for(int i = 0; i < m_plstServices->GetItemCount(); i++)
        {
            Settings::Get().Write(wxT("Discovery"), m_plstServices->GetButtonText(i), m_plstServices->IsSelected(i));
        }

        m_setDiscover.clear();
        m_pList->Clear();

        wxClientDC dc(this);
        dc.SetFont(m_pList->GetFont());
        LogElement* pElement(new LogElement(dc, GetClientSize().x, wxT("Discovery started"), 1));
        m_pList->AddElement(pElement);
        pElement->Filter(255);
        m_pList->Refresh();
        m_pList->Update();

        m_nDiscovered = 0;
        m_plblDiscovering->SetLabel(wxString::Format(wxT("Discovering...\n%04d Found"), m_nDiscovered));
        m_pbtnDiscover->SetLabel(wxT("Stop Discovery"));

        if(m_pBrowser)
        {
            delete m_pBrowser;
            m_pBrowser = 0;
        }
        m_pBrowser = new DNSServiceBrowser(this);
        m_pBrowser->Start(setServices);

        if(m_pSapWatch)
        {
            m_pSapWatch->Delete();
            m_pSapWatch = 0;
        }
        // @todo work out how to make the sapwatchthread non-blocking then we can use this....
        if(m_pbtnSAP->IsChecked())
        {
            m_pSapWatch = new SapWatchThread(this);
            m_pSapWatch->Run();
        }
    }
    else
    {
        wxClientDC dc(this);
        dc.SetFont(m_pList->GetFont());
        LogElement* pElement(new LogElement(dc, GetClientSize().x, wxT("Discovery stopped"), 1));
        m_pList->AddElement(pElement);
        pElement->Filter(255);
        m_pList->Refresh();


        m_pbtnDiscover->SetLabel(wxT("Discover"));
        m_plblDiscovering->SetLabel(wxEmptyString);
        if(m_pBrowser)
        {
            delete m_pBrowser;
            m_pBrowser = 0;
        }
        if(m_pSapWatch)
        {
            m_pSapWatch->Delete();
            m_pSapWatch = 0;
        }
    }
}

void pnlRTP::OnbtnManualClick(wxCommandEvent& event)
{
    m_pSwp1->ChangeSelection(wxT("Manual"));
    ListSources();
}

void pnlRTP::OnbtnImportClick(wxCommandEvent& event)
{
    m_pSwp1->ChangeSelection(wxT("Import"));
    m_pnlUSB->StartCheck();
    #ifdef __WXMSW__
    //ImportSources(wxString::Format("%s/import.pii", Settings::Get().GetDocumentDirectory().c_str()));
    #endif // __WXMSW__
}


void pnlRTP::OnlstImportFilesSelected(wxCommandEvent& event)
{
    m_pbtnImportImport->Enable(m_plstFiles->GetSelectionCount() >0);
}

void pnlRTP::OnbtnImportImportClick(wxCommandEvent& event)
{
    m_plblImportProgress->SetLabel("Importing sources.");
    m_plblImportProgress->Update();

    wxArrayInt ai = m_plstFiles->GetSelectedButtons();
    for(size_t i = 0; i < ai.GetCount(); i++)
    {
        ImportSources(m_plstFiles->GetButtonText(ai[i]));
    }

    m_plblImportProgress->SetLabel("All sources imported.");
    m_plblImportProgress->Update();
}

void pnlRTP::OnSettingEvent(SettingEvent& event)
{
    if(event.GetSection() == "ImportAoIP" && event.GetKey() == "USB" && m_pSwp1->GetSelectionName() == "Import")
    {
        wxLogDebug(wxT("Reading USB drive"));
        m_plblImportProgress->SetLabel("Reading USB drive...");
        m_plblImportProgress->Update();

        m_plstFiles->Clear();
        m_plstFiles->Freeze();
        m_pbtnImportImport->Enable(false);

        wxString sPath;

        //mount the drive
        #ifdef __WXGNU__
        if(wxDirExists(wxT("/mnt/share")) == false)
        {
            wxMkdir(wxT("/mnt/share"));
        }
        wxExecute(wxT("sudo umount /mnt/share"), wxEXEC_SYNC);
        wxExecute(wxString::Format(wxT("sudo mount -o umask=000 /dev/%s /mnt/share"), Settings::Get().Read(wxT("ImportAoIP"), wxT("USB"), wxEmptyString)), wxEXEC_SYNC);

        sPath = "/mnt/share";
        #else
        sPath = Settings::Get().Read(wxT("ImportAoIP"), wxT("USB"), wxEmptyString);
        #endif // __WXGNU__

        wxDirTraverseUsb traverser(m_plblImportProgress, m_plstFiles);
        wxDir dir(sPath);
        dir.Traverse(traverser, "*.pii");



        m_plstFiles->Thaw();
        wxLogDebug(wxT("USB drive read."));
        m_plblImportProgress->SetLabel("USB drive read.");
        m_plblImportProgress->Update();
    }
}

void pnlRTP::ImportSources(const wxString& sFileName)
{
    m_plblImportProgress->SetLabel(wxString::Format("Reading '%s'", sFileName.c_str()));
    m_plblImportProgress->Update();
    iniManager ini;
    if(ini.ReadIniFile(sFileName))
    {
        const iniSection* pSection = ini.GetSection("Import");
        if(pSection)
        {
            for(auto itData = pSection->GetDataBegin(); itData != pSection->GetDataEnd(); ++itData)
            {
                if(itData->second.Left(3).CmpNoCase("sap") == 0)
                {
                    wxString sSDP(itData->second.AfterFirst('[').BeforeFirst(']'));
                    sSDP.Replace("|", "\n");
                    sSDP.Replace("`", "\n");
                    sSDP.Replace("\t", "");
                    sSDP.Trim();
                    AoipSourceManager::Get().AddSource(itData->first, itData->second.BeforeFirst('['), sSDP);
                }
                else if(itData->second.Left(4).CmpNoCase("rtsp") == 0)
                {
                    AoipSourceManager::Get().AddSource(itData->first, itData->second);
                }
            }
            wmLog::Get()->Log(wxString::Format("Import AoIP: Read '%s'", sFileName.c_str()));
        }
        else
        {
            wmLog::Get()->Log(wxString::Format("Import AoIP: Reading '%s' invalid file", sFileName.c_str()));
            m_plblImportProgress->SetLabel(wxString::Format("Reading '%s' invalid file", sFileName.c_str()));
            m_plblImportProgress->Update();
        }
    }
    else
    {
        wmLog::Get()->Log(wxString::Format("Import AoIP: Reading '%s' failed", sFileName.c_str()));
        m_plblImportProgress->SetLabel(wxString::Format("Reading '%s' failed", sFileName.c_str()));
        m_plblImportProgress->Update();
    }
}
