#include "pnlOptions.h"
#include "anglemetersbuilder.h"

//(*InternalHeaders(pnlOptions)
#include <wx/intl.h>
#include <wx/string.h>
//*)

//(*IdInit(pnlOptions)
const long pnlOptions::ID_M_PBTN11 = wxNewId();
const long pnlOptions::ID_M_PBTN12 = wxNewId();
const long pnlOptions::ID_M_PLST25 = wxNewId();
const long pnlOptions::ID_M_PLST1 = wxNewId();
const long pnlOptions::ID_M_PLBL1 = wxNewId();
//*)

BEGIN_EVENT_TABLE(pnlOptions,wxPanel)
	//(*EventTable(pnlOptions)
	//*)
END_EVENT_TABLE()

pnlOptions::pnlOptions(wxWindow* parent,AngleMetersBuilder* pBuilder, wxWindowID id,const wxPoint& pos,const wxSize& size) :
    m_pBuilder(pBuilder)
{
	//(*Initialize(pnlOptions)
	Create(parent, id, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL, _T("id"));
	SetBackgroundColour(wxColour(0,0,0));
	m_pbnFreeze = new wmButton(this, ID_M_PBTN11, _("Freeze Meter"), wxPoint(95,165), wxSize(90,40), wmButton::STYLE_SELECT, wxDefaultValidator, _T("ID_M_PBTN11"));
	m_pbnFreeze->SetColourSelected(wxColour(wxT("#800000")));
	m_pbtnMeterClear = new wmButton(this, ID_M_PBTN12, _("Clear"), wxPoint(2,165), wxSize(90,40), wmButton::STYLE_NORMAL, wxDefaultValidator, _T("ID_M_PBTN12"));
	m_pbtnMeterClear->SetColourSelected(wxColour(wxT("#800000")));
	m_plstMeter_Peaks = new wmList(this, ID_M_PLST25, wxPoint(0,0), wxSize(190,42), wmList::STYLE_SELECT, 0, wxSize(-1,-1), 3, wxSize(5,5));
	m_plstMeter_Peaks->SetBackgroundColour(wxColour(0,0,0));
	m_plstText = new wmList(this, ID_M_PLST1, wxPoint(0,80), wxSize(190,42), wmList::STYLE_SELECT|wmList::STYLE_SELECT_MULTI, 0, wxSize(-1,-1), 3, wxSize(5,5));
	m_plstText->SetBackgroundColour(wxColour(0,0,0));
	m_pLbl1 = new wmLabel(this, ID_M_PLBL1, _("Display Level As Text"), wxPoint(0,50), wxSize(200,0), 0, _T("ID_M_PLBL1"));
	m_pLbl1->SetBorderState(uiRect::BORDER_NONE);
	m_pLbl1->SetForegroundColour(wxColour(255,255,255));
	m_pLbl1->SetBackgroundColour(wxColour(128,0,255));

	Connect(ID_M_PBTN11,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&pnlOptions::OnbnFreezeClick);
	Connect(ID_M_PBTN12,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&pnlOptions::OnbtnMeterClearClick);
	Connect(ID_M_PLST25,wxEVT_LIST_SELECTED,(wxObjectEventFunction)&pnlOptions::OnlstMeter_PeaksSelected);
	Connect(ID_M_PLST1,wxEVT_LIST_SELECTED,(wxObjectEventFunction)&pnlOptions::OnlstTextSelected);
	//*)

	m_plstMeter_Peaks->AddButton(wxT("Hide Peaks"));
	m_plstMeter_Peaks->AddButton(wxT("Show Peaks"));
	m_plstMeter_Peaks->AddButton(wxT("Hold Peaks"));

	m_plstText->AddButton(wxT("Current"));
	m_plstText->AddButton(wxT("Peak"));



	//m_pbtnMeterShading->SetToggleLook(true, wxT("Solid"), wxT("Graded"), 50.0);

	m_plstMeter_Peaks->SelectButton(m_pBuilder->ReadSetting(wxT("Peaks"), 1), true);
	m_pbnFreeze->ToggleSelection((m_pBuilder->ReadSetting(wxT("Freeze"),0)==1), false);

	if(m_pBuilder->ReadSetting(wxT("DisplayText_Current"), 1) == 1)
	{
	    m_plstText->SelectButton(0, true);
	}
	if(m_pBuilder->ReadSetting(wxT("DisplayText_Peak"), 1) == 1)
	{
	    m_plstText->SelectButton(1, true);
	}

}

pnlOptions::~pnlOptions()
{
	//(*Destroy(pnlOptions)
	//*)
}


void pnlOptions::OnlstMeter_PeaksSelected(wxCommandEvent& event)
{
    m_pBuilder->WriteSetting(wxT("Peaks"), event.GetInt());
}

void pnlOptions::OnbnFreezeClick(wxCommandEvent& event)
{
    m_pBuilder->WriteSetting(wxT("Freeze"), event.IsChecked());
}

void pnlOptions::OnbtnMeterClearClick(wxCommandEvent& event)
{
    m_pBuilder->ClearMeter();
}

void pnlOptions::OnlstTextSelected(wxCommandEvent& event)
{
    m_pBuilder->WriteSetting(wxT("DisplayText_Current"), m_plstText->IsSelected(0));
    m_pBuilder->WriteSetting(wxT("DisplayText_Peak"), m_plstText->IsSelected(1));
}
