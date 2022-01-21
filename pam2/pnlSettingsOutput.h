#ifndef PNLSETTINGSOUTPUT_H
#define PNLSETTINGSOUTPUT_H

//(*Headers(pnlSettingsOutput)
#include "wmbutton.h"
#include "wmedit.h"
#include "wmkeyboard.h"
#include "wmlabel.h"
#include "wmlist.h"
#include "wmslider.h"
#include "wmswitcherpanel.h"
#include <wx/notebook.h>
#include <wx/panel.h>
//*)

#include "wmipeditpnl.h"

class SettingEvent;

class pnlSettingsOutput: public wxPanel
{
	public:

		pnlSettingsOutput(wxWindow* parent,wxWindowID id=wxID_ANY,const wxPoint& pos=wxDefaultPosition,const wxSize& size=wxDefaultSize, long nStyle=0, wxString sNotused=wxEmptyString);
		virtual ~pnlSettingsOutput();

		void ShowSoundcardOutputs();

		//(*Declarations(pnlSettingsOutput)
		wmButton* m_pbtnBits;
		wmButton* m_pbtnChannels;
		wmButton* m_pbtnDNS;
		wmButton* m_pbtnPacketTime;
		wmButton* m_pbtnRTCP;
		wmButton* m_pbtnRTSP;
		wmButton* m_pbtnRestartStream;
		wmButton* m_pbtnRtpMap;
		wmButton* m_pbtnSAP;
		wmButton* m_pbtnSampleRate;
		wmButton* m_pbtnStats;
		wmButton* m_pbtnStream;
		wmEdit* m_pedtRTPPort;
		wmEdit* m_pedtRTSPPort;
		wmKeyboard* m_pkbd;
		wmLabel* m_pLbl10;
		wmLabel* m_pLbl11;
		wmLabel* m_pLbl12;
		wmLabel* m_pLbl13;
		wmLabel* m_pLbl1;
		wmLabel* m_pLbl2;
		wmLabel* m_pLbl3;
		wmLabel* m_pLbl4;
		wmLabel* m_pLbl5;
		wmLabel* m_pLbl6;
		wmLabel* m_pLbl7;
		wmLabel* m_pLbl8;
		wmLabel* m_pLbl9;
		wmLabel* m_plblOutputGain;
		wmLabel* m_plblStreamTime;
		wmList* m_plstDestination;
		wmList* m_plstLatency;
		wmList* m_plstPlayback;
		wmSlider* m_plsliderOutputGain;
		wmSwitcherPanel* m_pswpDestination;
		wmipeditpnl* m_ppnlAddress;
		wxPanel* pnlAoip;
		wxPanel* pnlDisabled;
		wxPanel* pnlSoundcard;
		//*)

	protected:

		//(*Identifiers(pnlSettingsOutput)
		static const long ID_M_PLBL4;
		static const long ID_M_PLST3;
		static const long ID_PANEL9;
		static const long ID_M_PLST4;
		static const long ID_M_PLBL5;
		static const long ID_M_PLST6;
		static const long ID_M_PLBL3;
		static const long ID_M_PSLIDER1;
		static const long ID_M_PLBL10;
		static const long ID_PANEL10;
		static const long ID_M_PLBL1;
		static const long ID_PANEL12;
		static const long ID_M_PLBL2;
		static const long ID_M_PLBL8;
		static const long ID_M_PBTN6;
		static const long ID_M_PLBL6;
		static const long ID_M_PEDT3;
		static const long ID_M_PLBL7;
		static const long ID_M_PBTN7;
		static const long ID_M_PEDT2;
		static const long ID_M_PBTN12;
		static const long ID_M_PBTN1;
		static const long ID_M_PBTN2;
		static const long ID_M_PLBL14;
		static const long ID_M_PLBL15;
		static const long ID_M_PKBD2;
		static const long ID_M_PLBL9;
		static const long ID_M_PBTN8;
		static const long ID_M_PBTN9;
		static const long ID_M_PBTN3;
		static const long ID_M_PLBL12;
		static const long ID_M_PBTN5;
		static const long ID_M_PLBL13;
		static const long ID_M_PBTN10;
		static const long ID_M_PBTN11;
		static const long ID_M_PLBL11;
		static const long ID_M_PBTN4;
		static const long ID_PANEL11;
		static const long ID_M_PSWP2;
		//*)

	private:

		//(*Handlers(pnlSettingsOutput)
		void OnlstDestinationSelected(wxCommandEvent& event);
		void OnlstPlaybackSelected(wxCommandEvent& event);
		void OnlstLatencySelected(wxCommandEvent& event);
		void OnbtnRTSPClick(wxCommandEvent& event);
		void OnedtRTSPPortText(wxCommandEvent& event);
		void OnlstPacketSelected(wxCommandEvent& event);
		void OnedtRTPPortText(wxCommandEvent& event);
		void OnbtnDNSClick(wxCommandEvent& event);
		void OnbtnSAPClick(wxCommandEvent& event);
		void OnbtnStreamClick(wxCommandEvent& event);
		void OnlsliderOutputGainMove(wxCommandEvent& event);
		void OnbtnRestartStreamHeld(wxCommandEvent& event);
		void OnbtnStatsClick(wxCommandEvent& event);
		void OnbtnRtpMapClick(wxCommandEvent& event);
		void OnbtnChannelsClick(wxCommandEvent& event);
		void OnbtnPacketTimeClick(wxCommandEvent& event);
		void OnbtnSampleRateClick(wxCommandEvent& event);
		void OnbtnBitsClick(wxCommandEvent& event);
		void OnbtnRTCPClick(wxCommandEvent& event);
		//*)
		void OnSettingChanged(SettingEvent& event);

        double ConvertGainToRatio(double dGain);
        double ConvertRatioToGain(double dRatio);

		DECLARE_EVENT_TABLE()

		void UpdateDisplayedSettings();
};

#endif
