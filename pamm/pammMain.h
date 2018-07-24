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
#include <wx/timer.h>
//*)

class PamProcess;
class PamServer;

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
        void OnSecondTrigger(wxTimerEvent& event);
        void OnbtnKillHeld(wxCommandEvent& event);
        void OnbtnTerminalClick(wxCommandEvent& event);
        //*)

        void LaunchPam();

        void Log(const wxChar *szString, ...);

        void OnPamRestart(wxCommandEvent& event);

        //(*Identifiers(pammDialog)
        static const long ID_M_PLBL1;
        static const long ID_M_PBTN1;
        static const long ID_M_PBTN4;
        static const long ID_M_PBTN5;
        static const long ID_M_PBTN2;
        static const long ID_M_PBTN3;
        static const long ID_LISTBOX1;
        static const long ID_M_PLBL2;
        static const long ID_M_PLBL3;
        static const long ID_TIMER1;
        //*)

        //(*Declarations(pammDialog)
        wmButton* m_pbtnTerminal;
        wxListBox* m_plbxLog;
        wmButton* m_pbtnReboot;
        wmLabel* m_plblTitle;
        wmButton* m_pbtnShutdown;
        wmLabel* m_plblHeartbeat;
        wmLabel* m_pLbl2;
        wxTimer m_timerSecond;
        wmButton* m_pbtnKill;
        wmButton* m_pbtnLaunch;
        //*)

        void OnPamHB();

        PamProcess *m_pProcess;
        PamServer* m_pServer;

        bool m_bRestart;
        unsigned int m_nCrashRestarts;

        DECLARE_EVENT_TABLE()
};

#endif // pammMAIN_H
