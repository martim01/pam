#include "LTCGeneratorpanel.h"
#include "LTCGeneratorbuilder.h"
#include "timedbuffer.h"
#include "session.h"
#include "settingevent.h"
#include <wx/log.h>


//(*InternalHeaders(LTCGeneratorPanel)
#include <wx/intl.h>
#include <wx/string.h>
//*)

//(*IdInit(LTCGeneratorPanel)
const long LTCGeneratorPanel::ID_M_PLBL1 = wxNewId();
const long LTCGeneratorPanel::ID_M_PLST1 = wxNewId();
const long LTCGeneratorPanel::ID_M_PLBL2 = wxNewId();
const long LTCGeneratorPanel::ID_M_PLST2 = wxNewId();
const long LTCGeneratorPanel::ID_M_PLBL3 = wxNewId();
const long LTCGeneratorPanel::ID_M_PLST3 = wxNewId();
const long LTCGeneratorPanel::ID_M_PLBL4 = wxNewId();
const long LTCGeneratorPanel::ID_M_PLST4 = wxNewId();
const long LTCGeneratorPanel::ID_M_PLBL5 = wxNewId();
const long LTCGeneratorPanel::ID_M_PBTN2 = wxNewId();
const long LTCGeneratorPanel::ID_M_PLBL11 = wxNewId();
const long LTCGeneratorPanel::ID_M_PBTN1 = wxNewId();
const long LTCGeneratorPanel::ID_M_PLBL8 = wxNewId();
const long LTCGeneratorPanel::ID_M_PBTN7 = wxNewId();
const long LTCGeneratorPanel::ID_M_PLBL14 = wxNewId();
const long LTCGeneratorPanel::ID_M_PBTN8 = wxNewId();
const long LTCGeneratorPanel::ID_M_PLBL6 = wxNewId();
const long LTCGeneratorPanel::ID_M_PBTN3 = wxNewId();
const long LTCGeneratorPanel::ID_M_PLBL12 = wxNewId();
const long LTCGeneratorPanel::ID_M_PBTN4 = wxNewId();
const long LTCGeneratorPanel::ID_M_PLBL9 = wxNewId();
const long LTCGeneratorPanel::ID_M_PBTN9 = wxNewId();
const long LTCGeneratorPanel::ID_M_PLBL15 = wxNewId();
const long LTCGeneratorPanel::ID_M_PBTN10 = wxNewId();
const long LTCGeneratorPanel::ID_M_PLBL7 = wxNewId();
const long LTCGeneratorPanel::ID_M_PBTN5 = wxNewId();
const long LTCGeneratorPanel::ID_M_PLBL13 = wxNewId();
const long LTCGeneratorPanel::ID_M_PBTN6 = wxNewId();
const long LTCGeneratorPanel::ID_M_PLBL10 = wxNewId();
const long LTCGeneratorPanel::ID_M_PBTN11 = wxNewId();
const long LTCGeneratorPanel::ID_M_PLBL16 = wxNewId();
const long LTCGeneratorPanel::ID_M_PBTN12 = wxNewId();
const long LTCGeneratorPanel::ID_PANEL1 = wxNewId();
//*)

wxIMPLEMENT_DYNAMIC_CLASS(LTCGeneratorPanel,pmPanel);

BEGIN_EVENT_TABLE(LTCGeneratorPanel,wxPanel)
	//(*EventTable(LTCGeneratorPanel)
	//*)
END_EVENT_TABLE()

