#include "pnlNetworkSetup.h"
#include <wx/log.h>


//(*InternalHeaders(pnlNetworkSetup)
#include <wx/font.h>
#include <wx/intl.h>
#include <wx/string.h>
//*)

//(*IdInit(pnlNetworkSetup)
const long pnlNetworkSetup::ID_M_PBTN1 = wxNewId();
const long pnlNetworkSetup::ID_M_PLBL1 = wxNewId();
const long pnlNetworkSetup::ID_M_PEDT1 = wxNewId();
const long pnlNetworkSetup::ID_M_PLBL2 = wxNewId();
const long pnlNetworkSetup::ID_M_PEDT2 = wxNewId();
const long pnlNetworkSetup::ID_M_PLBL4 = wxNewId();
const long pnlNetworkSetup::ID_M_PEDT3 = wxNewId();
const long pnlNetworkSetup::ID_M_PBTN2 = wxNewId();
const long pnlNetworkSetup::ID_M_PKBD1 = wxNewId();
const long pnlNetworkSetup::ID_M_PLBL5 = wxNewId();
const long pnlNetworkSetup::ID_M_PLBL3 = wxNewId();
//*)

BEGIN_EVENT_TABLE(pnlNetworkSetup,wxPanel)
	//(*EventTable(pnlNetworkSetup)
	//*)
END_EVENT_TABLE()

pnlNetworkSetup::pnlNetworkSetup(wxWindow* parent,wxWindowID id,const wxPoint& pos,const wxSize& size, long nStyle, const wxString& sEmpty)
{
	//(*Initialize(pnlNetworkSetup)
	Create(parent, id, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL, _T("id"));
	SetBackgroundColour(wxColour(0,0,0));
	m_pbtnStaticDHCP = new wmButton(this, ID_M_PBTN1, _("Type"), wxPoint(0,40), wxSize(200,40), wmButton::STYLE_SELECT, wxDefaultValidator, _T("ID_M_PBTN1"));
	m_pbtnStaticDHCP->SetBackgroundColour(wxColour(0,128,192));
	m_pLbl1 = new wmLabel(this, ID_M_PLBL1, _("IP Address"), wxPoint(0,85), wxSize(100,40), 0, _T("ID_M_PLBL1"));
	m_pLbl1->SetBorderState(uiRect::BORDER_NONE);
	m_pLbl1->SetForegroundColour(wxColour(255,255,255));
	m_pLbl1->SetBackgroundColour(wxColour(64,0,128));
	m_pedtAddress = new wmEdit(this, ID_M_PEDT1, wxEmptyString, wxPoint(100,85), wxSize(200,40), 0, wxDefaultValidator, _T("ID_M_PEDT1"));
	m_pedtAddress->SetValidation(5);
	m_pedtAddress->SetFocusedBackground(wxColour(wxT("#FFFF80")));
	m_pedtAddress->SetBorderStyle(1,1);
	m_pLbl2 = new wmLabel(this, ID_M_PLBL2, _("Subnet Mask"), wxPoint(0,130), wxSize(100,40), 0, _T("ID_M_PLBL2"));
	m_pLbl2->SetBorderState(uiRect::BORDER_NONE);
	m_pLbl2->SetForegroundColour(wxColour(255,255,255));
	m_pLbl2->SetBackgroundColour(wxColour(64,0,128));
	m_pedtSubnet = new wmEdit(this, ID_M_PEDT2, wxEmptyString, wxPoint(100,130), wxSize(200,40), 0, wxDefaultValidator, _T("ID_M_PEDT2"));
	m_pedtSubnet->SetValidation(5);
	m_pedtSubnet->SetFocusedBackground(wxColour(wxT("#FFFF80")));
	m_pedtSubnet->SetBorderStyle(1,1);
	m_pLbl4 = new wmLabel(this, ID_M_PLBL4, _("Gateway"), wxPoint(0,175), wxSize(100,40), 0, _T("ID_M_PLBL4"));
	m_pLbl4->SetBorderState(uiRect::BORDER_NONE);
	m_pLbl4->SetForegroundColour(wxColour(255,255,255));
	m_pLbl4->SetBackgroundColour(wxColour(64,0,128));
	m_pedtGateway = new wmEdit(this, ID_M_PEDT3, wxEmptyString, wxPoint(100,175), wxSize(200,40), 0, wxDefaultValidator, _T("ID_M_PEDT3"));
	m_pedtGateway->SetValidation(5);
	m_pedtGateway->SetFocusedBackground(wxColour(wxT("#FFFF80")));
	m_pedtGateway->SetBorderStyle(1,1);
	m_pbtnApply = new wmButton(this, ID_M_PBTN2, _("Apply"), wxPoint(320,175), wxSize(100,40), 0, wxDefaultValidator, _T("ID_M_PBTN2"));
	m_pkbd = new wmKeyboard(this, ID_M_PKBD1, wxPoint(10,230), wxSize(240,200), 5, 0);
	m_pkbd->SetForegroundColour(wxColour(255,255,255));
	wxFont m_pkbdFont(10,wxFONTFAMILY_SWISS,wxFONTSTYLE_NORMAL,wxFONTWEIGHT_BOLD,false,_T("Arial"),wxFONTENCODING_DEFAULT);
	m_pkbd->SetFont(m_pkbdFont);
	m_plblResult = new wmLabel(this, ID_M_PLBL5, _("Gateway"), wxPoint(250,230), wxSize(300,200), 0, _T("ID_M_PLBL5"));
	m_plblResult->SetBorderState(uiRect::BORDER_NONE);
	m_plblResult->SetForegroundColour(wxColour(255,0,0));
	m_plblResult->SetBackgroundColour(wxColour(0,0,0));
	wxFont m_plblResultFont(12,wxFONTFAMILY_SWISS,wxFONTSTYLE_NORMAL,wxFONTWEIGHT_BOLD,false,_T("Arial"),wxFONTENCODING_DEFAULT);
	m_plblResult->SetFont(m_plblResultFont);
	m_plblResult->SetTextAlign(wxALIGN_LEFT | wxALIGN_TOP);
	m_pLbl3 = new wmLabel(this, ID_M_PLBL3, _("Manage Network Settings"), wxPoint(0,5), wxSize(600,30), 0, _T("ID_M_PLBL3"));
	m_pLbl3->SetBorderState(uiRect::BORDER_NONE);
	m_pLbl3->SetForegroundColour(wxColour(255,255,255));
	m_pLbl3->SetBackgroundColour(wxColour(0,64,0));
	wxFont m_pLbl3Font(10,wxFONTFAMILY_SWISS,wxFONTSTYLE_NORMAL,wxFONTWEIGHT_BOLD,false,_T("Arial"),wxFONTENCODING_DEFAULT);
	m_pLbl3->SetFont(m_pLbl3Font);

	Connect(ID_M_PBTN1,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&pnlNetworkSetup::OntnStaticDHCPClick);
	Connect(ID_M_PEDT1,wxEVT_COMMAND_TEXT_ENTER,(wxObjectEventFunction)&pnlNetworkSetup::OnedtAddressTextEnter);
	Connect(ID_M_PEDT2,wxEVT_COMMAND_TEXT_ENTER,(wxObjectEventFunction)&pnlNetworkSetup::OnedtSubnetTextEnter);
	Connect(ID_M_PEDT3,wxEVT_COMMAND_TEXT_ENTER,(wxObjectEventFunction)&pnlNetworkSetup::OnedtGatewayTextEnter);
	//*)

	m_pbtnStaticDHCP->SetToggleLook(true, wxT("Static"), wxT("DHCP"), 60);
    m_pedtAddress->SetValidation(wmEdit::IP);
    m_pedtSubnet->SetValidation(wmEdit::IP);
    m_pedtGateway->SetValidation(wmEdit::IP);
    m_pkbd->SetReturnString(wxT("-->|"));
	SetSize(size);
	SetPosition(pos);
}

