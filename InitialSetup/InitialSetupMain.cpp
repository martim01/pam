/***************************************************************
 * Name:      InitialSetupMain.cpp
 * Purpose:   Code for Application Frame
 * Author:    Matthew Martin (martim01@outlook.com)
 * Created:   2021-02-22
 * Copyright: Matthew Martin (https://github.com/martim01)
 * License:
 **************************************************************/

#include "InitialSetupMain.h"
#include <wx/msgdlg.h>
#include "settings.h"
#include <wx/log.h>
//(*InternalHeaders(InitialSetupDialog)
#include <wx/font.h>
#include <wx/intl.h>
#include <wx/string.h>
//*)
#include <wx/textfile.h>
#include <iostream>
#include <wx/tokenzr.h>

#include <wx/xml/xml.h>
#ifdef __WXGNU__
#include <unistd.h>
#endif // __WXGNU__
//helper functions
enum wxbuildinfoformat {
    short_f, long_f };

wxString wxbuildinfo(wxbuildinfoformat format)
{
    wxString wxbuild(wxVERSION_STRING);

    if (format == long_f )
    {
#if defined(__WXMSW__)
        wxbuild << _T("-Windows");
#elif defined(__UNIX__)
        wxbuild << _T("-Linux");
#endif

#if wxUSE_UNICODE
        wxbuild << _T("-Unicode build");
#else
        wxbuild << _T("-ANSI build");
#endif // wxUSE_UNICODE
    }

    return wxbuild;
}

//(*IdInit(InitialSetupDialog)
const long InitialSetupDialog::ID_M_PLBL37 = wxNewId();
const long InitialSetupDialog::ID_M_PLBL13 = wxNewId();
const long InitialSetupDialog::ID_M_PLBL1 = wxNewId();
const long InitialSetupDialog::ID_M_PEDT1 = wxNewId();
const long InitialSetupDialog::ID_M_PKBD1 = wxNewId();
const long InitialSetupDialog::ID_PANEL1 = wxNewId();
const long InitialSetupDialog::ID_M_PLBL2 = wxNewId();
const long InitialSetupDialog::ID_M_PEDT2 = wxNewId();
const long InitialSetupDialog::ID_M_PKBD2 = wxNewId();
const long InitialSetupDialog::ID_PANEL2 = wxNewId();
const long InitialSetupDialog::ID_M_PLBL3 = wxNewId();
const long InitialSetupDialog::ID_M_PLST1 = wxNewId();
const long InitialSetupDialog::ID_PANEL3 = wxNewId();
const long InitialSetupDialog::ID_M_PSWP1 = wxNewId();
const long InitialSetupDialog::ID_M_PBTN7 = wxNewId();
//*)

const wxString InitialSetupDialog::STR_DTPARAM = "dtparam=audio";
const wxString InitialSetupDialog::STR_DTOVERALY = "dtoverlay=";

BEGIN_EVENT_TABLE(InitialSetupDialog,wxDialog)
    //(*EventTable(InitialSetupDialog)
    //*)
END_EVENT_TABLE()

