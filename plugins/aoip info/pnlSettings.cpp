#include "pnlSettings.h"
#include "aoipinfobuilder.h"
#include "settings.h"

//(*InternalHeaders(pnlSettings)
#include <wx/intl.h>
#include <wx/string.h>
//*)

//(*IdInit(pnlSettings)
const long pnlSettings::ID_M_PLST16 = wxNewId();
//*)

BEGIN_EVENT_TABLE(pnlSettings,wxPanel)
	//(*EventTable(pnlSettings)
	//*)
END_EVENT_TABLE()

pnlSettings::pnlSettings(wxWindow* parent, AoIPInfoBuilder* pBuilder, wxWindowID id,const wxPoint& pos,const wxSize& size) :
    m_pBuilder(pBuilder)
{

	Create(parent, id, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL, _T("id"));
	SetBackgroundColour(wxColour(0,0,0));

	m_plblQoS = new wmLabel(this, wxNewId(), _("QoS Interval"), wxPoint(0,0), wxSize(200,20), 0, _T("ID_M_PLBL85"));
	m_plblQoS->SetBorderState(uiRect::BORDER_NONE);
	m_plblQoS->GetUiRect().SetGradient(0);
	m_plblQoS->SetForegroundColour(wxColour(255,255,255));
	m_plblQoS->SetBackgroundColour(wxColour(0,128,64));

	m_plstInterval = new wmList(this, ID_M_PLST16, wxPoint(0,22), wxSize(200,80), wmList::STYLE_SELECT, 0, wxSize(-1,30), 3, wxSize(5,5));
	m_plstInterval->SetBackgroundColour(wxColour(0,0,0));


	m_plblHG = new wmLabel(this, wxNewId(), _("Histogram Granularity"), wxPoint(0,110), wxSize(200,20), 0, _T("ID_M_PLBL85"));
	m_plblHG->SetBorderState(uiRect::BORDER_NONE);
	m_plblHG->GetUiRect().SetGradient(0);
	m_plblHG->SetForegroundColour(wxColour(255,255,255));
	m_plblHG->SetBackgroundColour(wxColour(0,128,64));

	m_plstGranularity = new wmList(this, wxNewId(), wxPoint(0,132), wxSize(200,34), wmList::STYLE_NORMAL, 0, wxSize(-1,30), 3, wxSize(5,5));
	m_plstGranularity->SetBackgroundColour(wxColour(0,0,0));



	m_plblHR = new wmLabel(this, wxNewId(), _("Histogram Resolution"), wxPoint(0,170), wxSize(200,20), 0, _T("ID_M_PLBL85"));
	m_plblHR->SetBorderState(uiRect::BORDER_NONE);
	m_plblHR->GetUiRect().SetGradient(0);
	m_plblHR->SetForegroundColour(wxColour(255,255,255));
	m_plblHR->SetBackgroundColour(wxColour(0,128,64));

	m_plstResolution = new wmList(this, wxNewId(), wxPoint(0,192), wxSize(200,34), wmList::STYLE_NORMAL, 0, wxSize(-1,30), 3, wxSize(5,5));
	m_plstResolution->SetBackgroundColour(wxColour(0,0,0));


	Connect(ID_M_PLST16,wxEVT_LIST_SELECTED,(wxObjectEventFunction)&pnlSettings::OnlstIntervalSelected);
	Connect(m_plstResolution->GetId(),wxEVT_LIST_SELECTED,(wxObjectEventFunction)&pnlSettings::OnlstResolutionPressed);
    Connect(m_plstGranularity->GetId(),wxEVT_LIST_SELECTED,(wxObjectEventFunction)&pnlSettings::OnlstGranularityPressed);

    m_plstInterval->AddButton(wxT("50 ms"), wxNullBitmap, (void*)50);
    m_plstInterval->AddButton(wxT("100 ms"), wxNullBitmap, (void*)100);
    m_plstInterval->AddButton(wxT("250 ms"), wxNullBitmap, (void*)250);
    m_plstInterval->AddButton(wxT("500 ms"), wxNullBitmap, (void*)500);
    m_plstInterval->AddButton(wxT("1000 ms"), wxNullBitmap, (void*)1000);
    m_plstInterval->AddButton(wxT("5000 ms"), wxNullBitmap, (void*)5000);

    m_plstInterval->SelectButton(m_plstInterval->FindButton((void*)Settings::Get().Read(wxT("QoS"), wxT("Interval"), 1000)), true);

    m_plstGranularity->AddButton("Decrease");
    m_plstGranularity->AddButton("Reset");
    m_plstGranularity->AddButton("Increase");

    m_plstResolution->AddButton("Decrease");
    m_plstResolution->AddButton("Reset");
    m_plstResolution->AddButton("Increase");
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

void pnlSettings::OnlstGranularityPressed(wxCommandEvent& event)
{
    m_pBuilder->ChangeGranularity(event.GetInt());
}

void pnlSettings::OnlstResolutionPressed(wxCommandEvent& event)
{
    m_pBuilder->ChangeResolution(event.GetInt());
}
