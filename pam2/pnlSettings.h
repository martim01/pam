#ifndef PNLSETTINGS_H
#define PNLSETTINGS_H

//(*Headers(pnlSettings)
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
#include "pnlNetworkSetup.h"
#include "pnlRTP.h"
#include "pnlSettingsPlugins.h"
#include "pnlUpdate.h"
#include "pnlSettingsProfiles.h"
#include "pnlSettingsGenerators.h"
#include "pnlSettingsNmos.h"
#include "pnlSettingsOutput.h"
#include "pnlSettingsTime.h"
#include "pnlAoipManual.h"
#include "pnlSettingsInputNmos.h"
#include "pnlAbout.h"
class SettingEvent;

class pnlSettings: public wxPanel
{
	public:

		pnlSettings(wxWindow* parent,wxWindowID id=wxID_ANY,const wxPoint& pos=wxDefaultPosition,const wxSize& size=wxDefaultSize);
		virtual ~pnlSettings();

		void UpdateDisplayedSettings();

		//(*Declarations(pnlSettings)
		pnlAbout* m_ppnlAbout;
		pnlAoipManual* m_ppnlAoIPManual;
		pnlNetworkSetup* pnlSettingsNetwork;
		pnlSettingsGenerators* m_ppnlGenerators;
		pnlSettingsInputNmos* m_ppnlInputNmos;
		pnlSettingsNmos* m_ppnlNmos;
		pnlSettingsOutput* m_ppnlOutput;
		pnlSettingsPlugins* m_ppnlPlugins;
		pnlSettingsProfiles* m_ppnlProfiles;
		pnlSettingsTime* m_ppnlTime;
		pnlUpdate* m_ppnlUpdate;
		wmButton* m_pbtnCursor;
		wmButton* m_pbtnEnd;
		wmButton* m_pbtnHome;
		wmButton* m_pbtnManage;
		wmButton* m_pbtnNext;
		wmButton* m_pbtnPin;
		wmButton* m_pbtnPrevious;
		wmButton* m_ptbnOptions;
		wmEdit* m_pedtPin;
		wmKeyboard* m_pkbdPin;
		wmLabel* m_pLbl1;
		wmLabel* m_pLbl3;
		wmLabel* m_pLbl8;
		wmLabel* m_plblCurrentPIN;
		wmLabel* m_plblHostname;
		wmLabel* m_plblInputGain;
		wmLabel* m_plblVersion;
		wmList* m_plstDevices;
		wmList* m_plstInput;
		wmSlider* m_plsliderInputGain;
		wmSwitcherPanel* m_pswpInput;
		wmSwitcherPanel* m_pswpSettings;
		wxPanel* Panel1;
		wxPanel* pnlGeneral;
		wxPanel* pnlInput;
		//*)

		void ReloadRTP();

		void InputSessionChanged();

	protected:

		//(*Identifiers(pnlSettings)
		static const long ID_M_PLBL37;
		static const long ID_M_PLBL13;
		static const long ID_M_PLBL14;
		static const long ID_M_PLBL3;
		static const long ID_M_PLST1;
		static const long ID_M_PBTN1;
		static const long ID_M_PBTN2;
		static const long ID_M_PBTN4;
		static const long ID_M_PBTN5;
		static const long ID_M_PLBL1;
		static const long ID_M_PSLIDER1;
		static const long ID_PANEL10;
		static const long ID_PANEL11;
		static const long ID_PANEL12;
		static const long ID_M_PSWP2;
		static const long ID_M_PLST2;
		static const long ID_M_PBTN7;
		static const long ID_M_PLBL2;
		static const long ID_PANEL1;
		static const long ID_PANEL2;
		static const long ID_PANEL8;
		static const long ID_PANEL5;
		static const long ID_PANEL3;
		static const long ID_PANEL7;
		static const long ID_PANEL13;
		static const long ID_PANEL4;
		static const long ID_PANEL9;
		static const long ID_M_PBTN22;
		static const long ID_M_PBTN23;
		static const long ID_M_PBTN24;
		static const long ID_M_PEDT1;
		static const long ID_M_PKBD1;
		static const long ID_M_PLBL12;
		static const long ID_PANEL6;
		static const long ID_PANEL14;
		static const long ID_M_PSWP1;
		//*)

