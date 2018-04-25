/***************************************************************
 * Name:      pammMain.cpp
 * Purpose:   Code for Application Frame
 * Author:    Matthew Martin (martim01@outlook.com)
 * Created:   2017-11-01
 * Copyright: Matthew Martin ()
 * License:
 **************************************************************/

#include "pammMain.h"

//(*InternalHeaders(pammDialog)
#include <wx/font.h>
#include <wx/intl.h>
#include <wx/string.h>
//*)

#include "pamprocess.h"
#include <wx/stdpaths.h>

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

//(*IdInit(pammDialog)
const long pammDialog::ID_M_PLBL1 = wxNewId();
const long pammDialog::ID_M_PBTN1 = wxNewId();
const long pammDialog::ID_M_PBTN2 = wxNewId();
const long pammDialog::ID_M_PBTN3 = wxNewId();
const long pammDialog::ID_LISTBOX1 = wxNewId();
//*)

using   namespace std;
BEGIN_EVENT_TABLE(pammDialog,wxDialog)
    //(*EventTable(pammDialog)
    //*)
END_EVENT_TABLE()

pammDialog::pammDialog(wxWindow* parent,wxWindowID id) :
    m_pProcess(0)
{

    //(*Initialize(pammDialog)
    Create(parent, id, _("wxWidgets app"), wxDefaultPosition, wxDefaultSize, wxNO_BORDER, _T("id"));
    SetClientSize(wxSize(800,480));
    SetForegroundColour(wxColour(255,255,255));
    SetBackgroundColour(wxColour(176,176,176));
    wxFont thisFont(14,wxFONTFAMILY_SWISS,wxFONTSTYLE_NORMAL,wxFONTWEIGHT_NORMAL,false,_T("Arial"),wxFONTENCODING_DEFAULT);
    SetFont(thisFont);
    m_pLbl1 = new wmLabel(this, ID_M_PLBL1, _("Pi Audio Monitor Manager"), wxPoint(0,0), wxSize(800,40), 0, _T("ID_M_PLBL1"));
    m_pLbl1->SetBorderState(uiRect::BORDER_NONE);
    m_pLbl1->SetBackgroundColour(wxColour(15,83,210));
    m_pbtnLaunch = new wmButton(this, ID_M_PBTN1, _("Launch PAM"), wxPoint(50,360), wxSize(100,100), 0, wxDefaultValidator, _T("ID_M_PBTN1"));
    m_pbtnLaunch->SetBackgroundColour(wxColour(0,128,64));
    m_pbtnReboot = new wmButton(this, ID_M_PBTN2, _("Hold To Reboot"), wxPoint(450,360), wxSize(100,100), wmButton::STYLE_HOLD, wxDefaultValidator, _T("ID_M_PBTN2"));
    m_pbtnReboot->SetBackgroundColour(wxColour(128,0,0));
    m_pbtnShutdown = new wmButton(this, ID_M_PBTN3, _("Hold To Shutdown"), wxPoint(600,360), wxSize(100,100), wmButton::STYLE_HOLD, wxDefaultValidator, _T("ID_M_PBTN3"));
    m_pbtnShutdown->SetBackgroundColour(wxColour(128,0,0));
    m_plbxLog = new wxListBox(this, ID_LISTBOX1, wxPoint(10,50), wxSize(780,300), 0, 0, 0, wxDefaultValidator, _T("ID_LISTBOX1"));
    wxFont m_plbxLogFont(10,wxFONTFAMILY_SWISS,wxFONTSTYLE_NORMAL,wxFONTWEIGHT_NORMAL,false,_T("Arial"),wxFONTENCODING_DEFAULT);
    m_plbxLog->SetFont(m_plbxLogFont);

    Connect(ID_M_PBTN1,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&pammDialog::OnbtnLaunchClick);
    Connect(ID_M_PBTN2,wxEVT_BUTTON_HELD,(wxObjectEventFunction)&pammDialog::OnbtnRebootHeld);
    Connect(ID_M_PBTN3,wxEVT_BUTTON_HELD,(wxObjectEventFunction)&pammDialog::OnbtnShutdownHeld);
    //*)


    //LaunchPam();
    Connect(wxID_ANY, wxEVT_PROCESS_FINISHED, (wxObjectEventFunction)&pammDialog::OnPamClosed);

}

pammDialog::~pammDialog()
{
    //(*Destroy(pammDialog)
    //*)
}

void pammDialog::OnQuit(wxCommandEvent& event)
{
    Close();
}

void pammDialog::OnAbout(wxCommandEvent& event)
{
    wxString msg = wxbuildinfo(long_f);
}


void pammDialog::OnbtnLaunchClick(wxCommandEvent& event)
{
    LaunchPam();
}

void pammDialog::OnbtnRebootClick(wxCommandEvent& event)
{
}

void pammDialog::OnbtnShutdownClick(wxCommandEvent& event)
{
}


void pammDialog::OnPamClosed(wxCommandEvent& event)
{
    delete m_pProcess;
    m_pProcess = 0;
    if(event.GetExtraLong() != -1)
    {
        LaunchPam();
    }
    Log(wxT("Pam [#%d] closed. Exit code=%d"), event.GetInt(), event.GetExtraLong());
}

void pammDialog::LaunchPam()
{
    if(m_pProcess == 0)
    {
        m_pProcess = new PamProcess(this);
        #ifdef __WXDEBUG__
            #ifdef __WXMSW__
                wxString sPam = wxT("C:\\developer\\matt\\pam2\\bin\\Debug\\pam2");
            #else

              wxString sPam = wxT("/home/pi/pam/bin/Linux\ Release/pam2");
            #endif
        #else
            wxString sPam = wxT("/home/pi/pam/bin/Linux\ Release/pam2");
        #endif

        if ( !wxExecute(sPam, wxEXEC_ASYNC, m_pProcess) )
        {
            Log(wxString::Format(wxT("Failed to launch %s"), sPam.c_str()));
            delete m_pProcess;
            m_pProcess = 0;
        }
        else
        {
            Log(wxString::Format(wxT("Launched %s"), sPam.c_str()));
        }
    }
}

void pammDialog::Log(const wxChar *szString, ...)
{
    va_list argptr;
    va_start(argptr, szString);

    wxString sMessage;
    sMessage.PrintfV(szString, argptr);
    va_end(argptr);

    m_plbxLog->Append(wxString::Format(wxT("%s  %s"), wxDateTime::Now().Format(wxT("%Y-%m-%d %H:%M:%S")).c_str(), sMessage.c_str()));
}

void pammDialog::OnbtnRebootHeld(wxCommandEvent& event)
{
    #ifndef __WXMSW__
    wxExecute(wxT("sudo shutdown -r now"));
    #endif // __WXMSW__
}

void pammDialog::OnbtnShutdownHeld(wxCommandEvent& event)
{
    #ifndef __WXMSW__
    wxExecute(wxT("sudo shutdown now"));
    #endif // __WXMSW__
}

