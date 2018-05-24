#include "pnlIdentify.h"
#include "myprocess.h"
#include "identifybuilder.h"
#include "settings.h"
#include "soundfile.h"

//(*InternalHeaders(pnlIdentify)
#include <wx/font.h>
#include <wx/intl.h>
#include <wx/string.h>
//*)

using namespace std;

//(*IdInit(pnlIdentify)
const long pnlIdentify::ID_M_PLBL37 = wxNewId();
const long pnlIdentify::ID_M_PBTN1 = wxNewId();
const long pnlIdentify::ID_M_PLBL13 = wxNewId();
const long pnlIdentify::ID_M_PLBL7 = wxNewId();
const long pnlIdentify::ID_M_PLBL1 = wxNewId();
const long pnlIdentify::ID_M_PLBL5 = wxNewId();
const long pnlIdentify::ID_M_PLBL8 = wxNewId();
const long pnlIdentify::ID_M_PLBL4 = wxNewId();
const long pnlIdentify::ID_M_PLBL9 = wxNewId();
const long pnlIdentify::ID_M_PLBL3 = wxNewId();
const long pnlIdentify::ID_M_PLBL10 = wxNewId();
const long pnlIdentify::ID_M_PLBL2 = wxNewId();
const long pnlIdentify::ID_M_PLBL11 = wxNewId();
const long pnlIdentify::ID_M_PLBL6 = wxNewId();
const long pnlIdentify::ID_M_PLBL12 = wxNewId();
const long pnlIdentify::ID_PANEL1 = wxNewId();
//*)

DEFINE_EVENT_TYPE(wxEVT_GRACENOTE_BUSY)


BEGIN_EVENT_TABLE(pnlIdentify,wxPanel)
	//(*EventTable(pnlIdentify)
	//*)
END_EVENT_TABLE()

