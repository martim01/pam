#include "LTCpanel.h"
#include "LTCbuilder.h"
#include "timedbuffer.h"
#include "session.h"
#include "ltcdecoder.h"
#include "settingevent.h"

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
const long LTCPanel::ID_M_PLBL7 = wxNewId();
const long LTCPanel::ID_M_PLBL16 = wxNewId();
const long LTCPanel::ID_M_PLBL17 = wxNewId();
const long LTCPanel::ID_M_PLBL11 = wxNewId();
const long LTCPanel::ID_M_PLBL8 = wxNewId();
const long LTCPanel::ID_M_PLBL3 = wxNewId();
const long LTCPanel::ID_M_PLBL12 = wxNewId();
const long LTCPanel::ID_M_PLBL14 = wxNewId();
const long LTCPanel::ID_M_PLBL13 = wxNewId();
const long LTCPanel::ID_M_PLBL18 = wxNewId();
const long LTCPanel::ID_M_PLST1 = wxNewId();
const long LTCPanel::ID_M_PLBL15 = wxNewId();
const long LTCPanel::ID_M_PLBL19 = wxNewId();
const long LTCPanel::ID_M_PLST2 = wxNewId();
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
	m_plblLTCDate = new wmLabel(this, ID_M_PLBL1, _("2018-10-31"), wxPoint(0,50), wxSize(450,30), 0, _T("ID_M_PLBL1"));
	m_plblLTCDate->SetBorderState(uiRect::BORDER_NONE);
	m_plblLTCDate->GetUiRect().SetGradient(0);
	m_plblLTCDate->SetForegroundColour(wxColour(0,255,0));
	wxFont m_plblLTCDateFont(22,wxFONTFAMILY_SWISS,wxFONTSTYLE_NORMAL,wxFONTWEIGHT_NORMAL,false,_T("Courier New"),wxFONTENCODING_DEFAULT);
	m_plblLTCDate->SetFont(m_plblLTCDateFont);
	m_plblLTCTime = new wmLabel(this, ID_M_PLBL2, _("10:18:00.12"), wxPoint(0,80), wxSize(450,30), 0, _T("ID_M_PLBL2"));
	m_plblLTCTime->SetBorderState(uiRect::BORDER_NONE);
	m_plblLTCTime->GetUiRect().SetGradient(0);
	m_plblLTCTime->SetForegroundColour(wxColour(0,255,0));
	wxFont m_plblLTCTimeFont(22,wxFONTFAMILY_SWISS,wxFONTSTYLE_NORMAL,wxFONTWEIGHT_NORMAL,false,_T("Courier New"),wxFONTENCODING_DEFAULT);
	m_plblLTCTime->SetFont(m_plblLTCTimeFont);
	m_pLbl4 = new wmLabel(this, ID_M_PLBL9, _("Raw Data"), wxPoint(0,390), wxSize(600,30), 0, _T("ID_M_PLBL9"));
	m_pLbl4->SetBorderState(uiRect::BORDER_NONE);
	m_pLbl4->GetUiRect().SetGradient(0);
	m_pLbl4->SetForegroundColour(wxColour(255,255,255));
	m_pLbl4->SetBackgroundColour(wxColour(0,0,128));
	wxFont m_pLbl4Font(18,wxFONTFAMILY_SWISS,wxFONTSTYLE_NORMAL,wxFONTWEIGHT_NORMAL,false,_T("Oxygen"),wxFONTENCODING_DEFAULT);
	m_pLbl4->SetFont(m_pLbl4Font);
	m_plblRaw = new wmLabel(this, ID_M_PLBL10, _("10:18:00.12"), wxPoint(0,420), wxSize(600,50), 0, _T("ID_M_PLBL10"));
	m_plblRaw->SetBorderState(uiRect::BORDER_NONE);
	m_plblRaw->GetUiRect().SetGradient(0);
	m_plblRaw->SetForegroundColour(wxColour(0,255,0));
	wxFont m_plblRawFont(8,wxFONTFAMILY_SWISS,wxFONTSTYLE_NORMAL,wxFONTWEIGHT_NORMAL,false,_T("Courier New"),wxFONTENCODING_DEFAULT);
	m_plblRaw->SetFont(m_plblRawFont);
	m_pLblModeTitle = new wmLabel(this, ID_M_PLBL7, _("UserData:"), wxPoint(0,120), wxSize(200,30), 0, _T("ID_M_PLBL7"));
	m_pLblModeTitle->SetBorderState(uiRect::BORDER_NONE);
	m_pLblModeTitle->GetUiRect().SetGradient(0);
	m_pLblModeTitle->SetForegroundColour(wxColour(255,255,255));
	m_pLblModeTitle->SetBackgroundColour(wxColour(0,0,0));
	wxFont m_pLblModeTitleFont(22,wxFONTFAMILY_SWISS,wxFONTSTYLE_NORMAL,wxFONTWEIGHT_NORMAL,false,_T("Courier New"),wxFONTENCODING_DEFAULT);
	m_pLblModeTitle->SetFont(m_pLblModeTitleFont);
	m_pLblDateTitle = new wmLabel(this, ID_M_PLBL16, _("Date Format:"), wxPoint(0,150), wxSize(200,30), 0, _T("ID_M_PLBL16"));
	m_pLblDateTitle->SetBorderState(uiRect::BORDER_NONE);
	m_pLblDateTitle->GetUiRect().SetGradient(0);
	m_pLblDateTitle->SetForegroundColour(wxColour(255,255,255));
	m_pLblDateTitle->SetBackgroundColour(wxColour(0,0,0));
	wxFont m_pLblDateTitleFont(22,wxFONTFAMILY_SWISS,wxFONTSTYLE_NORMAL,wxFONTWEIGHT_NORMAL,false,_T("Courier New"),wxFONTENCODING_DEFAULT);
	m_pLblDateTitle->SetFont(m_pLblDateTitleFont);
	m_plblDateFormat = new wmLabel(this, ID_M_PLBL17, _("BBC"), wxPoint(200,150), wxSize(250,30), wxALIGN_LEFT, _T("ID_M_PLBL17"));
	m_plblDateFormat->SetBorderState(uiRect::BORDER_NONE);
	m_plblDateFormat->GetUiRect().SetGradient(0);
	m_plblDateFormat->SetForegroundColour(wxColour(242,252,131));
	wxFont m_plblDateFormatFont(22,wxFONTFAMILY_SWISS,wxFONTSTYLE_NORMAL,wxFONTWEIGHT_NORMAL,false,_T("Courier New"),wxFONTENCODING_DEFAULT);
	m_plblDateFormat->SetFont(m_plblDateFormatFont);
	m_plblMode = new wmLabel(this, ID_M_PLBL11, _("BBC"), wxPoint(200,120), wxSize(250,30), wxALIGN_LEFT, _T("ID_M_PLBL11"));
	m_plblMode->SetBorderState(uiRect::BORDER_NONE);
	m_plblMode->GetUiRect().SetGradient(0);
	m_plblMode->SetForegroundColour(wxColour(242,252,131));
	wxFont m_plblModeFont(22,wxFONTFAMILY_SWISS,wxFONTSTYLE_NORMAL,wxFONTWEIGHT_NORMAL,false,_T("Courier New"),wxFONTENCODING_DEFAULT);
	m_plblMode->SetFont(m_plblModeFont);
	m_pLblVolumeTitle = new wmLabel(this, ID_M_PLBL8, _("Amplitude:"), wxPoint(0,180), wxSize(200,30), 0, _T("ID_M_PLBL8"));
	m_pLblVolumeTitle->SetBorderState(uiRect::BORDER_NONE);
	m_pLblVolumeTitle->GetUiRect().SetGradient(0);
	m_pLblVolumeTitle->SetForegroundColour(wxColour(255,255,255));
	m_pLblVolumeTitle->SetBackgroundColour(wxColour(0,0,0));
	wxFont m_pLblVolumeTitleFont(22,wxFONTFAMILY_SWISS,wxFONTSTYLE_NORMAL,wxFONTWEIGHT_NORMAL,false,_T("Courier New"),wxFONTENCODING_DEFAULT);
	m_pLblVolumeTitle->SetFont(m_pLblVolumeTitleFont);
	m_pLblFPSTitle = new wmLabel(this, ID_M_PLBL3, _("FPS:"), wxPoint(0,210), wxSize(200,30), 0, _T("ID_M_PLBL3"));
	m_pLblFPSTitle->SetBorderState(uiRect::BORDER_NONE);
	m_pLblFPSTitle->GetUiRect().SetGradient(0);
	m_pLblFPSTitle->SetForegroundColour(wxColour(255,255,255));
	m_pLblFPSTitle->SetBackgroundColour(wxColour(0,0,0));
	wxFont m_pLblFPSTitleFont(22,wxFONTFAMILY_SWISS,wxFONTSTYLE_NORMAL,wxFONTWEIGHT_NORMAL,false,_T("Courier New"),wxFONTENCODING_DEFAULT);
	m_pLblFPSTitle->SetFont(m_pLblFPSTitleFont);
	m_pLblColourTitle = new wmLabel(this, ID_M_PLBL12, _("Colour:"), wxPoint(0,240), wxSize(200,30), 0, _T("ID_M_PLBL12"));
	m_pLblColourTitle->SetBorderState(uiRect::BORDER_NONE);
	m_pLblColourTitle->GetUiRect().SetGradient(0);
	m_pLblColourTitle->SetForegroundColour(wxColour(255,255,255));
	m_pLblColourTitle->SetBackgroundColour(wxColour(0,0,0));
	wxFont m_pLblColourTitleFont(22,wxFONTFAMILY_SWISS,wxFONTSTYLE_NORMAL,wxFONTWEIGHT_NORMAL,false,_T("Courier New"),wxFONTENCODING_DEFAULT);
	m_pLblColourTitle->SetFont(m_pLblColourTitleFont);
	m_plblColour = new wmLabel(this, ID_M_PLBL14, _("-1.4 dbFS"), wxPoint(200,240), wxSize(250,30), wxALIGN_LEFT, _T("ID_M_PLBL14"));
	m_plblColour->SetBorderState(uiRect::BORDER_NONE);
	m_plblColour->GetUiRect().SetGradient(0);
	m_plblColour->SetForegroundColour(wxColour(242,252,131));
	wxFont m_plblColourFont(22,wxFONTFAMILY_SWISS,wxFONTSTYLE_NORMAL,wxFONTWEIGHT_NORMAL,false,_T("Courier New"),wxFONTENCODING_DEFAULT);
	m_plblColour->SetFont(m_plblColourFont);
	m_plblListTitle = new wmLabel(this, ID_M_PLBL13, _("Set Date:"), wxPoint(0,340), wxSize(200,44), 0, _T("ID_M_PLBL13"));
	m_plblListTitle->SetBorderState(uiRect::BORDER_NONE);
	m_plblListTitle->GetUiRect().SetGradient(0);
	m_plblListTitle->SetForegroundColour(wxColour(255,255,255));
	m_plblListTitle->SetBackgroundColour(wxColour(0,0,0));
	wxFont m_plblListTitleFont(22,wxFONTFAMILY_SWISS,wxFONTSTYLE_NORMAL,wxFONTWEIGHT_NORMAL,false,_T("Courier New"),wxFONTENCODING_DEFAULT);
	m_plblListTitle->SetFont(m_plblListTitleFont);
	m_plblClockTitle = new wmLabel(this, ID_M_PLBL18, _("Clock:"), wxPoint(0,270), wxSize(200,30), 0, _T("ID_M_PLBL18"));
	m_plblClockTitle->SetBorderState(uiRect::BORDER_NONE);
	m_plblClockTitle->GetUiRect().SetGradient(0);
	m_plblClockTitle->SetForegroundColour(wxColour(255,255,255));
	m_plblClockTitle->SetBackgroundColour(wxColour(0,0,0));
	wxFont m_plblClockTitleFont(22,wxFONTFAMILY_SWISS,wxFONTSTYLE_NORMAL,wxFONTWEIGHT_NORMAL,false,_T("Courier New"),wxFONTENCODING_DEFAULT);
	m_plblClockTitle->SetFont(m_plblClockTitleFont);
	m_plstDate = new wmList(this, ID_M_PLST1, wxPoint(200,340), wxSize(400,44), wmList::STYLE_SELECT, 0, wxSize(-1,-1), 5, wxSize(3,1));
	m_plstDate->SetBackgroundColour(wxColour(0,0,0));
	m_plstDate->SetButtonColour(wxColour(wxT("#004000")));
	m_plstDate->SetSelectedButtonColour(wxColour(wxT("#FF8000")));
	m_plblClock = new wmLabel(this, ID_M_PLBL15, _("-1.4 dbFS"), wxPoint(200,270), wxSize(250,30), wxALIGN_LEFT, _T("ID_M_PLBL15"));
	m_plblClock->SetBorderState(uiRect::BORDER_NONE);
	m_plblClock->GetUiRect().SetGradient(0);
	m_plblClock->SetForegroundColour(wxColour(242,252,131));
	wxFont m_plblClockFont(22,wxFONTFAMILY_SWISS,wxFONTSTYLE_NORMAL,wxFONTWEIGHT_NORMAL,false,_T("Courier New"),wxFONTENCODING_DEFAULT);
	m_plblClock->SetFont(m_plblClockFont);
	m_pLbl2 = new wmLabel(this, ID_M_PLBL19, _("Channels"), wxPoint(460,90), wxSize(130,30), 0, _T("ID_M_PLBL19"));
	m_pLbl2->SetBorderState(uiRect::BORDER_NONE);
	m_pLbl2->GetUiRect().SetGradient(0);
	m_pLbl2->SetForegroundColour(wxColour(255,255,255));
	m_pLbl2->SetBackgroundColour(wxColour(0,0,0));
	wxFont m_pLbl2Font(16,wxFONTFAMILY_SWISS,wxFONTSTYLE_NORMAL,wxFONTWEIGHT_NORMAL,false,_T("Courier New"),wxFONTENCODING_DEFAULT);
	m_pLbl2->SetFont(m_pLbl2Font);
	m_plstChannels = new wmList(this, ID_M_PLST2, wxPoint(460,118), wxSize(130,162), wmList::STYLE_SELECT, 0, wxSize(-1,40), 2, wxSize(3,1));
	m_plstChannels->SetBackgroundColour(wxColour(0,0,0));
	m_plstChannels->SetButtonColour(wxColour(wxT("#004000")));
	m_plstChannels->SetSelectedButtonColour(wxColour(wxT("#FF8000")));
	m_plblFPS = new wmLabel(this, ID_M_PLBL4, _("-1.4 dbFS"), wxPoint(200,210), wxSize(250,30), wxALIGN_LEFT, _T("ID_M_PLBL4"));
	m_plblFPS->SetBorderState(uiRect::BORDER_NONE);
	m_plblFPS->GetUiRect().SetGradient(0);
	m_plblFPS->SetForegroundColour(wxColour(242,252,131));
	wxFont m_plblFPSFont(22,wxFONTFAMILY_SWISS,wxFONTSTYLE_NORMAL,wxFONTWEIGHT_NORMAL,false,_T("Courier New"),wxFONTENCODING_DEFAULT);
	m_plblFPS->SetFont(m_plblFPSFont);
	m_plblLTCVolume = new wmLabel(this, ID_M_PLBL5, _("-1.4 dbFS"), wxPoint(200,180), wxSize(250,30), wxALIGN_LEFT, _T("ID_M_PLBL5"));
	m_plblLTCVolume->SetBorderState(uiRect::BORDER_NONE);
	m_plblLTCVolume->GetUiRect().SetGradient(0);
	m_plblLTCVolume->SetForegroundColour(wxColour(242,252,131));
	wxFont m_plblLTCVolumeFont(22,wxFONTFAMILY_SWISS,wxFONTSTYLE_NORMAL,wxFONTWEIGHT_NORMAL,false,_T("Courier New"),wxFONTENCODING_DEFAULT);
	m_plblLTCVolume->SetFont(m_plblLTCVolumeFont);

	Connect(ID_M_PLST1,wxEVT_LIST_SELECTED,(wxObjectEventFunction)&LTCPanel::OnlstDateSelected);
	Connect(ID_M_PLST2,wxEVT_LIST_SELECTED,(wxObjectEventFunction)&LTCPanel::OnlstDateSelected);
	//*)

	size_t nIndex;
	nIndex = m_plstDate->AddButton(wxT("Auto"));
	nIndex = m_plstDate->AddButton(wxT("SMPTE"));
	nIndex = m_plstDate->AddButton(wxT("BBC"));
	nIndex = m_plstDate->AddButton(wxT("TVE"));
	nIndex = m_plstDate->AddButton(wxT("MTD"));

	m_plstDate->ConnectToSetting(m_pBuilder->GetSection(), "DateFormat", "Auto");


	m_plblLTCVolume->SetTextAlign(wxALIGN_LEFT | wxALIGN_CENTER_VERTICAL);
	m_plblMode->SetTextAlign(wxALIGN_LEFT | wxALIGN_CENTER_VERTICAL);
	m_plblFPS->SetTextAlign(wxALIGN_LEFT | wxALIGN_CENTER_VERTICAL);
	m_plblColour->SetTextAlign(wxALIGN_LEFT | wxALIGN_CENTER_VERTICAL);
	m_plblClock->SetTextAlign(wxALIGN_LEFT | wxALIGN_CENTER_VERTICAL);
	m_plblDateFormat->SetTextAlign(wxALIGN_LEFT | wxALIGN_CENTER_VERTICAL);

	m_pLblVolumeTitle->SetTextAlign(wxALIGN_RIGHT | wxALIGN_CENTER_VERTICAL);
	m_pLblModeTitle->SetTextAlign(wxALIGN_RIGHT | wxALIGN_CENTER_VERTICAL);
	m_pLblFPSTitle->SetTextAlign(wxALIGN_RIGHT | wxALIGN_CENTER_VERTICAL);
	m_pLblColourTitle->SetTextAlign(wxALIGN_RIGHT | wxALIGN_CENTER_VERTICAL);
	m_plblClockTitle->SetTextAlign(wxALIGN_RIGHT | wxALIGN_CENTER_VERTICAL);
	m_plblListTitle->SetTextAlign(wxALIGN_RIGHT | wxALIGN_CENTER_VERTICAL);

	Connect(wxEVT_LEFT_UP,(wxObjectEventFunction)&LTCPanel::OnLeftUp);
	m_pBuilder->RegisterForSettingsUpdates(this, "DateFormat");

	Connect(wxID_ANY, wxEVT_SETTING_CHANGED, (wxObjectEventFunction)&LTCPanel::OnSettingEvent);

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
        m_plblMode->SetLabel(m_pDecoder->GetMode());
        m_plblDateFormat->SetLabel(m_pDecoder->GetFormat());

        if(m_pDecoder->IsClockFlagSet())
        {
            m_plblClock->SetLabel(wxT("External"));
        }
        else
        {
            m_plblClock->SetLabel(wxT("Internal"));
        }
        if(m_pDecoder->IsColourFlagSet())
        {
            m_plblColour->SetLabel(wxT("Set"));
        }
        else
        {
            m_plblColour->SetLabel(wxT("Not Set"));
        }
    }

    if(m_pBuilder->WebsocketsActive())
    {
        Json::Value jsValue;
        jsValue["date"] = m_pDecoder->GetDate().ToStdString();
        jsValue["time"] = m_pDecoder->GetTime().ToStdString();
        jsValue["volume"] = m_pDecoder->GetAmplitude().ToStdString();
        jsValue["fps"] = m_pDecoder->GetFPS().ToStdString();
        jsValue["mode"] = m_pDecoder->GetMode().ToStdString();
        jsValue["date_format"] = m_pDecoder->GetFormat().ToStdString();
        jsValue["clock"] = (m_pDecoder->IsClockFlagSet() ? "External" : "Internal");
        jsValue["colour"] = m_pDecoder->IsColourFlagSet();
        m_pBuilder->SendWebsocketMessage(jsValue);
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



void LTCPanel::OnlstDateSelected(wxCommandEvent& event)
{
}

void LTCPanel::OnSettingEvent(SettingEvent& event)
{
    if(event.GetKey() == wxT("DateFormat"))
    {
        m_pDecoder->SetDateMode(event.GetValue(long(0)));
    }
}