InitialSetupDialog::InitialSetupDialog(wxWindow* parent,wxWindowID id)
{
    //(*Initialize(InitialSetupDialog)
    Create(parent, id, _("wxWidgets app"), wxDefaultPosition, wxDefaultSize, wxNO_BORDER, _T("id"));
    SetClientSize(wxSize(800,480));
    SetBackgroundColour(wxColour(0,0,0));
    m_plblHostname = new wmLabel(this, ID_M_PLBL37, _("Initial Setup"), wxPoint(0,0), wxSize(800,40), 0, _T("ID_M_PLBL37"));
    m_plblHostname->SetBorderState(uiRect::BORDER_NONE);
    m_plblHostname->GetUiRect().SetGradient(0);
    m_plblHostname->SetForegroundColour(wxColour(255,255,255));
    m_plblHostname->SetBackgroundColour(wxColour(61,146,146));
    wxFont m_plblHostnameFont(12,wxFONTFAMILY_SWISS,wxFONTSTYLE_NORMAL,wxFONTWEIGHT_BOLD,false,_T("Arial"),wxFONTENCODING_DEFAULT);
    m_plblHostname->SetFont(m_plblHostnameFont);
    m_pswp = new wmSwitcherPanel(this, ID_M_PSWP1, wxPoint(0,40), wxSize(700,440), wmSwitcherPanel::STYLE_NOSWIPE|wmSwitcherPanel::STYLE_NOANIMATION, _T("ID_M_PSWP1"));
    m_pswp->SetPageNameStyle(3);
    pnlHostname = new wxPanel(m_pswp, ID_PANEL1, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL, _T("ID_PANEL1"));
    pnlHostname->SetBackgroundColour(wxColour(0,0,0));
    m_pLbl1 = new wmLabel(pnlHostname, ID_M_PLBL1, _("Enter the Hostname for this unit:"), wxPoint(5,5), wxSize(600,30), 0, _T("ID_M_PLBL1"));
    m_pLbl1->SetBorderState(uiRect::BORDER_NONE);
    m_pLbl1->GetUiRect().SetGradient(0);
    m_pLbl1->SetForegroundColour(wxColour(255,255,255));
    m_pLbl1->SetBackgroundColour(wxColour(0,0,0));
    wxFont m_pLbl1Font(12,wxFONTFAMILY_SWISS,wxFONTSTYLE_NORMAL,wxFONTWEIGHT_BOLD,false,_T("Arial"),wxFONTENCODING_DEFAULT);
    m_pLbl1->SetFont(m_pLbl1Font);
    m_pedtName = new wmEdit(pnlHostname, ID_M_PEDT1, wxEmptyString, wxPoint(10,40), wxSize(600,45), 0, wxDefaultValidator, _T("ID_M_PEDT1"));
    m_pedtName->SetValidation(wmEdit::ASCII);
    m_pedtName->SetBackgroundColour(wxColour(255,255,255));
    m_pedtName->SetFocusedBackground(wxColour(wxT("#FFFFFF")));
    m_pedtName->SetFocusedForeground(wxColour(wxT("#000080")));
    m_pedtName->SetBorderStyle(1,1);
    m_pkdbHost = new wmKeyboard(pnlHostname, ID_M_PKBD1, wxPoint(10,90), wxDefaultSize, 0, 0);
    m_pkdbHost->SetForegroundColour(wxColour(255,255,255));
    pnlPassword = new wxPanel(m_pswp, ID_PANEL2, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL, _T("ID_PANEL2"));
    pnlPassword->SetBackgroundColour(wxColour(0,0,0));
    m_pLbl2 = new wmLabel(pnlPassword, ID_M_PLBL2, _("Enter a password for the default user:"), wxPoint(5,5), wxSize(600,30), 0, _T("ID_M_PLBL2"));
    m_pLbl2->SetBorderState(uiRect::BORDER_NONE);
    m_pLbl2->GetUiRect().SetGradient(0);
    m_pLbl2->SetForegroundColour(wxColour(255,255,255));
    m_pLbl2->SetBackgroundColour(wxColour(0,0,0));
    wxFont m_pLbl2Font(12,wxFONTFAMILY_SWISS,wxFONTSTYLE_NORMAL,wxFONTWEIGHT_BOLD,false,_T("Arial"),wxFONTENCODING_DEFAULT);
    m_pLbl2->SetFont(m_pLbl2Font);
    m_pedtPassword = new wmEdit(pnlPassword, ID_M_PEDT2, wxEmptyString, wxPoint(10,40), wxSize(600,45), 0, wxDefaultValidator, _T("ID_M_PEDT2"));
    m_pedtPassword->SetValidation(wmEdit::ASCII);
    m_pedtPassword->SetBackgroundColour(wxColour(255,255,255));
    m_pedtPassword->SetFocusedBackground(wxColour(wxT("#FFFFFF")));
    m_pedtPassword->SetFocusedForeground(wxColour(wxT("#000080")));
    m_pedtPassword->SetBorderStyle(1,1);
    m_pKbd2 = new wmKeyboard(pnlPassword, ID_M_PKBD2, wxPoint(10,90), wxDefaultSize, 0, 0);
    m_pKbd2->SetForegroundColour(wxColour(255,255,255));
    pnlAudio = new wxPanel(m_pswp, ID_PANEL3, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL, _T("ID_PANEL3"));
    pnlAudio->SetBackgroundColour(wxColour(0,0,0));
    m_pLbl3 = new wmLabel(pnlAudio, ID_M_PLBL3, _("Select the Audio HAT"), wxPoint(5,5), wxSize(700,30), 0, _T("ID_M_PLBL3"));
    m_pLbl3->SetBorderState(uiRect::BORDER_NONE);
    m_pLbl3->GetUiRect().SetGradient(0);
    m_pLbl3->SetForegroundColour(wxColour(255,255,255));
    m_pLbl3->SetBackgroundColour(wxColour(0,0,0));
    wxFont m_pLbl3Font(12,wxFONTFAMILY_SWISS,wxFONTSTYLE_NORMAL,wxFONTWEIGHT_BOLD,false,_T("Arial"),wxFONTENCODING_DEFAULT);
    m_pLbl3->SetFont(m_pLbl3Font);
    m_plstHat = new wmList(pnlAudio, ID_M_PLST1, wxPoint(0,35), wxSize(700,370), wmList::STYLE_SELECT, 2, wxSize(-1,40), 4, wxSize(5,5));
    m_plstHat->SetBackgroundColour(wxColour(0,0,0));
    m_plstHat->SetSelectedButtonColour(wxColour(wxT("#008000")));
    m_plstHat->SetDisabledColour(wxColour(wxT("#808080")));

    pnlAdvanced= new wxPanel(m_pswp, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL, _T("ID_PANEL3"));
    pnlAdvanced->SetBackgroundColour(wxColour(0,0,0));
    m_pbtnRotate = new wmButton(pnlAdvanced, wxID_ANY, _("Screen"), wxPoint(10,35), wxSize(200,40), wmButton::STYLE_SELECT, wxDefaultValidator, _T("ID_M_PBTN22"));
    m_pbtnRotate->SetToggle(true, wxT("Normal"), wxT("Rotate"), 40);

    m_pswp->AddPage(pnlHostname, _("Hostname"), true);
    m_pswp->AddPage(pnlPassword, _("Password"), false);
    m_pswp->AddPage(pnlAudio, _("HATs"), false);
    m_pswp->AddPage(pnlAdvanced, _("Advanced"), false);

    m_pbtnManage = new wmButton(this, ID_M_PBTN7, _("Finished"), wxPoint(705,425), wxSize(90,40), 0, wxDefaultValidator, _T("ID_M_PBTN7"));
    m_pbtnManage->SetBackgroundColour(wxColour(0,147,147));
    m_pbtnManage->SetColourDisabled(wxColour(wxT("#808080")));

    Connect(ID_M_PEDT1,wxEVT_COMMAND_TEXT_ENTER,(wxObjectEventFunction)&InitialSetupDialog::OnedtNameTextEnter);
    Connect(ID_M_PEDT2,wxEVT_COMMAND_TEXT_ENTER,(wxObjectEventFunction)&InitialSetupDialog::OnedtPasswordTextEnter);
    Connect(ID_M_PLST1,wxEVT_LIST_SELECTED,(wxObjectEventFunction)&InitialSetupDialog::OnlstHatSelected);
    Connect(ID_M_PSWP1,wxEVT_COMMAND_NOTEBOOK_PAGE_CHANGED,(wxObjectEventFunction)&InitialSetupDialog::OnswpPageChanged);
    Connect(ID_M_PBTN7,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&InitialSetupDialog::OnbtnManageClick);
    //*)


    wxXmlDocument xmlDoc;

    if(xmlDoc.Load(wxString::Format("%s/audio_hats.xml", Settings::Get().GetDocumentDirectory().c_str())))
    {
        for(wxXmlNode* pManNode = xmlDoc.GetRoot()->GetChildren(); pManNode; pManNode = pManNode->GetNext())
        {
            for(wxXmlNode* pModelNode = pManNode->GetChildren(); pModelNode; pModelNode = pModelNode->GetNext())
            {
                size_t nIndex = m_plstHat->AddButton(wxString::Format("%s %s", pManNode->GetAttribute("name", "").c_str(), pModelNode->GetAttribute("name", "").c_str()));
                m_plstHat->SetButtonAuxillaryText(nIndex,  pModelNode->GetAttribute("dtoverlay", ""));
            }
        }
    }


    m_nLine = -1;
    wxTextFile boot("/boot/config.txt");
    if(boot.Open())
    {
        for(size_t i = 0; i < boot.GetLineCount(); i++)
        {
            if(boot[i].Left(STR_DTOVERALY.length()) == STR_DTOVERALY)
            {
                wxString sOverlay = boot[i].After('=');

                for(size_t nButton = 0; nButton < m_plstHat->GetItemCount(); nButton++)
                {
                    wxLogDebug("Aux=%s", m_plstHat->GetButtonAuxillaryText(nButton).c_str());
                    if(sOverlay == m_plstHat->GetButtonAuxillaryText(nButton))
                    {
                        m_plstHat->SelectButton(nButton);
                        m_nLine = i;
                        break;
                    }
                }
            }
        }
        boot.Close();
    }
    else
    {
        wxLogDebug("Unable to open /boot/config.txt");
    }

    wxTextFile cmd("/boot/cmdline.txt");
    if(cmd.Open() && cmd.GetLineCount() > 0)
    {
        auto asCommands = wxStringTokenize(cmd[0], " ");
        for(size_t i = 0; i < asCommands.GetCount(); ++i)
        {
            if(asCommands[i] == "video=DSI-1:800x480@60,rotate=180")
            {
                m_pbtnRotate->ToggleSelection(true);
            }
        }
    }
    else
    {
        wxLogDebug("Unable to open /boot/cmdline.txt");
    }

    m_pedtName->SetFocus();
}

