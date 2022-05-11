#include "pnlChannelMapping.h"
#include "settingevent.h"
#include "settings.h"
#include "log.h"

//(*InternalHeaders(pnlChannelMapping)
#include <wx/intl.h>
#include <wx/string.h>
//*)

//(*IdInit(pnlChannelMapping)
const long pnlChannelMapping::ID_M_PBTN22 = wxNewId();
const long pnlChannelMapping::ID_M_PBTN1 = wxNewId();
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
const long pnlChannelMapping::ID_PANEL1 = wxNewId();
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

pnlChannelMapping::pnlChannelMapping(wxWindow* parent,const wxString& sSection, wxWindowID id,const wxPoint& pos,const wxSize& size, long nStyle, wxString sNotused) :
    m_sSection(sSection)
{
	Create(parent, id, wxDefaultPosition, wxSize(600,400), wxTAB_TRAVERSAL, _T("id"));
	SetBackgroundColour(wxColour(0,0,0));

	m_pbtnEnable = new wmButton(this, wxNewId(), _("Mapping"), wxPoint(200,40), wxSize(200,30), wmButton::STYLE_SELECT);
	m_pbtnEnable->SetToggle(true, "Disable", "Enable", 40);

    m_ppnlMapping = new wxPanel(this, 10, 80, 590, 200);
    m_ppnlMapping->SetBackgroundColour(*wxBLACK);

	m_pLbl[0] = new wmLabel(m_ppnlMapping, ID_M_PLBL14, _("Channel 1"), wxPoint(10,40), wxSize(65,30), 0, _T("ID_M_PLBL14"));
	m_pLbl[0]->SetBorderState(uiRect::BORDER_NONE);
	m_pLbl[0]->GetUiRect().SetGradient(0);
	m_pLbl[0]->SetForegroundColour(wxColour(255,255,255));
	m_pLbl[0]->SetBackgroundColour(wxColour(64,0,128));
	m_pLbl[1] = new wmLabel(m_ppnlMapping, ID_M_PLBL15, _("Channel 2"), wxPoint(80,40), wxSize(65,30), 0, _T("ID_M_PLBL15"));
	m_pLbl[1]->SetBorderState(uiRect::BORDER_NONE);
	m_pLbl[1]->GetUiRect().SetGradient(0);
	m_pLbl[1]->SetForegroundColour(wxColour(255,255,255));
	m_pLbl[1]->SetBackgroundColour(wxColour(64,0,128));
	m_pLbl[2] = new wmLabel(m_ppnlMapping, ID_M_PLBL16, _("Channel 3"), wxPoint(150,40), wxSize(65,30), 0, _T("ID_M_PLBL16"));
	m_pLbl[2]->SetBorderState(uiRect::BORDER_NONE);
	m_pLbl[2]->GetUiRect().SetGradient(0);
	m_pLbl[2]->SetForegroundColour(wxColour(255,255,255));
	m_pLbl[2]->SetBackgroundColour(wxColour(64,0,128));
	m_pLbl[3] = new wmLabel(m_ppnlMapping, ID_M_PLBL17, _("Channel 4"), wxPoint(220,40), wxSize(65,30), 0, _T("ID_M_PLBL17"));
	m_pLbl[3]->SetBorderState(uiRect::BORDER_NONE);
	m_pLbl[3]->GetUiRect().SetGradient(0);
	m_pLbl[3]->SetForegroundColour(wxColour(255,255,255));
	m_pLbl[3]->SetBackgroundColour(wxColour(64,0,128));
	m_pLbl[4] = new wmLabel(m_ppnlMapping, ID_M_PLBL18, _("Channel 5"), wxPoint(290,40), wxSize(65,30), 0, _T("ID_M_PLBL18"));
	m_pLbl[4]->SetBorderState(uiRect::BORDER_NONE);
	m_pLbl[4]->GetUiRect().SetGradient(0);
	m_pLbl[4]->SetForegroundColour(wxColour(255,255,255));
	m_pLbl[4]->SetBackgroundColour(wxColour(64,0,128));
	m_pLbl[5] = new wmLabel(m_ppnlMapping, ID_M_PLBL19, _("Channel 6"), wxPoint(360,40), wxSize(65,30), 0, _T("ID_M_PLBL19"));
	m_pLbl[5]->SetBorderState(uiRect::BORDER_NONE);
	m_pLbl[5]->GetUiRect().SetGradient(0);
	m_pLbl[5]->SetForegroundColour(wxColour(255,255,255));
	m_pLbl[5]->SetBackgroundColour(wxColour(64,0,128));
	m_pLbl[6] = new wmLabel(m_ppnlMapping, ID_M_PLBL20, _("Channel 7"), wxPoint(430,40), wxSize(65,30), 0, _T("ID_M_PLBL20"));
	m_pLbl[6]->SetBorderState(uiRect::BORDER_NONE);
	m_pLbl[6]->GetUiRect().SetGradient(0);
	m_pLbl[6]->SetForegroundColour(wxColour(255,255,255));
	m_pLbl[6]->SetBackgroundColour(wxColour(64,0,128));
	m_pLbl[7] = new wmLabel(m_ppnlMapping, ID_M_PLBL21, _("Channel 8"), wxPoint(500,40), wxSize(65,30), 0, _T("ID_M_PLBL21"));
	m_pLbl[7]->SetBorderState(uiRect::BORDER_NONE);
	m_pLbl[7]->GetUiRect().SetGradient(0);
	m_pLbl[7]->SetForegroundColour(wxColour(255,255,255));
	m_pLbl[7]->SetBackgroundColour(wxColour(64,0,128));
	m_pbtnCh[0] = new wmButton(m_ppnlMapping, ID_M_PBTN14, _("Undefined"), wxPoint(10,72), wxSize(65,40), wmButton::STYLE_NORMAL, wxDefaultValidator, _T("ID_M_PBTN14"));
	m_pbtnCh[0]->SetForegroundColour(wxColour(0,0,0));
	m_pbtnCh[0]->SetBackgroundColour(wxColour(255,255,255));
	m_pbtnCh[0]->SetColourDisabled(wxColour(wxT("#B0B0B0")));
	m_pbtnCh[1] = new wmButton(m_ppnlMapping, ID_M_PBTN15, _("Undefined"), wxPoint(80,72), wxSize(65,40), wmButton::STYLE_NORMAL, wxDefaultValidator, _T("ID_M_PBTN15"));
	m_pbtnCh[1]->SetForegroundColour(wxColour(0,0,0));
	m_pbtnCh[1]->SetBackgroundColour(wxColour(255,255,255));
	m_pbtnCh[1]->SetColourDisabled(wxColour(wxT("#B0B0B0")));
	m_pbtnCh[2] = new wmButton(m_ppnlMapping, ID_M_PBTN16, _("Undefined"), wxPoint(150,72), wxSize(65,40), wmButton::STYLE_NORMAL, wxDefaultValidator, _T("ID_M_PBTN16"));
	m_pbtnCh[2]->SetForegroundColour(wxColour(0,0,0));
	m_pbtnCh[2]->SetBackgroundColour(wxColour(255,255,255));
	m_pbtnCh[2]->SetColourDisabled(wxColour(wxT("#B0B0B0")));
	m_pbtnCh[3] = new wmButton(m_ppnlMapping, ID_M_PBTN17, _("Undefined"), wxPoint(220,72), wxSize(65,40), wmButton::STYLE_NORMAL, wxDefaultValidator, _T("ID_M_PBTN17"));
	m_pbtnCh[3]->SetForegroundColour(wxColour(0,0,0));
	m_pbtnCh[3]->SetBackgroundColour(wxColour(255,255,255));
	m_pbtnCh[3]->SetColourDisabled(wxColour(wxT("#B0B0B0")));
	m_pbtnCh[4] = new wmButton(m_ppnlMapping, ID_M_PBTN18, _("Undefined"), wxPoint(290,72), wxSize(65,40), wmButton::STYLE_NORMAL, wxDefaultValidator, _T("ID_M_PBTN18"));
	m_pbtnCh[4]->SetForegroundColour(wxColour(0,0,0));
	m_pbtnCh[4]->SetBackgroundColour(wxColour(255,255,255));
	m_pbtnCh[4]->SetColourDisabled(wxColour(wxT("#B0B0B0")));
	m_pbtnCh[5] = new wmButton(m_ppnlMapping, ID_M_PBTN19, _("Undefined"), wxPoint(360,72), wxSize(65,40), wmButton::STYLE_NORMAL, wxDefaultValidator, _T("ID_M_PBTN19"));
	m_pbtnCh[5]->SetForegroundColour(wxColour(0,0,0));
	m_pbtnCh[5]->SetBackgroundColour(wxColour(255,255,255));
	m_pbtnCh[5]->SetColourDisabled(wxColour(wxT("#B0B0B0")));
	m_pbtnCh[6] = new wmButton(m_ppnlMapping, ID_M_PBTN20, _("Undefined"), wxPoint(430,72), wxSize(65,40), wmButton::STYLE_NORMAL, wxDefaultValidator, _T("ID_M_PBTN20"));
	m_pbtnCh[6]->SetForegroundColour(wxColour(0,0,0));
	m_pbtnCh[6]->SetBackgroundColour(wxColour(255,255,255));
	m_pbtnCh[6]->SetColourDisabled(wxColour(wxT("#B0B0B0")));
	m_pbtnCh[7] = new wmButton(m_ppnlMapping, ID_M_PBTN21, _("Undefined"), wxPoint(500,72), wxSize(65,40), wmButton::STYLE_NORMAL, wxDefaultValidator, _T("ID_M_PBTN21"));
	m_pbtnCh[7]->SetForegroundColour(wxColour(0,0,0));
	m_pbtnCh[7]->SetBackgroundColour(wxColour(255,255,255));
	m_pbtnCh[7]->SetColourDisabled(wxColour(wxT("#B0B0B0")));
	m_plblCh[0] = new wmLabel(m_ppnlMapping, ID_M_PLBL22, _("U1"), wxPoint(10,114), wxSize(65,30), 0, _T("ID_M_PLBL22"));
	m_plblCh[0]->SetBorderState(uiRect::BORDER_NONE);
	m_plblCh[0]->GetUiRect().SetGradient(0);
	m_plblCh[0]->SetForegroundColour(wxColour(0,0,0));
	m_plblCh[0]->SetBackgroundColour(wxColour(191,207,208));
	m_plblCh[1] = new wmLabel(m_ppnlMapping, ID_M_PLBL23, _("U1"), wxPoint(80,114), wxSize(65,30), 0, _T("ID_M_PLBL23"));
	m_plblCh[1]->SetBorderState(uiRect::BORDER_NONE);
	m_plblCh[1]->GetUiRect().SetGradient(0);
	m_plblCh[1]->SetForegroundColour(wxColour(0,0,0));
	m_plblCh[1]->SetBackgroundColour(wxColour(191,207,208));
	m_plblCh[2]= new wmLabel(m_ppnlMapping, ID_M_PLBL24, _("U1"), wxPoint(150,114), wxSize(65,30), 0, _T("ID_M_PLBL24"));
	m_plblCh[2]->SetBorderState(uiRect::BORDER_NONE);
	m_plblCh[2]->GetUiRect().SetGradient(0);
	m_plblCh[2]->SetForegroundColour(wxColour(0,0,0));
	m_plblCh[2]->SetBackgroundColour(wxColour(191,207,208));
	m_plblCh[3] = new wmLabel(m_ppnlMapping, ID_M_PLBL25, _("U1"), wxPoint(220,114), wxSize(65,30), 0, _T("ID_M_PLBL25"));
	m_plblCh[3]->SetBorderState(uiRect::BORDER_NONE);
	m_plblCh[3]->GetUiRect().SetGradient(0);
	m_plblCh[3]->SetForegroundColour(wxColour(0,0,0));
	m_plblCh[3]->SetBackgroundColour(wxColour(191,207,208));
	m_plblCh[4] = new wmLabel(m_ppnlMapping, ID_M_PLBL26, _("U1"), wxPoint(290,114), wxSize(65,30), 0, _T("ID_M_PLBL26"));
	m_plblCh[4]->SetBorderState(uiRect::BORDER_NONE);
	m_plblCh[4]->GetUiRect().SetGradient(0);
	m_plblCh[4]->SetForegroundColour(wxColour(0,0,0));
	m_plblCh[4]->SetBackgroundColour(wxColour(191,207,208));
	m_plblCh[5] = new wmLabel(m_ppnlMapping, ID_M_PLBL27, _("U1"), wxPoint(360,114), wxSize(65,30), 0, _T("ID_M_PLBL27"));
	m_plblCh[5]->SetBorderState(uiRect::BORDER_NONE);
	m_plblCh[5]->GetUiRect().SetGradient(0);
	m_plblCh[5]->SetForegroundColour(wxColour(0,0,0));
	m_plblCh[5]->SetBackgroundColour(wxColour(191,207,208));
	m_plblCh[6] = new wmLabel(m_ppnlMapping, ID_M_PLBL28, _("U1"), wxPoint(430,114), wxSize(65,30), 0, _T("ID_M_PLBL28"));
	m_plblCh[6]->SetBorderState(uiRect::BORDER_NONE);
	m_plblCh[6]->GetUiRect().SetGradient(0);
	m_plblCh[6]->SetForegroundColour(wxColour(0,0,0));
	m_plblCh[6]->SetBackgroundColour(wxColour(191,207,208));
	m_plblCh[7] = new wmLabel(m_ppnlMapping, ID_M_PLBL29, _("U1"), wxPoint(500,114), wxSize(65,30), 0, _T("ID_M_PLBL29"));
	m_plblCh[7]->SetBorderState(uiRect::BORDER_NONE);
	m_plblCh[7]->GetUiRect().SetGradient(0);
	m_plblCh[7]->SetForegroundColour(wxColour(0,0,0));
	m_plblCh[7]->SetBackgroundColour(wxColour(191,207,208));
	m_pbtnBack = new wmButton(m_ppnlMapping, ID_M_PBTN22, _("Back"), wxPoint(260,330), wxSize(80,40), wmButton::STYLE_NORMAL, wxDefaultValidator, _T("ID_M_PBTN22"));
	m_pbtnBack->SetForegroundColour(wxColour(255,255,255));
	m_pbtnBack->SetBackgroundColour(wxColour(0,64,0));
	m_pbtnBack->SetColourDisabled(wxColour(wxT("#B0B0B0")));
	SetSize(size);
	SetPosition(pos);

    m_pbtnMapping->ConnectToSetting(m_sSection, "MappingEnabled", false);

	Settings::Get().AddHandler(this, m_sSection, "ChannelMapping");
	Settings::Get().AddHandler(this, m_sSection, "Channels");
	Settings::Get().AddHandler(this, m_sSection, "MappingEnabled");


	Bind(wxEVT_SETTING_CHANGED, &pnlChannelMapping::OnSettingChanged, this);

	for(size_t i = 0; i < 8; i++)
    {
        Bind(wxEVT_COMMAND_BUTTON_CLICKED, &pnlChannelMapping::OnChannelClicked, this, m_pbtnCh[i]->GetId());
    }

    m_nChannels = Settings::Get().Read(m_sSection, "Channels", 2);
    ShowChannels();

    ShowMapping(Settings::Get().Read(m_sSection, "ChannelMapping", "St"));
    //ShowButtons();

}

