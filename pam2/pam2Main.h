/***************************************************************
 * Name:      pam2Main.h
 * Purpose:   Defines Application Frame
 * Author:    Matthew Martin (martim01@outlook.com)
 * Created:   2017-11-01
 * Copyright: Matthew Martin ()
 * License:
 **************************************************************/

#ifndef PAM2MAIN_H
#define PAM2MAIN_H

//(*Headers(pam2Dialog)
#include <wx/notebook.h>
#include "wmbutton.h"
#include "wmlabel.h"
#include "wmswitcherpanel.h"
#include <wx/panel.h>
#include <wx/bmpbuttn.h>
#include <wx/dialog.h>
#include <wx/timer.h>
#include "wmlist.h"
//*)

#include "wmswitcherpanel.h"
#include "wmlist.h"
#include "session.h"

struct timedbuffer;
class MonitorPluginBuilder;
class pnlSettings;
class RtpThread;
class SettingEvent;
class pnlTests;
class pnlLog;
class SoundFile;
class pnlHelp;
class MonitorEvent;
struct session;
class PammClient;
class Generator;
class AudioEvent;


class pam2Dialog: public wxDialog
{
    public:

        pam2Dialog(wxWindow* parent,wxWindowID id = -1);
        virtual ~pam2Dialog();

    private:

        //(*Handlers(pam2Dialog)
        void OnQuit(wxCommandEvent& event);
        void OnAbout(wxCommandEvent& event);
        void OnlstScreensSelected(wxCommandEvent& event);
        void OnplstOptionsSelected(wxCommandEvent& event);
        void OntimerStartTrigger(wxTimerEvent& event);
        void Onm_timerFileTrigger(wxTimerEvent& event);
        void OntimerIpcTrigger(wxTimerEvent& event);
        void OnClose(wxCloseEvent& event);
        void OnbmpSplashClick(wxCommandEvent& event);
        void OnbtnMonitorClick(wxCommandEvent& event);
        void OnswpScreensPageChanged(wxNotebookEvent& event);
        void OnswpMainPageChanged(wxNotebookEvent& event);
        void OnswpSplashPageChanged(wxNotebookEvent& event);
        //*)

        //(*Identifiers(pam2Dialog)
        static const long ID_BITMAPBUTTON1;
        static const long ID_PANEL4;
        static const long ID_M_PSWP1;
        static const long ID_M_PLST1;
        static const long ID_M_PLST3;
        static const long ID_M_PLBL3;
        static const long ID_M_PLBL1;
        static const long ID_M_PLBL4;
        static const long ID_M_PLBL2;
        static const long ID_PANEL7;
        static const long ID_M_PBTN1;
        static const long ID_PANEL5;
        static const long ID_M_PLST2;
        static const long ID_PANEL2;
        static const long ID_M_PSWP2;
        static const long ID_PANEL6;
        static const long ID_M_PSWP4;
        static const long ID_PANEL1;
        static const long ID_PANEL3;
        static const long ID_M_PSWP3;
        static const long ID_TIMER1;
        static const long ID_TIMER2;
        static const long ID_TIMER3;
        //*)

        //(*Declarations(pam2Dialog)
        wmList* m_plstScreens;
        wmList* m_plstInbuilt;
        wxPanel* pnlMain;
        wxBitmapButton* m_pbmpSplash;
        wxPanel* Panel4;
        wmLabel* m_plblNetwork;
        wmLabel* m_plblOutput;
        wxPanel* pnlLists;
        wmList* m_plstOptions;
        wxPanel* Panel1;
        wxPanel* Panel3;
        wmLabel* m_plblCpu;
        wxTimer m_timerIpc;
        wmSwitcherPanel* m_pswpScreens;
        wmButton* m_pbtnMonitor;
        wxTimer m_timerFile;
        wmSwitcherPanel* m_pswpOptions;
        wxTimer timerStart;
        wxPanel* Panel2;
        wmSwitcherPanel* m_pswpMain;
        wxPanel* pnlSplash;
        wmSwitcherPanel* m_pswpSplash;
        wmLabel* m_plblInput;
        //*)

        void OnSettingChanged(SettingEvent& event);
        void OnMonitorRequest(MonitorEvent& event);

        void LoadMonitorPanels();
        void ShowMonitorPanel(const wxString& sPanel);
        void ShowOptionsPanel(const wxString& sPanel);

        void ShowMonitorList();

        void ShowSettingsPanel();
        void ShowTestPanels();
        void ShowLogPanel();
        void ShowHelpPanel();

        void RemoveOldFiles();

        void OnMonitorMax(wxCommandEvent& event);
        void OnAudioData(AudioEvent& event);
        void OnSession(wxCommandEvent& event);
        void OnPlaybackChannels(wxCommandEvent& event);
        void OnQoS(wxCommandEvent& event);

        void OnPluginsReload(wxCommandEvent& event);
        void InputSession(const session& aSession);

        void OnHelpClose(wxCommandEvent& event);
        void CreateAudioDevice(unsigned long nOutputSampleRate);
        void CreateAudioInputDevice();


        void PassDataToPanels(timedbuffer* pTimerBuffer);

        void InputChanged(const wxString& sKey);
        void OutputChanged(const wxString& sKey);
        void CheckPlayback(unsigned long nSampleRate, unsigned long nChannels);

        void MaximizeMonitor(bool bMax);

        void TellPluginsAboutOutputChannels();



        pnlSettings* m_ppnlSettings;
        pnlTests* m_ppnlTests;
        pnlHelp* m_ppnlHelp;
        pnlLog* m_ppnlLog;
        MonitorPluginBuilder* m_pSelectedMonitor;
        std::set<MonitorPluginBuilder*> m_setAlwaysPassAudio;

        std::multimap<size_t, wxString> m_mmMonitorPlugins;
        size_t m_nCurrentMonitorPage;

        wxDateTime m_dtLastRead;

        DECLARE_EVENT_TABLE()
};

#endif // PAM2MAIN_H
