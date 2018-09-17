/***************************************************************
 * Name:      pamupdatemanagerApp.cpp
 * Purpose:   Code for Application Class
 * Author:    Matthew Martin (martim01@outlook.com)
 * Created:   2017-11-01
 * Copyright: Matthew Martin ()
 * License:
 **************************************************************/

#include "pamupdatemanagerApp.h"
#include "settings.h"
#include <wx/stdpaths.h>

//(*AppHeaders
#include "pamupdatemanagerMain.h"
#include <wx/image.h>
//*)

IMPLEMENT_APP(pamupdatemanagerApp);

bool pamupdatemanagerApp::OnInit()
{
    Settings::Get().ReadSettings(wxString::Format(wxT("%s/pam/pam2.ini"), wxStandardPaths::Get().GetDocumentsDir().c_str()));
    //(*AppInitialize
    bool wxsOK = true;
    wxInitAllImageHandlers();
    if ( wxsOK )
    {
    	pamupdatemanagerDialog Dlg(0);
    	SetTopWindow(&Dlg);
    	Dlg.ShowModal();
    	wxsOK = false;
    }
    //*)
    return wxsOK;

}
