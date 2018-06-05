/***************************************************************
 * Name:      manifest_creatorMain.h
 * Purpose:   Defines Application Frame
 * Author:    Matthew Martin (matthew.martin@bbc.co.uk)
 * Created:   2018-06-05
 * Copyright: Matthew Martin ()
 * License:
 **************************************************************/

#ifndef MANIFEST_CREATORMAIN_H
#define MANIFEST_CREATORMAIN_H

//(*Headers(manifest_creatorDialog)
#include <wx/listctrl.h>
#include <wx/sizer.h>
#include <wx/dialog.h>
//*)

#include <wx/xml/xml.h>
#include <list>
#include <map>

class manifest_creatorDialog: public wxDialog
{
    public:

        manifest_creatorDialog(wxWindow* parent,wxWindowID id = -1);
        virtual ~manifest_creatorDialog();


    private:

        //(*Handlers(manifest_creatorDialog)
        void OnQuit(wxCommandEvent& event);
        void OnAbout(wxCommandEvent& event);
        //*)

        //(*Identifiers(manifest_creatorDialog)
        static const long ID_LISTCTRL1;
        static const long ID_LISTCTRL2;
        //*)

        //(*Declarations(manifest_creatorDialog)
        wxListCtrl* m_plstDependencies;
        wxListCtrl* m_plstChangelog;
        //*)




        DECLARE_EVENT_TABLE()
};

#endif // MANIFEST_CREATORMAIN_H
