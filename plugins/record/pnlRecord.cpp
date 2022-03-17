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
	m_pLbl1 = new wmLabel(this, ID_M_PLBL1, _("Filename"), wxPoint(10,50), wxSize(90,40), 0, _T("ID_M_PLBL1"));
	m_pLbl1->SetBorderState(uiRect::BORDER_FLAT);
	m_pLbl1->GetUiRect().SetGradient(0);
	m_pLbl1->SetForegroundColour(wxColour(255,255,255));
	m_pLbl1->SetBackgroundColour(wxColour(20,60,90));

	m_pbtnRecord = new wmButton(this, ID_M_PBTN1, _("Record"), wxPoint(103,150), wxSize(110,40), 0, wxDefaultValidator, _T("ID_M_PBTN1"));
	m_pbtnRecord->SetBackgroundColour(wxColour(128,0,0));
	m_pbtnRecord->SetColourSelected(wxColour(wxT("#6F0000")));
	m_pbtnRecord->SetColourDisabled(wxColour(wxT("#909090")));
	m_pLbl2 = new wmLabel(this, ID_M_PLBL2, _("File Length"), wxPoint(240,150), wxSize(150,30), 0, _T("ID_M_PLBL2"));
	m_pLbl2->SetBorderState(uiRect::BORDER_NONE);
	m_pLbl2->GetUiRect().SetGradient(0);
	m_pLbl2->SetForegroundColour(wxColour(255,255,255));
	m_pLbl2->SetBackgroundColour(wxColour(0,0,0));
	m_plblTime = new wmLabel(this, ID_M_PLBL3, _("00:00:00"), wxPoint(390,150), wxSize(150,30), 0, _T("ID_M_PLBL3"));
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

	m_pLblGroup = new wmLabel(this, wxNewId(), _("Channels"), wxPoint(10,97), wxSize(90,40), 0, _T("ID_M_PLBL2"));
	m_pLblGroup->SetBorderState(uiRect::BORDER_FLAT);
	m_pLblGroup->GetUiRect().SetGradient(0);
	m_pLblGroup->SetForegroundColour(wxColour(255,255,255));
	m_pLblGroup->SetBackgroundColour(wxColour(20,60,90));

	m_plstGroup = new wmList(this, wxNewId(), wxPoint(101, 95), wxSize(470,42), wmList::STYLE_SELECT | wmList::STYLE_SELECT_MULTI, wmList::SCROLL_NONE, wxSize(55,40), 8);
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
    std::vector<unsigned char> vChannels;
    unsigned char nChannel = 0;
    for(size_t i = 0; i < m_plstGroup->GetItemCount(); i++)
    {
        int ch = (int)m_plstGroup->GetButtonData(i);
        auto itCount = CH_GROUP_SIZE.find((subsession::enumChannelGrouping(ch)));
        if(itCount != CH_GROUP_SIZE.end())
        {
            if(m_plstGroup->IsSelected(i))
            {
                for(unsigned char nAdd = 0; nAdd < itCount->second; nAdd++)
                {
                    vChannels.push_back(nAdd+nChannel);
                }
            }
            nChannel += itCount->second;
        }
    }
    return vChannels;
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
    EnableRecord();
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
    EnableRecord();
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

        m_nInputChannels = std::min((unsigned int)256 ,aSession.GetCurrentSubsession()->nChannels);
        m_nSampleRate = aSession.GetCurrentSubsession()->nSampleRate;
    }
    else
    {
        m_nInputChannels = 0;
        m_nSampleRate = 48000;
    }

    EnableRecord();
}

void pnlRecord::EnableRecord()
{
    m_pbtnRecord->Enable(m_plstGroup->GetSelectionCount() > 0 && (m_pedtFile->GetValue().Trim()!=wxEmptyString));
}

void pnlRecord::OnedtFileText(wxCommandEvent& event)
{
    EnableRecord();
}

void pnlRecord::OnbtnClearClick(wxCommandEvent& event)
{
    m_pedtFile->SetValue(wxEmptyString);
    EnableRecord();
}
