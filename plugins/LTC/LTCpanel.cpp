#include "LTCpanel.h"
#include "LTCbuilder.h"
#include "timedbuffer.h"
#include "session.h"
#include "ltcdecoder.h"

//(*InternalHeaders(LTCPanel)
#include <wx/font.h>
#include <wx/intl.h>
#include <wx/string.h>
//*)

//(*IdInit(LTCPanel)
const long LTCPanel::ID_M_PLBL6 = wxNewId();
const long LTCPanel::ID_M_PLBL1 = wxNewId();
const long LTCPanel::ID_M_PLBL2 = wxNewId();
const long LTCPanel::ID_M_PLBL9 = wxNewId();
const long LTCPanel::ID_M_PLBL10 = wxNewId();
const long LTCPanel::ID_M_PLBL8 = wxNewId();
const long LTCPanel::ID_M_PLBL3 = wxNewId();
const long LTCPanel::ID_M_PLBL4 = wxNewId();
const long LTCPanel::ID_M_PLBL5 = wxNewId();
//*)

wxIMPLEMENT_DYNAMIC_CLASS(LTCPanel,pmPanel);

BEGIN_EVENT_TABLE(LTCPanel,wxPanel)
	//(*EventTable(LTCPanel)
	//*)
END_EVENT_TABLE()

