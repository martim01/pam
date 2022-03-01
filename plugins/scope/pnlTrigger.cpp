#include "pnlTrigger.h"
#include "scopebuilder.h"

//(*InternalHeaders(pnlTrigger)
#include <wx/intl.h>
#include <wx/string.h>
//*)

//(*IdInit(pnlTrigger)
const long pnlTrigger::ID_M_PBTN21 = wxNewId();
const long pnlTrigger::ID_M_PBTN22 = wxNewId();
const long pnlTrigger::ID_M_PBTN23 = wxNewId();
const long pnlTrigger::ID_M_PBTN24 = wxNewId();
const long pnlTrigger::ID_M_PBTN25 = wxNewId();
const long pnlTrigger::ID_M_PBTN26 = wxNewId();
const long pnlTrigger::ID_M_PBTN27 = wxNewId();
const long pnlTrigger::ID_M_PBTN28 = wxNewId();
//*)

BEGIN_EVENT_TABLE(pnlTrigger,wxPanel)
	//(*EventTable(pnlTrigger)
	//*)
END_EVENT_TABLE()

pnlTrigger::pnlTrigger(wxWindow* parent,ScopeBuilder* pBuilder, wxWindowID id,const wxPoint& pos,const wxSize& size) :
    m_pBuilder(pBuilder)
{
	//(*Initialize(pnlTrigger)
	Create(parent, id, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL, _T("id"));
	SetBackgroundColour(wxColour(0,0,0));
	m_pbtnTriggerUp1000 = new wmButton(this, ID_M_PBTN21, _("+0.01"), wxPoint(2,0), wxSize(90,40), wmButton::STYLE_REPEAT, wxDefaultValidator, _T("ID_M_PBTN21"));
	m_pbtnTriggerUp1000->SetColourDisabled(wxColour(wxT("#909090")));
	m_pbtnTriggerUp1000->SetTextColourDisabled(wxColour(wxT("#C0C0C0")));
	m_pbtnTriggerDown1000 = new wmButton(this, ID_M_PBTN22, _("-0.01"), wxPoint(100,0), wxSize(90,40), wmButton::STYLE_REPEAT, wxDefaultValidator, _T("ID_M_PBTN22"));
	m_pbtnTriggerDown1000->SetColourDisabled(wxColour(wxT("#909090")));
	m_pbtnTriggerDown1000->SetTextColourDisabled(wxColour(wxT("#C0C0C0")));
	m_pbtnTriggerUp100 = new wmButton(this, ID_M_PBTN23, _("+0.001"), wxPoint(2,45), wxSize(90,40), wmButton::STYLE_REPEAT, wxDefaultValidator, _T("ID_M_PBTN23"));
	m_pbtnTriggerUp100->SetColourDisabled(wxColour(wxT("#909090")));
	m_pbtnTriggerUp100->SetTextColourDisabled(wxColour(wxT("#C0C0C0")));
	m_pbtnTriggerDown100 = new wmButton(this, ID_M_PBTN24, _("-0.001"), wxPoint(100,45), wxSize(90,40), wmButton::STYLE_REPEAT, wxDefaultValidator, _T("ID_M_PBTN24"));
	m_pbtnTriggerDown100->SetColourDisabled(wxColour(wxT("#909090")));
	m_pbtnTriggerDown100->SetTextColourDisabled(wxColour(wxT("#C0C0C0")));
	m_pbtnTriggerUp10 = new wmButton(this, ID_M_PBTN25, _("+0.0001"), wxPoint(2,90), wxSize(90,40), wmButton::STYLE_REPEAT, wxDefaultValidator, _T("ID_M_PBTN25"));
	m_pbtnTriggerUp10->SetColourDisabled(wxColour(wxT("#909090")));
	m_pbtnTriggerUp10->SetTextColourDisabled(wxColour(wxT("#C0C0C0")));
	m_pbtnTriggerDown10 = new wmButton(this, ID_M_PBTN26, _("-0.0001"), wxPoint(100,90), wxSize(90,40), wmButton::STYLE_REPEAT, wxDefaultValidator, _T("ID_M_PBTN26"));
	m_pbtnTriggerDown10->SetColourDisabled(wxColour(wxT("#909090")));
	m_pbtnTriggerDown10->SetTextColourDisabled(wxColour(wxT("#C0C0C0")));
	m_pbtnTriggerUp1 = new wmButton(this, ID_M_PBTN27, _("+0.00001"), wxPoint(2,135), wxSize(90,40), wmButton::STYLE_REPEAT, wxDefaultValidator, _T("ID_M_PBTN27"));
	m_pbtnTriggerUp1->SetColourDisabled(wxColour(wxT("#909090")));
	m_pbtnTriggerUp1->SetTextColourDisabled(wxColour(wxT("#C0C0C0")));
	m_pbtnTriggerDown1 = new wmButton(this, ID_M_PBTN28, _("-0.0001"), wxPoint(100,135), wxSize(90,40), wmButton::STYLE_REPEAT, wxDefaultValidator, _T("ID_M_PBTN28"));
	m_pbtnTriggerDown1->SetColourDisabled(wxColour(wxT("#909090")));
	m_pbtnTriggerDown1->SetTextColourDisabled(wxColour(wxT("#C0C0C0")));

	Connect(ID_M_PBTN21,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&pnlTrigger::OnbtnTriggerUp1000Click);
	Connect(ID_M_PBTN22,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&pnlTrigger::OnbtnTriggerDown1000Click);
	Connect(ID_M_PBTN23,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&pnlTrigger::OnbtnTriggerUp100Click);
	Connect(ID_M_PBTN24,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&pnlTrigger::OnbtnTriggerDown100Click);
	Connect(ID_M_PBTN25,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&pnlTrigger::OnbtnTriggerUp10Click);
	Connect(ID_M_PBTN26,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&pnlTrigger::OnbtnTriggerDown10Click);
	Connect(ID_M_PBTN27,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&pnlTrigger::OnbtnTriggerUp1Click);
	Connect(ID_M_PBTN28,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&pnlTrigger::OnbtnTriggerDown1Click);
	//*)

	EnableButtons(m_pBuilder->ReadSetting(wxT("Autotrigger"),0) == 0);
}

