#include "pnlSettingsNmos.h"
#include "settings.h"
#include "settingevent.h"
#include <wx/log.h>
#include <wx/msgdlg.h>
#ifdef __NMOS__
#include "sender.h"
#include "nmos.h"
#include "senderbuttonfactory.h"
#include "wxclientapiposter.h"
#include "wxeventposter.h"
#endif // __NMOS__
#include "log.h"

//(*InternalHeaders(pnlSettingsNmos)
#include <wx/intl.h>
#include <wx/string.h>
//*)

//(*IdInit(pnlSettingsNmos)
const long pnlSettingsNmos::ID_M_PLBL8 = wxNewId();
const long pnlSettingsNmos::ID_M_PLST1 = wxNewId();
const long pnlSettingsNmos::ID_M_PLBL1 = wxNewId();
const long pnlSettingsNmos::ID_M_PLST2 = wxNewId();
const long pnlSettingsNmos::ID_M_PLBL2 = wxNewId();
const long pnlSettingsNmos::ID_M_PLST3 = wxNewId();
const long pnlSettingsNmos::ID_M_PLBL3 = wxNewId();
const long pnlSettingsNmos::ID_M_PLST4 = wxNewId();
const long pnlSettingsNmos::ID_M_PLBL4 = wxNewId();
const long pnlSettingsNmos::ID_M_PLST5 = wxNewId();
const long pnlSettingsNmos::ID_PANEL1 = wxNewId();
//*)

BEGIN_EVENT_TABLE(pnlSettingsNmos,wxPanel)
	//(*EventTable(pnlSettingsNmos)
	//*)
END_EVENT_TABLE()

