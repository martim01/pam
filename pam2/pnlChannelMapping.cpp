#include "pnlChannelMapping.h"
#include "settingevent.h"
#include "settings.h"
#include "log.h"

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

const std::multimap<unsigned int, wxString> pnlChannelMapping::MAPPING = {{1,"Mono"},{1, "U01"},
                                                                          {2,"Dual Mono"},{2, "Stereo"},{2, "Matrix"},{2, "U02"},
                                                                          {3, "U03"},
                                                                          {4, "SGRP"},{4, "U04"},
                                                                          {5, "U05"},
                                                                          {6, "5.1"},{6, "U06"},
                                                                          {7, "U07"},
                                                                          {8 , "7.1"},{8, "U08"}};





BEGIN_EVENT_TABLE(pnlChannelMapping,wxPanel)
	//(*EventTable(pnlChannelMapping)
	//*)
END_EVENT_TABLE()

pnlChannelMapping::pnlChannelMapping(wxWindow* parent,wxWindowID id,const wxPoint& pos,const wxSize& size, long nStyle, wxString sNotused)
{
	Create(parent, id, wxDefaultPosition, wxSize(600,400), wxTAB_TRAVERSAL, _T("id"));
	SetBackgroundColour(wxColour(0,0,0));
	m_pLbl[0] = new wmLabel(this, ID_M_PLBL14, _("Channel 1"), wxPoint(10,40), wxSize(65,30), 0, _T("ID_M_PLBL14"));
	m_pLbl[0]->SetBorderState(uiRect::BORDER_NONE);
	m_pLbl[0]->GetUiRect().SetGradient(0);
	m_pLbl[0]->SetForegroundColour(wxColour(255,255,255));
	m_pLbl[0]->SetBackgroundColour(wxColour(64,0,128));
	m_pLbl[1] = new wmLabel(this, ID_M_PLBL15, _("Channel 2"), wxPoint(80,40), wxSize(65,30), 0, _T("ID_M_PLBL15"));
	m_pLbl[1]->SetBorderState(uiRect::BORDER_NONE);
	m_pLbl[1]->GetUiRect().SetGradient(0);
	m_pLbl[1]->SetForegroundColour(wxColour(255,255,255));
	m_pLbl[1]->SetBackgroundColour(wxColour(64,0,128));
	m_pLbl[2] = new wmLabel(this, ID_M_PLBL16, _("Channel 3"), wxPoint(150,40), wxSize(65,30), 0, _T("ID_M_PLBL16"));
	m_pLbl[2]->SetBorderState(uiRect::BORDER_NONE);
	m_pLbl[2]->GetUiRect().SetGradient(0);
	m_pLbl[2]->SetForegroundColour(wxColour(255,255,255));
	m_pLbl[2]->SetBackgroundColour(wxColour(64,0,128));
	m_pLbl[3] = new wmLabel(this, ID_M_PLBL17, _("Channel 4"), wxPoint(220,40), wxSize(65,30), 0, _T("ID_M_PLBL17"));
	m_pLbl[3]->SetBorderState(uiRect::BORDER_NONE);
	m_pLbl[3]->GetUiRect().SetGradient(0);
	m_pLbl[3]->SetForegroundColour(wxColour(255,255,255));
	m_pLbl[3]->SetBackgroundColour(wxColour(64,0,128));
	m_pLbl[4] = new wmLabel(this, ID_M_PLBL18, _("Channel 5"), wxPoint(290,40), wxSize(65,30), 0, _T("ID_M_PLBL18"));
	m_pLbl[4]->SetBorderState(uiRect::BORDER_NONE);
	m_pLbl[4]->GetUiRect().SetGradient(0);
	m_pLbl[4]->SetForegroundColour(wxColour(255,255,255));
	m_pLbl[4]->SetBackgroundColour(wxColour(64,0,128));
	m_pLbl[5] = new wmLabel(this, ID_M_PLBL19, _("Channel 6"), wxPoint(360,40), wxSize(65,30), 0, _T("ID_M_PLBL19"));
	m_pLbl[5]->SetBorderState(uiRect::BORDER_NONE);
	m_pLbl[5]->GetUiRect().SetGradient(0);
	m_pLbl[5]->SetForegroundColour(wxColour(255,255,255));
	m_pLbl[5]->SetBackgroundColour(wxColour(64,0,128));
	m_pLbl[6] = new wmLabel(this, ID_M_PLBL20, _("Channel 7"), wxPoint(430,40), wxSize(65,30), 0, _T("ID_M_PLBL20"));
	m_pLbl[6]->SetBorderState(uiRect::BORDER_NONE);
	m_pLbl[6]->GetUiRect().SetGradient(0);
	m_pLbl[6]->SetForegroundColour(wxColour(255,255,255));
	m_pLbl[6]->SetBackgroundColour(wxColour(64,0,128));
	m_pLbl[7] = new wmLabel(this, ID_M_PLBL21, _("Channel 8"), wxPoint(500,40), wxSize(65,30), 0, _T("ID_M_PLBL21"));
	m_pLbl[7]->SetBorderState(uiRect::BORDER_NONE);
	m_pLbl[7]->GetUiRect().SetGradient(0);
	m_pLbl[7]->SetForegroundColour(wxColour(255,255,255));
	m_pLbl[7]->SetBackgroundColour(wxColour(64,0,128));
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
	m_plblCh[0] = new wmLabel(this, ID_M_PLBL22, _("U1"), wxPoint(10,114), wxSize(65,30), 0, _T("ID_M_PLBL22"));
	m_plblCh[0]->SetBorderState(uiRect::BORDER_NONE);
	m_plblCh[0]->GetUiRect().SetGradient(0);
	m_plblCh[0]->SetForegroundColour(wxColour(0,0,0));
	m_plblCh[0]->SetBackgroundColour(wxColour(191,207,208));
	m_plblCh[1] = new wmLabel(this, ID_M_PLBL23, _("U1"), wxPoint(80,114), wxSize(65,30), 0, _T("ID_M_PLBL23"));
	m_plblCh[1]->SetBorderState(uiRect::BORDER_NONE);
	m_plblCh[1]->GetUiRect().SetGradient(0);
	m_plblCh[1]->SetForegroundColour(wxColour(0,0,0));
	m_plblCh[1]->SetBackgroundColour(wxColour(191,207,208));
	m_plblCh[2]= new wmLabel(this, ID_M_PLBL24, _("U1"), wxPoint(150,114), wxSize(65,30), 0, _T("ID_M_PLBL24"));
	m_plblCh[2]->SetBorderState(uiRect::BORDER_NONE);
	m_plblCh[2]->GetUiRect().SetGradient(0);
	m_plblCh[2]->SetForegroundColour(wxColour(0,0,0));
	m_plblCh[2]->SetBackgroundColour(wxColour(191,207,208));
	m_plblCh[3] = new wmLabel(this, ID_M_PLBL25, _("U1"), wxPoint(220,114), wxSize(65,30), 0, _T("ID_M_PLBL25"));
	m_plblCh[3]->SetBorderState(uiRect::BORDER_NONE);
	m_plblCh[3]->GetUiRect().SetGradient(0);
	m_plblCh[3]->SetForegroundColour(wxColour(0,0,0));
	m_plblCh[3]->SetBackgroundColour(wxColour(191,207,208));
	m_plblCh[4] = new wmLabel(this, ID_M_PLBL26, _("U1"), wxPoint(290,114), wxSize(65,30), 0, _T("ID_M_PLBL26"));
	m_plblCh[4]->SetBorderState(uiRect::BORDER_NONE);
	m_plblCh[4]->GetUiRect().SetGradient(0);
	m_plblCh[4]->SetForegroundColour(wxColour(0,0,0));
	m_plblCh[4]->SetBackgroundColour(wxColour(191,207,208));
	m_plblCh[5] = new wmLabel(this, ID_M_PLBL27, _("U1"), wxPoint(360,114), wxSize(65,30), 0, _T("ID_M_PLBL27"));
	m_plblCh[5]->SetBorderState(uiRect::BORDER_NONE);
	m_plblCh[5]->GetUiRect().SetGradient(0);
	m_plblCh[5]->SetForegroundColour(wxColour(0,0,0));
	m_plblCh[5]->SetBackgroundColour(wxColour(191,207,208));
	m_plblCh[6] = new wmLabel(this, ID_M_PLBL28, _("U1"), wxPoint(430,114), wxSize(65,30), 0, _T("ID_M_PLBL28"));
	m_plblCh[6]->SetBorderState(uiRect::BORDER_NONE);
	m_plblCh[6]->GetUiRect().SetGradient(0);
	m_plblCh[6]->SetForegroundColour(wxColour(0,0,0));
	m_plblCh[6]->SetBackgroundColour(wxColour(191,207,208));
	m_plblCh[7] = new wmLabel(this, ID_M_PLBL29, _("U1"), wxPoint(500,114), wxSize(65,30), 0, _T("ID_M_PLBL29"));
	m_plblCh[7]->SetBorderState(uiRect::BORDER_NONE);
	m_plblCh[7]->GetUiRect().SetGradient(0);
	m_plblCh[7]->SetForegroundColour(wxColour(0,0,0));
	m_plblCh[7]->SetBackgroundColour(wxColour(191,207,208));
	m_pbtnBack = new wmButton(this, ID_M_PBTN22, _("Back"), wxPoint(260,330), wxSize(80,40), wmButton::STYLE_NORMAL, wxDefaultValidator, _T("ID_M_PBTN22"));
	m_pbtnBack->SetForegroundColour(wxColour(255,255,255));
	m_pbtnBack->SetBackgroundColour(wxColour(0,64,0));
	m_pbtnBack->SetColourDisabled(wxColour(wxT("#B0B0B0")));
	SetSize(size);
	SetPosition(pos);



	Settings::Get().AddHandler(this, "Server", "ChannelMapping");
	Settings::Get().AddHandler(this, "Server", "Channels");


	Bind(wxEVT_SETTING_CHANGED, &pnlChannelMapping::OnSettingChanged, this);

	for(size_t i = 0; i < 8; i++)
    {
        Bind(wxEVT_COMMAND_BUTTON_CLICKED, &pnlChannelMapping::OnChannelClicked, this, m_pbtnCh[i]->GetId());
    }

    m_nChannels = Settings::Get().Read("Server", "Channels", 2);
    ShowChannels();

    ShowButtons();

}

