#include "pnlNetworkSetup.h"
#include <wx/log.h>
#include "networkcontrol.h"
#include "dlgmask.h"
#include "dlgWiFi.h"
using namespace std;

//(*InternalHeaders(pnlNetworkSetup)
#include <wx/font.h>
#include <wx/intl.h>
#include <wx/string.h>
//*)

//(*IdInit(pnlNetworkSetup)
const long pnlNetworkSetup::ID_M_PBTN1 = wxNewId();
const long pnlNetworkSetup::ID_M_PLBL6 = wxNewId();
const long pnlNetworkSetup::ID_M_PLBL7 = wxNewId();
const long pnlNetworkSetup::ID_M_PLBL8 = wxNewId();
const long pnlNetworkSetup::ID_M_PLBL1 = wxNewId();
const long pnlNetworkSetup::ID_M_PLBL2 = wxNewId();
const long pnlNetworkSetup::ID_M_PLBL4 = wxNewId();
const long pnlNetworkSetup::ID_PANEL3 = wxNewId();
const long pnlNetworkSetup::ID_M_PKBD1 = wxNewId();
const long pnlNetworkSetup::ID_M_PLBL3 = wxNewId();
const long pnlNetworkSetup::ID_M_PBTN3 = wxNewId();
const long pnlNetworkSetup::ID_M_PLBL5 = wxNewId();
const long pnlNetworkSetup::ID_M_PBTN2 = wxNewId();
const long pnlNetworkSetup::ID_PANEL1 = wxNewId();
const long pnlNetworkSetup::ID_M_PSWP1 = wxNewId();
const long pnlNetworkSetup::ID_PANEL2 = wxNewId();
const long pnlNetworkSetup::ID_M_PLST1 = wxNewId();
//*)

BEGIN_EVENT_TABLE(pnlNetworkSetup,wxPanel)
	//(*EventTable(pnlNetworkSetup)
	//*)
END_EVENT_TABLE()