pnlSettingsNmos::pnlSettingsNmos(wxWindow* parent,wxWindowID id,const wxPoint& pos,const wxSize& size, long nStyle, const wxString& sId)
{
	//(*Initialize(pnlSettingsNmos)
	Create(parent, id, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL, _T("id"));
	SetBackgroundColour(wxColour(0,0,0));
	m_pLbl9 = new wmLabel(this, ID_M_PLBL8, _("Node"), wxPoint(10,12), wxSize(100,40), 0, _T("ID_M_PLBL8"));
	m_pLbl9->SetBorderState(uiRect::BORDER_NONE);
	m_pLbl9->GetUiRect().SetGradient(0);
	m_pLbl9->SetForegroundColour(wxColour(255,255,255));
	m_pLbl9->SetBackgroundColour(wxColour(64,0,128));
	m_plstNode = new wmList(this, ID_M_PLST1, wxPoint(110,10), wxSize(400,44), wmList::STYLE_SELECT, 0, wxSize(-1,-1), 4, wxSize(5,-1));
	m_plstNode->SetBackgroundColour(wxColour(0,0,0));
	m_pLbl1 = new wmLabel(this, ID_M_PLBL1, _("Client"), wxPoint(10,62), wxSize(100,40), 0, _T("ID_M_PLBL1"));
	m_pLbl1->SetBorderState(uiRect::BORDER_NONE);
	m_pLbl1->GetUiRect().SetGradient(0);
	m_pLbl1->SetForegroundColour(wxColour(255,255,255));
	m_pLbl1->SetBackgroundColour(wxColour(64,0,128));
	m_plstClient = new wmList(this, ID_M_PLST2, wxPoint(110,60), wxSize(400,44), wmList::STYLE_SELECT, 0, wxSize(-1,-1), 3, wxSize(5,-1));
	m_plstClient->Disable();
	m_plstClient->SetBackgroundColour(wxColour(0,0,0));
	m_pLbl2 = new wmLabel(this, ID_M_PLBL2, _("Discovery"), wxPoint(10,112), wxSize(100,40), 0, _T("ID_M_PLBL2"));
	m_pLbl2->SetBorderState(uiRect::BORDER_NONE);
	m_pLbl2->GetUiRect().SetGradient(0);
	m_pLbl2->SetForegroundColour(wxColour(255,255,255));
	m_pLbl2->SetBackgroundColour(wxColour(64,0,128));
	m_plstDiscovery = new wmList(this, ID_M_PLST3, wxPoint(110,110), wxSize(400,44), wmList::STYLE_SELECT|wmList::STYLE_SELECT_MULTI, 0, wxSize(-1,-1), 2, wxSize(5,-1));
	m_plstDiscovery->Disable();
	m_plstDiscovery->SetBackgroundColour(wxColour(0,0,0));
	Panel1 = new wxPanel(this, ID_PANEL1, wxPoint(10,160), wxSize(480,270), wxTAB_TRAVERSAL, _T("ID_PANEL1"));
	Panel1->SetBackgroundColour(wxColour(35,90,53));
	m_pLbl3 = new wmLabel(Panel1, ID_M_PLBL3, _("Registration Servers"), wxPoint(5,5), wxSize(232,28), 0, _T("ID_M_PLBL3"));
	m_pLbl3->SetBorderState(uiRect::BORDER_NONE);
	m_pLbl3->GetUiRect().SetGradient(0);
	m_pLbl3->SetForegroundColour(wxColour(255,255,255));
	m_pLbl3->SetBackgroundColour(wxColour(0,64,64));
	m_plstRegistration = new wmList(Panel1, ID_M_PLST4, wxPoint(5,35), wxSize(232,230), wmList::STYLE_SELECT|wmList::STYLE_SELECT_MULTI, 1, wxSize(-1,20), 1, wxSize(0,1));
	m_plstRegistration->Disable();
	m_plstRegistration->SetBackgroundColour(wxColour(0,0,0));
	m_pLbl4 = new wmLabel(Panel1, ID_M_PLBL4, _("Query Servers"), wxPoint(243,5), wxSize(232,28), 0, _T("ID_M_PLBL4"));
	m_pLbl4->SetBorderState(uiRect::BORDER_NONE);
	m_pLbl4->GetUiRect().SetGradient(0);
	m_pLbl4->SetForegroundColour(wxColour(255,255,255));
	m_pLbl4->SetBackgroundColour(wxColour(0,64,64));
	m_plstQuery = new wmList(Panel1, ID_M_PLST5, wxPoint(243,35), wxSize(232,230), wmList::STYLE_SELECT|wmList::STYLE_SELECT_MULTI, 1, wxSize(-1,20), 1, wxSize(0,1));
	m_plstQuery->Disable();
	m_plstQuery->SetBackgroundColour(wxColour(0,0,0));

	Connect(ID_M_PLST1,wxEVT_LIST_SELECTED,(wxObjectEventFunction)&pnlSettingsNmos::OnlstNodeSelected);
	Connect(ID_M_PLST2,wxEVT_LIST_SELECTED,(wxObjectEventFunction)&pnlSettingsNmos::OnlstClientSelected);
	Connect(ID_M_PLST3,wxEVT_LIST_SELECTED,(wxObjectEventFunction)&pnlSettingsNmos::OnlstClientSelected);
	Connect(ID_M_PLST4,wxEVT_LIST_SELECTED,(wxObjectEventFunction)&pnlSettingsNmos::OnlstClientSelected);
	Connect(ID_M_PLST5,wxEVT_LIST_SELECTED,(wxObjectEventFunction)&pnlSettingsNmos::OnlstClientSelected);
	//*)

	m_plstNode->AddButton("OFF");
	m_plstNode->AddButton("Receiver Only");
	m_plstNode->AddButton("Sender Only");
	m_plstNode->AddButton("Receiver And Sender");

	m_plstClient->AddButton("OFF");
	m_plstClient->AddButton("IS04 Connection");
	m_plstClient->AddButton("IS05 Connection");

    m_plstNode->SelectButton(Settings::Get().Read("NMOS", "Node", 0));
    m_plstClient->SelectButton(Settings::Get().Read("NMOS", "Client", 0));

    Bind(wxEVT_NMOS_REGNODE_FOUND, &pnlSettingsNmos::OnNmosRegistrationNodeFound, this);
    Bind(wxEVT_NMOS_REGNODE_REMOVED, &pnlSettingsNmos::OnNmosRegistrationNodeRemoved, this);
    Bind(wxEVT_NMOS_REGNODE_CHANGED, &pnlSettingsNmos::OnNmosRegistrationNodeChanged, this);
    Bind(wxEVT_NMOS_REGNODE_CHOSEN, &pnlSettingsNmos::OnNmosRegistrationNodeChosen, this);
    Bind(wxEVT_NMOS_REGISTRATION_CHANGED, &pnlSettingsNmos::OnNmosRegistrationModeChanged, this);
}