LTCGeneratorPanel::LTCGeneratorPanel(wxWindow* parent,LTCGeneratorBuilder* pBuilder, wxWindowID id,const wxPoint& pos,const wxSize& size) : pmPanel(),
    m_pBuilder(pBuilder)
{
	//(*Initialize(LTCGeneratorPanel)
	Create(parent, id, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL, _T("id"));
	SetBackgroundColour(wxColour(0,0,0));
	m_pLbl1 = new wmLabel(this, ID_M_PLBL1, _("Frames Per Second"), wxPoint(0,0), wxSize(300,30), 0, _T("ID_M_PLBL1"));
	m_pLbl1->SetBorderState(uiRect::BORDER_NONE);
	m_pLbl1->GetUiRect().SetGradient(wxNORTH);
	m_pLbl1->SetForegroundColour(wxColour(255,255,255));
	m_pLbl1->SetBackgroundColour(wxColour(0,0,90));
	m_plstFPS = new wmList(this, ID_M_PLST1, wxPoint(0,30), wxSize(300,34), wmList::STYLE_SELECT, 0, wxSize(-1,30), 5, wxSize(5,1));
	m_plstFPS->SetBackgroundColour(wxColour(0,0,0));
	m_plstFPS->SetSelectedButtonColour(wxColour(wxT("#FF8000")));
	m_pLbl2 = new wmLabel(this, ID_M_PLBL2, _("User Data Mode"), wxPoint(0,70), wxSize(295,30), 0, _T("ID_M_PLBL2"));
	m_pLbl2->SetBorderState(uiRect::BORDER_NONE);
	m_pLbl2->GetUiRect().SetGradient(wxNORTH);
	m_pLbl2->SetForegroundColour(wxColour(255,255,255));
	m_pLbl2->SetBackgroundColour(wxColour(0,0,90));
	m_plstUserData = new wmList(this, ID_M_PLST2, wxPoint(0,100), wxSize(295,34), wmList::STYLE_SELECT, 0, wxSize(-1,30), 5, wxSize(5,1));
	m_plstUserData->SetBackgroundColour(wxColour(0,0,0));
	m_plstUserData->SetSelectedButtonColour(wxColour(wxT("#FF8000")));
	m_pLbl3 = new wmLabel(this, ID_M_PLBL3, _("Date Format"), wxPoint(305,70), wxSize(295,30), 0, _T("ID_M_PLBL3"));
	m_pLbl3->SetBorderState(uiRect::BORDER_NONE);
	m_pLbl3->GetUiRect().SetGradient(wxNORTH);
	m_pLbl3->SetForegroundColour(wxColour(255,255,255));
	m_pLbl3->SetBackgroundColour(wxColour(0,0,90));
	m_plstData = new wmList(this, ID_M_PLST3, wxPoint(305,100), wxSize(295,34), wmList::STYLE_SELECT, 0, wxSize(-1,30), 5, wxSize(5,1));
	m_plstData->SetBackgroundColour(wxColour(0,0,0));
	m_plstData->SetSelectedButtonColour(wxColour(wxT("#FF8000")));
	m_pLbl4 = new wmLabel(this, ID_M_PLBL4, _("Time Settings"), wxPoint(0,210), wxSize(300,30), 0, _T("ID_M_PLBL4"));
	m_pLbl4->SetBorderState(uiRect::BORDER_NONE);
	m_pLbl4->GetUiRect().SetGradient(wxNORTH);
	m_pLbl4->SetForegroundColour(wxColour(255,255,255));
	m_pLbl4->SetBackgroundColour(wxColour(0,0,90));
	m_plstTime = new wmList(this, ID_M_PLST4, wxPoint(300,208), wxSize(295,34), wmList::STYLE_SELECT, 0, wxSize(-1,30), 3, wxSize(5,1));
	m_plstTime->SetBackgroundColour(wxColour(0,0,0));
	m_plstTime->SetSelectedButtonColour(wxColour(wxT("#FF8000")));
	pnlTime = new wxPanel(this, ID_PANEL1, wxPoint(0,245), wxSize(600,100), wxTAB_TRAVERSAL, _T("ID_PANEL1"));
	pnlTime->SetBackgroundColour(wxColour(0,0,0));
	m_pLbl5 = new wmLabel(pnlTime, ID_M_PLBL5, _("Years"), wxPoint(0,0), wxSize(100,30), 0, _T("ID_M_PLBL5"));
	m_pLbl5->SetBorderState(uiRect::BORDER_NONE);
	m_pLbl5->GetUiRect().SetGradient(wxNORTH);
	m_pLbl5->SetForegroundColour(wxColour(255,255,255));
	m_pLbl5->SetBackgroundColour(wxColour(0,0,64));
	m_pbtnYearMinus = new wmButton(pnlTime, ID_M_PBTN2, _("-1"), wxPoint(110,0), wxSize(50,30), 0, wxDefaultValidator, _T("ID_M_PBTN2"));
	m_plblYears = new wmLabel(pnlTime, ID_M_PLBL11, _("0"), wxPoint(165,0), wxSize(60,30), 0, _T("ID_M_PLBL11"));
	m_plblYears->SetBorderState(uiRect::BORDER_DOWN);
	m_plblYears->GetUiRect().SetGradient(0);
	m_plblYears->SetForegroundColour(wxColour(0,0,0));
	m_plblYears->SetBackgroundColour(wxColour(255,255,255));
	m_pbtnYearPlus = new wmButton(pnlTime, ID_M_PBTN1, _("+1"), wxPoint(230,0), wxSize(50,30), 0, wxDefaultValidator, _T("ID_M_PBTN1"));
	m_pLbl8 = new wmLabel(pnlTime, ID_M_PLBL8, _("Hours"), wxPoint(300,0), wxSize(100,30), 0, _T("ID_M_PLBL8"));
	m_pLbl8->SetBorderState(uiRect::BORDER_NONE);
	m_pLbl8->GetUiRect().SetGradient(wxNORTH);
	m_pLbl8->SetForegroundColour(wxColour(255,255,255));
	m_pLbl8->SetBackgroundColour(wxColour(0,0,64));
	m_pbtnHourMinus = new wmButton(pnlTime, ID_M_PBTN7, _("-1"), wxPoint(410,0), wxSize(50,30), 0, wxDefaultValidator, _T("ID_M_PBTN7"));
	m_plblHours = new wmLabel(pnlTime, ID_M_PLBL14, _("0"), wxPoint(465,0), wxSize(60,30), 0, _T("ID_M_PLBL14"));
	m_plblHours->SetBorderState(uiRect::BORDER_DOWN);
	m_plblHours->GetUiRect().SetGradient(0);
	m_plblHours->SetForegroundColour(wxColour(0,0,0));
	m_plblHours->SetBackgroundColour(wxColour(255,255,255));
	m_pbtnHourPlus = new wmButton(pnlTime, ID_M_PBTN8, _("+1"), wxPoint(530,0), wxSize(50,30), 0, wxDefaultValidator, _T("ID_M_PBTN8"));
	m_pLbl6 = new wmLabel(pnlTime, ID_M_PLBL6, _("Months"), wxPoint(0,35), wxSize(100,30), 0, _T("ID_M_PLBL6"));
	m_pLbl6->SetBorderState(uiRect::BORDER_NONE);
	m_pLbl6->GetUiRect().SetGradient(wxNORTH);
	m_pLbl6->SetForegroundColour(wxColour(255,255,255));
	m_pLbl6->SetBackgroundColour(wxColour(0,0,64));
	m_pbtnMonthMinus = new wmButton(pnlTime, ID_M_PBTN3, _("-1"), wxPoint(110,35), wxSize(50,30), 0, wxDefaultValidator, _T("ID_M_PBTN3"));
	m_plblMonths = new wmLabel(pnlTime, ID_M_PLBL12, _("0"), wxPoint(165,35), wxSize(60,30), 0, _T("ID_M_PLBL12"));
	m_plblMonths->SetBorderState(uiRect::BORDER_DOWN);
	m_plblMonths->GetUiRect().SetGradient(0);
	m_plblMonths->SetForegroundColour(wxColour(0,0,0));
	m_plblMonths->SetBackgroundColour(wxColour(255,255,255));
	m_pbtnMonthPlus = new wmButton(pnlTime, ID_M_PBTN4, _("+1"), wxPoint(230,35), wxSize(50,30), 0, wxDefaultValidator, _T("ID_M_PBTN4"));
	m_pLbl9 = new wmLabel(pnlTime, ID_M_PLBL9, _("Minutes"), wxPoint(300,35), wxSize(100,30), 0, _T("ID_M_PLBL9"));
	m_pLbl9->SetBorderState(uiRect::BORDER_NONE);
	m_pLbl9->GetUiRect().SetGradient(wxNORTH);
	m_pLbl9->SetForegroundColour(wxColour(255,255,255));
	m_pLbl9->SetBackgroundColour(wxColour(0,0,64));
	m_pbtnMinuteMinus = new wmButton(pnlTime, ID_M_PBTN9, _("-1"), wxPoint(410,35), wxSize(50,30), 0, wxDefaultValidator, _T("ID_M_PBTN9"));
	m_plblMinutes = new wmLabel(pnlTime, ID_M_PLBL15, _("0"), wxPoint(465,35), wxSize(60,30), 0, _T("ID_M_PLBL15"));
	m_plblMinutes->SetBorderState(uiRect::BORDER_DOWN);
	m_plblMinutes->GetUiRect().SetGradient(0);
	m_plblMinutes->SetForegroundColour(wxColour(0,0,0));
	m_plblMinutes->SetBackgroundColour(wxColour(255,255,255));
	m_ptbnMinutePlus = new wmButton(pnlTime, ID_M_PBTN10, _("+1"), wxPoint(530,35), wxSize(50,30), 0, wxDefaultValidator, _T("ID_M_PBTN10"));
	m_pLbl7 = new wmLabel(pnlTime, ID_M_PLBL7, _("Days"), wxPoint(0,70), wxSize(100,30), 0, _T("ID_M_PLBL7"));
	m_pLbl7->SetBorderState(uiRect::BORDER_NONE);
	m_pLbl7->GetUiRect().SetGradient(wxNORTH);
	m_pLbl7->SetForegroundColour(wxColour(255,255,255));
	m_pLbl7->SetBackgroundColour(wxColour(0,0,64));
	m_pbtnDayMinus = new wmButton(pnlTime, ID_M_PBTN5, _("-1"), wxPoint(110,70), wxSize(50,30), 0, wxDefaultValidator, _T("ID_M_PBTN5"));
	m_plblDays = new wmLabel(pnlTime, ID_M_PLBL13, _("0"), wxPoint(165,70), wxSize(60,30), 0, _T("ID_M_PLBL13"));
	m_plblDays->SetBorderState(uiRect::BORDER_DOWN);
	m_plblDays->GetUiRect().SetGradient(0);
	m_plblDays->SetForegroundColour(wxColour(0,0,0));
	m_plblDays->SetBackgroundColour(wxColour(255,255,255));
	m_pbtnDayPlus = new wmButton(pnlTime, ID_M_PBTN6, _("+1"), wxPoint(230,70), wxSize(50,30), 0, wxDefaultValidator, _T("ID_M_PBTN6"));
	m_pLbl10 = new wmLabel(pnlTime, ID_M_PLBL10, _("Seconds"), wxPoint(300,70), wxSize(100,30), 0, _T("ID_M_PLBL10"));
	m_pLbl10->SetBorderState(uiRect::BORDER_NONE);
	m_pLbl10->GetUiRect().SetGradient(wxNORTH);
	m_pLbl10->SetForegroundColour(wxColour(255,255,255));
	m_pLbl10->SetBackgroundColour(wxColour(0,0,64));
	m_ptbnSecondMinus = new wmButton(pnlTime, ID_M_PBTN11, _("-1"), wxPoint(410,70), wxSize(50,30), 0, wxDefaultValidator, _T("ID_M_PBTN11"));
	m_plblSeconds = new wmLabel(pnlTime, ID_M_PLBL16, _("0"), wxPoint(465,70), wxSize(60,30), 0, _T("ID_M_PLBL16"));
	m_plblSeconds->SetBorderState(uiRect::BORDER_DOWN);
	m_plblSeconds->GetUiRect().SetGradient(0);
	m_plblSeconds->SetForegroundColour(wxColour(0,0,0));
	m_plblSeconds->SetBackgroundColour(wxColour(255,255,255));
	m_pbtnSecondPlus = new wmButton(pnlTime, ID_M_PBTN12, _("+1"), wxPoint(530,70), wxSize(50,30), 0, wxDefaultValidator, _T("ID_M_PBTN12"));

	Connect(ID_M_PLST1,wxEVT_LIST_SELECTED,(wxObjectEventFunction)&LTCGeneratorPanel::OnlstFPSSelected);
	Connect(ID_M_PLST2,wxEVT_LIST_SELECTED,(wxObjectEventFunction)&LTCGeneratorPanel::OnlstUserDataSelected);
	Connect(ID_M_PLST3,wxEVT_LIST_SELECTED,(wxObjectEventFunction)&LTCGeneratorPanel::OnlstDataSelected);
	Connect(ID_M_PLST4,wxEVT_LIST_SELECTED,(wxObjectEventFunction)&LTCGeneratorPanel::OnlstTimeSelected);
	Connect(ID_M_PBTN2,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&LTCGeneratorPanel::OnbtnYearMinusClick);
	Connect(ID_M_PBTN1,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&LTCGeneratorPanel::OnbtnYearPlusClick);
	Connect(ID_M_PBTN7,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&LTCGeneratorPanel::OnbtnHourMinusClick);
	Connect(ID_M_PBTN8,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&LTCGeneratorPanel::OnbtnHourPlusClick);
	Connect(ID_M_PBTN3,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&LTCGeneratorPanel::OnbtnMonthMinusClick);
	Connect(ID_M_PBTN4,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&LTCGeneratorPanel::OnbtnMonthPlusClick);
	Connect(ID_M_PBTN9,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&LTCGeneratorPanel::OnbtnMinuteMinusClick);
	Connect(ID_M_PBTN10,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&LTCGeneratorPanel::OnbtnMinutePlusClick);
	Connect(ID_M_PBTN5,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&LTCGeneratorPanel::OnbtnDayMinusClick);
	Connect(ID_M_PBTN6,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&LTCGeneratorPanel::OnbtnDayPlusClick);
	Connect(ID_M_PBTN11,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&LTCGeneratorPanel::OnbtnSecondMinusClick);
	Connect(ID_M_PBTN12,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&LTCGeneratorPanel::OnbtnSecondPlusClick);
	//*)

	Connect(wxEVT_LEFT_UP,(wxObjectEventFunction)&LTCGeneratorPanel::OnLeftUp);




	m_plstData->AddButton(wxT("SMPTE"));
	m_plstData->AddButton(wxT("BBC"));
	m_plstData->AddButton(wxT("TVE"));
	m_plstData->AddButton(wxT("MTD"));

	m_plstUserData->AddButton(wxT("None"));
	m_plstUserData->AddButton(wxT("8-Bit"));
	m_plstUserData->AddButton(wxT("Date"));
	m_plstUserData->AddButton(wxT("Line"));


	m_plstFPS->AddButton(wxT("24"));
	m_plstFPS->AddButton(wxT("25"));
	m_plstFPS->AddButton(wxT("29.97"));
	m_plstFPS->AddButton(wxT("30"));

    m_plstTime->AddButton(wxT("Live"));
    m_plstTime->AddButton(wxT("Offset"));
    m_plstTime->AddButton(wxT("Absolute"));

	SetSize(size);
	SetPosition(pos);

    LoadSettings();

	m_pBuilder->RegisterForSettingsUpdates(wxT("FPS"), this);
	m_pBuilder->RegisterForSettingsUpdates(wxT("UserData"), this);
	m_pBuilder->RegisterForSettingsUpdates(wxT("DateFormat"), this);
	m_pBuilder->RegisterForSettingsUpdates(wxT("Amplitude"), this);
	m_pBuilder->RegisterForSettingsUpdates(wxT("OffsetYear"), this);
	m_pBuilder->RegisterForSettingsUpdates(wxT("OffsetMonth"), this);
	m_pBuilder->RegisterForSettingsUpdates(wxT("OffsetDay"), this);
	m_pBuilder->RegisterForSettingsUpdates(wxT("OffsetHour"), this);
	m_pBuilder->RegisterForSettingsUpdates(wxT("OffsetMinute"), this);
	m_pBuilder->RegisterForSettingsUpdates(wxT("OffsetSecond"), this);
	m_pBuilder->RegisterForSettingsUpdates(wxT("AbsYear"), this);
	m_pBuilder->RegisterForSettingsUpdates(wxT("AbsMonth"), this);
	m_pBuilder->RegisterForSettingsUpdates(wxT("AbsDay"), this);
	m_pBuilder->RegisterForSettingsUpdates(wxT("AbsHour"), this);
	m_pBuilder->RegisterForSettingsUpdates(wxT("AbsMinute"), this);
	m_pBuilder->RegisterForSettingsUpdates(wxT("AbsSecond"), this);
	m_pBuilder->RegisterForSettingsUpdates(wxT("Time"), this);

	Connect(wxID_ANY, wxEVT_SETTING_CHANGED, (wxObjectEventFunction)&::LTCGeneratorPanel::OnSettingEvent);
}