pnlIdentify::pnlIdentify(wxWindow* parent,IdentifyBuilder* pBuilder, wxWindowID id,const wxPoint& pos,const wxSize& size) :
    m_pBuilder(pBuilder)
{
	//(*Initialize(pnlIdentify)
	Create(parent, id, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL, _T("id"));
	SetBackgroundColour(wxColour(0,0,0));
	m_pLbl12 = new wmLabel(this, ID_M_PLBL37, _("Audio Identification"), wxPoint(0,0), wxSize(600,40), 0, _T("ID_M_PLBL37"));
	m_pLbl12->SetBorderState(uiRect::BORDER_NONE);
	m_pLbl12->SetForegroundColour(wxColour(255,255,255));
	m_pLbl12->SetBackgroundColour(wxColour(154,65,52));
	wxFont m_pLbl12Font(12,wxFONTFAMILY_SWISS,wxFONTSTYLE_NORMAL,wxFONTWEIGHT_BOLD,false,_T("Arial"),wxFONTENCODING_DEFAULT);
	m_pLbl12->SetFont(m_pLbl12Font);
	m_pbtnIdentify = new wmButton(this, ID_M_PBTN1, _("Identify"), wxPoint(50,50), wxSize(90,40), 0, wxDefaultValidator, _T("ID_M_PBTN1"));
	m_pbtnIdentify->SetBackgroundColour(wxColour(0,128,64));
	wxFont m_pbtnIdentifyFont(10,wxFONTFAMILY_SWISS,wxFONTSTYLE_NORMAL,wxFONTWEIGHT_BOLD,false,_T("Arial"),wxFONTENCODING_DEFAULT);
	m_pbtnIdentify->SetFont(m_pbtnIdentifyFont);
	m_pbtnIdentify->SetColourDisabled(wxColour(wxT("#505050")));
	m_plblStatus = new wmLabel(this, ID_M_PLBL13, wxEmptyString, wxPoint(150,60), wxSize(300,-1), wxALIGN_LEFT, _T("ID_M_PLBL13"));
	m_plblStatus->SetBorderState(uiRect::BORDER_NONE);
	m_plblStatus->SetForegroundColour(wxColour(255,255,255));
	m_plblStatus->SetBackgroundColour(wxColour(0,0,0));
	Panel2 = new wxPanel(this, ID_PANEL1, wxPoint(50,100), wxSize(480,190), wxTAB_TRAVERSAL, _T("ID_PANEL1"));
	Panel2->SetBackgroundColour(wxColour(255,255,255));
	m_pLbl7 = new wmLabel(Panel2, ID_M_PLBL7, _("Artist:"), wxPoint(5,5), wxDefaultSize, wxALIGN_LEFT, _T("ID_M_PLBL7"));
	m_pLbl7->SetBorderState(uiRect::BORDER_NONE);
	m_pLbl7->SetBackgroundColour(wxColour(255,255,255));
	wxFont m_pLbl7Font(10,wxFONTFAMILY_SWISS,wxFONTSTYLE_NORMAL,wxFONTWEIGHT_BOLD,false,_T("Arial"),wxFONTENCODING_DEFAULT);
	m_pLbl7->SetFont(m_pLbl7Font);
	m_plblArtist = new wmLabel(Panel2, ID_M_PLBL1, wxEmptyString, wxPoint(70,5), wxSize(400,-1), wxALIGN_LEFT, _T("ID_M_PLBL1"));
	m_plblArtist->SetBorderState(uiRect::BORDER_NONE);
	m_plblArtist->SetForegroundColour(wxColour(0,0,0));
	m_plblArtist->SetBackgroundColour(wxColour(255,255,255));
	wxFont m_plblArtistFont(10,wxFONTFAMILY_SWISS,wxFONTSTYLE_NORMAL,wxFONTWEIGHT_NORMAL,false,wxEmptyString,wxFONTENCODING_DEFAULT);
	m_plblArtist->SetFont(m_plblArtistFont);
	m_pLbl5 = new wmLabel(Panel2, ID_M_PLBL5, _("Album:"), wxPoint(5,65), wxDefaultSize, wxALIGN_LEFT, _T("ID_M_PLBL5"));
	m_pLbl5->SetBorderState(uiRect::BORDER_NONE);
	m_pLbl5->SetBackgroundColour(wxColour(255,255,255));
	wxFont m_pLbl5Font(10,wxFONTFAMILY_SWISS,wxFONTSTYLE_NORMAL,wxFONTWEIGHT_BOLD,false,_T("Arial"),wxFONTENCODING_DEFAULT);
	m_pLbl5->SetFont(m_pLbl5Font);
	m_plblAlbum = new wmLabel(Panel2, ID_M_PLBL8, wxEmptyString, wxPoint(70,65), wxSize(400,-1), wxALIGN_LEFT, _T("ID_M_PLBL8"));
	m_plblAlbum->SetBorderState(uiRect::BORDER_NONE);
	m_plblAlbum->SetForegroundColour(wxColour(0,0,0));
	m_plblAlbum->SetBackgroundColour(wxColour(255,255,255));
	wxFont m_plblAlbumFont(10,wxFONTFAMILY_SWISS,wxFONTSTYLE_NORMAL,wxFONTWEIGHT_NORMAL,false,wxEmptyString,wxFONTENCODING_DEFAULT);
	m_plblAlbum->SetFont(m_plblAlbumFont);
	m_pLbl4 = new wmLabel(Panel2, ID_M_PLBL4, _("Track:"), wxPoint(5,95), wxDefaultSize, wxALIGN_LEFT, _T("ID_M_PLBL4"));
	m_pLbl4->SetBorderState(uiRect::BORDER_NONE);
	m_pLbl4->SetBackgroundColour(wxColour(255,255,255));
	wxFont m_pLbl4Font(10,wxFONTFAMILY_SWISS,wxFONTSTYLE_NORMAL,wxFONTWEIGHT_BOLD,false,_T("Arial"),wxFONTENCODING_DEFAULT);
	m_pLbl4->SetFont(m_pLbl4Font);
	m_plblTrack = new wmLabel(Panel2, ID_M_PLBL9, wxEmptyString, wxPoint(70,95), wxSize(400,-1), wxALIGN_LEFT, _T("ID_M_PLBL9"));
	m_plblTrack->SetBorderState(uiRect::BORDER_NONE);
	m_plblTrack->SetForegroundColour(wxColour(0,0,0));
	m_plblTrack->SetBackgroundColour(wxColour(255,255,255));
	wxFont m_plblTrackFont(10,wxFONTFAMILY_SWISS,wxFONTSTYLE_NORMAL,wxFONTWEIGHT_NORMAL,false,wxEmptyString,wxFONTENCODING_DEFAULT);
	m_plblTrack->SetFont(m_plblTrackFont);
	m_pLbl3 = new wmLabel(Panel2, ID_M_PLBL3, _("Title:"), wxPoint(5,35), wxDefaultSize, wxALIGN_LEFT, _T("ID_M_PLBL3"));
	m_pLbl3->SetBorderState(uiRect::BORDER_NONE);
	m_pLbl3->SetBackgroundColour(wxColour(255,255,255));
	wxFont m_pLbl3Font(10,wxFONTFAMILY_SWISS,wxFONTSTYLE_NORMAL,wxFONTWEIGHT_BOLD,false,_T("Arial"),wxFONTENCODING_DEFAULT);
	m_pLbl3->SetFont(m_pLbl3Font);
	m_plblTitle = new wmLabel(Panel2, ID_M_PLBL10, wxEmptyString, wxPoint(70,35), wxSize(400,-1), wxALIGN_LEFT, _T("ID_M_PLBL10"));
	m_plblTitle->SetBorderState(uiRect::BORDER_NONE);
	m_plblTitle->SetForegroundColour(wxColour(0,0,0));
	m_plblTitle->SetBackgroundColour(wxColour(255,255,255));
	wxFont m_plblTitleFont(10,wxFONTFAMILY_SWISS,wxFONTSTYLE_NORMAL,wxFONTWEIGHT_NORMAL,false,wxEmptyString,wxFONTENCODING_DEFAULT);
	m_plblTitle->SetFont(m_plblTitleFont);
	m_pLbl2 = new wmLabel(Panel2, ID_M_PLBL2, _("Duration:"), wxPoint(5,125), wxDefaultSize, wxALIGN_LEFT, _T("ID_M_PLBL2"));
	m_pLbl2->SetBorderState(uiRect::BORDER_NONE);
	m_pLbl2->SetBackgroundColour(wxColour(255,255,255));
	wxFont m_pLbl2Font(10,wxFONTFAMILY_SWISS,wxFONTSTYLE_NORMAL,wxFONTWEIGHT_BOLD,false,_T("Arial"),wxFONTENCODING_DEFAULT);
	m_pLbl2->SetFont(m_pLbl2Font);
	m_plblDuration = new wmLabel(Panel2, ID_M_PLBL11, wxEmptyString, wxPoint(70,125), wxSize(400,-1), wxALIGN_LEFT, _T("ID_M_PLBL11"));
	m_plblDuration->SetBorderState(uiRect::BORDER_NONE);
	m_plblDuration->SetForegroundColour(wxColour(0,0,0));
	m_plblDuration->SetBackgroundColour(wxColour(255,255,255));
	wxFont m_plblDurationFont(10,wxFONTFAMILY_SWISS,wxFONTSTYLE_NORMAL,wxFONTWEIGHT_NORMAL,false,_T("Arial"),wxFONTENCODING_DEFAULT);
	m_plblDuration->SetFont(m_plblDurationFont);
	m_pLbl6 = new wmLabel(Panel2, ID_M_PLBL6, _("Match:"), wxPoint(5,155), wxDefaultSize, wxALIGN_LEFT, _T("ID_M_PLBL6"));
	m_pLbl6->SetBorderState(uiRect::BORDER_NONE);
	m_pLbl6->SetBackgroundColour(wxColour(255,255,255));
	wxFont m_pLbl6Font(10,wxFONTFAMILY_SWISS,wxFONTSTYLE_NORMAL,wxFONTWEIGHT_BOLD,false,_T("Arial"),wxFONTENCODING_DEFAULT);
	m_pLbl6->SetFont(m_pLbl6Font);
	m_plblMatch = new wmLabel(Panel2, ID_M_PLBL12, wxEmptyString, wxPoint(70,155), wxSize(400,-1), wxALIGN_LEFT, _T("ID_M_PLBL12"));
	m_plblMatch->SetBorderState(uiRect::BORDER_NONE);
	m_plblMatch->SetForegroundColour(wxColour(0,0,0));
	m_plblMatch->SetBackgroundColour(wxColour(255,255,255));
	wxFont m_plblMatchFont(10,wxFONTFAMILY_SWISS,wxFONTSTYLE_NORMAL,wxFONTWEIGHT_NORMAL,false,_T("Arial"),wxFONTENCODING_DEFAULT);
	m_plblMatch->SetFont(m_plblMatchFont);

	Connect(ID_M_PBTN1,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&pnlIdentify::OnbtnIdentifyClick);
	//*)
		SetPosition(pos);
	SetSize(size);

	m_pLbl7->GetUiRect().SetGradient(0);
	m_pLbl5->GetUiRect().SetGradient(0);
	m_pLbl4->GetUiRect().SetGradient(0);
	m_pLbl3->GetUiRect().SetGradient(0);
	m_pLbl2->GetUiRect().SetGradient(0);
	m_pLbl6->GetUiRect().SetGradient(0);
	m_plblArtist->GetUiRect().SetGradient(0);
	m_plblAlbum->GetUiRect().SetGradient(0);
	m_plblTrack->GetUiRect().SetGradient(0);
	m_plblTitle->GetUiRect().SetGradient(0);
	m_plblDuration->GetUiRect().SetGradient(0);
	m_plblMatch->GetUiRect().SetGradient(0);

	m_pLbl7->SetTextAlign(wxALIGN_LEFT | wxALIGN_CENTER_VERTICAL);
	m_pLbl5->SetTextAlign(wxALIGN_LEFT | wxALIGN_CENTER_VERTICAL);
	m_pLbl4->SetTextAlign(wxALIGN_LEFT | wxALIGN_CENTER_VERTICAL);
	m_pLbl3->SetTextAlign(wxALIGN_LEFT | wxALIGN_CENTER_VERTICAL);
	m_pLbl2->SetTextAlign(wxALIGN_LEFT | wxALIGN_CENTER_VERTICAL);
	m_pLbl6->SetTextAlign(wxALIGN_LEFT | wxALIGN_CENTER_VERTICAL);
	m_plblArtist->SetTextAlign(wxALIGN_LEFT | wxALIGN_CENTER_VERTICAL);
	m_plblAlbum->SetTextAlign(wxALIGN_LEFT | wxALIGN_CENTER_VERTICAL);
	m_plblTrack->SetTextAlign(wxALIGN_LEFT | wxALIGN_CENTER_VERTICAL);
	m_plblTitle->SetTextAlign(wxALIGN_LEFT | wxALIGN_CENTER_VERTICAL);
	m_plblMatch->SetTextAlign(wxALIGN_LEFT | wxALIGN_CENTER_VERTICAL);
	m_plblDuration->SetTextAlign(wxALIGN_LEFT | wxALIGN_CENTER_VERTICAL);

	Connect(wxID_ANY, wxEVT_PROCESS_FINISHED, (wxObjectEventFunction)&pnlIdentify::OnGracenoteFinished);

	m_bWriting = false;
	m_pProcess = 0;

	m_pSf = new SoundFile();
}