pnlSettingsNmos::~pnlSettingsNmos()
{
	//(*Destroy(pnlSettingsNmos)
	//*)
}


void pnlSettingsNmos::OnlstNodeSelected(wxCommandEvent& event)
{
    #ifdef __NMOS__
    Settings::Get().Write("NMOS", "Node", event.GetInt());
    m_plstClient->Enable(event.GetInt() != NmosManager::NODE_OFF);
    #endif
}

void pnlSettingsNmos::OnlstClientSelected(wxCommandEvent& event)
{
    Settings::Get().Write(wxT("NMOS"), wxT("Client"), event.GetInt());
}


void pnlSettingsNmos::OnNmosRegistrationNodeFound(const wxNmosNodeRegistrationEvent& event)
{
    pmlLog() << "----------------------------OnNMOSREGISTRATIONFOUND";
    m_plstRegistration->AddButton(event.GetNodeUrl().BeforeFirst('/'), wxNullBitmap, nullptr, wmList::wmENABLED,  wxColour(100,255,100));
    m_plstRegistration->Refresh();

}

void pnlSettingsNmos::OnNmosRegistrationNodeRemoved(const wxNmosNodeRegistrationEvent& event)
{
    m_plstRegistration->DeleteButton(m_plstRegistration->FindButton(event.GetNodeUrl().BeforeFirst('/')));
    m_plstRegistration->Refresh();
}

void pnlSettingsNmos::OnNmosRegistrationNodeChanged(const wxNmosNodeRegistrationEvent& event)
{
    size_t nButton = m_plstRegistration->FindButton(event.GetNodeUrl().BeforeFirst('/'));
    m_plstRegistration->SetButtonColour(nButton, event.GetNodeStatus() ? wxColour(100,255,100) : wxColour(255,100,100));
}

void pnlSettingsNmos::OnNmosRegistrationNodeChosen(const wxNmosNodeRegistrationEvent& event)
{
    for(size_t i = 0; i < m_plstRegistration->GetItemCount(); i++)
    {
        if(m_plstRegistration->GetButtonColour(i) == wxColour(100,100,255))
        {
            m_plstRegistration->SetButtonColour(i, wxColour(100, 255,100));
        }
    }

    size_t nButton = m_plstRegistration->FindButton(event.GetNodeUrl().BeforeFirst('/'));
    m_plstRegistration->SetButtonColour(nButton, wxColour(100,100,255));

}

void pnlSettingsNmos::OnNmosRegistrationModeChanged(const wxNmosNodeRegistrationEvent& event)
{

}

void pnlSettingsNmos::OnNmosQueryNodeFound(const wxNmosClientQueryEvent& event)
{
    m_plstQuery->AddButton(event.GetUrl());
}

void pnlSettingsNmos::OnNmosQueryNodeRemoved(const wxNmosClientQueryEvent& event)
{
    m_plstQuery->DeleteButton(m_plstQuery->FindButton(event.GetUrl()));
}

void pnlSettingsNmos::OnNmosQueryNodeChanged(const wxNmosClientQueryEvent& event)
{

}