LTCPanel::LTCPanel(wxWindow* parent,LTCBuilder* pBuilder, wxWindowID id,const wxPoint& pos,const wxSize& size) : pmPanel(),
    m_pBuilder(pBuilder)
{
	//(*Initialize(LTCPanel)
	Create(parent, id, wxDefaultPosition, wxSize(600,480), wxTAB_TRAVERSAL, _T("id"));
	SetBackgroundColour(wxColour(0,0,0));
	m_pLbl1 = new wmLabel(this, ID_M_PLBL6, _("LTC Decoder"), wxPoint(0,0), wxSize(600,40), 0, _T("ID_M_PLBL6"));
	m_pLbl1->SetBorderState(uiRect::BORDER_NONE);
	m_pLbl1->GetUiRect().SetGradient(wxNORTH);
	m_pLbl1->SetForegroundColour(wxColour(255,255,255));
	m_pLbl1->SetBackgroundColour(wxColour(128,64,0));
	wxFont m_pLbl1Font(22,wxFONTFAMILY_SWISS,wxFONTSTYLE_NORMAL,wxFONTWEIGHT_NORMAL,false,_T("Oxygen"),wxFONTENCODING_DEFAULT);
	m_pLbl1->SetFont(m_pLbl1Font);
	m_plblLTCDate = new wmLabel(this, ID_M_PLBL1, _("2018-10-31"), wxPoint(0,50), wxSize(600,50), 0, _T("ID_M_PLBL1"));
	m_plblLTCDate->SetBorderState(uiRect::BORDER_NONE);
	m_plblLTCDate->GetUiRect().SetGradient(0);
	m_plblLTCDate->SetForegroundColour(wxColour(0,255,0));
	wxFont m_plblLTCDateFont(36,wxFONTFAMILY_SWISS,wxFONTSTYLE_NORMAL,wxFONTWEIGHT_NORMAL,false,_T("Courier New"),wxFONTENCODING_DEFAULT);
	m_plblLTCDate->SetFont(m_plblLTCDateFont);
	m_plblLTCTime = new wmLabel(this, ID_M_PLBL2, _("10:18:00.12"), wxPoint(0,100), wxSize(600,50), 0, _T("ID_M_PLBL2"));
	m_plblLTCTime->SetBorderState(uiRect::BORDER_NONE);
	m_plblLTCTime->GetUiRect().SetGradient(0);
	m_plblLTCTime->SetForegroundColour(wxColour(0,255,0));
	wxFont m_plblLTCTimeFont(36,wxFONTFAMILY_SWISS,wxFONTSTYLE_NORMAL,wxFONTWEIGHT_NORMAL,false,_T("Courier New"),wxFONTENCODING_DEFAULT);
	m_plblLTCTime->SetFont(m_plblLTCTimeFont);
	m_pLbl4 = new wmLabel(this, ID_M_PLBL9, _("Raw Data"), wxPoint(0,370), wxSize(600,50), 0, _T("ID_M_PLBL9"));
	m_pLbl4->SetBorderState(uiRect::BORDER_NONE);
	m_pLbl4->GetUiRect().SetGradient(0);
	m_pLbl4->SetForegroundColour(wxColour(255,255,255));
	m_pLbl4->SetBackgroundColour(wxColour(0,0,128));
	wxFont m_pLbl4Font(22,wxFONTFAMILY_SWISS,wxFONTSTYLE_NORMAL,wxFONTWEIGHT_NORMAL,false,_T("Oxygen"),wxFONTENCODING_DEFAULT);
	m_pLbl4->SetFont(m_pLbl4Font);
	m_plblRaw = new wmLabel(this, ID_M_PLBL10, _("10:18:00.12"), wxPoint(0,420), wxSize(600,50), 0, _T("ID_M_PLBL10"));
	m_plblRaw->SetBorderState(uiRect::BORDER_NONE);
	m_plblRaw->GetUiRect().SetGradient(0);
	m_plblRaw->SetForegroundColour(wxColour(0,255,0));
	wxFont m_plblRawFont(8,wxFONTFAMILY_SWISS,wxFONTSTYLE_NORMAL,wxFONTWEIGHT_NORMAL,false,_T("Courier New"),wxFONTENCODING_DEFAULT);
	m_plblRaw->SetFont(m_plblRawFont);
	m_pLbl3 = new wmLabel(this, ID_M_PLBL8, _("LTC Amplitude:"), wxPoint(0,200), wxSize(300,50), 0, _T("ID_M_PLBL8"));
	m_pLbl3->SetBorderState(uiRect::BORDER_NONE);
	m_pLbl3->GetUiRect().SetGradient(0);
	m_pLbl3->SetForegroundColour(wxColour(255,255,255));
	m_pLbl3->SetBackgroundColour(wxColour(0,0,0));
	wxFont m_pLbl3Font(22,wxFONTFAMILY_SWISS,wxFONTSTYLE_NORMAL,wxFONTWEIGHT_NORMAL,false,_T("Courier New"),wxFONTENCODING_DEFAULT);
	m_pLbl3->SetFont(m_pLbl3Font);
	m_pLbl2 = new wmLabel(this, ID_M_PLBL3, _("Frames/Second"), wxPoint(0,250), wxSize(300,50), 0, _T("ID_M_PLBL3"));
	m_pLbl2->SetBorderState(uiRect::BORDER_NONE);
	m_pLbl2->GetUiRect().SetGradient(0);
	m_pLbl2->SetForegroundColour(wxColour(255,255,255));
	m_pLbl2->SetBackgroundColour(wxColour(0,0,0));
	wxFont m_pLbl2Font(22,wxFONTFAMILY_SWISS,wxFONTSTYLE_NORMAL,wxFONTWEIGHT_NORMAL,false,_T("Courier New"),wxFONTENCODING_DEFAULT);
	m_pLbl2->SetFont(m_pLbl2Font);
	m_plblFPS = new wmLabel(this, ID_M_PLBL4, _("-1.4 dbFS"), wxPoint(300,250), wxSize(300,50), wxALIGN_LEFT, _T("ID_M_PLBL4"));
	m_plblFPS->SetBorderState(uiRect::BORDER_NONE);
	m_plblFPS->GetUiRect().SetGradient(0);
	m_plblFPS->SetForegroundColour(wxColour(242,252,131));
	wxFont m_plblFPSFont(22,wxFONTFAMILY_SWISS,wxFONTSTYLE_NORMAL,wxFONTWEIGHT_NORMAL,false,_T("Courier New"),wxFONTENCODING_DEFAULT);
	m_plblFPS->SetFont(m_plblFPSFont);
	m_plblLTCVolume = new wmLabel(this, ID_M_PLBL5, _("-1.4 dbFS"), wxPoint(300,200), wxSize(300,50), wxALIGN_LEFT, _T("ID_M_PLBL5"));
	m_plblLTCVolume->SetBorderState(uiRect::BORDER_NONE);
	m_plblLTCVolume->GetUiRect().SetGradient(0);
	m_plblLTCVolume->SetForegroundColour(wxColour(242,252,131));
	wxFont m_plblLTCVolumeFont(22,wxFONTFAMILY_SWISS,wxFONTSTYLE_NORMAL,wxFONTWEIGHT_NORMAL,false,_T("Courier New"),wxFONTENCODING_DEFAULT);
	m_plblLTCVolume->SetFont(m_plblLTCVolumeFont);
	//*)

	m_plblLTCVolume->SetTextAlign(wxALIGN_LEFT | wxALIGN_CENTER_VERTICAL);

	Connect(wxEVT_LEFT_UP,(wxObjectEventFunction)&LTCPanel::OnLeftUp);

	SetSize(size);
	SetPosition(pos);

	m_nInputChannels = 0;
	m_pDecoder = new LtcDecoder();
}

LTCPanel::~LTCPanel()
{
	//(*Destroy(LTCPanel)
	//*)
}

void LTCPanel::SetAudioData(const timedbuffer* pBuffer)
{
    if(m_pDecoder->DecodeLtc(pBuffer, m_nInputChannels,0))
    {
        m_plblLTCDate->SetLabel(m_pDecoder->GetDate());
        m_plblLTCTime->SetLabel(m_pDecoder->GetTime());

        m_plblLTCVolume->SetLabel(m_pDecoder->GetAmplitude());
        m_plblRaw->SetLabel(m_pDecoder->GetRaw());
        m_plblFPS->SetLabel(m_pDecoder->GetFPS());
    }
}

void LTCPanel::InputSession(const session& aSession)
{
    if(aSession.GetCurrentSubsession() != aSession.lstSubsession.end())
    {
        m_nInputChannels = aSession.GetCurrentSubsession()->nChannels;
    }
    else
    {
        m_nInputChannels = 0;
    }
}

void LTCPanel::OutputChannels(const std::vector<char>& vChannels)
{

}


void LTCPanel::OnLeftUp(wxMouseEvent& event)
{

}


