#include "pnlRTP.h"

//(*InternalHeaders(pnlRTP)
#include <wx/font.h>
#include <wx/intl.h>
#include <wx/string.h>
//*)

#include "settings.h"
#include <wx/log.h>
#include <wx/tokenzr.h>
#include "settings.h"
#include "log.h"
#include "PamUsageEnvironment.h"
#include "PamTaskScheduler.h"
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
const long pnlRTP::ID_M_PLBL11 = wxNewId();
const long pnlRTP::ID_M_PLST4 = wxNewId();
const long pnlRTP::ID_M_PBTN10 = wxNewId();
const long pnlRTP::ID_HTMLWINDOW1 = wxNewId();
const long pnlRTP::ID_PANEL3 = wxNewId();
const long pnlRTP::ID_PANEL5 = wxNewId();
const long pnlRTP::ID_M_PBTN13 = wxNewId();
const long pnlRTP::ID_M_PLBL10 = wxNewId();
const long pnlRTP::ID_PANEL4 = wxNewId();
const long pnlRTP::ID_M_PSWP1 = wxNewId();
//*)

const std::string pnlRTP::STR_SAP[3] = {"239.255.255.255", "239.195.255.255", "224.2.127.254"};
const wxString pnlRTP::STR_SAP_SETTING[3] = {"SAP_LOCAL", "SAP_ORGANISATION", "SAP_GLOBAL"};

BEGIN_EVENT_TABLE(pnlRTP,wxPanel)
	//(*EventTable(pnlRTP)
	//*)
END_EVENT_TABLE()

