#include "pnlOptions.h"
#include "metersbuilder.h"

//(*InternalHeaders(pnlOptions)
#include <wx/intl.h>
#include <wx/string.h>
//*)

//(*IdInit(pnlOptions)
const long pnlOptions::ID_M_PBTN11 = wxNewId();
const long pnlOptions::ID_M_PBTN12 = wxNewId();
const long pnlOptions::ID_M_PLST25 = wxNewId();
//*)

BEGIN_EVENT_TABLE(pnlOptions,wxPanel)
	//(*EventTable(pnlOptions)
	//*)
END_EVENT_TABLE()

pnlOptions::pnlOptions(wxWindow* parent,MetersBuilder* pBuilder, wxWindowID id,const wxPoint& pos,const wxSize& size) :
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

	Connect(ID_M_PBTN11,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&pnlOptions::OnbnFreezeClick);
	Connect(ID_M_PBTN12,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&pnlOptions::OnbtnMeterClearClick);
	Connect(ID_M_PLST25,wxEVT_LIST_SELECTED,(wxObjectEventFunction)&pnlOptions::OnlstMeter_PeaksSelected);
	//*)

	m_plstMeter_Peaks->AddButton(wxT("Hide Peaks"));
	m_plstMeter_Peaks->AddButton(wxT("Show Peaks"));
	m_plstMeter_Peaks->AddButton(wxT("Hold Peaks"));




	//m_pbtnMeterShading->SetToggleLook(true, wxT("Solid"), wxT("Graded"), 50.0);

	m_plstMeter_Peaks->SelectButton(m_pBuilder->ReadSetting(wxT("Peaks"), 1), true);
	m_pbnFreeze->ToggleSelection((m_pBuilder->ReadSetting(wxT("Freeze"),0)==1), false);


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