pnlIdentify::~pnlIdentify()
{
	//(*Destroy(pnlIdentify)
	//*)
	delete m_pSf;
}


void pnlIdentify::OnbtnIdentifyClick(wxCommandEvent& event)
{
    wxString sWavFile = m_pBuilder->ReadSetting(wxT("Wav"), wxString::Format(wxT("%s/identify.wav"), Settings::Get().GetDocumentDirectory().c_str()));


    if(m_pSf->OpenToWrite(sWavFile, 2, m_nSampleRate, 16))
    {
        m_plblStatus->SetLabel(wxString::Format(wxT("Recording Audio: %.1f seconds"),0.0));
        m_bWriting = true;
        m_pbtnIdentify->Disable();

        m_plblAlbum->SetLabel(wxEmptyString);
        m_plblArtist->SetLabel(wxEmptyString);
        m_plblTrack->SetLabel(wxEmptyString);
        m_plblTitle->SetLabel(wxEmptyString);
        m_plblDuration->SetLabel(wxEmptyString);
        m_plblMatch->SetLabel(wxEmptyString);

        SendBusy(true);
    }
    else
    {
        m_plblStatus->SetLabel(wxString::Format(wxT("Failed to open file '%s'"), sWavFile.c_str()));
        m_bWriting = false;
    }
}

