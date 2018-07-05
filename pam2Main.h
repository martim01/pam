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

class Audio;
class Playback;
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
        //*)

        //(*Identifiers(pam2Dialog)
        static const long ID_BITMAPBUTTON1;
        static const long ID_PANEL4;
        static const long ID_M_PSWP1;
        static const long ID_M_PLST1;
        static const long ID_M_PLST2;
        static const long ID_PANEL2;
        static const long ID_M_PSWP2;
        static const long ID_PANEL1;
        static const long ID_PANEL3;
        static const long ID_M_PSWP3;
        static const long ID_TIMER1;
        static const long ID_TIMER2;
        static const long ID_TIMER3;
        //*)

        //(*Declarations(pam2Dialog)
        wmList* m_plstScreens;
        wxPanel* pnlMain;
        wxBitmapButton* m_pbmpSplash;
        wxPanel* pnlLists;
        wmList* m_plstOptions;
        wxPanel* Panel1;
        wxTimer m_timerIpc;
        wxTimer m_timerFile;
        wmSwitcherPanel* m_pswpOptions;
        wxTimer timerStart;
        wmSwitcherPanel* m_pswpMain;
        wxPanel* pnlSplash;
        wmSwitcherPanel* m_pswpSplash;
        //*)

        void OnSettingChanged(SettingEvent& event);
        void OnRTPSession(wxCommandEvent& event);
        void OnRTPSessionClosed(wxCommandEvent& event);
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
        void OnAudioData(wxCommandEvent& event);
        void OnQoS(wxCommandEvent& event);

        void OnPluginsReload(wxCommandEvent& event);
        void InputSession(const session& aSession);

        void OnHelpClose(wxCommandEvent& event);
        void CreateAudioInputDevice();
        void CreateAudioOutputDevice(unsigned long nSampleRate=0, unsigned long nChannels=0);

        void PassDataToPanels(timedbuffer* pTimerBuffer);

        void InputChanged(const wxString& sKey);
        void OutputChanged(const wxString& sKey);
        void CheckPlayback(unsigned long nSampleRate, unsigned long nChannels);

        void MaximizeMonitor(bool bMax);

        void TellPluginsAboutOutputChannels();
        void OpenFileForReading();

        void PopulateThreadList();

        void ClearSession();

        void ReadSoundFile(unsigned int nSize);

        void InitGenerator(const wxString& sSequence);
        void InitGenerator();

        Audio* m_pAudio;
        Playback* m_pPlayback;

        SoundFile* m_pSoundfile;
        Generator* m_pGenerator;
        Generator* m_pSequenceGenerator;

        pnlSettings* m_ppnlSettings;
        pnlTests* m_ppnlTests;
        pnlHelp* m_ppnlHelp;
        pnlLog* m_ppnlLog;
        MonitorPluginBuilder* m_pSelectedMonitor;

        std::map<wxString, RtpThread*> m_mRtp;
        std::set<wxString> m_setRtpOrphan;
        wxString m_sCurrentRtp;
        wxString m_sCurrentSequence;

        session m_Session;

        unsigned int m_nMonitorSource;
        unsigned int m_nPlaybackSource;

        std::multimap<size_t, wxString> m_mmMonitorPlugins;
        size_t m_nCurrentMonitorPage;

        wxDateTime m_dtLastRead;

        PammClient* m_pClient;
        DECLARE_EVENT_TABLE()
};

#endif // PAM2MAIN_H