pnlNetworkSetup::pnlNetworkSetup(wxWindow* parent,wxWindowID id,const wxPoint& pos,const wxSize& size, long nStyle, const wxString& sEmpty)
{
	//(*Initialize(pnlNetworkSetup)
	Create(parent, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL, _T("wxID_ANY"));
	SetBackgroundColour(wxColour(0,0,0));
	m_pbtnStaticDHCP = new wmButton(this, ID_M_PBTN1, _("Type"), wxPoint(350,85), wxSize(200,40), wmButton::STYLE_SELECT, wxDefaultValidator, _T("ID_M_PBTN1"));
	m_pbtnStaticDHCP->SetBackgroundColour(wxColour(0,128,192));
	m_plblConnected = new wmLabel(this, ID_M_PLBL6, _("Connected"), wxPoint(350,130), wxSize(200,40), 0, _T("ID_M_PLBL6"));
	m_plblConnected->SetBorderState(uiRect::BORDER_NONE);
	m_plblConnected->GetUiRect().SetGradient(0);
	m_plblConnected->SetForegroundColour(wxColour(255,255,255));
	m_plblConnected->SetBackgroundColour(wxColour(0,128,0));
	m_pLbl5 = new wmLabel(this, ID_M_PLBL7, _("Type"), wxPoint(350,175), wxSize(60,40), 0, _T("ID_M_PLBL7"));
	m_pLbl5->SetBorderState(uiRect::BORDER_NONE);
	m_pLbl5->GetUiRect().SetGradient(0);
	m_pLbl5->SetForegroundColour(wxColour(255,255,255));
	m_pLbl5->SetBackgroundColour(wxColour(64,0,128));
	m_plblType = new wmLabel(this, ID_M_PLBL8, wxEmptyString, wxPoint(410,175), wxSize(140,40), 0, _T("ID_M_PLBL8"));
	m_plblType->SetBorderState(uiRect::BORDER_NONE);
	m_plblType->GetUiRect().SetGradient(0);
	m_plblType->SetForegroundColour(wxColour(0,0,0));
	m_plblType->SetBackgroundColour(wxColour(255,255,255));
	m_pLbl1 = new wmLabel(this, ID_M_PLBL1, _("IP Address"), wxPoint(0,85), wxSize(100,40), 0, _T("ID_M_PLBL1"));
	m_pLbl1->SetBorderState(uiRect::BORDER_NONE);
	m_pLbl1->GetUiRect().SetGradient(0);
	m_pLbl1->SetForegroundColour(wxColour(255,255,255));
	m_pLbl1->SetBackgroundColour(wxColour(64,0,128));
	m_pLbl2 = new wmLabel(this, ID_M_PLBL2, _("Subnet Mask"), wxPoint(0,130), wxSize(100,40), 0, _T("ID_M_PLBL2"));
	m_pLbl2->SetBorderState(uiRect::BORDER_NONE);
	m_pLbl2->GetUiRect().SetGradient(0);
	m_pLbl2->SetForegroundColour(wxColour(255,255,255));
	m_pLbl2->SetBackgroundColour(wxColour(64,0,128));
	m_pLbl4 = new wmLabel(this, ID_M_PLBL4, _("Gateway"), wxPoint(0,175), wxSize(100,40), 0, _T("ID_M_PLBL4"));
	m_pLbl4->SetBorderState(uiRect::BORDER_NONE);
	m_pLbl4->GetUiRect().SetGradient(0);
	m_pLbl4->SetForegroundColour(wxColour(255,255,255));
	m_pLbl4->SetBackgroundColour(wxColour(64,0,128));
	m_ppnlGateway = new wmipeditpnl(this, ID_PANEL3, wxPoint(100,175), wxSize(200,40), wxTAB_TRAVERSAL, _T("ID_PANEL3"));
	m_ppnlGateway->SetForegroundColour(wxColour(0,0,0));
	m_ppnlGateway->SetBackgroundColour(wxColour(255,255,255));
	m_pkbd = new wmKeyboard(this, ID_M_PKBD1, wxPoint(10,230), wxSize(240,200), 5, 0);
	m_pkbd->SetForegroundColour(wxColour(255,255,255));
	wxFont m_pkbdFont(10,wxFONTFAMILY_SWISS,wxFONTSTYLE_NORMAL,wxFONTWEIGHT_BOLD,false,_T("Arial"),wxFONTENCODING_DEFAULT);
	m_pkbd->SetFont(m_pkbdFont);
	m_pLbl3 = new wmLabel(this, ID_M_PLBL3, _("Manage Network Settings"), wxPoint(0,5), wxSize(600,30), 0, _T("ID_M_PLBL3"));
	m_pLbl3->SetBorderState(uiRect::BORDER_NONE);
	m_pLbl3->GetUiRect().SetGradient(0);
	m_pLbl3->SetForegroundColour(wxColour(255,255,255));
	m_pLbl3->SetBackgroundColour(wxColour(0,64,0));
	wxFont m_pLbl3Font(10,wxFONTFAMILY_SWISS,wxFONTSTYLE_NORMAL,wxFONTWEIGHT_BOLD,false,_T("Arial"),wxFONTENCODING_DEFAULT);
	m_pLbl3->SetFont(m_pLbl3Font);
	m_pbtnMask = new wmButton(this, ID_M_PBTN3, wxEmptyString, wxPoint(100,130), wxSize(200,40), 0, wxDefaultValidator, _T("ID_M_PBTN3"));
	m_pbtnMask->SetForegroundColour(wxColour(0,0,0));
	m_pbtnMask->SetBackgroundColour(wxColour(255,255,255));
	m_pbtnMask->SetColourDisabled(wxColour(wxT("#909090")));
	m_pSwp1 = new wmSwitcherPanel(this, ID_M_PSWP1, wxPoint(310,230), wxSize(280,200), wmSwitcherPanel::STYLE_NOSWIPE|wmSwitcherPanel::STYLE_NOANIMATION, _T("ID_M_PSWP1"));
	m_pSwp1->SetPageNameStyle(0);
	Panel1 = new wxPanel(m_pSwp1, ID_PANEL1, wxPoint(0,50), wxSize(280,247), wxTAB_TRAVERSAL, _T("ID_PANEL1"));
	Panel1->SetBackgroundColour(wxColour(0,0,0));
	m_plblResult = new wmLabel(Panel1, ID_M_PLBL5, wxEmptyString, wxPoint(0,50), wxSize(280,150), 0, _T("ID_M_PLBL5"));
	m_plblResult->SetBorderState(uiRect::BORDER_NONE);
	m_plblResult->GetUiRect().SetGradient(0);
	m_plblResult->SetForegroundColour(wxColour(255,0,0));
	m_plblResult->SetBackgroundColour(wxColour(0,0,0));
	wxFont m_plblResultFont(12,wxFONTFAMILY_SWISS,wxFONTSTYLE_NORMAL,wxFONTWEIGHT_BOLD,false,_T("Arial"),wxFONTENCODING_DEFAULT);
	m_plblResult->SetFont(m_plblResultFont);
	m_plblResult->SetTextAlign(wxALIGN_LEFT | wxALIGN_TOP);
	m_pbtnApply = new wmButton(Panel1, ID_M_PBTN2, _("Apply"), wxPoint(40,0), wxSize(100,40), 0, wxDefaultValidator, _T("ID_M_PBTN2"));
	m_pSwp1->AddPage(Panel1, _("Page1"), false);
	m_ppnlAddress = new wmipeditpnl(this, ID_PANEL2, wxPoint(100,85), wxSize(200,40), wxTAB_TRAVERSAL, _T("ID_PANEL2"));
	m_ppnlAddress->SetForegroundColour(wxColour(0,0,0));
	m_ppnlAddress->SetBackgroundColour(wxColour(255,255,255));
	m_plstInterfaces = new wmList(this, ID_M_PLST1, wxPoint(0,36), wxSize(600,44), wmList::STYLE_SELECT, 0, wxSize(-1,-1), 5, wxSize(5,1));
	m_plstInterfaces->SetBackgroundColour(wxColour(0,0,0));
	m_plstInterfaces->SetSelectedButtonColour(wxColour(wxT("#EA7500")));

	Connect(ID_M_PBTN1,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&pnlNetworkSetup::OntnStaticDHCPClick);
	Connect(ID_M_PBTN3,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&pnlNetworkSetup::OnbtnMaskClick);
	Connect(ID_M_PLST1,wxEVT_LIST_SELECTED,(wxObjectEventFunction)&pnlNetworkSetup::OnlstInterfacesSelected);
	//*)

	m_pbtnScan  = new wmButton(Panel1, wxNewId(), _("Change"), wxPoint(150,0), wxSize(100,40), 0, wxDefaultValidator, _T("ID_M_PBTN2"));
    m_pbtnScan->Show(false);

	Connect(ID_M_PBTN2, wxEVT_COMMAND_BUTTON_CLICKED, (wxObjectEventFunction)&pnlNetworkSetup::OnbtnApplyClick);
	Connect(m_pbtnScan->GetId(), wxEVT_COMMAND_BUTTON_CLICKED, (wxObjectEventFunction)&pnlNetworkSetup::OnbtnScanClick);


	m_pbtnStaticDHCP->SetToggle(true, wxT("Static"), wxT("DHCP"), 40);

	for(map<wxString, networkInterface>::const_iterator itInterface = NetworkControl::Get().GetInterfaceBegin(); itInterface != NetworkControl::Get().GetInterfaceEnd(); ++itInterface)
    {
        m_plstInterfaces->AddButton(itInterface->first);
    }
    m_plstInterfaces->SelectButton(0, true);




    m_pkbd->SetReturnString(wxT("-->|"));
	SetSize(size);
	SetPosition(pos);

	m_timerRefresh.SetOwner(this, wxNewId());
	m_timerRefresh.Start(5000);
	Connect(m_timerRefresh.GetId(), wxEVT_TIMER, (wxObjectEventFunction)&pnlNetworkSetup::OnTimerRefresh);



}

