#include "pnlChannelMapping.h"

//(*InternalHeaders(pnlChannelMapping)
#include <wx/intl.h>
#include <wx/string.h>
//*)

//(*IdInit(pnlChannelMapping)
const long pnlChannelMapping::ID_M_PLBL14 = wxNewId();
const long pnlChannelMapping::ID_M_PLBL15 = wxNewId();
const long pnlChannelMapping::ID_M_PLBL16 = wxNewId();
const long pnlChannelMapping::ID_M_PLBL17 = wxNewId();
const long pnlChannelMapping::ID_M_PLBL18 = wxNewId();
const long pnlChannelMapping::ID_M_PLBL19 = wxNewId();
const long pnlChannelMapping::ID_M_PLBL20 = wxNewId();
const long pnlChannelMapping::ID_M_PLBL21 = wxNewId();
const long pnlChannelMapping::ID_M_PBTN14 = wxNewId();
const long pnlChannelMapping::ID_M_PBTN15 = wxNewId();
const long pnlChannelMapping::ID_M_PBTN16 = wxNewId();
const long pnlChannelMapping::ID_M_PBTN17 = wxNewId();
const long pnlChannelMapping::ID_M_PBTN18 = wxNewId();
const long pnlChannelMapping::ID_M_PBTN19 = wxNewId();
const long pnlChannelMapping::ID_M_PBTN20 = wxNewId();
const long pnlChannelMapping::ID_M_PBTN21 = wxNewId();
const long pnlChannelMapping::ID_M_PLBL22 = wxNewId();
const long pnlChannelMapping::ID_M_PLBL23 = wxNewId();
const long pnlChannelMapping::ID_M_PLBL24 = wxNewId();
const long pnlChannelMapping::ID_M_PLBL25 = wxNewId();
const long pnlChannelMapping::ID_M_PLBL26 = wxNewId();
const long pnlChannelMapping::ID_M_PLBL27 = wxNewId();
const long pnlChannelMapping::ID_M_PLBL28 = wxNewId();
const long pnlChannelMapping::ID_M_PLBL29 = wxNewId();
const long pnlChannelMapping::ID_M_PBTN22 = wxNewId();
//*)

BEGIN_EVENT_TABLE(pnlChannelMapping,wxPanel)
	//(*EventTable(pnlChannelMapping)
	//*)
END_EVENT_TABLE()