InitialSetupDialog::~InitialSetupDialog()
{
    //(*Destroy(InitialSetupDialog)
    //*)
}

void InitialSetupDialog::OnQuit(wxCommandEvent& event)
{
    Close();
}

void InitialSetupDialog::OnAbout(wxCommandEvent& event)
{
    wxString msg = wxbuildinfo(long_f);
    wxMessageBox(msg, _("Welcome to..."));
}

void InitialSetupDialog::OnedtNameTextEnter(wxCommandEvent& event)
{
    if(m_pedtName->GetValue().empty() == false)
    {
        m_pswp->ChangeSelection("Password");
    }
}

void InitialSetupDialog::OnedtPasswordTextEnter(wxCommandEvent& event)
{
    if(m_pedtPassword->GetValue().empty() == false)
    {
        m_pswp->ChangeSelection("HATs");
    }
}

void InitialSetupDialog::OnbtnManageClick(wxCommandEvent& event)
{
    if(m_pedtName->GetValue().empty() == false && m_pedtPassword->GetValue().empty() == false)
    {
        wxString sCommand(wxString::Format("sudo dosetup %s %s %s %d %d", m_pedtName->GetValue().c_str(), m_pedtPassword->GetValue().c_str(), m_sOverlay.c_str(), m_nLine, m_pbtnRotate->IsChecked()));
        wxLogDebug(sCommand);
        long nResult = wxExecute(sCommand, wxEXEC_SYNC);
        wxLogDebug("Result %d", nResult);

        EndModal(wxID_OK);
    }



}

void InitialSetupDialog::OnswpPageChanged(wxNotebookEvent& event)
{
        switch(m_pswp->GetSelection())
        {
            case 0:
                    m_pedtName->SetFocus();
                    break;
            case 1:
                    m_pedtPassword->SetFocus();
                    break;
        }
}

void InitialSetupDialog::OnlstHatSelected(wxCommandEvent& event)
{
    m_sOverlay = m_plstHat->GetButtonAuxillaryText(event.GetInt());
}