pnlRTP::pnlRTP(wxWindow* parent,wxWindowID id,const wxPoint& pos,const wxSize& size, long nStyle, const wxString& sId)
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
    m_pbtnLivewire = new wmButton(pnlDiscovery, ID_M_PBTN8, _("Livewire+"), wxPoint(5,390), wxSize(130,45), wmButton::STYLE_SELECT, wxDefaultValidator, _T("ID_M_PBTN8"));
    m_pbtnLivewire->SetForegroundColour(wxColour(255,255,255));
    m_pbtnLivewire->SetBackgroundColour(wxColour(0,0,160));
    m_pbtnLivewire->SetToggle(true, wxT("N"), wxT("Y"), 60);
    m_pbtnStartDiscovery = new wmButton(pnlDiscovery, ID_M_PBTN9, _("Discovery"), wxPoint(140,390), wxSize(200,45), wmButton::STYLE_SELECT, wxDefaultValidator, _T("ID_M_PBTN9"));
    m_pbtnStartDiscovery->SetForegroundColour(wxColour(255,255,255));
    m_pbtnStartDiscovery->SetBackgroundColour(wxColour(0,0,160));
    m_pbtnStartDiscovery->SetToggle(true, wxT("Stop"), wxT("Start"), 50);
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
    m_plstServices = new wmList(pnlDiscovery, ID_M_PLST2, wxPoint(5,65), wxSize(130,150), wmList::STYLE_SELECT|wmList::STYLE_SELECT_MULTI, 0, wxSize(-1,-1), 1, wxSize(1,5));
    m_plstServices->SetBackgroundColour(wxColour(0,0,0));
    m_pLbl9 = new wmLabel(pnlDiscovery, ID_M_PLBL11, _("SAP"), wxPoint(5,220), wxSize(130,30), 0, _T("ID_M_PLBL11"));
    m_pLbl9->SetBorderState(uiRect::BORDER_NONE);
    m_pLbl9->GetUiRect().SetGradient(0);
    m_pLbl9->SetForegroundColour(wxColour(255,255,255));
    m_pLbl9->SetBackgroundColour(wxColour(0,0,64));
    m_plstSAP = new wmList(pnlDiscovery, ID_M_PLST4, wxPoint(5,250), wxSize(130,135), wmList::STYLE_SELECT|wmList::STYLE_SELECT_MULTI, 0, wxSize(-1,-1), 1, wxSize(1,5));
    m_plstSAP->SetBackgroundColour(wxColour(0,0,0));
    m_pbtnManual = new wmButton(pnlDiscovery, ID_M_PBTN10, _("Back"), wxPoint(690,390), wxSize(100,45), 0, wxDefaultValidator, _T("ID_M_PBTN10"));
    m_pbtnManual->SetBackgroundColour(wxColour(0,128,0));
    m_pbtnManual->SetColourSelected(wxColour(wxT("#F07800")));
    m_phtmlResults = new wxTouchScreenHtml(pnlDiscovery, ID_HTMLWINDOW1, wxPoint(140,0), wxSize(650,385), 0, _T("ID_HTMLWINDOW1"));
    Panel3 = new wxPanel(m_pSwp1, ID_PANEL4, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL, _T("ID_PANEL4"));
    m_pnlUSB = new pnlUSB(Panel3, "*.pii", "Import", true, ID_PANEL5, wxPoint(0,0), wxSize(600,400), wxTAB_TRAVERSAL, _T("ID_PANEL5"));
    m_pnlUSB->SetBackgroundColour(wxColour(0,0,0));
    m_pbtnImportBack = new wmButton(Panel3, ID_M_PBTN13, _("Back"), wxPoint(690,390), wxSize(100,45), 0, wxDefaultValidator, _T("ID_M_PBTN13"));
    m_pbtnImportBack->SetBackgroundColour(wxColour(146,50,252));
    m_pbtnImportBack->SetColourSelected(wxColour(wxT("#F07800")));
    m_plblImportProgress = new wmLabel(Panel3, ID_M_PLBL10, wxEmptyString, wxPoint(610,10), wxSize(180,80), 0, _T("ID_M_PLBL10"));
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
    Connect(ID_M_PBTN9,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&pnlRTP::OnbtnStartDiscoveryClick);
    Connect(ID_M_PLST2,wxEVT_LIST_SELECTED,(wxObjectEventFunction)&pnlRTP::OnlstServicesSelected);
    Connect(ID_M_PLST4,wxEVT_LIST_SELECTED,(wxObjectEventFunction)&pnlRTP::Onm_plstSAPSelected);
    Connect(ID_M_PBTN10,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&pnlRTP::OnbtnManualClick);
    Connect(ID_M_PBTN13,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&pnlRTP::OnbtnManualClick);
    //*)

    m_pList = new wmListAdv(pnlDiscovery, wxNewId(), wxPoint(140,0), wxSize(650,385),0, wmListAdv::SCROLL_VERTICAL, wxSize(-1,30), 1, wxSize(0,1));
    m_pList->SetFont(wxFont(8,wxFONTFAMILY_SWISS, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD, false, "Arial", wxFONTENCODING_DEFAULT));
    m_pList->SetBackgroundColour(wxColour(100,100,100));

	m_plstSources->SetGradient(0);
	m_plstSources->SetTextAlign(wxALIGN_CENTER_VERTICAL | wxALIGN_LEFT);

	m_pkeyboard->SetBlankString(wxT(":"));
    m_pkeyboard->SetReturnString(wxT("-->|"));
	SetSize(size);
	SetPosition(pos);
    m_pnlUSB->SetSection(wxT("ImportAoIP"));
	m_plstServices->AddButton(wxT("RTSP"));
	//m_plstServices->AddButton(wxT("SIP"));
	//m_plstServices->AddButton(wxT("NMOS"));

    Panel3->SetBackgroundColour(*wxBLACK);

    AoipSourceManager::Get();

    Connect(m_pnlUSB->m_pbtnUpload->GetId(),wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&pnlRTP::OnbtnImportImportClick);

	Connect(wxID_ANY, wxEVT_ASM_DISCOVERY, (wxObjectEventFunction)&pnlRTP::OnDiscovery);
	Connect(wxID_ANY, wxEVT_ASM_DISCOVERY_FINISHED, (wxObjectEventFunction)&pnlRTP::OnDiscoveryFinished);

    m_nSelectedSource = 0;

	Settings::Get().AddHandler(this, wxT("ImportAoIP"), wxT("USB"));
	Connect(wxID_ANY, wxEVT_SETTING_CHANGED, (wxObjectEventFunction)&pnlRTP::OnSettingEvent);

	if(Settings::Get().Read(wxT("Discovery"), wxT("RTSP"),1) == 1)
    {
        m_plstServices->SelectButton(wxT("RTSP"), false);
    }
