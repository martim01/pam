#include "MP3Streamerpanel.h"
#include "MP3Streamerbuilder.h"
#include "timedbuffer.h"
#include "session.h"
#include "mp3encoder.h"
#include "testMP3Streamer.h"
#include <thread>
#include "ondemandmp3servermediasubsession.h"
#include "ondemandstreamer.h"
#include "log.h"


//(*InternalHeaders(MP3StreamerPanel)
#include <wx/font.h>
#include <wx/intl.h>
#include <wx/string.h>
//*)

//(*IdInit(MP3StreamerPanel)
const long MP3StreamerPanel::ID_M_PLBL3 = wxNewId();
const long MP3StreamerPanel::ID_M_PBTN3 = wxNewId();
const long MP3StreamerPanel::ID_M_PLBL1 = wxNewId();
const long MP3StreamerPanel::ID_M_PLBL9 = wxNewId();
const long MP3StreamerPanel::ID_M_PLBL2 = wxNewId();
const long MP3StreamerPanel::ID_M_PLST1 = wxNewId();
const long MP3StreamerPanel::ID_PANEL1 = wxNewId();
//*)

wxIMPLEMENT_DYNAMIC_CLASS(MP3StreamerPanel, pmPanel)

BEGIN_EVENT_TABLE(MP3StreamerPanel,pmPanel)
	//(*EventTable(MP3StreamerPanel)
	//*)
END_EVENT_TABLE()


MP3StreamerPanel::MP3StreamerPanel(wxWindow* parent,MP3StreamerBuilder* pBuilder, wxWindowID id,const wxPoint& pos,const wxSize& size) : pmPanel(),
    m_pBuilder(pBuilder),
    m_pUnicastStreamer(nullptr)
{
	//(*Initialize(MP3StreamerPanel)
	Create(parent, id, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL, _T("id"));
	m_pLbl2 = new wmLabel(this, ID_M_PLBL3, _("On Demand MP3 Streamer"), wxPoint(0,0), wxSize(600,38), 0, _T("ID_M_PLBL3"));
	m_pLbl2->SetBorderState(uiRect::BORDER_NONE);
	m_pLbl2->GetUiRect().SetGradient(0);
	m_pLbl2->SetForegroundColour(wxColour(255,255,255));
	m_pLbl2->SetBackgroundColour(wxColour(0,88,159));
	wxFont m_pLbl2Font(16,wxFONTFAMILY_SWISS,wxFONTSTYLE_NORMAL,wxFONTWEIGHT_NORMAL,false,_T("Tahoma"),wxFONTENCODING_DEFAULT);
	m_pLbl2->SetFont(m_pLbl2Font);
	m_pbtnStream = new wmButton(this, ID_M_PBTN3, _("Stream"), wxPoint(500,5), wxSize(200,40), wmButton::STYLE_SELECT, wxDefaultValidator, _T("ID_M_PBTN3"));
	m_pbtnStream->Hide();
	m_pbtnStream->SetBackgroundColour(wxColour(213,0,0));
	m_pbtnStream->SetToggle(true, wxT("Stop"), wxT("Start"), 50);
	m_ppnlUnicast = new wxPanel(this, ID_PANEL1, wxPoint(0,40), wxSize(600,440), wxTAB_TRAVERSAL, _T("ID_PANEL1"));
	m_pLbl1 = new wmLabel(m_ppnlUnicast, ID_M_PLBL1, _("Address"), wxPoint(0,5), wxSize(145,40), 0, _T("ID_M_PLBL1"));
	m_pLbl1->SetBorderState(uiRect::BORDER_NONE);
	m_pLbl1->GetUiRect().SetGradient(0);
	m_pLbl1->SetForegroundColour(wxColour(255,255,255));
	m_pLbl1->SetBackgroundColour(wxColour(0,0,0));
	wxFont m_pLbl1Font(16,wxFONTFAMILY_SWISS,wxFONTSTYLE_NORMAL,wxFONTWEIGHT_NORMAL,false,_T("Tahoma"),wxFONTENCODING_DEFAULT);
	m_pLbl1->SetFont(m_pLbl1Font);
	m_plblUnicastUrl = new wmLabel(m_ppnlUnicast, ID_M_PLBL9, wxEmptyString, wxPoint(146,5), wxSize(454,40), 0, _T("ID_M_PLBL9"));
	m_plblUnicastUrl->SetBorderState(uiRect::BORDER_NONE);
	m_plblUnicastUrl->GetUiRect().SetGradient(0);
	m_plblUnicastUrl->SetForegroundColour(wxColour(255,255,255));
	m_plblUnicastUrl->SetBackgroundColour(wxColour(0,128,64));
	wxFont m_plblUnicastUrlFont(16,wxFONTFAMILY_SWISS,wxFONTSTYLE_NORMAL,wxFONTWEIGHT_NORMAL,false,_T("Tahoma"),wxFONTENCODING_DEFAULT);
	m_plblUnicastUrl->SetFont(m_plblUnicastUrlFont);
	m_plblConnections = new wmLabel(m_ppnlUnicast, ID_M_PLBL2, _("Connections"), wxPoint(0,46), wxSize(600,28), 0, _T("ID_M_PLBL2"));
	m_plblConnections->SetBorderState(uiRect::BORDER_NONE);
	m_plblConnections->GetUiRect().SetGradient(0);
	m_plblConnections->SetForegroundColour(wxColour(0,0,0));
	m_plblConnections->SetBackgroundColour(wxColour(255,255,255));
	wxFont m_plblConnectionsFont(16,wxFONTFAMILY_SWISS,wxFONTSTYLE_NORMAL,wxFONTWEIGHT_NORMAL,false,_T("Tahoma"),wxFONTENCODING_DEFAULT);
	m_plblConnections->SetFont(m_plblConnectionsFont);
	m_plstConnections = new wmList(m_ppnlUnicast, ID_M_PLST1, wxPoint(0,75), wxSize(600,360), 0, 1, wxSize(-1,25), 1, wxSize(-1,-1));
	m_plstConnections->SetBorderStyle(3);
	m_plstConnections->SetButtonColour(wxColour(wxT("#FFFFFF")));
	m_plstConnections->SetTextButtonColour(wxColour(wxT("#000000")));

	Connect(ID_M_PBTN3,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&MP3StreamerPanel::OnbtnStreamClick);
	//*)
    SetSize(800,480);
	Connect(wxEVT_LEFT_UP,(wxObjectEventFunction)&MP3StreamerPanel::OnLeftUp);


    m_nPortCount = 8626;

    m_bStreaming = false;

	SetSize(size);
	SetPosition(pos);
}

