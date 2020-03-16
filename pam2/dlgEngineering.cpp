#include "dlgEngineering.h"
#include "settings.h"
#include "images/splash.xpm"

//(*InternalHeaders(dlgEngineering)
#include <wx/font.h>
#include <wx/intl.h>
#include <wx/string.h>
//*)

//(*IdInit(dlgEngineering)
const long dlgEngineering::ID_M_PLBL1 = wxNewId();
const long dlgEngineering::ID_M_PBTN2 = wxNewId();
const long dlgEngineering::ID_M_PBTN1 = wxNewId();
//*)

BEGIN_EVENT_TABLE(dlgEngineering,wxDialog)
	//(*EventTable(dlgEngineering)
	//*)
END_EVENT_TABLE()

dlgEngineering::dlgEngineering(wxWindow* parent,wxWindowID id,const wxPoint& pos,const wxSize& size)
{
	//(*Initialize(dlgEngineering)
	Create(parent, id, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxNO_BORDER, _T("id"));
	SetClientSize(wxSize(800,480));
	Move(wxDefaultPosition);
	SetBackgroundColour(wxColour(128,0,0));
	wxFont thisFont(14,wxFONTFAMILY_SWISS,wxFONTSTYLE_NORMAL,wxFONTWEIGHT_NORMAL,false,_T("Arial"),wxFONTENCODING_DEFAULT);
	SetFont(thisFont);
	m_pLbl1 = new wmLabel(this, ID_M_PLBL1, _("PAM Engineering Menu"), wxPoint(0,0), wxSize(780,50), 0, _T("ID_M_PLBL1"));
	m_pLbl1->SetBorderState(uiRect::BORDER_NONE);
	m_pLbl1->GetUiRect().SetGradient(0);
	m_pLbl1->SetForegroundColour(wxColour(255,255,255));
	wxFont m_pLbl1Font(18,wxFONTFAMILY_SWISS,wxFONTSTYLE_NORMAL,wxFONTWEIGHT_BOLD,false,_T("Arial"),wxFONTENCODING_DEFAULT);
	m_pLbl1->SetFont(m_pLbl1Font);
	m_pbtnReset = new wmButton(this, ID_M_PBTN2, _("Reset Settings"), wxPoint(350,100), wxSize(100,60), 0, wxDefaultValidator, _T("ID_M_PBTN2"));
	m_pbtnReset->Disable();
	wxFont m_pbtnResetFont(12,wxFONTFAMILY_SWISS,wxFONTSTYLE_NORMAL,wxFONTWEIGHT_NORMAL,false,_T("Arial"),wxFONTENCODING_DEFAULT);
	m_pbtnReset->SetFont(m_pbtnResetFont);
	m_pbtnClose = new wmButton(this, ID_M_PBTN1, _("Close"), wxPoint(350,410), wxSize(100,60), 0, wxDefaultValidator, _T("ID_M_PBTN1"));
	m_pbtnClose->Disable();
	wxFont m_pbtnCloseFont(12,wxFONTFAMILY_SWISS,wxFONTSTYLE_NORMAL,wxFONTWEIGHT_NORMAL,false,_T("Arial"),wxFONTENCODING_DEFAULT);
	m_pbtnClose->SetFont(m_pbtnCloseFont);

	Connect(ID_M_PBTN2,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&dlgEngineering::OnbtnResetClick);
	Connect(ID_M_PBTN1,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&dlgEngineering::OnbtnCloseClick);
	//*)
	m_timerClose.SetOwner(this, wxNewId());
	m_timerClose.Start(5000,true);
	Connect(m_timerClose.GetId(), wxEVT_TIMER, (wxObjectEventFunction)&dlgEngineering::OnTimeClose);
}

dlgEngineering::~dlgEngineering()
{
	//(*Destroy(dlgEngineering)
	//*)
}

void dlgEngineering::OnTimeClose(wxTimerEvent& event)
{
    EndModal(wxID_OK);
}


void dlgEngineering::OnbtnCloseClick(wxCommandEvent& event)
{
    EndModal(wxID_OK);
}

void dlgEngineering::OnbtnResetClick(wxCommandEvent& event)
{
    Settings::Get().ResetFile();
}