pnlChannelMapping::~pnlChannelMapping()
{
	//(*Destroy(pnlChannelMapping)
	//*)
}

void pnlChannelMapping::OnChannelClicked(wxCommandEvent& event)
{
    for(size_t i = 0; i < 8 ; i++)
    {
        if(m_pbtnCh[i]->GetLabel() == )
    }
}

void pnlChannelMapping::OnSettingChanged(SettingEvent& event)
{
    unsigned long nButton;
    if(event.GetSection() == "Server")
    {
        if(event.GetKey() == "Channels")
        {
            m_nChannels = event.GetValue(2l);
            ShowChannels();
        }
    }

    if(event.GetKey().BeforeFirst('_') == "Ch" && event.GetKey().AfterFirst('_').ToULong(&nButton))
    {
        ShowButtons(event.GetValue(), nButton);
    }
}

void pnlChannelMapping::ShowButtons(const wxString& sValue, int nChannel)
{
    if(nChannel == 0)
    {
        return;
    }

    m_pbtnCh[nChannel-1]->Show(nChannel <= m_nChannels && (sValue.empty() == false));

    if(sValue == "Mono")
    {
        ShowButton(nChannel+1);

        m_plblCh[nChannel-1]->SetLabel("M");
    }
    else if(sValue == "U01")
    {
        ShowButton(nChannel+1);

        m_plblCh[nChannel-1]->SetLabel("U1");
    }
    else if(sValue == "Dual Mono")
    {
        Settings::Get().Write("ChannelMapping", wxString::Format("Ch_%d", nChannel+1), "");
        ShowButton(nChannel+2);

        m_plblCh[nChannel-1]->SetLabel("M1");
        m_plblCh[nChannel]->SetLabel("M2");
    }
    else if(sValue == "Stereo")
    {
        Settings::Get().Write("ChannelMapping", wxString::Format("Ch_%d", nChannel+1), "");
        ShowButton(nChannel+2);

        m_plblCh[nChannel-1]->SetLabel("L");
        m_plblCh[nChannel]->SetLabel("R");
    }
    else if(sValue == "Matrix")
    {
        Settings::Get().Write("ChannelMapping", wxString::Format("Ch_%d", nChannel+1), "");
        ShowButton(nChannel+2);

        m_plblCh[nChannel-1]->SetLabel("Lt");
        m_plblCh[nChannel]->SetLabel("Rt");
    }
    else if(sValue == "U02")
    {
        Settings::Get().Write("ChannelMapping", wxString::Format("Ch_%d", nChannel+1), "");
        ShowButton(nChannel+2);

        m_plblCh[nChannel-1]->SetLabel("U1");
        m_plblCh[nChannel]->SetLabel("U2");
    }
    else if(sValue == "U03")
    {
        Settings::Get().Write("ChannelMapping", wxString::Format("Ch_%d", nChannel+1), "");
        Settings::Get().Write("ChannelMapping", wxString::Format("Ch_%d", nChannel+2), "");
        ShowButton(nChannel+3);

        m_plblCh[nChannel-1]->SetLabel("U1");
        m_plblCh[nChannel]->SetLabel("U2");
        m_plblCh[nChannel+1]->SetLabel("U3");
    }
    else if(sValue == "U04")
    {
        Settings::Get().Write("ChannelMapping", wxString::Format("Ch_%d", nChannel+1), "");
        Settings::Get().Write("ChannelMapping", wxString::Format("Ch_%d", nChannel+2), "");
        Settings::Get().Write("ChannelMapping", wxString::Format("Ch_%d", nChannel+3), "");
        ShowButton(nChannel+4);

        m_plblCh[nChannel-1]->SetLabel("U1");
        m_plblCh[nChannel]->SetLabel("U2");
        m_plblCh[nChannel+1]->SetLabel("U3");
        m_plblCh[nChannel+2]->SetLabel("U4");
    }
    else if(sValue == "SGRP")
    {
        Settings::Get().Write("ChannelMapping", wxString::Format("Ch_%d", nChannel+1), "");
        Settings::Get().Write("ChannelMapping", wxString::Format("Ch_%d", nChannel+2), "");
        Settings::Get().Write("ChannelMapping", wxString::Format("Ch_%d", nChannel+3), "");
        ShowButton(nChannel+4);

        m_plblCh[nChannel-1]->SetLabel("1");
        m_plblCh[nChannel]->SetLabel("2");
        m_plblCh[nChannel+1]->SetLabel("3");
        m_plblCh[nChannel+2]->SetLabel("4");
    }
    else if(sValue == "U05")
    {
        Settings::Get().Write("ChannelMapping", wxString::Format("Ch_%d", nChannel+1), "");
        Settings::Get().Write("ChannelMapping", wxString::Format("Ch_%d", nChannel+2), "");
        Settings::Get().Write("ChannelMapping", wxString::Format("Ch_%d", nChannel+3), "");
        Settings::Get().Write("ChannelMapping", wxString::Format("Ch_%d", nChannel+4), "");
        ShowButton(nChannel+5);

        m_plblCh[nChannel-1]->SetLabel("U1");
        m_plblCh[nChannel]->SetLabel("U2");
        m_plblCh[nChannel+1]->SetLabel("U3");
        m_plblCh[nChannel+2]->SetLabel("U4");
        m_plblCh[nChannel+3]->SetLabel("U5");
    }
    else if(sValue == "U06")
    {
        Settings::Get().Write("ChannelMapping", wxString::Format("Ch_%d", nChannel+1), "");
        Settings::Get().Write("ChannelMapping", wxString::Format("Ch_%d", nChannel+2), "");
        Settings::Get().Write("ChannelMapping", wxString::Format("Ch_%d", nChannel+3), "");
        Settings::Get().Write("ChannelMapping", wxString::Format("Ch_%d", nChannel+4), "");
        Settings::Get().Write("ChannelMapping", wxString::Format("Ch_%d", nChannel+5), "");
        ShowButton(nChannel+6);

        m_plblCh[nChannel-1]->SetLabel("U1");
        m_plblCh[nChannel]->SetLabel("U2");
        m_plblCh[nChannel+1]->SetLabel("U3");
        m_plblCh[nChannel+2]->SetLabel("U4");
        m_plblCh[nChannel+3]->SetLabel("U5");
        m_plblCh[nChannel+4]->SetLabel("U6");

    }
    else if(sValue == "5.1")
    {
        Settings::Get().Write("ChannelMapping", wxString::Format("Ch_%d", nChannel+1), "");
        Settings::Get().Write("ChannelMapping", wxString::Format("Ch_%d", nChannel+2), "");
        Settings::Get().Write("ChannelMapping", wxString::Format("Ch_%d", nChannel+3), "");
        Settings::Get().Write("ChannelMapping", wxString::Format("Ch_%d", nChannel+4), "");
        Settings::Get().Write("ChannelMapping", wxString::Format("Ch_%d", nChannel+5), "");
        ShowButton(nChannel+6);

        m_plblCh[nChannel-1]->SetLabel("L");
        m_plblCh[nChannel]->SetLabel("R");
        m_plblCh[nChannel+1]->SetLabel("C");
        m_plblCh[nChannel+2]->SetLabel("LFE");
        m_plblCh[nChannel+3]->SetLabel("Ls");
        m_plblCh[nChannel+4]->SetLabel("Rs");
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

        m_plblCh[nChannel-1]->SetLabel("U1");
        m_plblCh[nChannel]->SetLabel("U2");
        m_plblCh[nChannel+1]->SetLabel("U3");
        m_plblCh[nChannel+2]->SetLabel("U4");
        m_plblCh[nChannel+3]->SetLabel("U5");
        m_plblCh[nChannel+4]->SetLabel("U6");
        m_plblCh[nChannel+5]->SetLabel("U7");
    }
    else if(sValue == "7.1")
    {
        Settings::Get().Write("ChannelMapping", wxString::Format("Ch_%d", nChannel+1), "");
        Settings::Get().Write("ChannelMapping", wxString::Format("Ch_%d", nChannel+2), "");
        Settings::Get().Write("ChannelMapping", wxString::Format("Ch_%d", nChannel+3), "");
        Settings::Get().Write("ChannelMapping", wxString::Format("Ch_%d", nChannel+4), "");
        Settings::Get().Write("ChannelMapping", wxString::Format("Ch_%d", nChannel+5), "");
        Settings::Get().Write("ChannelMapping", wxString::Format("Ch_%d", nChannel+6), "");
        Settings::Get().Write("ChannelMapping", wxString::Format("Ch_%d", nChannel+7), "");

        m_plblCh[nChannel-1]->SetLabel("L");
        m_plblCh[nChannel]->SetLabel("R");
        m_plblCh[nChannel+1]->SetLabel("C");
        m_plblCh[nChannel+2]->SetLabel("LFE");
        m_plblCh[nChannel+3]->SetLabel("Ls");
        m_plblCh[nChannel+4]->SetLabel("Rs");
        m_plblCh[nChannel+5]->SetLabel("Lrs");
        m_plblCh[nChannel+6]->SetLabel("Rrs");
    }
    else if(sValue == "U08")
    {
        Settings::Get().Write("ChannelMapping", wxString::Format("Ch_%d", nChannel+1), "");
        Settings::Get().Write("ChannelMapping", wxString::Format("Ch_%d", nChannel+2), "");
        Settings::Get().Write("ChannelMapping", wxString::Format("Ch_%d", nChannel+3), "");
        Settings::Get().Write("ChannelMapping", wxString::Format("Ch_%d", nChannel+4), "");
        Settings::Get().Write("ChannelMapping", wxString::Format("Ch_%d", nChannel+5), "");
        Settings::Get().Write("ChannelMapping", wxString::Format("Ch_%d", nChannel+6), "");
        Settings::Get().Write("ChannelMapping", wxString::Format("Ch_%d", nChannel+7), "");
        ShowButton(nChannel+8);

        m_plblCh[nChannel-1]->SetLabel("U1");
        m_plblCh[nChannel]->SetLabel("U2");
        m_plblCh[nChannel+1]->SetLabel("U3");
        m_plblCh[nChannel+2]->SetLabel("U4");
        m_plblCh[nChannel+3]->SetLabel("U5");
        m_plblCh[nChannel+4]->SetLabel("U6");
        m_plblCh[nChannel+5]->SetLabel("U7");
        m_plblCh[nChannel+6]->SetLabel("U8");
    }
}

void pnlChannelMapping::ShowButton(int nButton)
{
    if(nButton <= 8 && Settings::Get().Read("ChannelMapping", wxString::Format("Ch_%d", nButton), "") == "")
    {
        Settings::Get().Write("ChannelMapping", wxString::Format("Ch_%d", nButton), "U01");
    }
}

void pnlChannelMapping::ShowChannels()
{
    for(unsigned long i = 0; i < 8; i++)
    {
        m_pLbl[i]->Show(i < m_nChannels);

        m_plblCh[i]->Show(i < m_nChannels);

        //if(i >= m_nChannels)
        //{
        //    Settings::Get().Write("ChannelMapping", wxString::Format("Ch_%d", i+1), "");
        //}
        m_pbtnCh[i]->Show(i < m_nChannels && Settings::Get().Read("ChannelMapping", wxString::Format("Ch_%d", i+1), "")!="");

        std::vector<wxString> vPopup;
        for(auto pairMap : MAPPING)
        {
            if(pairMap.first + i <= m_nChannels)
            {
                vPopup.push_back(pairMap.second);
            }
        }
        m_pbtnCh[i]->SetPopup(vPopup);

    }
}