MP3StreamerPanel::~MP3StreamerPanel()
{
	//(*Destroy(MP3StreamerPanel)
	//*)
}

void MP3StreamerPanel::SetAudioData(const timedbuffer* pBuffer)
{
    m_Encoder.AddSamples(pBuffer->GetBuffer(), pBuffer->GetBufferSize());
}

void MP3StreamerPanel::InputSession(const session& aSession)
{
    if(aSession.GetCurrentSubsession() != aSession.lstSubsession.end())
    {   //we have a session
        if(!m_pUnicastStreamer || m_Encoder.GetParameters().nChannels != aSession.GetCurrentSubsession()->nChannels || m_Encoder.GetParameters().nSampleRate != aSession.GetCurrentSubsession()->nSampleRate)
        {   //the session has changed or not had one yet

            //Create the Encoder
            MP3Parameters params;
            params.nChannels = aSession.GetCurrentSubsession()->nChannels;
            params.nSampleRate = aSession.GetCurrentSubsession()->nSampleRate;
            m_Encoder.StopLame();
            m_Encoder.InitLame(params);

            pmlLog(pml::LOG_INFO) << "MP3Streamer\t" << "New Session";

            //Create the Unicast Streamer
            if(!m_pUnicastStreamer)
            {
                Connect(wxID_ANY, wxEVT_ODS_ANNOUNCE, (wxObjectEventFunction)&MP3StreamerPanel::OnODSAnnounce);
                Connect(wxID_ANY, wxEVT_ODS_CONNECTION, (wxObjectEventFunction)&MP3StreamerPanel::OnODSConnection);
                Connect(wxID_ANY, wxEVT_ODS_DISCONNECTION, (wxObjectEventFunction)&MP3StreamerPanel::OnODSDisconnection);
                m_pUnicastStreamer = new OnDemandStreamer({this},{this}, "192.168.0.218", m_nPortCount);
                m_pUnicastStreamer->Create();
                m_pUnicastStreamer->SetSubsession(OnDemandMP3MediaSubsession::createNew(this, *m_pUnicastStreamer->envir(), m_Encoder));
                m_pUnicastStreamer->Run();
            }
        }
    }
}

void MP3StreamerPanel::OnODSConnection(wxCommandEvent& event)
{
    m_plstConnections->Freeze();
    m_plstConnections->AddButton(wxString::Format("[0x%X] %s:%04d", event.GetInt(), event.GetString().c_str(), event.GetExtraLong()), wxNullBitmap, (void*)event.GetInt());
    m_plstConnections->Thaw();
}

void MP3StreamerPanel::OnODSDisconnection(wxCommandEvent& event)
{
    m_plstConnections->Freeze();
    m_plstConnections->DeleteButtons((void*)event.GetInt());
    m_plstConnections->Thaw();
}

void MP3StreamerPanel::OutputChannels(const std::vector<char>& vChannels)
{

}


void MP3StreamerPanel::OnLeftUp(wxMouseEvent& event)
{
    m_pBuilder->Maximize((GetSize().x <= 600));
}



void MP3StreamerPanel::OnbtnStreamClick(wxCommandEvent& event)
{
    if(event.IsChecked() && !m_bStreaming)
    {
        m_Encoder.FlushBuffer();
        std::thread t1([this](){stream(m_Encoder);});
        t1.detach();
        m_bStreaming = true;
    }
}


void MP3StreamerPanel::OnODSAnnounce(wxCommandEvent& event)
{
    m_plblUnicastUrl->SetLabel(event.GetString());
}
