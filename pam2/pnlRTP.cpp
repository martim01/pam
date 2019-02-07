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


using namespace std;


//(*IdInit(pnlRTP)
const long pnlRTP::ID_M_PLST1 = wxNewId();
const long pnlRTP::ID_M_PBTN3 = wxNewId();
const long pnlRTP::ID_M_PBTN4 = wxNewId();
const long pnlRTP::ID_M_PBTN5 = wxNewId();
const long pnlRTP::ID_M_PBTN7 = wxNewId();
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
const long pnlRTP::ID_M_PSWP1 = wxNewId();
//*)

const wxString pnlRTP::STR_TABLE = wxT("<table border=\"1\" width=\"100%\"><tr><td width=\"20%\"><b>Service</b></td><td width=\"40%\"><b>Name</b></td><td width=\"40%\"><b>Host IP</b></td></tr>");

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
	m_pbtnDelete = new wmButton(Panel1, ID_M_PBTN5, _("Delete\nSelected"), wxPoint(220,390), wxSize(100,45), 0, wxDefaultValidator, _T("ID_M_PBTN5"));
	m_pbtnDelete->Disable();
	m_pbtnDelete->SetColourDisabled(wxColour(wxT("#909090")));
	m_pbtnDeleteAll = new wmButton(Panel1, ID_M_PBTN7, _("Hold To\nDelete All"), wxPoint(340,390), wxSize(100,45), wmButton::STYLE_HOLD, wxDefaultValidator, _T("ID_M_PBTN7"));
	m_pbtnDeleteAll->SetColourDisabled(wxColour(wxT("#909090")));
	m_pbtnDiscover = new wmButton(Panel1, ID_M_PBTN6, _("Discovery"), wxPoint(690,390), wxSize(100,45), wmButton::STYLE_SELECT, wxDefaultValidator, _T("ID_M_PBTN6"));
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
	m_plstServices = new wmList(pnlDiscovery, ID_M_PLST2, wxPoint(5,65), wxSize(130,160), wmList::STYLE_SELECT|wmList::STYLE_SELECT_MULTI, 0, wxSize(-1,40), 1, wxSize(1,5));
	m_plstServices->SetBackgroundColour(wxColour(0,0,0));
	m_pbtnManual = new wmButton(pnlDiscovery, ID_M_PBTN10, _("Manual"), wxPoint(690,390), wxSize(100,45), wmButton::STYLE_SELECT, wxDefaultValidator, _T("ID_M_PBTN10"));
	m_pbtnManual->SetBackgroundColour(wxColour(0,128,0));
	m_pbtnManual->SetColourSelected(wxColour(wxT("#F07800")));
	m_phtmlResults = new wxTouchScreenHtml(pnlDiscovery, ID_HTMLWINDOW1, wxPoint(140,0), wxSize(650,385), 0, _T("ID_HTMLWINDOW1"));
	m_pSwp1->AddPage(Panel1, _("Manual"), false);
	m_pSwp1->AddPage(Panel2, _("Edit"), false);
	m_pSwp1->AddPage(pnlDiscovery, _("Discovery"), false);

	Connect(ID_M_PLST1,wxEVT_LIST_SELECTED,(wxObjectEventFunction)&pnlRTP::OnlstSourcesSelected);
	Connect(ID_M_PBTN3,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&pnlRTP::OnbtnAddClick);
	Connect(ID_M_PBTN4,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&pnlRTP::OnbtnUpdateClick);
	Connect(ID_M_PBTN5,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&pnlRTP::OnbtnDeleteClick);
	Connect(ID_M_PBTN7,wxEVT_BUTTON_HELD,(wxObjectEventFunction)&pnlRTP::OnbtnDeleteAllHeld);
	Connect(ID_M_PBTN6,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&pnlRTP::OnbtnDiscoverClick);
	Connect(ID_M_PEDT1,wxEVT_COMMAND_TEXT_ENTER,(wxObjectEventFunction)&pnlRTP::OnedtNameTextEnter);
	Connect(ID_M_PEDT2,wxEVT_COMMAND_TEXT_ENTER,(wxObjectEventFunction)&pnlRTP::OnedtUrlTextEnter);
	Connect(ID_M_PBTN1,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&pnlRTP::OnbtnConfirmClick);
	Connect(ID_M_PBTN2,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&pnlRTP::OnbtnCancelClick);
	Connect(ID_M_PBTN8,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&pnlRTP::OnbtnSAPClick);
	Connect(ID_M_PBTN9,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&pnlRTP::OnbtnStartDiscoveryClick);
	Connect(ID_M_PLST2,wxEVT_LIST_SELECTED,(wxObjectEventFunction)&pnlRTP::OnlstServicesSelected);
	Connect(ID_M_PBTN10,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&pnlRTP::OnbtnManualClick);
	//*)

	Connect(wxID_ANY, wxEVT_SDP, (wxObjectEventFunction)&pnlRTP::OnSDPReceived);
	Connect(wxID_ANY, wxEVT_RTP_SESSION_CLOSED, (wxObjectEventFunction)&pnlRTP::OnRTPClosed);
	m_plstSources->SetGradient(0);
	m_plstSources->SetTextAlign(wxALIGN_CENTER_VERTICAL | wxALIGN_LEFT);

	m_pkeyboard->SetBlankString(wxT(":"));
    m_pkeyboard->SetReturnString(wxT("-->|"));
	SetSize(size);
	SetPosition(pos);

	m_plstServices->AddButton(wxT("RTSP"));
	//m_plstServices->AddButton(wxT("SIP"));
	//m_plstServices->AddButton(wxT("NMOS"));

	m_pBrowser = 0;
	m_pSapWatch = 0;
	Connect(wxID_ANY, wxEVT_BROWSE_RESOLVED, (wxObjectEventFunction)&pnlRTP::OnDiscovery);
	Connect(wxID_ANY, wxEVT_BROWSE_FINISHED, (wxObjectEventFunction)&pnlRTP::OnDiscoveryFinished);
	Connect(wxID_ANY, wxEVT_SAP, (wxObjectEventFunction)&pnlRTP::OnSap);

	m_phtmlResults->SetPage(STR_TABLE+m_sTableMiddle+wxT("</table>"));
	m_phtmlResults->End();

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
    m_pSwp1->ChangeSelection(1);
    m_pedtName->SetValue(m_sSelectedSource);
    m_pedtUrl->SetValue(m_sSelectedUrl);

    m_sSelectedSource = wxEmptyString;
    m_pedtName->SetFocus();
    m_pbtnConfirm->SetLabel(wxT("Add"));
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
    if(m_sSelectedSource != wxEmptyString)
    {
        Settings::Get().RemoveKey(wxT("AoIP"), m_sSelectedSource);
    }

    Settings::Get().Write(wxT("AoIP"), m_pedtName->GetValue(), m_pedtUrl->GetValue());

    ListSources();
    m_pSwp1->ChangeSelection(0);

    m_sSelectedSource = wxEmptyString;
    m_pbtnDelete->Disable();
    m_pbtnUpdate->Disable();

}

