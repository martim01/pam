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
#include "wxpamserver.h"
#include "wxpamconnection.h"
#include "version.h"
#include "settings.h"

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
const long pammDialog::ID_M_PBTN4 = wxNewId();
const long pammDialog::ID_M_PBTN5 = wxNewId();
const long pammDialog::ID_M_PBTN2 = wxNewId();
const long pammDialog::ID_M_PBTN3 = wxNewId();
const long pammDialog::ID_LISTBOX1 = wxNewId();
const long pammDialog::ID_M_PLBL2 = wxNewId();
const long pammDialog::ID_M_PLBL3 = wxNewId();
const long pammDialog::ID_TIMER1 = wxNewId();
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
    m_plblTitle = new wmLabel(this, ID_M_PLBL1, _("Pi Audio Monitor Manager"), wxPoint(0,0), wxSize(800,40), 0, _T("ID_M_PLBL1"));
    m_plblTitle->SetBorderState(uiRect::BORDER_NONE);
    m_plblTitle->SetBackgroundColour(wxColour(15,83,210));
    m_pbtnLaunch = new wmButton(this, ID_M_PBTN1, _("Launch PAM"), wxPoint(50,360), wxSize(100,100), 0, wxDefaultValidator, _T("ID_M_PBTN1"));
    m_pbtnLaunch->SetBackgroundColour(wxColour(0,128,64));
    m_pbtnLaunch->SetColourDisabled(wxColour(wxT("#A0A0A0")));
    m_pbtnKill = new wmButton(this, ID_M_PBTN4, _("Hold To Kill PAM"), wxPoint(235,410), wxSize(100,50), wmButton::STYLE_HOLD, wxDefaultValidator, _T("ID_M_PBTN4"));
    m_pbtnKill->Disable();
    m_pbtnKill->SetForegroundColour(wxColour(0,0,0));
    m_pbtnKill->SetBackgroundColour(wxColour(255,0,0));
    m_pbtnKill->SetColourDisabled(wxColour(wxT("#A0A0A0")));
    m_pbtnTerminal = new wmButton(this, ID_M_PBTN5, _("Terminal Emulator"), wxPoint(450,360), wxSize(100,100), wmButton::STYLE_NORMAL, wxDefaultValidator, _T("ID_M_PBTN5"));
    m_pbtnTerminal->SetBackgroundColour(wxColour(64,128,128));
    m_pbtnReboot = new wmButton(this, ID_M_PBTN2, _("Hold To Reboot"), wxPoint(570,360), wxSize(100,100), wmButton::STYLE_HOLD, wxDefaultValidator, _T("ID_M_PBTN2"));
    m_pbtnReboot->SetBackgroundColour(wxColour(128,0,0));
    m_pbtnShutdown = new wmButton(this, ID_M_PBTN3, _("Hold To Shutdown"), wxPoint(690,360), wxSize(100,100), wmButton::STYLE_HOLD, wxDefaultValidator, _T("ID_M_PBTN3"));
    m_pbtnShutdown->SetBackgroundColour(wxColour(128,0,0));
    m_plbxLog = new wxListBox(this, ID_LISTBOX1, wxPoint(10,50), wxSize(780,300), 0, 0, 0, wxDefaultValidator, _T("ID_LISTBOX1"));
    wxFont m_plbxLogFont(10,wxFONTFAMILY_SWISS,wxFONTSTYLE_NORMAL,wxFONTWEIGHT_NORMAL,false,_T("Arial"),wxFONTENCODING_DEFAULT);
    m_plbxLog->SetFont(m_plbxLogFont);
    m_pLbl2 = new wmLabel(this, ID_M_PLBL2, _("Time Since Heartbeat"), wxPoint(160,360), wxSize(250,25), 0, _T("ID_M_PLBL2"));
    m_pLbl2->SetBorderState(uiRect::BORDER_NONE);
    m_plblHeartbeat = new wmLabel(this, ID_M_PLBL3, _("00:00:00"), wxPoint(160,386), wxSize(250,25), 0, _T("ID_M_PLBL3"));
    m_plblHeartbeat->SetBorderState(uiRect::BORDER_NONE);
    m_plblHeartbeat->SetForegroundColour(wxColour(0,223,223));
    m_timerSecond.SetOwner(this, ID_TIMER1);
    m_timerSecond.Start(1000, false);

    Connect(ID_M_PBTN1,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&pammDialog::OnbtnLaunchClick);
    Connect(ID_M_PBTN4,wxEVT_BUTTON_HELD,(wxObjectEventFunction)&pammDialog::OnbtnKillHeld);
    Connect(ID_M_PBTN5,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&pammDialog::OnbtnTerminalClick);
    Connect(ID_M_PBTN2,wxEVT_BUTTON_HELD,(wxObjectEventFunction)&pammDialog::OnbtnRebootHeld);
    Connect(ID_M_PBTN3,wxEVT_BUTTON_HELD,(wxObjectEventFunction)&pammDialog::OnbtnShutdownHeld);
    Connect(ID_TIMER1,wxEVT_TIMER,(wxObjectEventFunction)&pammDialog::OnSecondTrigger);
    //*)

    SetPosition(wxPoint(0,0));
    #ifdef __WXGNU__
    wxExecute(wxT("sudo route add -net 224.0.0.0 netmask 240.0.0.0 eth0"));
    #endif // __WXGNU__

    m_nCrashRestarts = 0;

    //m_pServer = 0;
    //m_pServer = new PamServer();
    //m_pServer->Create(wxT("/tmp/pamm.ipc"));
    Connect(wxID_ANY, wxEVT_PROCESS_FINISHED, (wxObjectEventFunction)&pammDialog::OnPamClosed);

    Connect(wxID_ANY, wxEVT_IPC_RESTART, (wxObjectEventFunction)&pammDialog::OnPamRestart);

    m_bRestart = false;

    m_plblTitle->SetLabel(wxString::Format(wxT("Pi Audio Monitor Manager: %d.%d.%d.%d"), AutoVersion::MAJOR, AutoVersion::MINOR, AutoVersion::BUILD,AutoVersion::REVISION));

    Log(wxString::Format(wxT("Executable Path: %s"), Settings::Get().GetExecutableDirectory().c_str()));
    Log(wxString::Format(wxT("Document Path: %s"), Settings::Get().GetDocumentDirectory().c_str()));
    Log(wxString::Format(wxT("Config Path: %s"), Settings::Get().GetConfigDirectory().c_str()));
    Log(wxString::Format(wxT("Monitor Plugins Path: %s"), Settings::Get().GetMonitorPluginDirectory().c_str()));
    Log(wxString::Format(wxT("Test Plugins Path: %s"), Settings::Get().GetTestPluginDirectory().c_str()));
}

