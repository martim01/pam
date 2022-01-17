/***************************************************************
 * Name:      pamupdatemanagerApp.h
 * Purpose:   Defines Application Class
 * Author:    Matthew Martin (martim01@outlook.com)
 * Created:   2017-11-01
 * Copyright: Matthew Martin ()
 * License:
 **************************************************************/

#pragma once
#include <wx/app.h>
#include <wx/filename.h>
#include <wx/cmdline.h>

class pamupdatemanagerApp : public wxApp
{
    public:
        virtual bool OnInit();

        		/**	Initialise command line parser */
		void OnInitCmdLine(wxCmdLineParser& parser);

		/**	Handle command line parser result */
		bool OnCmdLineParsed(wxCmdLineParser& parser);

    private:
        wxString m_sDevice;
        wxFileName m_fnUpdate;
};


