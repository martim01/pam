#include "pnldelay.h"
#include "settings.h"
#include "settingevent.h"

//(*InternalHeaders(pnlDelay)
#include <wx/intl.h>
#include <wx/string.h>
//*)
#include "fftdiffbuilder.h"


BEGIN_EVENT_TABLE(pnlDelay,wxPanel)
	//(*EventTable(pnlDelay)
	//*)
END_EVENT_TABLE()

pnlDelay::pnlDelay(wxWindow* parent,fftdiffBuilder* pBuilder, wxWindowID id,const wxPoint& pos,const wxSize& size) :
    m_pBuilder(pBuilder)
{
	Create(parent, id, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL, _T("id"));
	SetBackgroundColour(wxColour(0,0,0));

    m_plstMode = new wmList(this, wxNewId(), wxPoint(5,0), wxSize(190,44), wmList::STYLE_SELECT, wmList::SCROLL_NONE, wxSize(-1,40), 3);
    m_plstMode->AddButton("Off");
    m_plstMode->AddButton("One-Shot");
    m_plstMode->AddButton("Auto");
    m_plstMode->ConnectToSetting(m_pBuilder->GetSection(), "Delay", size_t(0));

    m_pbtnCalculate = new wmButton(this, wxNewId(), _("Calculate"), wxPoint(2,175), wxSize(190,30), wmButton::STYLE_NORMAL, wxDefaultValidator, _T("ID_M_PBTN1"));

    Bind(wxEVT_COMMAND_BUTTON_CLICKED, &pnlDelay::OnbtnCalculateClick, this, m_pbtnCalculate->GetId());

}

pnlDelay::~pnlDelay()
{
	//(*Destroy(pnlDelay)
	//*)
}


void pnlDelay::OnbtnCalculateClick(wxCommandEvent& event)
{
    m_pBuilder->CalculateDelay();
}