void pnlIdentify::SetAudioData(const timedbuffer* pBuffer)
{
    if(m_bWriting)
    {
        m_pSf->WriteAudio(pBuffer);//, m_nInputChannels, m_nChannels[0], m_nChannels[1]);

        m_plblStatus->SetLabel(wxString::Format(wxT("Recording Audio: %.1f seconds"),static_cast<double>(m_pSf->GetLength())/134400.0));

        if(m_pSf->GetLength() > m_nSampleRate*28)
        {
            m_pSf->Close();
            m_bWriting = false;
            LaunchGracenote();
        }
    }
}


void pnlIdentify::LaunchGracenote()
{
    if(!m_pProcess)
    {
        m_plblStatus->SetLabel(wxT("Querying Gracenote"));

        wxString sApp = m_pBuilder->ReadSetting(wxT("Application"), wxEmptyString);
        wxString sLicence = m_pBuilder->ReadSetting(wxT("Licence"), wxEmptyString);
        wxString sWavFile = m_pBuilder->ReadSetting(wxT("Wav"), wxEmptyString);
        wxString sPass = m_pBuilder->ReadSetting(wxT("Pass"), wxEmptyString);

        sApp.Replace(wxT("/"), wxT("\\"));
        sLicence.Replace(wxT("/"), wxT("\\"));
        sWavFile.Replace(wxT("/"), wxT("\\"));
        wxString sCmd; //(wxT(".\\musicid_stream.exe 350638583 21062C0B56DC5C080DAC1693D997F576 .\\licence.txt online .\\identify.wav"));
        sCmd << sApp << wxT(" ") << sPass << wxT(" ") << sLicence << wxT(" online ") << sWavFile;

        m_pProcess = new MyProcess(this, sCmd);
        if ( !wxExecute(sCmd, wxEXEC_ASYNC, m_pProcess) )
        {
            m_plblStatus->SetLabel(wxString::Format(wxT("Execution of '%s' failed."), sCmd.c_str()));
            delete m_pProcess;
            m_pProcess = 0;

            SendBusy(false);
        }
    }
}

