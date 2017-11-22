#include "pnlVertical.h"
#include "scopebuilder.h"

//(*InternalHeaders(pnlVertical)
#include <wx/intl.h>
#include <wx/string.h>
//*)

//(*IdInit(pnlVertical)
const long pnlVertical::ID_M_PBTN13 = wxNewId();
const long pnlVertical::ID_M_PBTN14 = wxNewId();
const long pnlVertical::ID_M_PBTN15 = wxNewId();
const long pnlVertical::ID_M_PBTN16 = wxNewId();
const long pnlVertical::ID_M_PBTN17 = wxNewId();
const long pnlVertical::ID_M_PBTN18 = wxNewId();
const long pnlVertical::ID_M_PBTN19 = wxNewId();
const long pnlVertical::ID_M_PBTN20 = wxNewId();
//*)

BEGIN_EVENT_TABLE(pnlVertical,wxPanel)
	//(*EventTable(pnlVertical)
	//*)
END_EVENT_TABLE()

pnlVertical::pnlVertical(wxWindow* parent, ScopeBuilder* pBuilder, wxWindowID id,const wxPoint& pos,const wxSize& size) :
    m_pBuilder(pBuilder)
{
	//(*Initialize(pnlVertical)
	Create(parent, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL, _T("wxID_ANY"));
	SetBackgroundColour(wxColour(0,0,0));
	m_pbtnVerticalIn10 = new wmButton(this, ID_M_PBTN13, _("Zoom In\nx10"), wxPoint(2,0), wxSize(90,40), 0, wxDefaultValidator, _T("ID_M_PBTN13"));
	m_pbtnVerticalOut10 = new wmButton(this, ID_M_PBTN14, _("Zoom Out\nx10"), wxPoint(100,0), wxSize(90,40), 0, wxDefaultValidator, _T("ID_M_PBTN14"));
	m_pbtnVerticalIn4 = new wmButton(this, ID_M_PBTN15, _("Zoom In\nx4"), wxPoint(2,45), wxSize(90,40), 0, wxDefaultValidator, _T("ID_M_PBTN15"));
	m_pbtnVerticalOut4 = new wmButton(this, ID_M_PBTN16, _("Zoom Out\nx4"), wxPoint(100,45), wxSize(90,40), 0, wxDefaultValidator, _T("ID_M_PBTN16"));
	m_pbtnVerticalIn2 = new wmButton(this, ID_M_PBTN17, _("Zoom In\nx2"), wxPoint(2,90), wxSize(90,40), 0, wxDefaultValidator, _T("ID_M_PBTN17"));
	m_pbtnVerticalOut2 = new wmButton(this, ID_M_PBTN18, _("Zoom Out\nx2"), wxPoint(100,90), wxSize(90,40), 0, wxDefaultValidator, _T("ID_M_PBTN18"));
	m_pbtnVerticalIn1 = new wmButton(this, ID_M_PBTN19, _("Zoom In\n0.01"), wxPoint(2,135), wxSize(90,40), wmButton::STYLE_REPEAT, wxDefaultValidator, _T("ID_M_PBTN19"));
	m_pbtnVerticalOut1 = new wmButton(this, ID_M_PBTN20, _("Zoom Out\n0.01"), wxPoint(100,135), wxSize(90,40), wmButton::STYLE_REPEAT, wxDefaultValidator, _T("ID_M_PBTN20"));

	Connect(ID_M_PBTN13,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&pnlVertical::OnbtnVerticalIn10Click);
	Connect(ID_M_PBTN14,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&pnlVertical::OnbtnVerticalOut10Click);
	Connect(ID_M_PBTN15,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&pnlVertical::OnbtnVerticalIn4Click);
	Connect(ID_M_PBTN16,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&pnlVertical::OnbtnVerticalOut4Click);
	Connect(ID_M_PBTN17,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&pnlVertical::OnbtnVerticalIn2Click);
	Connect(ID_M_PBTN18,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&pnlVertical::OnbtnVerticalOut2Click);
	Connect(ID_M_PBTN19,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&pnlVertical::OnbtnVerticalIn1Click);
	Connect(ID_M_PBTN20,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&pnlVertical::OnbtnVerticalOut1Click);
	//*)
}

pnlVertical::~pnlVertical()
{
	//(*Destroy(pnlVertical)
	//*)
}


void pnlVertical::OnbtnVerticalIn10Click(wxCommandEvent& event)
{
    AlterVerticalZoom(0.1);
}

void pnlVertical::OnbtnVerticalOut10Click(wxCommandEvent& event)
{
    AlterVerticalZoom(10.0);
}

void pnlVertical::OnbtnVerticalIn4Click(wxCommandEvent& event)
{
    AlterVerticalZoom(0.25);
}

void pnlVertical::OnbtnVerticalOut4Click(wxCommandEvent& event)
{
    AlterVerticalZoom(4.0);
}

void pnlVertical::OnbtnVerticalIn2Click(wxCommandEvent& event)
{
    AlterVerticalZoom(0.5);
}

void pnlVertical::OnbtnVerticalOut2Click(wxCommandEvent& event)
{
    AlterVerticalZoom(2.0);
}

void pnlVertical::OnbtnVerticalIn1Click(wxCommandEvent& event)
{
    AlterVerticalZoom(0.9);
}

void pnlVertical::OnbtnVerticalOut1Click(wxCommandEvent& event)
{
    AlterVerticalZoom(1.1);
}


void pnlVertical::AlterVerticalZoom(double dMod)
{
    double dVertical = m_pBuilder->ReadSetting(wxT("Vertical"), 1.0);
    dVertical *= dMod;
    m_pBuilder->WriteSetting(wxT("Vertical"), dVertical);
}