LTCGeneratorPanel::~LTCGeneratorPanel()
{
	//(*Destroy(LTCGeneratorPanel)
	//*)
}



void LTCGeneratorPanel::OnLeftUp(wxMouseEvent& event)
{

}

void LTCGeneratorPanel::OnlstFPSSelected(wxCommandEvent& event)
{
    m_pBuilder->WriteSetting(wxT("FPS"), event.GetString());
}

void LTCGeneratorPanel::OnlstUserDataSelected(wxCommandEvent& event)
{
    m_pBuilder->WriteSetting(wxT("UserData"), event.GetInt());
}

void LTCGeneratorPanel::OnlstDataSelected(wxCommandEvent& event)
{
    m_pBuilder->WriteSetting(wxT("DateFormat"), event.GetInt());
}

void LTCGeneratorPanel::OnbtnYearMinusClick(wxCommandEvent& event)
{
    m_pBuilder->WriteSetting(wxString::Format(wxT("%sYear"),m_sOffset.c_str()), m_nYear-1);
}

void LTCGeneratorPanel::OnbtnYearPlusClick(wxCommandEvent& event)
{
    m_pBuilder->WriteSetting(wxString::Format(wxT("%sYear"),m_sOffset.c_str()), m_nYear+1);
}

void LTCGeneratorPanel::OnbtnMonthMinusClick(wxCommandEvent& event)
{
    m_pBuilder->WriteSetting(wxString::Format(wxT("%sMonth"),m_sOffset.c_str()), m_nMonth-1);
}

