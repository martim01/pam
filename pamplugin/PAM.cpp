/*
 * This file is part of the Code::Blocks IDE and licensed under the GNU General Public License, version 3
 * http://www.gnu.org/licenses/gpl-3.0.html
 *
 * $Revision: 9543 $
 * $Id: pam.cpp 9543 2014-01-07 08:12:54Z mortenmacfly $
 * $HeadURL: http://svn.code.sf.net/p/codeblocks/code/branches/release-16.xx/src/plugins/pam/pam.cpp $
 */

#include "sdk.h"
#ifndef CB_PRECOMP
    #include <wx/fs_zip.h>
    #include <wx/intl.h>
    #include <wx/menu.h>
    #include <wx/string.h>
    #include <wx/utils.h>
    #include <wx/xrc/xmlres.h>
    #include "cbproject.h"
    #include "configmanager.h"
    #include "globals.h"
    #include "logmanager.h"
    #include "manager.h"
    #include "projectmanager.h"
#endif
#include <wx/filesys.h>
#include "pam.h"
#include "dlgWizard.h"
#include <wx/msgdlg.h>
// this auto-registers the plugin
namespace
{
    PluginRegistrant<pam> reg(_T("pam"));

    int idLaunch = wxNewId();
}

BEGIN_EVENT_TABLE(pam, cbPlugin)
	EVT_MENU(idLaunch, pam::OnLaunch)
END_EVENT_TABLE()

pam::pam()
{
    if(!Manager::LoadResource(_T("pam.zip")))
    {
        NotifyMissingFile(_T("pam.zip"));
    }
}

pam::~pam()
{
}

void pam::OnAttach()
{
    Manager::Get()->GetLogManager()->DebugLog(wxT("PAM OnAttach"));
	m_FileNewMenu = 0;
	cbPlugin::OnAttach();
}

void pam::OnRelease(bool appShutDown)
{
	if (m_FileNewMenu)
	{
		m_FileNewMenu->Delete(idLaunch);
		m_FileNewMenu = 0;
	}
	cbPlugin::OnRelease(appShutDown);
}

void pam::BuildMenu(wxMenuBar* menuBar)
{
	if (m_FileNewMenu)
	{
		m_FileNewMenu->Delete(idLaunch);
		m_FileNewMenu = 0;
	}

    const int pos = menuBar->FindMenu(_("&File"));
    if (pos != wxNOT_FOUND)
    {
        wxMenu* fm = menuBar->GetMenu(pos);
        int id = fm->FindItem(_("New"));
        wxMenuItem* mn = fm->FindItem(id);
        m_FileNewMenu = mn ? mn->GetSubMenu() : 0;
        if (m_FileNewMenu)
        {
			m_FileNewMenu->Insert(2, idLaunch, _("PAM Plugin..."));
        }
		else
			Manager::Get()->GetLogManager()->DebugLog(_T("Could not find File->New menu!"));
    }
    else
        Manager::Get()->GetLogManager()->DebugLog(_T("Could not find File menu!"));
}

void pam::OnLaunch(cb_unused wxCommandEvent& event)
{
    ProjectManager* prjMan = Manager::Get()->GetProjectManager();

    dlgWizard dlg(Manager::Get()->GetAppWindow());
    PlaceWindow(&dlg);
    if (dlg.ShowModal() == wxID_OK)
    {
        if(prjMan->LoadProject(dlg.GetNewProjectPath()) == NULL)
        {
            wxMessageBox(wxString::Format(wxT("Can't open %s"), dlg.GetNewProjectPath().c_str()));
        }
    }
}
