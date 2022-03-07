#include "pnlOptions.h"
#include "scopebuilder.h"
#include "settingevent.h"

//(*InternalHeaders(pnlOptions)
#include <wx/intl.h>
#include <wx/string.h>
//*)

//(*IdInit(pnlOptions)
const long pnlOptions::ID_M_PLST21 = wxNewId();
const long pnlOptions::ID_M_PBTN30 = wxNewId();
const long pnlOptions::ID_M_PBTN32 = wxNewId();
const long pnlOptions::ID_M_PBTN29 = wxNewId();
const long pnlOptions::ID_M_PBTN33 = wxNewId();
//*)

BEGIN_EVENT_TABLE(pnlOptions,wxPanel)
	//(*EventTable(pnlOptions)
	//*)
END_EVENT_TABLE()

pnlOptions::pnlOptions(wxWindow* parent,ScopeBuilder* pBuilder, wxWindowID id,const wxPoint& pos,const wxSize& size) :
    m_pBuilder(pBuilder)
{
	//(*Initialize(pnlOptions)
	Create(parent, id, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL, _T("id"));
	SetBackgroundColour(wxColour(0,0,0));
	m_plstScope_Mode = new wmList(this, ID_M_PLST21, wxPoint(0,-2), wxSize(95,135), wmList::STYLE_SELECT, 0, wxSize(-1,-1), 1, wxSize(5,5));
	m_plstScope_Mode->SetBackgroundColour(wxColour(0,0,0));
	m_pbtnScope_Autotrigger = new wmButton(this, ID_M_PBTN30, _("Autotrigger"), wxPoint(100,0), wxSize(90,40), wmButton::STYLE_SELECT, wxDefaultValidator, _T("ID_M_PBTN30"));
	m_pbtnScope_Autotrigger->SetColourSelected(wxColour(wxT("#800000")));
	m_pbtnScope_ResetSlide = new wmButton(this, ID_M_PBTN32, _("Reset Slide"), wxPoint(100,45), wxSize(90,40), wmButton::STYLE_NORMAL, wxDefaultValidator, _T("ID_M_PBTN32"));
	m_pbtnScope_ResetSlide->SetColourSelected(wxColour(wxT("#800000")));
	m_pbtnScope_ClearMemory = new wmButton(this, ID_M_PBTN29, _("Clear Memory"), wxPoint(100,135), wxSize(90,40), wmButton::STYLE_NORMAL, wxDefaultValidator, _T("ID_M_PBTN29"));
	m_pbtnScope_ClearMemory->SetColourSelected(wxColour(wxT("#800000")));
	m_pbtnScope_Memory = new wmButton(this, ID_M_PBTN33, _("Memory"), wxPoint(2,135), wxSize(90,40), wmButton::STYLE_SELECT, wxDefaultValidator, _T("ID_M_PBTN33"));
	m_pbtnScope_Memory->SetColourSelected(wxColour(wxT("#800000")));

	Connect(ID_M_PLST21,wxEVT_LIST_SELECTED,(wxObjectEventFunction)&pnlOptions::OnlstScope_ModeSelected);
	Connect(ID_M_PBTN30,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&pnlOptions::OnbtnScope_AutotriggerClick);
	Connect(ID_M_PBTN32,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&pnlOptions::OnbtnScope_ResetSlideClick);
	Connect(ID_M_PBTN29,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&pnlOptions::OnbtnScope_ClearMemoryClick);
	Connect(ID_M_PBTN33,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&pnlOptions::OnbtnScope_MemoryClick);
	//*)

	m_plstScope_Mode->AddButton(wxT("Normal"));
    m_plstScope_Mode->AddButton(wxT("Slide"));
    m_plstScope_Mode->AddButton(wxT("Cursor"));
    m_plstScope_Mode->ConnectToSetting(m_pBuilder->GetSection(), "Mode", size_t(0));

    m_pbtnScope_Autotrigger->ConnectToSetting(m_pBuilder->GetSection(), "Autotrigger", false);

    m_pBuilder->RegisterForSettingsUpdates(this, "Mode");

}

pnlOptions::~pnlOptions()
{
	//(*Destroy(pnlOptions)
	//*)
}


void pnlOptions::OnlstScope_ModeSelected(wxCommandEvent& event)
{
}

void pnlOptions::OnbtnScope_AutotriggerClick(wxCommandEvent& event)
{
}

void pnlOptions::OnbtnScope_ResetSlideClick(wxCommandEvent& event)
{
    m_pBuilder->ResetSlide();
}

void pnlOptions::OnbtnScope_ClearMemoryClick(wxCommandEvent& event)
{
    m_pBuilder->ClearMemory();
}

void pnlOptions::OnbtnScope_MemoryClick(wxCommandEvent& event)
{
    m_pBuilder->SetMemory(event.IsChecked());
}


void pnlOptions::OnSettingChanged(SettingEvent& event)
{
}
