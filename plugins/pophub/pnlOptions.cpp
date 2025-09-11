#include "pnlOptions.h"

#include <wx/intl.h>
#include <wx/string.h>

#include "pophubbuilder.h"


BEGIN_EVENT_TABLE(pnlOptions,wxPanel)
END_EVENT_TABLE()

pnlOptions::pnlOptions(wxWindow* parent,pophubBuilder* pBuilder, wxWindowID id,const wxPoint& pos,const wxSize& size) :
    m_pBuilder(pBuilder)
{
	Create(parent, id, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL, _T("id"));
	SetBackgroundColour(wxColour(0,0,0));

	m_pbtnM3M6 = new wmButton(this, wxID_ANY, _("M+S"), wxPoint(2,0), wxSize(196,35), wmButton::STYLE_SELECT, wxDefaultValidator, _T("ID_M_PBTN1"));
	m_pbtnM3M6->SetForegroundColour(wxColour(255,255,255));
	m_pbtnM3M6->SetBackgroundColour(wxColour(64,0,128));
	m_pbtnM3M6->SetToggle(true, "M3", "M6", 50.0);
	
	
	m_plblSpeed = new wmLabel(this, wxID_ANY, "Meter Speed", wxPoint(0,40), wxSize(200,25));
	m_plblSpeed->SetBorderState(uiRect::BORDER_NONE);
	m_plblSpeed->SetForegroundColour(wxColour(255,255,255));
	m_plblSpeed->SetBackgroundColour(wxColour(63,115,192));

	m_plstSpeed = new wmList(this, wxID_ANY, wxPoint(0,65), wxSize(200,32), wmList::STYLE_SELECT, 0, wxSize(-1,30), 3, wxSize(2,5));
	m_plstSpeed->SetBackgroundColour(wxColour(0,0,0));

	m_plstSpeed->AddButton("Slow");
	m_plstSpeed->AddButton("Normal");
	m_plstSpeed->AddButton("Fast");
	
	m_plblPeaks = new wmLabel(this, wxID_ANY, "Peaks", wxPoint(0,100), wxSize(200,25));
	m_plblPeaks->SetBorderState(uiRect::BORDER_NONE);
	m_plblPeaks->SetForegroundColour(wxColour(255,255,255));
	m_plblPeaks->SetBackgroundColour(wxColour(63,115,192));

	m_plstMeterPeaks = new wmList(this, wxID_ANY, wxPoint(0,125), wxSize(200,32), wmList::STYLE_SELECT, 0, wxSize(-1,30), 3, wxSize(2,5));
	m_plstMeterPeaks->SetBackgroundColour(wxColour(0,0,0));
	
	m_plstMeterPeaks->AddButton("Hide");
	m_plstMeterPeaks->AddButton("Show");
	m_plstMeterPeaks->AddButton("Hold");
	m_plstMeterPeaks->ConnectToSetting(m_pBuilder->GetSection(), "peaks", size_t(1));

	m_pbtnLabelCurrent = new wmButton(this, wxID_ANY, _("Level"), wxPoint(2,160), wxSize(196,35), wmButton::STYLE_SELECT);
	m_pbtnLabelCurrent->SetForegroundColour(wxColour(255,255,255));
	m_pbtnLabelCurrent->SetBackgroundColour(wxColour(64,0,128));
	m_pbtnLabelCurrent->SetToggle(true, "Hide", "Show", 50.0);
	

	m_pbtnLabelPeak = new wmButton(this, wxID_ANY, _("Peak"), wxPoint(2,197), wxSize(196,35), wmButton::STYLE_SELECT);
	m_pbtnLabelPeak->SetForegroundColour(wxColour(255,255,255));
	m_pbtnLabelPeak->SetBackgroundColour(wxColour(64,0,128));
	m_pbtnLabelPeak->SetToggle(true, "Hide", "Show", 50.0);

	m_pbtnLabelCurrent->ConnectToSetting(m_pBuilder->GetSection(), "display_text_current", false);
	m_pbtnLabelPeak->ConnectToSetting(m_pBuilder->GetSection(), "display_text_peak", false);
	m_pbtnM3M6->ConnectToSetting(m_pBuilder->GetSection(), "m3m6", true);
	m_plstSpeed->ConnectToSetting(m_pBuilder->GetSection(), "speed", size_t(1));

	
	

}

pnlOptions::~pnlOptions()
{
}

