#include "pnlOptions.h"
#include "ptpbuilder.h"
//(*InternalHeaders(pnlOptions)
#include <wx/intl.h>
#include <wx/string.h>
//*)

//(*IdInit(pnlOptions)
const long pnlOptions::ID_M_PLST5 = wxNewId();
//*)

BEGIN_EVENT_TABLE(pnlOptions,wxPanel)
	//(*EventTable(pnlOptions)
	//*)
END_EVENT_TABLE()

pnlOptions::pnlOptions(wxWindow* parent, ptpBuilder* pBuilder,wxWindowID id,const wxPoint& pos,const wxSize& size) :
    m_pBuilder(pBuilder)
{
	//(*Initialize(pnlOptions)
	Create(parent, id, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL, _T("id"));
	SetBackgroundColour(wxColour(0,0,0));
	m_plstWindow = new wmList(this, ID_M_PLST5, wxPoint(0,0), wxSize(190,100), wmList::STYLE_SELECT, 0, wxSize(-1,30), 2, wxSize(5,5));
	m_plstWindow->SetBackgroundColour(wxColour(0,0,0));

	Connect(ID_M_PLST5,wxEVT_LIST_SELECTED,(wxObjectEventFunction)&pnlOptions::OnlstWindowSelected);
	//*)

	m_plblHG = new wmLabel(this, wxNewId(), _("Histogram Granularity"), wxPoint(0,105), wxSize(200,20), 0, _T("ID_M_PLBL85"));
	m_plblHG->SetBorderState(uiRect::BORDER_NONE);
	m_plblHG->GetUiRect().SetGradient(0);
	m_plblHG->SetForegroundColour(wxColour(255,255,255));
	m_plblHG->SetBackgroundColour(wxColour(0,128,64));

	m_plstGranularity = new wmList(this, wxNewId(), wxPoint(0,126), wxSize(200,34), wmList::STYLE_NORMAL, 0, wxSize(-1,30), 3, wxSize(5,5));
	m_plstGranularity->SetBackgroundColour(wxColour(0,0,0));

	m_plblHR = new wmLabel(this, wxNewId(), _("Histogram Resolution"), wxPoint(0,165), wxSize(200,20), 0, _T("ID_M_PLBL85"));
	m_plblHR->SetBorderState(uiRect::BORDER_NONE);
	m_plblHR->GetUiRect().SetGradient(0);
	m_plblHR->SetForegroundColour(wxColour(255,255,255));
	m_plblHR->SetBackgroundColour(wxColour(0,128,64));

	m_plstResolution = new wmList(this, wxNewId(), wxPoint(0,186), wxSize(200,34), wmList::STYLE_NORMAL, 0, wxSize(-1,30), 3, wxSize(5,5));
	m_plstResolution->SetBackgroundColour(wxColour(0,0,0));

	m_pbtnClear = new wmButton(this, wxNewId(), _("Clear Graphs"), wxPoint(100,225), wxSize(90,30), wmButton::STYLE_NORMAL, wxDefaultValidator, _T("ID_M_PBTN29"));

    Connect(m_plstResolution->GetId(),wxEVT_LIST_SELECTED,(wxObjectEventFunction)&pnlOptions::OnlstResolutionPressed);
    Connect(m_plstGranularity->GetId(),wxEVT_LIST_SELECTED,(wxObjectEventFunction)&pnlOptions::OnlstGranularityPressed);
    Connect(m_pbtnClear->GetId(),wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&pnlOptions::OnbtnClearClick);

    m_plstGranularity->AddButton("Decrease");
    m_plstGranularity->AddButton("Reset");
    m_plstGranularity->AddButton("Increase");

    m_plstResolution->AddButton("Decrease");
    m_plstResolution->AddButton("Reset");
    m_plstResolution->AddButton("Increase");

	m_plstWindow->AddButton("Info");
	m_plstWindow->AddButton("Offset Graph");
	m_plstWindow->AddButton("Offset Histogram");
	m_plstWindow->AddButton("Delay Graph");
	m_plstWindow->AddButton("Delay Histogram");

	m_plstWindow->SelectButton(m_pBuilder->ReadSetting("Window", "Info"));

}

pnlOptions::~pnlOptions()
{
	//(*Destroy(pnlOptions)
	//*)
}


void pnlOptions::OnlstWindowSelected(wxCommandEvent& event)
{
    m_pBuilder->WriteSetting("Window", event.GetString());
}

void pnlOptions::OnbtnClearClick(wxCommandEvent& event)
{
    m_pBuilder->ClearGraphs();
}

void pnlOptions::OnlstGranularityPressed(wxCommandEvent& event)
{
    m_pBuilder->ChangeGranularity(event.GetInt());
}

void pnlOptions::OnlstResolutionPressed(wxCommandEvent& event)
{
    m_pBuilder->ChangeResolution(event.GetInt());
}
