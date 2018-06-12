/***************************************************************
 * Name:      pamupdateserverApp.cpp
 * Purpose:   Code for Application Class
 * Author:    Matthew Martin (matthew.martin@bbc.co.uk)
 * Created:   2018-06-11
 * Copyright: Matthew Martin ()
 * License:
 **************************************************************/

#include "pamupdateserverApp.h"

//(*AppHeaders
#include "pamupdateserverMain.h"
#include <wx/image.h>
//*)

IMPLEMENT_APP(pamupdateserverApp);

bool pamupdateserverApp::OnInit()
{
    //(*AppInitialize
    bool wxsOK = true;
    wxInitAllImageHandlers();
    if ( wxsOK )
    {
    	pamupdateserverDialog Dlg(0);
    	SetTopWindow(&Dlg);
    	Dlg.ShowModal();
    	wxsOK = false;
    }
    //*)
    return wxsOK;

}
