#include "pnlPhase.h"
#include "pophubbuilder.h"

#include <wx/intl.h>
#include <wx/string.h>

#include "settings.h"

BEGIN_EVENT_TABLE(pnlPhase,wxPanel)
END_EVENT_TABLE()

pnlPhase::pnlPhase(wxWindow* parent,pophubBuilder* pBuilder, wxWindowID id,const wxPoint& pos,const wxSize& size) :
    m_pBuilder(pBuilder)
{
	Create(parent, id, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL, _T("id"));
	SetBackgroundColour(wxColour(0,0,0));

	m_pbtnDisplay = new wmButton(this, wxID_ANY, "Display", wxPoint(2,0), wxSize(196,30), wmButton::STYLE_SELECT);
	m_pbtnDisplay->SetForegroundColour(wxColour(255,255,255));
	m_pbtnDisplay->SetBackgroundColour(wxColour(64,0,128));
	m_pbtnDisplay->SetToggle(true, "Label", "Meter", 50.0);
	m_pbtnDisplay->ConnectToSetting(pBuilder->GetSection(), "phase_display", true);

	m_plblThresholdTitle = new wmLabel(this, wxID_ANY, "Threshold", wxPoint(2,37), wxSize(196,25), 0, _T("ID_M_PLBL1"));
	m_plblThresholdTitle->SetBorderState(uiRect::BORDER_NONE);
	m_plblThresholdTitle->SetForegroundColour(wxColour(255,255,255));
	m_plblThresholdTitle->SetBackgroundColour(wxColour(63,115,192));

	m_plstThresholds = new wmList(this, wxID_ANY, wxPoint(0,65), wxSize(200,185), wmList::STYLE_SELECT, 0, wxSize(-1,30), 4, wxSize(5,5));
	m_plstThresholds->SetBackgroundColour(wxColour(0,0,0));
	m_plstThresholds->AddButton("-0.9");
	m_plstThresholds->AddButton("-0.8");
	m_plstThresholds->AddButton("-0.7");
	m_plstThresholds->AddButton("-0.6");
	m_plstThresholds->AddButton("-0.5");
	m_plstThresholds->AddButton("-0.4");
	m_plstThresholds->AddButton("-0.3");
	m_plstThresholds->AddButton("-0.2");
	m_plstThresholds->AddButton("-0.1");
	m_plstThresholds->AddButton("0");
	m_plstThresholds->AddButton("0.1");
	m_plstThresholds->AddButton("0.2");
	m_plstThresholds->AddButton("0.3");
	m_plstThresholds->AddButton("0.4");

	m_plstThresholds->ConnectToSetting(m_pBuilder->GetSection(), "phase_threshold", "-0.5");
}

pnlPhase::~pnlPhase()
{
	//(*Destroy(pnlPhase)
	//*)
}