void LTCGeneratorPanel::OnbtnMonthPlusClick(wxCommandEvent& event)
{
    m_pBuilder->WriteSetting(wxString::Format(wxT("%sMonth"),m_sOffset.c_str()), m_nMonth+1);
}

void LTCGeneratorPanel::OnbtnDayMinusClick(wxCommandEvent& event)
{
    m_pBuilder->WriteSetting(wxString::Format(wxT("%sDay"),m_sOffset.c_str()), m_nDay-1);
}

void LTCGeneratorPanel::OnbtnDayPlusClick(wxCommandEvent& event)
{
    m_pBuilder->WriteSetting(wxString::Format(wxT("%sDay"),m_sOffset.c_str()), m_nDay+1);
}

void LTCGeneratorPanel::OnbtnHourMinusClick(wxCommandEvent& event)
{
    m_pBuilder->WriteSetting(wxString::Format(wxT("%sHour"),m_sOffset.c_str()), m_nHour-1);
}

void LTCGeneratorPanel::OnbtnHourPlusClick(wxCommandEvent& event)
{
    m_pBuilder->WriteSetting(wxString::Format(wxT("%sHour"),m_sOffset.c_str()), m_nHour+1);
}

void LTCGeneratorPanel::OnbtnMinuteMinusClick(wxCommandEvent& event)
{
    m_pBuilder->WriteSetting(wxString::Format(wxT("%sMinute"),m_sOffset.c_str()), m_nMinute-1);
}