pnlNetworkSetup::~pnlNetworkSetup()
{
	//(*Destroy(pnlNetworkSetup)
	//*)
}


void pnlNetworkSetup::OntnStaticDHCPClick(wxCommandEvent& event)
{
    m_ppnlAddress->Enable((event.IsChecked() == false));
    m_pbtnMask->Enable((event.IsChecked() == false));
    m_ppnlGateway->Enable((event.IsChecked() == false));
    m_pkbd->Enable((event.IsChecked() == false));

    if(event.IsChecked())
    {
        //m_ppnlAddress->SetValue(wxEmptyString);
        //m_pbtnMask->SetLabel(wxEmptyString);
        //m_ppnlGateway->SetValue(wxEmptyString);
        m_pLbl1->SetBackgroundColour(wxColour(128,128,128));
        m_pLbl2->SetBackgroundColour(wxColour(128,128,128));
        m_pLbl4->SetBackgroundColour(wxColour(128,128,128));
    }
    else
    {
        m_ppnlAddress->SetFocus();
        m_pLbl1->SetBackgroundColour(wxColour(64,0,128));
        m_pLbl2->SetBackgroundColour(wxColour(64,0,128));
        m_pLbl4->SetBackgroundColour(wxColour(64,0,128));
    }
}

void pnlNetworkSetup::OnedtAddressTextEnter(wxCommandEvent& event)
{

}

