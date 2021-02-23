/***************************************************************
 * Name:      InitialSetupApp.cpp
 * Purpose:   Code for Application Class
 * Author:    Matthew Martin (martim01@outlook.com)
 * Created:   2021-02-22
 * Copyright: Matthew Martin (https://github.com/martim01)
 * License:
 **************************************************************/

#include "InitialSetupApp.h"

//(*AppHeaders
#include "InitialSetupMain.h"
#include <wx/image.h>
//*)

IMPLEMENT_APP(InitialSetupApp);

bool InitialSetupApp::OnInit()
{
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
