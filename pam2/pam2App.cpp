/***************************************************************
 * Name:      pam2App.cpp
 * Purpose:   Code for Application Class
 * Author:    Matthew Martin (martim01@outlook.com)
 * Created:   2017-11-01
 * Copyright: Matthew Martin ()
 * License:
 **************************************************************/

#include "pam2App.h"

//(*AppHeaders
#include "pam2Main.h"
#include <wx/image.h>
//*)
#include "soundcardmanager.h"
#include "settings.h"
#include <wx/stdpaths.h>
#include <wx/log.h>
#include <wx/textfile.h>
#include "iomanager.h"
#include "dlgEngineering.h"

IMPLEMENT_APP(pam2App);

bool pam2App::OnInit()
{
    m_bReset = false;
    #ifndef __WXDEBUG__
    wxLog::SetLogLevel(0);
    #endif
    #ifdef __WXGNU__
    wxExecute(wxT("sudo route add -net 224.0.0.0 netmask 240.0.0.0 eth0"));
    double dSeconds;
    wxTextFile uptime;
    if(uptime.Open("/proc/uptime"))
    {
        if(uptime.GetFirstLine().BeforeFirst(' ').ToDouble(&dSeconds))
        {
            if(dSeconds < 60)
            {
                dlgEngineering aDlg(NULL);
                aDlg.ShowModal();
            }
        }
    }
    #else
    dlgEngineering aDlg(NULL);
    aDlg.ShowModal();
    #endif



    //#ifdef __WXGNU__
    Settings::Get().ReadSettings(wxString::Format(wxT("%s/pam/pam2.ini"), wxStandardPaths::Get().GetDocumentsDir().c_str()));
    Settings::Get().RemoveKey(wxT("AoIP"), wxT("NMOS_IS-04"));
    Settings::Get().Write("Startup", "Starting",1);

    m_timerHold.SetOwner(this, wxNewId());
    Connect(m_timerHold.GetId(), wxEVT_TIMER, (wxObjectEventFunction)&pam2App::OnTimerHold);

    SoundcardManager::Get().Initialize();

    //#else
   // Settings::Get().ReadSettings(wxString::Format(wxT("%s/documents/pam2.ini"), wxStandardPaths::Get().GetExecutablePath().c_str()));
   // #endif
    //(*AppInitialize
    bool wxsOK = true;
    wxInitAllImageHandlers();
    if ( wxsOK )
    {
    	pam2Dialog Dlg(0);
    	SetTopWindow(&Dlg);
    	Dlg.ShowModal();
    	wxsOK = false;
    }
    //*)
    return wxsOK;

}

int pam2App::FilterEvent(wxEvent& event)
{

    if(event.GetEventType() == wxEVT_LEFT_DOWN)
    {
        if(!m_bReset)
    	{
    	    m_timerHold.Start(1000,true);
    	}
    }
    if(event.GetEventType() == wxEVT_LEFT_UP)
    {
        if(!m_bReset)
    	{
    	    m_timerHold.Stop();
    	}
    }

    return -1;
}

void pam2App::OnTimerHold(wxTimerEvent& event)
{
    if(!m_bReset)
    {
        m_sInput = Settings::Get().Read(wxT("Input"), wxT("Type"), wxT("Soundcard"));
        Settings::Get().Write(wxT("Input"), wxT("Reset"), true);
        Settings::Get().Write(wxT("Input"), wxT("Type"), wxT("Disabled"));

        m_timerHold.Start(4000,true);
        m_bReset = true;
    }
    else
    {
        Settings::Get().Write(wxT("Input"), wxT("Type"), m_sInput);
        Settings::Get().Write(wxT("Input"), wxT("Reset"), false);
        m_bReset = false;
    }

}
