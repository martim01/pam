#include "pnlRecord.h"
#include "timedbuffer.h"
#include "settings.h"
#include <wx/stdpaths.h>
#include "session.h"
#include "settings.h"

//(*InternalHeaders(pnlRecord)
#include <wx/font.h>
#include <wx/intl.h>
#include <wx/string.h>
//*)

//(*IdInit(pnlRecord)
const long pnlRecord::ID_M_PLBL37 = wxNewId();
const long pnlRecord::ID_M_PKBD1 = wxNewId();
const long pnlRecord::ID_M_PLBL1 = wxNewId();
const long pnlRecord::ID_M_PBTN1 = wxNewId();
const long pnlRecord::ID_M_PLBL2 = wxNewId();
const long pnlRecord::ID_M_PLBL3 = wxNewId();
const long pnlRecord::ID_M_PEDT1 = wxNewId();
const long pnlRecord::ID_M_PBTN2 = wxNewId();
const long pnlRecord::ID_M_PBTN3 = wxNewId();
const long pnlRecord::ID_TIMER1 = wxNewId();
//*)

BEGIN_EVENT_TABLE(pnlRecord,wxPanel)
	//(*EventTable(pnlRecord)
	//*)
END_EVENT_TABLE()

pnlRecord::pnlRecord(wxWindow* parent,wxWindowID id,const wxPoint& pos,const wxSize& size)
{
	//(*Initialize(pnlRecord)
	Create(parent, id, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL, _T("id"));
	SetBackgroundColour(wxColour(0,0,0));
	wxFont thisFont(12,wxFONTFAMILY_SWISS,wxFONTSTYLE_NORMAL,wxFONTWEIGHT_NORMAL,false,_T("Arial"),wxFONTENCODING_DEFAULT);
	SetFont(thisFont);
	m_pLbl12 = new wmLabel(this, ID_M_PLBL37, _("Recorder"), wxPoint(0,0), wxSize(600,40), 0, _T("ID_M_PLBL37"));
	m_pLbl12->SetBorderState(uiRect::BORDER_NONE);
	m_pLbl12->SetForegroundColour(wxColour(255,255,255));
	m_pLbl12->SetBackgroundColour(wxColour(91,149,57));
	wxFont m_pLbl12Font(12,wxFONTFAMILY_SWISS,wxFONTSTYLE_NORMAL,wxFONTWEIGHT_BOLD,false,_T("Arial"),wxFONTENCODING_DEFAULT);
	m_pLbl12->SetFont(m_pLbl12Font);
	m_pKbd1 = new wmKeyboard(this, ID_M_PKBD1, wxPoint(0,200), wxSize(600,260), 0, 0);
	m_pKbd1->SetForegroundColour(wxColour(255,255,255));
	wxFont m_pKbd1Font(12,wxFONTFAMILY_SWISS,wxFONTSTYLE_NORMAL,wxFONTWEIGHT_NORMAL,false,_T("Arial"),wxFONTENCODING_DEFAULT);
	m_pKbd1->SetFont(m_pKbd1Font);
	m_pLbl1 = new wmLabel(this, ID_M_PLBL1, _("Filename"), wxPoint(0,50), wxSize(100,40), 0, _T("ID_M_PLBL1"));
	m_pLbl1->SetBorderState(uiRect::BORDER_NONE);
	m_pLbl1->SetForegroundColour(wxColour(255,255,255));
	m_pLbl1->SetBackgroundColour(wxColour(0,0,0));
	m_pbtnRecord = new wmButton(this, ID_M_PBTN1, _("Record"), wxPoint(110,100), wxSize(100,80), 0, wxDefaultValidator, _T("ID_M_PBTN1"));
	m_pbtnRecord->SetBackgroundColour(wxColour(128,0,0));
	m_pbtnRecord->SetColourSelected(wxColour(wxT("#6F0000")));
	m_pbtnRecord->SetColourDisabled(wxColour(wxT("#909090")));
	m_pLbl2 = new wmLabel(this, ID_M_PLBL2, _("File Length"), wxPoint(240,100), wxSize(150,30), 0, _T("ID_M_PLBL2"));
	m_pLbl2->SetBorderState(uiRect::BORDER_NONE);
	m_pLbl2->SetForegroundColour(wxColour(255,255,255));
	m_pLbl2->SetBackgroundColour(wxColour(0,0,0));
	m_plblTime = new wmLabel(this, ID_M_PLBL3, _("00:00:00"), wxPoint(390,100), wxSize(150,30), 0, _T("ID_M_PLBL3"));
	m_plblTime->SetBorderState(uiRect::BORDER_NONE);
	m_plblTime->SetForegroundColour(wxColour(0,255,128));
	m_plblTime->SetBackgroundColour(wxColour(0,0,0));
	m_pedtFile = new wmEdit(this, ID_M_PEDT1, wxEmptyString, wxPoint(101,50), wxSize(270,40), 0, wxDefaultValidator, _T("ID_M_PEDT1"));
	m_pedtFile->SetValidation(24397);
	m_pedtFile->SetForegroundColour(wxColour(0,0,0));
	m_pedtFile->SetBackgroundColour(wxColour(255,255,255));
	m_pedtFile->SetFocusedBackground(wxColour(wxT("#B0FFD8")));
	m_pedtFile->SetFocusedForeground(wxColour(wxT("#000000")));
	m_pedtFile->SetBorderStyle(1,1);
	m_pbtnDate = new wmButton(this, ID_M_PBTN2, _("Date Label"), wxPoint(380,50), wxSize(100,40), 0, wxDefaultValidator, _T("ID_M_PBTN2"));
	m_pbtnClear = new wmButton(this, ID_M_PBTN3, _("Clear"), wxPoint(500,50), wxSize(80,40), 0, wxDefaultValidator, _T("ID_M_PBTN3"));
	m_timerSecond.SetOwner(this, ID_TIMER1);
	m_timerSecond.Start(1000, false);

	Connect(ID_M_PBTN1,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&pnlRecord::OnbtnRecordClick);
	Connect(ID_M_PEDT1,wxEVT_COMMAND_TEXT_UPDATED,(wxObjectEventFunction)&pnlRecord::OnedtFileText);
	Connect(ID_M_PBTN2,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&pnlRecord::OnbtnDateClick);
	Connect(ID_M_PBTN3,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&pnlRecord::OnbtnClearClick);
	Connect(ID_TIMER1,wxEVT_TIMER,(wxObjectEventFunction)&pnlRecord::OntimerSecondTrigger);
	//*)
	m_pedtFile->SetFocus();
	m_pbtnRecord->Enable(false);
	m_bRecording = false;
}