void pnlNetworkSetup::OnedtSubnetTextEnter(wxCommandEvent& event)
{
    m_ppnlGateway->SetFocus();
}

void pnlNetworkSetup::OnedtGatewayText(wxCommandEvent& event)
{

}

void pnlNetworkSetup::OnbtnApplyClick(wxCommandEvent& event)
{
    unsigned long nMask(0);
    m_pbtnMask->GetLabel().AfterFirst(wxT('/')).BeforeFirst(wxT(' ')).ToULong(&nMask);
    m_plblResult->SetLabel(NetworkControl::Get().SetupNetworking(m_sInterface, m_ppnlAddress->GetValue(), nMask, m_ppnlGateway->GetValue(), m_pbtnStaticDHCP->IsChecked()));

}

void pnlNetworkSetup::OnbtnCancelClick(wxCommandEvent& event)
{
}

void pnlNetworkSetup::OnedtGatewayTextEnter(wxCommandEvent& event)
{
    m_ppnlAddress->SetFocus();
}

void pnlNetworkSetup::OnbtnMaskClick(wxCommandEvent& event)
{
    wxArrayString asButtons;
	asButtons.Add(wxT("/32 (255.255.255.255)"));
    asButtons.Add(wxT("/31 (255.255.255.254)"));
    asButtons.Add(wxT("/30 (255.255.255.252)"));
    asButtons.Add(wxT("/29 (255.255.255.248)"));
    asButtons.Add(wxT("/28 (255.255.255.240)"));
    asButtons.Add(wxT("/27 (255.255.255.224)"));
    asButtons.Add(wxT("/26 (255.255.255.192)"));
    asButtons.Add(wxT("/25 (255.255.255.128)"));
    asButtons.Add(wxT("/24 (255.255.255.0)"));
    asButtons.Add(wxT("/23 (255.255.254.0)"));
    asButtons.Add(wxT("/22 (255.255.252.0)"));
    asButtons.Add(wxT("/21 (255.255.248.0)"));
    asButtons.Add(wxT("/20 (255.255.240.0)"));
    asButtons.Add(wxT("/19 (255.255.224.0)"));
    asButtons.Add(wxT("/18 (255.255.192.0)"));
    asButtons.Add(wxT("/17 (255.255.128.0)"));
    asButtons.Add(wxT("/16 (255.255.0.0)"));
    asButtons.Add(wxT("/15 (255.254.0.0)"));
    asButtons.Add(wxT("/14 (255.252.0.0)"));
    asButtons.Add(wxT("/13 (255.248.0.0)"));
    asButtons.Add(wxT("/12 (255.240.0.0)"));
    asButtons.Add(wxT("/11 (255.224.0.0)"));
    asButtons.Add(wxT("/10 (255.192.0.0)"));
    asButtons.Add(wxT("/9 (255.128.0.0)"));
    asButtons.Add(wxT("/8 (255.0.0.0)"));
    asButtons.Add(wxT("/7 (254.0.0.0)"));
    asButtons.Add(wxT("/6 (252.0.0.0)"));
    asButtons.Add(wxT("/5 (248.0.0.0)"));
    asButtons.Add(wxT("/4 (240.0.0.0)"));
    asButtons.Add(wxT("/3 (224.0.0.0)"));
    asButtons.Add(wxT("/2 (192.0.0.0)"));
    asButtons.Add(wxT("/1 (128.0.0.0)"));


    dlgMask aDlg(this, asButtons, m_pbtnMask->GetLabel(), wxNewId(), ClientToScreen(m_pbtnMask->GetPosition()), m_pbtnMask->GetSize());
    if(aDlg.ShowModal()== wxID_OK)
    {
        m_pbtnMask->SetLabel(aDlg.m_sSelected);
        m_ppnlGateway->SetFocus();
    }
}