pnlChannelMapping::~pnlChannelMapping()
{
	//(*Destroy(pnlChannelMapping)
	//*)
}

void pnlChannelMapping::ShowButton(int nButton)
{
    if(nButton < m_nChannels)
    {
        if(m_pbtnCh[nButton]->IsShown() == false)
        {
            m_pbtnCh[nButton]->SetLabel("U01");
            m_pbtnCh[nButton]->Show(true);
            ShowButton(nButton+1);
        }
    }
}

void pnlChannelMapping::HideButtons(int nButton, int nCount)
{
    for(int i = nButton; i < nCount+nButton; i++)
    {
        if(i < 8)
        {
            m_pbtnCh[i]->SetLabel("");
            m_pbtnCh[i]->Show(false);
        }
    }
}

void pnlChannelMapping::OnChannelClicked(wxCommandEvent& event)
{
    for(size_t i = 0; i < m_nChannels ; i++)
    {
        if(m_pbtnCh[i]->IsShown())
        {
            if(m_pbtnCh[i]->GetLabel() == "Mono")
            {
                ShowButton(i+1);
            }
            else if(m_pbtnCh[i]->GetLabel() == "U01")
            {
                ShowButton(i+1);
            }
            else if(m_pbtnCh[i]->GetLabel() == "Dual Mono")
            {
                HideButtons(i+1, 1);
                ShowButton(i+2);
            }
            else if(m_pbtnCh[i]->GetLabel() == "Stereo")
            {
                HideButtons(i+1, 1);
                ShowButton(i+2);
            }
            else if(m_pbtnCh[i]->GetLabel() == "Matrix")
            {
                HideButtons(i+1, 1);
                ShowButton(i+2);
            }
            else if(m_pbtnCh[i]->GetLabel() == "U02")
            {
                HideButtons(i+1, 1);
                ShowButton(i+2);
            }
            else if(m_pbtnCh[i]->GetLabel() == "U03")
            {
                HideButtons(i+1, 2);
                ShowButton(i+3);
            }
            else if(m_pbtnCh[i]->GetLabel() == "U04")
            {
                HideButtons(i+1, 3);
                ShowButton(i+4);
            }
            else if(m_pbtnCh[i]->GetLabel() == "SGRP")
            {
                HideButtons(i+1, 3);
                ShowButton(i+4);
            }
            else if(m_pbtnCh[i]->GetLabel() == "U05")
            {
                HideButtons(i+1, 4);
                ShowButton(i+5);
            }
            else if(m_pbtnCh[i]->GetLabel() == "U06")
            {
                HideButtons(i+1, 5);
                ShowButton(i+6);
            }
            else if(m_pbtnCh[i]->GetLabel() == "5.1")
            {
                HideButtons(i+1, 5);
                ShowButton(i+6);
            }
            else if(m_pbtnCh[i]->GetLabel() == "U07")
            {
                HideButtons(i+1, 6);
                ShowButton(i+7);
            }
            else if(m_pbtnCh[i]->GetLabel() == "7.1")
            {
                HideButtons(i+1, 7);
            }
            else if(m_pbtnCh[i]->GetLabel() == "U08")
            {
                HideButtons(i+1, 7);
            }
        }
    }
    ShowChannelLabels();
    SaveSettings();
}

