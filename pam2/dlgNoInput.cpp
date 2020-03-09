#include "dlgNoInput.h"
#include "settings.h"
//(*InternalHeaders(dlgNoInput)
#include <wx/font.h>
#include <wx/intl.h>
#include <wx/string.h>
//*)

//(*IdInit(dlgNoInput)
const long dlgNoInput::ID_M_PBTN1 = wxNewId();
//*)

BEGIN_EVENT_TABLE(dlgNoInput,wxDialog)
	//(*EventTable(dlgNoInput)
	//*)
END_EVENT_TABLE()

dlgNoInput::dlgNoInput(wxWindow* parent,wxWindowID id,const wxPoint& pos,const wxSize& size)
{
	//(*Initialize(dlgNoInput)
	Create(parent, id, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxNO_BORDER, _T("id"));
	SetClientSize(wxSize(100,50));
	Move(wxPoint(40,400));
	SetForegroundColour(wxColour(255,0,0));
	SetBackgroundColour(wxColour(0,0,0));
	wxFont thisFont(24,wxFONTFAMILY_SWISS,wxFONTSTYLE_NORMAL,wxFONTWEIGHT_NORMAL,false,_T("Tahoma"),wxFONTENCODING_DEFAULT);
	SetFont(thisFont);
	m_pbtnInput = new wmButton(this, ID_M_PBTN1, _("No AES! Reset"), wxPoint(0,0), wxSize(100,50), 0, wxDefaultValidator, _T("ID_M_PBTN1"));
	m_pbtnInput->SetForegroundColour(wxColour(255,255,255));
	m_pbtnInput->SetBackgroundColour(wxColour(236,0,0));
	wxFont m_pbtnInputFont(12,wxFONTFAMILY_SWISS,wxFONTSTYLE_NORMAL,wxFONTWEIGHT_BOLD,false,_T("Tahoma"),wxFONTENCODING_DEFAULT);
	m_pbtnInput->SetFont(m_pbtnInputFont);

	Connect(ID_M_PBTN1,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&dlgNoInput::OnbtnInputClick);
	//*)
	m_pbtnInput->SetColourDisabled(wxColour(128,128,128));
	m_timerReset.SetOwner(this, wxNewId());
	Connect(m_timerReset.GetId(), wxEVT_TIMER, (wxObjectEventFunction)&dlgNoInput::OnTimerReset);

	if(Settings::Get().Read(wxT("Input"), wxT("Reset"), false) == true)
	{
	    m_pbtnInput->Hide();
	}
}

dlgNoInput::~dlgNoInput()
{
	//(*Destroy(dlgNoInput)
	//*)
}


void dlgNoInput::OnbtnInputClick(wxCommandEvent& event)
{
    m_sInput = Settings::Get().Read(wxT("Input"), wxT("Type"), wxT("Soundcard"));
    Settings::Get().Write(wxT("Input"), wxT("Type"), wxT("Disabled"));
    m_pbtnInput->Enable(false);
    m_timerReset.Start(2000,true);
}

void dlgNoInput::OnTimerReset(wxTimerEvent& event)
{
    Settings::Get().Write(wxT("Input"), wxT("Type"), m_sInput);
    m_pbtnInput->Enable(true);
}