pnlChannelMapping::pnlChannelMapping(wxWindow* parent,wxWindowID id,const wxPoint& pos,const wxSize& size, long nStyle, wxString sNotused)
{
	Create(parent, id, wxDefaultPosition, wxSize(600,400), wxTAB_TRAVERSAL, _T("id"));
	SetBackgroundColour(wxColour(0,0,0));
	m_pLbl13 = new wmLabel(this, ID_M_PLBL14, _("Channel 1"), wxPoint(10,40), wxSize(65,30), 0, _T("ID_M_PLBL14"));
	m_pLbl13->SetBorderState(uiRect::BORDER_NONE);
	m_pLbl13->GetUiRect().SetGradient(0);
	m_pLbl13->SetForegroundColour(wxColour(255,255,255));
	m_pLbl13->SetBackgroundColour(wxColour(64,0,128));
	m_pLbl14 = new wmLabel(this, ID_M_PLBL15, _("Channel 2"), wxPoint(80,40), wxSize(65,30), 0, _T("ID_M_PLBL15"));
	m_pLbl14->SetBorderState(uiRect::BORDER_NONE);
	m_pLbl14->GetUiRect().SetGradient(0);
	m_pLbl14->SetForegroundColour(wxColour(255,255,255));
	m_pLbl14->SetBackgroundColour(wxColour(64,0,128));
	m_pLbl15 = new wmLabel(this, ID_M_PLBL16, _("Channel 3"), wxPoint(150,40), wxSize(65,30), 0, _T("ID_M_PLBL16"));
	m_pLbl15->SetBorderState(uiRect::BORDER_NONE);
	m_pLbl15->GetUiRect().SetGradient(0);
	m_pLbl15->SetForegroundColour(wxColour(255,255,255));
	m_pLbl15->SetBackgroundColour(wxColour(64,0,128));
	m_pLbl16 = new wmLabel(this, ID_M_PLBL17, _("Channel 4"), wxPoint(220,40), wxSize(65,30), 0, _T("ID_M_PLBL17"));
	m_pLbl16->SetBorderState(uiRect::BORDER_NONE);
	m_pLbl16->GetUiRect().SetGradient(0);
	m_pLbl16->SetForegroundColour(wxColour(255,255,255));
	m_pLbl16->SetBackgroundColour(wxColour(64,0,128));
	m_pLbl17 = new wmLabel(this, ID_M_PLBL18, _("Channel 5"), wxPoint(290,40), wxSize(65,30), 0, _T("ID_M_PLBL18"));
	m_pLbl17->SetBorderState(uiRect::BORDER_NONE);
	m_pLbl17->GetUiRect().SetGradient(0);
	m_pLbl17->SetForegroundColour(wxColour(255,255,255));
	m_pLbl17->SetBackgroundColour(wxColour(64,0,128));
	m_pLbl18 = new wmLabel(this, ID_M_PLBL19, _("Channel 6"), wxPoint(360,40), wxSize(65,30), 0, _T("ID_M_PLBL19"));
	m_pLbl18->SetBorderState(uiRect::BORDER_NONE);
	m_pLbl18->GetUiRect().SetGradient(0);
	m_pLbl18->SetForegroundColour(wxColour(255,255,255));
	m_pLbl18->SetBackgroundColour(wxColour(64,0,128));
	m_pLbl19 = new wmLabel(this, ID_M_PLBL20, _("Channel 7"), wxPoint(430,40), wxSize(65,30), 0, _T("ID_M_PLBL20"));
	m_pLbl19->SetBorderState(uiRect::BORDER_NONE);
	m_pLbl19->GetUiRect().SetGradient(0);
	m_pLbl19->SetForegroundColour(wxColour(255,255,255));
	m_pLbl19->SetBackgroundColour(wxColour(64,0,128));
	m_pLbl20 = new wmLabel(this, ID_M_PLBL21, _("Channel 8"), wxPoint(500,40), wxSize(65,30), 0, _T("ID_M_PLBL21"));
	m_pLbl20->SetBorderState(uiRect::BORDER_NONE);
	m_pLbl20->GetUiRect().SetGradient(0);
	m_pLbl20->SetForegroundColour(wxColour(255,255,255));
	m_pLbl20->SetBackgroundColour(wxColour(64,0,128));
	m_pbtnCh[0] = new wmButton(this, ID_M_PBTN14, _("Undefined"), wxPoint(10,72), wxSize(65,40), wmButton::STYLE_NORMAL, wxDefaultValidator, _T("ID_M_PBTN14"));
	m_pbtnCh[0]->SetForegroundColour(wxColour(0,0,0));
	m_pbtnCh[0]->SetBackgroundColour(wxColour(255,255,255));
	m_pbtnCh[0]->SetColourDisabled(wxColour(wxT("#B0B0B0")));
	m_pbtnCh[1] = new wmButton(this, ID_M_PBTN15, _("Undefined"), wxPoint(80,72), wxSize(65,40), wmButton::STYLE_NORMAL, wxDefaultValidator, _T("ID_M_PBTN15"));
	m_pbtnCh[1]->SetForegroundColour(wxColour(0,0,0));
	m_pbtnCh[1]->SetBackgroundColour(wxColour(255,255,255));
	m_pbtnCh[1]->SetColourDisabled(wxColour(wxT("#B0B0B0")));
	m_pbtnCh[2] = new wmButton(this, ID_M_PBTN16, _("Undefined"), wxPoint(150,72), wxSize(65,40), wmButton::STYLE_NORMAL, wxDefaultValidator, _T("ID_M_PBTN16"));
	m_pbtnCh[2]->SetForegroundColour(wxColour(0,0,0));
	m_pbtnCh[2]->SetBackgroundColour(wxColour(255,255,255));
	m_pbtnCh[2]->SetColourDisabled(wxColour(wxT("#B0B0B0")));
	m_pbtnCh[3] = new wmButton(this, ID_M_PBTN17, _("Undefined"), wxPoint(220,72), wxSize(65,40), wmButton::STYLE_NORMAL, wxDefaultValidator, _T("ID_M_PBTN17"));
	m_pbtnCh[3]->SetForegroundColour(wxColour(0,0,0));
	m_pbtnCh[3]->SetBackgroundColour(wxColour(255,255,255));
	m_pbtnCh[3]->SetColourDisabled(wxColour(wxT("#B0B0B0")));
	m_pbtnCh[4] = new wmButton(this, ID_M_PBTN18, _("Undefined"), wxPoint(290,72), wxSize(65,40), wmButton::STYLE_NORMAL, wxDefaultValidator, _T("ID_M_PBTN18"));
	m_pbtnCh[4]->SetForegroundColour(wxColour(0,0,0));
	m_pbtnCh[4]->SetBackgroundColour(wxColour(255,255,255));
	m_pbtnCh[4]->SetColourDisabled(wxColour(wxT("#B0B0B0")));
	m_pbtnCh[5] = new wmButton(this, ID_M_PBTN19, _("Undefined"), wxPoint(360,72), wxSize(65,40), wmButton::STYLE_NORMAL, wxDefaultValidator, _T("ID_M_PBTN19"));
	m_pbtnCh[5]->SetForegroundColour(wxColour(0,0,0));
	m_pbtnCh[5]->SetBackgroundColour(wxColour(255,255,255));
	m_pbtnCh[5]->SetColourDisabled(wxColour(wxT("#B0B0B0")));
	m_pbtnCh[6] = new wmButton(this, ID_M_PBTN20, _("Undefined"), wxPoint(430,72), wxSize(65,40), wmButton::STYLE_NORMAL, wxDefaultValidator, _T("ID_M_PBTN20"));
	m_pbtnCh[6]->SetForegroundColour(wxColour(0,0,0));
	m_pbtnCh[6]->SetBackgroundColour(wxColour(255,255,255));
	m_pbtnCh[6]->SetColourDisabled(wxColour(wxT("#B0B0B0")));
	m_pbtnCh[7] = new wmButton(this, ID_M_PBTN21, _("Undefined"), wxPoint(500,72), wxSize(65,40), wmButton::STYLE_NORMAL, wxDefaultValidator, _T("ID_M_PBTN21"));
	m_pbtnCh[7]->SetForegroundColour(wxColour(0,0,0));
	m_pbtnCh[7]->SetBackgroundColour(wxColour(255,255,255));
	m_pbtnCh[7]->SetColourDisabled(wxColour(wxT("#B0B0B0")));
	m_plblCh1 = new wmLabel(this, ID_M_PLBL22, _("U1"), wxPoint(10,114), wxSize(65,30), 0, _T("ID_M_PLBL22"));
	m_plblCh1->SetBorderState(uiRect::BORDER_NONE);
	m_plblCh1->GetUiRect().SetGradient(0);
	m_plblCh1->SetForegroundColour(wxColour(0,0,0));
	m_plblCh1->SetBackgroundColour(wxColour(191,207,208));
	m_plblCh2 = new wmLabel(this, ID_M_PLBL23, _("U1"), wxPoint(80,114), wxSize(65,30), 0, _T("ID_M_PLBL23"));
	m_plblCh2->SetBorderState(uiRect::BORDER_NONE);
	m_plblCh2->GetUiRect().SetGradient(0);
	m_plblCh2->SetForegroundColour(wxColour(0,0,0));
	m_plblCh2->SetBackgroundColour(wxColour(191,207,208));
	m_plblCh3 = new wmLabel(this, ID_M_PLBL24, _("U1"), wxPoint(150,114), wxSize(65,30), 0, _T("ID_M_PLBL24"));
	m_plblCh3->SetBorderState(uiRect::BORDER_NONE);
	m_plblCh3->GetUiRect().SetGradient(0);
	m_plblCh3->SetForegroundColour(wxColour(0,0,0));
	m_plblCh3->SetBackgroundColour(wxColour(191,207,208));
	m_plblCh4 = new wmLabel(this, ID_M_PLBL25, _("U1"), wxPoint(220,114), wxSize(65,30), 0, _T("ID_M_PLBL25"));
	m_plblCh4->SetBorderState(uiRect::BORDER_NONE);
	m_plblCh4->GetUiRect().SetGradient(0);
	m_plblCh4->SetForegroundColour(wxColour(0,0,0));
	m_plblCh4->SetBackgroundColour(wxColour(191,207,208));
	m_plblCh5 = new wmLabel(this, ID_M_PLBL26, _("U1"), wxPoint(290,114), wxSize(65,30), 0, _T("ID_M_PLBL26"));
	m_plblCh5->SetBorderState(uiRect::BORDER_NONE);
	m_plblCh5->GetUiRect().SetGradient(0);
	m_plblCh5->SetForegroundColour(wxColour(0,0,0));
	m_plblCh5->SetBackgroundColour(wxColour(191,207,208));
	m_plblCh6 = new wmLabel(this, ID_M_PLBL27, _("U1"), wxPoint(360,114), wxSize(65,30), 0, _T("ID_M_PLBL27"));
	m_plblCh6->SetBorderState(uiRect::BORDER_NONE);
	m_plblCh6->GetUiRect().SetGradient(0);
	m_plblCh6->SetForegroundColour(wxColour(0,0,0));
	m_plblCh6->SetBackgroundColour(wxColour(191,207,208));
	m_plblCh7 = new wmLabel(this, ID_M_PLBL28, _("U1"), wxPoint(430,114), wxSize(65,30), 0, _T("ID_M_PLBL28"));
	m_plblCh7->SetBorderState(uiRect::BORDER_NONE);
	m_plblCh7->GetUiRect().SetGradient(0);
	m_plblCh7->SetForegroundColour(wxColour(0,0,0));
	m_plblCh7->SetBackgroundColour(wxColour(191,207,208));
	m_plblCh8 = new wmLabel(this, ID_M_PLBL29, _("U1"), wxPoint(500,114), wxSize(65,30), 0, _T("ID_M_PLBL29"));
	m_plblCh8->SetBorderState(uiRect::BORDER_NONE);
	m_plblCh8->GetUiRect().SetGradient(0);
	m_plblCh8->SetForegroundColour(wxColour(0,0,0));
	m_plblCh8->SetBackgroundColour(wxColour(191,207,208));
	m_pbtnBack = new wmButton(this, ID_M_PBTN22, _("Back"), wxPoint(260,330), wxSize(80,40), wmButton::STYLE_NORMAL, wxDefaultValidator, _T("ID_M_PBTN22"));
	m_pbtnBack->SetForegroundColour(wxColour(255,255,255));
	m_pbtnBack->SetBackgroundColour(wxColour(0,64,0));
	m_pbtnBack->SetColourDisabled(wxColour(wxT("#B0B0B0")));
	SetSize(size);
	SetPosition(pos);

	m_pbtnCh[0]->ConnectToSetting("ChannelMapping", "Ch_1", "Stereo");
	m_pbtnCh[1]->ConnectToSetting("ChannelMapping", "Ch_2", "");
	m_pbtnCh[2]->ConnectToSetting("ChannelMapping", "Ch_3", "Stereo");
	m_pbtnCh[3]->ConnectToSetting("ChannelMapping", "Ch_4", "");
	m_pbtnCh[4]->ConnectToSetting("ChannelMapping", "Ch_5", "Stereo");
	m_pbtnCh[5]->ConnectToSetting("ChannelMapping", "Ch_6", "");
	m_pbtnCh[6]->ConnectToSetting("ChannelMapping", "Ch_7", "Stereo");
	m_pbtnCh[7]->ConnectToSetting("ChannelMapping", "Ch_8", "");

	m_pbtnCh[0]->SetPopup({"Mono", "Dual Mono", "Stereo", "Matrix", "5.1", "7.1", "SGRP", "U01", "U02", "U03", "U04", "U05", "U06", "U07", "U08"});
	m_pbtnCh[1]->SetPopup({"Mono", "Dual Mono", "Stereo", "Matrix", "5.1", "SGRP", "U01", "U02", "U03", "U04", "U05", "U06", "U07"});
	m_pbtnCh[2]->SetPopup({"Mono", "Dual Mono", "Stereo", "Matrix", "5.1", "SGRP", "U01", "U02", "U03", "U04", "U05", "U06"});
	m_pbtnCh[3]->SetPopup({"Mono", "Dual Mono", "Stereo", "Matrix", "SGRP", "U01", "U02", "U03", "U04", "U05"});
	m_pbtnCh[4]->SetPopup({"Mono", "Dual Mono", "Stereo", "Matrix", "SGRP", "U01", "U02", "U03", "U04"});
	m_pbtnCh[5]->SetPopup({"Mono", "Dual Mono", "Stereo", "Matrix", "U01", "U02", "U03"});
	m_pbtnCh[6]->SetPopup({"Mono", "Dual Mono", "Stereo", "Matrix", "U01", "U02"});
	m_pbtnCh[7]->SetPopup({"Mono", "U01"});

	Settings::Get().AddHandler(this, "ChannelMapping");
	Bind(wxEVT_SETTING_CHANGED, pnlChannelMapping::OnSettingChanged, this);


	ShowButtons(Settings::Get().Read("ChannelMapping", "Ch_1", "Stereo"), 1);
	ShowButtons(Settings::Get().Read("ChannelMapping", "Ch_2", ""), 2);
	ShowButtons(Settings::Get().Read("ChannelMapping", "Ch_3", "Stereo"), 3);
	ShowButtons(Settings::Get().Read("ChannelMapping", "Ch_4", ""), 4);
	ShowButtons(Settings::Get().Read("ChannelMapping", "Ch_5", "Stereo"), 5);
	ShowButtons(Settings::Get().Read("ChannelMapping", "Ch_6", ""), 6);
	ShowButtons(Settings::Get().Read("ChannelMapping", "Ch_7", "Stereo"), 7);
	ShowButtons(Settings::Get().Read("ChannelMapping", "Ch_8", ""), 8);
}

