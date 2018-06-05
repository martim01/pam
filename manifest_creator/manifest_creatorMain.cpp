/***************************************************************
 * Name:      manifest_creatorMain.cpp
 * Purpose:   Code for Application Frame
 * Author:    Matthew Martin (matthew.martin@bbc.co.uk)
 * Created:   2018-06-05
 * Copyright: Matthew Martin ()
 * License:
 **************************************************************/

#include "manifest_creatorMain.h"
#include <wx/msgdlg.h>
#include <wx/dir.h>
#include <wx/textfile.h>
#include <wx/filename.h>

//(*InternalHeaders(manifest_creatorDialog)
#include <wx/intl.h>
#include <wx/string.h>
//*)

//helper functions
enum wxbuildinfoformat {
    short_f, long_f };

wxString wxbuildinfo(wxbuildinfoformat format)
{
    wxString wxbuild(wxVERSION_STRING);

    if (format == long_f )
    {
#if defined(__WXMSW__)
        wxbuild << _T("-Windows");
#elif defined(__UNIX__)
        wxbuild << _T("-Linux");
#endif

#if wxUSE_UNICODE
        wxbuild << _T("-Unicode build");
#else
        wxbuild << _T("-ANSI build");
#endif // wxUSE_UNICODE
    }

    return wxbuild;
}

//(*IdInit(manifest_creatorDialog)
const long manifest_creatorDialog::ID_LISTCTRL1 = wxNewId();
const long manifest_creatorDialog::ID_LISTCTRL2 = wxNewId();
//*)

using namespace std;

BEGIN_EVENT_TABLE(manifest_creatorDialog,wxDialog)
    //(*EventTable(manifest_creatorDialog)
    //*)
END_EVENT_TABLE()

manifest_creatorDialog::manifest_creatorDialog(wxWindow* parent,wxWindowID id)
{
    //(*Initialize(manifest_creatorDialog)
    wxStaticBoxSizer* StaticBoxSizer2;
    wxBoxSizer* BoxSizer1;
    wxStaticBoxSizer* StaticBoxSizer1;

    Create(parent, id, _("Manifest Creator"), wxDefaultPosition, wxDefaultSize, wxDEFAULT_DIALOG_STYLE|wxRESIZE_BORDER, _T("id"));
    BoxSizer1 = new wxBoxSizer(wxHORIZONTAL);
    StaticBoxSizer1 = new wxStaticBoxSizer(wxHORIZONTAL, this, _("Dependencies"));
    m_plstDependencies = new wxListCtrl(this, ID_LISTCTRL1, wxDefaultPosition, wxDefaultSize, wxLC_REPORT, wxDefaultValidator, _T("ID_LISTCTRL1"));
    StaticBoxSizer1->Add(m_plstDependencies, 1, wxALL|wxEXPAND, 2);
    BoxSizer1->Add(StaticBoxSizer1, 1, wxALL|wxEXPAND, 2);
    StaticBoxSizer2 = new wxStaticBoxSizer(wxHORIZONTAL, this, _("Change Log"));
    m_plstChangelog = new wxListCtrl(this, ID_LISTCTRL2, wxDefaultPosition, wxDefaultSize, wxLC_REPORT, wxDefaultValidator, _T("ID_LISTCTRL2"));
    StaticBoxSizer2->Add(m_plstChangelog, 1, wxALL|wxEXPAND, 2);
    BoxSizer1->Add(StaticBoxSizer2, 1, wxALL|wxEXPAND, 2);
    SetSizer(BoxSizer1);
    BoxSizer1->Fit(this);
    BoxSizer1->SetSizeHints(this);
    //*)
    m_plstDependencies->InsertColumn(0, wxT("Library"));
    m_plstDependencies->InsertColumn(1, wxT("Version"));

    m_plstChangelog->InsertColumn(0, wxT("Version"));
    m_plstChangelog->InsertColumn(2, wxT("Change"));




    for(list<wxString>::iterator itLib = m_lstDependencies.begin(); itLib != m_lstDependencies.end(); ++itLib)
    {
        int nIndex = m_plstDependencies->InsertItem(m_plstDependencies->GetItemCount(), (*itLib));
    }

    for(list<pair<wxString, wxString> >::iterator itChange = m_lstChangelog.begin(); itChange != m_lstChangelog.end(); ++itChange)
    {
        int nIndex = m_plstChangelog->InsertItem(m_plstChangelog->GetItemCount(), (*itChange).first);
        m_plstChangelog->SetItem(nIndex,1, (*itChange).second);
    }


}

manifest_creatorDialog::~manifest_creatorDialog()
{
    //(*Destroy(manifest_creatorDialog)
    //*)

}

void manifest_creatorDialog::OnQuit(wxCommandEvent& event)
{
    Close();
}

void manifest_creatorDialog::OnAbout(wxCommandEvent& event)
{
    wxString msg = wxbuildinfo(long_f);
    wxMessageBox(msg, _("Welcome to..."));
}