		void OnSettingChanged(SettingEvent& event);

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
		void OnbtnMeterShadingClick(wxCommandEvent& event);
		void OnlstInputSelected(wxCommandEvent& event);
		void OnswpSettingsPageChanged(wxNotebookEvent& event);
		void OnbtnHomeClick(wxCommandEvent& event);
		void OnbtnPreviousClick(wxCommandEvent& event);
		void OnbtnNextClick(wxCommandEvent& event);
		void OnbtnEndClick(wxCommandEvent& event);
		void OnlstAudioSourcesSelected(wxCommandEvent& event);
		void OnlstAogFilesSelected(wxCommandEvent& event);
		void OnbtnAogHomeClick(wxCommandEvent& event);
		void OnbtnAogPrevClick(wxCommandEvent& event);
		void OnbtnAogNextClick(wxCommandEvent& event);
		void OnbtnAogEndClick(wxCommandEvent& event);
		void OnbtnMinus1Click(wxCommandEvent& event);
		void OnbtnPlus1Click(wxCommandEvent& event);
		void OnbtnPlus10Click(wxCommandEvent& event);
		void OnbtnMinus10Click(wxCommandEvent& event);
		void OnlstShapeSelected(wxCommandEvent& event);
		void Onbtn450Click(wxCommandEvent& event);
		void Onbtn900Click(wxCommandEvent& event);
		void Onbtn1000Click(wxCommandEvent& event);
		void Onbtn5000Click(wxCommandEvent& event);
		void Onbtn10000Click(wxCommandEvent& event);
		void Onbtn0dbuClick(wxCommandEvent& event);
		void OnbtnSequencesClick(wxCommandEvent& event);
		void OnlstColourSelected(wxCommandEvent& event);
		void OnbtnNoise0dBuClick(wxCommandEvent& event);
		void OnbtnCursorClick(wxCommandEvent& event);
		void OnbtnOptionsClick(wxCommandEvent& event);
		void OnlstOutputLeftSelected(wxCommandEvent& event);
		void OnlstOutputRightSelected(wxCommandEvent& event);
		void OnbtnPinClick(wxCommandEvent& event);
		void OnedtPinTextEnter(wxCommandEvent& event);
		void OnlstDestinationSelected(wxCommandEvent& event);
		void OnbtnStreamClick(wxCommandEvent& event);
		void OnlstPacketSelected(wxCommandEvent& event);
		void OnedtRTSPPortText(wxCommandEvent& event);
		void OnedtRTPPortText(wxCommandEvent& event);
		void OnbtnRTSPClick(wxCommandEvent& event);
		void OnbtnManageClick(wxCommandEvent& event);
		void OnlstDevicesPaged(wxCommandEvent& event);
		void OnRTSPPort_UnicastText(wxCommandEvent& event);
		void OnRTPPort_UnicastText(wxCommandEvent& event);
		void OnbtnSAPClick(wxCommandEvent& event);
		void OnbtnDNSClick(wxCommandEvent& event);
		void OnlsliderInputGainMove(wxCommandEvent& event);
		//*)

        void RefreshInputs();
		void ShowSoundcardInputs();

		void ShowRTPDefined();


        void EnableInputButtons(int nMode);
        void ShowPagingButtons();


        void PopulateChannelList(wmList* pList, int nSelected);

        double ConvertGainToRatio(double dGain);
        double ConvertRatioToGain(double dRatio);

        void ShowGain(bool bShow);
        size_t m_nNmosButton;

		DECLARE_EVENT_TABLE()
};

#endif

