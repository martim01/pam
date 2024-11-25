/***************************************************************
 * Name:      InitialSetupMain.h
 * Purpose:   Defines Application Frame
 * Author:    Matthew Martin (martim01@outlook.com)
 * Created:   2021-02-22
 * Copyright: Matthew Martin (https://github.com/martim01)
 * License:
 **************************************************************/

#ifndef INITIALSETUPMAIN_H
#define INITIALSETUPMAIN_H

//(*Headers(InitialSetupDialog)
#include "wmbutton.h"
#include "wmedit.h"
#include "wmkeyboard.h"
#include "wmlabel.h"
#include "wmlist.h"
#include "wmswitcherpanel.h"
#include <wx/dialog.h>
#include <wx/notebook.h>
#include <wx/panel.h>
#include "wmbutton.h"
//*)

class InitialSetupDialog: public wxDialog
{
    public:

        InitialSetupDialog(wxWindow* parent,wxWindowID id = -1);
        virtual ~InitialSetupDialog();

    private:

        //(*Handlers(InitialSetupDialog)
        void OnQuit(wxCommandEvent& event);
        void OnAbout(wxCommandEvent& event);
        void OnedtNameTextEnter(wxCommandEvent& event);
        void OnedtPasswordTextEnter(wxCommandEvent& event);
        void OnbtnManageClick(wxCommandEvent& event);
        void OnswpPageChanged(wxNotebookEvent& event);
        void OnlstHatSelected(wxCommandEvent& event);
        //*)

        //(*Identifiers(InitialSetupDialog)
        static const long ID_M_PLBL37;
        static const long ID_M_PLBL13;
        static const long ID_M_PLBL1;
        static const long ID_M_PEDT1;
        static const long ID_M_PKBD1;
        static const long ID_PANEL1;
        static const long ID_M_PLBL2;
        static const long ID_M_PEDT2;
        static const long ID_M_PKBD2;
        static const long ID_PANEL2;
        static const long ID_M_PLBL3;
        static const long ID_M_PLST1;
        static const long ID_PANEL3;
        static const long ID_M_PSWP1;
        static const long ID_M_PBTN7;
        //*)

        //(*Declarations(InitialSetupDialog)
        wmButton* m_pbtnManage;
        wmEdit* m_pedtName;
        wmEdit* m_pedtPassword;
        wmKeyboard* m_pKbd2;
        wmKeyboard* m_pkdbHost;
        wmLabel* m_pLbl1;
        wmLabel* m_pLbl2;
        wmLabel* m_pLbl3;
        wmLabel* m_plblHostname;
        wmLabel* m_plblTitle;
        wmList* m_plstHat;
        wmSwitcherPanel* m_pswp;
        wxPanel* pnlAudio;
        wxPanel* pnlHostname;
        wxPanel* pnlPassword;
        //*)

        wxPanel* pnlAdvanced;
        wmButton* m_pbtnRotate;


        DECLARE_EVENT_TABLE()

        wxString m_sOverlay;
        int m_nLine;
        static const wxString STR_DTPARAM;
        static const wxString STR_DTOVERALY;
};

#endif // INITIALSETUPMAIN_H
