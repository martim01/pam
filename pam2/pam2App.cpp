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

IMPLEMENT_APP(pam2App);

bool pam2App::OnInit()
{
    #ifndef __WXDEBUG__
    wxLog::SetLogLevel(0);
    #endif
    SoundcardManager::Get().Initialize();

    //#ifdef __WXGNU__
    Settings::Get().ReadSettings(wxString::Format(wxT("%s/pam/pam2.ini"), wxStandardPaths::Get().GetDocumentsDir().c_str()));
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