pnlChannelMapping::~pnlChannelMapping()
{
	//(*Destroy(pnlChannelMapping)
	//*)
}

void pnlChannelMapping::OnSettingChanged(SettingEvent& event)
{
    unsigned long nButton;
    if(event.GetKey().BeforeFirst('_') == "Ch" && event.GetKey().AfterFirst('_').ToULong(&nButton))
    {
        ShowButtons(event.GetValue(), nButton);
    }
}

void pnlChannelMapping::ShowButtons(const wxString& sValue, int nChannel)
{
    if(sValue == "Mono" || sValue == "U01")
    {
        ShowButton(nChannel+1);

        m_plblCh[nChannel-1] = "U1";
    }
    else if(sValue == "Dual Mono")
    {
        Settings::Get().Write("ChannelMapping", wxString::Format("Ch_%d", nChannel+1), "");
        ShowButton(nChannel+2);

        m_plblCh[nChannel-1] = "M1";
        m_plblCh[nChannel] = "M2";
    }
    else if(sValue == "Stereo")
    {
        Settings::Get().Write("ChannelMapping", wxString::Format("Ch_%d", nChannel+1), "");
        ShowButton(nChannel+2);

        m_plblCh[nChannel-1] = "L";
        m_plblCh[nChannel] = "R";
    }
    else if(sValue == "Matrix")
    {
        Settings::Get().Write("ChannelMapping", wxString::Format("Ch_%d", nChannel+1), "");
        ShowButton(nChannel+2);

        m_plblCh[nChannel-1] = "Lt";
        m_plblCh[nChannel] = "Rt";
    }
    else if(sValue == "U02")
    {
        Settings::Get().Write("ChannelMapping", wxString::Format("Ch_%d", nChannel+1), "");
        ShowButton(nChannel+2);

        m_plblCh[nChannel-1] = "U1";
        m_plblCh[nChannel] = "U2";
    }
    else if(sValue == "U03")
    {
        Settings::Get().Write("ChannelMapping", wxString::Format("Ch_%d", nChannel+1), "");
        Settings::Get().Write("ChannelMapping", wxString::Format("Ch_%d", nChannel+2), "");
        ShowButton(nChannel+3);

        m_plblCh[nChannel-1] = "U1";
        m_plblCh[nChannel] = "U2";
        m_plblCh[nChannel+1] = "U3";
    }
    else if(sValue == "U04")
    {
        Settings::Get().Write("ChannelMapping", wxString::Format("Ch_%d", nChannel+1), "");
        Settings::Get().Write("ChannelMapping", wxString::Format("Ch_%d", nChannel+2), "");
        Settings::Get().Write("ChannelMapping", wxString::Format("Ch_%d", nChannel+3), "");
        ShowButton(nChannel+4);

        m_plblCh[nChannel-1] = "U1";
        m_plblCh[nChannel] = "U2";
        m_plblCh[nChannel+1] = "U3";
        m_plblCh[nChannel+2] = "U4";
    }
    else if sValue == "SGRP")
    {
        Settings::Get().Write("ChannelMapping", wxString::Format("Ch_%d", nChannel+1), "");
        Settings::Get().Write("ChannelMapping", wxString::Format("Ch_%d", nChannel+2), "");
        Settings::Get().Write("ChannelMapping", wxString::Format("Ch_%d", nChannel+3), "");
        ShowButton(nChannel+4);

        m_plblCh[nChannel-1] = "1";
        m_plblCh[nChannel] = "2";
        m_plblCh[nChannel+1] = "3";
        m_plblCh[nChannel+2] = "4";
    }
    else if(sValue == "U05")
    {
        Settings::Get().Write("ChannelMapping", wxString::Format("Ch_%d", nChannel+1), "");
        Settings::Get().Write("ChannelMapping", wxString::Format("Ch_%d", nChannel+2), "");
        Settings::Get().Write("ChannelMapping", wxString::Format("Ch_%d", nChannel+3), "");
        Settings::Get().Write("ChannelMapping", wxString::Format("Ch_%d", nChannel+4), "");
        ShowButton(nChannel+5);

        m_plblCh[nChannel-1] = "U1";
        m_plblCh[nChannel] = "U2";
        m_plblCh[nChannel+1] = "U3";
        m_plblCh[nChannel+2] = "U4";
        m_plblCh[nChannel+3] = "U5";
    }
    else if(sValue == "U06")
    {
        Settings::Get().Write("ChannelMapping", wxString::Format("Ch_%d", nChannel+1), "");
        Settings::Get().Write("ChannelMapping", wxString::Format("Ch_%d", nChannel+2), "");
        Settings::Get().Write("ChannelMapping", wxString::Format("Ch_%d", nChannel+3), "");
        Settings::Get().Write("ChannelMapping", wxString::Format("Ch_%d", nChannel+4), "");
        Settings::Get().Write("ChannelMapping", wxString::Format("Ch_%d", nChannel+5), "");
        ShowButton(nChannel+6);

        m_plblCh[nChannel-1] = "U1";
        m_plblCh[nChannel] = "U2";
        m_plblCh[nChannel+1] = "U3";
        m_plblCh[nChannel+2] = "U4";
        m_plblCh[nChannel+3] = "U5";
        m_plblCh[nChannel+4] = "U6";

    }
    else if(sValue == "5.1")
    {
        Settings::Get().Write("ChannelMapping", wxString::Format("Ch_%d", nChannel+1), "");
        Settings::Get().Write("ChannelMapping", wxString::Format("Ch_%d", nChannel+2), "");
        Settings::Get().Write("ChannelMapping", wxString::Format("Ch_%d", nChannel+3), "");
        Settings::Get().Write("ChannelMapping", wxString::Format("Ch_%d", nChannel+4), "");
        Settings::Get().Write("ChannelMapping", wxString::Format("Ch_%d", nChannel+5), "");
        ShowButton(nChannel+6);

        m_plblCh[nChannel-1] = "L";
        m_plblCh[nChannel] = "R";
        m_plblCh[nChannel+1] = "C";
        m_plblCh[nChannel+2] = "LFE";
        m_plblCh[nChannel+3] = "Ls";
        m_plblCh[nChannel+4] = "Rs";
    }
    else if(sValue == "U07")
    {
        Settings::Get().Write("ChannelMapping", wxString::Format("Ch_%d", nChannel+1), "");
        Settings::Get().Write("ChannelMapping", wxString::Format("Ch_%d", nChannel+2), "");
        Settings::Get().Write("ChannelMapping", wxString::Format("Ch_%d", nChannel+3), "");
        Settings::Get().Write("ChannelMapping", wxString::Format("Ch_%d", nChannel+4), "");
        Settings::Get().Write("ChannelMapping", wxString::Format("Ch_%d", nChannel+5), "");
        Settings::Get().Write("ChannelMapping", wxString::Format("Ch_%d", nChannel+6), "");
        ShowButton(nChannel+7);

        m_plblCh[nChannel-1] = "U1";
        m_plblCh[nChannel] = "U2";
        m_plblCh[nChannel+1] = "U3";
        m_plblCh[nChannel+2] = "U4";
        m_plblCh[nChannel+3] = "U5";
        m_plblCh[nChannel+4] = "U6";
        m_plblCh[nChannel+5] = "U7";
    }
    else if(sValue == "U07" || sValue == "7.1")
    {
        Settings::Get().Write("ChannelMapping", wxString::Format("Ch_%d", nChannel+1), "");
        Settings::Get().Write("ChannelMapping", wxString::Format("Ch_%d", nChannel+2), "");
        Settings::Get().Write("ChannelMapping", wxString::Format("Ch_%d", nChannel+3), "");
        Settings::Get().Write("ChannelMapping", wxString::Format("Ch_%d", nChannel+4), "");
        Settings::Get().Write("ChannelMapping", wxString::Format("Ch_%d", nChannel+5), "");
        Settings::Get().Write("ChannelMapping", wxString::Format("Ch_%d", nChannel+6), "");
        Settings::Get().Write("ChannelMapping", wxString::Format("Ch_%d", nChannel+7), "");

        m_plblCh[nChannel-1] = "L";
        m_plblCh[nChannel] = "R";
        m_plblCh[nChannel+1] = "C";
        m_plblCh[nChannel+2] = "LFE";
        m_plblCh[nChannel+3] = "Ls";
        m_plblCh[nChannel+4] = "Rs";
        m_plblCh[nChannel+5] = "Lrs";
        m_plblCh[nChannel+6] = "Rrs";
    }
}

void pnlChannelMapping::ShowButtons(int nButton)
{
    if(nButton <= 8 && Settings::Get().Read("ChannelMapping", wxString::Format("Ch_%d", nButton), "") == "")
    {
        Settings::Get().Write("ChannelMapping", wxString::Format("Ch_%d", nButton), "U01");
    }
}