void pnlChannelMapping::SaveSettings()
{
    wxString sMapping, sChannelMapping;
    for(size_t i = 0; i < m_nChannels ; i++)
    {
        if(m_pbtnCh[i]->IsShown())
        {
            sMapping = m_pbtnCh[i]->GetLabel();
            if(sMapping == "Mono")
            {
                sMapping = "M";
            }
            else if(sMapping == "Dual Mono")
            {
                sMapping = "DM";
            }
            else if(sMapping == "Stereo")
            {
                sMapping = "St";
            }
            else if(sMapping == "Matrix")
            {
                sMapping = "LtRt";
            }
            else if(sMapping == "5.1")
            {
                sMapping = "51";
            }
            else if(m_pbtnCh[i]->GetLabel() == "7.1")
            {
                sMapping = "71";
            }

            if(sChannelMapping.empty() == false)
            {
                sChannelMapping += ",";
            }
            sChannelMapping += sMapping;
        }
    }
    Settings::Get().Write(m_sSection, "ChannelMapping", sChannelMapping);
}


void pnlChannelMapping::OnSettingChanged(SettingEvent& event)
{
    unsigned long nButton;
    if(event.GetSection() == m_sSection)
    {
        if(event.GetKey() == "Channels")
        {
            m_nChannels = event.GetValue(2l);
            ShowChannels();
            ShowMapping(Settings::Get().Read(m_sSection, "ChannelMapping", "St"));
        }
        else if(event.GetKey() == "MappingEnabled")
        {
            m_ppnlMapping->Show(event.GetValue(false));
        }
    }
}