pammDialog::~pammDialog()
{
    //(*Destroy(pammDialog)
    //*)
    //delete m_pServer;
    //wxRemove(wxT("/tmp/pamm.ipc"));
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
    if(m_nCrashRestarts > 0)
    {
        Log(wxT("Manual launch so reset crash count."));
        m_nCrashRestarts = 0;   //reset crash restarts
    }
    LaunchPam();
}



void pammDialog::OnPamClosed(wxCommandEvent& event)
{
    delete m_pProcess;
    m_pProcess = 0;
    m_pbtnLaunch->Enable();
    m_pbtnKill->Enable(false);

    Log(wxT("Pam [#%d] closed. Exit code=%d"), event.GetInt(), event.GetExtraLong());

    if(event.GetExtraLong() != -1 && event.GetExtraLong() != -15)
    {
        m_nCrashRestarts++;
        Log(wxT("Pam [#%d] closed. Crash count %d"), event.GetInt(), m_nCrashRestarts);
    }
    if((event.GetExtraLong() != -1 && m_nCrashRestarts < 10) || m_bRestart)
    {
        m_bRestart = false;
        LaunchPam();
    }

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

              wxString sPam = wxT("pam2");
            #endif
        #else
            wxString sPam = wxT("pam2");
        #endif

        if ( wxExecute(sPam, wxEXEC_ASYNC, m_pProcess) <= 0)
        {
            Log(wxString::Format(wxT("Failed to launch %s"), sPam.c_str()));
            delete m_pProcess;
            m_pProcess = 0;
        }
        else
        {
            m_pbtnLaunch->Enable(false);
            m_pbtnKill->Enable();
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

    if(m_plbxLog->GetCount() > 10000)
    {
        m_plbxLog->Delete(0);
    }
}

void pammDialog::OnbtnRebootHeld(wxCommandEvent& event)
{
    wxShutdown(wxSHUTDOWN_REBOOT);
}

void pammDialog::OnbtnShutdownHeld(wxCommandEvent& event)
{
    wxShutdown();
}


void pammDialog::OnSecondTrigger(wxTimerEvent& event)
{

}

void pammDialog::OnbtnKillHeld(wxCommandEvent& event)
{
    Log(wxT("User attempt to kill PAM"));
    wxKill(m_pProcess->GetPid(), wxSIGKILL);
}


void pammDialog::OnPamRestart(wxCommandEvent& event)
{
    Log(wxT("Pam wants to restart"));
    m_bRestart = true;
    wxKill(m_pProcess->GetPid());
}

void pammDialog::OnbtnTerminalClick(wxCommandEvent& event)
{
    #ifdef __WXGTK__
    wxExecute("xterm -fullscreen");
    #else
    wxExecute("cmd");
    #endif // __WXGTK__

}
