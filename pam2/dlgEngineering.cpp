#include "dlgEngineering.h"
#include "settings.h"
#include "images/splash.xpm"

//(*InternalHeaders(dlgEngineering)
#include <wx/font.h>
#include <wx/intl.h>
#include <wx/string.h>
//*)

//(*IdInit(dlgEngineering)
const long dlgEngineering::ID_M_PBTN5 = wxNewId();
const long dlgEngineering::ID_M_PBTN4 = wxNewId();
const long dlgEngineering::ID_M_PBTN3 = wxNewId();
const long dlgEngineering::ID_M_PBTN2 = wxNewId();
const long dlgEngineering::ID_M_PBTN1 = wxNewId();
const long dlgEngineering::ID_PANEL1 = wxNewId();
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
	m_pBackground = new wxBitmapPanel(this, ID_PANEL1, wxPoint(0,0), wxSize(800,480), wxTAB_TRAVERSAL, _T("ID_PANEL1"));
	m_pbtnSoundcard = new wmButton(m_pBackground, ID_M_PBTN5, _("Set Soundcard"), wxPoint(10,410), wxSize(100,60), 0, wxDefaultValidator, _T("ID_M_PBTN5"));
	m_pbtnSoundcard->SetBackgroundColour(wxColour(0,128,64));
	wxFont m_pbtnSoundcardFont(12,wxFONTFAMILY_SWISS,wxFONTSTYLE_NORMAL,wxFONTWEIGHT_NORMAL,false,_T("Arial"),wxFONTENCODING_DEFAULT);
	m_pbtnSoundcard->SetFont(m_pbtnSoundcardFont);
	m_pbtnConfig = new wmButton(m_pBackground, ID_M_PBTN4, _("Raspi Config"), wxPoint(120,410), wxSize(100,60), 0, wxDefaultValidator, _T("ID_M_PBTN4"));
	m_pbtnConfig->Hide();
	m_pbtnConfig->SetBackgroundColour(wxColour(0,128,64));
	wxFont m_pbtnConfigFont(12,wxFONTFAMILY_SWISS,wxFONTSTYLE_NORMAL,wxFONTWEIGHT_NORMAL,false,_T("Arial"),wxFONTENCODING_DEFAULT);
	m_pbtnConfig->SetFont(m_pbtnConfigFont);
	m_pbtnFilesystem = new wmButton(m_pBackground, ID_M_PBTN3, _("Expand Filesystem"), wxPoint(230,410), wxSize(100,60), 0, wxDefaultValidator, _T("ID_M_PBTN3"));
	m_pbtnFilesystem->Hide();
	m_pbtnFilesystem->SetBackgroundColour(wxColour(0,128,64));
	wxFont m_pbtnFilesystemFont(12,wxFONTFAMILY_SWISS,wxFONTSTYLE_NORMAL,wxFONTWEIGHT_NORMAL,false,_T("Arial"),wxFONTENCODING_DEFAULT);
	m_pbtnFilesystem->SetFont(m_pbtnFilesystemFont);
	m_pbtnReset = new wmButton(m_pBackground, ID_M_PBTN2, _("Reset Settings"), wxPoint(340,410), wxSize(100,60), 0, wxDefaultValidator, _T("ID_M_PBTN2"));
	m_pbtnReset->SetBackgroundColour(wxColour(0,128,64));
	wxFont m_pbtnResetFont(12,wxFONTFAMILY_SWISS,wxFONTSTYLE_NORMAL,wxFONTWEIGHT_NORMAL,false,_T("Arial"),wxFONTENCODING_DEFAULT);
	m_pbtnReset->SetFont(m_pbtnResetFont);
	m_pbtnClose = new wmButton(m_pBackground, ID_M_PBTN1, _("Close"), wxPoint(690,410), wxSize(100,60), 0, wxDefaultValidator, _T("ID_M_PBTN1"));
	m_pbtnClose->SetBackgroundColour(wxColour(0,128,64));
	wxFont m_pbtnCloseFont(12,wxFONTFAMILY_SWISS,wxFONTSTYLE_NORMAL,wxFONTWEIGHT_NORMAL,false,_T("Arial"),wxFONTENCODING_DEFAULT);
	m_pbtnClose->SetFont(m_pbtnCloseFont);

	Connect(ID_M_PBTN5,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&dlgEngineering::OnbtnSoundcardClick);
	Connect(ID_M_PBTN4,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&dlgEngineering::OnbtnConfigClick);
	Connect(ID_M_PBTN3,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&dlgEngineering::OnbtnFilesystemClick);
	Connect(ID_M_PBTN2,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&dlgEngineering::OnbtnResetClick);
	Connect(ID_M_PBTN1,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&dlgEngineering::OnbtnCloseClick);
	//*)
	m_pbtnSoundcard->Hide();
	m_pBackground->SetBackgroundBitmap(wxBitmap(splash_xpm));
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


void dlgEngineering::OnbtnSoundcardClick(wxCommandEvent& event)
{
}

void dlgEngineering::OnbtnConfigClick(wxCommandEvent& event)
{
}

void dlgEngineering::OnbtnFilesystemClick(wxCommandEvent& event)
{
}