void pnlChannelMapping::ShowMapping(const wxString& sMapping)
{
    wxArrayString asMapping = wxStringTokenize(sMapping, ",");
    int nButton = 0;
    for(size_t j = 0; j < asMapping.GetCount() && nButton < m_nChannels; j++)
    {
        if(asMapping[j].CmpNoCase("M") == 0)
        {
            m_pbtnCh[nButton]->Show();
            m_pbtnCh[nButton]->SetLabel("Mono");

            ++nButton;
        }
        else if(asMapping[j].CmpNoCase("U01") == 0)
        {
            m_pbtnCh[nButton]->Show();
            m_pbtnCh[nButton]->SetLabel("U01");

            ++nButton;
        }
        else if(asMapping[j].CmpNoCase("DM") == 0)
        {
            m_pbtnCh[nButton]->Show();
            m_pbtnCh[nButton]->SetLabel("Dual Mono");

            HideButtons(nButton+1, 1);
            nButton+=2;

        }
        else if(asMapping[j].CmpNoCase("ST") == 0)
        {
            m_pbtnCh[nButton]->Show();
            m_pbtnCh[nButton]->SetLabel("Stereo");

            HideButtons(nButton+1, 1);
            nButton+=2;
        }
        else if(asMapping[j].CmpNoCase("LtRt") == 0)
        {
            m_pbtnCh[nButton]->Show();
            m_pbtnCh[nButton]->SetLabel("Matrix");

            HideButtons(nButton+1, 1);
            nButton+=2;
        }
        else if(asMapping[j].CmpNoCase("U02") == 0)
        {
            m_pbtnCh[nButton]->Show();
            m_pbtnCh[nButton]->SetLabel("U02");

            HideButtons(nButton+1, 1);
            nButton+=2;
        }
        else if(asMapping[j].CmpNoCase("U03") == 0)
        {
            m_pbtnCh[nButton]->Show();
            m_pbtnCh[nButton]->SetLabel("U03");

            HideButtons(nButton+1, 2);
            nButton+=3;
        }
        else if(asMapping[j].CmpNoCase("U04") == 0)
        {
            m_pbtnCh[nButton]->Show();
            m_pbtnCh[nButton]->SetLabel("U04");

            HideButtons(nButton+1, 3);
            nButton+=4;
        }
        else if(asMapping[j].CmpNoCase("SGRP") == 0)
        {
            m_pbtnCh[nButton]->Show();
            m_pbtnCh[nButton]->SetLabel("SGRP");

            HideButtons(nButton+1, 3);
            nButton+=4;
        }
        else if(asMapping[j].CmpNoCase("U05") == 0)
        {
            m_pbtnCh[nButton]->Show();
            m_pbtnCh[nButton]->SetLabel("U02");

            HideButtons(nButton+1, 4);
            nButton+=5;
        }
        else if(asMapping[j].CmpNoCase("U06") == 0)
        {
            m_pbtnCh[nButton]->Show();
            m_pbtnCh[nButton]->SetLabel("U06");

            HideButtons(nButton+1, 5);
            nButton+=6;
        }
        else if(asMapping[j].CmpNoCase("51") == 0)
        {
            m_pbtnCh[nButton]->Show();
            m_pbtnCh[nButton]->SetLabel("5.1");

            HideButtons(nButton+1, 5);
            nButton+=6;
        }
        else if(asMapping[j].CmpNoCase("U07") == 0)
        {
            m_pbtnCh[nButton]->Show();
            m_pbtnCh[nButton]->SetLabel("U07");

            HideButtons(nButton+1, 6);
            nButton+=7;
        }
        else if(asMapping[j].CmpNoCase("71") == 0)
        {
            m_pbtnCh[nButton]->Show();
            m_pbtnCh[nButton]->SetLabel("7.1");

            HideButtons(nButton+1, 7);
            nButton+=8;
        }
        else if(asMapping[j].CmpNoCase("U08") == 0)
        {
            m_pbtnCh[nButton]->Show();
            m_pbtnCh[nButton]->SetLabel("U08");

            HideButtons(nButton+1, 7);
            nButton+=8;
        }
    }
    ShowChannelLabels();
}