void pnlNetworkSetup::OnlstInterfacesSelected(wxCommandEvent& event)
{
    m_sInterface = event.GetString();
    NetworkControl::Get().CheckConnection(m_sInterface);
    map<wxString, networkInterface>::const_iterator itInterface= NetworkControl::Get().FindInterface(event.GetString());
    if(itInterface != NetworkControl::Get().GetInterfaceEnd())
    {
        m_pbtnStaticDHCP->ToggleSelection((itInterface->second.bStatic==false), true);
        m_ppnlAddress->SetValue(itInterface->second.sAddress);
        m_ppnlGateway->SetValue(itInterface->second.sGateway);
        m_pbtnMask->SetLabel(wxString::Format(wxT("/%d (%s)"), itInterface->second.nMask, NetworkControl::Get().ConvertMaskToAddress(itInterface->second.nMask)));

        if(itInterface->second.bWireless)
        {
            if(itInterface->second.sEssid != wxEmptyString)
            {
                m_plblType->SetLabel(itInterface->second.sEssid);
            }
            else
            {
                m_plblType->SetLabel(wxT("WiFi"));
            }
            m_pbtnScan->Show(true);
        }
        else
        {
            m_plblType->SetLabel(wxT("Ethernet"));
            m_pbtnScan->Show(false);
        }
    }
    else
    {
        m_ppnlAddress->SetValue(wxEmptyString);
        m_ppnlGateway->SetValue(wxEmptyString);
        m_pbtnMask->SetLabel(wxEmptyString);
        m_plblConnected->SetLabel(wxT("No Connection"));
        m_plblConnected->SetBackgroundColour(wxColour(128,128,128));
        m_plblType->SetLabel(wxEmptyString);
    }
    ShowConnectionDetails();
}

void pnlNetworkSetup::ShowConnectionDetails()
{
    NetworkControl::Get().CheckConnection(m_sInterface);    //refresh the connetion details
    map<wxString, networkInterface>::const_iterator itInterface= NetworkControl::Get().FindInterface(m_sInterface);
    if(itInterface != NetworkControl::Get().GetInterfaceEnd())
    {
        if(itInterface->second.bConnected)
        {
            m_plblConnected->SetLabel(wxT("Connected"));
            m_plblConnected->SetBackgroundColour(wxColour(0,128,0));
            m_pLbl1->SetBackgroundColour(wxColour(0,128,0));
        }
        else
        {
            m_plblConnected->SetLabel(wxT("No Connection"));
            m_plblConnected->SetBackgroundColour(wxColour(128,128,128));
            m_pLbl1->SetBackgroundColour(wxColour(128,128,128));
        }

        if(itInterface->second.bWireless)
        {
            if(itInterface->second.sEssid != wxEmptyString)
            {
                m_plblType->SetLabel(itInterface->second.sEssid);
            }
            else
            {
                m_plblType->SetLabel(wxT("WiFi"));
            }
        }
    }
}

void pnlNetworkSetup::OnbtnScanClick(wxCommandEvent& event)
{
    m_timerRefresh.Stop();
    dlgWiFi aDlg(NULL, m_plblType->GetLabel(), m_sInterface);
    if(aDlg.ShowModal() == wxID_OK)
    {
        m_plstInterfaces->SelectButton(m_sInterface, true); //refresh the connection settings
    }
    m_timerRefresh.Start(5000);
}

void pnlNetworkSetup::OnTimerRefresh(wxTimerEvent& event)
{
 //   if(IsShownOnScreen())
 //   {
 //       NetworkControl::Get().CheckConnection(m_sInterface);    //refresh the connetion details
 //       ShowConnectionDetails();
 //   }
}