pnlTrigger::~pnlTrigger()
{
	//(*Destroy(pnlTrigger)
	//*)
}


void pnlTrigger::OnbtnTriggerUp1000Click(wxCommandEvent& event)
{
    AlterTrigger(0.01);
}

void pnlTrigger::OnbtnTriggerDown1000Click(wxCommandEvent& event)
{
    AlterTrigger(-0.01);
}

void pnlTrigger::OnbtnTriggerUp100Click(wxCommandEvent& event)
{
    AlterTrigger(0.001);
}

void pnlTrigger::OnbtnTriggerDown100Click(wxCommandEvent& event)
{
    AlterTrigger(-0.001);
}

void pnlTrigger::OnbtnTriggerUp10Click(wxCommandEvent& event)
{
    AlterTrigger(0.0001);
}

void pnlTrigger::OnbtnTriggerDown10Click(wxCommandEvent& event)
{
    AlterTrigger(-0.0001);
}

void pnlTrigger::OnbtnTriggerUp1Click(wxCommandEvent& event)
{
    AlterTrigger(0.00001);
}

void pnlTrigger::OnbtnTriggerDown1Click(wxCommandEvent& event)
{
    AlterTrigger(-0.00001);
}


void pnlTrigger::AlterTrigger(double dMod)
{
    double dTrigger = m_pBuilder->ReadSetting(wxT("Trigger"), 0.5);
    dTrigger += dMod;
    m_pBuilder->WriteSetting(wxT("Trigger"), dTrigger);
}


void pnlTrigger::EnableButtons(bool bEnable)
{
    m_pbtnTriggerDown1000->Enable(bEnable);
    m_pbtnTriggerDown1->Enable(bEnable);
    m_pbtnTriggerUp100->Enable(bEnable);
    m_pbtnTriggerDown10->Enable(bEnable);
    m_pbtnTriggerUp1000->Enable(bEnable);
    m_pbtnTriggerUp10->Enable(bEnable);
    m_pbtnTriggerUp1->Enable(bEnable);
    m_pbtnTriggerDown100->Enable(bEnable);
}