void LTCGeneratorPanel::OnbtnMinutePlusClick(wxCommandEvent& event)
{
    m_pBuilder->WriteSetting(wxString::Format(wxT("%sMinute"),m_sOffset.c_str()), m_nMinute+1);
}

void LTCGeneratorPanel::OnbtnSecondMinusClick(wxCommandEvent& event)
{
    m_pBuilder->WriteSetting(wxString::Format(wxT("%sSecond"),m_sOffset.c_str()), m_nSecond-1);
}

void LTCGeneratorPanel::OnbtnSecondPlusClick(wxCommandEvent& event)
{
    m_pBuilder->WriteSetting(wxString::Format(wxT("%sSecond"),m_sOffset.c_str()), m_nSecond+1);
}

void LTCGeneratorPanel::LoadSettings()
{
    m_plstFPS->SelectButton(m_pBuilder->ReadSetting(wxT("FPS"), wxT("25")));
    m_plstData->SelectButton(m_pBuilder->ReadSetting(wxT("DateFormat"), 0));
    m_plstUserData->SelectButton(m_pBuilder->ReadSetting(wxT("UserData"), 0));
    m_plstTime->SelectButton(m_pBuilder->ReadSetting(wxT("Time"), 0));

}

void LTCGeneratorPanel::OnSettingEvent(SettingEvent& event)
{

    if(event.GetKey() == wxT("FPS"))
    {
        m_plstFPS->SelectButton(event.GetValue(),false);
    }
	else if(event.GetKey() == wxT("UserData"))
    {
        m_plstUserData->SelectButton(event.GetValue(long(0)), false);
    }
	else if(event.GetKey() == wxT("DateFormat"))
    {
        m_plstData->SelectButton(event.GetValue(long(0)),false);
    }
	else if(event.GetKey() == wxT("Amplitude"))
    {

    }
	else if(event.GetKey() == wxT("OffsetYear"))
    {
        m_nYear = event.GetValue(long(0));
        m_plblYears->SetLabel(event.GetValue());
    }
	else if(event.GetKey() == wxT("OffsetMonth"))
    {
        m_nMonth = event.GetValue(long(0));
        m_plblMonths->SetLabel(event.GetValue());
    }
	else if(event.GetKey() == wxT("OffsetDay"))
    {
        m_nDay = event.GetValue(long(0));
        m_plblDays->SetLabel(event.GetValue());
    }
	else if(event.GetKey() == wxT("OffsetHour"))
    {
        m_nHour = event.GetValue(long(0));
        m_plblHours->SetLabel(event.GetValue());
    }
	else if(event.GetKey() == wxT("OffsetMinute"))
    {
        m_nMinute = event.GetValue(long(0));
        m_plblMinutes->SetLabel(event.GetValue());
    }
	else if(event.GetKey() == wxT("OffsetSecond"))
    {
        m_nSecond = event.GetValue(long(0));
        m_plblSeconds->SetLabel(event.GetValue());
    }
    else if(event.GetKey() == wxT("Time"))
    {
        m_plstTime->SelectButton(event.GetValue(long(0)),false);
    }
}