pnlNetworkSetup::~pnlNetworkSetup()
{
	//(*Destroy(pnlNetworkSetup)
	//*)
}


void pnlNetworkSetup::OntnStaticDHCPClick(wxCommandEvent& event)
{
    m_pedtAddress->Enable((event.IsChecked() == false));
    m_pedtSubnet->Enable((event.IsChecked() == false));
    m_pedtGateway->Enable((event.IsChecked() == false));
    m_pkbd->Enable((event.IsChecked() == false));

    if(event.IsChecked())
    {
        m_pLbl1->SetBackgroundColour(wxColour(128,128,128));
        m_pLbl2->SetBackgroundColour(wxColour(128,128,128));
        m_pLbl4->SetBackgroundColour(wxColour(128,128,128));
    }
    else
    {
        m_pLbl1->SetBackgroundColour(wxColour(64,0,128));
        m_pLbl2->SetBackgroundColour(wxColour(64,0,128));
        m_pLbl4->SetBackgroundColour(wxColour(64,0,128));
    }
}

void pnlNetworkSetup::OnedtAddressTextEnter(wxCommandEvent& event)
{
    m_pedtSubnet->SetFocus();
}

void pnlNetworkSetup::OnedtSubnetTextEnter(wxCommandEvent& event)
{
    m_pedtGateway->SetFocus();
}

void pnlNetworkSetup::OnedtGatewayText(wxCommandEvent& event)
{

}

void pnlNetworkSetup::OnbtnApplyClick(wxCommandEvent& event)
{

}

void pnlNetworkSetup::OnbtnCancelClick(wxCommandEvent& event)
{
}

void pnlNetworkSetup::OnedtGatewayTextEnter(wxCommandEvent& event)
{
    m_pedtAddress->SetFocus();
}
