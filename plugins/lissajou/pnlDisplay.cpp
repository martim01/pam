#include "pnlDisplay.h"
#include "lissajoubuilder.h"
//(*InternalHeaders(pnlDisplay)
#include <wx/intl.h>
#include <wx/string.h>
//*)

//(*IdInit(pnlDisplay)
const long pnlDisplay::ID_M_PLST24 = wxNewId();
const long pnlDisplay::ID_M_PBTN36 = wxNewId();
const long pnlDisplay::ID_M_PBTN37 = wxNewId();
const long pnlDisplay::ID_M_PBTN1 = wxNewId();
//*)

BEGIN_EVENT_TABLE(pnlDisplay,wxPanel)
	//(*EventTable(pnlDisplay)
	//*)
END_EVENT_TABLE()

pnlDisplay::pnlDisplay(wxWindow* parent,LissajouBuilder* pBuilder, wxWindowID id,const wxPoint& pos,const wxSize& size) :
    m_pBuilder(pBuilder)
{
	//(*Initialize(pnlDisplay)
	Create(parent, id, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL, _T("id"));
	SetBackgroundColour(wxColour(0,0,0));
	m_plstScaling = new wmList(this, ID_M_PLST24, wxPoint(0,0), wxSize(190,45), wmList::STYLE_SELECT, 0, wxSize(-1,-1), 3, wxSize(5,5));
	m_plstScaling->SetBackgroundColour(wxColour(0,0,0));
	m_pbtnRotate = new wmButton(this, ID_M_PBTN36, _("Axis"), wxPoint(2,50), wxSize(196,35), wmButton::STYLE_SELECT, wxDefaultValidator, _T("ID_M_PBTN36"));
	m_pbtnRotate->SetBackgroundColour(wxColour(0,64,0));
	m_pbtnRotate->SetColourSelected(wxColour(wxT("#B40000")));
	m_pbtnLevels = new wmButton(this, ID_M_PBTN37, _("Levels"), wxPoint(2,90), wxSize(196,35), wmButton::STYLE_SELECT, wxDefaultValidator, _T("ID_M_PBTN37"));
	m_pbtnLevels->SetBackgroundColour(wxColour(0,64,0));
	m_pbtnLevels->SetColourSelected(wxColour(wxT("#008080")));
	m_pbtnDisplay = new wmButton(this, ID_M_PBTN1, _("Display"), wxPoint(2,130), wxSize(196,35), wmButton::STYLE_SELECT, wxDefaultValidator, _T("ID_M_PBTN1"));
	m_pbtnDisplay->SetBackgroundColour(wxColour(0,64,0));
	m_pbtnDisplay->SetColourSelected(wxColour(wxT("#008080")));

	Connect(ID_M_PLST24,wxEVT_LIST_SELECTED,(wxObjectEventFunction)&pnlDisplay::OnlstScalingSelected);
	Connect(ID_M_PBTN36,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&pnlDisplay::OnbtnRotateClick);
	Connect(ID_M_PBTN37,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&pnlDisplay::OnbtnLevelsClick);
	Connect(ID_M_PBTN1,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&pnlDisplay::OnbtnDisplayClick);
	//*)
    SetBackgroundColour(*wxBLACK);
	m_plstScaling->AddButton(wxT("dB"));
	m_plstScaling->AddButton(wxT("Linear"));
	m_plstScaling->AddButton(wxT("Auto Scale"));

	m_plstScaling->ConnectToSetting(m_pBuilder->GetSection(), "Scale", size_t(1));


	m_pbtnRotate->SetToggle(true, wxT("LvR"), wxT("MvS"), 50.0);
	m_pbtnRotate->ConnectToSetting(m_pBuilder->GetSection(), "Rotate", true);

	m_pbtnLevels->SetToggle(true, wxT("Hide"), wxT("Show"), 50.0);
	m_pbtnLevels->ConnectToSetting(m_pBuilder->GetSection(), "Levels", false);

	m_pbtnDisplay->SetToggle(true, wxT("Stars"), wxT("Hull"), 50.0);
	m_pbtnDisplay->ConnectToSetting(m_pBuilder->GetSection(), "Display", false);



}

pnlDisplay::~pnlDisplay()
{
	//(*Destroy(pnlDisplay)
	//*)
}


void pnlDisplay::OnlstScalingSelected(wxCommandEvent& event)
{
}

void pnlDisplay::OnbtnRotateClick(wxCommandEvent& event)
{
}

void pnlDisplay::OnbtnLevelsClick(wxCommandEvent& event)
{
}

void pnlDisplay::OnbtnDisplayClick(wxCommandEvent& event)
{
}
