#ifndef PNLSETTINGS_H
#define PNLSETTINGS_H

//(*Headers(pnlSettings)
#include <wx/notebook.h>
#include "wmbutton.h"
#include "wmlabel.h"
#include "wmswitcherpanel.h"
#include <wx/panel.h>
#include "wmlist.h"
//*)
#include "pnlNetworkSetup.h"
#include "pnlRTP.h"
#include "pnlSettingsPlugins.h"


class pnlSettings: public wxPanel
{
	public:

		pnlSettings(wxWindow* parent,wxWindowID id=wxID_ANY,const wxPoint& pos=wxDefaultPosition,const wxSize& size=wxDefaultSize);
		virtual ~pnlSettings();

		//(*Declarations(pnlSettings)
		wmLabel* m_pLbl4;
		wmList* m_plstLatency;
		wmLabel* m_pLbl3;
		wmList* m_plstDevices;
		wmSwitcherPanel* m_pswpSettings;
		wmButton* m_pbtnOutput;
		wmLabel* m_pLbl1;
		pnlSettingsPlugins* m_ppnlPlugins;
		pnlNetworkSetup* pnlSettingsNetwork;
		wmList* m_plstPlayback;
		wmList* m_plstBuffer;
		wmLabel* m_pLbl5;
		wmLabel* m_plblSettings;
		wmList* m_plstInput;
		wxPanel* pnlThreads;
		wxPanel* pnlOutput;
		pnlRTP* pnlSettingsRTP;
		wmList* m_plstThreads;
		wxPanel* pnlInput;
		//*)

		void ReloadRTP();

	protected:

		//(*Identifiers(pnlSettings)
		static const long ID_M_PLBL37;
		static const long ID_M_PLBL3;
		static const long ID_M_PLST1;
		static const long ID_M_PLST2;
		static const long ID_PANEL1;
		static const long ID_M_PLBL4;
		static const long ID_M_PLBL1;
		static const long ID_M_PLST5;
		static const long ID_M_PBTN3;
		static const long ID_M_PLST6;
		static const long ID_M_PLST4;
		static const long ID_M_PLBL5;
		static const long ID_PANEL2;
		static const long ID_PANEL4;
		static const long ID_PANEL5;
		static const long ID_PANEL3;
		static const long ID_M_PLST3;
		static const long ID_PANEL6;
		static const long ID_M_PSWP1;
		//*)

	private:

		//(*Handlers(pnlSettings)
		void OnlstDevicesSelected(wxCommandEvent& event);
		void OnlstMeters_M36Selected(wxCommandEvent& event);
		void OnlstMeters_SpeedSelected(wxCommandEvent& event);
		void OnBtnExit(wxCommandEvent& event);
		void OnbtnInputClick(wxCommandEvent& event);
		void OnbtnAddSourceClick(wxCommandEvent& event);
		void OnbtnOutputClick(wxCommandEvent& event);
		void OnlstPlaybackSelected(wxCommandEvent& event);
		void OnbtnDeleteSourceClick(wxCommandEvent& event);
		void OnbtnDeleteSourceHeld(wxCommandEvent& event);
		void OnlstBufferSelected(wxCommandEvent& event);
		void OnbtnMeterShadingClick(wxCommandEvent& event);
		void OnlblLatencySelected(wxCommandEvent& event);
		void OnlstInputSelected(wxCommandEvent& event);
		void OnswpSettingsPageChanged(wxNotebookEvent& event);
		//*)

        void RefreshInputs();
		void ShowSoundcardInputs();
		void ShowRTPDefined();
		void ShowFiles();
        void ShowSoundcardOutputs();

		DECLARE_EVENT_TABLE()
};

#endif

