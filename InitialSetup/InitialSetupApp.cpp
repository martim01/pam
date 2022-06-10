/***************************************************************
 * Name:      InitialSetupApp.cpp
 * Purpose:   Code for Application Class
 * Author:    Matthew Martin (martim01@outlook.com)
 * Created:   2021-02-22
 * Copyright: Matthew Martin (https://github.com/martim01)
 * License:
 **************************************************************/

#include "InitialSetupApp.h"
#include "settings.h"
#include "pam2_paths.h"

//(*AppHeaders
#include "InitialSetupMain.h"
#include <wx/image.h>
//*)

IMPLEMENT_APP(InitialSetupApp);

bool InitialSetupApp::OnInit()
{
    Settings::Get().ReadSettings(wxString::Format(wxT("%s/pam/pam2.ini"), getenv("HOME")));
    //(*AppInitialize
    bool wxsOK = true;
    wxInitAllImageHandlers();
    if ( wxsOK )
    {
    	InitialSetupDialog Dlg(0);
    	SetTopWindow(&Dlg);
    	Dlg.ShowModal();
    	wxsOK = false;
    }
    //*)
    return wxsOK;

}
