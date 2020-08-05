#include "pnlMeterSettings.h"

//(*InternalHeaders(pnlMeterSettings)
#include <wx/intl.h>
#include <wx/string.h>
//*)

#include "anglemetersbuilder.h"

//(*IdInit(pnlMeterSettings)
const long pnlMeterSettings::ID_M_PBTN1 = wxNewId();
const long pnlMeterSettings::ID_M_PLST2 = wxNewId();
const long pnlMeterSettings::ID_M_PBTN2 = wxNewId();
const long pnlMeterSettings::ID_M_PLBL1 = wxNewId();
//*)

BEGIN_EVENT_TABLE(pnlMeterSettings,wxPanel)
	//(*EventTable(pnlMeterSettings)
	//*)
END_EVENT_TABLE()

pnlMeterSettings::pnlMeterSettings(wxWindow* parent,AngleMetersBuilder* pBuilder, wxWindowID id,const wxPoint& pos,const wxSize& size) :
    m_pBuilder(pBuilder)
{
	//(*Initialize(pnlMeterSettings)
	Create(parent, id, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL, _T("id"));
	SetBackgroundColour(wxColour(0,0,0));
	m_pbtnM3M6 = new wmButton(this, ID_M_PBTN1, _("M+S"), wxPoint(2,0), wxSize(196,35), wmButton::STYLE_SELECT, wxDefaultValidator, _T("ID_M_PBTN1"));
	m_pbtnM3M6->SetForegroundColour(wxColour(255,255,255));
	m_pbtnM3M6->SetBackgroundColour(wxColour(64,0,128));
	m_plstSpeed = new wmList(this, ID_M_PLST2, wxPoint(0,75), wxSize(200,42), wmList::STYLE_SELECT, 0, wxSize(-1,-1), 3, wxSize(5,5));
	m_plstSpeed->SetBackgroundColour(wxColour(0,0,0));
	m_pbtnStereo = new wmButton(this, ID_M_PBTN2, _("Mode"), wxPoint(2,160), wxSize(198,35), wmButton::STYLE_SELECT, wxDefaultValidator, _T("ID_M_PBTN2"));
	m_pbtnStereo->SetForegroundColour(wxColour(255,255,255));
	m_pbtnStereo->SetBackgroundColour(wxColour(0,113,0));
	m_pLbl1 = new wmLabel(this, ID_M_PLBL1, _("Meter Speed"), wxPoint(2,45), wxSize(196,30), 0, _T("ID_M_PLBL1"));
	m_pLbl1->SetBorderState(uiRect::BORDER_NONE);
	m_pLbl1->SetForegroundColour(wxColour(255,255,255));
	m_pLbl1->SetBackgroundColour(wxColour(63,115,192));

	Connect(ID_M_PBTN1,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&pnlMeterSettings::OnbtnM3M6Click);
	Connect(ID_M_PLST2,wxEVT_LIST_SELECTED,(wxObjectEventFunction)&pnlMeterSettings::OnlstSpeedSelected);
	Connect(ID_M_PBTN2,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&pnlMeterSettings::OnbtnStereoClick);
	//*)

	m_plstSpeed->AddButton(wxT("Slow"));
	m_plstSpeed->AddButton(wxT("Normal"));
	m_plstSpeed->AddButton(wxT("Fast"));

	m_pbtnM3M6->SetToggle(true, wxT("M3"), wxT("M6"), 50.0);
	m_pbtnStereo->SetToggle(true, wxT("Mono"), wxT("Stereo"), 50.0);

	m_plstSpeed->SelectButton((m_pBuilder->ReadSetting(wxT("Speed"),1) == 1),true);
	m_pbtnM3M6->ToggleSelection(m_pBuilder->ReadSetting(wxT("M3M6"),1) == 1, true);
	m_pbtnStereo->ToggleSelection(m_pBuilder->ReadSetting(wxT("Stereo"),0) == 1, true);

}

pnlMeterSettings::~pnlMeterSettings()
{
	//(*Destroy(pnlMeterSettings)
	//*)
}


void pnlMeterSettings::OnbtnM3M6Click(wxCommandEvent& event)
{
    if(event.IsChecked())
    {
        m_pBuilder->WriteSetting(wxT("M3M6"),1);
    }
    else
    {
        m_pBuilder->WriteSetting(wxT("M3M6"),0);
    }
}

void pnlMeterSettings::OnlstSpeedSelected(wxCommandEvent& event)
{
    m_pBuilder->WriteSetting(wxT("Speed"), event.GetInt());
}



void pnlMeterSettings::OnbtnStereoClick(wxCommandEvent& event)
{
    m_pBuilder->WriteSetting(wxT("Stereo"), event.IsChecked());
}