void pnlChannelMapping::ShowChannelLabels()
{
    for(size_t i = 0; i < m_nChannels ; i++)
    {
        if(m_pbtnCh[i]->IsShown())
        {
            if(m_pbtnCh[i]->GetLabel() == "Mono")
            {
                m_plblCh[i]->SetLabel("M");
            }
            else if(m_pbtnCh[i]->GetLabel() == "U01")
            {
                m_plblCh[i]->SetLabel("U1");
            }
            else if(m_pbtnCh[i]->GetLabel() == "Dual Mono")
            {
                m_plblCh[i]->SetLabel("M1");
                m_plblCh[i+1]->SetLabel("M2");
            }
            else if(m_pbtnCh[i]->GetLabel() == "Stereo")
            {
                m_plblCh[i]->SetLabel("L");
                m_plblCh[i+1]->SetLabel("R");
            }
            else if(m_pbtnCh[i]->GetLabel() == "Matrix")
            {
                m_plblCh[i]->SetLabel("Lt");
                m_plblCh[i+1]->SetLabel("Rt");
            }
            else if(m_pbtnCh[i]->GetLabel() == "U02")
            {
                m_plblCh[i]->SetLabel("U1");
                m_plblCh[i+1]->SetLabel("U2");
            }
            else if(m_pbtnCh[i]->GetLabel() == "U03")
            {
                m_plblCh[i]->SetLabel("U1");
                m_plblCh[i+1]->SetLabel("U2");
                m_plblCh[i+2]->SetLabel("U3");
            }
            else if(m_pbtnCh[i]->GetLabel() == "U04")
            {
                m_plblCh[i]->SetLabel("U1");
                m_plblCh[i+1]->SetLabel("U2");
                m_plblCh[i+2]->SetLabel("U3");
                m_plblCh[i+3]->SetLabel("U4");
            }
            else if(m_pbtnCh[i]->GetLabel() == "SGRP")
            {
                m_plblCh[i]->SetLabel("1");
                m_plblCh[i+1]->SetLabel("2");
                m_plblCh[i+2]->SetLabel("3");
                m_plblCh[i+3]->SetLabel("4");
            }
            else if(m_pbtnCh[i]->GetLabel() == "U05")
            {
                m_plblCh[i]->SetLabel("U1");
                m_plblCh[i+1]->SetLabel("U2");
                m_plblCh[i+2]->SetLabel("U3");
                m_plblCh[i+3]->SetLabel("U4");
                m_plblCh[i+4]->SetLabel("U5");
            }
            else if(m_pbtnCh[i]->GetLabel() == "U06")
            {
                m_plblCh[i]->SetLabel("U1");
                m_plblCh[i+1]->SetLabel("U2");
                m_plblCh[i+2]->SetLabel("U3");
                m_plblCh[i+3]->SetLabel("U4");
                m_plblCh[i+4]->SetLabel("U5");
                m_plblCh[i+5]->SetLabel("U6");
            }
            else if(m_pbtnCh[i]->GetLabel() == "5.1")
            {
                m_plblCh[i]->SetLabel("L");
                m_plblCh[i+1]->SetLabel("R");
                m_plblCh[i+2]->SetLabel("C");
                m_plblCh[i+3]->SetLabel("LFE");
                m_plblCh[i+4]->SetLabel("Ls");
                m_plblCh[i+5]->SetLabel("Rs");
            }
            else if(m_pbtnCh[i]->GetLabel() == "U07")
            {
                m_plblCh[i]->SetLabel("U1");
                m_plblCh[i+1]->SetLabel("U2");
                m_plblCh[i+2]->SetLabel("U3");
                m_plblCh[i+3]->SetLabel("U4");
                m_plblCh[i+4]->SetLabel("U5");
                m_plblCh[i+5]->SetLabel("U6");
                m_plblCh[i+6]->SetLabel("U7");
            }
            else if(m_pbtnCh[i]->GetLabel() == "7.1")
            {
                m_plblCh[i]->SetLabel("L");
                m_plblCh[i+1]->SetLabel("R");
                m_plblCh[i+2]->SetLabel("C");
                m_plblCh[i+3]->SetLabel("LFE");
                m_plblCh[i+4]->SetLabel("Ls");
                m_plblCh[i+5]->SetLabel("Rs");
                m_plblCh[i+6]->SetLabel("Lrs");
                m_plblCh[i+7]->SetLabel("Rrs");
            }
            else if(m_pbtnCh[i]->GetLabel() == "U08")
            {
                m_plblCh[i]->SetLabel("U1");
                m_plblCh[i+1]->SetLabel("U2");
                m_plblCh[i+2]->SetLabel("U3");
                m_plblCh[i+3]->SetLabel("U4");
                m_plblCh[i+4]->SetLabel("U5");
                m_plblCh[i+5]->SetLabel("U6");
                m_plblCh[i+6]->SetLabel("U7");
                m_plblCh[i+7]->SetLabel("U8");
            }
        }
    }

}


void pnlChannelMapping::ShowChannels()
{
    for(unsigned long i = 0; i < 8; i++)
    {
        m_pLbl[i]->Show(i < m_nChannels);
        m_plblCh[i]->Show(i < m_nChannels);
        m_pbtnCh[i]->Show(i < m_nChannels);

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

