/***************************************************************
 * Name:      pammApp.cpp
 * Purpose:   Code for Application Class
 * Author:    Matthew Martin (martim01@outlook.com)
 * Created:   2017-11-01
 * Copyright: Matthew Martin ()
 * License:
 **************************************************************/

#include "pammApp.h"

//(*AppHeaders
#include "pammMain.h"
#include <wx/image.h>
//*)

IMPLEMENT_APP(pammApp);

bool pammApp::OnInit()
{
    //(*AppInitialize
    bool wxsOK = true;
    wxInitAllImageHandlers();
    if ( wxsOK )
    {
    	pammDialog Dlg(0);
    	SetTopWindow(&Dlg);
    	Dlg.ShowModal();
    	wxsOK = false;
    }
    //*)
    return wxsOK;

}
