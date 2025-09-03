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
	m_pbtnM3M6->SetToggle(true, wxT("M3"), wxT("M6"), 50.0);
	
	
	m_pLblSpeed = new wmLabel(this, wxID_ANY, _("Meter Speed"), wxPoint(2,45), wxSize(196,25), 0, _T("ID_M_PLBL1"));
	m_pLblSpeed->SetBorderState(uiRect::BORDER_NONE);
	m_pLblSpeed->SetForegroundColour(wxColour(255,255,255));
	m_pLblSpeed->SetBackgroundColour(wxColour(63,115,192));

	m_plstSpeed = new wmList(this, wxID_ANY, wxPoint(0,72), wxSize(200,37), wmList::STYLE_SELECT, 0, wxSize(-1,35), 3, wxSize(2,5));
	m_plstSpeed->SetBackgroundColour(wxColour(0,0,0));

	m_plstSpeed->AddButton(wxT("Slow"));
	m_plstSpeed->AddButton(wxT("Normal"));
	m_plstSpeed->AddButton(wxT("Fast"));
	
	m_pbtnM3M6->ConnectToSetting(m_pBuilder->GetSection(), "m3m6", true);
	m_plstSpeed->ConnectToSetting(m_pBuilder->GetSection(), "speed", size_t(1));

	m_plstMeterPeaks = new wmList(this, wxID_ANY, wxPoint(0,115), wxSize(200,37), wmList::STYLE_SELECT, 0, wxSize(-1,35), 3, wxSize(2,5));
	m_plstMeterPeaks->SetBackgroundColour(wxColour(0,0,0));

	
	m_pLblText = new wmLabel(this, wxID_ANY, _("Display Level As Text"), wxPoint(0,165), wxSize(200,25), 0, _T("ID_M_PLBL1"));
	m_pLblText->SetBorderState(uiRect::BORDER_NONE);
	m_pLblText->GetUiRect().SetGradient(0);
	m_pLblText->SetForegroundColour(wxColour(255,255,255));
	m_pLblText->SetBackgroundColour(wxColour(63,115,192));

	m_plstText = new wmList(this, wxID_ANY, wxPoint(0,192), wxSize(190,37), wmList::STYLE_SELECT|wmList::STYLE_SELECT_MULTI, 0, wxSize(-1,35), 3, wxSize(2,5));
	m_plstText->SetBackgroundColour(wxColour(0,0,0));
	
	

	Bind(wxEVT_LIST_SELECTED, [this](wxCommandEvent& event)
	{
		m_pBuilder->WriteSetting("display_text_current", m_plstText->IsSelected(0));
    	m_pBuilder->WriteSetting("display_text_peak", m_plstText->IsSelected(1));
	}, m_plstText->GetId());

	
	m_plstMeterPeaks->AddButton("Hide Peaks");
	m_plstMeterPeaks->AddButton("Show Peaks");
	m_plstMeterPeaks->AddButton("Hold Peaks");
	m_plstMeterPeaks->ConnectToSetting(m_pBuilder->GetSection(), "peaks", size_t(1));

	m_plstText->AddButton("Current");
	m_plstText->AddButton("Peak");

	if(m_pBuilder->ReadSetting("display_text_current", 1) == 1)
	{
	    m_plstText->SelectButton(0, true);
	}
	if(m_pBuilder->ReadSetting("display_text_peak", 1) == 1)
	{
	    m_plstText->SelectButton(1, true);
	}

}

pnlOptions::~pnlOptions()
{
}

