#include "pnlSettings.h"
#include "aoipinfobuilder.h"
#include "settings.h"

//(*InternalHeaders(pnlSettings)
#include <wx/intl.h>
#include <wx/string.h>
//*)

//(*IdInit(pnlSettings)
const long pnlSettings::ID_M_PLST16 = wxNewId();
const long pnlSettings::ID_M_PBTN29 = wxNewId();
//*)

BEGIN_EVENT_TABLE(pnlSettings,wxPanel)
	//(*EventTable(pnlSettings)
	//*)
END_EVENT_TABLE()

pnlSettings::pnlSettings(wxWindow* parent, AoIPInfoBuilder* pBuilder, wxWindowID id,const wxPoint& pos,const wxSize& size) :
    m_pBuilder(pBuilder)
{
	//(*Initialize(pnlSettings)
	Create(parent, id, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL, _T("id"));
	SetBackgroundColour(wxColour(0,0,0));
	m_plstInterval = new wmList(this, ID_M_PLST16, wxPoint(0,0), wxSize(190,135), wmList::STYLE_SELECT, 0, wxSize(-1,-1), 3, wxSize(5,5));
	m_plstInterval->SetBackgroundColour(wxColour(0,0,0));
	m_pbtnClear = new wmButton(this, ID_M_PBTN29, _("Clear Graphs"), wxPoint(100,135), wxSize(90,40), wmButton::STYLE_NORMAL, wxDefaultValidator, _T("ID_M_PBTN29"));
	m_pbtnClear->SetColourSelected(wxColour(wxT("#800000")));

	Connect(ID_M_PLST16,wxEVT_LIST_SELECTED,(wxObjectEventFunction)&pnlSettings::OnlstIntervalSelected);
	Connect(ID_M_PBTN29,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&pnlSettings::OnbtnClearClick);
	//*)

    m_plstInterval->AddButton(wxT("50 ms"), wxNullBitmap, (void*)50);
    m_plstInterval->AddButton(wxT("100 ms"), wxNullBitmap, (void*)100);
    m_plstInterval->AddButton(wxT("250 ms"), wxNullBitmap, (void*)250);
    m_plstInterval->AddButton(wxT("500 ms"), wxNullBitmap, (void*)500);
    m_plstInterval->AddButton(wxT("1000 ms"), wxNullBitmap, (void*)1000);
    m_plstInterval->AddButton(wxT("5000 ms"), wxNullBitmap, (void*)5000);

    m_plstInterval->SelectButton(m_plstInterval->FindButton((void*)Settings::Get().Read(wxT("QoS"), wxT("Interval"), 1000)), true);
}

pnlSettings::~pnlSettings()
{
	//(*Destroy(pnlSettings)
	//*)
}


void pnlSettings::OnlstIntervalSelected(wxCommandEvent& event)
{
    Settings::Get().Write(wxT("QoS"), wxT("Interval"), (int)event.GetClientData());
}

void pnlSettings::OnbtnClearClick(wxCommandEvent& event)
{
    m_pBuilder->ClearGraphs();
}
