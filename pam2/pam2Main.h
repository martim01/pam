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
#include "wmbutton.h"
#include "wmlabel.h"
#include "wmlist.h"
#include "wmswitcherpanel.h"
#include <wx/bmpbuttn.h>
#include <wx/dialog.h>
#include <wx/notebook.h>
#include <wx/panel.h>
#include <wx/timer.h>
//*)

#include "wmswitcherpanel.h"
#include "wmlist.h"
#include "session.h"
#include <memory>
#include "pnlAoIPSelection.h"
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
class dlgNoInput;
class wxNmosEvent;
class FlowAudioRaw;
class Sender;
class Receiver;
class wxNmosClientCurlEvent;
class wxNmosClientNodeEvent;
class wxNmosClientDeviceEvent;
class wxNmosClientSourceEvent;
class wxNmosClientFlowEvent;
class wxNmosClientSenderEvent;
class wxNmosClientReceiverEvent;

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
        void OnbtnInputClick(wxCommandEvent& event);
        //*)

        //(*Identifiers(pam2Dialog)
        static const long ID_BITMAPBUTTON1;
        static const long ID_PANEL4;
        static const long ID_M_PSWP1;
        static const long ID_M_PLST1;
        static const long ID_M_PLST3;
        static const long ID_M_PBTN2;
        static const long ID_M_PLBL4;
        static const long ID_M_PBTN3;
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
        static const long ID_PANEL8;
        static const long ID_M_PSWP3;
        static const long ID_TIMER1;
        static const long ID_TIMER2;
        static const long ID_TIMER3;
        //*)

        //(*Declarations(pam2Dialog)
        pnlAoIPSelection* pnlAoip;
        wmButton* m_pbtnCPU;
        wmButton* m_pbtnInput;
        wmButton* m_pbtnMonitor;
        wmLabel* m_plblNetwork;
        wmLabel* m_plblOutput;
        wmList* m_plstInbuilt;
        wmList* m_plstOptions;
        wmList* m_plstScreens;
        wmSwitcherPanel* m_pswpMain;
        wmSwitcherPanel* m_pswpOptions;
        wmSwitcherPanel* m_pswpScreens;
        wmSwitcherPanel* m_pswpSplash;
        wxBitmapButton* m_pbmpSplash;
        wxPanel* Panel1;
        wxPanel* Panel2;
        wxPanel* Panel3;
        wxPanel* Panel4;
        wxPanel* pnlLists;
        wxPanel* pnlMain;
        wxPanel* pnlSplash;
        wxTimer m_timerFile;
        wxTimer m_timerIpc;
        wxTimer timerStart;
        //*)

        void SetupNmos();
        void StartNmos();
        void StopNmos();
        void StartNmosClient();
        void StopNmosClient();

        void OnSettingChanged(SettingEvent& event);
        void OnMonitorRequest(MonitorEvent& event);

        void LoadMonitorPanels();
        void ShowMonitorPanel(const wxString& sPanel);
        void ShowOptionsPanel(const wxString& sPanel);

        void ShowMonitorList();

        void ShowSettingsPanel();
        void ShowTestPanels();
        void ShowLogPanel(bool bTests=false);
        void ShowHelpPanel();

        void RemoveOldFiles();

        void OnMonitorMax(wxCommandEvent& event);
        void OnAudioData(AudioEvent& event);
        void OnInputFailed(wxCommandEvent& event);
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



        void OnTarget(wxNmosEvent& event);
        void OnPatchSender(wxNmosEvent& event);
        void OnPatchReceiver(wxNmosEvent& event);
        void OnSenderActivated(wxNmosEvent& event);
        void OnReceiverActivated(wxNmosEvent& event);

        void OnNmosSenderChanged(wxNmosClientSenderEvent& event);
        void OnNmosReceiverChanged(wxNmosClientReceiverEvent& event);
        void OnNmosFlowChanged(wxNmosClientFlowEvent& event);

        void OnNmosSubscribeRequest(wxNmosClientCurlEvent& event);




        pnlSettings* m_ppnlSettings;
        pnlTests* m_ppnlTests;
        pnlHelp* m_ppnlHelp;
        pnlLog* m_ppnlLog;
        dlgNoInput* m_pdlgNoInput;
        MonitorPluginBuilder* m_pSelectedMonitor;
        std::set<MonitorPluginBuilder*> m_setAlwaysPassAudio;

        std::multimap<size_t, wxString> m_mmMonitorPlugins;
        size_t m_nCurrentMonitorPage;

        wxDateTime m_dtLastRead;
        bool m_bInputFailed;

        #ifdef __NMOS__
        void ActivateSender(std::shared_ptr<Sender> pSender);
        void ActivateReceiver(std::shared_ptr<Receiver> pReceiver);
        std::shared_ptr<FlowAudioRaw> m_pFlow;
        std::shared_ptr<Sender> m_pSender;
        std::multimap<std::string, std::shared_ptr<Sender> > m_mmLonelySender;
        #endif // __NMOS__
        DECLARE_EVENT_TABLE()
};

#endif // PAM2MAIN_H