void pnlRTP::OnbtnUpdateClick(wxCommandEvent& event)
{
    m_pedtName->SetValue(m_sSelectedSource);
    m_pedtUrl->SetValue(m_sSelectedUrl);
    m_pSwp1->ChangeSelection(1);
    m_pedtName->SetFocus();

    m_pbtnConfirm->SetLabel(wxT("Update"));
}

void pnlRTP::OnbtnDeleteClick(wxCommandEvent& event)
{
    Settings::Get().RemoveKey(wxT("AoIP"), m_sSelectedSource);

    m_sSelectedSource = wxEmptyString;
    ListSources();
    m_pbtnDelete->Disable();
    m_pbtnUpdate->Disable();
}


void pnlRTP::ListSources()
{
    m_plstSources->Freeze();
    m_plstSources->Clear();

    map<wxString, wxString>::const_iterator itBegin, itEnd;
    if(Settings::Get().GetSectionDataBegin(wxT("AoIP"), itBegin) && Settings::Get().GetSectionDataEnd(wxT("AoIP"), itEnd))
    {
        for(map<wxString, wxString>::const_iterator itSource = itBegin; itSource != itEnd; ++itSource)
        {
            m_plstSources->AddButton(itSource->first);
            m_plstSources->AddButton(itSource->second.BeforeFirst(wxT('[')));
        }
    }
    m_plstSources->Thaw();
}

void pnlRTP::OnlstSourcesSelected(wxCommandEvent& event)
{
    m_sSelectedSource = m_plstSources->GetButtonText(2*(event.GetInt()/2));
    m_sSelectedUrl = m_plstSources->GetButtonText(2*(event.GetInt()/2)+1);

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
}


