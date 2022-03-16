#include "pnlRecord.h"
#include "timedbuffer.h"
#include "settings.h"
#include <wx/stdpaths.h>
#include <wx/filename.h>
#include "session.h"
#include "settings.h"
#include "recordthread.h"

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
	Create(parent, id, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL, _T("id"));
	SetBackgroundColour(wxColour(0,0,0));
	wxFont thisFont(12,wxFONTFAMILY_SWISS,wxFONTSTYLE_NORMAL,wxFONTWEIGHT_NORMAL,false,_T("Arial"),wxFONTENCODING_DEFAULT);
	SetFont(thisFont);
	m_pLbl12 = new wmLabel(this, ID_M_PLBL37, _("Recorder"), wxPoint(0,0), wxSize(600,40), 0, _T("ID_M_PLBL37"));
	m_pLbl12->SetBorderState(uiRect::BORDER_NONE);
	m_pLbl12->GetUiRect().SetGradient(0);
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
	m_pLbl1->GetUiRect().SetGradient(0);
	m_pLbl1->SetForegroundColour(wxColour(255,255,255));
	m_pLbl1->SetBackgroundColour(wxColour(0,0,0));
	m_pbtnRecord = new wmButton(this, ID_M_PBTN1, _("Record"), wxPoint(10,100), wxSize(100,80), 0, wxDefaultValidator, _T("ID_M_PBTN1"));
	m_pbtnRecord->SetBackgroundColour(wxColour(128,0,0));
	m_pbtnRecord->SetColourSelected(wxColour(wxT("#6F0000")));
	m_pbtnRecord->SetColourDisabled(wxColour(wxT("#909090")));
	m_pLbl2 = new wmLabel(this, ID_M_PLBL2, _("File Length"), wxPoint(240,100), wxSize(150,30), 0, _T("ID_M_PLBL2"));
	m_pLbl2->SetBorderState(uiRect::BORDER_NONE);
	m_pLbl2->GetUiRect().SetGradient(0);
	m_pLbl2->SetForegroundColour(wxColour(255,255,255));
	m_pLbl2->SetBackgroundColour(wxColour(0,0,0));
	m_plblTime = new wmLabel(this, ID_M_PLBL3, _("00:00:00"), wxPoint(390,100), wxSize(150,30), 0, _T("ID_M_PLBL3"));
	m_plblTime->SetBorderState(uiRect::BORDER_NONE);
	m_plblTime->GetUiRect().SetGradient(0);
	m_plblTime->SetForegroundColour(wxColour(0,255,128));
	m_plblTime->SetBackgroundColour(wxColour(0,0,0));
	m_pedtFile = new wmEdit(this, ID_M_PEDT1, wxEmptyString, wxPoint(101,50), wxSize(270,40), 0, wxDefaultValidator, _T("ID_M_PEDT1"));
	m_pedtFile->SetValidation(0);
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

	m_plstGroup = new wmList(this, wxNewId(), wxPoint(120, 140), wxSize(470,42), wmList::STYLE_SELECT | wmList::STYLE_SELECT_MULTI, wmList::SCROLL_NONE, wxSize(-1,40), 8);
    m_plstGroup->SetBackgroundColour(*wxBLACK);

    Bind(wxEVT_LIST_SELECTED, &pnlRecord::OnlstGroupSelected, this , m_plstGroup->GetId());
	m_pedtFile->SetFocus();
	m_pbtnRecord->Enable(false);
	m_pRecorder = nullptr;
}

pnlRecord::~pnlRecord()
{
	//(*Destroy(pnlRecord)
	//*)
	if(m_pRecorder)
	{
        m_pRecorder->Stop();
        m_pRecorder->Wait();
	}

}