//    if(Settings::Get().Read(wxT("Discovery"), wxT("SIP"),0) == 1)
//    {
//        m_plstServices->SelectButton(wxT("SIP"), false);
//    }
//    if(Settings::Get().Read(wxT("Discovery"), wxT("NMOS"),0) == 1)
//    {
//        m_plstServices->SelectButton(wxT("NMOS"), false);
//    }

    int nIndex = m_plstSAP->AddButton("Local\n239.255.255.255");
    if(Settings::Get().Read("Discovery", STR_SAP_SETTING[LOCAL] ,0) == 1)
    {
        m_plstSAP->SelectButton(nIndex, false);
    }

    nIndex = m_plstSAP->AddButton("Organisation\n239.195.255255");
    if(Settings::Get().Read("Discovery", STR_SAP_SETTING[ORGANISATION] ,0) == 1)
    {
        m_plstSAP->SelectButton(nIndex, false);
    }
    nIndex = m_plstSAP->AddButton("Global\n224.2.127.254");
    if(Settings::Get().Read("Discovery", STR_SAP_SETTING[GLOBAL] ,0) == 1)
    {
        m_plstSAP->SelectButton(nIndex, false);
    }

    m_pbtnLivewire->ConnectToSetting("Discovery", "Livewire", false);


	ListSources();
}