pnlRecord::~pnlRecord()
{
	//(*Destroy(pnlRecord)
	//*)
}


void pnlRecord::OnbtnRecordClick(wxCommandEvent& event)
{
    if(!m_bRecording)
    {
        if(m_sf.OpenToWrite(wxString::Format(wxT("%s/%s.wav"), Settings::Get().GetDocumentDirectory().c_str(), m_pedtFile->GetValue().c_str()), m_nInputChannels, m_nSampleRate, 16))
        {
            m_pbtnRecord->SetLabel(wxT("STOP"));
            m_dtRecording = wxDateTime::Now();
            m_bRecording = true;
            m_pKbd1->Disable();
            m_pedtFile->Disable();
            Settings::Get().Write(wxT("Test"), wxT("Lock"), 1);
        }
    }
    else
    {
        m_sf.Close();
        m_pbtnRecord->SetLabel(wxT("Record"));
        m_bRecording = false;
        m_plblTime->SetLabel(wxEmptyString);
        m_pKbd1->Enable();
        m_pedtFile->Enable();
        Settings::Get().Write(wxT("Test"), wxT("Lock"), 0);
    }
}


void pnlRecord::SetAudioData(const timedbuffer* pBuffer)
{
    if(m_bRecording)
    {
        m_sf.WriteAudio(pBuffer->GetBuffer(), pBuffer->GetBufferSize());

    }
}

void pnlRecord::OntimerSecondTrigger(wxTimerEvent& event)
{
    if(m_bRecording)
    {
        m_plblTime->SetLabel((wxDateTime::Now()-m_dtRecording).Format(wxT("%H:%M:%S")));
    }
}

void pnlRecord::OnbtnDateClick(wxCommandEvent& event)
{
    m_pedtFile->SetValue(wxDateTime::Now().Format(wxT("%Y_%m-%d_%H%M")));
}


void pnlRecord::InputSession(const session& aSession)
{
    if(aSession.itCurrentSubsession != aSession.lstSubsession.end())
    {
        m_nInputChannels = aSession.itCurrentSubsession->nChannels;
        m_nSampleRate = aSession.itCurrentSubsession->nSampleRate;
    }
    else
    {
        m_nInputChannels = 0;
        m_nSampleRate = 48000;
    }

    m_pbtnRecord->Enable((m_nInputChannels>0));
}

void pnlRecord::OnedtFileText(wxCommandEvent& event)
{
    m_pbtnRecord->Enable((m_pedtFile->GetValue().empty() == false));
}

void pnlRecord::OnbtnClearClick(wxCommandEvent& event)
{
    m_pedtFile->SetValue(wxEmptyString);
    m_pbtnRecord->Enable((m_pedtFile->GetValue().empty() == false));
}