void LTCGeneratorPanel::OnlstTimeSelected(wxCommandEvent& event)
{
    m_pBuilder->WriteSetting(wxT("Time"), event.GetInt());
    if(event.GetInt() == 1)
    {
        m_sOffset = wxT("Offset");
        pnlTime->Show();
    }
    else if(event.GetInt() == 2)
    {
        m_sOffset = wxT("Abs");
        pnlTime->Show();
    }
    else
    {
        pnlTime->Show(false);
    }

    m_nYear = m_pBuilder->ReadSetting(wxString::Format(wxT("%sYear"),m_sOffset.c_str()), event.GetInt()==1?0:2018);
    m_nMonth = m_pBuilder->ReadSetting(wxString::Format(wxT("%sMonth"),m_sOffset.c_str()), event.GetInt()==1?0:1);
    m_nDay = m_pBuilder->ReadSetting(wxString::Format(wxT("%sDay"),m_sOffset.c_str()), event.GetInt()==1?0:1);
    m_nHour = m_pBuilder->ReadSetting(wxString::Format(wxT("%sHour"),m_sOffset.c_str()), 0);
    m_nMinute = m_pBuilder->ReadSetting(wxString::Format(wxT("%sMinute"),m_sOffset.c_str()), 0);
    m_nSecond = m_pBuilder->ReadSetting(wxString::Format(wxT("%sSecond"),m_sOffset.c_str()), 0);

    m_plblYears->SetLabel(wxString::Format(wxT("%d"), m_nYear));
    m_plblMonths->SetLabel(wxString::Format(wxT("%d"), m_nMonth));
    m_plblDays->SetLabel(wxString::Format(wxT("%d"), m_nDay));
    m_plblHours->SetLabel(wxString::Format(wxT("%d"), m_nHour));
    m_plblMinutes->SetLabel(wxString::Format(wxT("%d"), m_nMinute));
    m_plblSeconds->SetLabel(wxString::Format(wxT("%d"), m_nSecond));
}