void pnlRTP::OnDiscovery(wxCommandEvent& event)
{
    dnsInstance* pInstance = reinterpret_cast<dnsInstance*>(event.GetClientData());
    if(m_setDiscover.insert(make_pair(pInstance->sName, pInstance->sHostIP)).second)
    {
        wxString sAddress;
        if(pInstance->nPort == 554)
        {
            sAddress = (wxString::Format(wxT("rtsp://%s/by-name/%s"), pInstance->sHostIP.c_str(), pInstance->sName.c_str()));
        }
        else
        {
            sAddress = (wxString::Format(wxT("rtsp://%s:%d/by-name/%s"), pInstance->sHostIP.c_str(), pInstance->nPort, pInstance->sName.c_str()));
        }
       // GetSDP(sAddress);

        Settings::Get().Write(wxT("AoIP"), wxString::Format(wxT("%s(%s)"), pInstance->sName.BeforeFirst(wxT('@')).c_str(), pInstance->sHostIP.c_str()), sAddress);

        m_sTableMiddle << (wxString::Format(wxT("<tr><td>%s</td><td>%s</td><td>%s:%d</td></tr>"), pInstance->sService.c_str(), pInstance->sName.c_str(), pInstance->sHostIP.c_str(), pInstance->nPort));

        m_phtmlResults->SetPage(STR_TABLE+m_sTableMiddle+wxT("</table>"));
        m_phtmlResults->End();


        m_nDiscovered++;
        m_plblDiscovering->SetLabel(wxString::Format(wxT("Discovering...\n%04d Found"), m_nDiscovered));
        m_plblDiscovering->Update();
    }
    wxLogDebug(wxT("Discover Done"));
}

void pnlRTP::OnSap(wxCommandEvent& event)
{
    wxString sIpAddress = event.GetString().BeforeFirst(wxT('\n'));
    wxString sSDP = event.GetString().AfterFirst(wxT('\n'));
    wxString sName;

    //is it an L24 or L16 session
    if(sSDP.Find(wxT("a=rtpmap:96 L24")) != wxNOT_FOUND || sSDP.Find(wxT("a=rtpmap:96 L16")) != wxNOT_FOUND)
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
            m_plblDiscovering->SetLabel(wxString::Format(wxT("Discovering...\n%04d Found"), m_nDiscovered));
            wmLog::Get()->Log(wxString::Format(wxT("SAP response from %s\n%s"), sIpAddress.c_str(), sSDP.c_str()));
            sSDP.Replace(wxT("\n"), wxT("`"));
            Settings::Get().Write(wxT("AoIP"), sName, wxString::Format(wxT("sap:%s [%s]"), sIpAddress.c_str(), sSDP.c_str()));

            m_sTableMiddle << (wxString::Format(wxT("<tr><td>SAP</td><td>%s</td><td>%s</td></tr>"), sName.c_str(), sIpAddress.c_str()));

            m_phtmlResults->SetPage(STR_TABLE+m_sTableMiddle+wxT("</table>"));
            m_phtmlResults->End();
        }
    }
}

void pnlRTP::OnDiscoveryFinished(wxCommandEvent& event)
{
    wxLogDebug(wxT("Discover Finished"));
    m_plblDiscovering->SetLabel(wxString::Format(wxT("DNS_SD Discovery finished...\n%04d Found"), m_nDiscovered));
}

void pnlRTP::OnbtnDeleteAllHeld(wxCommandEvent& event)
{
    // @todo Delete all RTP Sources
    Settings::Get().RemoveSection(wxT("AoIP"));
    m_sSelectedSource = wxEmptyString;
    ListSources();
    m_pbtnDelete->Disable();
    m_pbtnUpdate->Disable();
}


void pnlRTP::GetSDP(const wxString& sUrl)
{
    m_queueUrl.push(sUrl);
    GetSDP();
}

void pnlRTP::GetSDP()
{
    if(m_pThread == 0 && m_queueUrl.empty() == false)
    {
        m_pThread = new RtpThread(this, Settings::Get().Read(wxT("AoIP"), wxT("Interface"), wxEmptyString), wxEmptyString, m_queueUrl.front(), 4096, true);
        m_pThread->Create();
        m_pThread->Run();
    }
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
        m_sTableMiddle = wxEmptyString;
        m_phtmlResults->SetPage(STR_TABLE+m_sTableMiddle+wxT("</table>"));
        m_phtmlResults->End();

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
