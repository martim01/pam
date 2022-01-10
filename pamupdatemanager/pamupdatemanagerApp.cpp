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
#include <wx/cmdline.h>

//(*AppHeaders
#include "pamupdatemanagerMain.h"
#include <wx/image.h>
//*)

IMPLEMENT_APP(pamupdatemanagerApp);

bool pamupdatemanagerApp::OnInit()
{
    if(!wxApp::OnInit())
		return false;

    Settings::Get().ReadSettings(wxString::Format(wxT("%s/pam/pam2.ini"), wxStandardPaths::Get().GetDocumentsDir().c_str()));

    bool wxsOK = true;
    wxInitAllImageHandlers();
    if ( wxsOK )
    {
    	pamupdatemanagerDialog Dlg(0, m_sDevice, m_fnUpdate);
    	SetTopWindow(&Dlg);
    	Dlg.ShowModal();
    	wxsOK = false;
    }
    return wxsOK;

}

void pamupdatemanagerApp::OnInitCmdLine(wxCmdLineParser& parser)
{
	parser.SetLogo("PAM Update Manager");
	parser.AddParam("Device");
	parser.AddParam("File");
}

bool pamupdatemanagerApp::OnCmdLineParsed(wxCmdLineParser& parser)
{
    m_sDevice = parser.GetParam(0);
    m_fnUpdate.Assign("/mnt/share/"+parser.GetParam(1));
	return true;
}
