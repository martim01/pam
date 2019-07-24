/***************************************************************
 * Name:      pamplugin3
 * Purpose:   Code::Blocks plugin
 * Author:    Matthew Martin (matthew.martin@bbc.co.uk)
 * Created:   2018-06-28
 * Copyright: Matthew Martin
 * License:   GPL
 **************************************************************/

#ifndef PAM_H_INCLUDED
#define PAM_H_INCLUDED


// For compilers that support precompilation, includes <wx/wx.h>
#include <wx/wxprec.h>

#ifndef WX_PRECOMP
    #include <wx/wx.h>
#endif

#include <cbplugin.h> // for "class cbWizardPlugin"

class pam : public cbPlugin
{
    public:
        /** Constructor. */
        pam();
        /** Destructor. */
        virtual ~pam();

        virtual void OnAttach();
        virtual void OnRelease(bool appShutDown);

        virtual void BuildMenu(wxMenuBar* menuBar);

		void OnLaunch(wxCommandEvent& event);

	protected:
	    void CreateProject();
	    void CreateFiles();

		wxMenu* m_FileNewMenu;

		DECLARE_EVENT_TABLE();
};

#endif // PAM_H_INCLUDED
