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

    m_plstMode = new wmList(this, wxNewId(), wxPoint(5,0), wxSize(190,34), wmList::STYLE_SELECT, wmList::SCROLL_NONE, wxSize(-1,30), 3);
    m_plstMode->AddButton("Off");
    m_plstMode->AddButton("One-Shot");
    m_plstMode->AddButton("Auto");
    m_plstMode->SetBackgroundColour(*wxBLACK);
    m_plstMode->ConnectToSetting(m_pBuilder->GetSection(), "DelayMode", size_t(0));

    m_plblWindow = new wmLabel(this, wxNewId(), "Maximum Possible Delay", wxPoint(5,40), wxSize(190,25));
    m_plblWindow->SetBackgroundColour(wxColour(80,90,140));
    m_plblWindow->SetForegroundColour(*wxWHITE);

    m_plstWindow = new wmList(this, wxNewId(), wxPoint(5,65), wxSize(190,64), wmList::STYLE_SELECT, wmList::SCROLL_NONE, wxSize(-1,30), 4);

    m_plstWindow->AddButton("50ms", wxNullBitmap, (void*)50);
    m_plstWindow->AddButton("100ms", wxNullBitmap, (void*)100);
    m_plstWindow->AddButton("250ms", wxNullBitmap, (void*)250);
    m_plstWindow->AddButton("500ms", wxNullBitmap, (void*)500);
    m_plstWindow->AddButton("1s", wxNullBitmap, (void*)1000);
    m_plstWindow->AddButton("2s", wxNullBitmap, (void*)2000);
    m_plstWindow->AddButton("5s", wxNullBitmap, (void*)5000);
    m_plstWindow->AddButton("10s", wxNullBitmap, (void*)10000);
    m_plstWindow->SetBackgroundColour(*wxBLACK);
    m_plstWindow->ConnectToSetting(m_pBuilder->GetSection(), "DelayWindow", (void*)50);

    m_plblAccuracy = new wmLabel(this, wxNewId(), "Alignment Accuracy", wxPoint(5,135), wxSize(190,25));
    m_plblAccuracy->SetBackgroundColour(wxColour(80,90,140));
    m_plblAccuracy->SetForegroundColour(*wxWHITE);
    m_plstAccuracy = new wmList(this, wxNewId(), wxPoint(5,160), wxSize(190,34), wmList::STYLE_SELECT, wmList::SCROLL_NONE, wxSize(-1,30), 5);
    m_plstAccuracy->AddButton("1", wxNullBitmap, (void*)1);
    m_plstAccuracy->AddButton("12", wxNullBitmap, (void*)12);
    m_plstAccuracy->AddButton("24", wxNullBitmap, (void*)24);
    m_plstAccuracy->AddButton("48", wxNullBitmap, (void*)48);
    m_plstAccuracy->AddButton("96", wxNullBitmap, (void*)96);
    m_plstAccuracy->SetBackgroundColour(*wxBLACK);
    m_plstAccuracy->ConnectToSetting(m_pBuilder->GetSection(), "DelayAccuracy", (void*)50);

    m_pbtnCalculate = new wmButton(this, wxNewId(), _("Calculate"), wxPoint(5,200), wxSize(190,30), wmButton::STYLE_NORMAL, wxDefaultValidator, _T("ID_M_PBTN1"));

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

