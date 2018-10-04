#ifndef PNLSETTINGS_H
#define PNLSETTINGS_H

//(*Headers(pnlSettings)
#include <wx/notebook.h>
#include "wmbutton.h"
#include "wmkeyboard.h"
#include "wmlabel.h"
#include <wx/statbox.h>
#include "wmswitcherpanel.h"
#include <wx/panel.h>
#include "wmslider.h"
#include "wmedit.h"
#include "wmlist.h"
//*)
#include "pnlNetworkSetup.h"
#include "pnlRTP.h"
#include "pnlSettingsPlugins.h"
#include "pnlUpdate.h"
#include "pnlSettingsProfiles.h"

class pnlSettings: public wxPanel
{
	public:

		pnlSettings(wxWindow* parent,wxWindowID id=wxID_ANY,const wxPoint& pos=wxDefaultPosition,const wxSize& size=wxDefaultSize);
		virtual ~pnlSettings();

		//(*Declarations(pnlSettings)
		wmLabel* m_pLbl4;
		wmList* m_plstLatency;
		wxPanel* pnlGeneral;
		wmLabel* m_pLbl3;
		wmLabel* m_plblHostname;
		wxPanel* Panel5;
		wmLabel* m_pLbl7;
		wxPanel* pnlGenerator;
		wmEdit* m_pedtPin;
		wmButton* m_pbtn900;
		wmList* m_plstDevices;
		wmSwitcherPanel* m_pswpSettings;
		wmList* m_plstAogFiles;
		wmButton* m_pbtnPrevious;
		wmLabel* m_plblCurrentPIN;
		wxPanel* Panel4;
		wmLabel* m_plbldB;
		wmButton* m_pbtnAogHome;
		wmButton* m_pbtnOutput;
		wmLabel* m_pLbl1;
		wmButton* m_pbtnAogEnd;
		wmButton* m_pbtnCursor;
		pnlSettingsPlugins* m_ppnlPlugins;
		pnlNetworkSetup* pnlSettingsNetwork;
		wmButton* m_pbtnPlus1;
		wmButton* m_pbtnPin;
		wmButton* m_pbtnNext;
		wmButton* m_pbtn10000;
		wmList* m_plstPlayback;
		wmSwitcherPanel* m_pswpAog;
		wxStaticBox* StaticBox1;
		wmButton* m_pbtn1000;
		wmButton* m_pbtnNoise0dBu;
		wxPanel* Panel3;
		wmSlider* m_pNoiseAmplitude;
		wmButton* m_pbtnAogNext;
		wmLabel* m_pLbl5;
		wxStaticBox* StaticBox2;
		wmLabel* m_plblNoisedB;
		wmList* m_plstInput;
		wmList* m_plstAudioSources;
		wmButton* m_pbtnMinus10;
		wmSlider* m_pAmplitude;
		wmLabel* m_pLbl6;
		wmButton* m_ptbnOptions;
		wmButton* m_pbtn450;
		wmLabel* m_pLbl8;
		wmLabel* m_pLbl2;
		wmButton* m_pbtnAogPrev;
		wmButton* m_pbtnMinus1;
		wmSlider* m_pSlider;
		wxStaticBox* StaticBox3;
		wmList* m_plstShape;
		wmButton* m_pbtnHome;
		wxPanel* pnlOutput;
		wxPanel* Panel2;
		wmButton* m_pbtn5000;
		wmButton* m_pbtnPlus10;
		pnlRTP* pnlSettingsRTP;
		wmLabel* m_plblVersion;
		wmList* m_plstOutputRight;
		wxPanel* pnlInput;
		wmButton* m_pbtn0dbu;
		wmButton* m_pbtnEnd;
		wmList* m_plstOutputLeft;
		wmLabel* m_plblFrequency;
		wmList* m_plstColour;
		pnlSettingsProfiles* m_ppnlProfiles;
		wmKeyboard* m_pkbdPin;
		pnlUpdate* m_ppnlUpdate;
		wmButton* m_pbtnSequences;
		wmLabel* m_plblInput;
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
		static const long ID_M_PLST2;
		static const long ID_M_PBTN1;
		static const long ID_M_PBTN2;
		static const long ID_M_PBTN4;
		static const long ID_M_PBTN5;
		static const long ID_PANEL1;
		static const long ID_M_PLBL4;
		static const long ID_M_PBTN3;
		static const long ID_M_PLST6;
		static const long ID_M_PLST4;
		static const long ID_M_PLBL5;
		static const long ID_PANEL2;
		static const long ID_M_PLBL2;
		static const long ID_M_PLST8;
		static const long ID_M_PLBL6;
		static const long ID_M_PLBL8;
		static const long ID_M_PLST3;
		static const long ID_M_PLST10;
		static const long ID_M_PLBL10;
		static const long ID_M_PLBL11;
		static const long ID_PANEL11;
		static const long ID_M_PLST7;
		static const long ID_M_PBTN6;
		static const long ID_M_PBTN7;
		static const long ID_M_PBTN8;
		static const long ID_M_PBTN9;
		static const long ID_PANEL9;
		static const long ID_STATICBOX2;
		static const long ID_STATICBOX1;
		static const long ID_SLIDER;
		static const long ID_M_PLBL7;
		static const long ID_M_PBTN12;
		static const long ID_M_PBTN11;
		static const long ID_M_PBTN10;
		static const long ID_M_PBTN13;
		static const long ID_M_PBTN14;
		static const long ID_M_PBTN15;
		static const long ID_M_PBTN16;
		static const long ID_M_PBTN17;
		static const long ID_M_PBTN18;
		static const long ID_CUSTOM1;
		static const long ID_M_PLBL9;
		static const long ID_M_PBTN19;
		static const long ID_M_PLST9;
		static const long ID_PANEL10;
		static const long ID_M_PLST5;
		static const long ID_STATICBOX3;
		static const long ID_CUSTOM2;
		static const long ID_M_PLBL1;
		static const long ID_M_PBTN21;
		static const long ID_PANEL12;
		static const long ID_M_PSWP2;
		static const long ID_M_PBTN20;
		static const long ID_PANEL8;
		static const long ID_PANEL4;
		static const long ID_PANEL5;
		static const long ID_PANEL3;
		static const long ID_PANEL7;
		static const long ID_PANEL13;
		static const long ID_M_PBTN22;
		static const long ID_M_PBTN23;
		static const long ID_M_PBTN24;
		static const long ID_M_PEDT1;
		static const long ID_M_PKBD1;
		static const long ID_M_PLBL12;
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
		void OnbtnMeterShadingClick(wxCommandEvent& event);
		void OnlblLatencySelected(wxCommandEvent& event);
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
		//*)

        void RefreshInputs();
		void ShowSoundcardInputs();
		void ShowRTPDefined();
		void ShowFiles();
		void ShowSequences();
        void ShowSoundcardOutputs();

        void ShowPagingButtons();
        void ShowAogPagingButtons();

        void OnSliderMove(wxCommandEvent& event);
        void OnAmplitudeMove(wxCommandEvent& event);
        void OnNoiseAmplitudeMove(wxCommandEvent& event);

        void PopulateChannelList(wmList* pList, int nSelected);

		DECLARE_EVENT_TABLE()
};

#endif

