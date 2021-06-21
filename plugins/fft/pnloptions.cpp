#include "pnloptions.h"
#include "settings.h"
#include "settingevent.h"

//(*InternalHeaders(pnlOptions)
#include <wx/intl.h>
#include <wx/string.h>
//*)
#include "fftbuilder.h"

//(*IdInit(pnlOptions)
const long pnlOptions::ID_M_PBTN1 = wxNewId();
const long pnlOptions::ID_M_PBTN34 = wxNewId();
//*)

BEGIN_EVENT_TABLE(pnlOptions,wxPanel)
	//(*EventTable(pnlOptions)
	//*)
END_EVENT_TABLE()

pnlOptions::pnlOptions(wxWindow* parent,FFTBuilder* pBuilder, wxWindowID id,const wxPoint& pos,const wxSize& size) :
    m_pBuilder(pBuilder)
{
	//(*Initialize(pnlOptions)
	Create(parent, id, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL, _T("id"));
	SetBackgroundColour(wxColour(0,0,0));
	m_pbtnFFT_Hold = new wmButton(this, ID_M_PBTN1, _("Hold"), wxPoint(2,0), wxSize(90,40), wmButton::STYLE_SELECT, wxDefaultValidator, _T("ID_M_PBTN1"));
	m_pbtnFFT_Hold->SetColourSelected(wxColour(wxT("#800000")));
	m_pbtnFFT_Cursor = new wmButton(this, ID_M_PBTN34, _("Cursor Mode"), wxPoint(100,0), wxSize(90,40), wmButton::STYLE_SELECT, wxDefaultValidator, _T("ID_M_PBTN34"));
	m_pbtnFFT_Cursor->SetColourSelected(wxColour(wxT("#800000")));

	Connect(ID_M_PBTN1,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&pnlOptions::OnbtnFFT_HoldClick);
	Connect(ID_M_PBTN34,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&pnlOptions::OnbtnFFT_CursorClick);
	//*)

	m_pbtnFFT_Peak = new wmButton(this, wxNewId(), _("Peak"), wxPoint(2,50), wxSize(190,30), wmButton::STYLE_SELECT, wxDefaultValidator, _T("ID_M_PBTN1"));
    m_pbtnFFT_Peak->SetToggle(true, "Off", "On");

    m_pbtnFFT_Peak->ToggleSelection((m_pBuilder->ReadSetting("peaks", 0)==1), true);

    m_pbtnFFT_PeakReset = new wmButton(this, wxNewId(), _("Peak Reset"), wxPoint(2,85), wxSize(190,40), wmButton::STYLE_NORMAL, wxDefaultValidator, _T("ID_M_PBTN1"));

    Connect(m_pbtnFFT_Peak->GetId(),wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&pnlOptions::OnbtnFFT_PeakClick);
    Connect(m_pbtnFFT_PeakReset->GetId(),wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&pnlOptions::OnbtnFFT_PeakResetClick);


	pBuilder->RegisterForSettingsUpdates(wxT("Cursor"), this);

	Connect(wxID_ANY, wxEVT_SETTING_CHANGED, (wxObjectEventFunction)&pnlOptions::OnSettingChanged);

}

pnlOptions::~pnlOptions()
{
	//(*Destroy(pnlOptions)
	//*)
}


void pnlOptions::OnbtnFFT_HoldClick(wxCommandEvent& event)
{
    m_pBuilder->OnHoldClicked(event);
}

void pnlOptions::OnbtnFFT_CursorClick(wxCommandEvent& event)
{
    m_pBuilder->OnCursorMode(event.IsChecked());
}


void pnlOptions::OnSettingChanged(SettingEvent& event)
{
    if(event.GetKey() == wxT("Cursor"))
    {
        m_pbtnFFT_Cursor->ToggleSelection((m_pBuilder->ReadSetting(wxT("Cursor"),0)==1), false);
    }
}


void pnlOptions::OnbtnFFT_PeakClick(wxCommandEvent& event)
{
    m_pBuilder->WriteSetting("peaks",event.IsChecked());
}

void pnlOptions::OnbtnFFT_PeakResetClick(wxCommandEvent& event)
{
    m_pBuilder->ResetPeaks();
}
