#include "pnlmode.h"
#include "settings.h"
#include "settingevent.h"

//(*InternalHeaders(pnlMode)
#include <wx/intl.h>
#include <wx/string.h>
//*)
#include "fftdiffbuilder.h"

//(*IdInit(pnlMode)
const long pnlMode::ID_M_PBTN1 = wxNewId();
const long pnlMode::ID_M_PBTN34 = wxNewId();
//*)

BEGIN_EVENT_TABLE(pnlMode,wxPanel)
	//(*EventTable(pnlMode)
	//*)
END_EVENT_TABLE()

pnlMode::pnlMode(wxWindow* parent,fftdiffBuilder* pBuilder, wxWindowID id,const wxPoint& pos,const wxSize& size) :
    m_pBuilder(pBuilder)
{
	Create(parent, id, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL, _T("id"));
	SetBackgroundColour(wxColour(0,0,0));
	m_pbtnFFT_Hold = new wmButton(this, ID_M_PBTN1, _("Hold"), wxPoint(2,0), wxSize(190,30), wmButton::STYLE_SELECT, wxDefaultValidator, _T("ID_M_PBTN1"));
	m_pbtnFFT_Hold->SetColourSelected(wxColour(wxT("#800000")));
	m_pbtnFFT_Hold->SetToggle(true, "Off", "On", 45);

	m_pbtnFFT_Cursor = new wmButton(this, ID_M_PBTN34, _("Cursor"), wxPoint(2,35), wxSize(190,30), wmButton::STYLE_SELECT, wxDefaultValidator, _T("ID_M_PBTN34"));
	m_pbtnFFT_Cursor->SetColourSelected(wxColour(wxT("#800000")));
	m_pbtnFFT_Cursor->SetToggle(true, "Off", "On", 45);

    m_pbtnFFT_Cursor->ConnectToSetting(m_pBuilder->GetSection(), "Cursor", false);
    m_pbtnFFT_Hold->ConnectToSetting(m_pBuilder->GetSection(), "Hold", false);

}

pnlMode::~pnlMode()
{
	//(*Destroy(pnlMode)
	//*)
}


