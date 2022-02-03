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
	Create(parent, id, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL, _T("id"));
	SetBackgroundColour(wxColour(0,0,0));
	m_pbtnFFT_Hold = new wmButton(this, ID_M_PBTN1, _("Hold"), wxPoint(2,0), wxSize(190,30), wmButton::STYLE_SELECT, wxDefaultValidator, _T("ID_M_PBTN1"));
	m_pbtnFFT_Hold->SetColourSelected(wxColour(wxT("#800000")));
	m_pbtnFFT_Hold->SetToggle(true, "Off", "On");

	m_pbtnFFT_Cursor = new wmButton(this, ID_M_PBTN34, _("Cursor Mode"), wxPoint(2,35), wxSize(190,30), wmButton::STYLE_SELECT, wxDefaultValidator, _T("ID_M_PBTN34"));
	m_pbtnFFT_Cursor->SetColourSelected(wxColour(wxT("#800000")));
	m_pbtnFFT_Cursor->SetToggle(true, "Off", "On");

	m_pbtnFFT_Peak = new wmButton(this, wxNewId(), _("Peak"), wxPoint(2,70), wxSize(190,30), wmButton::STYLE_SELECT, wxDefaultValidator, _T("ID_M_PBTN1"));
    m_pbtnFFT_Peak->SetToggle(true, "Off", "On");
    m_pbtnFFT_Peak->ConnectToSetting(m_pBuilder->GetName(),"peaks", false);

    m_pbtnFFT_PeakReset = new wmButton(this, wxNewId(), _("Peak Reset"), wxPoint(2,105), wxSize(190,40), wmButton::STYLE_NORMAL, wxDefaultValidator, _T("ID_M_PBTN1"));
    Connect(m_pbtnFFT_PeakReset->GetId(),wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&pnlOptions::OnbtnFFT_PeakResetClick);

    m_pbtnFFT_Cursor->ConnectToSetting(m_pBuilder->GetName(), "Cursor", false);
    m_pbtnFFT_Hold->ConnectToSetting(m_pBuilder->GetName(), "Hold", false);

	pBuilder->RegisterForSettingsUpdates(wxT("Cursor"), this);


}

pnlOptions::~pnlOptions()
{
	//(*Destroy(pnlOptions)
	//*)
}


void pnlOptions::OnbtnFFT_PeakResetClick(wxCommandEvent& event)
{
    m_pBuilder->ResetPeaks();
}