pnlRTP::~pnlRTP()
{
	//(*Destroy(pnlRTP)
	//*)
    Settings::Get().RemoveHandler(this);
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
    if(source.sDetails.empty() == false)
    {
        m_pedtUrl->SetValue(source.sDetails);
        m_pedtUrl->Enable(true);
    }
    else
    {
        m_pedtUrl->SetValue("SDP");
        m_pedtUrl->Enable(false);
    }

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

    for(auto pairSource : AoipSourceManager::Get().GetSources())
    {
        if(pairSource.first > 0)
        {
            m_plstSources->AddButton(pairSource.second.sName, wxNullBitmap, (void*)pairSource.first);
            if(pairSource.second.sDetails.empty() == false)
            {
                m_plstSources->AddButton(pairSource.second.sDetails, wxNullBitmap, (void*)pairSource.first);
            }
            else
            {
                m_plstSources->AddButton("SDP", wxNullBitmap, (void*)pairSource.first);
            }
        }
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
    //m_pnlUSB->StopCheck();
}


void pnlRTP::OnDiscovery(wxCommandEvent& event)
{

    wxClientDC dc(this);
    dc.SetFont(m_pList->GetFont());
    m_pList->AddElement(std::make_shared<LogElement>(dc, GetClientSize().x, event.GetString(), 8), true, nullptr);
    m_pList->Refresh();

    m_nDiscovered+= event.GetInt();
    m_plblDiscovering->SetLabel(wxString::Format(wxT("Discovering...\n%lu Found"), m_nDiscovered));
    m_plblDiscovering->Update();
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
    m_plstSAP->Enable(!event.IsChecked());


    m_pbtnManual->Enable(!event.IsChecked());
    m_pbtnDiscover->Update();

    std::set<std::string> setServices;
    std::set<std::string> setSAP;

    if(event.IsChecked())
    {
        if(m_plstServices->IsSelected(0))   //RTSP
        {
            setServices.insert("_rtsp._tcp");
        }
        if(m_plstServices->IsSelected(1))   //SIP
        {
            setServices.insert("_sipuri._udp");
        }
        if(m_plstServices->IsSelected(2))   //NMOS
        {
            setServices.insert("_nmos-query._tcp");
            setServices.insert("_nmos-node._tcp");
        }

        for(int i = 0; i < m_plstServices->GetItemCount(); i++)
        {
            Settings::Get().Write(wxT("Discovery"), m_plstServices->GetButtonText(i), m_plstServices->IsSelected(i));
        }

        for(int i = 0; i < 3; i++)
        {
            if(m_plstSAP->IsSelected(i))
            {
                setSAP.insert(STR_SAP[i]);
            }
        }


        AoipSourceManager::Get().StartDiscovery(this, setServices, setSAP, Settings::Get().Read("Discovery", "Livewire", false));
        m_pList->Clear();


        wxClientDC dc(this);
        dc.SetFont(m_pList->GetFont());
        m_pList->AddElement(std::make_shared<LogElement>(dc, GetClientSize().x, "Discovery started", 1), true, nullptr);
        m_pList->Refresh();
        m_pList->Update();

        m_nDiscovered = 0;
        m_plblDiscovering->SetLabel(wxString::Format(wxT("Discovering...\n%lu Found"), m_nDiscovered));
        m_pbtnDiscover->SetLabel(wxT("Stop Discovery"));

    }
    else
    {
        wxClientDC dc(this);
        dc.SetFont(m_pList->GetFont());
        m_pList->AddElement(std::make_shared<LogElement>(dc, GetClientSize().x, "Discovery stopped", 1), true, nullptr);
        m_pList->Refresh();


        m_pbtnDiscover->SetLabel(wxT("Discover"));
        m_plblDiscovering->SetLabel(wxEmptyString);

        AoipSourceManager::Get().StopDiscovery();
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

}

void pnlRTP::OnbtnImportImportClick(wxCommandEvent& event)
{
    UsbChecker::UnmountDevice();

    m_pnlUSB->Log("Importing sources.");

    wxString sDevice;
    wxArrayInt ai = m_pnlUSB->m_plstFiles->GetSelectedButtons();
    for(size_t i = 0; i < ai.GetCount(); i++)
    {
        wxString sNextDevice = m_pnlUSB->GetDevice(m_pnlUSB->m_plstFiles->GetButtonText(ai[i]));
        if(sNextDevice.empty() == false)
        {
            if(sNextDevice != sDevice)
            {
                UsbChecker::UnmountDevice();
                if(UsbChecker::MountDevice(sNextDevice) != 0)
                {
                    sDevice = "";
                    m_pnlUSB->Log("Failed to mount "+sNextDevice);
                    m_pnlUSB->Log(strerror(errno));
                }
                else
                {
                    sDevice = sNextDevice;
                }
            }

            if(sDevice.empty() == false)
            {
                ImportSources("/mnt/share/"+m_pnlUSB->m_plstFiles->GetButtonText(ai[i]));
            }
        }

    }

    m_pnlUSB->Log("All sources imported.");
}

void pnlRTP::OnSettingEvent(SettingEvent& event)
{
    if(event.GetSection() == "ImportAoIP" && event.GetKey() == "USB" && m_pSwp1->GetSelectionName() == "Import")
    {
        m_pnlUSB->StartCheck();
    }
}

void pnlRTP::ImportSources(const wxString& sFileName)
{
    m_pnlUSB->Log(wxString::Format("Reading '%s'", sFileName.c_str()));

    iniManager ini;
    if(ini.ReadIniFile(sFileName))
    {
        auto pSection = ini.GetSection("Import");
        if(pSection)
        {
            for(auto itData = pSection->GetDataBegin(); itData != pSection->GetDataEnd(); ++itData)
            {
                if(itData->second.Left(3).CmpNoCase("sap") == 0)
                {
                    wxString sSDP(itData->second.AfterFirst('[').BeforeFirst(']'));
                    sSDP.Replace("|", "\n");
                    sSDP.Replace("'", "\n");
                    sSDP.Replace("`", "\n");
                    sSDP.Replace("\t", "");
                    sSDP.Trim();
                    pmlLog(pml::LOG_INFO, "pam::aoip") << "Import: " << sSDP;
                    AoipSourceManager::Get().AddSource(itData->first, itData->second.BeforeFirst('['), sSDP);
                }
                else if(itData->second.Left(4).CmpNoCase("rtsp") == 0)
                {
                    AoipSourceManager::Get().AddSource(itData->first, itData->second);
                }
            }
            pmlLog(pml::LOG_INFO, "pam::aoip") << "Import AoIP: Read '" << sFileName << "'";
        }
        else
        {
            pmlLog(pml::LOG_ERROR, "pam::aoip") << "Import AoIP: Reading '" << sFileName << "' invalid file";
            m_pnlUSB->Log(wxString::Format("Reading '%s' invalid file", sFileName.c_str()));
        }
    }
    else
    {
        pmlLog(pml::LOG_ERROR, "pam::aoip") << "Import AoIP: Reading '" << sFileName << "' failed";
        m_pnlUSB->Log(wxString::Format("Reading '%s' failed", sFileName.c_str()));
    }
}

void pnlRTP::Onm_plstSAPSelected(wxCommandEvent& event)
{
    if(event.GetInt() >= 0 && event.GetInt() < 3)
    {
        Settings::Get().Write("Discovery", STR_SAP_SETTING[event.GetInt()], m_plstSAP->IsSelected(event.GetInt()));
    }
}