#include "pnlDisplay.h"
#include "radarbuilder.h"
//(*InternalHeaders(pnlDisplay)
#include <wx/intl.h>
#include <wx/string.h>
//*)

//(*IdInit(pnlDisplay)
const long pnlDisplay::ID_M_PLST24 = wxNewId();
const long pnlDisplay::ID_M_PLBL2 = wxNewId();
const long pnlDisplay::ID_M_PLST1 = wxNewId();
const long pnlDisplay::ID_M_PLBL1 = wxNewId();
//*)

BEGIN_EVENT_TABLE(pnlDisplay,wxPanel)
	//(*EventTable(pnlDisplay)
	//*)
END_EVENT_TABLE()

pnlDisplay::pnlDisplay(wxWindow* parent,RadarBuilder* pBuilder, wxWindowID id,const wxPoint& pos,const wxSize& size) :
    m_pBuilder(pBuilder)
{
	//(*Initialize(pnlDisplay)
	Create(parent, id, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL, _T("id"));
	SetBackgroundColour(wxColour(0,0,0));
	m_plstTimeframe = new wmList(this, ID_M_PLST24, wxPoint(0,25), wxSize(190,80), wmList::STYLE_SELECT, 0, wxSize(-1,35), 3, wxSize(2,2));
	m_plstTimeframe->SetBackgroundColour(wxColour(0,0,0));
	m_pLbl2 = new wmLabel(this, ID_M_PLBL2, _("Refresh Rate"), wxPoint(0,105), wxSize(200,25), 0, _T("ID_M_PLBL2"));
	m_pLbl2->SetBorderState(uiRect::BORDER_NONE);
	m_pLbl2->SetForegroundColour(wxColour(255,255,255));
	m_pLbl2->SetBackgroundColour(wxColour(64,128,128));
	m_plstPoints = new wmList(this, ID_M_PLST1, wxPoint(0,130), wxSize(190,80), wmList::STYLE_SELECT, 0, wxSize(-1,35), 3, wxSize(2,2));
	m_plstPoints->SetBackgroundColour(wxColour(0,0,0));
	m_pLbl1 = new wmLabel(this, ID_M_PLBL1, _("Timespan"), wxPoint(0,0), wxSize(200,25), 0, _T("ID_M_PLBL1"));
	m_pLbl1->SetBorderState(uiRect::BORDER_NONE);
	m_pLbl1->SetForegroundColour(wxColour(255,255,255));
	m_pLbl1->SetBackgroundColour(wxColour(64,128,128));

	Connect(ID_M_PLST24,wxEVT_LIST_SELECTED,(wxObjectEventFunction)&pnlDisplay::OnlstTimeframeSelected);
	Connect(ID_M_PLST1,wxEVT_LIST_SELECTED,(wxObjectEventFunction)&pnlDisplay::OnlstPointsSelected);
	//*)
    SetBackgroundColour(*wxBLACK);
	m_plstTimeframe->AddButton(wxT("10s"), wxNullBitmap, (void*)10);
	m_plstTimeframe->AddButton(wxT("30s"), wxNullBitmap, (void*)30);
	m_plstTimeframe->AddButton(wxT("1m"), wxNullBitmap, (void*)60);
	m_plstTimeframe->AddButton(wxT("5m"), wxNullBitmap, (void*)300);
	m_plstTimeframe->AddButton(wxT("10m"), wxNullBitmap, (void*)600);
	m_plstTimeframe->AddButton(wxT("30m"), wxNullBitmap, (void*)1800);

	m_plstTimeframe->SelectButton(m_plstTimeframe->FindButton((void*)m_pBuilder->ReadSetting(wxT("Timeframe"),60)), true);

	m_plstPoints->AddButton(wxT("50ms"), wxNullBitmap, (void*)50);
	m_plstPoints->AddButton(wxT("100ms"), wxNullBitmap, (void*)100);
	m_plstPoints->AddButton(wxT("250ms"), wxNullBitmap, (void*)250);
	m_plstPoints->AddButton(wxT("500ms"), wxNullBitmap, (void*)500);
	m_plstPoints->AddButton(wxT("1s"), wxNullBitmap, (void*)1000);
	m_plstPoints->AddButton(wxT("10s"), wxNullBitmap, (void*)10000);

	m_plstPoints->SelectButton(m_plstPoints->FindButton((void*)m_pBuilder->ReadSetting(wxT("RefreshRate"),250)), true);


}

pnlDisplay::~pnlDisplay()
{
	//(*Destroy(pnlDisplay)
	//*)
}



void pnlDisplay::OnlstTimeframeSelected(wxCommandEvent& event)
{
    m_pBuilder->WriteSetting(wxT("Timeframe"), (int)event.GetClientData());
}

void pnlDisplay::OnlstPointsSelected(wxCommandEvent& event)
{
    m_pBuilder->WriteSetting(wxT("RefreshRate"), (int)event.GetClientData());
}
