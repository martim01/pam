#include "pnlSettingsNmos.h"
#include "settings.h"
#include "settingevent.h"
#include <wx/log.h>
#include <wx/msgdlg.h>
#ifdef __NMOS__
#include "sender.h"
#include "clientapi.h"
#include "senderbuttonfactory.h"
#endif // __NMOS__
//(*InternalHeaders(pnlSettingsNmos)
#include <wx/intl.h>
#include <wx/string.h>
//*)

//(*IdInit(pnlSettingsNmos)
const long pnlSettingsNmos::ID_M_PLBL8 = wxNewId();
const long pnlSettingsNmos::ID_M_PLST1 = wxNewId();
const long pnlSettingsNmos::ID_M_PLBL1 = wxNewId();
const long pnlSettingsNmos::ID_M_PLST2 = wxNewId();
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
	m_plstNode = new wmList(this, ID_M_PLST1, wxPoint(110,10), wxSize(400,44), 0, 0, wxSize(-1,-1), 4, wxSize(-1,-1));
	m_plstNode->SetBackgroundColour(wxColour(0,0,0));
	m_pLbl1 = new wmLabel(this, ID_M_PLBL1, _("Client"), wxPoint(10,62), wxSize(100,40), 0, _T("ID_M_PLBL1"));
	m_pLbl1->SetBorderState(uiRect::BORDER_NONE);
	m_pLbl1->GetUiRect().SetGradient(0);
	m_pLbl1->SetForegroundColour(wxColour(255,255,255));
	m_pLbl1->SetBackgroundColour(wxColour(64,0,128));
	m_plstClient = new wmList(this, ID_M_PLST2, wxPoint(110,60), wxSize(400,44), 0, 0, wxSize(-1,-1), 3, wxSize(-1,-1));
	m_plstClient->Disable();
	m_plstClient->SetBackgroundColour(wxColour(0,0,0));

	Connect(ID_M_PLST1,wxEVT_LIST_SELECTED,(wxObjectEventFunction)&pnlSettingsNmos::OnlstNodeSelected);
	Connect(ID_M_PLST2,wxEVT_LIST_SELECTED,(wxObjectEventFunction)&pnlSettingsNmos::OnlstClientSelected);
	//*)

	m_plstNode->AddButton("OFF");
	m_plstNode->AddButton("Receiver Only");
	m_plstNode->AddButton("Sender Only");
	m_plstNode->AddButton("Receiver And Sender");

	m_plstClient->AddButton("OFF");
	m_plstClient->AddButton("IS04 Connection");
	m_plstClient->AddButton("IS05 Connection");

    m_plstNode->SelectButton(Settings::Get().Read("NMOS", "NODE", 0));
    m_plstClient->SelectButton(Settings::Get().Read("NMOS", "Client", 0));

}

pnlSettingsNmos::~pnlSettingsNmos()
{
	//(*Destroy(pnlSettingsNmos)
	//*)
}


void pnlSettingsNmos::OnlstNodeSelected(wxCommandEvent& event)
{
    Settings::Get().Write("NMOS", "Node", event.GetInt());
    m_plstClient->Enable(event.GetString() != 0);
}

void pnlSettingsNmos::OnlstClientSelected(wxCommandEvent& event)
{
    Settings::Get().Write(wxT("NMOS"), wxT("Client"), event.GetInt());
}
