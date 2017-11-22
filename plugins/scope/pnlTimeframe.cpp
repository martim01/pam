#include "pnlTimeframe.h"
#include "scopebuilder.h"

//(*InternalHeaders(pnlTimeframe)
#include <wx/intl.h>
#include <wx/string.h>
//*)

//(*IdInit(pnlTimeframe)
const long pnlTimeframe::ID_M_PBTN9 = wxNewId();
const long pnlTimeframe::ID_M_PBTN8 = wxNewId();
const long pnlTimeframe::ID_M_PBTN2 = wxNewId();
const long pnlTimeframe::ID_M_PBTN3 = wxNewId();
const long pnlTimeframe::ID_M_PBTN4 = wxNewId();
const long pnlTimeframe::ID_M_PBTN5 = wxNewId();
const long pnlTimeframe::ID_M_PBTN6 = wxNewId();
const long pnlTimeframe::ID_M_PBTN7 = wxNewId();
//*)

BEGIN_EVENT_TABLE(pnlTimeframe,wxPanel)
	//(*EventTable(pnlTimeframe)
	//*)
END_EVENT_TABLE()

pnlTimeframe::pnlTimeframe(wxWindow* parent,ScopeBuilder* pBuilder, wxWindowID id,const wxPoint& pos,const wxSize& size) :
    m_pBuilder(pBuilder)
{
	//(*Initialize(pnlTimeframe)
	Create(parent, id, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL, _T("id"));
	SetBackgroundColour(wxColour(0,0,0));
	m_pbtnScopeIn1000 = new wmButton(this, ID_M_PBTN9, _("Zoom In\nx10"), wxPoint(2,0), wxSize(90,40), 0, wxDefaultValidator, _T("ID_M_PBTN9"));
	m_pbtnScopeOut1000 = new wmButton(this, ID_M_PBTN8, _("Zoom Out\nx10"), wxPoint(100,0), wxSize(90,40), 0, wxDefaultValidator, _T("ID_M_PBTN8"));
	m_btnScopeIn100 = new wmButton(this, ID_M_PBTN2, _("Zoom In\nx4"), wxPoint(2,45), wxSize(90,40), 0, wxDefaultValidator, _T("ID_M_PBTN2"));
	m_pbtnScopeOut100 = new wmButton(this, ID_M_PBTN3, _("Zoom Out\nx4"), wxPoint(100,45), wxSize(90,40), 0, wxDefaultValidator, _T("ID_M_PBTN3"));
	m_pbtnScopeIn10 = new wmButton(this, ID_M_PBTN4, _("Zoom In\nx2"), wxPoint(2,90), wxSize(90,40), 0, wxDefaultValidator, _T("ID_M_PBTN4"));
	m_pbtnScopeOut10 = new wmButton(this, ID_M_PBTN5, _("Zoom Out\nx2"), wxPoint(100,90), wxSize(90,40), 0, wxDefaultValidator, _T("ID_M_PBTN5"));
	m_pbtnScopeIn1 = new wmButton(this, ID_M_PBTN6, _("Zoom In\n1 sample"), wxPoint(2,135), wxSize(90,40), wmButton::STYLE_REPEAT, wxDefaultValidator, _T("ID_M_PBTN6"));
	m_pbtnScopeOut1 = new wmButton(this, ID_M_PBTN7, _("Zoom Out\n1 sample"), wxPoint(100,135), wxSize(90,40), wmButton::STYLE_REPEAT, wxDefaultValidator, _T("ID_M_PBTN7"));

	Connect(ID_M_PBTN9,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&pnlTimeframe::OnbtnScopeIn1000Click);
	Connect(ID_M_PBTN8,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&pnlTimeframe::OnbtnScopeOut1000Click);
	Connect(ID_M_PBTN2,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&pnlTimeframe::OnbtnScopeIn100Click);
	Connect(ID_M_PBTN3,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&pnlTimeframe::OnbtnScopeOut100Click);
	Connect(ID_M_PBTN4,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&pnlTimeframe::OnbtnScopeIn10Click);
	Connect(ID_M_PBTN5,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&pnlTimeframe::OnbtnScopeOut10Click);
	Connect(ID_M_PBTN6,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&pnlTimeframe::OnbtnScopeIn1Click);
	Connect(ID_M_PBTN7,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&pnlTimeframe::OnbtnScopeOut1Click);
	//*)
}

pnlTimeframe::~pnlTimeframe()
{
	//(*Destroy(pnlTimeframe)
	//*)
}


void pnlTimeframe::OnbtnScopeIn1000Click(wxCommandEvent& event)
{
    AlterTimeframe(0.1);
}

void pnlTimeframe::OnbtnScopeOut1000Click(wxCommandEvent& event)
{
    AlterTimeframe(10.0);
}

void pnlTimeframe::OnbtnScopeIn100Click(wxCommandEvent& event)
{
    AlterTimeframe(0.25);
}

void pnlTimeframe::OnbtnScopeOut100Click(wxCommandEvent& event)
{
    AlterTimeframe(4.0);
}

void pnlTimeframe::OnbtnScopeIn10Click(wxCommandEvent& event)
{
    AlterTimeframe(0.5);
}

void pnlTimeframe::OnbtnScopeOut10Click(wxCommandEvent& event)
{
    AlterTimeframe(2.0);
}

void pnlTimeframe::OnbtnScopeIn1Click(wxCommandEvent& event)
{
    AlterTimeframe(0.9);
}

void pnlTimeframe::OnbtnScopeOut1Click(wxCommandEvent& event)
{
    AlterTimeframe(1.1);
}

void pnlTimeframe::AlterTimeframe(double dMod)
{
    double dTimeframe = m_pBuilder->ReadSetting(wxT("Timeframe"), 1.0);
    dTimeframe *= dMod;
    m_pBuilder->WriteSetting(wxT("Timeframe"), dTimeframe);
}
