/***************************************************************
 * Name:      pammMain.h
 * Purpose:   Defines Application Frame
 * Author:    Matthew Martin (martim01@outlook.com)
 * Created:   2017-11-01
 * Copyright: Matthew Martin ()
 * License:
 **************************************************************/

#ifndef PAMMMAIN_H
#define PAMMMAIN_H

//(*Headers(pammDialog)
#include "wmbutton.h"
#include <wx/listbox.h>
#include "wmlabel.h"
#include <wx/dialog.h>
//*)

class PamProcess;

class pammDialog: public wxDialog
{
    public:

        pammDialog(wxWindow* parent,wxWindowID id = -1);
        virtual ~pammDialog();

    private:

        void OnPamClosed(wxCommandEvent& event);

        //(*Handlers(pammDialog)
        void OnQuit(wxCommandEvent& event);
        void OnAbout(wxCommandEvent& event);
        void OnlstScreensSelected(wxCommandEvent& event);
        void OnplstOptionsSelected(wxCommandEvent& event);
        void OntimerStartTrigger(wxTimerEvent& event);
        void Onm_timerFileTrigger(wxTimerEvent& event);
        void OnbtnLaunchClick(wxCommandEvent& event);
        void OnbtnRebootClick(wxCommandEvent& event);
        void OnbtnShutdownClick(wxCommandEvent& event);
        void OnbtnRebootHeld(wxCommandEvent& event);
        void OnbtnShutdownHeld(wxCommandEvent& event);
        //*)

        void LaunchPam();

        void Log(const wxChar *szString, ...);

        //(*Identifiers(pammDialog)
        static const long ID_M_PLBL1;
        static const long ID_M_PBTN1;
        static const long ID_M_PBTN2;
        static const long ID_M_PBTN3;
        static const long ID_LISTBOX1;
        //*)

        //(*Declarations(pammDialog)
        wxListBox* m_plbxLog;
        wmButton* m_pbtnReboot;
        wmLabel* m_pLbl1;
        wmButton* m_pbtnShutdown;
        wmButton* m_pbtnLaunch;
        //*)

        PamProcess *m_pProcess;
        DECLARE_EVENT_TABLE()
};

#endif // pammMAIN_H
