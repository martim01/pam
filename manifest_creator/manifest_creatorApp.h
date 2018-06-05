/***************************************************************
 * Name:      manifest_creatorApp.h
 * Purpose:   Defines Application Class
 * Author:    Matthew Martin (matthew.martin@bbc.co.uk)
 * Created:   2018-06-05
 * Copyright: Matthew Martin ()
 * License:
 **************************************************************/

#ifndef MANIFEST_CREATORAPP_H
#define MANIFEST_CREATORAPP_H

#include <list>
#include <wx/string.h>

class manifest_creatorApp
{
    public:
        manifest_creatorApp();


        void ReadProjectFile();
        void ReadChangeLogFile();
        void WriteManifest();
        wxString ReadVersionFile(wxString sFile);

    wxString m_sName;
    wxString m_sType;
    wxString m_sPath;
    wxString m_sVersion;
    std::list<wxString> m_lstDependencies;
    std::list<std::pair<wxString, wxString> >m_lstChangelog;
};

#endif // MANIFEST_CREATORAPP_H