std::vector<unsigned char> pnlRecord::GetChannels()
{
    wxArrayInt ai = m_plstGroup->GetSelectedButtons();
    for(size_t i = 0; i < ai.GetCount(); i++)
    {
        switch(reinterpret_cast<subsession::enumChannelGrouping>(m_plstGroup->GetButtonData(ai[i])))
        {
            case subsession::enumChannelGrouping::M:
            case subsession::enumChannelGrouping::U01:
                break;
            case subsession::enumChannelGrouping::DM:
            case subsession::enumChannelGrouping::ST:
            case subsession::enumChannelGrouping::U02:
            case subsession::enumChannelGrouping::LtRt:
                break;
            case subsession::enumChannelGrouping::U03:
                break;
            case subsession::enumChannelGrouping::SGRP:
            case subsession::enumChannelGrouping::U04:
                break;
            case subsession::enumChannelGrouping::U05:
                break;
            case subsession::enumChannelGrouping::FIVE1:
            case subsession::enumChannelGrouping::U06:
                break;
            case subsession::enumChannelGrouping::U07:
                break;
            case subsession::enumChannelGrouping::SEVEN1:
            case subsession::enumChannelGrouping::U08:
                break;
        }
    }
}

void pnlRecord::OnbtnRecordClick(wxCommandEvent& event)
{
    if(!m_pRecorder)
    {
        m_pRecorder = new RecordThread();
        if(wxFileName::DirExists(Settings::Get().GetWavDirectory()) == false)
        {
            wxFileName::Mkdir(Settings::Get().GetWavDirectory());
        }



        if(m_pRecorder->Init(wxString::Format(wxT("%s/%s.wav"), Settings::Get().GetWavDirectory().c_str(), m_pedtFile->GetValue().c_str()), GetChannels(), m_nSampleRate, 16))
        {
            m_pbtnRecord->SetLabel(wxT("STOP"));
            m_dtRecording = wxDateTime::Now();
            m_pKbd1->Disable();
            m_pedtFile->Disable();
            Settings::Get().Write(wxT("Test"), wxT("Lock"), 1);
            m_pRecorder->Run();
        }
        else
        {
            delete m_pRecorder;
            m_pRecorder = nullptr;
        }
    }
    else
    {
        m_pRecorder->Stop();
        m_pRecorder->Wait();
        m_pRecorder = nullptr;

        m_pbtnRecord->SetLabel(wxT("Record"));
        m_plblTime->SetLabel(wxEmptyString);
        m_pKbd1->Enable();
        m_pedtFile->Enable();
        Settings::Get().Write(wxT("Test"), wxT("Lock"), 0);
    }
}

void pnlRecord::OnlstGroupSelected(wxCommandEvent& event)
{
    m_pbtnRecord->Enable((m_nInputChannels>0) && m_plstGroup->GetSelectionCount() > 0);
}

void pnlRecord::SetAudioData(const timedbuffer* pBuffer)
{
    if(m_pRecorder)
    {
        m_pRecorder->AddToBuffer(pBuffer);
    }
}

void pnlRecord::OntimerSecondTrigger(wxTimerEvent& event)
{
    if(m_pRecorder)
    {
        m_plblTime->SetLabel((wxDateTime::Now()-m_dtRecording).Format(wxT("%H:%M:%S")));
    }
}

void pnlRecord::OnbtnDateClick(wxCommandEvent& event)
{
   m_pedtFile->SetValue(wxDateTime::Now().Format(wxT("%Y_%m-%d_%H%M")));
    m_pbtnRecord->Enable();
}


void pnlRecord::InputSession(const session& aSession)
{
    m_plstGroup->Clear();
    if(aSession.GetCurrentSubsession() != aSession.lstSubsession.end())
    {
        unsigned char nGroup(0xFF);
        for(size_t i = 0; i < aSession.GetCurrentSubsession()->vChannels.size(); i++)
        {
            if(aSession.GetCurrentSubsession()->vChannels[i].nId != nGroup)
            {
                m_plstGroup->AddButton(GetChannelGroupName(aSession.GetCurrentSubsession()->vChannels[i]), wxNullBitmap, (void*)aSession.GetCurrentSubsession()->vChannels[i].grouping);
                nGroup = aSession.GetCurrentSubsession()->vChannels[i].nId;
            }
        }
        m_plstGroup->SelectButton(0);

        m_nInputChannels = std::min((unsigned int)256 ,aSession.GetCurrentSubsession()->nChannels);
        m_nSampleRate = aSession.GetCurrentSubsession()->nSampleRate;
    }
    else
    {
        m_nInputChannels = 0;
        m_nSampleRate = 48000;
    }

    m_pbtnRecord->Enable((m_nInputChannels>0) && m_plstGroup->GetSelectionCount() > 0);
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
