#include "pnloptions.h"
#include "settings.h"
#include "settingevent.h"

//(*InternalHeaders(pnlOptions)
#include <wx/intl.h>
#include <wx/string.h>
//*)
#include "fftdiffbuilder.h"

//(*IdInit(pnlOptions)
const long pnlOptions::ID_M_PBTN1 = wxNewId();
const long pnlOptions::ID_M_PBTN34 = wxNewId();
//*)

BEGIN_EVENT_TABLE(pnlOptions,wxPanel)
	//(*EventTable(pnlOptions)
	//*)
END_EVENT_TABLE()

pnlOptions::pnlOptions(wxWindow* parent,fftdiffBuilder* pBuilder, wxWindowID id,const wxPoint& pos,const wxSize& size) :
    m_pBuilder(pBuilder)
{
	Create(parent, id, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL, _T("id"));
	SetBackgroundColour(wxColour(0,0,0));


    m_pbtnAverage = new wmButton(this, wxNewId(), _("Average"), wxPoint(2,0), wxSize(190,30), wmButton::STYLE_SELECT, wxDefaultValidator, _T("ID_M_PBTN1"));
    m_pbtnAverage->SetToggle(true, "Off", "On", 45);
    m_pbtnAverage->ConnectToSetting(m_pBuilder->GetSection(),"average", true);

    m_pbtnAverageReset = new wmButton(this, wxNewId(), _("Average Reset"), wxPoint(2,35), wxSize(190,30), wmButton::STYLE_NORMAL, wxDefaultValidator, _T("ID_M_PBTN1"));

    m_pbtnMax = new wmButton(this, wxNewId(), _("Max"), wxPoint(2,70), wxSize(190,30), wmButton::STYLE_SELECT, wxDefaultValidator, _T("ID_M_PBTN1"));
    m_pbtnMax->SetToggle(true, "Off", "On", 45);
    m_pbtnMax->ConnectToSetting(m_pBuilder->GetSection(),"max", false);

    m_pbtnMaxReset = new wmButton(this, wxNewId(), _("Max Reset"), wxPoint(2,105), wxSize(190,30), wmButton::STYLE_NORMAL, wxDefaultValidator, _T("ID_M_PBTN1"));

    m_pbtnMin = new wmButton(this, wxNewId(), _("Min"), wxPoint(2,140), wxSize(190,30), wmButton::STYLE_SELECT, wxDefaultValidator, _T("ID_M_PBTN1"));
    m_pbtnMin->SetToggle(true, "Off", "On", 45);
    m_pbtnMin->ConnectToSetting(m_pBuilder->GetSection(),"min", false);

    m_pbtnMinReset = new wmButton(this, wxNewId(), _("Min Reset"), wxPoint(2,175), wxSize(190,30), wmButton::STYLE_NORMAL, wxDefaultValidator, _T("ID_M_PBTN1"));

    Bind(wxEVT_COMMAND_BUTTON_CLICKED, &pnlOptions::OnbtnMaxResetClick, this, m_pbtnMaxReset->GetId());
    Bind(wxEVT_COMMAND_BUTTON_CLICKED, &pnlOptions::OnbtnMinResetClick, this, m_pbtnMinReset->GetId());
    Bind(wxEVT_COMMAND_BUTTON_CLICKED, &pnlOptions::OnbtnAverageResetClick, this, m_pbtnAverageReset->GetId());

}

pnlOptions::~pnlOptions()
{
	//(*Destroy(pnlOptions)
	//*)
}


void pnlOptions::OnbtnMaxResetClick(wxCommandEvent& event)
{
    m_pBuilder->ResetMax();
}

void pnlOptions::OnbtnMinResetClick(wxCommandEvent& event)
{
    m_pBuilder->ResetMin();
}

void pnlOptions::OnbtnAverageResetClick(wxCommandEvent& event)
{
    m_pBuilder->ResetAverage();
}