void pnlIdentify::SendBusy(bool bBusy)
{
    wxCommandEvent eventBusy(wxEVT_GRACENOTE_BUSY);
    eventBusy.SetInt(bBusy);
    wxPostEvent(GetEventHandler(), eventBusy);
}


void pnlIdentify::OnGracenoteFinished(wxCommandEvent& event)
{
    SendBusy(false);
    m_plblStatus->SetLabel(wxEmptyString);
    if(m_pProcess)
    {
        m_plblTitle->SetLabel(m_pProcess->GetResultAttribute(wxT("track_title")));
        m_plblTrack->SetLabel(m_pProcess->GetResultAttribute(wxT("track_number")));

        unsigned long nMs;
        wxString sDuration(m_pProcess->GetResultAttribute(wxT("track_duration")));
        wxString sDurationTime = sDuration.BeforeFirst(wxT(' '));
        wxString sDurationUnit = sDuration.AfterFirst(wxT(' '));
        if(sDurationTime.ToULong(&nMs))
        {
            if(sDurationUnit == wxT("s"))
            {
                nMs *= 1000;
            }
            wxTimeSpan ts(0,0,0,nMs);
            m_plblDuration->SetLabel(ts.Format(wxT("%H:%M:%S:%l")));

        }
        if(m_pProcess->GetResultAttribute(wxT("match_position")).ToULong(&nMs))
        {
            wxTimeSpan ts(0,0,0,nMs);
            m_plblMatch->SetLabel(ts.Format(wxT("%H:%M:%S:%l")));;
        }
        m_plblAlbum->SetLabel(m_pProcess->GetResultAttribute(wxT("album_title")));
        m_plblArtist->SetLabel(m_pProcess->GetResultAttribute(wxT("artist")));
        m_plblTrack->SetLabel(m_pProcess->GetResultAttribute(wxT("album_track")));
        m_plblStatus->SetLabel(m_pProcess->GetResultAttribute(wxT("error")));

        delete m_pProcess;
        m_pProcess = 0;
        m_pbtnIdentify->Enable();
    }
}


void pnlIdentify::OutputChannels(const vector<char>& vChannels)
{
    if(vChannels.size() > 1)
    {
        m_nChannels[0] = vChannels[0];
        m_nChannels[1] = vChannels[1];
    }
}

void pnlIdentify::InputSession(const session& aSession)
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
}
